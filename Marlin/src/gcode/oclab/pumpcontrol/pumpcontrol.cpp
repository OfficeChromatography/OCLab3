#include "src/module/endstops.h"
#include "src/MarlinCore.h"
#include "src/ForceSensor/ForceSensor.h"
#include "src/module/motion.h"
#include "pumpcontrol.h"

extern ForceSensor force ;
extern xyze_pos_t current_position;

int PumpControl::errorFunction(float set, float real){
  int error = abs(set - real);
  return (error)<1?(1):(error);
}
    
void PumpControl::calculateNewPosition(){
  if(pressure_read< min_pressure){
    int error = errorFunction(min_pressure, pressure_read);
    pos+=(0.025*error);
  }
  if(pressure_read > max_pressure){
    int error = errorFunction(max_pressure, pressure_read);
    pos-=(0.025*error);
  }
}

void PumpControl::move(){
  endstops.enable(true);
  do_blocking_move_to_z(pos, G0_FEEDRATE/500);
  pressure_read = force.readPressure();
  SERIAL_ECHOPAIR("Ps: ", pressure_set);
  SERIAL_ECHOLNPAIR("\tPr: ", pressure_read);
}

PumpControl::PumpControl(float pressure_set){
  toHigh = pressure_set > ABSOLUTE_MAX_PRESSURE;
  toLow = pressure_set < ABSOLUTE_MIN_PRESSURE;

  if(toHigh || toLow){
    (toHigh)?(SERIAL_ECHOLN("Pressure settled too high!!")):
    (SERIAL_ECHOLN("Pressure settled too low, must be greater than 2!!"));
  }
  else{
    this->pressure_set = pressure_set;
    min_pressure = pressure_set-1;
    max_pressure = pressure_set+1;
  }
}

bool PumpControl::is_out_of_range(){
  if(toHigh || toLow){
    (toHigh)?(SERIAL_ECHOLN("Pressure settled too high!!")):
    (SERIAL_ECHOLN("Pressure settled too low, must be greater than 2!!"));
    return true;
  }
  return false;
}

void PumpControl::compute(){
  if(!is_out_of_range()){
    while(pressure_read<min_pressure || pressure_read>=max_pressure){
      calculateNewPosition();
      if(pressure_read < ABSOLUTE_MAX_PRESSURE){
        move();
      }else{
        SERIAL_ECHOLN("Pressure is too high!!");
        break;
      }
    }
  }
}

