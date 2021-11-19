const int tmp_sensor_pin = A0;
const float baseline_tmp = 20.0;

float tmp_values_array[] = {};

void setup(){

    Serial.begin(9600);

    
}


void loop(){

    int tmp_sensor_value = analogRead(tmp_sensor_pin);
    //Serial.print("tmp sensor value: \n ");
    //Serial.print(tmp_sensor_value);

    float voltage_provided_to_sensor = 5.0;
    float sensor_max_value = 1024.0;
    float tmp_sensor_voltage = (tmp_sensor_value/sensor_max_value)* voltage_provided_to_sensor;

    float tmp_degrees_celcius = (tmp_sensor_voltage - .5) * 100;

    float tmp_sensor_degrees_max = 125;
    float tmp_sensor_degrees_min = -40;
    float tmp_sensor_degrees_range = tmp_sensor_degrees_max - tmp_sensor_degrees_min;
    
    float tmp_degrees_celcius_2 = (tmp_sensor_degrees_range / sensor_max_value) * tmp_sensor_value + tmp_sensor_degrees_min;

    //tmp_values_array.push(tmp_degrees_celcius);
 
  
      Serial.println(tmp_degrees_celcius);
      Serial.print(" ");


       //if(std::ssize(arr) > 5){
         // float tmp_values_array[] = {};
        //}

        delay(1);
    //Serial.print("tmp_degrees_celcius: \n ");
    //Serial.print(tmp_degrees_celcius);

    
    //Serial.print(" \n ");
    //Serial.print("tmp_degrees_celcius_2: \n ");
    //Serial.print(tmp_degrees_celcius_2);
    //Serial.print(" \n ");


}
