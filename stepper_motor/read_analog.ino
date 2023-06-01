
void setup() {

  // Set the maximum speed in steps per second:

  Serial.begin(9600);

}

void loop() {

//   int n0 = analogRead(A0);
  int n1 = analogRead(A1);
  int n2 = analogRead(A2);
  int n3 = analogRead(A3);
  int n4 = analogRead(A4);
  int n5 = analogRead(A5);

  Serial.println(n1);
  Serial.println(n2);
  Serial.println(n3);
  Serial.println(n4);
  Serial.println(n5);
}

