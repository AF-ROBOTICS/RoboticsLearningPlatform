/* 
 *  Filename: drive.h
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
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
  motorAB.motorRForward(spd);
  motorAB.motorLForward(spd);
}

// drive the DFECBot in reverse until next command
void reverse(int spd){
  motorAB.motorRReverse(spd);
  motorAB.motorLReverse(spd);
}

private:
  MotorDriver motorAB;
};
#endif
