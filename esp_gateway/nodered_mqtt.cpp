#include "nodered_mqtt.h"


const char* ssid = "kzwifi";
const char* password = "15011999";

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

const char* mqtt_user = "kz";
const char* mqtt_password = "1234";

const char* batterie_topic1 = "topic/batteryvoltage1";
const char* ultrason_topic1 = "topic/distanceobstacle1";
const char* topic_obstacle1 = "topic/nbobstacle1";
/*
const char* batterie_topic2 = "topic/batteryvoltage2";
const char* ultrason_topic2 = "topic/distanceobstacle2";
const char* topic_obstacle2 = "topic/nbobstacle2";

const char* batterie_topic3 = "topic/batteryvoltage3";
const char* ultrason_topic3 = "topic/distanceobstacle3";
const char* topic_obstacle3 = "topic/nbobstacle3";*/

const char* bouton_haut_topic = "topic/buttonh";
const char* bouton_bas_topic = "topic/buttonb";
const char* bouton_gauche_topic = "topic/buttong";
const char* bouton_droite_topic = "topic/buttond";




const char* slider_topic = "topic/speed";

WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_setup() {
  // Initialisation du port série
  Serial.begin(115200);

  // Connexion au réseau Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au réseau Wi-Fi...");
  }

  // Connexion au serveur MQTT
  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connexion au serveur MQTT...");
    if (client.connect("ESP8266Client"/*,mqtt_user,mqtt_password*/)) {
      Serial.println("Connecté au serveur MQTT");
    } else {
      Serial.print("Erreur de connexion au serveur MQTT, code d'erreur : ");
      Serial.println(client.state());
      delay(1000);
    }
  }

  // Souscription aux topics
  client.subscribe(batterie_topic1);
  client.subscribe(ultrason_topic1);
  client.subscribe(topic_obstacle1);
/*
  client.subscribe(batterie_topic2);
  client.subscribe(ultrason_topic2);
  client.subscribe(topic_obstacle2);

  client.subscribe(batterie_topic3);
  client.subscribe(ultrason_topic3);
  client.subscribe(topic_obstacle3);*/




  client.subscribe(bouton_haut_topic);
  client.subscribe(bouton_bas_topic);
  client.subscribe(bouton_gauche_topic);
  client.subscribe(bouton_droite_topic);
  client.subscribe(slider_topic);
}

void Mqttloop() {
  // Vérification de la connexion au serveur MQTT
  
  if (!client.connected()) {
    reconnect();
  }

  // Lecture des messages entrants
  if(!client.loop())
    client.connect("MQTTClient");
}

void reconnect() {
  // Boucle jusqu'à ce que nous soyons reconnectés
  while (!client.connected()) {
  //  Serial.println("Tentative de reconnexion au serveur MQTT...");
    if (client.connect("ESP8266Client"/*, mqtt_user, mqtt_password*/)) {
  //    Serial.println("Reconnecté au serveur MQTT");
      client.subscribe(batterie_topic1);
      client.subscribe(ultrason_topic1);
      client.subscribe(topic_obstacle1);
/*
      client.subscribe(batterie_topic2);
      client.subscribe(ultrason_topic2);
      client.subscribe(topic_obstacle2);

      client.subscribe(batterie_topic3);
      client.subscribe(ultrason_topic3);
      client.subscribe(topic_obstacle3);*/



      client.subscribe(bouton_haut_topic);
      client.subscribe(bouton_bas_topic);
      client.subscribe(bouton_gauche_topic);
      client.subscribe(bouton_droite_topic);
      client.subscribe(slider_topic);
    } else {
      Serial.print("Erreur de reconnexion au serveur MQTT, code d'erreur : ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = "";
  
  // Conversion du tableau de bytes en chaîne de caractères
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  // Traitement du message en fonction du topic
  if (topicStr == bouton_haut_topic) {
    if (payloadStr == "1") {
      FNCSendChar('F','*');
      // Action lorsque le bouton Haut est appuyé
    } else {
      // Action lorsque le bouton Haut est relâché
    }
  } else if (topicStr == bouton_bas_topic) {
    if (payloadStr == "1") {
      FNCSendChar('H','*');
      // Action lorsque le bouton Bas est appuyé
    } else {
      // Action lorsque le bouton Bas est relâché
    }
  } else if (topicStr == bouton_gauche_topic) {
    if (payloadStr == "1") {
      FNCSendChar('G','*');
      // Action lorsque le bouton Gauche est appuyé
    } else {
      // Action lorsque le bouton Gauche est relâché
    }
  } else if (topicStr == bouton_droite_topic) {
    if (payloadStr == "1") {
      FNCSendChar('D','*');
      // Action lorsque le bouton Droite est appuyé
    } else {
      // Action lorsque le bouton Droite est relâché
    }
  } else if (topicStr == slider_topic) {
    // Conversion de la valeur en entier
    int valeurSlider = payloadStr.toInt();
    Serial.println(valeurSlider);
    FNCSendInt(valeurSlider);
    // Action en fonction de la valeur du slider
  }
}

void MqttSendDistance(int a,float valeur) {
  char payload[6];
  dtostrf(valeur, 5, 2, payload);
  if (a == 1) {client.publish(ultrason_topic1, payload);}
  /*
  if (a == 2) {client.publish(ultrason_topic2, payload);}
  if (a == 3) {client.publish(ultrason_topic3, payload);}*/
}

void MqttSendBatterie(int a,float valeur) {
  char payload[6];
  dtostrf(valeur, 5, 2, payload);
  if (a == 1) {client.publish(batterie_topic1, payload);}
  /*
  if (a == 2) {client.publish(batterie_topic2, payload);}
  if (a == 3) {client.publish(batterie_topic3, payload);}*/
  
}

void MqttSendObstacle(int a,int valeur) {
  char payload[6];
  dtostrf(valeur, 5, 2, payload);
  if (a == 1) {client.publish(topic_obstacle1, payload);}
  /*
  if (a == 2) {client.publish(topic_obstacle2, payload);}
  if (a == 3) {client.publish(topic_obstacle3, payload);}*/
}







