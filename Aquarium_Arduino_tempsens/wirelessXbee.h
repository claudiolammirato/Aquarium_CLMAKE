#ifndef wirelessXbee_h
#define wirelessXbee_h

#include <string.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <typeinfo>
#include <Arduino.h>



void writeXbee(std::string str, std::string& , std::string addresslow, Uart& mySerial);
void stream2hex(std::string str, std::string& hexstr, bool capital = false);
void hex2stream(std::string hexstr, std::string& str);
void readXbee(Uart& mySerial);

std::string dec2hex(long a);
long hex2dec(const std::string & hexstr);
std::string hexadd(const std::string & hex1, const std::string & hex2);
std::string hexmin(const std::string & hex1, const std::string & hex2);
std::string hexand(const std::string & hex1, const std::string & hex2);




#endif