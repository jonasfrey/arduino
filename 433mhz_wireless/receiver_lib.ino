#include <RH_ASK.h>
#include <SPI.h> 
 
RH_ASK rf_driver;
 
#define n_pin_data_receive 11 // connect receiver pin to digital 11

void setup()
{
    rf_driver.init();
    Serial.begin(9600);
    pinMode(13, OUTPUT);

}
 
void loop()
{
    // Set buffer to size of expected message
    uint8_t a_buffer[11];
    uint8_t buflen = sizeof(a_buffer);
    // Check if received packet is correct size
    if (rf_driver.recv(a_buffer, &buflen))
    {
      
      // Message received with valid checksum
      Serial.print("Message Received: ");
      Serial.println((char*)a_buffer);
      char * s_msg = (char*)a_buffer;
      if(s_msg[0] == '0'){
        digitalWrite(13, 0);
      } 
      if(s_msg[0] == '1'){
        digitalWrite(13, 1);
      }
    }else{
      Serial.println("no data received :(");
    }
}