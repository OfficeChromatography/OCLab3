#include "openValve.h"
#include "string.h"
#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"
#include "../gcode/gcode.h"

ValveOpen::ValveOpen(){
    pin = 16;
    open = false;
}

void ValveOpen::toggleValve(){
    if (open) {
        extDigitalWrite(pin, 0);
        analogWrite(pin, 0);
        closeValve();
    } else {
        extDigitalWrite(pin, 1);
        analogWrite(pin, 255);
        openValve();
    }
}
void ValveOpen::openValve(){
    open = true;
}
void ValveOpen::closeValve(){
    open = false;
}
bool ValveOpen::getOpen(){
    return open;
}

