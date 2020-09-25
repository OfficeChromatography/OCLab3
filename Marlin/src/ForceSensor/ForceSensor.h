#include "Arduino.h"
#include "Wire.h"


#define FORCE_DEFAULT_ADDR (0x28)   ///< Most common I2C address
#define SIZE_DEFAULT_MOVING_AVERANGE 10    ///< Most common I2C address
/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with ForceSensor
*/
/**************************************************************************/
class ForceSensor {
public:
  ForceSensor(uint16_t PSI_min = 0, uint16_t PSI_max = 10, uint16_t zero_offset = 1060);

  void begin(uint8_t i2c_addr = FORCE_DEFAULT_ADDR,
                TwoWire *twoWire = &Wire);

  double readPressure(void);
  
  double sample(void);
  double overSampleAndMean(int number_of_samples);
  double movingAverange(double marcel);

  double movingAverangeSamples[SIZE_DEFAULT_MOVING_AVERANGE];

  
private:
    int windowsSize = SIZE_DEFAULT_MOVING_AVERANGE;

    void getZero(void);

    double _zero_offset;
    uint8_t _i2c_addr;

    // int movingAverangeSamples[SIZE_DEFAULT_MOVING_AVERANGE];

    uint32_t _PSI_min, _PSI_max;

    TwoWire *_i2c;
};

