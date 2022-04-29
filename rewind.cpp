
    #include <LiquidCrystal.h>
    #include <time.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    
    LiquidCrystal o_lcd(8,9,10,11,12,13);

    #define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038 // ive seen both 2048 and 2038
    // #define N_TAU M_PI*2



    // b = arctan(a/b)
    // b = arctan(484/1)
    //   
    //       +
    //      /| 
    //     /b|b =  
    //    /  |
    //  C/   |A = 484mm (distance hinge to screw)
    //  /   y|y = gamma = 90deg
    // +-----+
    //  B=1 (distance screw traveling in one millimeter) 

    int f_calc_n_motor_delay_microseconds(
        int n_distance_hinge_to_screw_millimeter, 
        int n_distance_screw_traveling_in_one_revolution_millimeter
    ){

        int a = n_distance_screw_traveling_in_one_revolution_millimeter;
        int b = n_distance_hinge_to_screw_millimeter;

        double n_radians_hinge_per_screw_revolution = atan((double) a/(double) b); // ca 0.00206611

        int n_degrees_max = 360; 
        double n_tau = M_PI*2;
        double n_degrees_hinge_per_screw_revolution = (n_radians_hinge_per_screw_revolution / n_tau )*n_degrees_max; // ca 0.11837954
        //The earth rotates once every 23 hours, 56 minutes and 4.09053 seconds
        double n_degrees_earth_rotation_per_second = 0.004158533;// (double) n_degrees_max/(double)((23*60*60)+(56*60)+(4.09053)); //0,004158533 //wwwwttttfff why the fuck doesnt this work 

        double n_revolutions_per_second_to_match_sidereal_period = (float) n_degrees_earth_rotation_per_second / (float) n_degrees_hinge_per_screw_revolution; // 0,035128811

        double n_seconds_per_revolution_to_match_sidereal_period = (double)1.0/n_revolutions_per_second_to_match_sidereal_period;

        int n_steps_for_full_revolution = 2048;
        // int n_steps_for_full_revolution = 2038; //i have seen both, 2048 and 2038, after testing 2038 is better, might have to do with wrong n_distance_hinge_to_screw_millimeter

        double n_seconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period;// around 28
        double n_microseconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period*1000.0*1000.0;// around 28

        int n_minimum_millisecond_delay_between_steps = 2;

        // 10sec * 1000ms = 10000ms / 2048steps = 4.88...
        int n_calculated_delay_microseconds_for_seconds_per_revolution = n_microseconds_per_revolution / n_steps_for_full_revolution;
    
        Serial.println("n_distance_screw_traveling_in_one_revolution_millimeter");
        Serial.println(n_distance_screw_traveling_in_one_revolution_millimeter);
        Serial.println("---\n");

        Serial.println("n_distance_hinge_to_screw_millimeter");
        Serial.println(n_distance_hinge_to_screw_millimeter);
        Serial.println("---\n");

        Serial.println("n_radians_hinge_per_screw_revolution");
        Serial.println(n_radians_hinge_per_screw_revolution);
        Serial.println("---\n");

        Serial.println("n_degrees_earth_rotation_per_second");
        Serial.println(n_degrees_earth_rotation_per_second);
        Serial.println("---\n");

        Serial.println("n_revolutions_per_second_to_match_sidereal_period");
        Serial.println(n_revolutions_per_second_to_match_sidereal_period);
        Serial.println("---\n");

        Serial.println("n_seconds_per_revolution_to_match_sidereal_period");
        Serial.println(n_seconds_per_revolution_to_match_sidereal_period);
        Serial.println("---\n");

        Serial.println("n_steps_for_full_revolution");
        Serial.println(n_steps_for_full_revolution);
        Serial.println("---\n");

        Serial.println("n_seconds_per_revolution");
        Serial.println(n_seconds_per_revolution);
        Serial.println("---\n");

        Serial.println("n_microseconds_per_revolution");
        Serial.println(n_microseconds_per_revolution);  
        Serial.println("---\n");

        Serial.println("n_calculated_delay_microseconds_for_seconds_per_revolution");
        Serial.println(n_calculated_delay_microseconds_for_seconds_per_revolution);          
        Serial.println("---\n");

        return n_calculated_delay_microseconds_for_seconds_per_revolution;

    }

    int n_distance_hinge_to_screw_millimeter = 471; // 23:35
    int n_distance_screw_traveling_in_one_revolution_millimeter = 1;
    


    int a_b_stepper_pin_number[4] = {4,5,6,7};

    int step_number = 0;

    // push button stuff 
    // int n_pin_button_directiontoggler = 0;  // unfortunately i run out of pins and pin 0 cannot be used as digitalRead input pin
    int n_pin_button_analog = A4; // i remove a unused LED , now a pin is free
    int n_pin_button_digital = 2;

    int b_button_down = 0;
    int b_button_down_last = 0;
    int b_click = 0;
    unsigned long n_b_click_ts_milliseconds =  0;
    unsigned long n_b_click_ts_milliseconds_last =  0;
    int n_b_click_delta_ts_milliseconds = 0; 

    int n_read_value_button_analog = 0; 
    int n_read_value_button_analog_last = 0; 
    int b_read_value_button_digital = 0; 
    int b_read_value_button_digital_last = 0; 

    int n_pin_led_red = 1;
    int n_pin_led_green = 2;
    int b_button_directiontoggler = 0;
    int b_direction = 0; 

    int n_rewind_delay_microseconds = 2009;// lower number, faster rewind

    int b_button_directiontoggler_last = 0; 
    // end push button stuff

    int n_lcd_backlight_pin = 3; // 3 is a PWM pin which can be used to analogWrite !!!

    int n_pin_potentiometer = A5; 
    int n_value_potentiometer = 0; 
    int n_value_potentiometer_last = 0; 


    unsigned long n_ts_microseconds = 0;
    unsigned long n_ts_microseconds_last = 0;
    int n_delta_ts_microseconds = 0;

  
    int n_calculated_motor_delay_microseconds;



    void setup()
    {

        Serial.begin(9600);

        
        n_calculated_motor_delay_microseconds = f_calc_n_motor_delay_microseconds(
            n_distance_hinge_to_screw_millimeter, 
            n_distance_screw_traveling_in_one_revolution_millimeter
        ); 
        pinMode(n_lcd_backlight_pin, OUTPUT);
        pinMode(n_pin_potentiometer, INPUT);

        pinMode(n_pin_button_analog, INPUT);
        pinMode(n_pin_button_digital, INPUT);

        analogWrite(n_lcd_backlight_pin, 123);

        o_lcd.begin(16,2);

        int n_i = 0; 
        while(n_i < 4){
            pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
            n_i++; 
        }

    }

    long n_time = 0; 

    int n_max_stepper_pins = 4;

    int a_b_stepper_pin_voltage[4] = {0,0,0,0};

    long n_stepper_pin_modulo = 0; 


    int b_backwards = 1; // direction

    float n_value_potentiometer_normalized = 0.0;
    float n_value_potentiometer_normalized_last = 0.0;
    int n_value_potentiometer_max = 1024;
    int n_delay_microseconds = 0; 

    void loop()
    {   
        n_ts_microseconds = micros();

        b_backwards = 1;
        
        n_time++;  
        n_stepper_pin_modulo = abs((3*b_backwards)-(n_time % n_max_stepper_pins));
        int n_i = 0; 

        while(n_i < 4){
            digitalWrite(a_b_stepper_pin_number[n_i], n_stepper_pin_modulo == n_i);
            n_i++;
        }
        
        if(n_time >= (__LONG_MAX__)){
            n_time = 0; 
        }

        n_delta_ts_microseconds = abs(n_ts_microseconds - n_ts_microseconds_last);

        int n_safety = 0;
        delayMicroseconds(n_calculated_motor_delay_microseconds-n_delta_ts_microseconds+n_safety);
        
        n_ts_microseconds_last = n_ts_microseconds;

    }


