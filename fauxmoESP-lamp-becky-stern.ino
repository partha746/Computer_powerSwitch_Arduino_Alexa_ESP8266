#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include "ESPAsyncWebServer.h"
#include <ESPAsyncTCP.h>
#include <Hash.h>

#define WIFI_SSID "T3"
#define WIFI_PASS "*****"
#define RELAY_PIN D8
#define FlagPin D7

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

void tog(int flag){
  float elapsedTime = 0;
  float oldTime = 0;
  Serial.println("Toggle");
  oldTime = millis();
  
  digitalWrite(RELAY_PIN, HIGH);
  while (elapsedTime < 700){
    elapsedTime = millis() - oldTime;
  }
  digitalWrite(RELAY_PIN, LOW);  
  if (flag == 1){
    digitalWrite(FlagPin, HIGH);
  }
  else{
    digitalWrite(FlagPin, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  wifiSetup();

  fauxmo.enable(true);
  fauxmo.addDevice("Computer");
  fauxmo.onSetState(callbackSetState);
  fauxmo.onGetState(callbackGetState);
}

void loop() {
  fauxmo.handle();
}

void callbackSetState(unsigned char device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.println(device_name); 
  if (state) {
    tog(1);  
  }
  else{
    tog(0);
  }
}

bool callbackGetState(unsigned char device_id, const char * device_name)
{
  return digitalRead(FlagPin);
}
