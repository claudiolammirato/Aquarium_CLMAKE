#ifndef WIFI_FUNC_H
#define WIFI_FUNC_H

#include <Arduino.h>
#include <TelnetStream.h>
#include <ThingsBoard.h>
#include "arduino_secrets.h" 

void printext(float temp, float hum, float internal_temp);
void printwifistatus();
void thingsb_aquarium_send(float external_temp,float external_hum,float internal_temp);
void reconnect();
void resetcustom();
void webserver_aquarium(WiFiServer server);


#endif
