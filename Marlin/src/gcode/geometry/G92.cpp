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
#include "../../MPRSensor/Adafruit_MPRLS.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"


#if ENABLED(I2C_POSITION_ENCODERS)
  #include "../../feature/encoder_i2c.h"
#endif

/**
 * G92: Set current position to given X Y Z E
 */

// one complete cicle of pump on and off
void pump_it(){
  const millis_t cicle = 250;
  static millis_t pump_on = millis();
  static millis_t pump_off = millis();

  while(millis()<pump_off+cicle){}
  WRITE(10,HIGH);
  pump_on = millis();
  while(millis()<(pump_on+cicle)){};
  WRITE(10,LOW);
  pump_off = millis();
}

// one complete cicle of vavle open and close at a certain frequency
void valve_it(uint16_t f){
  static millis_t valve_on = millis();
  static millis_t valve_off = millis();

  while(millis()<(valve_off+1/f)){};
  WRITE(16,HIGH);
  valve_on = millis();
  while(millis()<(valve_on+1/f)){};
  WRITE(10,LOW);
  valve_off = millis();

}

void GcodeSuite::G94(){
  WRITE(9,HIGH);
  delay(250);
  WRITE(9,LOW);
  delay(250);
  WRITE(9,HIGH);
  delay(250);
  WRITE(9,LOW);
  delay(250);
  WRITE(9,HIGH);
  delay(250);
  WRITE(9,LOW);
  delay(250);
}

void GcodeSuite::G93(){

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin

Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
mpr.begin();

if(parser.seen('P')){
  // pressure value in mmHg
  int16_t p = parser.intval('P');
  SERIAL_ECHOLNPAIR("Pressure:",p);
  // SERIAL_ECHOLNPAIR("",mpr.readPressure());
  // Reads the sensor and open the pump if the preassure is too low
  while(p<mpr.readPressure()){
    SERIAL_ECHOLNPAIR("ENTRO AL WHILE");
    pump_it();
  }
}
  else{
    SERIAL_ECHOLNPGM("Please Insert the pressure wanted");
  }
  // frequency value in Hz 
  if(parser.seen('F') && parser.seen('Q')){
    int16_t f = parser.intval('F');
    int16_t q = parser.intval('Q');

    SERIAL_ECHOLNPAIR("Frequency:",f);
    SERIAL_ECHOLNPAIR("Quantity:",q);
    
    for(int16_t i=0;i<q;i++){
      valve_it(f);
    }
  }
  else
  {
    SERIAL_ECHOLNPGM("Please Insert the frequency wanted and quantity");
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
