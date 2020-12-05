  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <SPIFFS.h>
 #include <WiFiUdp.h>
 #include <ArduinoOTA.h>
 //#include "screens.h"
 extern void OTAStartScreen(bool a);
 extern void OTAEndScreen();
 extern void OTAProgressScreen(int progress);
 extern void OTAErrorScreen();
// Wi-Fi
const char*   SSID      = "Narnia2.4";
const char*   PASSWORD  = "feju1909";
const char*   hostname  = "Fuscazul";
const char*   hostpswd  = "fusc@";
// Setup ------------------------------------------------
void myOTA() {
  //WiFi
  OTAStartScreen(0);
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED) { }
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(hostpswd);
  OTAStartScreen(1);

  // Fim
  ArduinoOTA.onEnd([](){
    OTAEndScreen();delay(5000);
    esp_restart();
  });

  // Progresso
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
   progress = (progress * 100 / total);
   OTAProgressScreen(progress);
  });

  // Falha
  ArduinoOTA.onError([](ota_error_t error) {
  OTAErrorScreen();
  delay(5000);esp_restart();
  });

  // Inicializa OTA
  ArduinoOTA.begin();
  
}
