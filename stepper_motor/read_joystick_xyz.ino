
#include <Stepper.h> 

#define N_STEPS_PER_REV 200
#define N_MICROSTEPS_DIVISOR 1
#define N_PIN_DIR 2
#define N_PIN_STEP 3
Stepper o_stepper_z(N_STEPS_PER_REV/N_MICROSTEPS_DIVISOR, N_PIN_DIR,N_PIN_STEP); 

void setup() {
  o_stepper_z.setSpeed(3000);

  Serial.begin(9600);

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

    return 0.;
}
void loop() {

    int n_x = analogRead(A1);
    int n_y = analogRead(A2);
    int n_z = analogRead(A3);

    double n_x_snor = f_n_speed_signed_nor(n_x);
    double n_y_snor = f_n_speed_signed_nor(n_y);
    double n_z_snor = f_n_speed_signed_nor_special(n_y);


    o_stepper_z.setSpeed(abs(2222.0*n_x_snor));
    if(abs(n_x_snor) > 0.1){
        int n_step_z = (n_x_snor > 0) - (n_x_snor < 0);
        o_stepper_z.step(-n_step_z*10);
  Serial.println(n_step_z*10);
    }

    // o_stepper_z.setSpeed((int)(abs(n_z_snor*3000.)));
    // if(abs(n_z_snor) > 0.1){
    //     int n_step_z = (n_z_snor > 0) - (n_z_snor < 0);
    //     o_stepper_z.step(n_step_z);
    // }


}

