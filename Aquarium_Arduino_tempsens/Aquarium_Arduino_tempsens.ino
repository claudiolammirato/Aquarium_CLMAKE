#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
//library for wifi Debug with putty
#include <TelnetStream.h>


#include "wifi_func.h"

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;      // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password

int status = WL_IDLE_STATUS;

void setup() {
  //Initialize serial:
  Serial.begin(9600);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);

  // you're connected now, so print out the status:
  printWifiStatus();
  //Initialize Telnet
  TelnetStream.begin();

}

void loop() {
  // check for WiFi OTA updates
  ArduinoOTA.poll();
  //Telnet message
  static unsigned long next;
  if (millis() - next > 5000) {
    next = millis();
    log();
  }
}


void log() {
 TelnetStream.println("Claudio");
}
