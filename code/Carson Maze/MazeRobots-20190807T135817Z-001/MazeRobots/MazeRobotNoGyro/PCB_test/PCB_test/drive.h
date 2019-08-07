/*! \file drive.h
 *  \brief Contains mid-level drive commands for the robot, such as driving forward and turning.
 */

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
#include <Encoder.h>
#include "util.h"

// create an instance of the MotorDrive class called motorAB
//MotorDriver motorAB;

constexpr const double edges_per_mm = 9.4024 * 0.813;

double degreesToEdges(double deg) {
  return deg * edges_per_mm * 81.04 * 0.85 / 90.0 / 0.813;
}

/*!
 * The Robot class provides functions to drive the DFECBot.
 */
class Robot {
public:
Robot()
{
  lEnc.write(0);
  rEnc.write(0);
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

 void turnRight(int spd){
    motorAB.motorLForward(spd*.5);
    motorAB.motorRReverse(spd*.5);
 }

void forward(int spd){
    motorAB.motorRForward(spd);
    motorAB.motorLForward(spd);
 }
 
void reverse(int spd){
    motorAB.motorRReverse(spd);
    motorAB.motorLReverse(spd);
 }

void drive(int lSpd, int rSpd) {
    motorAB.motorLSpeed(lSpd);
    motorAB.motorRSpeed(rSpd);
}

/*!
 * \brief Uses PID control to drive the robot at a certain speed.
 * 
 * Note that speed inputs to this function are in the range [-1.0, 1.0].
 * This function does not block.
 */
void drivePID(double lSpd, double rSpd) {
  lSetpoint = lSpd;
  rSetpoint = rSpd;
  updatePID();
  drive(255 * lOutput, 255 * rOutput);
}

/*!
 * Returns the current raw value of the left encoder (in EDGES) 
 */
long readLEnc() {
  return lEnc.read();
}

/*! 
 * Returns the current raw value of the right encoder (in EDGES)
 */
long readREnc() {
  return rEnc.read();
}

/*!
 * Turns the robot a specified number of degrees. Positive means counterclockwise, as viewed from above the robot.
 * This function blocks.
 */
void turnAngle(double angle) {
  const double rawTarget = degreesToEdges(angle);
  
  const double lStart = lEnc.read();
  const double rStart = rEnc.read();
  
  const double lTarget = -rawTarget + lEnc.read();
  const double rTarget =  rawTarget + rEnc.read();

  // Using signs like this allows for using the same condition regardless of turning CW or CCW--It works, I promise
  while (signum(lEnc.read() - lTarget) * signum(rawTarget) >= 0 && signum(rEnc.read() - rTarget) * signum(rawTarget) <= 0) {
    const double ramp = bothRamp(lStart, lTarget, lEnc.read());

    lSetpoint = (0.07 + ramp * 0.3) * ((angle < 0.0) ? 1 : -1);
    rSetpoint = -lSetpoint;
    
    updatePID();
    
    drive(255 * lOutput, 255 * rOutput);
    
    delay(5);
  }

  drive(0, 0);
}

/*! 
 * Recalculates the motor output power based on the current setpoints--note that this does NOT actually set the motors to the new speed!
 */
void updatePID() {
    double t = millis();

    if (t - lastPIDUpdate < 5) {
      return;
    }

    long a = lEnc.read();
    long b = rEnc.read();

    // da and db are the velocity error values in edges per millisecond
    double da = (a - lLastPos) / (t - lastPIDUpdate) - lSetpoint * 7.0;
    double db = (b - rLastPos) / (t - lastPIDUpdate) - rSetpoint * 7.0;
    
    lLastPos = a;
    rLastPos = b;
    
    // Feedforward and proportional gains for each wheel
    const double lF = 7.0 / 8.0, lP = 0.4;
    const double rF = 7.0 / 7.6, rP = 0.4;

    lOutput = lSetpoint * lF - da * lP;
    rOutput = rSetpoint * rF - db * rP;
    
    lastPIDUpdate = millis();
}

private:
  MotorDriver motorAB;
  
  Encoder lEnc = Encoder(2, 11);
  Encoder rEnc = Encoder(3, 12);
  
  long lLastPos = 0, rLastPos = 0;
  double lSetpoint = 0, rSetpoint = 0;
  double lOutput = 0, rOutput = 0;
  double lastPIDUpdate = millis();
};
#endif
