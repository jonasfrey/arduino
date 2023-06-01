#include <RH_ASK.h>
#include <SPI.h> 
 
RH_ASK rf_driver;
 
#define n_pin_data_receive 11 // connect receiver pin to digital 11
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


}