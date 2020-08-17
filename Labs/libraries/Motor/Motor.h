/* 
 *  Filename: drive.h
 *  Author: Capt Steven Beyer
 *  Created: 6 Mar 2019
 *  Description: Header file that provides functions to drive the 
 *    DFECBot.
 *		L_DIR  = 2;  // direction b
 *		L_PWM  = 3;  // PWM at 490 Hz
 *		R_DIR  = 4;  // direction a
 *		R_PWM  = 5;  // PWM at 980 Hz

 *  
 *  Required Files:
 *    Libraries : none
 *    Packages  : none
 *    Files     : none
 */


const int L_DIR  = 2;  // direction b
const int L_PWM  = 3;  // PWM at 490 Hz
const int R_DIR  = 4;  // direction a
const int R_PWM  = 5;  // PWM at 980 Hz

// ------------Motor_Init------------
// Initialize GPIO pins
// Used to control the direction of the motors and
// PWM signal applied to the motors.
// The motors are initially stopped
// Input: none
// Output: none
void Motor_Init(){
  // Initialize pins
  pinMode(R_PWM, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  
  // stop the motors
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(){
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Forward(int leftDuty, int rightDuty){
  // Set both motors forward
  digitalWrite(L_DIR,HIGH);
  digitalWrite(R_DIR,HIGH);

  // set PWM for each motor
  analogWrite(L_PWM, leftDuty);
  analogWrite(R_PWM, rightDuty);
}

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and the right wheel
// backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Right(int leftDuty, int rightDuty){
  // Set left wheel forward and right wheel backward
  digitalWrite(L_DIR,HIGH);
  digitalWrite(R_DIR,LOW);

  // set PWM for each motor
  analogWrite(L_PWM, leftDuty);
  analogWrite(R_PWM, rightDuty);
}

// ------------Motor_Left------------
// Turn the robot to the left by running the
// left wheel backward and the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Left(int leftDuty, int rightDuty){
  // Set left wheel backward and right wheel forward
  digitalWrite(L_DIR,LOW);
  digitalWrite(R_DIR,HIGH);

  // set PWM for each motor
  analogWrite(L_PWM, leftDuty);
  analogWrite(R_PWM, rightDuty);
}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 255)
//        rightDuty duty cycle of right wheel (0 to 255)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Backward(int leftDuty, int rightDuty){
  // Set both wheels backward
  digitalWrite(L_DIR,LOW);
  digitalWrite(R_DIR,LOW);

  // set PWM for each motor
  analogWrite(L_PWM, leftDuty);
  analogWrite(R_PWM, rightDuty);
}
