/* 
 *  Filename: drive.h
 *  Author: Capt Steven Beyer
 *  Created: 8 Mar 2019
 *  Description: Header file that provides functions to drive the 
 *    DFECBot.
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

// initializes robot and sets up motor pins
void init(){
  motorAB.init();
  motorAB.begin();
  motorAB.stopBothMotors();
}

// stop all motors
void allStop(){
  motorAB.stopBothMotors();
}

// turn the DFECBot left until next command
void turnLeft(int spd){
  motorAB.motorRForward(spd*.5);
  motorAB.motorLReverse(spd*.5);
}

// turn the DFECBot right until next command
void turnRight(int spd){
  motorAB.motorLForward(spd*.5);
  motorAB.motorRReverse(spd*.5);
}

// drive the DFECBot forward until next command
void forward(int spd){
  motorAB.motorRForward(spd*1.065);
  motorAB.motorLForward(spd);
}

// drive the DFECBot in reverse until next command
void reverse(int spd){
  motorAB.motorRReverse(spd);
  motorAB.motorLReverse(spd);
}

// correct right slightly
// To-Do: Drive each wheel forward, one slightly slower than other
void slightSlightRight(int spd){
  motorAB.motorLForward(spd);
  motorAB.motorRForward(spd*.965);
}

void slightRight(int spd){
  motorAB.motorLForward(spd*1.25);
  motorAB.motorRForward(spd*.615);
}

void hardRight(int spd){
  motorAB.motorLForward(spd*1.55);
  motorAB.motorRForward(spd*.415);
}

// correct left slightly
// To-Do: Drive each wheel forward, one slightly slower than other
void slightSlightLeft(int spd){
  motorAB.motorLForward(spd*.9);
  motorAB.motorRForward(spd*1.065);
}

void slightLeft(int spd){
  motorAB.motorLForward(spd*.55);
  motorAB.motorRForward(spd*1.315);
}

void hardLeft(int spd){
  motorAB.motorLForward(spd*.35);
  motorAB.motorRForward(spd*1.615);
}

private:
  MotorDriver motorAB;
};
#endif
