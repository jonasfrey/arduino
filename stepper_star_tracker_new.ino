
#define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038 // ive seen both 2048 and 2038

  // b = arctan(a/b)
  // b = arctan(484/1)
  //   
  //       +
  //      /| 
  //     /b|b =  
  //    /  |
  //  C/   |A = 484mm (distance hinge to screw)
  //  /   y|y = gamma = 90deg
  // +-----+
  //  B=1 (distance screw traveling in one millimeter) 


int n_distance_hinge_to_screw_millimeter = 484;
int n_distance_screw_traveling_in_one_revolution_millimeter = 1;
  
int a = n_distance_hinge_to_screw_millimeter;
int b = n_distance_screw_traveling_in_one_revolution_millimeter;

double n_tau = M_PI * 2;
double n_radians_hinge_per_screw_revolution = atan(a/b);

int n_degrees_max = 360; 
double n_degrees_hinge_per_screw_revolution = (n_radians_hinge_per_screw_revolution / n_tau )*n_degrees_max;

double n_degrees_earth_rotation_per_second = n_degrees_max/(23*60+56+(4.09/60.0));

double n_revolutions_per_second_to_match_sidereal_period = n_degrees_earth_rotation_per_second / n_degrees_hinge_per_screw_revolution;

double n_seconds_per_revolution_to_match_sidereal_period = 1.0/n_revolutions_per_second_to_match_sidereal_period;

int a_b_stepper_pin_number[4] = {9,10,11,12};

int step_number = 0;
void setup()
{
    int n_i = 0; 
    while(n_i < 4){
        pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
        n_i++; 
    }

}
long n_time = 0; 

int n_max_stepper_pins = 4;

int a_b_stepper_pin_voltage[4] = {0,0,0,0};

long n_stepper_pin_modulo = 0; 

int n_steps_for_full_resolution = 2048;

float n_seconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period;

int n_minimum_millisecond_delay_between_steps = 2;

// 10sec * 1000ms = 10000ms / 2048steps = 4.88...
int n_calculated_delay_microseconds_for_seconds_per_revolution = ((float) n_seconds_per_revolution*(1000) / n_steps_for_full_resolution)*1000; 
int b_backwards = 1; // direction



void loop()
{


    // one loop counts as one step!!
    n_time++;  
    n_stepper_pin_modulo = abs((3*b_backwards)-(n_time % n_max_stepper_pins));
    int n_i = 0; 
    while(n_i < 4){
        digitalWrite(a_b_stepper_pin_number[n_i], n_stepper_pin_modulo == n_i);
        n_i++;
    }
    
    if(n_time >= (__LONG_MAX__)){
        n_time = 0; 
    }
    // delayMicroseconds(n_calculated_delay_microseconds_for_seconds_per_revolution);
    delayMicroseconds(3000); // tmp winding back up 

}
