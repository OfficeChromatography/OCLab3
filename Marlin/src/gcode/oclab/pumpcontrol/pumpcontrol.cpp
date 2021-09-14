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
  if(this->pressure_read< this->min_pressure){
    int error = this->errorFunction(this->min_pressure, this->pressure_read);
    this->pos+=(0.03*error);
  }
  if(this->pressure_read> this->max_pressure){
    int error = this->errorFunction(this->max_pressure, this->pressure_read);
    this->pos-=(0.03*error);
  }
}

void PumpControl::move(){
  endstops.enable(true);
  do_blocking_move_to_z(this->pos, G0_FEEDRATE);
  this->pressure_read = force.readPressure();
  SERIAL_ECHOPAIR("Ps: ", this->pressure_set);
  SERIAL_ECHOLNPAIR("\tPr: ", this->pressure_read);
}

PumpControl::PumpControl(float pressure_set){
  boolean toHigh = pressure_set > this->ABSOLUTE_MAX_PRESSURE;
  boolean toLow = pressure_set < this->ABSOLUTE_MIN_PRESSURE;

  if(toHigh || toLow){
    (toHigh)?(SERIAL_ECHOLN("Pressure settled too high!!")):
    (SERIAL_ECHOLN("Pressure settled too low, must be greater than 2!!"));
  }
  else{
    this->pressure_set = pressure_set;
    this->min_pressure = pressure_set-1;
    this->max_pressure = pressure_set-1;
  }
}

void PumpControl::compute(){
  while(pressure_read<min_pressure || pressure_read>=max_pressure){
    this->calculateNewPosition();
    if(this->pressure_read < this->ABSOLUTE_MAX_PRESSURE){
      this->move();
    }else{
      SERIAL_ECHOLN("Pressure is too high!!");
      break;
    }
  }
}

