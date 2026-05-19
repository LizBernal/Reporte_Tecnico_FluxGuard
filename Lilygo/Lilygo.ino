// ======================================================
// FluxGuard - Gateway LilyGO T-SIM7070G
// ======================================================
// Funciones:
// - Recepción ESP-NOW
// - Conexión GPRS/WiFi
// - Publicación MQTT
// - Envío de datos a la nube
// ======================================================
#define TINY_GSM_MODEM_SIM7070
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

// --- DATOS TELCEL ---
const char apn[]      = "internet.itelcel.com"; 
const char gprsUser[] = "";
const char gprsPass[] = "";

// --- DATOS WIFI ---
const char* wifi_ssid = "Mary";         
const char* wifi_pass = "xvfq8759";    

// --- CONFIG MQTT ---
const char* mqtt_server = "broker.hivemq.com";
const char* topic_pub   = "itsoeh/mary_gress/fluxguard/datos";

// --- PINES LILYGO T-SIM7070G ---
#define PIN_PWR 4
#define PIN_TX  27
#define PIN_RX  26
#define I2C_SDA 21
#define I2C_SCL 22

HardwareSerial modemSerial(1);
TinyGsm modem(modemSerial);
TinyGsmClient gsmClient(modem);
WiFiClient wifiClient;
PubSubClient mqtt;

String lastPayload = "";
bool newData = false;
bool useGPRS = false; 

// Callback para recibir datos del Heltec (LoRa) vía ESP-NOW
void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *incomingData, int len) {
  char message[251];
  int msgLen = min(len, 250);
  memcpy(message, incomingData, msgLen);
  message[msgLen] = '\0';
  lastPayload = String(message);
  newData = true; 
}

void setup() {
  Serial.begin(115200);
  
  // 1. FORZAR ENERGÍA DESDE EL CUBO (Configuración AXP/IP5306)
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(0x34); 
  Wire.write(0x12); 
  Wire.write(0x4D); // Activa canales de potencia para el módem
  Wire.endTransmission();

  // Encendido físico del SIM7070G
  pinMode(PIN_PWR, OUTPUT);
  digitalWrite(PIN_PWR, HIGH); 
  delay(1500); 
  digitalWrite(PIN_PWR, LOW);
  
  modemSerial.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(5000); 

  Serial.println("\n[SISTEMA] Iniciando Flux-Guard Gateway...");
  modem.sendAT("+CPSMS=0"); // Desactivar ahorro de energía para registro fuerte
  modem.waitResponse();

  // 2. PRIORIDAD TELCEL
  Serial.println("[MODEM] Buscando red Telcel...");
  int intentos = 0;
  while(intentos < 2 && !useGPRS) {
    if (modem.restart() && modem.gprsConnect(apn, gprsUser, gprsPass)) {
      Serial.println("[MODEM] CONECTADO A TELCEL.");
      mqtt.setClient(gsmClient);
      useGPRS = true;
    } else {
      intentos++;
      Serial.printf("[MODEM] Falló intento %d/2\n", intentos);
      delay(2000);
    }
  }

  // 3. RESPALDO WIFI
  if (!useGPRS) {
    Serial.println("[SISTEMA] Telcel falló. Usando WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_pass);
    
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) { 
      delay(500); Serial.print("."); 
    }
    
    if(WiFi.status() == WL_CONNECTED) {
      Serial.println("\n[WIFI] CONECTADO.");
      mqtt.setClient(wifiClient);
    }
  }

  mqtt.setServer(mqtt_server, 1883);
  mqtt.setBufferSize(512);

  // 4. ESP-NOW
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("[SISTEMA] Gateway listo. Esperando al Heltec...");
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    String clientID = "FluxGuard_Mary_" + String(random(0xffff), HEX);
    Serial.print("[MQTT] Conectando vía " + String(useGPRS ? "Telcel" : "WiFi") + "...");
    if (mqtt.connect(clientID.c_str())) {
      Serial.println(" OK!");
    } else {
      Serial.print(" Error rc="); Serial.print(mqtt.state());
      delay(5000);
    }
  }
}

void loop() {
  if (!mqtt.connected()) reconnectMQTT();
  mqtt.loop();

  if (newData) {
    Serial.println("\n>>> DATO RECIBIDO DEL HELTEC <<<");
    Serial.println("Payload: " + lastPayload);
    
    if (mqtt.publish(topic_pub, lastPayload.c_str())) {
      Serial.println("[ÉXITO] Publicado en HiveMQ.");
    } else {
      Serial.println("[ERROR] No se pudo publicar.");
    }
    newData = false; 
  }
}