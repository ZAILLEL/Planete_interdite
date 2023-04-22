#include "Freenove_SPI_LIB.h"
#include "ESP_COM.h"




unsigned long lastscanning = 0;
unsigned long lastmanuelmode = 0;
unsigned long lastESPRX = 0;

unsigned long lastBatteryTX = 0;
unsigned long lastSonarTX = 0;

unsigned int state = 0;
struct esp_rx esp;

char char_button;
char select;
int icpt=0;

void setup() {

  pinsSetup();
  Serial.begin(9600);
  setup_esp();

}

void loop() {
 
  do_task();

}



void manuel_mode()
{
     switch(esp.button_control)
      {
        case 'F' :
          freenvove_move_forward(esp.speed);
          Serial.print("FORWARD");
          break;
        case 'B':
          freenvove_move_backward(esp.speed);
          Serial.println("BACKWARD");
          break;
        case 'L':
          freenvove_move_left(esp.speed);
          Serial.println("LEFT");
          break;
        case 'R':
          freenvove_move_right(esp.speed);
          Serial.println("RIGHT");
          break;
        case 'S':
          freenvove_stop();
          Serial.println("STOP");
          break;
        default: 
        //
        break;
      }
}


void do_task()
{

  if (millis() - lastscanning >= 100) {
    obstacle_avoid(); 
    lastscanning = millis();  // enregistre le temps de l'appel
  }

  if (millis() - lastmanuelmode >= 200) {
    manuel_mode(); 
    lastmanuelmode = millis();  
  }

  if (millis() - lastESPRX >= 100) {
    ESP_RX_MANAGER(); 
    lastESPRX = millis();  
  }


  if (millis() - lastBatteryTX >= 100) {
    SendBattaryVoltageEsp8266(); 
    lastBatteryTX = millis();  
  }

  if (millis() - lastSonarTX >= 100) {     // Envoie de la donnée ULTRASON toutes les 
    SendSonarEsp8266(); 
    lastSonarTX = millis();  
  }

}

void obstacle_avoid()
{
switch(state)
{
  case _NORMAL_RUN:
    freenvove_move_forward(100);
    if(getSonar() < 100) state = _OBSTACLE_DETECTED;
    Serial.println(getSonar());
  break;
  case _OBSTACLE_DETECTED : 
    freenvove_stop();
    switch(SonnarScanning()){
      case 0 : state = _TURNING_RIGHT; break;
      case 1 : state = _NORMAL_RUN; break;
      case 2 : state = _TURNING_LEFT; break;
      default : break;
    }
    break;

    case _TURNING_RIGHT :
      freenvove_move_right(100);
      if(getSonar() > 120) state = _NORMAL_RUN;
      break;

    case _TURNING_LEFT : 
      freenvove_move_left(100);
      if(getSonar()>120) state = _NORMAL_RUN;
      break;

    default : break;
  }

}

void ESP_RX_MANAGER()
{
  esp.speed = Esp8266ReceiveInt('*');
  esp.button_control = Esp8266ReceiveChar('/');
  esp.select_mode = Esp8266ReceiveChar('+');
}










