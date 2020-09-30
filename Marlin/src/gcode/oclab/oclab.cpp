#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#include "../../ForceSensor/ForceSensor.h"
#include "../../NeoPixel/Adafruit_NeoPixel.h"
#include "../../module/motion.h"
#include "../../feature/joystick.h"
#include "../../module/endstops.h"


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

// one complete cicle of pump on and off
void pump_it(){
  extDigitalWrite(9, 1);
  analogWrite(9, 255);
  delay(250);
  extDigitalWrite(9, 0);
  analogWrite(9, 0);
  delay(250);
}

// one complete cicle of vavle open and close at a certain frequency
void valve_it(uint16_t f){
  // valve open
  extDigitalWrite(16, 1);
  analogWrite(16, 255);
  if(f<500){
    delay(500/f);
// void GcodeSuite::pumpsyringe(float pressure_set){
//   float pos=current_position.z;
//   float pressure_read = force.readPressure();
//   // set_relative_mode(true);
//   // planner.synchronize();
//   SERIAL_ECHOLNPAIR("PSET:",pressure_set);
//   float min_presure = pressure_set-0.1;
//   float max_presure = pressure_set+0.1;

//   while(pressure_read<min_presure || pressure_read>=max_presure){
//     if(pressure_read<min_presure){
//       pos+=0.01;
//     }
//     else{
//       pos-=0.01;
//     }
//     do_blocking_move_to_z(pos, 10);
//     pressure_read = force.readPressure();
//     SERIAL_ECHOLNPAIR("\tP_for:",pressure_read);
//   }
// }
  }
  else{
    delayMicroseconds(500000/f);
  }
  // valve close
  extDigitalWrite(16, 0);
  analogWrite(16, 0);
  if(f<500){
    delay(500/f);
  }
  else{
    delayMicroseconds(500000/f);
  }
}

void staticClean(){
  // PUMPS IN
  extDigitalWrite(9, 1);
  analogWrite(9, 255);
  // WAIT
  delay(250);
  extDigitalWrite(9, 0);
  analogWrite(9, 0);
  delay(250);
}


void colorWipe(uint32_t color, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}


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

void GcodeSuite::G95(){
  SERIAL_ECHOLNPAIR("Pressure:",force.readPressure());
}

void GcodeSuite::G96(){
  staticClean();
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
    int16_t f = parser.intval('F');
    
    // Open the valve
    valve_it(f);
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the frequency wanted");
  }
}

void GcodeSuite::G40(){
  extDigitalWrite(16, 0);
  analogWrite(16, 0);
}

void GcodeSuite::G41(){
  extDigitalWrite(16, 0);
  analogWrite(16, 0);
}

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

// void GcodeSuite::pumpsyringe(float pressure_set){
//   float pos=current_position.z;
//   float pressure_read = force.readPressure();
//   // set_relative_mode(true);
//   // planner.synchronize();
//   SERIAL_ECHOLNPAIR("PSET:",pressure_set);
//   float min_presure = pressure_set-0.1;
//   float max_presure = pressure_set+0.1;

//   while(pressure_read<min_presure || pressure_read>=max_presure){
//     if(pressure_read<min_presure){
//       pos+=0.01;
//     }
//     else{
//       pos-=0.01;
//     }
//     do_blocking_move_to_z(pos, 10);
//     pressure_read = force.readPressure();
//     SERIAL_ECHOLNPAIR("\tP_for:",pressure_read);
//   }
// }