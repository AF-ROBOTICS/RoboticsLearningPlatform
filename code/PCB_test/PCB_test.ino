#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"

// pins to connect motor to.
const int adir    = 4;  // direction a
const int pwma  = 6;  // HW
const int bdir    = 9;  // direction b
const int pwmb  = 3; // HW
// sets the speed of the robot
const int speed = 200;
// adjust turn delay to get closer to 90 deg turn
const int turnDelay = 1000;

// create an instance of the robot class from drive.h
Robot robot;

void setup(){
  // initialize robot instance setting pins and turn delay
  robot.init(pwma, adir, pwmb, bdir, turnDelay);
	Serial.begin(96000);
}

// basic code to go forward for a second and then turnRight
void loop(){
  robot.forward(speed);
  delay(1000);
  robot.turnRight(speed);
  delay(1000);
}
