void setup() {
  Serial.begin(9600);           //  setup serial
}

void loop() {
  int n_a0 = analogRead(A0);  // read the input pin
  int n_a1 = analogRead(A1);  // read the input pin
  Serial.println(n_a0);          // debug value
  Serial.println(n_a1);          // debug value
}