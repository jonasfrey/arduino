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

    struct O_nema_stepper{
      double n_microstepping_divisor;
      double n_steps_per_revolution;
      double n_revolutions_per_minute_max;
      double n_revolutions_per_minute_min;
      double n_revolutions_per_minute;
      double n_speed_nor;
      int n_pin_step; 
      int n_pin_direction; 


      
      double n_revolutions_per_second;
      double n_microseconds_delay_between_step;
      double n_steps_per_second;

      double b_direction_forward; 
      double b_direction_forward__last; 
    };

    void f_init_o_nema_stepper(
      O_nema_stepper * o_nema_stepper, 
      int n_pin_step,
      int n_pin_direction
    ){
      
      // enter the static data 
      o_nema_stepper->n_microstepping_divisor = 1.0;
      o_nema_stepper->n_steps_per_revolution = 200.0;
      // o_nema_stepper->n_revolutions_per_minute_max = 3000.0;
      o_nema_stepper->n_revolutions_per_minute_max = 5.*60.0; // security measurement
      o_nema_stepper->n_revolutions_per_minute_min = 0.0;
      //speed 0 at init 
      o_nema_stepper->n_speed_nor = 0.0;
      o_nema_stepper->n_revolutions_per_minute = 0.0;
      o_nema_stepper->n_steps_per_second = 0.0;
      o_nema_stepper->n_revolutions_per_second = 0.0;
      o_nema_stepper->n_microseconds_delay_between_step = 0.0;
      o_nema_stepper->n_steps_per_second = 0.0;
      o_nema_stepper->b_direction_forward = 1.0;
      o_nema_stepper->b_direction_forward__last = 1.0;
      //pins
      o_nema_stepper->n_pin_step = n_pin_step;
      o_nema_stepper->n_pin_direction = n_pin_direction;

      f_setup_pinMode_o_nema_stepper(o_nema_stepper);

    }

    void f_setup_pinMode_o_nema_stepper(
      O_nema_stepper * o_nema_stepper
    ){
        pinMode(o_nema_stepper->n_pin_step, OUTPUT);
        pinMode(o_nema_stepper->n_pin_direction, OUTPUT);

    }

    void f_update_speed_normalized_o_nema_stepper(
      O_nema_stepper * o_nema_stepper,
      double n_speed_nor
    ){
      double * n_ptr_n ;
      double n_speed_normalized_fract = abs(modf(n_speed_nor, n_ptr_n));

      double n_revolutions_per_minute = n_speed_normalized_fract * (
        o_nema_stepper->n_revolutions_per_minute_max 
        - o_nema_stepper->n_revolutions_per_minute_min
      ) + o_nema_stepper->n_revolutions_per_minute_min;

      f_update_rpm_o_nema_stepper(
        o_nema_stepper, 
        n_revolutions_per_minute
      );
    };

    void f_update_rpm_o_nema_stepper(
      O_nema_stepper * o_nema_stepper, 
      double n_revolutions_per_minute
    ){
      if(n_revolutions_per_minute > o_nema_stepper->n_revolutions_per_minute_max){
        Serial.println("cannot set the speed:");
        Serial.println(n_revolutions_per_minute);
        Serial.println("the maximum rpm (revolutions per minute) (speed) is ");
        Serial.println(o_nema_stepper->n_revolutions_per_minute_max);
      }
      if(n_revolutions_per_minute < o_nema_stepper->n_revolutions_per_minute_min){
        Serial.println("cannot set the speed:");
        Serial.println(n_revolutions_per_minute);
        Serial.println("the minimum rpm (revolutions per minute) (speed) is ");
        Serial.println(0.);
      }
      if(
        n_revolutions_per_minute <= o_nema_stepper->n_revolutions_per_minute_max,
        n_revolutions_per_minute >= o_nema_stepper->n_revolutions_per_minute_min
      ){
        o_nema_stepper->n_revolutions_per_minute = n_revolutions_per_minute;
        o_nema_stepper->n_revolutions_per_second 
          = o_nema_stepper->n_revolutions_per_minute / 60.;
        o_nema_stepper->n_steps_per_second 
          = o_nema_stepper->n_revolutions_per_second
            * o_nema_stepper->n_steps_per_revolution;
        o_nema_stepper->n_microseconds_delay_between_step 
          = (1l*1000l*1000l) / o_nema_stepper->n_steps_per_second;

        // Serial.println(o_nema_stepper->n_microseconds_delay_between_step);

      }
    }


    struct O_nema_stepper o_nema_stepper;
    

    void setup() {

      f_init_o_nema_stepper(
        &o_nema_stepper,
        0,// pin step
        1// pin direction
      );
      Serial.begin(9600);

    }

    // void f_update_o_byj48_stepper(
    //   O_byj48_stepper * o_byj48_stepper, 
    //   long n_mic_secs_delay
    // ){
    //   o_byj48_stepper->n_t_mic_sec_stepper += n_mic_secs_delay; 
    //   double n_t_mic_sec_diff = o_byj48_stepper->n_t_mic_sec_stepper - o_byj48_stepper->n_microseconds_delay_between_step;
    //   if(n_t_mic_sec_diff > 0){
    //     o_byj48_stepper->n_t_mic_sec_stepper = n_t_mic_sec_diff;
    //     if(o_byj48_stepper->n_rpm > 0.){
    //       f_write_step_o_byj48_stepper(o_byj48_stepper);
    //     }
    //   }
    // }

    void f_write_step_o_nema_stepper(
      O_nema_stepper * o_nema_stepper
    ){
      if(o_nema_stepper->b_direction_forward__last !=  o_nema_stepper->b_direction_forward){
        int n_direction_state = (int) o_nema_stepper->b_direction_forward != 0;
        digitalWrite(o_nema_stepper->n_pin_direction, n_direction_state);
        o_nema_stepper->b_direction_forward = n_direction_state;
        o_nema_stepper->b_direction_forward__last = n_direction_state;
        Serial.println("direction change");
      }
      digitalWrite(o_nema_stepper->n_pin_step, 1.0);
      delayMicroseconds(
        o_nema_stepper->n_microseconds_delay_between_step/2.
      );
      digitalWrite(o_nema_stepper->n_pin_step, 0.0);
      delayMicroseconds(
        o_nema_stepper->n_microseconds_delay_between_step/2.
      );
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

      o_nema_stepper.b_direction_forward = n_x_nor < 0.;
      o_nema_stepper.n_microseconds_delay_between_step = 10000.0;
      // f_update_speed_normalized_o_nema_stepper(
      //   &o_nema_stepper, 
      //    0.5 //n_speed_nor_x
      // );
      f_write_step_o_nema_stepper(
        &o_nema_stepper
      );
      // for(int n = 0; n < 200; n+=1){

      //   digitalWrite(1, 1);
      //   delay(1);
      //   // delayMicroseconds(500);
      //   digitalWrite(1, 0);
      //   delay(1);
      //   // delayMicroseconds(500);
      // }
        // Serial.println(n_y_nor);

        // delay(1);
        // delayMicroseconds(n_t_mic_sec_delay);// delay will be called by f_write_step_o_nema_stepper

    }
