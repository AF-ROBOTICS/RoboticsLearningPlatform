#include "WiiChuck.h"
#include <RF24.h>
#include <SPI.h>


//Defining packet struct for radio transmission
typedef struct {
  int X = 0;
  int Y = 0;
  bool Z = false;
  bool C = false;
} infoPacket;

//Initializing packet
infoPacket newPacket;


//Radio initialization: pins and address
RF24 radio(7, 8);
const byte address[6] = "00001";

//Initializing new WiiChuck object
WiiChuck nunchuck;

void setup() {

  //rev up those nunchucks
  nunchuck.begin();
  //Start serial for debug
  Serial.begin(115200);

  //Starting up radio
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  
}

void loop() {
  //Call for new data from nunchuck
  nunchuck.update();
  //Set attributes of newPacket to nunchuck readings
  newPacket.X = nunchuck.joyX;
  newPacket.Y = nunchuck.joyY;
  newPacket.Z = nunchuck.buttonZ;
  newPacket.C = nunchuck.buttonC;
  Serial.print("yVal: "); Serial.println(newPacket.Y);
  Serial.print("xVal: "); Serial.println(newPacket.X);
  Serial.print("zVal: "); Serial.println(newPacket.Z);
  Serial.print("cVal: "); Serial.println(newPacket.C);
  //Still gonna send it
  radio.write(&newPacket, sizeof(newPacket));
  delay(100);
}
