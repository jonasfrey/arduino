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

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

int n_pin_step = 12;
int n_pin_dir = 11;
// the setup function runs once when you press reset or power the board
void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    // Serial.begin(115200); // stepper wont work after using Serial.begin ... wtf is going on
    pinMode(n_pin_step, OUTPUT);
    pinMode(n_pin_dir, OUTPUT);
}

// the loop function runs over and over again forever
int n_i = 0;
void loop()
{
    n_i+=1;
    
    digitalWrite(n_pin_dir, (n_i%2)); // turn the LED on (HIGH is the voltage level)

    for (int n = 0; n < 200; n += 1)
    {
        digitalWrite(n_pin_step, 1); // turn the LED on (HIGH is the voltage level)
        delayMicroseconds(300);              // wait for a second
        digitalWrite(n_pin_step, 0);  // turn the LED off by making the voltage LOW
        delayMicroseconds(300);              // wait for a second
    }


    delay(1000); // wait for a second
}