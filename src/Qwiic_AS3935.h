#include "Wire.h"
#include "SPI.h"

#ifndef AS3935.h
#define AS3935.h

enum AS3935_Register {
  //Most, if not all of these registers have many functions. 

	AFE_GAIN          = 0x00,
  THRESHOLD         = 0x01,
  LIGHTNING         = 0x02,
  INT_MASK_ANT      = 0x03,
  ENERGY_LIGHT_LSB  = 0x04,
  ENERGY_LIGHT_MSB  = 0x05,
  ENERGY_LIGHT_MMSB = 0x06,
  DISTANCE          = 0x07,
  FREQ_DISP_IRQ     = 0x08,

};

#define NOISE_TO_HIGH 0x01 //B00000001
#define DISTURBER_DETECT 0x04 //B00000100
#define LIGHTNING 0x08 //B00001000
#define INDOOR  0x12
#define OUTDOOR  0xE
#define GAIN_MASK 0xF
#define SPIKE_MASK 0xF
#define DISTANCE_MASK 0x1F
#define FLOOR_MASK 0x07

class Qwiic_AS3935
{
  public: 

    void Qwiic_AS3935::powerDown();
    void Qwiic_AS3935::indoorOutdoorSetting( _setting);
    void Qwiic_AS3935::watchdogThreshold(uint8_t _sensitivity);
    void Qwiic_AS3935::setNoiseLevel(_floor);
    void spikeReduction(_spSensitivity);
    void Qwiic_AS3935::lightningThreshold(uint8_t _strikes);
    void Qwiic_AS3935::clearStatistics();
    uint8_t Qwiic_AS3935::readInterruptReg();
    void Qwiic_AS3935::maskDisturber(uint8_t _state);
    void Qwiic_AS3935::antennaTuning(uint8_t _divisionRatio);
    uint8_t Qwiic_AS3935::distanceToStorm();
    void Qwiic_AS3935::displayOscillatorSettings(_state, _osc);
    uint8_t Qwiic_AS3935::readRegister(uint8_t reg, uint8_t _len);
  
  private:

    TwoWire *_i2cPort; 

};
#endif

