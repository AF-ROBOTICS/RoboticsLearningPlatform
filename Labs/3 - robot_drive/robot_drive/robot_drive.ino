/* 
 *  Filename: robot_drive.ino
 *  Author: Capt Steven Beyer
 *  Created: 7 Mar 2019
 *  Description: Arduino Sketch to drive the robot in different patterns.
 *  
 *  Assignment:
 *    1) Drive the DFECBot forward.
 *    2) Drive the DFECBot forward and reverse.
 *    3) Drive the DFECBot forward, turn around, and return to start.
 *    4) Drive the DFECBot in a square.
 *    5) Drive the DFECBot in a small circle.
 *    6) Drive the DFECBot in a large circle.
 *    7) Drive the DFECBot in a pattern of choice.
 *    
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : Motor.h
 */

#include "Motor.h"

// sets the speed of a wheel 0 - 255
int UL = 100;
int UR = 100;

// adjust turn delay to get closer to 90 deg turn
const int turnDelay = 500;

void setup(){
  // initialize robot instance setting pins
  Motor_Init();
}

void loop(){
  /* 
   *  Code to make the DFECBot go forward, turn left, turn right, go 
   *  backward and stop. This code will repeat continuously 
   *  until the DFECBot is powered off
   *  ***Delete this example code before submission***
   */
  Motor_Forward(UL, UR);
  delay(2000);
  Motor_Left(UL, UR);
  delay(2000);
  Motor_Right(UL,UR);
  delay(2000);
  Motor_Backward(UL,UR);
  delay(2000);
  Motor_Stop();
  delay(2000);

  /*
   * 1. Code to drive forward
   */


   /*
   * 2. Code to drive forward and reverse
   */
   

   /*
   * 3. Code to drive forward, turn around, and return to start
   */
   

   /*
   * 4. Code to drive in a square
   */

   
   /*
   * 5. Code to drive in a small circle
   */


   /*
   * 6. Code to drive in a large circle
   */
   

   /*
   * 7. Code to drive in a pattern of choice
   */
}
