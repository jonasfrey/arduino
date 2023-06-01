// Includes the Arduino Stepper Library

// |tit|val|
// |---|---|
// |Operating Voltage|?|
// |Operating Current|?|
// |Number of phases|?|
// |Gear Reduction Ratio|?|
// |Step Angle|?|
// |Frequency|?|
// |In-traction Torque|?|
// |Self-positioning Torque|?|
// |Friction torque|?|
// |Pull in torque|?|

#define n_pin_ain_potentiometer_x A0 // Arduino pin connected to VRX pin
#define n_pin_ain_potentiometer_y A1 // Arduino pin connected to VRY pin


struct O_nema_stepper
{
  double n_microstepping_divisor;
  double n_steps_per_revolution;
  double n_revolutions_per_minute_max;
  double n_revolutions_per_minute_min;
  double n_revolutions_per_minute;
  double n_speed_nor;
  int n_pin_step;
  int n_pin_direction;

  double n_revolutions_per_second;
  double n_microseconds_delay_between_step;
  double n_steps_per_second;

  unsigned long n_ts_mics;
  unsigned long n_ts_mics__last;
  unsigned long n_ts_mics__diff;

  double b_direction_forward;
  double b_direction_forward__last;
};

unsigned long f_n_ts_mics()
{
  // The micros() function returns the number of microseconds from the time, the Arduino board begins running the current program. This number overflows i.e. goes back to zero after approximately 70 minutes. On 16 MHz Arduino boards (e.g. Duemilanove and Nano), this function has a resolution of four microseconds (i.e. the value returned is always a multiple of four). On 8 MHz Arduino boards (e.g. the LilyPad), this function has a resolution of eight microseconds.
  return micros();
}


void f_init_o_nema_stepper(
    O_nema_stepper *o_nema_stepper,
    int n_pin_step,
    int n_pin_direction)
{

  // enter the static data
  o_nema_stepper->n_microstepping_divisor = 1.0;
  o_nema_stepper->n_steps_per_revolution = 200.0;
  // o_nema_stepper->n_revolutions_per_minute_max = 3000.0;
  o_nema_stepper->n_revolutions_per_minute_max = 3. * 60.0; // security measurement
  o_nema_stepper->n_revolutions_per_minute_min = 0.0;
  // speed 0 at init
  o_nema_stepper->n_speed_nor = 0.0;
  o_nema_stepper->n_revolutions_per_minute = 0.0;
  o_nema_stepper->n_steps_per_second = 0.0;
  o_nema_stepper->n_revolutions_per_second = 0.0;
  o_nema_stepper->n_microseconds_delay_between_step = 0.0;
  o_nema_stepper->n_steps_per_second = 0.0;
  o_nema_stepper->b_direction_forward = 1.0;
  o_nema_stepper->b_direction_forward__last = -1.0;
  o_nema_stepper->n_ts_mics = 0;
  o_nema_stepper->n_ts_mics__last = 0;
  o_nema_stepper->n_ts_mics__diff = 0;
  // pins
  o_nema_stepper->n_pin_step = n_pin_step;
  o_nema_stepper->n_pin_direction = n_pin_direction;

  f_setup_pinMode_o_nema_stepper(o_nema_stepper);
}

void f_setup_pinMode_o_nema_stepper(
    O_nema_stepper *o_nema_stepper)
{
  pinMode(o_nema_stepper->n_pin_step, OUTPUT);
  pinMode(o_nema_stepper->n_pin_direction, OUTPUT);
}

void f_update_speed_normalized_o_nema_stepper(
    O_nema_stepper *o_nema_stepper,
    double n_speed_nor)
{
  double *n_ptr_n;
  double n_speed_normalized_fract = abs(modf(n_speed_nor, n_ptr_n));

  double n_revolutions_per_minute = n_speed_normalized_fract * (o_nema_stepper->n_revolutions_per_minute_max - o_nema_stepper->n_revolutions_per_minute_min) + o_nema_stepper->n_revolutions_per_minute_min;

  f_update_rpm_o_nema_stepper(
      o_nema_stepper,
      n_revolutions_per_minute);
};

