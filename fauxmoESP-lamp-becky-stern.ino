#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include "ESPAsyncWebServer.h"
#include <ESPAsyncTCP.h>
#include <Hash.h>

#define WIFI_SSID "T3"
#define WIFI_PASS "diwana_746"
#define RELAY_PIN D8

fauxmoESP fauxmo;

void wifiSetup() {
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void tog(){
  float elapsedTime = 0;
  float oldTime = 0;
  Serial.println("Toggle");
  oldTime = millis();
  
  digitalWrite(RELAY_PIN, HIGH);
  while (elapsedTime < 700){
    elapsedTime = millis() - oldTime;
  }
  digitalWrite(RELAY_PIN, LOW);  
}

void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.println(device_name); 
  tog();
}

void setup() {
    Serial.begin(9600);
    pinMode(RELAY_PIN, OUTPUT);
    wifiSetup();

    fauxmo.addDevice("Computer");
    fauxmo.onMessage(callback);
}

void loop() {
  fauxmo.handle();
}
