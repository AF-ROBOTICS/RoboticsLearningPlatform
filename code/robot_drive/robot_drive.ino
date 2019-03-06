#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"


// sets the speed of the robot
const int speed = 200;
// adjust turn delay to get closer to 90 deg turn
const int turnDelay = 1000;

// create an instance of the robot class from drive.h
Robot robot;

void setup(){
  // initialize robot instance setting pins and turn delay
  robot.init(turnDelay);
	Serial.begin(96000);
}

// basic code to go forward for a second and then turn right
void loop(){
  robot.forward(speed);
  delay(1000);
  robot.turnRight(speed);
  delay(turnDelay);
  robot.allStop()
  delay(1000);
}
