#include <Stepper.h> 

#define STEPS 200

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver

Stepper stepper(STEPS, 7,6); // Pin 2 connected to DIRECTION & Pin 3 connected to STEP Pin of Driver

#define motorInterfaceType 1

int Pval = 0;

int potVal = 0;

void setup() {

  // Set the maximum speed in steps per second:

  stepper.setSpeed(3000);

}
int n_i = 0;
void loop() {
  n_i+=1;
  int b = n_i % 2;
  int n_sign = (b < 0) - (b > 0);

  stepper.step(STEPS*n_sign);
  

}

