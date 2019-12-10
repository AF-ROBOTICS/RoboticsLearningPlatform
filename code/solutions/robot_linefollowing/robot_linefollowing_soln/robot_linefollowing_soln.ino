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
const int lineR1 = 2;
const int lineR2 = 5;
const int lineC = 7;
const int lineL2 = 8;
const int lineL1 = 10;

const int speed = 150;

boolean hardLeft = false;
boolean hardRight = false;
boolean offTrack = false;

Robot robot;

void setup() {
  robot.init();
  Serial.begin(9600);
}

void loop() {
  // read and print value ouputted by DFECBot's sensors
  int line_R1 = readQD(lineR1);
  int line_R2 = readQD(lineR2);
  int line_C = readQD(lineC);
  int line_L2 = readQD(lineL2);
  int line_L1 = readQD(lineL1);

  // center line sensor detects line
  if (line_C < 200 and line_R1 > 200 and line_R2 >200 and line_L1 > 200 and line_L2 >200){
    robot.forward(speed);
    hardLeft = false;
    hardRight = false;
    offTrack = false;
  }

  // slightly off center to the right
  else if (line_C < 200 and line_R2 < 200){
    robot.slightSlightRight(speed);
    hardLeft = false;
    hardRight = false;
    offTrack = false;
  }

  // slightly off center to the left
  else if (line_C < 200 and line_L2 <200){
    robot.slightSlightLeft(speed);
    hardLeft = false;
    hardRight = false;
    offTrack = false;
  }

  // right of center
  else if (line_R2 < 200 and line_R1 < 200){
    robot.slightRight(speed);
    hardLeft = false;
    hardRight = false;
    offTrack = false;
  }

  // left of center
  else if (line_L2 < 200 and line_L1 < 200){
    robot.slightLeft(speed);
    hardLeft = false;
    hardRight = false;
    offTrack = false;
  }

  // far right of center
  else if (line_R1 < 200 and line_R2 > 200){
    if(!offTrack){
      robot.hardRight(speed);
      hardRight = true;
    }
  }

  // far left of center
  else if (line_L1 < 200 and line_L2 > 200){
    if(!offTrack){
      robot.hardLeft(speed);
      hardLeft = true;
    }
  }
  
  // no line detected
  else{
    if (hardLeft){
      robot.slightLeft(speed);
    }
    else if (hardRight){
      robot.slightRight(speed);
    }
    else{
      robot.forward(speed);
    }
    offTrack = true;
  }

  //delay(1000);
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
