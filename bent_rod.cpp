
    #include <time.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    

    #define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038 // ive seen both 2048 and 2038
    // #define N_TAU M_PI*2



    // double n_radius_millimeter = 483; 
    double n_radius_millimeter = 483; 
    double n_degrees_earth_rotation_per_second = 0.004158533;// (double) n_degrees_max/(double)((23*60*60)+(56*60)+(4.09053)); //0,004158533 //wwwwttttfff why the fuck doesnt this work 
    double n_millimeter_circle_circumfence = n_radius_millimeter * 2.0 * M_PI;
    double n_sec_per_sidereal_day = 86164.0905; 
    double n_milliseconds_per_sidereal_day = n_sec_per_sidereal_day * 1000; 
    double n_microseconds_per_sidereal_day = n_sec_per_sidereal_day * 1000 * 1000;

    double n_seconds_per_millimeter = n_sec_per_sidereal_day / n_millimeter_circle_circumfence; 
    double n_revolutions_per_second = 1.0/n_seconds_per_millimeter; // since 1mm = 1 revolution
    double n_steps_per_rev = 2038.0;
    double n_steps_per_second = n_revolutions_per_second * n_steps_per_rev; 
    double n_delay_microseconds_between_step = (1000.0 * 1000.0) / n_steps_per_second;
    //!!!! since the max value of delayMicroseconds is (2^14)-1 = 16384-1
    // we have to split the delay function into delay((int) microsec / 1000.0) , delayMicroseconds(microsec%1000.0)

    double n_delay_microseconds_between_step_is_limited_to_2_power_14 = (int) n_delay_microseconds_between_step % 1000;
    int n_delay_milliseconds = (int) (n_delay_microseconds_between_step / 1000.0);

    int n_time = 0; 
    int n_stepper_pin_modulo = 0; 
    int n_max_stepper_pins = 4; 
    int a_b_stepper_pin_number[4] = {4,5,6,7};

    double n_microseconds_arduino_void_loop = 7.0;

    void setup()
    {

        Serial.begin(9600);

        Serial.println("n_radius_millimeter");
        Serial.println(n_radius_millimeter);
        Serial.println("n_degrees_earth_rotation_per_second");
        Serial.println(n_degrees_earth_rotation_per_second);
        Serial.println("n_millimeter_circle_circumfence");
        Serial.println(n_millimeter_circle_circumfence);
        Serial.println("n_sec_per_sidereal_day");
        Serial.println(n_sec_per_sidereal_day);
        Serial.println("n_milliseconds_per_sidereal_day");
        Serial.println(n_milliseconds_per_sidereal_day);
        Serial.println("n_microseconds_per_sidereal_day");
        Serial.println(n_microseconds_per_sidereal_day);
        Serial.println("n_seconds_per_millimeter");
        Serial.println(n_seconds_per_millimeter);
        Serial.println("n_revolutions_per_second");
        Serial.println(n_revolutions_per_second);
        Serial.println("n_steps_per_rev");
        Serial.println(n_steps_per_rev);
        Serial.println("n_steps_per_second");
        Serial.println(n_steps_per_second);
        Serial.println("n_delay_microseconds_between_step");
        Serial.println(n_delay_microseconds_between_step);

        Serial.println("n_delay_microseconds_between_step_is_limited_to_2_power_14");
        Serial.println(n_delay_microseconds_between_step_is_limited_to_2_power_14);
        Serial.println("n_delay_milliseconds");
        Serial.println(n_delay_milliseconds);

    

        int n_i = 0; 
        while(n_i < 4){
            pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
            n_i++; 
        }

    }

    void loop()
    {   

        
        n_time++;  
        n_stepper_pin_modulo = (n_time % n_max_stepper_pins);
        int n_i = 0; 

        while(n_i < 4){
            digitalWrite(a_b_stepper_pin_number[n_i], n_stepper_pin_modulo == n_i);
            n_i++;
        }
        
        if(n_time > n_max_stepper_pins*100){
            n_time = 0; 
        }

        //!!!! since the max value of delayMicroseconds is (2^14)-1 = 16384-1
        // we have to split the delay function into delay((int) microsec / 1000.0) , delayMicroseconds(microsec%1000.0)
        delay(n_delay_milliseconds);
        delayMicroseconds(n_delay_microseconds_between_step_is_limited_to_2_power_14);
        

    }


