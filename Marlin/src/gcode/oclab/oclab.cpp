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



void colorWipe(uint32_t color, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}

// Control the RGB LEDs
void GcodeSuite::G93(){
  if (parser.seen('R')&&parser.seen('G')&&parser.seen('B')&&parser.seen('I')){
    int red = parser.intval('R'); 
    int green = parser.intval('G'); 
    int blue = parser.intval('B'); 
    int brigthness = parser.intval('I'); 

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(brigthness); // Set BRIGHTNESS to about 1/5 (max = 255)
    colorWipe(strip.Color(red, green, blue),50);
  }
  else
  {
    colorWipe(strip.Color(0, 0, 0),50);
  }
}


void GcodeSuite::G94(){}

// Returns the pressure
void GcodeSuite::G95(){
  SERIAL_ECHOLNPAIR("Pressure:",force.readPressure());
}

void GcodeSuite::G96(){
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
    valve.frequencyToggleValve(frequency);
  }
  else{
    SERIAL_ECHOLNPGM("Please Insert the frequency wanted");
  }
}


void GcodeSuite::G40(){
  valve.toggleValve();
}

void GcodeSuite::G41(){}

void GcodeSuite::pumpsyringe(float pressure_set){
  float pos=current_position.z;
  float pressure_read = force.readPressure();
  // set_relative_mode(true);
  // planner.synchronize();
  SERIAL_ECHOLNPAIR("PSET:",pressure_set);
  float min_pressure = pressure_set-0.5;
  float max_pressure = pressure_set+0.5;

  while(pressure_read<min_pressure || pressure_read>=max_pressure){
    endstops.enable(true);
    if(pressure_read<min_pressure){
        pos+=0.01;
    }
    if(pressure_read>max_pressure){
        pos-=0.01;
    }
    do_blocking_move_to_z(pos, 10);
    pressure_read = force.readPressure();
    SERIAL_ECHOLNPAIR("force_N:",force.readPressure());
  }
}

