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


const int defaultSpeed = 75;
// pin used by the left line sensor
const int lineL = 10;
const int lineC = 11;
const int lineR = 12;
int change = 0;

int leftSpeed = defaultSpeed;
int rightSpeed = defaultSpeed;

Robot robot;

void setup() {
  robot.init();
  Serial.begin(9600);
}

void loop() {
  // read and print value ouputted by DFECBot's sensors
  int line_L = readQD(lineR);
  int line_C = readQD(lineC);
  int line_R = readQD(lineL);

  // center line sensor detects line
  if (line_C > 1000){
    leftSpeed = defaultSpeed;
    rightSpeed = defaultSpeed;

  }

  // slightly off center to the right
  else if (line_R > 1000){
    change = change + 5;
    leftSpeed = defaultSpeed - change;
    rightSpeed = defaultSpeed + change;
  }

  // slightly off center to the left
  else if (line_L >1000){
    change = change - 5;
    leftSpeed = defaultSpeed - change;
    rightSpeed = defaultSpeed + change;
  }

   robot.forward2(leftSpeed, rightSpeed);
}

int readQD(int line_pin){
  pinMode(line_pin,OUTPUT);
  digitalWrite(line_pin,HIGH);
  delayMicroseconds(10);
  pinMode(line_pin,INPUT);

  long time=micros();

  while(digitalRead(line_pin)==HIGH&&micros()-time<3000);
  int diff=micros()-time;
  return diff;
}
