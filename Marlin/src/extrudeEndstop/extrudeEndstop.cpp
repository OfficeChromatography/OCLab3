#include "extrudeEndstop.h"
#include "string.h"
#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"
#include "../gcode/gcode.h"
#include "../MarlinCore.h"

ExtrudeEndstop::ExtrudeEndstop(){
    pin = 25;
    state = false;
}

void ExtrudeEndstop::checkEndstop(){
    if (state && digitalRead(pin) == HIGH){
        disable_e_steppers(); //quickstop_stepper()
        state = false;
    } else if (digitalRead(pin) == LOW) {
        state = true;
    };
}