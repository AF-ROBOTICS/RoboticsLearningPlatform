const byte irL = A0;
const byte irC = A1;
const byte irR = A2;

void setup() {
  // put your setup code here, to run once:
  pinMode(irL, INPUT);
  pinMode(irC, INPUT);
  pinMode(irR, INPUT);

  Serial.begin(9600);
}

float distCalc(float sensorVal){
    return 11.5402*pow(0.995796, sensorVal);
  }

void loop() {
  // put your main code here, to run repeatedly:
  float outputIR_L = analogRead(irL);
  float outputIR_C = analogRead(irC);
  float outputIR_R = analogRead(irR);

  float distL = distCalc(outputIR_L);
  float distC = distCalc(outputIR_C);
  float distR = distCalc(outputIR_R);

  Serial.print("Left: ");
  Serial.print(distL);
  Serial.print(" ");

  Serial.print("Center: ");
  Serial.print(distC);
  Serial.print(" ");

  Serial.print("Right: ");
  Serial.println(distR);
}
