#include <BlockDriver.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <MinimumSerial.h>
#include <SysCall.h>
#include <sdios.h>
#include <FreeStack.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include "credentials.h"

fauxmoESP fauxmo;

#define Relay D0
#define ID_Relay "Computer"

void wifiSetup() {
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void tog(){
  float elapsedTime = 0;
  float oldTime = 0;
  Serial.println("Toggle");
  oldTime = millis();
  
  digitalWrite(Relay, LOW);
  while (elapsedTime < 700){
    elapsedTime = millis() - oldTime;
  }
  digitalWrite(Relay, HIGH);  
}

void setup() {
    Serial.begin(9600);
    Serial.println();

    pinMode(Relay, OUTPUT);
    digitalWrite(Relay, HIGH);

    wifiSetup();

    fauxmo.createServer(true);
    fauxmo.setPort(80);

    fauxmo.enable(true);

    fauxmo.addDevice(ID_Relay);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        if (strcmp(device_name, ID_Relay)==0) {
            if(state){
              tog();
            }
            else{
              tog();
            }          
        }
    });
}

void loop() {
    fauxmo.handle();
}
