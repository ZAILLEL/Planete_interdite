#pragma once 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "FREENOVE_COM.h"




void connectWiFi();
void connectMQTT() ;
void Mqttloop();

void MqttSendBatterie(int a,float valeur);
void MqttSendDistance(int a,float valeur);
void MqttSendObstacle(int a,int valeur);

void mqtt_setup();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);