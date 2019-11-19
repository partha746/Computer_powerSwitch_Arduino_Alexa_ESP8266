#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include "fauxmoESP.h"
#include "ESPAsyncWebServer.h"
#include <ESPAsyncTCP.h>

#define RELAY_PIN D8
#define FlagPin D7

fauxmoESP fauxmo;
ESP8266WiFiMulti wifiMulti;

void wifiSetup() {
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("", "");
    wifiMulti.addAP("", "");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
    Serial.printf("Connected to SSID: %s & IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
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
    Serial.println("ON");
  }
  else{
    digitalWrite(FlagPin, LOW);
    Serial.println("OFF");
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
