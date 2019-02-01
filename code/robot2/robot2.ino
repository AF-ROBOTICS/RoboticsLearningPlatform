

#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"

const int adir    = 4;  // direction a
const int pwma  = 6;  // HW
const int bdir    = 9;  // direction b
const int pwmb  = 10; // HW
const int speed = 200;
const int turnDelay = 1000;
const int pause = 275;
const byte irL = A0;
const byte irC = A1;
const byte irR = A2;


Robot robot;

void setup(){
  robot.init(pwma, adir, pwmb, bdir, turnDelay, pause);
	Serial.begin(96000);
}

void loop(){
  robot.forward(speed);
  delay(5);
  robot.turnLeft(speed);
  delay(5);
  robot.turnRight(speed);
  delay(5);
  robot.reverse(speed);
  delay(5);
}
