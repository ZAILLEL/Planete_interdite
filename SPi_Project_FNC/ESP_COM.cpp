#include "ESP_COM.h"



SoftwareSerial esp8266(2, 3); // RX, TX

void setup_esp()
{
     esp8266.begin(9600); 
}

void SendBattaryVoltageEsp8266()
{
   esp8266.write(getBatteryVoltage())+'_';
}


void SendSonarEsp8266()
{
   esp8266.write(getSonar()+'^');
}


float Esp8266ReceiveFloat(char k)
{
  
 float value = 0.0;
 if (esp8266.available()) 
 { 
    String data = esp8266.readStringUntil(k); // Lire les données envoyées par l'ESP
    int separatorIndex = data.indexOf(','); // Trouver l'indice de la virgule séparant les deux valeurs
    if (separatorIndex > 0 && separatorIndex < data.length() - 1) // Vérifier que la virgule n'est pas à la fin ou au début de la chaîne
    { 
      value = data.substring(0, separatorIndex).toFloat(); // Extraire la première valeur flottante et la stocker dans value1
    }
  }

  Serial.print("receive from esp8266 :  ");
  Serial.println(value);
  return value;
}

int Esp8266ReceiveInt(char k)
{
  int value = 0;
  if (esp8266.available())
  {
    String data = esp8266.readStringUntil(k);
    int separatorIndex = data.indexOf(',');
    if (separatorIndex > 0 && separatorIndex < data.length() - 1)
    {
      value = data.substring(0, separatorIndex).toInt();
    }
  }

  Serial.print("Received from ESP8266: ");
  Serial.println(value);
  return value;
}

char Esp8266ReceiveChar(char k)
{
  char value = '\0'; // initialiser la variable value avec le caractère nul ('\0')
  
  if (esp8266.available()) 
  { 
    String data = esp8266.readStringUntil(k); // Lire les données envoyées par l'ESP jusqu'au caractère de fin de transmission défini par k
    if (data.length() > 0) // Vérifier que des données ont été lues
    { 
      value = data.charAt(0); // Extraire le premier caractère de la chaîne de caractères et le stocker dans value
    }
  }

  Serial.print("Reçu depuis esp8266 :  ");
  Serial.println(value);
  return value; // Renvoyer le caractère reçu
}




