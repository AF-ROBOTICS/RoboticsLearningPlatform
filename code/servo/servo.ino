#include <Servo.h>
#include <RF24.h>

//Radio object:
RF24 radio(7, 8); //CE, CSN

//radio address
const byte address[6] = "00001";
int angle = 0;

//Defining the struct for packets
//over the radio
typedef struct {
  int X = 0;
  int Y = 0;
  bool Z = false;
} infoPacket;

infoPacket newPacket;
Servo servo;

void setup(){
	Serial.begin(115200);
  servo.attach(2);
  servo.write(90);
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
  randomSeed(millis());
  if (radio.available()){
    while (radio.available()){
      radio.read(&newPacket, sizeof(infoPacket));
    }
  }
  int xVal = newPacket.X - 138;
  int yVal = newPacket.Y -130;
  int zVal = newPacket.Z;
  Serial.print("yVal: "); Serial.println(yVal);
  Serial.print("xVal: "); Serial.println(xVal);
  Serial.print("zVal: "); Serial.println(zVal);
  
 if(zVal == 1)
 {
  if (xVal < -50 && -25 < yVal < 25 && angle < 180){
      angle += 1;
      Serial.print("Increase servo.");
  }
  else if (xVal > 50 && -25 < yVal <25 && angle > 0){
    angle -= 1;
    Serial.println("Decrease servo.");
  }
 }
 Serial.print("angle:"); Serial.println(angle);
 servo.write(angle);
}
