#define A 2
#define B 3
#define C 4
#define D 5
 
#define NUMBER_OF_STEPS_PER_REV_FUllSTEPPING 2038
  // b = arctan(a/b)
  // b = arctan(420/1)
  //   
  //       +
  //      /| 
  //     /b|b =  
  //    /  |
  //  C/   |A = 420mm 
  //  /   y|y = gamma = 90deg
  // +-----+
  //  B=1 


  int number_of_steps_per_rev_fullstepping = 2048;
  int number_of_steps_per_rev_halfstepping = number_of_steps_per_rev_fullstepping * 2;
  
  int distance_hinge_to_camera_millimeter = 234;
  int distance_screw_traveling_in_one_revolution_millimeter = 1;
  
  int a = distance_hinge_to_camera_millimeter;
  int b = distance_screw_traveling_in_one_revolution_millimeter;
  
  double radians_hinge_per_screw_revolution = atan(a/b);
  double degrees_hinge_per_screw_revolution = (radians_hinge_per_screw_revolution / 6.28318)*360;

  // sidereal period 23 hours, 56 minutes and 4.09053 seconds
  double degrees_earth_rotation_per_minute = 360/(23*60+56+(4.09/60.0));

  double revolutions_per_minute_to_match_sidereal_period = degrees_earth_rotation_per_minute / degrees_hinge_per_screw_revolution;

  double steps_per_minute_to_match_sidereal_period = revolutions_per_minute_to_match_sidereal_period * number_of_steps_per_rev_halfstepping;

  //double microseconds_per_step = (60*1000*1000) / steps_per_minute_to_match_sidereal_period;

//  int substeps_per_step = 8;
//  
//  double microseconds_per_substep = microseconds_per_step / substeps_per_step;
  
  unsigned long microseconds_per_minute = 10L*1000L*1000L;

  double microseconds_between_step_to_match_one_revolution_per_minute_when_fullstepping = microseconds_per_minute / number_of_steps_per_rev_fullstepping;

void setup(){

  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void write_stepper_pins(int a,int b,int c,int d){
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}

void halfstepping(float microseconds_per_substep){
  write_stepper_pins(1,0,0,0);
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(1,1,0,0);//halfstepping
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(0,1,0,0);
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(0,1,1,0);//halfstepping
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(0,0,1,0);
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(0,0,1,1);//halfstepping
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(0,0,0,1);
  delayMicroseconds(microseconds_per_substep);
  write_stepper_pins(1,0,0,1);//halfstepping
  delayMicroseconds(microseconds_per_substep);
}

void fullstepping(float microseconds_per_fullstep, int direction_forward){

  if(direction_forward){
    write_stepper_pins(1,0,0,0);
    delayMicroseconds(microseconds_per_fullstep);
    write_stepper_pins(0,1,0,0);
    delayMicroseconds(microseconds_per_fullstep);
    write_stepper_pins(0,0,1,0);
    delayMicroseconds(microseconds_per_fullstep);
    write_stepper_pins(0,0,0,1);
    delayMicroseconds(microseconds_per_fullstep);    
  }else{
    write_stepper_pins(0,0,0,1);
    delayMicroseconds(microseconds_per_fullstep);    
    write_stepper_pins(0,0,1,0);
    delayMicroseconds(microseconds_per_fullstep);
    write_stepper_pins(0,1,0,0);
    delayMicroseconds(microseconds_per_fullstep);
    write_stepper_pins(1,0,0,0);
    delayMicroseconds(microseconds_per_fullstep);
  }
    

}

void loop(){
    while(1){
      fullstepping(microseconds_between_step_to_match_one_revolution_per_minute_when_fullstepping);
    }
}
