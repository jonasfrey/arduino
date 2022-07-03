
const int n_receiver_pin = 7;

void setup(){
  Serial.begin(9600);
  
}

void loop(){
    int n_input_value = analogRead(A7);

    Serial.println(n_input_value);
}