/* 
 *  Filename: Robot Maze Level 2
 *  Author: Capt Steven Beyer
 *  Created: 4 May 2020
 *  Description: The second level of the maze requires the robot 
 *    to follow a white line to the end of the maze, detect the wall, 
 *    turn around, and follow the line to the beginning of the maze, 
 *    detect the wall, turn around, and stop.
 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : Motor.h, QTR-8RC.h, SharpDistSensor.h
 */
#include <SharpDistSensor.h>
#include "QTR-8RC.h"
#include "Motor.h"

/******************** Line Sensor variables *****************/
QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

/******************** Distance Sensor variables *************/
const byte irC = A1;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Create an object instance of the SharpDistSensor class for each sensor
SharpDistSensor sensorC(irC, medianFilterWindowSize);

#define WALL_TOO_CLOSE 100
#define OFFSET 50     // dist from IR sensor to center of robot in mm

/******************** Controller variables ******************/
#define PWMNOMINAL 100
#define SWING 20
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)

// Proportional gain constant
const int Kp = 3;

/******************** System variables *********************/
#define NUMTURNS 2
int num_turns = 0;

// semaphores
int Turn = 0;
int Stop = 0;

// timers
int TurnTime = 0;
const int TURN = 160;

void setup()
{
  Motor_Init();
  
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){6, 7, 8, 9, 10, 11, 12, 13}, SensorCount);
  //qtr.setEmitterPin(5);
  sensorC.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);

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
  
  // read calibrated line sensor values and obtain a measure of the line position
  // with a position of 0.1 mm relative to the center of the line 
  // -33.2 mm to 33.2 mm
  int16_t position = qtr.readLineBlack(sensorValues);

  // read center distance sensor
  unsigned int distC = sensorC.getDist() + OFFSET;

  // not turning or stoping so use line following
  if (!Turn && !Stop){
    
    // determine if turn
    if(distC < WALL_TOO_CLOSE){
      Turn = 1;
      num_turns++;
    }
    
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
  // turning
  else if (Turn && !Stop){
    TurnTime++;
    Motor_Left(PWMNOMINAL, PWMNOMINAL);
    
    // if turn is complete
    if(TurnTime > TURN){
      Turn = 0;
      Motor_Stop();
      TurnTime = 0;
      
      // if we have reached the number of turns
      if(num_turns == NUMTURNS){
        Stop = 1;
      }
    }
  }
  // at second wall so stop
  else if (Stop){
    Motor_Stop();
  }  
}
