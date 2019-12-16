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
#include <QTRSensors.h>
#include "TB6612FNG.h"
#include "Motor.h"
 
QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

#define PWMNOMINAL 150
#define SWING 50
#define PWMIN (PWMNOMINAL-SWING)
#define PWMAX (PWMNOMINAL+SWING)

int32_t UL, UR; 

uint32_t Kp = 20;

// create an instance of the robot class from drive.h
Robot robot;

void setup() {
  
  robot.Motor_Init();
  
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){6, 7, 8, 9, 10, 11, 12, 13}, SensorCount);
  qtr.setEmitterPin(1);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();

  UR = UL = PWMNOMINAL;
  
  delay(1000);
}

void loop() {
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  // compute our "error" from the line position.  We will make it so that the error is
  // zero when the middle sensor is over the line, because this is our goal.  Error
  // will range from -2500 to +2500.  If we have sensor 0 on the left and sensor 7 on
  // the right,  a reading of -2500 means that we see the line on the left and a reading
  // of +2500 means we see the line on the right.
  int error = position - 2500;

  UR = PWMNOMINAL - Kp*error;
  UL = PWMNOMINAL + Kp*error;

  if(UR < PWMIN) UR = PWMIN;
  if(UR > PWMAX) UR = PWMAX;
  if(UL < PWMIN) UL = PWMIN;
  if(UL > PWMAX) UL = PWMAX;
  robot.Motor_Forward(UL, UR);
}
