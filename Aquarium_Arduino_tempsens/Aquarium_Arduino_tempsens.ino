#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include "wiring_private.h"
#include <iostream>
#include <string>
#include "wirelessXbee.h"



//library for wifi Debug with putty
#include "wifi_func.h"

//library for DHT Sensor
#include "sensortemphum.h"

//DHT Variables
float external_temp;
float external_hum;
float internal_temp;

//Arduino IOT 33 mySerial Initialization
Uart mySerial (&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);
//Specifi Function for mySerial
void SERCOM0_Handler()
{
    mySerial.IrqHandler();
}

//Adrress Low of Xbee tfor communication
std::string addresslow = "407C6019";




void setup() {
  
  // Reassign pins 5 and 6 to SERCOM alt
  pinPeripheral(5, PIO_SERCOM_ALT);
  pinPeripheral(6, PIO_SERCOM_ALT); 

  //Initialize serial:
  TelnetStream.begin();
  //Serial for Xbee
  mySerial.begin(9600);
  Serial.begin(9600);

  

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
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

    //Print external temp and Hum - wifi library and save it to ThingsBoard
    printext(external_temp, external_hum,internal_temp);
    thingsb_aquarium_send(external_temp,external_hum,internal_temp);

    std::string msg_string="Claudio_" + std::to_string(external_temp);
    std::string msg_hex;
    std::string msg_out;

    //Convert to HEX
    stream2hex(msg_string, msg_hex, true);
    writeXbee(msg_hex,msg_out, addresslow);
    //byte ans[50];
    unsigned char hexNumber[msg_out.length()/2];
    unsigned short temp;
  
    for (int i=0;i<msg_out.length();i+=2){
      std::stringstream my_ss;
      my_ss << msg_out.substr(i, 2).c_str();
      my_ss >> std::hex >> temp;
      hexNumber[i/2] = temp;
      
           
    }


    Serial.println(msg_out.substr(0, 2).c_str());
    Serial.println(msg_out.c_str());
    
    Serial.println("");

    for(int i=0;i<sizeof(hexNumber);i++){
      Serial.print(hexNumber[i], HEX);
    }
    Serial.println("");
    mySerial.write(hexNumber, sizeof(hexNumber));
    //byte xxx[] = {0x7E, 0x00, 0X15, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x7C, 0x60, 0x19, 0xFF, 0xFE, 0x00, 0x00, 0x43, 0x6C, 0x61, 0x75, 0x64, 0x69, 0x6F, 0x46};
    //mySerial.write(ans, sizeof(ans));
    
  }
}
