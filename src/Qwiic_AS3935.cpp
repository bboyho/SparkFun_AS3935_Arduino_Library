/*
  This is a library for the ASM AS3935 Franklin Lightning Detector
  By: Elias Santistevan
  SparkFun Electronics
  Date: October, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
*/


#include "Qwiic_AS3935.h"

Qwiic_AS3935::Qwiic_AS3935()
{
  //Default settings here
  // Communication selection?
}

Qwiic_AS3935::begin( uint8_t address, TwoWire &wirePort )
{
  _address = address; 
  _i2cPort = &wirePort
  _i2cPort->begin(); 

  return 1;
}

// REG0x00, bit[0], manufacturer default: 0. 
// The product consumes 1-2uA while powered down. If the board is powered down 
// the the TRCO will need to be recalibrated: REG0x08[5] = 1, wait 2 ms, REG0x08[5] = 0.
void Qwiic_AS3935::powerDown()
{
  writeRegister(AFE_GAIN, 0x1, 1, 0); 
}

// REG0x00, bits [5:1], manufacturer default: 10010 (INDOOR). 
// This funciton changes toggles the chip's settings for Indoors and Outdoors. 
void Qwiic_AS3935::indoorOutdoorSetting( uint8_t _setting )
{
  if(_setting != INDOOR | _setting != OUTDOOR)
    return;

  if(_setting == INDOOR)
    writeRegister(AFE_GAIN, GAIN_MASK, INDOOR, 1); 
  if(_setting == OUTDOOR)
    writeRegister(AFE_GAIN, GAIN_MASK, OUTDOOR, 1); 
}

// REG0x01, bits[3:0], manufacturer default: 0010 (2). 
// This setting determines the threshold for events that trigger the 
// IRQ Pin.  
void Qwiic_AS3935::watchdogThreshold( uint8_t _sensitivity )
{
  if(_sensitiviy < 0 | _sensitiviy > 10)// 10 is the max sensitivity setting
    return; 

  writeRegister(THRESHOLD, GAIN_MASK, _sensitiviy, 1); 
}

// REG0x01, bits [6:4], manufacturer default: 010 (2).
// The noise floor level is compared to a known reference voltage. If this
// level is exceeded the chip will issue an interrupt to the IRQ pin,
// broadcasting that it can not operate properly due to noise (INT_NH).
// Check datasheet for specific noise level tolerances when setting this register. 
void Qwiic_AS3935::setNoiseLevel( uint8_t _floor )
{
  if(_floor < 0 | _floor > 7)
    return; 
  
  writeRegister(THRESHOLD, FLOOR_MASK, _floor, 4); 
}

// REG0x02, bits [3:0], manufacturer default: 0010 (2).
// This setting, like the watchdog threshold, can help determine between false
// events and actual lightning. The shape of the spike is analyzed during the
// chip's signal validation routine. Increasing this value increases robustness
// at the cost of sensitivity to distant events. 
void Qwiic_AS3935::spikeReduction( uint8_t _spSensitivity )
{
  if(_spSensitivity < 0 | _spSensitivity > 11)
    return; 

  writeRegister(LIGHTNING, SPIKE_MASK, _spSensitivity, 0); 
}


// REG0x02, bits [5:4], manufacturer default: 0 (single lightning strike).
// The number of lightning events before IRQ is set high. 15 minutes is The 
// window of time before the number of detected lightning events is reset. 
// The number of lightning strikes can be set to 1,5,9, or 16. 
void Qwiic_AS3935::lightningThreshold( uint8_t _strikes )
{

  if(__strikes != 1 | strikes != 5 | _strikes != 9 | _strikes != 16) 
    return; 

  if( _strikes == 1)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), 0, 4); //Demonstrative
  if( _strikes == 5)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), 1, 4); 
  if( _strikes == 9)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), 1, 5); 
  if( _strikes == 16)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), (1<<5)|(1<<4), 4); 
}


// REG0x02, bit [6], manufacturer default: 1. 
// This register clears the number of lightning strikes that has been read in
// the last 15 minute block. 
void Qwiic_AS3935::clearStatistics(bool _clearStat)
{
  if(_clearStat != true)
    return;
  //Write high, then low, then high to clear.
  writeRegister(LIGHTNING, (1<<6), 1, 6)
  writeRegister(LIGHTNING, (1<<6), 0, 6)//Demonstrative
  writeRegister(LIGHTNING, (1<<6), 1, 6)
}

// REG0x03, bits [3:0], manufacturer default: 0. 
// When there is an event that exceeds the watchdog threshold, the register is written
// with the type of event. This consists of two messages: INT_D (disturber detected) and 
// INT_L (Lightning detected). A third interrupt INT_NH (noise level too HIGH) 
// indicates that the noise level has been exceeded and will persist until the
// noise has ended. 
uint8_t Qwiic_AS3935::readInterruptReg()
{
    uint8_t __interValue; 
    _interValue = readRegister(INT_MASK_ANT, 15, 4); //Value 1111 or first 4 bits
    return(_interValue); 
}

