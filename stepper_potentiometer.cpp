
    #include <time.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    

    #define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038 // ive seen both 2048 and 2038
    // #define N_TAU M_PI*2

    int n_time = 0; 
    int n_stepper_pin_modulo = 0; 
    int n_max_stepper_pins = 4; 
    int a_b_stepper_pin_number[4] = {4,5,6,7};

    double n_microseconds_arduino_void_loop = 7.0;
    
    int n_potentiometer_input_pin = 0; 

    void setup()
    {

        Serial.begin(9600);
    

        // pinMode(n_potentiometer_input_pin, INPUT);
        int n_i = 0; 
        while(n_i < 4){
            pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
            n_i++; 
        }

    }
    float n_delay_milliseconds_minimum = 2.0; 
    float n_delay_milliseconds_maximum = 20.0;
    float n_delay_milliseconds = 2.0;
    float n_delay_milliseconds_int = 2;
    float n_delay_microseconds_int = 0; 
    int n_stepper_time = 0;
    void loop()
    {   

        float n_potentiometer_input_value = analogRead(A0);
        float n_potentiometer_input_value_normalized = n_potentiometer_input_value / 1024.0;
        float n_potentiometer_input_value_normalized_signed = n_potentiometer_input_value_normalized - 0.5;

        if(n_time > n_max_stepper_pins-1){
            n_time = 0; 
        }

        if(n_potentiometer_input_value_normalized_signed > 0.0){
            n_stepper_time = n_time; // 0, 1, 2, 3 , 0 , 1, 2, 3, 0, ...
        }else{
            n_stepper_time = n_max_stepper_pins-1 - n_time; // 3, 2, 1, 0, 3, 2, 1, 0 , 3, 2, .... 
        }
        // Serial.println("n_stepper_time");
        // Serial.println(n_stepper_time);
        n_stepper_pin_modulo = n_stepper_time;
        // n_stepper_pin_modulo = (n_stepper_time % n_max_stepper_pins);
        int n_i = 0;
        while(n_i < n_max_stepper_pins){
            digitalWrite(a_b_stepper_pin_number[n_i], n_stepper_pin_modulo == n_i);
            n_i++;
        }
            

        float n_potentiometer_input_value_normalized_signed_absolute = fabs(
            n_potentiometer_input_value_normalized_signed
        );


        n_time++;  

        float n_potentiometer_input_value_normalized_signed_absolute_normalized = 
            n_potentiometer_input_value_normalized_signed_absolute / 0.5;
        float n_potentiometer_input_value_normalized_signed_absolute_normalized_inverted = 
            1.0 - n_potentiometer_input_value_normalized_signed_absolute_normalized;

        // Serial.println("n_potentiometer_input_value_normalized_signed_absolute_normalized_inverted");
        // Serial.println(n_potentiometer_input_value_normalized_signed_absolute_normalized_inverted);

        n_delay_milliseconds = (n_delay_milliseconds_minimum + (n_potentiometer_input_value_normalized_signed_absolute_normalized_inverted * n_delay_milliseconds_maximum));

        n_delay_milliseconds_int = (int) n_delay_milliseconds;
        n_delay_microseconds_int = (int) ((int)(n_delay_milliseconds*1000.0) % (int)1000);


        delay(n_delay_milliseconds_int);
        delayMicroseconds(n_delay_microseconds_int);

        // Serial.println("n_potentiometer_input_value");
        // Serial.println(n_potentiometer_input_value);
        // Serial.println("n_delay_milliseconds_int");
        // Serial.println(n_delay_milliseconds_int);
        // Serial.println("n_delay_microseconds_int");
        // Serial.println(n_delay_microseconds_int);


    }


