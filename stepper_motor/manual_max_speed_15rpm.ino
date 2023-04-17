//Includes the Arduino Stepper Library

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
int a_n_pin__stepper[
	n_len_a_n_pin__stepper
] = {
    4,
    5,
    6,
    7
};

long n_t_mic_sec_mod = 0; // mic = micro , sec = seconds, mod = modulo
long n_mod_t_mic_sec_mod = 1000000;
long n_mics_accumulator = 10;

long n_t_mic_sec_stepper = 0;
long n_t_mic_sec_stepper__last = 0;
long n_t_mic_sec_stepper__diff = 0;
double n_t_mic_sec_stepper__diff_thresh = 0.0;

double n_t_mic_sec_stepper__diff_thresh_max = 2500.; // one step per 1 000 000 microseconds -> one step per 1 000 milliseconds -> one step per 1 second

int n_idx_active_a_n_pin_stepper = 0; 

void setup() {

    int n_i = 0;
    while(n_i < 4){
        pinMode(a_n_pin__stepper[n_i], OUTPUT);
        n_i++; 
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

void loop() {
	n_step_count += 1;

	double n_direction_forward = 1.;
	double n_direction_backward = -1.;
	double n_direction = n_direction_forward;
	
	if((int)((double)n_step_count / (n_steps_per_revolution/1.)) % 2 == 0){
		// toggles direction every quarter of a rotation
		n_direction = n_direction_backward;
	}
	
	n_idx_active_a_n_pin_stepper = 
		(n_idx_active_a_n_pin_stepper + 1) % n_len_a_n_pin__stepper;
	
	int n_idx = n_idx_active_a_n_pin_stepper;
	if(n_direction == n_direction_backward){
		n_idx = (n_len_a_n_pin__stepper-1) - n_idx_active_a_n_pin_stepper;
	}
	f_write_step(
		a_n_pin__stepper, 
		n_idx
	);

  	delayMicroseconds(n_microseconds_delay_between_step_targetspeed);
}
