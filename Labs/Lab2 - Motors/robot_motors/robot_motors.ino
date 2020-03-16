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
 *    Files     : TB6612FNG.h, drive.h
 */

#include "Motor.h"
#include "TB6612FNG.h"

// create an instance of the robot class from drive.h
Robot robot;

void setup(){
  // initialize robot instance setting pins
  robot.Motor_Init();
}

void loop(){
  /* 
   *  Code to drive the DFECBot forward at different duty cycles
   *  This code will repeat continuously until the DFECBot is powered off
   */
   // drive the left and right motors at (50/255)% = 19.6% duty cycle
   // robot.Motor_Forward(int Value, int Value)
   robot.Motor_Forward(50, 50); 
   delay(2000);
   
   // drive both motors at 25% duty cycle
   robot.Motor_Forward( , );
   delay(2000);
   
   // drive both motors at 50% duty cycle
   robot.Motor_Forward( , );
   delay(2000);
   
   // drive both motors at 75% duty cycle
   robot.Motor_Forward( , );
   delay(2000);
   
   // drive both motors at 100% duty cycle
   robot.Motor_Forward( , );
   delay(2000);
   
   // drive both motors at 15% duty cycle
   robot.Motor_Forward( , );
   delay(2000);
   
   // stop both motors
   robot.Motor_Stop();
   delay(2000);
}
