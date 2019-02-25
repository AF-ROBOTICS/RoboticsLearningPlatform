

#include <Wire.h>
#include <Servo.h>
#include "TB6612FNG.h"
#include "drive.h"
#include <RF24.h>

const int adir    = 4;  // direction a
const int pwma  = 6;  // HW
const int bdir    = 9;  // direction b
const int pwmb  = 3; // HW
const int speed = 200;
const int turnDelay = 1000;
const int pause = 275;
const int servoPin = 5;
int angle = 90;

//Radio object:
RF24 radio(7, 8); //CE, CSN

//radio address
const byte address[6] = "00001";

//Defining the struct for packets
//over the radio
typedef struct {
  int X = 0;
  int Y = 0;
  bool Z = false;
  bool C = false;
} infoPacket;

infoPacket newPacket;
Robot robot;
Servo servo;

void setup(){
	Serial.begin(115200);
 
  robot.init(pwma, adir, pwmb, bdir, turnDelay, pause);
  servo.attach(servoPin);
  servo.write(angle);
  // Starting up radio
  radio.begin();
  radio.setChannel(115);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  Serial.print("Radio booting...\n");
  delay(1000);
}

void loop(){
  if (radio.available()){
    while (radio.available()){
      radio.read(&newPacket, sizeof(infoPacket));
    }
  }
  int xVal = newPacket.X - 138;
  int yVal = newPacket.Y -130;
  int zVal = newPacket.Z;
  int cVal = newPacket.C;
  Serial.print("yVal: "); Serial.println(yVal);
  Serial.print("xVal: "); Serial.println(xVal);
  Serial.print("zVal: "); Serial.println(zVal);
  Serial.print("cVal: "); Serial.println(cVal);

  if(zVal == 0){
  if(yVal > 75 && -25 < xVal < 25) {
    robot.forward(speed);
  }
  else if(yVal > 50 && -25 < xVal < 25){
    robot.forward(.5*speed);
  }
  else if (yVal < -75 && -25 < xVal < 25){
    robot.reverse(speed);
  }
  else if (yVal < -50 && -25 < xVal < 25){
    robot.reverse(.5*speed);
  }
  else if (xVal < -50 && -25 < yVal < 25){
    robot.ncTurnLeft(speed);
  }
  else if (xVal > 50 && -25 < yVal <25){
    robot.ncTurnRight(speed);
  }
  else{
    robot.allStop();
  }
  }
  else if(zVal == 1){
    robot.allStop();
        if (xVal < -50 && -25 < yVal < 25 && angle < 180){
        angle += 1;
        Serial.print("Increase servo.");
    }
    else if (xVal > 50 && -25 < yVal <25 && angle > 0){
      angle -= 1;
      Serial.println("Decrease servo.");
    }
  }
  if(cVal == 1){
    angle = 90;
  }
  servo.write(angle);
}
