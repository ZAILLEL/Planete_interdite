#include "nodered_mqtt.h"
#include "FREENOVE_COM.h"

 int test ;
 int icpt = 0;
unsigned long lastFNCRX = 0;
unsigned long lastMQTTTX = 0;
unsigned long lastMQTTRX = 0;
int obstacle_detected = 0;
struct freenove freenove;

void setup() {

  setup_arduino_com();
  mqtt_setup();
  
}

void loop() {

  icpt++;

  Mqttloop();
 // millis();

  //Serial.print(ArduinoReceive('s'));
  
   //if(icpt%50 == 1) MqttSendBatterie();


 // MqttSendDistance(FNCReceive('d'));
// Lire le premier octet disponible
if (Serial.available()) test = Serial.read() - '0';

while (Serial.available() > 0) {
  int discard = Serial.read();
}


}

void do_task()
{

  if (millis() - lastFNCRX >= 100) {
    FREENOVE_RX_MANAGER(); 
    lastFNCRX = millis();  
  }

  if (millis() -   lastMQTTTX >= 125) {
    MQTT_TX_MANAGER(); 
    lastMQTTTX = millis();  
  }

  if (millis() -   lastMQTTRX >= 150) {
    Mqttloop(); 
    lastMQTTRX = millis();  
  }




}

void FREENOVE_RX_MANAGER()
{
  freenove.battery = FNCReceive('_');
  freenove.sonar = FNCReceive('^');
}

void MQTT_TX_MANAGER()
{
    MqttSendBatterie(1,freenove.battery);
    MqttSendDistance(1,freenove.sonar);
    MqttSendObstacle(1,obstacle_detected);
}


