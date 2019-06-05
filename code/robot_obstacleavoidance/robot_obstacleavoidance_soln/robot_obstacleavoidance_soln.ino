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
#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"
#include <RF24.h>

const int adir    = 4;  // direction a
const int pwma  = 6;  // HW
const int bdir    = 9;  // direction b
const int pwmb  = 10; // HW
const int speed = 200;
const int turnDelay = 1000;
const int pause = 275;
const byte irL = A0;
const byte irC = A1;
const byte irR = A2;

//Radio object:
RF24 radio(7, 8); //CE, CSN

//radio address
const byte address[6] = "00001";

//Defining the struct for packets
//over the radio
typedef struct {
  int X = 0;
  int Y = 0;
  bool Z = 0;
} infoPacket;

infoPacket newPacket;
Robot robot;

void setup(){
  robot.init(pwma, adir, pwmb, bdir, turnDelay, pause);
  Serial.begin(115200);
  //Starting up radio
//  radio.begin();
//  radio.setChannel(115);
//  radio.openReadingPipe(1, address);
//  radio.setPALevel(RF24_PA_MAX);
//  radio.setDataRate(RF24_250KBPS);
//  radio.startListening();
//  Serial.print("Radio booting...\n");
//  delay(1000);
}

void loop(){
  randomSeed(millis());
//  if (radio.available()){
//    while (radio.available()){
//      radio.read(&newPacket, sizeof(infoPacket));
//    }
//  }
//  int xVal = newPacket.X - 138;
//  int yVal = newPacket.Y -130;
//  int zVal = newPacket.Z;
  
  float outputLeft = analogRead(irL);
  float outputCenter = analogRead(irC);
  float outputRight = analogRead(irR);

  float distLeft = robot.distCalc(outputLeft);
  float distCenter = robot.distCalc(outputCenter);
  float distRight = robot.distCalc(outputRight);

  // Wall in front -> Turn
  if(distCenter < 4) {
    // no wall to the left or right
    if(distLeft > 4 && distRight > 4){
      // randomly turn left or right
      if(random(1, 3) == 1){
        robot.turnRight(speed);
      }
      else{
        robot.turnLeft(speed);
      }
    }
      // no wall to the left but there is one to the right -> turn left
     else if(distLeft > 4){
      robot.turnLeft(speed);
     }
     // no wall to the right but there is one to the left -> turn right
     else if(distRight > 4){
      robot.turnRight(speed);
     }
     // wall in front, left, and right -> turn around
     else{
      robot.turnAround(speed);
     }
  }
  // getting too close to the left wall, slight right
  else if (distLeft < 3.5){
    robot.slightRight(speed);
  }
  // getting too close to the right wall, slight left
  else if (distRight < 3.5){
    robot.slightLeft(speed);
  }
  // all is good, continue
  else {
    Serial.print("forward");
    robot.forward(speed);
//    if(yVal > 75 && -25 < xVal < 25) {
//      robot.forward(speed);
//    }
//    else if(yVal > 50 && -25 < xVal < 25){
//      robot.forward(.5*speed);
//    }
//    else if (yVal < -75 && -25 < xVal < 25){
//      robot.reverse(speed);
//    }
//    else if (yVal < -50 && -25 < xVal < 25){
//      robot.reverse(.5*speed);
//    }
//    else if (xVal < -50 && -25 < yVal < 25){
//      robot.ncTurnLeft(speed);
//    }
//    else if (xVal > 50 && -25 < yVal <25){
//      robot.ncTurnRight(speed);
//    }
//    else{
//      robot.allStop();
//    }
  }
}
