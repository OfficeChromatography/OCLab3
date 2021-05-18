#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#include "../../ForceSensor/ForceSensor.h"
#include "../../NeoPixel/Adafruit_NeoPixel.h"
#include "../../module/motion.h"
#include "../../feature/joystick.h"
#include "../../module/endstops.h"
#include "../../OpenValve/openValve.h"


#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

#define LED_PIN_PIXEL    6
#define LED_COUNT 16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN_PIXEL, NEO_GRBW + NEO_KHZ800);
extern ForceSensor force ;
extern ValveOpen valve ;



void colorWipe(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}

// Control the RGB LEDs
void GcodeSuite::G93(){
  if (parser.seen('R')&&parser.seen('G')&&parser.seen('B')){
    int red = parser.intval('R'); 
    int green = parser.intval('G'); 
    int blue = parser.intval('B'); 
    // int brigthness = parser.intval('I'); 

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    // strip.setBrightness(brigthness); // Set BRIGHTNESS to about 1/5 (max = 255)
    colorWipe(strip.Color(red, green, blue));
  }
  else
  {
    SERIAL_ECHOLN("Please send an RGB and brigthness value");
  }
}




// Returns the pressure
void GcodeSuite::G95(){

  if(parser.seen('M')){
    SERIAL_ECHOLNPAIR("Mass[gr]:",force.readMass());
  }

  if(parser.seen('R')){
    SERIAL_ECHOLNPAIR("RawData:",force.sample());
  }
  
  if(parser.seen('P')){
    SERIAL_ECHOLNPAIR("Pressure[psi]:",force.readPressure());
  }

  if(parser.seen('F')){
    SERIAL_ECHOLNPAIR("Force[N]:",force.readForce());
  }
}

void GcodeSuite::G96(){
  float ABSOLUTE_MAX_PRESSURE = 80;
  float pressure_set = 30;

  if (parser.seen('P')){
    pressure_set = parser.intval('P'); 
  }

  get_destination_from_command();
  xyze_pos_t final_destination = destination;
  // float final_destination_z = destination.z;
  destination.z = current_position.z;

  float pressure_read = force.readPressure();
  // set_relative_mode(true);
  // planner.synchronize();
  
  SERIAL_ECHOLNPAIR("PSET:",pressure_set);
  float min_pressure = pressure_set*0.95;
  float max_pressure = pressure_set*1.05;
  while((pressure_read<min_pressure || pressure_read>=max_pressure) && cartes.x != final_destination.x){
    if(pressure_read<min_pressure){
        destination.z+=(0.01*errorFunction(min_pressure, pressure_read));
    }
    if(pressure_read>max_pressure){
        destination.z-=(0.01*errorFunction(max_pressure, pressure_read));
    }
    if(pressure_read < ABSOLUTE_MAX_PRESSURE){
      endstops.enable(true);
      prepare_line_to_destination();
      sync_plan_position();
      pressure_read = force.readPressure();
      get_cartesian_from_steppers();
      SERIAL_ECHOLNPAIR("CURRENT_X:",cartes.x);
      SERIAL_ECHOLNPAIR("FINAL_X:",final_destination.x);
      SERIAL_ECHOLNPAIR("force_N:",force.readPressure());
    }
    else{
      SERIAL_ECHOLN("Pressure is too high!!");
      break;
    }
  }
  SERIAL_ECHOLNPAIR("MOVES STILL", planner.movesplanned());
  planner.synchronize();
    //   }// float pressure_read = force.readPressure();
}


// Initial 
void GcodeSuite::G97(){
  float pressure_set;
  
  if (parser.seen('P')){
    pressure_set = parser.intval('P'); 
    pumpsyringe(pressure_set);
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the pressure wanted");
  }
  
}

void GcodeSuite::G98(){
  // frequency value in Hz 
  if(parser.seen('F')){
    int frequency = parser.intval('F');
    if(frequency<=valve.getMaxFrequency()){
      valve.frequencyToggleValve(frequency);
    }
    else
    {
      SERIAL_ECHOLNPAIR("Frequency should be equal or less than:", valve.getMaxFrequency());
    }    
  }
  else{
    SERIAL_ECHOLNPGM("Please Insert the frequency wanted");
  }
}

// void GcodeSuite::G40(){
//   float distanceX;
//   float distanceZ;
//   (parser.seen('X')) ? distanceX = parser.floatval('X') : 0;
//   (parser.seen('Z')) ? distanceZ = parser.floatval('Z') : 0;
//   xyz_pos_t pos = {current_position.x + distanceX, current_position.y, current_position.z + distanceZ};
//   valve.openValve();
//   do_blocking_move_to(pos,10);
//   valve.closeValve();
// }


void GcodeSuite::G40(){
  planner.synchronize();
  valve.toggleValve();
}

void GcodeSuite::G41(){
  int pin = 11;
  while(digitalRead(pin) == HIGH){   
    current_position.e -= 0.01;
    line_to_current_position(homing_feedrate(Z_AXIS));
  }
}

void GcodeSuite::pumpsyringe(float pressure_set){
  float ABSOLUTE_MAX_PRESSURE = 80;
  float pos=current_position.z;
  float pressure_read = force.readPressure();
  // set_relative_mode(true);
  // planner.synchronize();

  if(pressure_set > ABSOLUTE_MAX_PRESSURE){
    SERIAL_ECHOLN("Pressure settled too high!!");
  }
  else{
    SERIAL_ECHOLNPAIR("PSET:",pressure_set);
    float min_pressure = pressure_set*0.95;
    float max_pressure = pressure_set*1.05;
    while(pressure_read<min_pressure || pressure_read>=max_pressure){
      if(pressure_read<min_pressure){
          pos+=(0.01*errorFunction(min_pressure, pressure_read));
      }
      if(pressure_read>max_pressure){
          pos-=(0.01*errorFunction(max_pressure, pressure_read));
      }
      if(pressure_read < ABSOLUTE_MAX_PRESSURE){
        endstops.enable(true);
        do_blocking_move_to_z(pos, 10);
        pressure_read = force.readPressure();
        SERIAL_ECHOLNPAIR("force_N:",force.readPressure());
      }
      else{
        SERIAL_ECHOLN("Pressure is too high!!");
        break;
      }
    }
  }
}

int GcodeSuite::errorFunction(float set, float real){
  int error = abs(set - real);
  return (error)<1?(1):(error);
}

void GcodeSuite::G94(){
  if(parser.seen('X') && parser.seen('Z') && parser.seen('F') && parser.seen('P')){
    float pressure = parser.floatval('P');
    float feedrate = parser.floatval('F');
    xyze_pos_t initial =  current_position;
    xyze_pos_t temporary = current_position;
    xyze_pos_t destination = { parser.floatval('X'), current_position.y , parser.floatval('Z') };
    // list <double>  all_pressures_measured;
    boolean reverse = false;
    double pressure_mesured;
    pumpsyringe(pressure);
    while(temporary.z<=destination.z){
      pressure_mesured = force.forceToPressure(force.digitalToForce(force.sample()));
      if(pressure_mesured < pressure){
        temporary.z+=0.1;
      }
      if(reverse){
        if(temporary.x<=destination.x){
          temporary.x+=0.01;
        }
        else{
          reverse = true;
        }
      }
      else{
        if(temporary.x>=initial.x){
          temporary.x-=0.01;
        }
        else{
          reverse = false;
        }
      }
      do_blocking_move_to(temporary, feedrate);
    }
  }
}
