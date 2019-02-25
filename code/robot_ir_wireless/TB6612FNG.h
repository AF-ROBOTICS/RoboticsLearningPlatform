

#ifndef __TB6612FNG_H__
#define __TB6612FNG_H__

// #include <Arduino.h>
#include <stdint.h>

/**
 * ATmega328P, pwm works on pins 3, 5, 6, 9, 10, and 11 at 490/980Hz
 */
class MotorDriver {
public:

  MotorDriver(void){
    ;
  }
  
  void init(int pwmA, int adir, int pwmB, int bdir, int reset=-1){
    pwmA_pin = pwmA;
    pwmB_pin = pwmB;
    Adir_pin = adir;
    Bdir_pin = bdir;
    reset_pin = reset;

    pinMode(pwmA_pin, OUTPUT);
    pinMode(pwmB_pin, OUTPUT);
    pinMode(Adir_pin, OUTPUT);
    pinMode(Bdir_pin, OUTPUT);

    if(reset_pin){
      pinMode(reset_pin, OUTPUT);
      digitalWrite(reset_pin, HIGH);
    }
  }

  void begin(){
    if(reset_pin){
      digitalWrite(reset_pin, LOW);
      delay(100);
      digitalWrite(reset_pin, HIGH);
    }
  }

  void motorRForward(uint8_t speed){
    digitalWrite(Adir_pin,HIGH);
    analogWrite(pwmA_pin, speed);
  }

  void motorLForward(uint8_t speed){
    digitalWrite(Bdir_pin,HIGH);
    analogWrite(pwmB_pin, speed);
  }

  void motorRReverse(uint8_t speed){
    digitalWrite(Adir_pin,LOW);
    analogWrite(pwmA_pin, speed);
  }

  void motorLReverse(uint8_t speed){
    digitalWrite(Bdir_pin,LOW);
    analogWrite(pwmB_pin, speed);
  }

  /**
   * Stop is defined as PWM set to zero.
   */
  void stopBothMotors(){
    stopMotorR();
    stopMotorL();
  }

  void stopMotorR(){
//    digitalWrite(A0_pin,HIGH);
//    digitalWrite(A1_pin,HIGH);
    analogWrite(pwmA_pin, 0);
  }

  void stopMotorL(){
//    digitalWrite(B0_pin,HIGH);
//    digitalWrite(B1_pin,HIGH);
    analogWrite(pwmB_pin, 0);
  }



//  /**
//   * Coast is defined as a high impedance state. No current runs through motors
//   * and they just spin.
//   */
//  void coastBothMotors(){
//    motor0Coast();
//    motor1Coast();
//  }
//
//  void motor0Coast(){
//    digitalWrite(A0_pin,LOW);
//    digitalWrite(A1_pin,LOW);
//    analogWrite(pwmA_pin, 255);
//  }
//
//  void motor1Coast(){
//    digitalWrite(B0_pin,LOW);
//    digitalWrite(B1_pin,LOW);
//    analogWrite(pwmB_pin, 255);
//  }

protected:

  int pwmA_pin, pwmB_pin, Adir_pin, Bdir_pin, reset_pin;
};

#endif
