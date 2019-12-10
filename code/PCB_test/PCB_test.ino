/* 
 *  Filename: PCB_test.ino
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
 *  Description: Arduino Sketch that tests the operation of the PCB board.
 *    If the board is working correctly both motors will drive forward for
 *    one second and then the right motor will go backwards while the left
 *    motor will go forwards (right turn). Both motors will stop and pause
 *    for one second.
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : TB6612FNG.h, drive.h
 */

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
  robot.init();
  Serial.begin(96000);
}

// basic code to go forward for a second and then turn right
void loop(){
  robot.forward(speed);
  delay(1000);
  robot.turnRight(speed);
  delay(turnDelay);
  robot.turnLeft(speed);
  delay(turnDelay);
  robot.allStop();
  delay(1000);
}
