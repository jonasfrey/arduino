// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;


struct dataStruct{
  float press_norm ; 
  float press_hg;
  float temp;
  unsigned long counter;
   
}myData;

byte tx_buf[sizeof(myData)] = {0};

void setup()
{
    Serial.begin(9600);	  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
         
   myData.press_norm=1000.11; 
myData.press_hg=0.59; 
myData.temp=22.394; 
         
}

void loop()
{
     

  memcpy(tx_buf, &myData, sizeof(myData) );
  byte zize=sizeof(myData);

driver.send((uint8_t *)tx_buf, zize);

   // driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    myData.counter++;
    delay(2000);
}