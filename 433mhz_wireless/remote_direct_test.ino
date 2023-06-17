

#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;

#define n_pin_data_transmit 12 // connect sender pin to digital 12

#define n_pin_direction_right 2
#define n_pin_pwm_right 3
#define n_pin_direction_left 4
#define n_pin_pwm_left 5
struct O_controller
{
    float n_right_y_nor;
    float n_left_y_nor;
} o_controller;

byte tx_buf[sizeof(o_controller)] = {0};


void setup()
{
    Serial.begin(9600); // Debugging only
    rf_driver.init();
    pinMode(13, OUTPUT);
    pinMode(n_pin_direction_right, OUTPUT);
    pinMode(n_pin_pwm_right, OUTPUT);
    pinMode(n_pin_direction_left, OUTPUT);
    pinMode(n_pin_pwm_left, OUTPUT);

    o_controller.n_left_y_nor = 0.0;
    o_controller.n_right_y_nor = 0.0;
}
int n_idx = 0;

void loop()
{
    int n_right_y = analogRead(A4);
    int n_left_y = analogRead(A5);

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

    // Serial.println(o_controller.n_right_y_nor);
    Serial.println("sender");
    Serial.println(o_controller.n_right_y_nor);

    memcpy(tx_buf, &o_controller, sizeof(o_controller));
    byte n_bytes = sizeof(o_controller);
    rf_driver.send((uint8_t *)tx_buf, n_bytes);

    rf_driver.waitPacketSent();
}