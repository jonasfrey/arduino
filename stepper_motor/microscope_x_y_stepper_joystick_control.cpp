
#define n_pin_ain_potentiometer_x  A0 // Arduino pin connected to VRX pin
#define n_pin_ain_potentiometer_y  A1 // Arduino pin connected to VRY pin

// |tit|val|
// |---|---|
// |Operating Voltage|5VDC|
// |Operating Current|240mA (typical)|
// |Number of phases|4|
// |Gear Reduction Ratio|64:1|
// |Step Angle|5.625°/64|
// |Frequency|100Hz|
// |In-traction Torque|>34.3mN.m(120Hz)|
// |Self-positioning Torque|>34.3mN.m|
// |Friction torque|600-1200 gf.cm|
// |Pull in torque|300 gf.cm|  
// ![](./28BYJ48-Stepper-Motor-Gear-Ratio-Explanation.png)

// const int n_steps_per_revolution = 2048; // some people only say its 2048, 
const int n_steps_per_revolution = 2038; // some say its 2038...

double n_rpm_max = 15.0; // many people say max rpm for this stepper is 15.

double n_rpm_targetspeed = 15.0; // this is the 

double n_revolutions_per_second_targetspeed = n_rpm_targetspeed / 60.;
double n_steps_per_second_targetspeed = n_revolutions_per_second_targetspeed * n_steps_per_revolution;
double n_microseconds_delay_between_step_targetspeed = 1000000 / n_steps_per_second_targetspeed;
// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
const int n_len_a_n_pin__stepper = 4;

int a_n_pin__stepper_x[n_len_a_n_pin__stepper] = {
    4,
    5,
    6,
    7
};
int a_n_pin__stepper_y[n_len_a_n_pin__stepper] = {
    8,
    9,
    10,
    11
};

int n_idx_active_a_n_pin_stepper_x = 0; 
int n_idx_active_a_n_pin_stepper_y = 0; 

void setup() {

	for(int n_i = 0;n_i< n_len_a_n_pin__stepper; n_i+=1){
        pinMode(a_n_pin__stepper_x[n_i], OUTPUT);
	}
	for(int n_i = 0;n_i< n_len_a_n_pin__stepper; n_i+=1){
        pinMode(a_n_pin__stepper_y[n_i], OUTPUT);
	}

    Serial.begin(9600) ;
}


void f_write_step(
  int a_n_pin__stepper[], 
  int n_idx_active_a_n_pin_stepper
){
    int n_i = 0;
    while(n_i < n_len_a_n_pin__stepper){
        int b = n_i == n_idx_active_a_n_pin_stepper; 
        digitalWrite(a_n_pin__stepper[n_i], b);
        n_i++;
    }
}


long n_step_count = 0;

double n_mic_secs_slow_down = n_microseconds_delay_between_step_targetspeed*2.;
double n_speed_nor_x = 1.; // nor = normalized, 1. = max speed , 0. slowest speed 2 times slower than fastest speed, so a quarter the full speed
double n_speed_nor_y = 1.; // nor = normalized, 1. = max speed , 0. slowest speed 2 times slower than fastest speed, so a quarter the full speed

int n_mic_secs_delay = 200;
long n_mic_secs = 0;

long n_t_mic_sec_stepper_x = 0;
long n_t_mic_sec_stepper_x__last = 0;
long n_t_mic_sec_stepper_x__diff = 0;
double n_t_mic_sec_stepper_x__diff_thresh = 0.0;

long n_t_mic_sec_stepper_y = 0;
long n_t_mic_sec_stepper_y__last = 0;
long n_t_mic_sec_stepper_y__diff = 0;
double n_t_mic_sec_stepper_y__diff_thresh = 0.0;

void loop() {

    int n_x = analogRead(n_pin_ain_potentiometer_x);
    int n_y = analogRead(n_pin_ain_potentiometer_y);
    double n_x_nor = (((float) n_x / 1024.0)-.5)*2.;
    double n_y_nor = (((float) n_y / 1024.0)-.5)*2.;
    n_speed_nor_x = abs(n_x_nor);
    n_speed_nor_y = abs(n_y_nor);
    if(n_speed_nor_x < 0.1){
        n_speed_nor_x = 0.;
    }
    if(n_speed_nor_y < 0.1){
        n_speed_nor_y = 0.;
    }
	n_mic_secs += n_mic_secs_delay;
	if(n_mic_secs > 10000000){
		n_mic_secs = 0;//prevent overflow
		n_t_mic_sec_stepper_x = 0;
		n_t_mic_sec_stepper_x__last = 0;
		n_t_mic_sec_stepper_x__diff = 0;
		n_t_mic_sec_stepper_y = 0;
		n_t_mic_sec_stepper_y__last = 0;
		n_t_mic_sec_stepper_y__diff = 0;
	}
	
	n_t_mic_sec_stepper_x__diff_thresh =
		n_microseconds_delay_between_step_targetspeed
		+ n_mic_secs_slow_down * (1.-n_speed_nor_x);


	n_t_mic_sec_stepper_x = n_mic_secs; 
	n_t_mic_sec_stepper_x__diff = n_t_mic_sec_stepper_x - n_t_mic_sec_stepper_x__last;
	if(
		n_t_mic_sec_stepper_x__diff
		> n_t_mic_sec_stepper_x__diff_thresh

        && n_speed_nor_x != 0.
	){
		n_step_count += 1;

        double n_direction_forward = 1.;
        double n_direction_backward = -1.;
        double n_direction = n_direction_forward;
		if(n_x_nor < 0.){
            n_direction = n_direction_backward;
        }

		n_idx_active_a_n_pin_stepper_x = 
			(n_idx_active_a_n_pin_stepper_x + 1) % n_len_a_n_pin__stepper;
		
		int n_idx = n_idx_active_a_n_pin_stepper_x;
		if(n_direction == n_direction_backward){
			n_idx = (n_len_a_n_pin__stepper-1) - n_idx_active_a_n_pin_stepper_x;
		}
		f_write_step(
			a_n_pin__stepper_x, 
			n_idx
		);
		n_t_mic_sec_stepper_x__last = n_mic_secs;
	}


	n_t_mic_sec_stepper_y__diff_thresh =
		n_microseconds_delay_between_step_targetspeed;
		+ n_mic_secs_slow_down * (1.-n_speed_nor_y);
	n_t_mic_sec_stepper_y = n_mic_secs; 
	n_t_mic_sec_stepper_y__diff = n_t_mic_sec_stepper_y - n_t_mic_sec_stepper_y__last;
	if(
		n_t_mic_sec_stepper_y__diff
		> n_t_mic_sec_stepper_y__diff_thresh

        && n_speed_nor_y != 0.
	){
		n_step_count += 1;

        double n_direction_forward = 1.;
        double n_direction_backward = -1.;
        double n_direction = n_direction_forward;
		if(n_y_nor < 0.){
            n_direction = n_direction_backward;
        }

		
		n_idx_active_a_n_pin_stepper_y = 
			(n_idx_active_a_n_pin_stepper_y + 1) % n_len_a_n_pin__stepper;
		
		int n_idx = n_idx_active_a_n_pin_stepper_y;
		if(n_direction == n_direction_backward){
			n_idx = (n_len_a_n_pin__stepper-1) - n_idx_active_a_n_pin_stepper_y;
		}
		f_write_step(
			a_n_pin__stepper_y, 
			n_idx
		);
		n_t_mic_sec_stepper_y__last = n_mic_secs;
	}

  	delayMicroseconds(n_mic_secs_delay);
}
