#include <Stepper.h> 

#define STEPS 200
#define n_pin_ain_potentiometer_x A4
#define n_pin_ain_potentiometer_y A5
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver

Stepper stepper(STEPS, 7,6); // Pin 2 connected to DIRECTION & Pin 3 connected to STEP Pin of Driver

#define motorInterfaceType 1

int Pval = 0;

int potVal = 0;

void setup() {

  // Set the maximum speed in steps per second:

  Serial.begin(9600);
  stepper.setSpeed(3000);

}

void loop() {

  int n_x = analogRead(n_pin_ain_potentiometer_x);
  int n_y = analogRead(n_pin_ain_potentiometer_y);

  double n_x_nor = (((float)n_x / 1024.0) - .5) * 2.;
  double n_y_nor = (((float)n_y / 1024.0) - .5) * 2.;
  double n_speed_nor_x = abs(n_x_nor);
  double n_speed_nor_y = abs(n_y_nor);

  if(n_speed_nor_x > 0.9){
    n_speed_nor_x = 1.0;
  }
if(n_speed_nor_y > 0.9){
    n_speed_nor_y = 1.0;
  }
  int n_speed = (int)(n_speed_nor_x*3000.)+1;
  stepper.setSpeed(n_speed);
  int n_dir = 1.;
  if(n_x_nor > 0.){
    n_dir = -1;
  }
  if(n_speed_nor_x > 0.){
    stepper.step(n_dir * 1);
  }
 
  

}

