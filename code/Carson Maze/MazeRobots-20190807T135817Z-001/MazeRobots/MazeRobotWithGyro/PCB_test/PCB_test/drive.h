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
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

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
  
    lsm.begin();
  
    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
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
 * This function spins the robot slowly to calibrate
 * the gyroscope, and allow for compass measurements
 * to be taken.
 */
void calibrateGyro() {
  int total = 0;
  
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);
  
  double xStart = m.magnetic.x;
  double yStart = m.magnetic.y;

  double xAvg = xStart;
  double yAvg = yStart;

  double xStddev = 0.0;
  double yStddev = 0.0;

  while (1) {
    drivePID(-0.1, 0.1);
    lsm.read();
    lsm.getEvent(&a, &m, &g, &temp);

    
    if (total != 0) {
      xStddev = xStddev + (m.magnetic.x - (xAvg / total)) * (m.magnetic.x - (xAvg + m.magnetic.x) / (total + 1));
      yStddev = yStddev + (m.magnetic.y - (yAvg / total)) * (m.magnetic.y - (yAvg + m.magnetic.y) / (total + 1));
    }

    xAvg += m.magnetic.x;
    yAvg += m.magnetic.y;


    ++total;

    if (abs(xStart - m.magnetic.x) + abs(yStart - m.magnetic.y) < 0.02 && total > 50) {
      break;
    }
  }
  drive(0, 0);

  gyroXCenter = xAvg / total;
  gyroYCenter = yAvg / total;

  xStddev = sqrt(xStddev / total);
  yStddev = sqrt(yStddev / total);

  gyroXStddev = xStddev;
  gyroYStddev = yStddev;

  gyroOffset = getGyroHeading();

  Serial.print("Calibrated with ");
  Serial.print(total);
  Serial.println(" samples");
  Serial.println(xStddev);
  Serial.println(yStddev);
}

/*!
 * Turns the robot to a specific gyro heading.
 */
void turnAbsoluteAngle(double angle) {
  Serial.print("Turning to ");
  Serial.println(angle);
  while (1) {
    double err = getAngleDiff(getGyroHeading(), angle);
    
    Serial.println(err);
    
    if (abs(err) < 2.0) {
      drive(0, 0);
      break;
    }

    
    double spd = signum(err) * min(0.15, 0.04 + abs(err / 270.0));
    drivePID(-spd, spd);
  }
}

/*!
 * \brief Returns the heading of the robot in degrees.
 * 
 * The heading is in the range [0, 360) relative measured to 'East',
 * where a more positive angle means more counter-clockwise if the robot
 * is viewed from above.
 */
double getGyroHeading() {
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  double ang = 0;
  for (int i = 0; i < 30; ++i) {
    ang += (360.0 / (2 * PI) * atan2((m.magnetic.y - gyroYCenter) / gyroYStddev, (m.magnetic.x - gyroXCenter) / gyroXStddev)) / 30.0;
  }
  
  ang = 360.0 - ang;
  ang -= gyroOffset;
  ang -= 360.0 * floor(ang / 360.0);
  return ang;
}

/*!
 * Uses the gyroscope to turn the robot to a specific Direction
 */
void turnAbsoluteAngle(Direction dir) {
  turnAbsoluteAngle((__builtin_ctz(dir) % 4) * 90.0);
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
  
  double gyroXCenter, gyroYCenter, gyroOffset = 0.0;
  double gyroXStddev = 0.0, gyroYStddev = 0.0;
  
  Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
};
#endif
