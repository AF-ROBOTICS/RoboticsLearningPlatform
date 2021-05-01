/* 
 *  Filename: robot_wallfollowing.ino
 *  Author: Capt Steven Beyer
 *  Created: 21 April 2020
 *  Description: Example Arduino Sketch that prints 
 *    values from the DFECBot's right IR Sensor.
 *  
 *  Source: Adapted from SharpDistSensorArray.ino
 *    https://github.com/DrGFreeman/SharpDistSensor
 *    MIT License
 *    Copyright (c) 2018 Julien de la Bruere-Terreault <drgfreeman@tuta.io>
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    
 *  Assignment:
 *    1) Print the values from the DFECBot's center and 
 *      left GP2Y0A51SK0F Analog Distance Sensor to the serial 
 *      monitor.
 *    2) Use a ruler to confirm the accuracy of each distance 
 *      sensor - the sensor should be fairly accurate between 
 *      3 cm and 12 cm.
 *    3) Program the DFECBot to follow wall on right.
 *    4) Program the DFECBot to follow wall on left.
 *    5) Program the DFECBot to stay between two walls.
 *    Note: Remove all print statements/delays when running your wall following
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : Motor.h, SharpDistSensor.h
 */

#include <SharpDistSensor.h>
#include "Motor.h"

/******************** Sensor variables *****************/
// Analog pins used by the sensors
const byte irR = A0;
const byte irC = A1;
const byte irL = A2;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Create an object instance of the SharpDistSensor class for each sensor
SharpDistSensor sensorR(irR, medianFilterWindowSize);
SharpDistSensor sensorC(irC, medianFilterWindowSize);
SharpDistSensor sensorL(irL, medianFilterWindowSize);


/******************** Controller variables *****************/
#define OFFSET 50     // dist from IR sensor to center of robot in mm
#define DESIRED 100   // desired dist from wall
#define PWMNOMINAL 100
#define SWING 40
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)

// Proportional gain constant
const int Kp = 2;


// choose between modes:
//  0 - follow wall on right
//  1 - follow wall on left
//  2 - follow walls on both sides
int Mode;

void setup() {
  // Initialize the motors
  Motor_Init();
  
  // Set parameters for each sensor
  sensorR.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  sensorC.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  sensorL.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);

  Serial.begin(9600);
}

void loop() {
  int error, setPoint, pwmR, pwmL;
  
  // Read distance (in mm) for each sensor
  unsigned int distR = sensorR.getDist() + OFFSET;
  unsigned int distC = sensorC.getDist() + OFFSET;
  unsigned int distL = sensorL.getDist() + OFFSET;

  if (distR < 125){
    Mode = 0;
  }
  else if (distL <125){
    Mode = 1;
  }
  else if (distL <125 && distR < 125){
    Mode = 2;
  }
  // follow wall on right
  if(Mode == 0){
    error = DESIRED - distR;
  }

  // follow wall on left
  if(Mode == 1){
    error = distL - DESIRED;
  }

  // follow walls on both sides
  if(Mode == 2){
    if((distL > DESIRED) && (distR > DESIRED)){
      setPoint = (distL + distR)/2;
    }else{
      setPoint = DESIRED;
    }
    if(distL < distR){
      error = distL - setPoint;
    }else{
      error = setPoint - distR;
    }
  }
  
  // Proportional controller
  pwmR = PWMNOMINAL+Kp*error;
  pwmL = PWMNOMINAL-Kp*error;

  // Bounds checking
  if(pwmR < PWMMIN) pwmR = PWMMIN;
  if(pwmR > PWMMAX) pwmR = PWMMAX;
  if(pwmL < PWMMIN) pwmL = PWMMIN;
  if(pwmL > PWMMAX) pwmL = PWMMAX;
  
  Motor_Forward(pwmL, pwmR);
}
