

#include <Wire.h>
#include "TB6612FNG.h"
#include "drive.h"

const int adir    = 4;  // direction a
const int pwma  = 6;  // HW
const int bdir    = 9;  // direction b
const int pwmb  = 10; // HW
const int speed = 50;
const int turnDelay = 1000;
const int pause = 250;
const byte lineC = A5;

Robot robot;

void setup(){
  robot.init(pwma, adir, pwmb, bdir, turnDelay, pause);
	Serial.begin(9600);
  //pinMode(lineC, INPUT);
  delay(1000);
}

void loop(){

  // get line sensor value
    float outputLine_C = analogRead(lineC);
    Serial.print("Center: ");
    Serial.println(outputLine_C);
    
    // if on board (ouputLine_C > 100)
    if(outputLine_C > 60) {
      robot.forward(speed);
    }
    else{
      robot.turnAround(speed);
    }
}
