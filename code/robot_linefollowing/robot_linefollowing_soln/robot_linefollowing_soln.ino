/* 
 *  Filename: robot_linefollowing.ino
 *  Author: Capt Steven Beyer
 *  Created: 8 Mar 2019
 *  Description: Example Arduino Sketch that prints 
 *    values outputted by the DFECBot's left QRE1113 Line Sensor.
 *    
 *  Assignment:
 *    1) Print the values outputted by the DFECBot's center and right 
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
const int lineC = A4;
const int lineR = A5;

const int speed = 100;

Robot robot;

void setup() {
  // set the pin connected to the left line sensor to an input
  pinMode(lineL, INPUT);
  pinMode(lineC, INPUT);
  pinMode(lineR, INPUT);
  robot.init();
  Serial.begin(9600);
}

void loop() {
  // read and print value ouputted by DFECBot's sensors
  float line_L = analogRead(lineL);
  float line_R = analogRead(lineR);
  float line_C = analogRead(lineC);
  Serial.print("Left: "); Serial.println(line_L);
  Serial.print("Center: "); Serial.println(line_C);
  Serial.print("Right: "); Serial.println(line_R);

  // center line sensor detects line
  if (line_L < 650 and line_C > 650 and line_R < 650){
    robot.forward(speed);
    Serial.println("Center");
  }
  // left line sensor detects line
  else if (line_L > 650 and line_C < 650 and line_R < 650){
    robot.slightLeft(speed);
    Serial.println("Left");
  }
  // right line sensor detects line
  else if (line_L < 650 and line_C < 650 and line_R > 650){
    robot.slightRight(speed);
    Serial.println("Right");
  }
  // no line detected, stay straight
  else{
    robot.forward(speed);
    Serial.println("Default");
  }
}
