/* 
 *  Filename: robot_linefollowing.ino
 *  Author: Capt Steven Beyer
 *  Created: 23 April 2020
 *  Description: Example Arduino Sketch that prints 
 *    values from the DFECBot's QTR-8RC line sensor array.
 *  
 *  Source: Adapted from QTRRCExample.ino and QTRSensors.h
 *    https://github.com/pololu/qtr-sensors-arduino
 *    
 *  Assignment:
 *    1) Use a ruler to confirm the accuracy of the line sensor 
 *    read function
 *    2) Program the DFECBot to follow a line
 *    Note: Remove all print statements/delays when running your line following
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : Motor.h, QTR-8RC.h
 */

#include "QTR-8RC.h"
#include "Motor.h"

/******************** Sensor variables *****************/
QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

/******************** Controller variables *****************/
#define PWMNOMINAL 100
#define SWING 20
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)

// Proportional gain constant
const int Kp = 3;

void setup()
{
  Motor_Init();
  
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){6, 7, 8, 9, 10, 11, 12, 13}, SensorCount);
  //qtr.setEmitterPin(5);

  delay(500);

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
}

void loop()
{
  int pwmR, pwmL;
  
  // read calibrated sensor values and obtain a measure of the line position
  // with a position of 0.1 mm relative to the center of the line 
  // -33.2 mm to 33.2 mm
  int16_t position = qtr.readLineBlack(sensorValues);

  // Proportional controller
  pwmR = PWMNOMINAL - Kp*position;
  pwmL = PWMNOMINAL + Kp*position;

  // Bounds checking
  if(pwmR < PWMMIN) pwmR = PWMMIN;
  if(pwmR > PWMMAX) pwmR = PWMMAX;
  if(pwmL < PWMMIN) pwmL = PWMMIN;
  if(pwmL > PWMMAX) pwmL = PWMMAX;
  
  Motor_Forward(pwmL, pwmR);
  
}
