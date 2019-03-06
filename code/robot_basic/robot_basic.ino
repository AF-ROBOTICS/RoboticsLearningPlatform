

#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"

const int speed = 200;
const int turnDelay = 1000;


Robot robot;

void setup(){
  robot.init(turnDelay);
	Serial.begin(96000);
}

void loop(){
  robot.forward(speed/2);
  delay(1000);
  robot.turnLeft(speed);
  delay(1000);
  robot.turnRight(speed);
  delay(1000);
  robot.reverse(speed/2);
  delay(1000);
}
