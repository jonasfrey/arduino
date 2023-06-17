
//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int n_steps_per_rev = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper o_stepper_x = Stepper(n_steps_per_rev, 8, 10, 9, 11);
Stepper o_stepper_y = Stepper(n_steps_per_rev, 2, 4, 3, 5);
// Stepper o_stepper_x = Stepper(n_steps_per_rev, 8, 9, 10, 11);
// Stepper o_stepper_y = Stepper(n_steps_per_rev, 2, 3, 4, 5);

void setup() {
    // Serial.begin(9600);
    // Nothing to do (Stepper Library sets pins as outputs)
}
double f_n_speed_signed_nor(
    int n_speed
){

  double n_nor = (((float)n_speed / 1024.0) - .5) * 2.;
  return n_nor;//returns from -1.0 to 1.0 

}
void loop() {

    int n_x = analogRead(A5);
    int n_y = analogRead(A4);

    double n_x_snor = f_n_speed_signed_nor(n_x);
    double n_y_snor = f_n_speed_signed_nor(n_y);
    float n_speed_max = 15.0;
    int n_speed_x = (int) abs(n_x_snor * n_speed_max);
    int n_speed_y = (int) abs(n_y_snor * n_speed_max);
    int n_steps = 2;
    int n_steps_x = (n_x_snor > 0.) ? n_steps : -n_steps;
    int n_steps_y = (n_y_snor > 0.) ? n_steps : -n_steps;
    // Serial.println(n_x_snor);
        // o_stepper_x.setSpeed(10.);
    // Serial.println(n_y_snor);
    if(n_x_snor > 0.1 || n_x_snor < -0.1){
        // Serial.println(n_speed_x);
        // Serial.println(n_speed_y);

        // Rotate CW slowly at 5 RPM
        o_stepper_x.step(n_steps_x);
        // o_stepper_x.setSpeed(5);
        // o_stepper_x.step(1111);
    }else{
            digitalWrite(8, 0);
            digitalWrite(9, 0);
            digitalWrite(10, 0);
            digitalWrite(11, 0);
    }
    // Serial.println(n_y_snor);
    if(n_y_snor > 0.1 || n_y_snor < -0.1){
        // Serial.println(n_speed_y);
        // Serial.println(n_speed_y);

        // Rotate CW slowly at 5 RPM
        o_stepper_y.setSpeed(n_speed_y);
        o_stepper_y.step(n_steps_y);
        // o_stepper_x.setSpeed(5);
        // o_stepper_x.step(1111);
    }else{
            digitalWrite(2, 0);
            digitalWrite(3, 0);
            digitalWrite(4, 0);
            digitalWrite(5, 0);
    }


}
