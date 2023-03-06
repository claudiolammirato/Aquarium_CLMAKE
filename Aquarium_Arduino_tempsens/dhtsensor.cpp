#include "dhtSensor.h"

int DHTPIN = 2;
int DHTTYPE = 22;
DHT dht(DHTPIN, DHTTYPE);


float retrievetemp(){
    dht.begin();
    float temp= dht.readTemperature();
    delay(2000);
  return temp;
}

float retrievehum(){
    dht.begin();
    float hum= dht.readHumidity();
    delay(2000);
  return hum;
}
