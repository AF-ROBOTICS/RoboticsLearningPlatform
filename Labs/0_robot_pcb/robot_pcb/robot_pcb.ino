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
 *    Files     : Motor.h
 */

#include "Motor.h"


// set the speed of the robot to 25% Duty Cycle
const int speed = 122;

void setup(){
  // initialize robot instance setting pins and turn delay
  Motor_Init();
}

// basic code to go forward for a second and then turn right
void loop(){
  Motor_Forward(speed, speed);
  delay(1000);
  Motor_Left(speed, speed);
  delay(1000);
  Motor_Right(speed, speed);
  delay(1000);
  Motor_Backward(speed,speed);
  delay(1000);
  Motor_Stop();
  delay(1000);
}
