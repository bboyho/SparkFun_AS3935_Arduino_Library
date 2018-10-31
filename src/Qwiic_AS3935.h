#ifndef Sparkfun_AS3935_h
#define Sparkfun_AS3935_h

#include "Wire.h"
#include "SPI.h"
#include "Arduino.h"

enum AS3935_Register {

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

#define AS3935_DEFAULT_ADDRESS 0x01

#define NOISE_TO_HIGH     0x01 //B00000001
#define DISTURBER_DETECT  0x04 //B00000100
#define LIGHTNING         0x08 //B00001000
#define INDOOR            0x12
#define OUTDOOR           0xE

// Masks for various registers, there are some redundant values that I kept 
// for the sake of clarity in the code.
#define GAIN_MASK         0xF
#define SPIKE_MASK        0xF
#define DISTANCE_MASK     0x1F
#define FLOOR_MASK        0x07
#define OSC_MASK          0x07
#define CAP_MASK          0x07

class Qwiic_AS3935
{
  public: 

    // REG0x00, bit[0], manufacturer default: 0. 
    // The product consumes 1-2uA while powered down. If the board is powered down 
    // the the TRCO will need to be recalibrated: REG0x08[5] = 1, wait 2 ms, REG0x08[5] = 0.
    void powerDown();
    // REG0x00, bits [5:1], manufacturer default: 10010 (INDOOR). 
    // This funciton changes toggles the chip's settings for Indoors and Outdoors. 
    void indoorOutdoorSetting(uint8_t _setting);
    // REG0x01, bits[3:0], manufacturer default: 0010 (2). 
    // This setting determines the threshold for events that trigger the 
    // IRQ Pin.  
    void watchdogThreshold(uint8_t _sensitivity);
    // REG0x01, bits [6:4], manufacturer default: 010 (2).
    // The noise floor level is compared to a known reference voltage. If this
    // level is exceeded the chip will issue an interrupt to the IRQ pin,
    // broadcasting that it can not operate properly due to noise (INT_NH).
    // Check datasheet for specific noise level tolerances when setting this register. 
    void setNoiseLevel(uint8_t _floor);
    // REG0x02, bits [3:0], manufacturer default: 0010 (2).
    // This setting, like the watchdog threshold, can help determine between false
    // events and actual lightning. The shape of the spike is analyzed during the
    // chip's signal validation routine. Increasing this value increases robustness
    // at the cost of sensitivity to distant events. 
    void spikeReduction(uint8_t _spSensitivity);
    // REG0x02, bits [5:4], manufacturer default: 0 (single lightning strike).
    // The number of lightning events before IRQ is set high. 15 minutes is The 
    // window of time before the number of detected lightning events is reset. 
    // The number of lightning strikes can be set to 1,5,9, or 16. 
    void lightningThreshold(uint8_t _strikes);
    // REG0x02, bit [6], manufacturer default: 1. 
    // This register clears the number of lightning strikes that has been read in
    // the last 15 minute block. 
    void clearStatistics(bool _clearStat);
    // REG0x03, bits [3:0], manufacturer default: 0. 
    // When there is an event that exceeds the watchdog threshold, the register is written
    // with the type of event. This consists of two messages: INT_D (disturber detected) and 
    // INT_L (Lightning detected). A third interrupt INT_NH (noise level too HIGH) 
    // indicates that the noise level has been exceeded and will persist until the
    // noise has ended. 
    uint8_t readInterruptReg();
    // REG0x03, bit [5], manufacturere default: 0.
    // This setting will change whether or not disturbers trigger the IRQ Pin. 
    void maskDisturber(uint8_t _state);
    // REG0x03, bit [7:6], manufacturer default: 0 (16 division ratio). 
    // The antenna is designed to resonate at 500kHz and so can be tuned with the
    // following setting. The accuracy of the antenna must be within 3.5 percent of
    // that value for proper signal validation and distance estimation.
    void antennaTuning(uint8_t _divisionRatio);
    // REG0x07, bit [5:0], manufacturer default: 0. 
    // This register holds the distance to the front of the storm and not the
    // distance to a lightning strike.  
    uint8_t distanceToStorm();
    // REG0x08, bits [5,6,7], manufacturer default: 0. 
    // This will send the frequency of the oscillators to the IRQ pin. 
    //  _osc 1, bit[5] = TRCO - Timer RCO Oscillators 1.1MHz
    //  _osc 2, bit[6] = SRCO - System RCO at 32.768kHz
    //  _osc 3, bit[7] = LCO - Frequency of the Antenna
    void displayOscillator(bool _state, uint8_t _osc);
    // REG0x08, bits [3:0], manufacturer default: 0. 
    // This setting will add capacitance to the series RLC antenna on the product.
    // It's possible to add 0-120pF in steps of 8pF to the antenna. 
    void tuneCap(uint8_t _farad)
    // LSB =  REG0x04, bits[7:0]
    // MSB =  REG0x05, bits[7:0]
    // MMSB = REG0x06, bits[4:0]
    // This returns a 20 bit value that is the 'energy' of the lightning strike.
    // According to the datasheet this is only a pure value that doesn't have any
    // physical meaning. 
    void Qwiic_AS3935::lightningEnergy()
  
  private:
    // This function handles all I2C write commands. It takes the register to write
    // to, then will mask the part of the register that coincides with the
    // setting, and then write the given bits to the register. 
    void writeRegister(uint8_t _reg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition);
    // This function reads the given register. 
    uint8_t readRegister(uint8_t reg, uint8_t _len);
    TwoWire *_i2cPort; 

};
#endif

