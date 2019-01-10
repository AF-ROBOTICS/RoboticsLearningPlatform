int ldrL = A3;
int ldrC = A4;
int ldrR = A5;

void setup() {
  // put your setup code here, to run once:
  pinMode(ldrL, INPUT);
  //pinMode(ldrC, INPUT);
  //pinMode(ldrR, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int readingL = analogRead(ldrL);
  //int readingC = analogRead(ldrC);
  //int readingR = analogRead(ldrR);
  Serial.print("Left: ");
  Serial.print(readingL);
  Serial.println(" ");
//
//  Serial.print("Center: ");
//  Serial.print(readingC);
//  Serial.print(" ");
//
//  Serial.print("Right: ");
//  Serial.print(readingR);

  delay(1000); // print every second
}
