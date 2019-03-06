/* 
 *  
 *  Filename: TB6612FNG.h
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
 *  Description: Header file that handles all of the GPIO to motor communication
 *  
 */


#ifndef __TB6612FNG_H__
#define __TB6612FNG_H__

//#include <stdint.h>

// pins to connect motor to.
const int Rdir    = 4;  // direction a
const int pwmR  = 6;  // HW
const int Ldir    = 9;  // direction b
const int pwmL  = 3; // HW

/**
 * ATmega328P, pwm works on pins 3, 5, 6, 9, 10, and 11 at 490/980Hz
 */
class MotorDriver {
public:

  MotorDriver(void){
    ;
  }

  // initialize pin numbers and modes
  void init(int reset=-1){
    pwmR_pin = pwmR;
    pwmL_pin = pwmL;
    Rdir_pin = Rdir;
    Ldir_pin = Ldir;
    reset_pin = reset;

    pinMode(pwmR_pin, OUTPUT);
    pinMode(pwmL_pin, OUTPUT);
    pinMode(Rdir_pin, OUTPUT);
    pinMode(Ldir_pin, OUTPUT);

    if(reset_pin){
      pinMode(reset_pin, OUTPUT);
      digitalWrite(reset_pin, HIGH);
    }
  }

  // reset the motor driver
  void begin(){
    if(reset_pin){
      digitalWrite(reset_pin, LOW);
      delay(100);
      digitalWrite(reset_pin, HIGH);
    }
  }

  // Move the right motor forward
  void motorRForward(uint8_t speed){
    digitalWrite(Rdir_pin,HIGH);
    analogWrite(pwmR_pin, speed);
  }

  // Move the left motor forward
  void motorLForward(uint8_t speed){
    digitalWrite(Ldir_pin,HIGH);
    analogWrite(pwmL_pin, speed);
  }

  // Move the right motor backward
  void motorRReverse(uint8_t speed){
    digitalWrite(Rdir_pin,LOW);
    analogWrite(pwmR_pin, speed);
  }

  // Move the left motor forward
  void motorLReverse(uint8_t speed){
    digitalWrite(Ldir_pin,LOW);
    analogWrite(pwmL_pin, speed);
  }

  // Stop both motors
  void stopBothMotors(){
    stopMotorR();
    stopMotorL();
  }

  // Stop right motor
  void stopMotorR(){
    analogWrite(pwmR_pin, 0);
  }

  // Stop left motor
  void stopMotorL(){
    analogWrite(pwmL_pin, 0);
  }

protected:
  int pwmR_pin, pwmL_pin, Rdir_pin, Ldir_pin, reset_pin;
};

#endif
