#include <CheapStepper.h>

#include <Stepper.h>

/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground through 220 ohm resistor (or use the
    built-in LED on most Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/StateChangeDetection
*/

// this constant won't change:
const int  buttonPin = 2;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:

int btn_pressed = 0;         // current state of the button

// Define number of steps per rotation:
const int stepsPerRevolution = 2048;

int delta_ts_switch = 0; 
int delta_first_second_ts = 0; 
int ts = 0;
int delta_limit = 500;
int first_ts = 0; 
int second_ts = 0; 
int delta_ts_switch_changed = 0; 
int last_btn_pressed = 0; 
int led_light_length_ms = 30; 

int steps[ 4 ][ 4 ] = { 
  { 1, 0, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 0, 1 }, 
  };

int steps_index = 0; 

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
    // Set the speed to 5 rpm:
  //myStepper.setSpeed(15);
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
        
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
  // read the pushbutton input pin:
  btn_pressed = digitalRead(buttonPin);
  //Serial.println(btn_pressed);

  if(btn_pressed && !last_btn_pressed){
    ts = millis();
    delta_ts_switch_changed = 0;
  }
    
  if(btn_pressed && last_btn_pressed){
    
    int delta_ts_now = millis()-ts; 
  
      if(delta_ts_now > delta_limit && !delta_ts_switch_changed){
        delta_ts_switch_changed = 1; 
        delta_ts_switch = (1+delta_ts_switch) % 2;

        }
    }




  if(delta_ts_switch){
  digitalWrite(4, 1);
    
//myStepper.step(stepsPerRevolution);

    if(btn_pressed && !last_btn_pressed){
      if(first_ts != 0){
        if(second_ts != 0){
          first_ts  = 0; 
          second_ts = 0; 
          delta_first_second_ts = 0; 
          first_ts = millis();
          }else{
            second_ts = millis();
            delta_first_second_ts = second_ts - first_ts; 
          }
        }else{
          first_ts = millis();
          
          }
      }

      //Serial.println(delta_first_second_ts);

if(first_ts == 0){
  digitalWrite(5, 0);
}else{
  digitalWrite(5, 1);
}
if(second_ts == 0){
  digitalWrite(6, 0);
}else{
  digitalWrite(6, 1);
}


if(millis() % delta_first_second_ts < led_light_length_ms){
  digitalWrite(3, 1);
  }else{
  digitalWrite(3, 0);
    }



      
   }else{

      digitalWrite(4, 0);
   }


Serial.println((micros()));
  
  if(int(micros()/100) % (int((delta_first_second_ts*1000/1024))/100)== 0){
    Serial.println("step");
    steps_index = (steps_index+1)%4;
      for (int i = 0; i < 4; i++) {
      digitalWrite(8+i, steps[steps_index][i]);
    }
  }


        
   last_btn_pressed = btn_pressed; 
     //Serial.println(delta_first_second_ts);

}