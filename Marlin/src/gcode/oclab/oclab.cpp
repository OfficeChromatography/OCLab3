#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#include "../../ForceSensor/ForceSensor.h"
#include "../../NeoPixel/Adafruit_NeoPixel.h"
#include "../../module/motion.h"
#include "../../feature/joystick.h"
#include "../../module/endstops.h"
#include "../../OpenValve/openValve.h"
#include "DHT.h"
#include "src/gcode/oclab/pumpcontrol/pumpcontrol.h"

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
extern DHT dht;


// UTIL FUNCTIONS

int errorFunction(float set, float real){
  int error = abs(set - real);
  return (error)<1?(1):(error);
}

void pumpsyringe(float pressure_set){
  PumpControl pump =  PumpControl(pressure_set);
  pump.compute();
}

void colorWipe(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}


// -------------------------- GCode Functions ---------------------//

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

// Not implemented
void GcodeSuite::G94(){}

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

// Not implemented
void GcodeSuite::G96(){
  float temperature = 0;
  float humidity = 0;
  
  humidity = dht.readHumidity(true);
  temperature = dht.readTemperature(false);
  
  SERIAL_ECHOPAIR("T:", temperature);
  SERIAL_ECHOLNPAIR(" H:", humidity);
}

// Set the pressure 
void GcodeSuite::G97(){
  if (parser.seen('P')){
    float pressure_set = parser.intval('P'); 
    pumpsyringe(pressure_set);
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the pressure wanted");
  }
  
}

// Open the valve at certain frequency
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

// Close the valve
void GcodeSuite::G40(){
  planner.synchronize();
  valve.closeValve();
}

// Open the valve
void GcodeSuite::G41(){
  planner.synchronize();
  valve.openValve();
}




