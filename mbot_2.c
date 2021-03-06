/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    DCMotorDriverTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me DC motor device.
 *
 * Function List:
 *    1. void MeDCMotor::run(int16_t speed)
 *    2. void MeDCMotor::stop(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeMCore.h"

MeLineFollower lineFinder(PORT_3); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */
MeRGBLed led;

MeDCMotor motorLeft(PORT_1);
MeDCMotor motorRight(PORT_2);

MeDCMotor motorLeft1(M1);
MeDCMotor motorRight1(M2);


MeUltrasonicSensor ultra_sonic_sensor(PORT_7);

int crazy_mode_countdown = 100; 

uint8_t buzpin = 8;

uint8_t motorSpeed = 100;
uint8_t max_color_rgb = 255;

int motor_speed_max = 255;
int motor_speed_min = -255;

int led_rgb_max = 255;

int motor_left_index = 1;
int led_left_index = 0;

int motor_right_index = 2;
int led_right_index = 1;

int loop_counter_ms = 0;
int loop_counter_microseconds = 0;

float motor_left_speed = 0;
float motor_right_speed = 0;

int buzzer_ts_microsec_now = 0; 
int buzzer_ts_microsec_last = 0; 
int buzzer_ts_microsec_delta_last_now = 0; 

        
// 1 = 100% , 0 = 0%, 0.5 = 50%
int forward_speed_normalized = 0.5;

//has to be 1 to correctly calculate with buzzer frequencies
int loop_delay_ms = 1; 

// history of the sensor states
int line_finder_states_array[100];

struct Led_rgb {
    float r;
    float g;
    float b;
};

struct Line_sensor {
    int on;
};

struct M_bot {
    float speed_left;
    float speed_right;
    struct Led_rgb led_left;
    struct Led_rgb led_right;

    struct Line_sensor line_sensor_left;
    struct Line_sensor line_sensor_right;
};
    struct M_bot m_bot = {
        0.0,
        0.0,
        {0.0 ,0.0 ,0.0} ,
        {0.0 ,0.0 ,0.0} ,
        0,
        0
    };

void setup()
{

    Serial.begin(9600);
    int motor_left_index = 1;
    int led_left_index = 0;

    int motor_right_index = 2;
    int led_right_index = 1;

    led.setpin(13);

    pinMode(buzpin,OUTPUT);

}

void loop()
{
    // last loop        this loop
    // [0,1,2,3,2,3] -> [0,0,1,2,3,2]
    // shift array to right 
    // for(int i = sizeof(line_finder_states_array)-1; i>0; i--){
    //     line_finder_states_array[i] = line_finder_states_array[i-1];
    // }
    //    line_finder_states_array[0] = lineFinder.readSensors();
    //    int sensors_delay_index = 10;
    //    int sensor_state_delayed = line_finder_states_array[sensors_delay_index];
    //    int sensor_state = sensor_state_delayed;

    int sensor_state = lineFinder.readSensors();


    if(sensor_state == 0) {
        m_bot.line_sensor_left.on = 1;
        m_bot.line_sensor_right.on = 1;
    }
    if(sensor_state == 1) {
        m_bot.line_sensor_left.on = 0;
        m_bot.line_sensor_right.on = 1;
    }
    if(sensor_state == 2) {
        m_bot.line_sensor_left.on = 1;
        m_bot.line_sensor_right.on = 0;
    }
    if(sensor_state == 3) {        
        m_bot.line_sensor_left.on = 0;
        m_bot.line_sensor_right.on = 0;
    }

    if(m_bot.line_sensor_left.on && m_bot.line_sensor_right.on){
        m_bot.speed_left = forward_speed_normalized;
        m_bot.speed_right = forward_speed_normalized;
    }
    Serial.println(printf("line sensor left right: %i : %i", m_bot.line_sensor_left.on, m_bot.line_sensor_right.on));
    Serial.println(printf("sensor state: %i ", sensor_state));

    if(!m_bot.line_sensor_left.on){
        m_bot.speed_right = 0.0;
    }
    if(!m_bot.line_sensor_right.on){
        m_bot.speed_left = 0.0;
    }

    if(!m_bot.line_sensor_left.on && !m_bot.line_sensor_right.on){
        // either the line has stopped or the mbot 
        // was driving out of the line
        m_bot.speed_right = 0.0;
        m_bot.speed_left = 0.0;
    }

    
    update_m_bot(m_bot);
    
    delay(1);
    
    loop_counter_ms ++;


}

int toggle_every_nth_ms(int toggle_every_nth, int infinit_incrementing_number){
    
    // Serial.println("toggle every 20 ms :");
    // Serial.println( int((infinit_incrementing_number / float(toggle_every_nth))) );
    
    return int(int((infinit_incrementing_number / float(toggle_every_nth))) % 2 == 0);

}

void update_m_bot(M_bot m_bot){
    
    setMotorSpeed(motor_right_index,m_bot.speed_right);
    setMotorSpeed(motor_left_index,m_bot.speed_left);

    if(m_bot.speed_left == 0){
        stopMotor(motor_left_index);
    }

    if(m_bot.speed_right == 0){
        stopMotor(motor_right_index);
    }

    led.setColorAt(
        led_left_index, 
        m_bot.led_left.r * led_rgb_max, 
        m_bot.led_left.g * led_rgb_max, 
        m_bot.led_left.b * led_rgb_max
    );
    led.setColorAt(
        led_right_index, 
        m_bot.led_right.r * led_rgb_max, 
        m_bot.led_right.g * led_rgb_max, 
        m_bot.led_right.b * led_rgb_max
    );
    
    led.show();
}

void setMotorSpeed(int motorIndex, float speed)
{
    // un normalize speed 
    // 0.5 -> 128 
    // -0.25 -> -64
    speed = speed * motor_speed_max;
    
    speed = speed * -1;

    if(motorIndex == motor_right_index){
        speed = speed * -1;
        
        motorRight.run(speed);
        motorRight1.run(speed);
    }

    if(motorIndex ==  motor_left_index){

        motorLeft.run(speed);
        motorLeft1.run(speed);
    }


}

void stopMotor(int motorIndex){
    if(motorIndex ==  motor_right_index){
        motorRight.stop();
        motorRight1.stop();
    }
    if(motorIndex ==  motor_left_index){
        motorLeft.stop();
        motorLeft1.stop();
    }
}

void makeTurn(char direction[], int radius, float speed){

    if(direction == "right"){
        int motor_index = motor_right_index;
    }

    if(direction == "left"){
        int motor_index = motor_left_index;
    }

    // the radius depends on ratio between motors 
    // m1 = 1, m2 = 1, radius = infinity
    // m1 = 1, m2 = -1, radius = 0 
    // m1 = 1, m2 = 0, radius = (~80mm, distance from wheel to wheel)
    // m1 = 1, m2 = 0.5, radius = ... 

    // i think tangens , sinus or cosinus is needed here...
}

//we have to use this library because tone() not working with MeMcore...
void myTone(byte pin, uint16_t frequency, uint16_t duration)
{ // input parameters: Arduino pin number, frequency in Hz, duration in milliseconds
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  pinMode(pin,OUTPUT);
  while (millis()-startTime< duration)
  {
    digitalWrite(pin,HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin,LOW);
    delayMicroseconds(halfPeriod);
  }
  pinMode(pin,INPUT);
}
