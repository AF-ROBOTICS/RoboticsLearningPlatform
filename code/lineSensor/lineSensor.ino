int lineL = A3;
int lineC = A4;
int lineR = A5;

void setup() {
  // put your setup code here, to run once:
  pinMode(lineL, INPUT);
  pinMode(lineC, INPUT);
  pinMode(lineR, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float outputLine_L = analogRead(lineL);
  float outputLine_C = analogRead(lineC);
  float outputLine_R = analogRead(lineR);

  Serial.print("Left: ");
  Serial.print(outputLine_L);
  Serial.print(" ");

  Serial.print("Center: ");
  Serial.print(outputLine_C);
  Serial.print(" ");

  Serial.print("Right: ");
  Serial.println(outputLine_R);

  delay(1000); // print every second
}
