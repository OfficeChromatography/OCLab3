/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../ForceSensor/ForceSensor.h"
#include "../../NeoPixel/Adafruit_NeoPixel.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../feature/joystick.h"
#include "../../module/endstops.h"

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    6
#define LED_COUNT 16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
extern ForceSensor force ;


#if ENABLED(I2C_POSITION_ENCODERS)
  #include "../../feature/encoder_i2c.h"
#endif

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


/**
 * G92: Set current position to given X Y Z E
 */

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
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
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
    double difference = min_pressure-pressure_read;
    endstops.enable(true);
    pos+=0.01* (double) difference;
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


void GcodeSuite::G92() {

  bool sync_E = false, sync_XYZ = false;

  #if ENABLED(USE_GCODE_SUBCODES)
    const uint8_t subcode_G92 = parser.subcode;
  #else
    constexpr uint8_t subcode_G92 = 0;
  #endif

  switch (subcode_G92) {
    default: break;
    #if ENABLED(CNC_COORDINATE_SYSTEMS)
      case 1: {
        // Zero the G92 values and restore current position
        #if !IS_SCARA
          LOOP_XYZ(i) if (position_shift[i]) {
            position_shift[i] = 0;
            update_workspace_offset((AxisEnum)i);
          }
        #endif // Not SCARA
      } return;
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      case 9: {
        LOOP_XYZE(i) {
          if (parser.seenval(axis_codes[i])) {
            current_position[i] = parser.value_axis_units((AxisEnum)i);
            if (i == E_AXIS) sync_E = true; else sync_XYZ = true;
          }
        }
      } break;
    #endif
    case 0: {
      LOOP_XYZE(i) {
        if (parser.seenval(axis_codes[i])) {
          const float l = parser.value_axis_units((AxisEnum)i),
                      v = i == E_AXIS ? l : LOGICAL_TO_NATIVE(l, i),
                      d = v - current_position[i];
          if (!NEAR_ZERO(d)) {
            #if IS_SCARA || !HAS_POSITION_SHIFT
              if (i == E_AXIS) sync_E = true; else sync_XYZ = true;
              current_position[i] = v;        // Without workspaces revert to Marlin 1.0 behavior
            #elif HAS_POSITION_SHIFT
              if (i == E_AXIS) {
                sync_E = true;
                current_position.e = v;       // When using coordinate spaces, only E is set directly
              }
              else {
                position_shift[i] += d;       // Other axes simply offset the coordinate space
                update_workspace_offset((AxisEnum)i);
              }
            #endif
          }
        }
      }
    } break;
  }

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    // Apply workspace offset to the active coordinate system
    if (WITHIN(active_coordinate_system, 0, MAX_COORDINATE_SYSTEMS - 1))
      coordinate_system[active_coordinate_system] = position_shift;
  #endif

  if    (sync_XYZ) sync_plan_position();
  else if (sync_E) sync_plan_position_e();

  report_current_position();
}
