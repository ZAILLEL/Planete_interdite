
#pragma once 
#include <SoftwareSerial.h>



struct freenove{
  float battery;
  float sonar;
};


float FNCReceive(char k);

void FNCSendChar(char c,char d);
void FNCSendInt(int i);

void setup_arduino_com();