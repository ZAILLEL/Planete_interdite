#include "FREENOVE_COM.h"


SoftwareSerial arduino(2 ,3); // RX, TX


void setup_arduino_com()
{
   arduino.begin(9600); // Initialisation de la communication série avec l'Arduino
}

float FNCReceive(char k)
{
  
 float value = 0.0;
 if (arduino.available()) // Si des données sont disponibles sur la liaison série de l'Arduino
 { 
    String data = arduino.readStringUntil(k); // Lire les données envoyées par l'Arduino
    int separatorIndex = data.indexOf(','); // Trouver l'indice de la virgule séparant les deux valeurs
    if (separatorIndex > 0 && separatorIndex < data.length() - 1) // Vérifier que la virgule n'est pas à la fin ou au début de la chaîne
    { 
      value = data.substring(0, separatorIndex).toFloat(); // Extraire la première valeur flottante et la stocker dans value1
    }
  }

  Serial.print("receive from arduino :  ");
  Serial.println(value);
  return value;
}

void FNCSendChar(char c,char d) 
{
  /*if (Serial.available()) 
  { // Si des données sont disponibles sur la liaison série de l'ordinateur*/
    Serial.print(c);
    arduino.write(c);
    arduino.write(d);

  //}
}

void FNCSendInt(int i) 
{
/*  if (Serial.available()) 
  { // Si des données sont disponibles sur la liaison série de l'ordinateur*/
    Serial.print(i);
    arduino.write((byte*)&i, sizeof(int));
 // }
}

