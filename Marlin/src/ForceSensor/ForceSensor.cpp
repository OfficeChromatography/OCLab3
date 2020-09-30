/*!
 * @file Adafruit_MPRLS.cpp
 *
 * @mainpage Adafruit MPRLS Pressure sensor
 *
 * @section intro_sec Introduction
 *
 * Designed specifically to work with the MPRLS sensor from Adafruit
 * ----> https://www.adafruit.com/products/3965
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * MIT license, all text here must be included in any redistribution.
 *
 */
#include "string.h"
#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"
#include "../gcode/gcode.h"
#include "ForceSensor.h"
#include "../module/motion.h"
#include "../module/endstops.h"

/**************************************************************************/
/*!
    @brief constructor initializes default configuration value
   skip
    @param PSI_min The minimum PSI measurement range of the sensor, default 0
    @param PSI_max The maximum PSI measurement range of the sensor, default 25
*/
/**************************************************************************/
ForceSensor::ForceSensor(uint16_t PSI_min, uint16_t PSI_max, uint16_t zero_offset) {
  _PSI_min = PSI_min;
  _PSI_max = PSI_max;
  _zero_offset = zero_offset;
//   getZero();
}

/**************************************************************************/
/*!
    @brief  setup and initialize communication with the hardware
    @param i2c_addr The I2C address for the sensor (default is 0x18)
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults
   to &Wire
    @returns True on success, False if sensor not found
*/
/**************************************************************************/
void ForceSensor::begin(uint8_t i2c_addr, TwoWire *twoWire) {
    _i2c_addr = i2c_addr;
    _i2c = twoWire;

    _i2c->begin();
    
    getZero();
}

/**************************************************************************/
/*!
    @brief Read and calculate the pressure
    @returns The measured pressure, in hPa on success, NAN on failure
*/
/**************************************************************************/
double ForceSensor::readPressure(void) {
    double meanResult = overSampleAndMean(10);
    
    double finalResult = movingAverange(meanResult);
    // double pressure = 0.030315924080126185*finalResult -42.01436809654545;

    double force = (finalResult-_zero_offset)*44.4822/(15000-finalResult);
    double psi = force*5.57838;
    double psi_error = 1.5740433314276985*psi -14.703999746307119;

    // double withoutoffset = finalResult-_zero_offset;
    // double result = 0.02809467153305912*withoutoffset;


    // uint16_t result;
    // SERIAL_ECHOLNPAIR("raw_for:",raw);
    // SERIAL_ECHOLNPAIR("mean_for:",result);
    // SERIAL_ECHOLNPAIR("wind_for:",move);
    // SERIAL_ECHOPAIR("\tzero_for:",_zero_offset);
    // if (result > _zero_offset) {
    // //     // force = ((raw-_zero_offset)*4.44822)/14000;
    // //     // pressure = (force/(4*3.14))*145.0380;
    // result = (result*0.02776580669424442)-41.735036877949476;
    // }
    return psi_error;
    // return move;
}


/**************************************************************************/
/*!
    @brief Read 24 bits of measurement data from the device
    @returns -1 on failure (check status) or 24 bits of raw ADC reading
*/
/**************************************************************************/
double ForceSensor::sample(void) {
    int sample;

    _i2c->requestFrom(_i2c_addr, (uint8_t)2);
    delay(4);
    sample = Wire.read();
    sample <<= 8;
    sample |= Wire.read();
    sample = sample & 0x3FFF;

    return (double)sample;
}

double ForceSensor::overSampleAndMean(int number_of_samples){
    double mean=0;
    for(int i=0;i<number_of_samples;i++){
        mean += sample();
    }
    mean = mean/number_of_samples;
    return mean;
}

double ForceSensor::movingAverange(double sample){
    double result = 0.0;
    for(int i = windowsSize-1;i>0;i--){
        movingAverangeSamples[i] = movingAverangeSamples[i-1];
        result += movingAverangeSamples[i];
    }
    movingAverangeSamples[0] = sample/windowsSize;
    result+=movingAverangeSamples[0];
    return result;
}   

void ForceSensor::getZero(void) {
    double move,result;
    endstops.enable(true);
    float pos=current_position.z-1;
    do_blocking_move_to_z(pos, 10);
    // homeaxis(Z_AXIS);
    
    for(int i = 0;i<windowsSize;i++) movingAverangeSamples[i]=0;
    for(int i = 0;i<=windowsSize;i++){
        result = overSampleAndMean(10);
        move = movingAverange(result);
    }
    _zero_offset = move;
}
