
void setup() {

  // Set the maximum speed in steps per second:

  Serial.begin(9600);

}

void loop() {

    int n0 = analogRead(A0);
    int n1 = analogRead(A1);
    int n2 = analogRead(A2);
    int n3 = analogRead(A3);
    int n4 = analogRead(A4);
    int n5 = analogRead(A5);
    Serial.print(-1000); // To freeze the lower limit
    Serial.print(" ");
    Serial.print(1000); // To freeze the upper limit
    Serial.print(" ");

    Serial.print("A0:");
    Serial.print(n0);
    Serial.print(",");
    Serial.print("A1:");
    Serial.print(n1);
    Serial.print(",");
    Serial.print("A2:");
    Serial.print(n2);
    Serial.print(",");
    Serial.print("A3:");
    Serial.print(n3);
    Serial.print(",");
    Serial.print("A4:");
    Serial.print(n4);
    Serial.print(",");
    Serial.print("A5:");
    Serial.println(n5);
    
}

