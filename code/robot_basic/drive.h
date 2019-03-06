/* 
 *  Filename: drive.h
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
 *  Description: Header file that provides functions to drive the DFECBot.
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : TB6612FNG.h
 */
#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "TB6612FNG.h"

// pauses the motor between different commands to help with precision
const int motorPause = 275;

// controls how long the robot turns and is set during init
static int turnPause;

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
void init(int turnP){
   motorAB.init();
   motorAB.begin();
   turnPause = turnP;
   motorAB.stopBothMotors();
}

// stop all motors
void allStop(){
    motorAB.stopBothMotors();
}

 // turn the DFECBot left 90 deg
 void turnLeft(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorRForward(spd*.5);
    motorAB.motorLReverse(spd*.5);
    delay(turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
 }

 // turn the DFECBot left until next command
 void ncTurnLeft(int spd){
    motorAB.motorRForward(spd*.5);
    motorAB.motorLReverse(spd*.5);
 }

 void halfLeft(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorRForward(spd*.5);
    motorAB.motorLReverse(spd*.5);
    delay(.5*turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
  }

  void quarterLeft(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorRForward(spd*.5);
    motorAB.motorLReverse(spd*.5);
    delay(.25*turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
  }
 
  void turnRight(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorLForward(spd*.5);
    motorAB.motorRReverse(spd*.5);
    delay(turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
 }
 void ncTurnRight(int spd){
    motorAB.motorLForward(spd*.5);
    motorAB.motorRReverse(spd*.5);
 }
  void halfRight(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorLForward(spd*.5);
    motorAB.motorRReverse(spd*.5);
    delay(.5*turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
 }
 void quarterRight(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorLForward(spd*.5);
    motorAB.motorRReverse(spd*.5);
    delay(.25*turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
 }
 void turnAround(int spd){
    motorAB.stopBothMotors();
    delay(motorPause);
    motorAB.motorRForward(spd*.5);
    motorAB.motorLReverse(spd*.5);
    delay(2* turnPause);
    motorAB.stopBothMotors();
    delay(motorPause);
 }

 void forward(int spd){
    motorAB.motorRForward(spd);
    motorAB.motorLForward(spd);
 }
 
  void reverse(int spd){
    motorAB.motorRReverse(spd);
    motorAB.motorLReverse(spd);
 }

 void slightRight(int spd){
    motorAB.motorLForward(spd);
    motorAB.motorRForward(spd*.85);
 }

  void slightLeft(int spd){
    motorAB.motorLForward(spd*.85);
    motorAB.motorRForward(spd);
 }

  void clockwiseCircle(int spd){
    motorAB.motorLForward(spd);
    motorAB.motorRForward(spd*.5);
  }

  void counterClockwiseCircle(int spd){
    motorAB.motorLForward(spd*.5);
    motorAB.motorRForward(spd);
  }

  float distCalc(float sensorVal){
    return 11.5402*pow(0.995796, sensorVal);
  }

private:
  MotorDriver motorAB;
};
#endif
