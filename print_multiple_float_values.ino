
//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int n_steps_per_rev = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper o_stepper_x = Stepper(n_steps_per_rev, 2,4,3,5);
Stepper o_stepper_y = Stepper(n_steps_per_rev, 6,8,7,9);
Stepper o_stepper_z = Stepper(n_steps_per_rev, 10,12,11,13);

void setup() {
    Serial.begin(9600);
    // Nothing to do (Stepper Library sets pins as outputs)
}
float f_n_val_signed_nor(
    int n_val, 
    int n_min, // 
    int n_max //1024
){
  float n_range = (float)(n_max-n_min); 
  double n_nor = (n_val - n_min)/n_range;//value from 0. to 1.
  n_nor = (n_nor -.5)*2.;//value from -1. to 1.
  return n_nor;
}

void f_update_stepper(
    Stepper &o_stepper, 
    float n_speed_signed_nor,
    float n_speed_max
){

    int n_speed = (int) abs(n_speed_signed_nor * n_speed_max);
    int n_steps = 2;
    int n_steps = (n_speed_signed_nor > 0.) ? n_steps : -n_steps;

    if(n_speed > 0.1 || n_speed < -0.1){
        o_stepper_x.step(n_steps);
    }else{
        digitalWrite(8, 0);
        digitalWrite(9, 0);
        digitalWrite(10, 0);
        digitalWrite(11, 0);
    }

}

void loop() {

    int n_x = analogRead(A0);
    int n_y = analogRead(A1);
    int n_z = analogRead(A2);
    float n_x_signed_nor = f_n_val_signed_nor(n_x, 848, 942);
    float n_y_signed_nor = f_n_val_signed_nor(n_y, 33, 963);
    float n_z_signed_nor = f_n_val_signed_nor(n_z, 0, 970);
    // int n_y = analogRead(A4);
    // int n_z = analogRead(A4);

    // Serial.println("---");

    // Serial.print("x:");
    // Serial.print(n_x_signed_nor);

    // Serial.print("y:");
    // Serial.print(n_y_signed_nor);

    // Serial.print("z:");
    // Serial.print(n_z_signed_nor);



   char s_output_line[128];

   char s_n_x_signed_nor[10]; // float converted to string
   dtostrf(n_x_signed_nor, 2, 2, s_n_x_signed_nor);  // 8
   char s_n_y_signed_nor[10]; // float converted to string
   dtostrf(n_y_signed_nor, 2, 2, s_n_y_signed_nor);  // 8
   char s_n_z_signed_nor[10]; // float converted to string
   dtostrf(n_z_signed_nor, 2, 2, s_n_z_signed_nor);  // 8

   sprintf(s_output_line, "x: %6s, y: %6s, z: %6s", s_n_x_signed_nor, s_n_y_signed_nor, s_n_z_signed_nor);

   Serial.println(s_output_line);
  
}