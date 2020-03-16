/* 
 *  Filename: drive.h
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
 *  Description: Header file that provides functions to drive the 
 *    DFECBot.
 *    Ldir  = digital pin 2;  // direction b
 *    pwmL  = digital pin 3;  // PWM
 *    Rdir  = digital pin 4;  // direction a
 *    pwmR  = digital pin 5;  // PWM
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : TB6612FNG.h
 */
 
#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "TB6612FNG.h"

// create an instance of the MotorDrive class called motorAB
MotorDriver motorAB;

/*
 * The Robot class provides functions to drive the DFECBot.
 */
class Robot {
public:
Robot(){
    ;
}

// ------------Motor_Init------------
// Initialize GPIO pins for output, which will be
// used to control the direction of the motors and
// to enable or disable the drivers.
// The motors are initially stopped
// Input: none
// Output: none
void Motor_Init(){
  motorAB.init();
  motorAB.begin();
  motorAB.stopBothMotors();
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(){
  motorAB.stopBothMotors();
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Forward(int leftDuty, int rightDuty){
  motorAB.motorLForward(leftDuty);
  motorAB.motorRForward(rightDuty);
}

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and the right wheel
// backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Right(int leftDuty, int rightDuty){
  motorAB.motorLForward(leftDuty);
  motorAB.motorRReverse(rightDuty);
}

// ------------Motor_Left------------
// Turn the robot to the left by running the
// left wheel backward and the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Left(int leftDuty, int rightDuty){
  motorAB.motorLReverse(leftDuty);
  motorAB.motorRForward(rightDuty);
}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Backward(int leftDuty, int rightDuty){
  motorAB.motorLReverse(leftDuty);
  motorAB.motorRReverse(rightDuty);
}

private:
  MotorDriver motorAB;
};
#endif
