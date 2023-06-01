
#include <RH_ASK.h>
#include <SPI.h> 
 
RH_ASK rf_driver;
 
#define n_pin_data_transmit 12 // connect sender pin to digital 12
void setup()
{
    Serial.begin(9600);

    rf_driver.init();
    pinMode(13, OUTPUT);

}
int n_idx = 0;
void f_send(
    int n
){
    const char *s_msg = "0";
    if(n > 0){
        s_msg = "1";
    }
    rf_driver.send((uint8_t *)s_msg, strlen(s_msg));
    rf_driver.waitPacketSent();
    Serial.println("data sent");
    Serial.println(s_msg);

    digitalWrite(13, n);
}
void loop()
{

    f_send(1);
    delay(222);
    f_send(0);
    delay(222);
    f_send(1);
    delay(222);
    f_send(0);
    delay(222);
    
    f_send(1);
    delay(1111);
    f_send(0);
    delay(1111);
    f_send(1);
    delay(1111);
    f_send(0);
    delay(1111);


}