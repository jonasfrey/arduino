

void setup() {

  // Set the maximum speed in steps per second:
  Serial.begin(9600);



}

double f_n_speed_signed_nor(
    int n_speed
){

  double n_nor = (((float)n_speed / 1024.0) - .5) * 2.;
  return n_nor;//returns from -1.0 to 1.0 

}

void loop() {

    int n_x = analogRead(A1);
    int n_y = analogRead(A2);
    int n_z = analogRead(A3);
    


    Serial.print(-1000); // To freeze the lower limit
    Serial.print(" ");
    Serial.print(1000); // To freeze the upper limit
    Serial.print(" ");
    Serial.println(n_x); 
    Serial.println(n_y); 
    Serial.println(n_z); 
}

