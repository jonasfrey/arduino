
void setup() {

  // Set the maximum speed in steps per second:

  Serial.begin(9600);

}

void loop() {

//   int n0 = analogRead(A0);
  int n1 = analogRead(A1);
  int n2 = analogRead(A2);
  int n3 = analogRead(A3);
    Serial.print(-1000); // To freeze the lower limit
    Serial.print(" ");
    Serial.print(1000); // To freeze the upper limit
    Serial.print(" ");
    Serial.println(n1); 
    Serial.println(n2); 
    Serial.println(n3); 
}

