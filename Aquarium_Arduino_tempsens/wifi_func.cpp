#include "wifi_func.h"

void printprova(float temperature,float humidity) {
  TelnetStream.print("La temperatura è: ");
  TelnetStream.print(temperature);
  TelnetStream.println("°C");
  TelnetStream.print("L'umidità è: ");
  TelnetStream.print(humidity);
  TelnetStream.println("%");

}