void f_update_rpm_o_nema_stepper(
    O_nema_stepper *o_nema_stepper,
    double n_revolutions_per_minute)
{
  if (n_revolutions_per_minute > o_nema_stepper->n_revolutions_per_minute_max)
  {
    Serial.println("cannot set the speed:");
    Serial.println(n_revolutions_per_minute);
    Serial.println("the maximum rpm (revolutions per minute) (speed) is ");
    Serial.println(o_nema_stepper->n_revolutions_per_minute_max);
  }
  if (n_revolutions_per_minute < o_nema_stepper->n_revolutions_per_minute_min)
  {
    Serial.println("cannot set the speed:");
    Serial.println(n_revolutions_per_minute);
    Serial.println("the minimum rpm (revolutions per minute) (speed) is ");
    Serial.println(0.);
  }
  if (
      n_revolutions_per_minute <= o_nema_stepper->n_revolutions_per_minute_max,
      n_revolutions_per_minute >= o_nema_stepper->n_revolutions_per_minute_min)
  {
    o_nema_stepper->n_revolutions_per_minute = n_revolutions_per_minute;
    o_nema_stepper->n_revolutions_per_second = o_nema_stepper->n_revolutions_per_minute / 60.;
    o_nema_stepper->n_steps_per_second = o_nema_stepper->n_revolutions_per_second * o_nema_stepper->n_steps_per_revolution;
    o_nema_stepper->n_microseconds_delay_between_step = (1l * 1000l * 1000l) / o_nema_stepper->n_steps_per_second;

    // Serial.println(o_nema_stepper->n_microseconds_delay_between_step);
  }
}

struct O_nema_stepper o_nema_stepper;

void setup()
{

  Serial.begin(9600);
  f_init_o_nema_stepper(
      &o_nema_stepper,
      6, // pin step
      7  // pin direction
  );
  f_update_speed_normalized_o_nema_stepper(
    &o_nema_stepper, 
    .99
  );
}

void f_update_o_nema_stepper(
    O_nema_stepper *o_nema_stepper)
{
  o_nema_stepper->n_ts_mics = f_n_ts_mics();
  o_nema_stepper->n_ts_mics__diff = o_nema_stepper->n_ts_mics - o_nema_stepper->n_ts_mics__last;
  // digitalWrite(o_nema_stepper->n_pin_step, 0);
  if(
    o_nema_stepper->b_direction_forward != o_nema_stepper->b_direction_forward__last
    || 
    o_nema_stepper->b_direction_forward__last < 0.
  ){
      digitalWrite(o_nema_stepper->n_pin_direction, o_nema_stepper->b_direction_forward);
      o_nema_stepper->b_direction_forward__last = o_nema_stepper->b_direction_forward; 
  }
  if (o_nema_stepper->n_ts_mics__diff > o_nema_stepper->n_microseconds_delay_between_step)
  {
    if (o_nema_stepper->n_revolutions_per_minute > 0.)
    {
      digitalWrite(o_nema_stepper->n_pin_step, 1);
      o_nema_stepper->n_ts_mics__last = o_nema_stepper->n_ts_mics;
      digitalWrite(o_nema_stepper->n_pin_step, 0);

    }
  }

}



void loop()
{

  int n_x = analogRead(n_pin_ain_potentiometer_x);
  int n_y = analogRead(n_pin_ain_potentiometer_y);
  Serial.println(n_x);
  Serial.println(n_y);
  double n_x_nor = (((float)n_x / 1024.0) - .5) * 2.;
  double n_y_nor = (((float)n_y / 1024.0) - .5) * 2.;
  double n_speed_nor_x = abs(n_x_nor);
  double n_speed_nor_y = abs(n_y_nor);
  if (n_speed_nor_x < 0.1)
  {
    n_speed_nor_x = 0.;
  }
  if (n_speed_nor_y < 0.1)
  {
    n_speed_nor_y = 0.;
  }
  f_update_speed_normalized_o_nema_stepper(
    &o_nema_stepper, 
    n_speed_nor_x
  );
  o_nema_stepper.b_direction_forward = (n_x_nor > 0.);
  f_update_o_nema_stepper(&o_nema_stepper);
}
