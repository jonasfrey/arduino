
    #include <LiquidCrystal.h>
    #include <time.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    
    LiquidCrystal o_lcd(8,9,10,11,12,13);

    #define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038 // ive seen both 2048 and 2038

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


    // int n_distance_hinge_to_screw_millimeter = 484; // back to normal
    int n_distance_hinge_to_screw_millimeter = 490; // 23:35
    // int n_distance_hinge_to_screw_millimeter = 492; // slower a bit 
    // int n_distance_hinge_to_screw_millimeter = 472; // faster a bit 
    // int n_distance_hinge_to_screw_millimeter = 489; // maybe this
    // int n_distance_hinge_to_screw_millimeter = 550; // faster
    // int n_distance_hinge_to_screw_millimeter = 460; // slower
    // int n_distance_hinge_to_screw_millimeter = 420; // slower and haha lol 420 xD

    int n_distance_screw_traveling_in_one_revolution_millimeter = 1;
    
    int a = n_distance_screw_traveling_in_one_revolution_millimeter;
    int b = n_distance_hinge_to_screw_millimeter;

    double n_tau = M_PI * 2;
    double n_radians_hinge_per_screw_revolution = atan((double) a/(double) b); // ca 0.00206611

    int n_degrees_max = 360; 
    double n_degrees_hinge_per_screw_revolution = (n_radians_hinge_per_screw_revolution / n_tau )*n_degrees_max; // ca 0.11837954
    //The earth rotates once every 23 hours, 56 minutes and 4.09053 seconds
    double n_degrees_earth_rotation_per_second = 0.004158533;// (double) n_degrees_max/(double)((23*60*60)+(56*60)+(4.09053)); //0,004158533 //wwwwttttfff why the fuck doesnt this work 

    double n_revolutions_per_second_to_match_sidereal_period = (float) n_degrees_earth_rotation_per_second / (float) n_degrees_hinge_per_screw_revolution; // 0,035128811

    double n_seconds_per_revolution_to_match_sidereal_period = (double)1.0/n_revolutions_per_second_to_match_sidereal_period;

    int a_b_stepper_pin_number[4] = {4,5,6,7};

    int step_number = 0;

    // push button stuff 
    // int n_pin_button_directiontoggler = 0;  // unfortunately i run out of pins and pin 0 cannot be used as digitalRead input pin
    int n_pin_button = A4; 
    int b_button_down = 0;
    int b_button_down_last = 0;
    int b_click = 0;
    int n_analog_read_value_button = 0; 
    int n_analog_read_value_button_last = 0; 

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

    char * s_lcd_row_1 = "row 1 ready"; 
    char * s_lcd_row_2 = "row 2 ready"; 

    const int n_length_menu_options = 5; // adjust everytime you add an option 
    const int n_length_max_menu_option_name = 16+1; 

    char * s_menu_option_name_backlight                 =  "lcd backlight:"; 
    char * s_menu_option_name_distance_hinge            =  "distance hinge:";
    char * s_menu_option_name_delay_microseconds        =  "delay microsec:";
    char * s_menu_option_name_direciton_motor           =  "direction motor:";
    char * s_menu_option_name_potentiometer             =  "potentiometer:";

    char a_s_menu_option_name[n_length_menu_options][n_length_max_menu_option_name];
    
    void f_prepare_menu_options(){

        s_menu_option_name_backlight = strcpy(a_s_menu_option_name[0], s_menu_option_name_backlight);
        s_menu_option_name_distance_hinge = strcpy(a_s_menu_option_name[1], s_menu_option_name_distance_hinge);
        s_menu_option_name_delay_microseconds = strcpy(a_s_menu_option_name[2], s_menu_option_name_delay_microseconds);
        s_menu_option_name_direciton_motor = strcpy(a_s_menu_option_name[3], s_menu_option_name_direciton_motor);
        s_menu_option_name_potentiometer = strcpy(a_s_menu_option_name[4], s_menu_option_name_potentiometer);
    }
    
    int n_index_menu_option = 0;
    
    void f_print_lcd_rows(){
        o_lcd.clear();
        o_lcd.setCursor(0, 0); 
        o_lcd.print(s_lcd_row_1);
        o_lcd.setCursor(0, 1); 
        o_lcd.print(s_lcd_row_2);
    }

    void setup()
    {
        f_prepare_menu_options();



        pinMode(n_lcd_backlight_pin, OUTPUT);
        pinMode(n_pin_potentiometer, INPUT);
        pinMode(n_pin_button, INPUT);


        // Kontrast und Hintergrund Steuerung des LCD
        analogWrite(n_lcd_backlight_pin, 255);

        o_lcd.begin(16,2);


        // o_lcd.clear();
        int n_i = 0; 
        while(n_i < 4){
            pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
            n_i++; 
        }

        pinMode(n_pin_led_red, OUTPUT); 
        pinMode(n_pin_led_green, OUTPUT); 

        Serial.begin(9600);


    }
    long n_time = 0; 

    int n_max_stepper_pins = 4;

    int a_b_stepper_pin_voltage[4] = {0,0,0,0};

    long n_stepper_pin_modulo = 0; 

    int n_steps_for_full_revolution = 2048;
    // int n_steps_for_full_revolution = 2038; //i have seen both, 2048 and 2038, after testing 2038 is better, might have to do with wrong n_distance_hinge_to_screw_millimeter

    double n_seconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period;// around 28
    double n_microseconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period*1000.0*1000.0;// around 28

    int n_minimum_millisecond_delay_between_steps = 2;

    // 10sec * 1000ms = 10000ms / 2048steps = 4.88...
    int n_calculated_delay_microseconds_for_seconds_per_revolution = n_microseconds_per_revolution / n_steps_for_full_revolution;
    int b_backwards = 1; // direction

    float n_value_potentiometer_normalized = 0.0;
    float n_value_potentiometer_normalized_last = 0.0;
    int n_value_potentiometer_max = 1024;
    int n_delay_microseconds = 0; 
    void loop()
    {   
        b_click = 0;
        n_value_potentiometer = analogRead(n_pin_potentiometer); 
        n_value_potentiometer_normalized = (float) n_value_potentiometer / n_value_potentiometer_max;

        // // only for debugging, will prevent stepper from working correctly!!!!
        // Serial.println("n_radians_hinge_per_screw_revolution");
        // Serial.println(n_radians_hinge_per_screw_revolution, 10);
        // Serial.println("n_degrees_hinge_per_screw_revolution");
        // Serial.println(n_degrees_hinge_per_screw_revolution, 10);
        // Serial.println("n_revolutions_per_second_to_match_sidereal_period");
        // Serial.println(n_revolutions_per_second_to_match_sidereal_period, 10);
        // Serial.println("n_seconds_per_revolution_to_match_sidereal_period");
        // Serial.println(n_seconds_per_revolution_to_match_sidereal_period, 10);
        // Serial.println("n_degrees_earth_rotation_per_second");
        // Serial.println(n_degrees_earth_rotation_per_second, 10);
        // Serial.println("n_revolutions_per_second_to_match_sidereal_period");
        // Serial.println(n_revolutions_per_second_to_match_sidereal_period, 10);
        // Serial.println("n_calculated_delay_microseconds_for_seconds_per_revolution");
        // Serial.println(n_calculated_delay_microseconds_for_seconds_per_revolution);
    
        // push button stuff 
        n_analog_read_value_button = analogRead(n_pin_button);


        if(n_analog_read_value_button && n_analog_read_value_button_last == 0){
            b_click = 1;
        }
        if(b_click){
            n_index_menu_option = ((n_index_menu_option +1) % n_length_menu_options);
            s_lcd_row_1 = a_s_menu_option_name[n_index_menu_option];
        }

        // if(a_s_menu_option_name[n_index_menu_option] == s_menu_option_name_backlight){
        //     int n_value_lcd_backlight = (int) (n_value_potentiometer_normalized * 255);
        //     analogWrite(n_lcd_backlight_pin, n_value_lcd_backlight);
            
        //     sprintf(s_lcd_row_2, "value:%i", n_value_lcd_backlight); 
        // }

        // if(a_s_menu_option_name[n_index_menu_option] == s_menu_option_name_potentiometer){
        //     sprintf(s_lcd_row_2, "value:%i", n_value_potentiometer); 
        // }
        
        // if(a_s_menu_option_name[n_index_menu_option] == s_menu_option_name_direciton_motor){
        //     //direction via delta , can be very buggy if potentiometer is not soldered
        //     // if(n_value_potentiometer > n_value_potentiometer_last){
        //     //     b_direction = 1;
        //     // }
        //     // if(n_value_potentiometer < n_value_potentiometer_last){
        //     //     b_direction = 0;
        //     // }
        //     b_direction = n_value_potentiometer_normalized < 0.5;
            
        //     sprintf(s_lcd_row_2, "value:%i", b_direction); 
        // }


        if(b_direction){
            digitalWrite(n_pin_led_red, 1); // Turn the LED on
            digitalWrite(n_pin_led_green, 0); // Turn the LED on
        }else{
            digitalWrite(n_pin_led_red, 0); // Turn the LED on
            digitalWrite(n_pin_led_green, 1); // Turn the LED on
        }
        b_backwards = b_direction;
        
        n_analog_read_value_button_last = n_analog_read_value_button;
        b_button_down_last = b_button_down;
        n_value_potentiometer_last = n_value_potentiometer;
        n_value_potentiometer_normalized_last = n_value_potentiometer_normalized;

        // end push button stuff

        // one loop counts as one step!!
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
        f_print_lcd_rows();

        delayMicroseconds(n_delay_microseconds);

    }


