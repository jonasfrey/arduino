#include './O_28BYJ48_stepper.c'

    struct O_byj48_stepper o_byj48_stepper__1;


    void setup() {

      f_init_o_byj48_stepper(
        &o_byj48_stepper__1,
        2,
        3,
        4,
        5
      );

      f_update_rpm_o_byj48_stepper(
        &o_byj48_stepper__1, 
        15.
      );
      o_byj48_stepper__1.b_direction_forward = 0;
      Serial.begin(9600);
    }

    long n_t_mic_sec_delay = 200; // this should be low to achieve a high precision
    long n_mic_secs = 0;
    void loop() {
      // n_mic_secs+=n_t_mic_sec_delay;
      // if(n_mic_secs > 10l*1000l*1000l){//10 seconds
      //   n_mic_secs = 0;
      // }
      // if((n_mic_secs / 1000l*1000l) % 2 == 0.){ // toggle every other second
      //   o_byj48_stepper__1.b_direction_forward = 0;
      //   f_update_speed_normalized_o_byj48_stepper(
      //     0.5, // half of the max speed
      //     &o_byj48_stepper__1
      //   );
      // }else{
      //   o_byj48_stepper__1.b_direction_forward = 1;
      //   f_update_speed_normalized_o_byj48_stepper(
      //     1.0, // max speed
      //     &o_byj48_stepper__1
      //   );
      // }

      f_update_o_byj48_stepper(
        &o_byj48_stepper__1, 
        n_t_mic_sec_delay
      );

        delayMicroseconds(n_t_mic_sec_delay);

    }
