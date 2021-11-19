/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

void blink_pulse_width_modulated(int freq_microseconds, int width_percentage, int pin_number){
  
  int delay_one = freq_microseconds / 100 * width_percentage;
  int delay_two = freq_microseconds - delay_one;
  
  digitalWrite(pin_number, HIGH);  
  delayMicroseconds(delay_one);                     
  
  digitalWrite(pin_number, LOW);   
  delayMicroseconds(delay_two);  
}
int time_int = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  int potentiometer_value = analogRead(A0);
  Serial.println(potentiometer_value);


  time_int = time_int + 1;
  
  int slow_increase = int(float(time_int)*0.1)%100;
  float freq = 0.1/1024 * potentiometer_value;
  int sine_in_de_crease = int(sin( (time_int*freq) )*50)+50;
  Serial.println(slow_increase);
  blink_pulse_width_modulated(1000, int(100/1024*potentiometer_value), 13);


  
}