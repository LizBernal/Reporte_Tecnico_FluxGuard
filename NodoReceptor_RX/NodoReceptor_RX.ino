// ======================================================
// FluxGuard - Nodo Receptor RX
// ======================================================
// Funciones:
// - Recepción LoRa
// - Procesamiento JSON
// - Comunicación ESP-NOW
// - Visualización OLED
// ======================================================
#include <RadioLib.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// --- CONFIGURACIÓN LILYGO ---
uint8_t macLilygo[] = {0x10, 0x06, 0x1C, 0xB2, 0x35, 0x58};

// --- PINES HELTEC V3 ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 21, 18, 17);
#define VEXT_PIN 36 
SX1262 radio = new Module(8, 14, 12, 13); 

void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("\r\n[ESP-NOW] Envío a Lilygo: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "ENTREGADO (ÉXITO)" : "FALLIDO");
}

void setup() {
  Serial.begin(115200);

  pinMode(VEXT_PIN, OUTPUT);
  digitalWrite(VEXT_PIN, LOW); 
  delay(100);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 30, "Iniciando Flux-Guard...");
  u8g2.sendBuffer();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // CAMBIO 1: Canal del WiFi a 7
  esp_wifi_set_channel(7, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {}; 
  memcpy(peerInfo.peer_addr, macLilygo, 6);
  
  // CAMBIO 2: Canal del Peer a 7
  peerInfo.channel = 7; 
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al añadir Lilygo");
  }

  SPI.begin(9, 11, 10, 8);
  int state = radio.begin(915.0);
  if (state == RADIOLIB_ERR_NONE) {
    radio.setSyncWord(0x12);
    radio.setSpreadingFactor(7);
    radio.setBandwidth(125.0);
    radio.setCodingRate(5);
    Serial.println("Heltec Lista (Canal 7) - Esperando...");
  }
}

void loop() {
  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    // CAMBIO 3: Asegurar canal 7 antes de disparar el envío
    esp_wifi_set_channel(7, WIFI_SECOND_CHAN_NONE);
    delay(10); 
    
    esp_now_send(macLilygo, (uint8_t *) str.c_str(), str.length());
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, str);

    if (!error) {
      float i = doc["i"], p = doc["p"], wh = doc["wh"];
      float tc = doc["t_c"], ta = doc["t_a"], dt = doc["dt"];
      float ha = doc["h"], st = doc["st"];
      float lat = doc["lat"], lng = doc["lng"];
      int rssi = radio.getRSSI();

      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_5x7_tf); 
      u8g2.setCursor(0, 8); u8g2.printf("P:%0.1fW I:%0.2fA E:%0.3fWh", p, i, wh);
      u8g2.setCursor(0, 18); u8g2.printf("Tc:%0.1f Ta:%0.1f DT:%0.1f", tc, ta, dt);
      u8g2.setCursor(0, 28); u8g2.printf("Hum:%0.0f%% ST:%0.1fC", ha, st);
      u8g2.setCursor(0, 38);
      if(lat != 0) u8g2.printf("GPS:%0.4f,%0.4f", lat, lng);
      else u8g2.print("GPS: Sin senal");
      u8g2.setCursor(0, 52); u8g2.printf("Signal: %d dBm", rssi);
      u8g2.setCursor(0, 64);
      if(dt > 20) u8g2.print("!!! ALERTA SOBRECARGA !!!");
      else u8g2.print("SISTEMA OK - FLUXGUARD");
      u8g2.sendBuffer();
      
      Serial.println("RX LoRa OK: " + str);
    }
  }
}