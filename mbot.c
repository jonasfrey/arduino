#include "MeMCore.h"

// MeMCore library API stuff, port numbers may have to be changed
MeRGBLed led;
MeDCMotor motorLeft(PORT_1);
MeDCMotor motorRight(PORT_2);
MeDCMotor motorLeft1(M1);
MeDCMotor motorRight1(M2);
MeLineFollower lineFinder(PORT_4); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */
MeUltrasonicSensor ultra_sonic_sensor(PORT_7);

int crazy_mode_countdown = 100; 

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
// Currently, the largest value that will produce an accurate delay when using delayMicroseconds is 16383
// This function works very accurately in the range 3 microseconds and up to 16383. We cannot assure that delayMicroseconds will perform precisely for smaller delay-times. Larger delay times may actually delay for an extremely brief time. As of Arduino 0018, delayMicroseconds() no longer disables interrupts.
int loop_counter_us = 0; 
int loop_delay_ms = 1; 

// normalized means all values for example from 0-255 get mapped to 0-1 (for humans 0-100%)
// 1 => 100% => 255, 0 = 0% => 0, 0.5 = 50% => 128

int forward_speed_normalized = 0.5;

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

struct Motor {
    float speed;
};

struct Buzzer {
    int pin; 
    int frequency
    float pulsewidth_normalized;
}

struct M_bot {

    struct buzzer; 
    struct motor_left; 
    struct motor_right; 

    struct Led_rgb led_left;
    struct Led_rgb led_right;

    struct Line_sensor line_sensor_left;
    struct Line_sensor line_sensor_right;

};
    struct M_bot m_bot = {
        
        {8,440,0.5}, 
        0.0,//this is a struct but since it has only one member variable it doesnt need {}, it event will throw an error if curly brackets are used
        0.0,
        {0.0 ,0.0 ,0.0},
        {0.0 ,0.0 ,0.0},
        0,
        0
    };
void setup()
{
    Serial.begin(9600);
    led.setpin(13);
    pinMode(m_bot.buzzer.pin,OUTPUT);
}

void one_ms_loop(){
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
    m_bot.line_sensor_state = sensor_state;

    //update m_bot line sensors left and right
    if(sensor_state == 0) {
        m_bot.line_sensor_left.on = 1.0;
        m_bot.line_sensor_right.on = 1.0;
    }
    if(sensor_state == 1) {
        m_bot.line_sensor_left.on = 0.0;
        m_bot.line_sensor_right.on = 1.0;
    }
    if(sensor_state == 2) {
        m_bot.line_sensor_left.on = 1.0;
        m_bot.line_sensor_right.on = 0.0;
    }
    if(sensor_state == 3) {        
        m_bot.line_sensor_left.on = 0.0;
        m_bot.line_sensor_right.on = 0.0;
    }

    // follow line
    if(m_bot.line_sensor_left.on && m_bot.line_sensor_right.on){
        m_bot.motor_left.speed = .5;
        m_bot.motor_right.speed = 0.5;
    }
    if(!m_bot.line_sensor_left.on){
        m_bot.motor_right.speed = 0.0;
    }
    if(!m_bot.line_sensor_right.on){
        m_bot.motor_left.speed = 0.0;
    }
    if(!m_bot.line_sensor_left.on && !m_bot.line_sensor_right.on){
        // either the line has stopped or the mbot 
        // was driving out of the line
        m_bot.motor_right.speed = 0.0;
        m_bot.motor_left.speed = 0.0;
    }

    // fooling around 
    m_bot.motor_right.speed = 1.0;
    // float frequency_factor_modulated = ((sin(loop_counter_ms * 0.1)*2.0)-1.0);
    m_bot.motor_left.speed = sin(loop_counter_ms * 0.05) * 1.0;
    m_bot.motor_right.speed = cos(loop_counter_ms * 0.05) * 0.5 + 0.5;
    // m_bot.motor_left.speed = (loop_counter_ms % 1000) * 0.001;
    Serial.println("asdf :");
    Serial.println( m_bot.motor_left.speed );
    // default stuff 

    if(toggle_every_nth_ms( 20, loop_counter_ms)){
        m_bot.buzzer.frequency = 1000;
    }else{
        m_bot.buzzer.frequency = 1400;
    }

    if(toggle_every_nth_ms(5, loop_counter_ms)){
    
        m_bot.led_left.r = random(100)*0.01;
        m_bot.led_left.g = random(100)*0.01;
        m_bot.led_left.b = random(100)*0.01;
        m_bot.led_right.r = random(100)*0.01;
        m_bot.led_right.g = random(100)*0.01;
        m_bot.led_right.b = random(100)*0.01;
    }

    update_m_bot(m_bot);
    
}

// us (microseconds) needed for the PWM pulse width modulation of the buzzer
void one_us_loop(){
    float modulo_microseconds = 1000000f/m_bot.buzzer.frequency;
    if( loop_counter_us % modulo_microseconds < modulo_microseconds * m_bot.buzzer.pulsewidth_normalized ){
        // duty cycle
        digitalWrite(m_bot.buzzer.pin,HIGH);
    }else{
        digitalWrite(m_bot.buzzer.pin,LOW);
    }
}

void loop()

{

    loop_counter_ms = 0;
    // Currently, the largest value that will produce an accurate delay when using delayMicroseconds is 16383
    // This function works very accurately in the range 3 microseconds and up to 16383. We cannot assure that delayMicroseconds will perform precisely for smaller delay-times. Larger delay times may actually delay for an extremely brief time. As of Arduino 0018, delayMicroseconds() no longer disables interrupts.
    int loop_counter_us = 0; 
    int loop_delay_ms = 1;

    if(loop_counter_us % 1000 == 0){
        loop_counter_ms++; 
        one_ms_loop();
    }
    one_us_loop();
    loop_counter_us++; 
    delayMicroseconds(1); 
}

int toggle_every_nth_ms(int toggle_every_nth, int infinit_incrementing_number){
    // Serial.println("toggle every 20 ms :");
    // Serial.println( int((infinit_incrementing_number / float(toggle_every_nth))) );
    return int(int((infinit_incrementing_number / float(toggle_every_nth))) % 2 == 0);
}

void update_m_bot(M_bot m_bot){
    setMotorSpeed(motor_right_index,m_bot.motor_right.speed);
    setMotorSpeed(motor_left_index,m_bot.motor_left.speed);
    if(m_bot.motor_left.speed == 0){
        stopMotor(motor_left_index);
    }
    if(m_bot.motor_right.speed == 0){
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
