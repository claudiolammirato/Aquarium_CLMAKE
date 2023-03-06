#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>


//library for wifi Debug with putty
#include "wifi_func.h"

//library for DHT Sensor
#include "sensortemphum.h"

//DHT Variables
float external_temp;
float external_hum;
float internal_temp;

void setup() {
  
  //Initialize serial:
  TelnetStream.begin();

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "claudio", InternalStorage);
}

void loop() {
    
  // check for WiFi OTA updates
  ArduinoOTA.poll();
  //Telnet message
  static unsigned long next;
  if (millis() - next > 10000) {
    next = millis();
    external_temp = retrievetemp();
    external_hum = retrievehum();
    internal_temp = aquariumtemp();

    //print Wifi Status
    printwifistatus();
    //Print external temp and Hum
    printext(external_temp, external_hum,internal_temp);
    thingsb_aquarium_send(external_temp,external_hum,internal_temp);
  }
}
