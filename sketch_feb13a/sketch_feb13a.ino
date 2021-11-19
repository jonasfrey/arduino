
int switch_pressed = 0;


void setup(){
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(2, INPUT);
}
void loop(){

  switch_pressed = digitalRead(2);


  if(switch_pressed){
    digitalWrite(3, 1);
    digitalWrite(4, 0);
    digitalWrite(5, 0);
  }

  if(!switch_pressed){

    digitalWrite(3, 0);
    digitalWrite(4, 0);
    digitalWrite(5, 1);

    delay(100);

    digitalWrite(4, 1);
    digitalWrite(5, 0);

    delay(100);

  }

}
