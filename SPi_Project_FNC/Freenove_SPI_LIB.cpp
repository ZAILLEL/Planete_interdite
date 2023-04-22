#include "Freenove_SPI_LIB.h"

Servo servo;             //create servo object
byte servoOffset = 0;    //change the value to Calibrate servo

void calculateVoltageCompensation();

int speedOffset;

void pinsSetup() {

  
	pinMode(PIN_DIRECTION_LEFT, OUTPUT);
	pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT);
	pinMode(PIN_DIRECTION_RIGHT, OUTPUT);
	pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT);

	pinMode(PIN_SONIC_TRIG, OUTPUT);// set trigPin to output mode
	pinMode(PIN_SONIC_ECHO, INPUT); // set echoPin to input mode

	pinMode(PIN_TRACKING_LEFT, INPUT); // 
	pinMode(PIN_TRACKING_RIGHT, INPUT); // 
	pinMode(PIN_TRACKING_CENTER, INPUT); // 

	setBuzzer(false);
  
  servo.attach(PIN_SERVO);        //initialize servo 
  servo.write(90 + servoOffset);  // change servoOffset to Calibrate servo
  calculateVoltageCompensation();
}


int SonnarScanning(){

  float distance[3];

  servo.write(45);
  delay(1000);
  distance[0] = getSonar();   

  servo.write(90);
  delay(1000);
  distance[1] = getSonar();

  servo.write(135);
  delay(1000);
  distance[2] = getSonar();

  servo.write(90);

  float max = distance[0];
  
  int max_index = 0;
  for (int i=0;i<3;i++) {   
    if (distance[i] > distance[max_index]) {
      max = distance[i];
      max_index = i;
    }  
  }

  Serial.println(distance[0]);
  Serial.println(distance[1]);
  Serial.println(distance[2]);  
  return max_index;
}


void motorRun(int speedl, int speedr) {
	int dirL = 0, dirR = 0;
	if (speedl > 0) {
		dirL = 0;
	}
	else {
		dirL = 1;
		speedl = -speedl;
	}

	if (speedr > 0) {
		dirR = 1;
	}
	else {
		dirR = 0;
		speedr = -speedr;
	}

	speedl = constrain(speedl, 0, 255);
	speedr = constrain(speedr, 0, 255);

	if (abs(speedl) < MOTOR_PWM_DEAD && abs(speedr) < MOTOR_PWM_DEAD) {
		speedl = 0;
		speedr = 0;
	}

	digitalWrite(PIN_DIRECTION_LEFT, dirL);
	digitalWrite(PIN_DIRECTION_RIGHT, dirR);
	analogWrite(PIN_MOTOR_PWM_LEFT, speedl);
	analogWrite(PIN_MOTOR_PWM_RIGHT, speedr);
}


float getBatteryVoltage() {

	  float batteryVoltage;
		pinMode(PIN_BATTERY, INPUT);
		int batteryADC = analogRead(PIN_BATTERY);
    batteryVoltage = batteryADC / 1023.0 * 5.0 * 4;  // Calcul Flottant : pas optimale, à revoir
    return batteryVoltage;
}


void setBuzzer(bool flag) {
	//isBuzzered = flag;
	pinMode(PIN_BUZZER, flag);
	digitalWrite(PIN_BUZZER, flag);
}

void freenvove_move_backward(int speed)  {  motorRun(speed, speed); }
void freenvove_move_forward(int speed)  {  motorRun(-speed, -speed); }
void freenvove_move_right(int speed)    {  motorRun(speed, -speed); }
void freenvove_move_left(int speed)     {  motorRun(-speed, speed); }
void freenvove_stop()                   {  motorRun(0, 0);}




void resetCarAction() {
	motorRun(0, 0);
	setBuzzer(false);
}


float getSonar() {
  unsigned long pingTime;
  float distance;
  digitalWrite(PIN_SONIC_TRIG, HIGH); // make trigPin output high level lasting for 10μs to triger HC_SR04,
  delayMicroseconds(10);
  digitalWrite(PIN_SONIC_TRIG, LOW);
  pingTime = pulseIn(PIN_SONIC_ECHO, HIGH, SONIC_TIMEOUT); // Wait HC-SR04 returning to the high level and measure out this waitting time
  if (pingTime != 0)
    distance = (float)pingTime * SOUND_VELOCITY / 2 / 10000; // calculate the distance according to the time
  else
    distance = MAX_DISTANCE;
  return distance; // return the distance value
}



/*
void alarm(u8 beat, u8 repeat) {
	beat = constrain(beat, 1, 9);
	repeat = constrain(repeat, 1, 255);
	for (int j = 0; j < repeat; j++) {
		for (int i = 0; i < beat; i++) {
			setBuzzer(true);
			delay(100);
			setBuzzer(false);
			delay(100);
		}
		delay(500);
	}
}*/

void updateAutomaticObstacleAvoidance() {
  int distance[3], tempDistance[3][5], sumDisntance;
  static u8 leftToRight = 0, servoAngle = 0, lastServoAngle = 0;  //
  const u8 scanAngle[2][3] = { {150, 90, 30}, {30, 90, 150} };

  for (int i = 0; i < 3; i++)
  {
    servoAngle = scanAngle[leftToRight][i];
    servo.write(servoAngle);
    if (lastServoAngle != servoAngle) {
      delay(130);
    }
    lastServoAngle = servoAngle;
    for (int j = 0; j < 5; j++) {
      tempDistance[i][j] = getSonar();
      delayMicroseconds(2 * SONIC_TIMEOUT);
      sumDisntance += tempDistance[i][j];
    }
    if (leftToRight == 0) {
      distance[i] = sumDisntance / 5;
    }
    else {
      distance[2 - i] = sumDisntance / 5;
    }
    sumDisntance = 0;
  }
  leftToRight = (leftToRight + 1) % 2;

  if (distance[1] < OBSTACLE_DISTANCE) {        //Too little distance ahead
    if (distance[0] > distance[2] && distance[0] > OBSTACLE_DISTANCE) {     //Left distance is greater than right distance
      motorRun((150 + speedOffset), (150 + speedOffset)); //Move back
      delay(100);
      motorRun(-(150 + speedOffset), (150 + speedOffset));  
    }
    else if (distance[0] < distance[2] && distance[2] > OBSTACLE_DISTANCE) {                   //Right distance is greater than left distance
      motorRun((150 + speedOffset), (150 + speedOffset)); //Move back 
      delay(100);
      motorRun((150 + speedOffset), -(150 + speedOffset));
    }
    else {                      //Get into the dead corner, move back, then turn.
      motorRun((150 + speedOffset), (150 + speedOffset));
      delay(100);
      motorRun(-(150 + speedOffset), (150 + speedOffset));
    }
  }
  else {                        //No obstacles ahead
    if (distance[0] <  OBSTACLE_DISTANCE_LOW) {      //Obstacles on the left front.
      motorRun((150 + speedOffset), (150 + speedOffset)); //Move back
      delay(100);
      motorRun(-(180 + speedOffset), -(50 + speedOffset));
    }
    else if (distance[2] <  OBSTACLE_DISTANCE_LOW) {     //Obstacles on the right front.
      motorRun((150 + speedOffset), (150 + speedOffset)); //Move back
      delay(100);
      motorRun(-(50 + speedOffset), -(180 + speedOffset));
    }
    else {                        //Cruising
      motorRun((80 + speedOffset), (80 + speedOffset));
    }
  }
}

void calculateVoltageCompensation() {
  float voltageOffset = 8.4 - getBatteryVoltage();
  speedOffset = voltageOffset * 20;
}



