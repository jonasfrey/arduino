void setup(){
    int n_pin_builtin_led = 13;
    pinMode(n_pin_builtin_led, OUTPUT);
}
void loop(){
    digitalWrite(n_pin_builtin_led, 1);
    delay(100);
    digitalWrite(n_pin_builtin_led, 0);
    delay(1000);
}