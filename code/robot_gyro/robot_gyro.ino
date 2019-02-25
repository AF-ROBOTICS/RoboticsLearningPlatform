

#include <Wire.h>
#include <Servo.h>
#include "TB6612FNG.h"
#include "drive.h"
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

void setup(){
	Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

   Serial.println("Gyroscope Test"); Serial.println("");

  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the FXAS21002C ... check your connections */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while(1);
  }

  Serial.println("FXOS8700 Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accelmag.begin(ACCEL_RANGE_4G))
  {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while(1);
  }

  sensor_t gyro_sensor, accel, mag;
  gyro.getSensor(&gyro_sensor);
  accelmag.getSensor(&accel, &mag);
  delay(500);
}

void loop(){
  sensors_event_t gevent, aevent, mevent;
  gyro.getEvent(&gevent);
  accelmag.getEvent(&aevent, &mevent);
  
  /* Display the gyro results (speed is measured in rad/s) */
  Serial.print("GYRO ");
  Serial.print("X: "); Serial.print(gevent.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gevent.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gevent.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");

  /* Display the accel results (acceleration is measured in m/s^2) */
  Serial.print("ACCEL ");
  Serial.print("X: "); Serial.print(aevent.acceleration.x, 4); Serial.print("  ");
  Serial.print("Y: "); Serial.print(aevent.acceleration.y, 4); Serial.print("  ");
  Serial.print("Z: "); Serial.print(aevent.acceleration.z, 4); Serial.print("  ");
  Serial.println("m/s^2");

  /* Display the mag results (mag data is in uTesla) */
  Serial.print("MAG ");
  Serial.print("X: "); Serial.print(mevent.magnetic.x, 1); Serial.print("  ");
  Serial.print("Y: "); Serial.print(mevent.magnetic.y, 1); Serial.print("  ");
  Serial.print("Z: "); Serial.print(mevent.magnetic.z, 1); Serial.print("  ");
  Serial.println("uT");

  Serial.println("");

  delay(500);

  
}
