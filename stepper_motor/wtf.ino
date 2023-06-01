
#include <Stepper.h> 

#define N_STEPS_PER_REV 200
#define N_MICROSTEPS_DIVISOR 1
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver

Stepper o_stepper_z(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 3,4); 
Stepper o_stepper_x(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 5,6); 
Stepper o_stepper_y(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, 7,8); 


void setup() {

  // Set the maximum speed in steps per second:
  Serial.begin(9600);

  o_stepper_z.setSpeed(3000);
  o_stepper_x.setSpeed(3000);
  o_stepper_y.setSpeed(3000);

}

double f_n_speed_signed_nor(
    int n_speed
){

    
  double n_nor = (((float)n_speed / 1024.0) - .5) * 2.;
  return n_nor;//returns from -1.0 to 1.0 

}
double f_n_speed_signed_nor_special(
    int n_speed
){
    double n_snor = 0.;
    if(n_speed > 960){
        n_snor = 1.;
    }
    if(n_speed < 500){
        n_snor = -1.;
    }
    return n_snor;
}
void loop() {

    int n_x = analogRead(A1);
    int n_y = analogRead(A2);
    int n_z = analogRead(A3);

    double n_x_snor = f_n_speed_signed_nor(n_x);
    double n_y_snor = f_n_speed_signed_nor(n_y);
    double n_z_snor = f_n_speed_signed_nor_special(n_z);

    o_stepper_x.setSpeed(((int)(abs(n_x_snor)*2000.))+1);
    o_stepper_y.setSpeed(((int)(abs(n_y_snor)*2000.))+1);
    o_stepper_z.setSpeed(500);

    if(abs(n_x_snor) > 0.1){
        int n_step_x = (n_x_snor > 0) - (n_x_snor < 0);
        o_stepper_x.step(n_step_x);
    }
    if(abs(n_y_snor) > 0.1){
        int n_step_y = (n_y_snor > 0) - (n_y_snor < 0);
        o_stepper_y.step(n_step_y);
    }
    if(abs(n_z_snor) > 0.1){
        int n_step_z = (n_z_snor > 0) - (n_z_snor < 0);
        o_stepper_z.step(n_step_z);
    }
    

    Serial.println(n_z_snor);

    // Serial.print(-1000); // To freeze the lower limit
    // Serial.print(" ");
    // Serial.print(1000); // To freeze the upper limit
    // Serial.print(" ");
    // Serial.println(n1); 
    // Serial.println(n2); 
    // Serial.println(n3); 
}

