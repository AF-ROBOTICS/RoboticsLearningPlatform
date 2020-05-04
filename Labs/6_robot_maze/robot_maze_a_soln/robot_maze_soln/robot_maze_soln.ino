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
#include "Classifier.h"

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
#define SWING 20
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)

// Proportional gain constant
const int Kp = 3;
const int Kp2 = 2;


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

int Turn = 0;
int NextState = 0;
int NextSquare = 1;
int Time = 0;
const int SQUARE_TIME = 160;
enum scenario next_state = Error;
int TurnTime = 0;
const int TURN = 160;

void loop() {
  int error, setPoint, pwmR, pwmL;
  enum scenario wall_state = Error;
  
  // Read distance (in mm) for each sensor
  int16_t distR = sensorR.getDist() + OFFSET;
  int16_t distC = sensorC.getDist() + OFFSET;
  int16_t distL = sensorL.getDist() + OFFSET;

  wall_state = Classify(distL, distC, distR);

  // Error state, pause
  if(wall_state == Error || wall_state == LeftTooClose || wall_state == RightTooClose || wall_state == CenterTooClose){
    Motor_Stop();
  }

  // semaphores: NextSquare - move to next square when true
  //             Turn - begin turn when true
  //             NextState - next state has been recorded when true
  
  // code to move to next square, obtain next state when halfway to
  // next square, and then set turn variables when in next square
  // if turn is about to occur
  if(NextSquare){
    
    // walls on both sides, so use proportional controller to stay in middle
    if((wall_state == Straight && next_state == Straight) || (wall_state == Blocked && next_state == Blocked)){
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
      // Proportional controller
      pwmR = PWMNOMINAL+Kp*error;
      pwmL = PWMNOMINAL-Kp*error;
    
      // Bounds checking
      if(pwmR < PWMMIN) pwmR = PWMMIN;
      if(pwmR > PWMMAX) pwmR = PWMMAX;
      if(pwmL < PWMMIN) pwmL = PWMMIN;
      if(pwmL > PWMMAX) pwmL = PWMMAX;
    }
    
    // gap, so stick to wall
    else{
      if((wall_state == LeftJoint) || (wall_state == LeftTurn)){
        error = DESIRED - distR;
      }
      else if((wall_state == RightJoint) || (wall_state == RightTurn)){
        error = distL - DESIRED;
      }
      else{
        error = 0;
      }
      pwmR = PWMNOMINAL + Kp2*error;
      pwmL = PWMNOMINAL - Kp2*error;
      Motor_Forward(pwmL, pwmR);
    }
    
    // when halfway to next square, gather next state information
    if((Time >= SQUARE_TIME/2) && !NextState){
      next_state = wall_state;
      NextState = 1;
    }
    
    // when in next square, if turn, set turn variables
    // reset tachometer step counter for turn or next square math
    // distance = (change in steps*wheel circ)/(360 pulses per rotation)
    else if((Time >= SQUARE_TIME) && NextState){
      Time = 0;
      NextState = 0;
      // if a turn is available, determine if turn is necessary
      // currently not smart: turns in direction available with left
      // as first choice
      if(next_state == Blocked){
        TurnTime = 2*TURN;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == LeftTurn){
        TurnTime = TURN;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == RightTurn){
        TurnTime = TURN*3;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == TeeJoint){
        TurnTime = TURN*3;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == CrossRoad){
        TurnTime = TURN;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == LeftJoint){
        TurnTime = 0;
        NextSquare = 0;
        Turn = 1;
      }
      else if(next_state == RightJoint){
        TurnTime = 0;
        NextSquare = 0;
        Turn = 1;
      }
      // if no turn available move to next square
      else if(next_state == Straight){
        NextSquare = 1;
        Turn = 0;
      }
    }
  }
  // begin turn
  else if(Turn){
    Motor_Left(PWMNOMINAL, PWMNOMINAL);
    if(Time > TurnTime){
      Turn = 0;
      Motor_Stop();
      NextSquare = 1;
    }
  }
  Time++;
}
