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
#include "Filter.h" // must include MegunoLink library
#include "TB6612FNG.h"
#include "Motor.h"
#include "IRDistance.h"

Robot robot;

// pin used by the left IR sensor
const int irL = A0;
const int irC = A1;
const int irR = A2;

// Sensor values
volatile uint32_t nl, nc, nr;
int32_t Left, Center, Right;

// Exponential filters
ExponentialFilter<uint32_t> ADCFilterL(20, 0);
ExponentialFilter<uint32_t> ADCFilterC(20, 0);
ExponentialFilter<uint32_t> ADCFilterR(20, 0);

// Proportional controller values
#define DESIRED_DIST 172
int32_t SetPoint = DESIRED_DIST;
int32_t WallError;

// Motor values
#define PWMNOMINAL 150
#define SWING 50
#define PWMIN (PWMNOMINAL-SWING)
#define PWMAX (PWMNOMINAL+SWING)
int32_t UL, UR;    // speed of a wheel 0 - 255
uint32_t Kp = 3;  // porportional controller multiplier

void setup() {
  Serial.begin(9600);

  robot.Motor_Init();
  
  // initialize filter
  uint32_t rawL, rawC, rawR;
  rawL = analogRead(irL);
  rawC = analogRead(irC);
  rawR = analogRead(irR);

  ADCFilterL.SetCurrent(rawL);
  ADCFilterC.SetCurrent(rawC);
  ADCFilterR.SetCurrent(rawR);
}

void loop() {
  // read raw values of IR sensors
  uint32_t rawL, rawC, rawR;
  rawL = analogRead(irL);
  rawC = analogRead(irC);
  rawR = analogRead(irR);

  // filter raw values of IR sensors
  ADCFilterL.Filter(rawL);
  ADCFilterC.Filter(rawC);
  ADCFilterR.Filter(rawR);

  nl = ADCFilterL.Current();
  nc = ADCFilterC.Current();
  nr = ADCFilterR.Current();

  Left = LeftConvert(nl);
  Center = CenterConvert(nc);
  Right = RightConvert(nr);

  if((Left>DESIRED_DIST)&&(Right>DESIRED_DIST)){
    SetPoint = (Left+Right)/2;
  }else{
    SetPoint = DESIRED_DIST;
  }

  if(Left < Right){
    WallError = Left - SetPoint;
  }else{
    WallError = SetPoint - Right;
  }
  UR = PWMNOMINAL+Kp*WallError;
  UL = PWMNOMINAL-Kp*WallError;

  if(UR < PWMIN) UR = PWMIN;
  if(UR > PWMAX) UR = PWMAX;
  if(UL < PWMIN) UL = PWMIN;
  if(UL > PWMAX) UL = PWMAX;
  robot.Motor_Forward(UL, UR);
}