// REG0x03, bit [5], manufacturere default: 0.
// This setting will change whether or not disturbers trigger the IRQ Pin. 
void Qwiic_AS3935::maskDisturber(bool _state)
{
  if(_state == true)
    writeRegister(INT_MASK_ANT, (1<<5), 1, 5); 
  if(_state == false)
    writeRegister(INT_MASK_ANT, (1<<5), 0, 5); //Demonstrative
  
}

// REG0x03, bit [7:6], manufacturer default: 0 (16 division ratio). 
// The antenna is designed to resonate at 500kHz and so can be tuned with the
// following setting. The accuracy of the antenna must be within 3.5 percent of
// that value for proper signal validation and distance estimation.
void Qwiic_AS3935::antennaTuning(uint8_t _divisionRatio)
{
  if(_divisionRatio != 16 | _divisionRatio != 32 | _divisionRatio != 64 | _divisionRatio != 128)  
    return;

  if(_divisionRatio == 16) 
    writeRegister(INT_MASK_ANT, ((1<<7)|(1<<6)), 0, 6); //Demonstrative
  if(_divisionRatio == 32) 
    writeRegister(INT_MASK_ANT, ((1<<7)|(1<<6)), 1, 6); 
  else if(_divisionRatio == 64) 
    writeRegister(INT_MASK_ANT, ((1<<7)|(1<<6)), 1, 7); 
  else if(_divisionRatio == 128) 
    writeRegister(INT_MASK_ANT, ((1<<7)|(1<<6)), ((1<<7)|(1<<6)), 6); 
}
// REG0x07, bit [5:0], manufacturer default: 0. 
// This register holds the distance to the front of the storm and not the
// distance to a lightning strike.  
uint8_t Qwiic_AS3935::distanceToStorm()
{
  uint8_t _dist = readRegister(DISTANCE, 1); 
  return(_dist); 
}
// REG0x08, bits [5,6,7], manufacturer default: 0. 
// This will send the frequency of the oscillators to the IRQ pin. 
//  _osc 1, bit[5] = TRCO - Timer RCO Oscillators 1.1MHz
//  _osc 2, bit[6] = SRCO - System RCO at 32.768kHz
//  _osc 3, bit[7] = LCO - Frequency of the Antenna
void Qwiic_AS3935::displayOscillator(bool _state, uint8_t _osc)
{
  if(_osc < 0 | _osc > 3) 
    return;

  if(_state == true){
    if(_osc == 1)
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 1, 5); 
    if(_osc == 2)
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 1, 6); 
    if(_osc == 3)
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 1, 7); 
  }
  if(_state == false){
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 0, 5); //Demonstrative
    if(_osc == 2)
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 0, 6); 
    if(_osc == 3)
      writeRegister(FREQ_DISP_IRQ, OSC_MASK, 0, 7); 
  }
}
// REG0x08, bits [3:0], manufacturer default: 0. 
// This setting will add capacitance to the series RLC antenna on the product
// to help tune its resonance. The datasheet specifies being within 3.5 percent
// of 500kHz to get optimal lightning detection and distance sensing.  
// It's possible to add up to 120pF in steps of 8pF to the antenna. 
void Qwiic_AS3935::tuneCap(uint8_t _farad)
{
  if(_farad > 15)
   return;

  writeRegister(FREQ_DISP_IRQ, CAP_MASK, _farad, 0);    
}

// LSB =  REG0x04, bits[7:0]
// MSB =  REG0x05, bits[7:0]
// MMSB = REG0x06, bits[4:0]
// This returns a 20 bit value that is the 'energy' of the lightning strike.
// According to the datasheet this is only a pure value that doesn't have any
// physical meaning. 
void Qwiic_AS3935::lightningEnergy()
{
  uint8_t _lightBuf[3];
  _lightBuf[2] = readRegister(ENERGY_LIGHT_MMSB, 1);
  _lightBuf[2] &= 0xF; //Only interested in the first four bits. 
  _lightBuf[1] = readRegister(ENERGY_LIGHT_MSB, 1);
  _lightBuf[0] = readRegister(ENERGY_LIGHT_LSB, 1);
  return *_lightBuf;
}
  
// This function handles all I2C write commands. It takes the register to write
// to, then will mask the part of the register that coincides with the
// given register, and then write the given bits to the register starting at
// the given start position.  
void Qwiic_AS3935::writeRegister(uint8_t _reg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition)
{
	_i2cPort->beginTransmission(_address); 
	_i2cPort->write(_reg);
  _i2cPort->write(_reg &= (~mask)); 
  _i2cPort->write(_reg |= _bits<<_startPosition); 
  _i2cPort->endTransmission(); 
}

// This function reads the given register. 
uint8_t Qwiic_AS3935::readRegister(uint8_t reg, uint8_t _len)
{
    _i2cPort->beginTransmission(_address); 
    _i2cPort->write(reg); //Moves pointer to register in question and writes to it. 
    _i2cPort->endTransmission(false); //This tells the product 
    _i2cPort->requestFrom(_address, _len); 
    uint8_t _regValue = _i2cPort->read(reg); 
    return(_regValue);
}
