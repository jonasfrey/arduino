    //Includes the Arduino Stepper Library

    // |tit|val|
    // |---|---|
    // |Operating Voltage|5VDC|
    // |Operating Current|240mA (typical)|
    // |Number of phases|4|
    // |Gear Reduction Ratio|64:1|
    // |Step Angle|5.625°/64|
    // |Frequency|100Hz|
    // |In-traction Torque|>34.3mN.m(120Hz)|
    // |Self-positioning Torque|>34.3mN.m|
    // |Friction torque|600-1200 gf.cm|
    // |Pull in torque|300 gf.cm|  
    // ![](./28BYJ48-Stepper-Motor-Gear-Ratio-Explanation.png)



#define n_pin_ain_potentiometer_x  A0 // Arduino pin connected to VRX pin
#define n_pin_ain_potentiometer_y  A1 // Arduino pin connected to VRY pin

    struct O_byj48_stepper{
      double n_operating_voltage_typical;
      double n_operating_current_ma_typical;
      double n_number_of_phases;
      double n_gear_reduction_ratio_to_one;
      double n_step_angle_degrees;
      double n_frequency_hz;
      double n_in_traction_torque_mnm_hz;
      double n_friction_torque_gf_cm;
      double n_pull_in_tourqu_gf_cm;
      int n_len_a_n_pin;
      int a_n_pin[4];
      double n_rpm_max;
      double n_rpm_min;
      double n_rpm;
      double n_revolutions_per_second;
      double n_steps_per_second;
      double n_microseconds_delay_between_step;
      double n_steps_per_revolution;
      int n_idx_active_a_n_pin; 
      double b_direction_forward; 
      long n_t_mic_sec_stepper;
      long n_t_mic_sec_stepper__last;
      long n_t_mic_sec_stepper__diff;
      double n_t_mic_sec_stepper__diff_thresh;

    };

    void f_init_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper, 
      // int a_n_pin[]
    int n_pin1,
    int n_pin2,
    int n_pin3,
    int n_pin4
    ){
      o_byj48_stepper->n_len_a_n_pin = 4;
      // enter the static data 
      o_byj48_stepper->n_operating_voltage_typical = 5.0;
      o_byj48_stepper->n_operating_current_ma_typical = 240.0;
      o_byj48_stepper->n_number_of_phases = 4.0;
      o_byj48_stepper->n_gear_reduction_ratio_to_one = 64.0;
      o_byj48_stepper->n_step_angle_degrees = 5.625;
      o_byj48_stepper->n_frequency_hz = 100.;
      o_byj48_stepper->n_in_traction_torque_mnm_hz = 120.;
      o_byj48_stepper->n_friction_torque_gf_cm = 600.;
      o_byj48_stepper->n_pull_in_tourqu_gf_cm = 300.;
      
      //o_byj48_stepper->a_n_pin = a_n_pin;
      //   for(int n_i = 0;n_i< o_byj48_stepper->n_len_a_n_pin; n_i+=1){
      //       o_byj48_stepper->a_n_pin[n_i] = a_n_pin[n_i];
      // }

      o_byj48_stepper->a_n_pin[0] = n_pin1;
      o_byj48_stepper->a_n_pin[1] = n_pin2;
      o_byj48_stepper->a_n_pin[2] = n_pin3;
      o_byj48_stepper->a_n_pin[3] = n_pin4;

      o_byj48_stepper->n_revolutions_per_second = 0.;
      o_byj48_stepper->n_steps_per_second = 0.;
      o_byj48_stepper->n_microseconds_delay_between_step = 0.;
      o_byj48_stepper->n_rpm_max = 15.;
      o_byj48_stepper->n_rpm_min = 0.;
      o_byj48_stepper->n_rpm = 0.;

      // o_byj48_stepper->n_steps_per_revolution = 2048; // some people only say its 2048, 
      o_byj48_stepper->n_steps_per_revolution = 2038.0; // some say its 2038...

      o_byj48_stepper->b_direction_forward = 1;

      o_byj48_stepper->n_t_mic_sec_stepper = 0;
      o_byj48_stepper->n_t_mic_sec_stepper__last = 0;
      o_byj48_stepper->n_t_mic_sec_stepper__diff = 0;
      o_byj48_stepper->n_t_mic_sec_stepper__diff_thresh = 0.0;

      o_byj48_stepper->n_idx_active_a_n_pin = 0;
    }

    void f_setup_pinMode_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper
    ){
      for(int n_i = 0;n_i< o_byj48_stepper->n_len_a_n_pin; n_i+=1){
            pinMode(o_byj48_stepper->a_n_pin[n_i], OUTPUT);
      }
    }

    void f_update_speed_normalized_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper,
      double n_speed_nor
    ){
      double * n_ptr_n ;
      double n_speed_normalized_fract = abs(modf(n_speed_nor, n_ptr_n));

      double n_rpm = n_speed_normalized_fract * (
        o_byj48_stepper->n_rpm_max 
        - o_byj48_stepper->n_rpm_min
      ) + o_byj48_stepper->n_rpm_min;

      f_update_rpm_o_byj48_stepper(
        o_byj48_stepper, 
        n_rpm
      );
    };

    void f_update_rpm_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper, 
      double n_rpm
    ){
      if(n_rpm > o_byj48_stepper->n_rpm_max){
        Serial.println("cannot set the speed:");
        Serial.println(n_rpm);
        Serial.println("the maximum rpm (revolutions per minute) (speed) is ");
        Serial.println(o_byj48_stepper->n_rpm_max);
      }
      if(n_rpm < o_byj48_stepper->n_rpm_min){
        Serial.println("cannot set the speed:");
        Serial.println(n_rpm);
        Serial.println("the minimum rpm (revolutions per minute) (speed) is ");
        Serial.println(0.);
      }
      if(
        n_rpm <= o_byj48_stepper->n_rpm_max,
        n_rpm >= o_byj48_stepper->n_rpm_min
      ){
        o_byj48_stepper->n_rpm = n_rpm;
        o_byj48_stepper->n_revolutions_per_second 
          = o_byj48_stepper->n_rpm / 60.;
        o_byj48_stepper->n_steps_per_second 
          = o_byj48_stepper->n_revolutions_per_second
            * o_byj48_stepper->n_steps_per_revolution;
        o_byj48_stepper->n_microseconds_delay_between_step 
          = (1l*1000l*1000l) / o_byj48_stepper->n_steps_per_second;
      }
    }





    struct O_byj48_stepper o_byj48_stepper__x;
    struct O_byj48_stepper o_byj48_stepper__y;
    

    void setup() {

      f_init_o_byj48_stepper(
        &o_byj48_stepper__x,
        4,
        5,
        6,
        7
      );

      f_init_o_byj48_stepper(
        &o_byj48_stepper__y,
        8,
        9,
        10,
        11
      );
      f_setup_pinMode_o_byj48_stepper(&o_byj48_stepper__x);
      f_setup_pinMode_o_byj48_stepper(&o_byj48_stepper__y);

      o_byj48_stepper__x.b_direction_forward = 1;
      o_byj48_stepper__y.b_direction_forward = 1;
      Serial.begin(9600);
    }

    void f_update_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper, 
      long n_mic_secs_delay
    ){
      o_byj48_stepper->n_t_mic_sec_stepper += n_mic_secs_delay; 
      double n_t_mic_sec_diff = o_byj48_stepper->n_t_mic_sec_stepper - o_byj48_stepper->n_microseconds_delay_between_step;
      if(n_t_mic_sec_diff > 0){
        o_byj48_stepper->n_t_mic_sec_stepper = n_t_mic_sec_diff;
        if(o_byj48_stepper->n_rpm > 0.){
          f_write_step_o_byj48_stepper(o_byj48_stepper);
        }
      }
    }

    void f_write_step_o_byj48_stepper(
      O_byj48_stepper * o_byj48_stepper
    ){
      if(o_byj48_stepper->b_direction_forward){
        o_byj48_stepper->n_idx_active_a_n_pin = 
          (o_byj48_stepper->n_idx_active_a_n_pin+1) % o_byj48_stepper->n_len_a_n_pin;
      }else{
        o_byj48_stepper->n_idx_active_a_n_pin = 
          o_byj48_stepper->n_idx_active_a_n_pin -1;
        if(o_byj48_stepper->n_idx_active_a_n_pin < 0){
          o_byj48_stepper->n_idx_active_a_n_pin = (o_byj48_stepper->n_len_a_n_pin-1);
        }
      }



      for(int n_i = 0;n_i< o_byj48_stepper->n_len_a_n_pin; n_i+=1){
            int b = n_i == o_byj48_stepper->n_idx_active_a_n_pin; 
            digitalWrite(o_byj48_stepper->a_n_pin[n_i], b);
      }
    }



    long n_t_mic_sec_delay = 500; // this should be low to achieve a high precision
    long n_mic_secs = 0;
    void loop() {


      int n_x = analogRead(n_pin_ain_potentiometer_x);
      int n_y = analogRead(n_pin_ain_potentiometer_y);
      double n_x_nor = (((float) n_x / 1024.0)-.5)*2.;
      double n_y_nor = (((float) n_y / 1024.0)-.5)*2.;
      double n_speed_nor_x = abs(n_x_nor);
      double n_speed_nor_y = abs(n_y_nor);
      if(n_speed_nor_x < 0.1){n_speed_nor_x = 0.;}
      if(n_speed_nor_y < 0.1){n_speed_nor_y = 0.;}

      o_byj48_stepper__x.b_direction_forward = n_x_nor < 0.;
      o_byj48_stepper__y.b_direction_forward = n_y_nor < 0.;

      f_update_speed_normalized_o_byj48_stepper(
        &o_byj48_stepper__x, 
        n_speed_nor_x
      );

      f_update_speed_normalized_o_byj48_stepper(
        &o_byj48_stepper__y, 
        n_speed_nor_y
      );


      f_update_o_byj48_stepper(
        &o_byj48_stepper__x, 
        n_t_mic_sec_delay
      );
      f_update_o_byj48_stepper(
        &o_byj48_stepper__y, 
        n_t_mic_sec_delay
      );

        delayMicroseconds(n_t_mic_sec_delay);

    }
