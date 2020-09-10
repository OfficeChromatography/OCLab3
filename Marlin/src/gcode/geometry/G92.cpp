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
#include "../../MPRSensor/Adafruit_MPRLS.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../feature/joystick.h"

extern Adafruit_MPRLS mpr ;


#if ENABLED(I2C_POSITION_ENCODERS)
  #include "../../feature/encoder_i2c.h"
#endif

// #define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
// #define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin


// Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
millis_t last_pump_state_change_ms = millis();  //Needed initialization
millis_t last_preassure_check_ms = millis();   //Needed initialization
bool next_state = 0;
/**
 * G92: Set current position to given X Y Z E
 */

// one complete cicle of pump on and off
void pump_it(){
  float pressure;
  // Read Preassure
  pressure=mpr.readPressure();
  SERIAL_ECHOLNPAIR("BP ",pressure);

  extDigitalWrite(9, 1);
  analogWrite(9, 255);
  delay(250);
  extDigitalWrite(9, 0);
  analogWrite(9, 0);
  delay(250);

  // Read Preassure
  pressure=mpr.readPressure();
  SERIAL_ECHOLNPAIR("AP ",pressure);
}

// one complete cicle of vavle open and close at a certain frequency
void valve_it(uint16_t f){
  float pressure;
  // Read Preassure
  pressure = mpr.readPressure();
  SERIAL_ECHOLNPAIR("BV ",pressure);

  // valve
  extDigitalWrite(16, 1);
  analogWrite(16, 255);
  if(f<500){
    delay(500/f);
  }
  else{
    delayMicroseconds(500000/f);
  }
  extDigitalWrite(16, 0);
  analogWrite(16, 0);
  if(f<500){
    delay(500/f);
  }
  else{
    delayMicroseconds(500000/f);
  }

  // Read Preassure
  pressure=mpr.readPressure();
  SERIAL_ECHOLNPAIR("AV ",pressure);
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

// void pumpit(){
//   const millis_t ms = millis();
//   //check if 250ms has already pass since the last change of state (on/off or off/on). This is so that we can guarantee that the pump will always be ON or OFF for 250ms
//   if (ELAPSED(ms, last_pump_state_change_ms + 250)) {
//       // Check if the preassure is < 5 if so and if the next_state of the pump should be ON
//       extDigitalWrite(9, 1);
//       analogWrite(9, 255);
//       delay(250);
//       extDigitalWrite(9, 0);
//       analogWrite(9, 0);
//       delay(250);
//       // Update the last_pump_state_change_ms to this moment
//       last_pump_state_change_ms = ms;
//     }
// }


// float get_pressure(uint16_t period){
//   static float pressure;
//   const millis_t ms = millis();
//     if (ELAPSED(ms, last_preassure_check_ms + period)){
//       last_preassure_check_ms = ms;
//       pressure = mpr.readPressure();
//       // SERIAL_ECHOLNPAIR("Pressure:",pressure);
//     }
//   return pressure;
// }

void GcodeSuite::G93(){
  // frequency value in Hz 
  if(parser.seen('F')){
    int16_t f = parser.intval('F');
    SERIAL_ECHOLNPAIR("Frequency:",f);
    valve_it(f);
    // Pressure Checks
    float pressure=mpr.readPressure();
    float set_pressure = parser.floatval('P');
    float min_pressure = set_pressure;
    if(pressure<min_pressure){
      pump_it();
      pressure=mpr.readPressure();
    }
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the frequency wanted");
  }
}


void GcodeSuite::G94(){
  // Number of pumxps
  int n_pumps = 0; 
  int number_of_pumps=7;
  if (parser.seen('N')){
     number_of_pumps = parser.intval('N'); 
  }
  if(parser.seen('P')){
    float preassure_set = parser.floatval('P');
    float pressure =  mpr.readPressure();
    // planner.synchronize();
    while(pressure<preassure_set && n_pumps<number_of_pumps){
        pump_it();
        pressure=mpr.readPressure();
        n_pumps++;
    }
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the pressure wanted");
  }
}

void GcodeSuite::G95(){
  SERIAL_ECHOPAIR("P_pre:",mpr.readPressure());
  SERIAL_ECHOLNPAIR("\tP_for:",joystick.show_raw());
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
  //  
  // // SERIAL_ECHOLNPAIR("Frequency:"); 
SERIAL_ECHOLNPAIR("PSET:",joystick.show_raw());
  
}

void GcodeSuite::G41(){
  extDigitalWrite(16, 0);
  analogWrite(16, 0);
}

void GcodeSuite::pumpsyringe(float pressure_set){
  float pos=current_position.z;
  // float pressure_read = mpr.readPressure();
  float pressure_read = joystick.show_raw();
  // set_relative_mode(true);
  // planner.synchronize();
  SERIAL_ECHOLNPAIR("PSET:",pressure_set);
  float min_presure = pressure_set-1;
  float max_presure = pressure_set+1;

  while(pressure_read<min_presure || pressure_read>=max_presure){
    if(pressure_read<min_presure){
      pos+=0.001;
    }
    else{
      pos-=2;
    }
    do_blocking_move_to_z(pos, 10);
      // pressure_read = mpr.readPressure();
      pressure_read = joystick.show_raw();
      SERIAL_ECHOPAIR("P_pre:",mpr.readPressure());
      SERIAL_ECHOLNPAIR("\tP_for:",joystick.show_raw());
  }
}


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
