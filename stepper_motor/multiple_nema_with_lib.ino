#include <Stepper.h> 

#define N_STEPS_PER_REV 200
#define N_MICROSTEPS_DIVISOR 1
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver

Stepper o_stepper_1(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 3,4); 
Stepper o_stepper_2(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 5,6); 
Stepper o_stepper_3(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 7,8); 




void setup() {

  // Set the maximum speed in steps per second:

  o_stepper_1.setSpeed(3000);
  o_stepper_2.setSpeed(3000);
  o_stepper_3.setSpeed(3000);

}

void loop() {

 
o_stepper_1.step(1);
o_stepper_2.step(1);
o_stepper_3.step(1);
  

}

