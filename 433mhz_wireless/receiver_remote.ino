#include <RH_ASK.h>
#include <SPI.h> 
 
RH_ASK rf_driver;
 
#define n_pin_data_receive 11 // connect receiver pin to digital 11


#define n_pin_direction_right 2
#define n_pin_pwm_right 3
#define n_pin_direction_left 4
#define n_pin_pwm_left 5

struct O_controller
{
    float n_right_y_nor;
    float n_left_y_nor;
} o_controller;

void setup()
{
    rf_driver.init();
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(n_pin_direction_right, OUTPUT);
    pinMode(n_pin_pwm_right, OUTPUT);
    pinMode(n_pin_direction_left, OUTPUT);
    pinMode(n_pin_pwm_left, OUTPUT);
}
 
void loop()
{
    // Set buffer to size of expected message
    uint8_t a_buffer[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(a_buffer);
    // Check if received packet is correct size
    if (rf_driver.recv(a_buffer, &buflen))
    {

        memcpy(&o_controller, a_buffer, sizeof(o_controller));
        Serial.println("receiver");
        Serial.println(o_controller.n_left_y_nor);

        digitalWrite(13, 1);


    }else{
        digitalWrite(13, 0);
    }


    o_controller.n_right_y_nor = (((float)n_right_y / 1024.0) - .5) * 2.;
    o_controller.n_left_y_nor = (((float)n_left_y / 1024.0) - .5) * 2.;
    if(o_controller.n_right_y_nor < 0){
        digitalWrite(n_pin_direction_right, 1);
    }else{
        digitalWrite(n_pin_direction_right, 0);
    }
    if(o_controller.n_left_y_nor < 0){
        digitalWrite(n_pin_direction_left, 1);
    }else{
        digitalWrite(n_pin_direction_left, 0);
    }


    analogWrite(n_pin_pwm_right, (int)(abs(o_controller.n_right_y_nor*255.0)));
    analogWrite(n_pin_pwm_left, (int)(abs(o_controller.n_left_y_nor*255.0)));

}

