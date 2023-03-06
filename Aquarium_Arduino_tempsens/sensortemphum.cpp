#include "sensortemphum.h"

int DHTPIN = 2;
int DHTTYPE = 22;
DHT dht(DHTPIN, DHTTYPE);

//define ds18b20
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


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

float aquariumtemp(){
    sensors.begin();
    return sensors.getTempCByIndex(0);

}
