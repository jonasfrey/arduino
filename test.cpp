
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
// int n_distance_hinge_to_screw_millimeter = 492; // 
// int n_distance_hinge_to_screw_millimeter = 472; // 484 with 2048 steps was to fast, so we test 472
// int n_distance_hinge_to_screw_millimeter = 420; // 420 haha lol xD 
// int n_distance_hinge_to_screw_millimeter = 320; // 320 relly now!!
// int n_distance_hinge_to_screw_millimeter = 220; //wtf i dont see no fcking difference
// int n_distance_hinge_to_screw_millimeter = 500; //ok i give up for today
// int n_distance_hinge_to_screw_millimeter = 550; //and here we go again 
// int n_distance_hinge_to_screw_millimeter = 600; //and here we go again 

int n_distance_screw_traveling_in_one_revolution_millimeter = 1;
  
int a = n_distance_screw_traveling_in_one_revolution_millimeter;
int b = n_distance_hinge_to_screw_millimeter;

double n_tau = M_PI * 2;
double n_radians_hinge_per_screw_revolution = atan((double) a/(double) b); // ca 0.00206611

int n_degrees_max = 360; 
double n_degrees_hinge_per_screw_revolution = (n_radians_hinge_per_screw_revolution / n_tau )*n_degrees_max; // ca 0.11837954
//The earth rotates once every 23 hours, 56 minutes and 4.09053 seconds
double n_degrees_earth_rotation_per_second = n_degrees_max/(23*60*60+56*60+4.09);

double n_revolutions_per_second_to_match_sidereal_period = n_degrees_earth_rotation_per_second / n_degrees_hinge_per_screw_revolution;

double n_seconds_per_revolution_to_match_sidereal_period = 1.0/n_revolutions_per_second_to_match_sidereal_period;

int a_b_stepper_pin_number[4] = {9,10,11,12};

int step_number = 0;

// push button stuff 
int n_pin_led_red = 7;
int n_pin_led_green = 8;
int n_pin_button_directiontoggler = 3; 
int b_button_directiontoggler = 0;
int b_direction = 0; 

int n_rewind_delay_microseconds = 2009;// lower number, faster rewind

int b_button_directiontoggler_last = 0; 
// end push button stuff

void setup()
{

    int n_i = 0; 
    while(n_i < 4){
        pinMode(a_b_stepper_pin_number[n_i], OUTPUT);
        n_i++; 
    }

  pinMode(n_pin_led_red, OUTPUT); 
  pinMode(n_pin_led_green, OUTPUT); 
  pinMode(n_pin_button_directiontoggler, INPUT);

  Serial.begin(9600);


}
long n_time = 0; 

int n_max_stepper_pins = 4;

int a_b_stepper_pin_voltage[4] = {0,0,0,0};

long n_stepper_pin_modulo = 0; 

int n_steps_for_full_resolution = 2048;
// int n_steps_for_full_resolution = 2038; //i have seen both, 2048 and 2038, after testing 2038 is better, might have to do with wrong n_distance_hinge_to_screw_millimeter

float n_seconds_per_revolution = n_seconds_per_revolution_to_match_sidereal_period;

int n_minimum_millisecond_delay_between_steps = 2;

// 10sec * 1000ms = 10000ms / 2048steps = 4.88...
int n_calculated_delay_microseconds_for_seconds_per_revolution = ((float) n_seconds_per_revolution*(1000) / n_steps_for_full_resolution)*1000; 
int b_backwards = 1; // direction



int n_delay_microseconds = 0; 
void loop()
{
    
    Serial.println("n_radians_hinge_per_screw_revolution");
    Serial.println(n_radians_hinge_per_screw_revolution, 10);
    Serial.println("n_degrees_hinge_per_screw_revolution");
    Serial.println(n_degrees_hinge_per_screw_revolution, 10);
    Serial.println("n_revolutions_per_second_to_match_sidereal_period");
    Serial.println(n_revolutions_per_second_to_match_sidereal_period, 10);
    Serial.println("n_seconds_per_revolution_to_match_sidereal_period");
    Serial.println(n_seconds_per_revolution_to_match_sidereal_period, 10);
    Serial.println("n_calculated_delay_microseconds_for_seconds_per_revolution");
    Serial.println(n_calculated_delay_microseconds_for_seconds_per_revolution);
  
    // push button stuff 
    b_button_directiontoggler = digitalRead(n_pin_button_directiontoggler);

    Serial.println(b_button_directiontoggler);

    if(b_button_directiontoggler){
        if(b_button_directiontoggler_last != b_button_directiontoggler){
    
            b_direction = !b_direction;
        }
        // fast rewind if button is pressed
        n_delay_microseconds = n_rewind_delay_microseconds;
    }else{
        // normal calculated sidereal rewind if button not pressed
        n_delay_microseconds = n_calculated_delay_microseconds_for_seconds_per_revolution;
    }

    if(b_direction){
        digitalWrite(n_pin_led_red, 1); // Turn the LED on
        digitalWrite(n_pin_led_green, 0); // Turn the LED on
    }else{
        digitalWrite(n_pin_led_red, 0); // Turn the LED on
        digitalWrite(n_pin_led_green, 1); // Turn the LED on
    }
    b_backwards = b_direction;
    
    b_button_directiontoggler_last = b_button_directiontoggler;
    // end push button stuff

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
    delayMicroseconds(n_delay_microseconds);

}


