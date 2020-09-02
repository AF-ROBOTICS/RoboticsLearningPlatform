/* 
 *  Filename: robot_drive.ino
 *  Author: Capt Steven Beyer
 *  Created: 7 Mar 2019 (edited 2 March 2020)
 *  Description: Arduino Sketch to drive the robot at different duty cycles
 *  
 *  Assignment:
 *    1) Run the below code while connected to the Moku:Lab
 *    
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : drive.h
 */

#include "Motor.h"

void setup(){
  // initialize robot instance setting pins
  Motor_Init();
}

void loop(){
  /* 
  *  Code to drive the DFECBot forward at different duty cycles
  *  This code will repeat continuously until the DFECBot is powered off
  */
  // drive the left and right motors at (50/255)% = 19.6% duty cycle
  // robot.Motor_Forward(int Value, int Value)
  Motor_Forward(50, 50); 
  delay(2000);
  
  // drive both motors at 25% duty cycle
  Motor_Forward(64, 64);
  delay(2000);
  
  // drive both motors at 50% duty cycle
  Motor_Forward(128, 128);
  delay(2000);
  
  // drive both motors at 75% duty cycle
  Motor_Forward(191, 191);
  delay(2000);
  
  // drive both motors at 100% duty cycle
  Motor_Forward(255, 255);
  delay(2000);
  
  // drive both motors at 15% duty cycle
  Motor_Forward(38, 38);
  delay(2000);
  
  // stop both motors
  Motor_Stop();
  delay(2000);
}
