#include <Servo.h>
#include <RF24.h>

Servo servo;
const int servoFeedback = 7;

void setup(){
	Serial.begin(115200);
  servo.attach(5);
  pinMode(servoFeedback, INPUT);
}

void loop(){
//   servo.writeMicroseconds(0);
//   delay(1000);
   servo.writeMicroseconds(1200);
//
//  delay(1000);
}
