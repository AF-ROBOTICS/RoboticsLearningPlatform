// Global variable
static int LED_Pin = 11;

void setup() {
  // Set LED Pin as an output pin
  pinMode(LED_Pin, OUTPUT);
}

void loop() {
  // pulse the LED on (dim to bright)
  for(int i = 0; i < 255; i++){
    analogWrite(LED_Pin, i);
    delay(5);
  }

  for(int i = 255; i >= 0; i--){
    analogWrite(LED_Pin, i);
    delay(5);
  }
}
