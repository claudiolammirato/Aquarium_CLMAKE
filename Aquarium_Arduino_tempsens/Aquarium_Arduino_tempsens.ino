#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include "wiring_private.h"
#include <iostream>
#include <string>
#include "wirelessXbee.h"


//Arduino IOT 33 mySerial Initialization
Uart mySerial (&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);

WiFiServer server(80); 


//Specifi Function for mySerial
void SERCOM0_Handler(){
    mySerial.IrqHandler();
}




//library for wifi Debug with putty
#include "wifi_func.h"

//library for DHT Sensor
#include "sensortemphum.h"

//DHT Variables
float external_temp;
float external_hum;
float internal_temp;



//Adrress Low of Xbee tfor communication
std::string addresslow = "407C6019";

//OPTIONS
//Send time
const int send_time = 10*1000;




void setup() {
  
  // Reassign pins 5 and 6 to SERCOM alt
  pinPeripheral(5, PIO_SERCOM_ALT);
  pinPeripheral(6, PIO_SERCOM_ALT); 

  //Initialize serial:
  TelnetStream.begin();
  //Serial for Xbee
  mySerial.begin(9600);

  Serial.begin(9600);
  server.begin();


  

  // start the WiFi OTA library with internal (flash) based storage
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
}

void loop() {
    
  // check for WiFi OTA updates
  ArduinoOTA.poll();
  //Telnet message
  static unsigned long next;
  if (millis() - next > send_time) {
    next = millis();
    external_temp = retrievetemp();
    external_hum = retrievehum();
    internal_temp = aquariumtemp();

    //print Wifi Status
    printwifistatus();

    //Print external temp and Hum - wifi library and save it to ThingsBoard
    printext(external_temp, external_hum,internal_temp);
    thingsb_aquarium_send(external_temp,external_hum,internal_temp);

    std::string msg_string_et= std::to_string(external_temp)+ "_exttemp";
    std::string msg_string_it= std::to_string(internal_temp)+ "_int_temp";
    std::string msg_string_eh= std::to_string(external_hum)+ "_exthum";
    std::string msg_hex_et;
    std::string msg_out_et;
    std::string msg_hex_it;
    std::string msg_out_it;
    std::string msg_hex_eh;
    std::string msg_out_eh;

    //Convert to HEX
    stream2hex(msg_string_et, msg_hex_et, true);
    //send to Xbee
    writeXbee(msg_hex_et,msg_out_et, addresslow, mySerial);
    delay(50);
    
    //Convert to HEX
    stream2hex(msg_string_it, msg_hex_it, true);
    //send to Xbee
    writeXbee(msg_hex_it,msg_out_it, addresslow, mySerial);
    delay(50);
    //Convert to HEX
    stream2hex(msg_string_eh, msg_hex_eh, true);
    //send to Xbee
    writeXbee(msg_hex_eh,msg_out_eh, addresslow, mySerial);
    delay(50);

    //webserver
    webserver_aquarium(server, external_temp);
    

  }

  //read xbee message
  readXbee(mySerial);
  

  delay(150);
}
