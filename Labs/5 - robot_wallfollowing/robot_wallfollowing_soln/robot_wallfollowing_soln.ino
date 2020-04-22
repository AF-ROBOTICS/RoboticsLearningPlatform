/* 
 *  Filename: robot_obstacleavoidance.ino
 *  Author: Capt Steven Beyer
 *  Created: 21 April 2020
 *  Description: Example Arduino Sketch that prints 
 *    values from the DFECBot's left IR Sensor.
 *  
 *  Source: Based off the SharpDistSensorArray.ino
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
 *      right GP2Y0A51SK0F Analog Distance Sensor to the serial 
 *      monitor.
 *    3) Use a ruler to confirm the accuracy of each distance 
 *      sensor - the sensor should be fairly accurate between 
 *      3 cm and 12 cm.
 *    2) Program the DFECBot to detect  an object within 4 cm 
 *      in front of, to the left of, and to the right of the 
 *      DFECBott using the three Distance Sensors.
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
#define OFFSET 25     // dist from IR sensor to center of robot in mm
#define DESIRED 100   // desired dist from wall
#define PWMNOMINAL 100
#define SWING 15
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)

// Controller variables
int SetPoint = 100;
int Error;
const int Kp = 2;
int UR, UL;

// choose between modes:
//  0 - follow wall on right
//  1 - follow wall on left
//  2 - follow walls on both sides
const int Mode = 0;

void setup() {
  // Initialize the motors
  Motor_Init();
  
  // Set parameters for each sensor
  sensorR.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  sensorC.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  sensorL.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
}

void loop() {
  // Read distance (in mm) for each sensor
  unsigned int distR = sensorR.getDist() + offset;
  unsigned int distC = sensorC.getDist() + offset;
  unsigned int distL = sensorL.getDist() + offset;

  // follow wall on right
  if(Mode == 0){
    Error = DESIRED - distR;
  }

  // follow wall on left
  if(Mode == 1){
    Error = distL - DESIRED;
  }

  // follow walls on both sides
  if(Mode == 2){
    if((distL > DESIRED) && (distR > DESIRED)){
      SetPoint = (distL + distR)/2;
    }else{
      SetPoint = DESIRED;
    }
    if(distL < distR){
      Error = distL - SetPoint;
    }else{
      Error = SetPoint - distR;
    }
  }
  
  // Proportional controller
  UR = PWMNOMINAL+Kp*Error;
  UL = PWMNOMINAL-Kp*Error;
  if(UR < PWMMIN) UR = PWMMIN;
  if(UR > PWMMAX) UR = PWMMAX;
  if(UL < PWMMIN) UL = PWMMIN;
  if(UL > PWMMAX) UL = PWMMAX;
  
  Motor_Forward(UL, UR);
}
