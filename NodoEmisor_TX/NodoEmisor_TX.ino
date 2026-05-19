// ======================================================
// FluxGuard - Nodo Emisor TX
// ======================================================
// Funciones:
// - Lectura de sensores
// - Envío LoRa
// - Generación JSON
// - Lectura GPS
// ======================================================
#include <RadioLib.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Adafruit_ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>

// Pantalla y Radio Pines V3
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 21, 18, 17);
#define VEXT_PIN 36 
SX1262 radio = new Module(8, 14, 12, 13); 

// Sensores y Variables
TwoWire I2C_EXT = TwoWire(1); 
Adafruit_ADS1115 ads;
TinyGPSPlus gps;
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);
DHT dht(5, DHT11);

float Wh = 0;
unsigned long t_ant = 0;
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  pinMode(VEXT_PIN, OUTPUT);
  digitalWrite(VEXT_PIN, LOW); delay(100);
  u8g2.begin();

  SPI.begin(9, 11, 10, 8);
  radio.begin(915.0);
  radio.setSyncWord(0x12);
  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  radio.setCodingRate(5);

  I2C_EXT.begin(41, 42, 100000);
  ads.begin(0x48, &I2C_EXT);
  ads.setGain(GAIN_FOUR);
  Serial2.begin(9600, SERIAL_8N1, 7, 6); 
  sensors.begin();
  dht.begin();
  t_ant = millis();
}

void loop() {
  while (Serial2.available() > 0) gps.encode(Serial2.read());

  unsigned long t_act = millis();
  int16_t raw = ads.readADC_Differential_0_1();
  float I = abs((raw * 0.03125 / 35.0) * 2.0);
  float P = 127.0 * I * 0.9;
  if(P > 0.5) Wh += P * ((t_act - t_ant) / 3600000.0);
  t_ant = t_act;

  if (millis() - ultimoEnvio >= 5000) {
    ultimoEnvio = millis();
    sensors.requestTemperatures();
    float tc = sensors.getTempCByIndex(0);
    float ta = dht.readTemperature();
    float ha = dht.readHumidity();
    float dt = tc - ta;
    float st = dht.computeHeatIndex(ta, ha, false);

    // --- PANTALLA EMISORA ---
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 10); u8g2.print("I: "); u8g2.print(I, 2); u8g2.print("A | P: "); u8g2.print(P, 1); u8g2.print("W");
    u8g2.setCursor(0, 21); u8g2.print("E: "); u8g2.print(Wh, 3); u8g2.print(" Wh");
    u8g2.setCursor(0, 32); u8g2.print("Tc: "); u8g2.print(tc, 1); u8g2.print(" | Ta: "); u8g2.print(ta, 1);
    u8g2.setCursor(0, 43); u8g2.print("H: "); u8g2.print(ha, 0); u8g2.print("% | ST: "); u8g2.print(st, 1);
    u8g2.setCursor(0, 54); 
    if(gps.location.isValid()) { u8g2.print("GPS: OK"); } else { u8g2.print("GPS: Buscando..."); }
    u8g2.setCursor(0, 64); u8g2.print(dt > 20 ? "!! SOBRECARGA !!" : "SISTEMA NORMAL");
    u8g2.sendBuffer();

    // --- ENVÍO JSON ---
    StaticJsonDocument<400> doc;
    doc["equipo"] = "FLUXGUARD";
    doc["i"] = I; doc["p"] = P; doc["t_c"] = tc; doc["t_a"] = ta;
    doc["h"] = ha; doc["st"] = st; doc["wh"] = Wh; doc["dt"] = dt;
    if (gps.location.isValid()) { doc["lat"] = gps.location.lat(); doc["lng"] = gps.location.lng(); }

    String output;
    serializeJson(doc, output);
    radio.transmit(output);
    Serial.println("TX: " + output);
  }
}
