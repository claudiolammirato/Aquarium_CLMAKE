#ifndef SENSORTEMPHUM_H
#define SENSORTEMPHUM_H

#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>



float retrievetemp();
float retrievehum();

float aquariumtemp();


#endif
