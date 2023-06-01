

#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;

#define n_pin_data_transmit 12 // connect sender pin to digital 12

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
    // Serial.println(o_controller.n_right_y_nor);
    Serial.println("sender");
    Serial.println(o_controller.n_left_y_nor);

    memcpy(tx_buf, &o_controller, sizeof(o_controller));
    byte n_bytes = sizeof(o_controller);
    rf_driver.send((uint8_t *)tx_buf, n_bytes);

    rf_driver.waitPacketSent();
}