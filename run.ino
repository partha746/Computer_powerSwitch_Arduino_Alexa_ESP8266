#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "fauxmoESP.h"
#include <BlynkSimpleEsp8266.h>
#include "wifi_passphrares.h"

fauxmoESP fauxmo;

#define Relay D0
#define ID_Relay "Computer"

void OTA(){
  ArduinoOTA.setHostname("Computer_trigger");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void wifiSetup() {
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", SSID1);
    WiFi.begin(SSID1, WifiPass);
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
  OTA();
  
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);

  wifiSetup();

  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice(ID_Relay);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
//      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      if (strcmp(device_name, ID_Relay)== 0) {
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
  ArduinoOTA.handle();    
  fauxmo.handle();
}
