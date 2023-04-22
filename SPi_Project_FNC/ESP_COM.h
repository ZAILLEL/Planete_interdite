#include <SoftwareSerial.h>
#include "Freenove_SPI_LIB.h"

void SendSonarEsp8266();
void SendBattaryVoltageEsp8266();

void setup_esp();

float Esp8266ReceiveFloat(char k);
int Esp8266ReceiveInt(char k);
char Esp8266ReceiveChar(char k);