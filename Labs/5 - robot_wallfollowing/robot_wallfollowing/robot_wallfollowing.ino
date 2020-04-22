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

// Number of sensors used on the bot
const byte numSensors = 1;

// Analog pins used by the sensors
const byte irL = A2;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Define the array of SharpDistSensor objects
SharpDistSensor sensorArray[] ={
  SharpDistSensor(irL, medianFilterWindowSize)
  // Add as many sensors as required
};

// Define an array of integers that will store the measured distances
uint16_t distArray[numSensors];

void setup() {
  // Initialize the motors
  Motor_Init();
  
  // Set parameters for each sensor in array
  for (int i = 0; i < numSensors; i++){
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  }
  Serial.begin(9600);
}

void loop() {
  // Read distance (in mm) for each sensor into an array of distances and print
  for (int i = 0; i < numSensors; i++){
    distArray[i] = sensorArray[i].getDist();
    Serial.println(distArray[i]);
  }
  delay(50);
}
