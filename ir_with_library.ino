/*      
*  IR read codes     
*  by Hanie kiani     
*  https://electropeak.com/learn/        
*/     
#include <IRremote.h>     
int RECV_PIN =12;     
     
IRrecv irrecv(RECV_PIN);     
decode_results results;     
void setup(){     
 Serial.begin(9600);     
 irrecv.enableIRIn();      
}     
void loop(){     
   if (irrecv.decode(&results)){     
int value = results.value;     
Serial.println(value);      


       irrecv.resume();      
   }     
}      

