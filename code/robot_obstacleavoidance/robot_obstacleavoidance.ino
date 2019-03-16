/* 
 *  Filename: robot_obstacleavoidance.ino
 *  Author: Capt Steven Beyer
 *  Created: 16 Mar 2019
 *  Description: Example Arduino Sketch that prints 
 *    values from the DFECBot's left IR Sensor.
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
 *    3) OPTIONAL: Limit drifting within a maze.
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : TB6612FNG.h, drive.h
 */
#include "TB6612FNG.h"
#include "drive.h"

// pin used by the left IR sensor
const int irL = A0;

void setup() {
  // set the pin connected to the left IR sensor to an input
  pinMode(irL, INPUT);
  Serial.begin(9600);
}

/*
 * Function used to convert from analog voltage to distance. 
 *  Derived from graph in GP2Y0A51SK0F datasheet found at 
 *  https://www.pololu.com/product/2450/resources
 *  The IR sensor is effective between 2-15 cm.
 */
float distCalc(float sensorVal){
    return 11.5402*pow(0.995796, sensorVal);
  }

void loop() {
  // read/convert/print value from the DFECBot's left IR sensor
  float ir_L = analogRead(irL);
  float distL = distCalc(ir_L);
  Serial.print("Left: "); Serial.println(distL);
}
