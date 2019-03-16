/* 
 *  Filename: robot_linefollowing.ino
 *  Author: Capt Steven Beyer
 *  Created: 8 Mar 2019
 *  Description: Example Arduino Sketch that prints 
 *    values from the DFECBot's left QRE1113 Line Sensor.
 *    
 *  Assignment:
 *    1) Print the values from the DFECBot's center and right 
 *      QRE1113 Line Sensors to the serial monitor.
 *    3) Observe how these values change if the sensor is over a solid 
 *      black line.
 *    2) Program the DFECBot to follow a line using the 3 QRE1113 Line
 *      Sensors.
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : TB6612FNG.h, drive.h
 */
#include "TB6612FNG.h"
#include "drive.h"
 
// pin used by the left line sensor
const int lineL = A3;

void setup() {
  // set the pin connected to the left line sensor to an input
  pinMode(lineL, INPUT);
  Serial.begin(9600);
}

void loop() {
  // read and print value from the DFECBot's left line sensor
  float line_L = analogRead(lineL);
  Serial.print("Left: "); Serial.println(line_L);

  // print every second
  delay(1000); 
}
