#include <SPI.h>
#include <RF24.h>

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

void setup() {
  //Begin serial port
  Serial.begin(115200);

  //Starting up radio
  radio.begin();
  radio.setChannel(115);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  Serial.print("Radio booting...\n");
  delay(1000);

}

void loop() {

  if (radio.available()){
    while (radio.available()){
      radio.read(&newPacket, sizeof(infoPacket));
    }

    //Serial output for debugging
    Serial.print("\nX: ");
    Serial.print(newPacket.X - 138);
    Serial.print("\n");
    Serial.print("Y: ");
    Serial.print(newPacket.Y - 130);
    Serial.print("\n");
    Serial.print("Z: ");
    Serial.print(newPacket.Z);
    Serial.print("\n");
  }
}
