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

Qwiic_AS3935::begin(uint8_t address, TwoWire &wirePort)
{
  _address = address; 
  _i2cPort = &wirePort
  _i2cPort->begin(); 

  return 1;
}




// Occupies Reg0x00, bit[0].
// The product consumes 1-2uA while powered down. If the board is powered down 
// the the TRCO will need to be recalibrated: REG0x08[5] = 1, wait 2 ms, REG0x08[5] = 0.
void Qwiic_AS3935::powerDown()
{
  _i2cPort->beginTransmission(_address);     
  _i2cPort->write(AFE_GAIN);
  _i2cPort->write(AFE_GAIN |= 0x1); 
}

// This function modifies the Register0x00, bits [1:5]
void Qwiic_AS3935::indoorOutdoorSetting( _setting)
{
  if(_setting != INDOOR | _setting != OUTDOOR)
    return;

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(AFE_GAIN);
  _i2cPort->write(AFE_GAIN &= ~(GAIN_MASK << 1))
  if(_setting == INDOOR)
    _i2cPort->write(AFE_GAIN |= INDOOR);   
  if(_setting == OUTDOOR)
    _i2cPort->write(AFE_GAIN |= OUTDOOR);   
  writeRegister(AFE_GAIN, GAIN_MASK, INDOOR, 1); 
}

//  Occupies Reg0x00 bits[5:1]. 
//  The Watchdog Threshold is located in register0x01 bits [3:0] and settings range from 0-10.
// This setting determines the level of threshold for events that constitue a
// distruber vs lightning. Increasing this threshold reduces the chip's
// sensitivity to far away events.
void Qwiic_AS3935::watchdogThreshold(uint8_t _sensitivity)
{
  if(_sensitiviy > 10)// 10 is the max sensitivity setting
    return; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(THRESHOLD); 
  _i2cPort->write(THRESHOLD &= ~(GAIN_MASK << 1)); //Clear register
  _i2cPort->write(THRESHOLD |= (_sensitiviy << 1));//Apply setting
  _i2cPort->endTransmission(); 
  writeRegister(THRESHOLD, GAIN_MASK, _sensitiviy, 1); 
}
    //Default Settings here and then many functions below to change those
    //settings, number of lightning strikes, watchdog threshold, masking
    //disturbers, antenna tuning, indoor/outdoor operation and the noise 
    //level threshold (AFE_GAIN BOOST)

// This is the floor that determines whether a distruber is a disturber 
// or actual lightning. This is interconnected with the AFE gain and the watchdog
// sensitivity. This option is found in REG0x01, bits [6:4] and the default setting is  
// 010 or 2. 
void Qwiic_AS3935::setNoiseLevel(_floor)
{
  if(_floor < 0 | _floor > 7)
    return; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(THRESHOLD); 
  _i2cPort->write(THRESHOLD &= ~(FLOOR_MASK)); //Clear the register
  _i2cPort->write(THRESHOLD |= (_floor << 4);  //Write the floor setting
  _i2cPort->endTransmission();  
  
  writeRegister(THRESHOLD, FLOOR_MASK, _floor, 4); 

}

void Qwiic_AS3935::spikeReduction(_spSensitivity)
{
  //Sensitivity ranges from 0-11 and occupy bits [3:0], default is 2. 
  if(_spSensitivity < 0 | _spSensitivity > 11)
    return; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(LIGHTNING); 
  _i2cPort->write(LIGHTNING &= ~(SPIKE_MASK); 
  _i2cPort->write(LIGHTNING |= (_spSensitivity)); 
  _i2cPort->endTransmission(); 

  writeRegister(LIGHTNING, SPIKE_MASK, _spSensitivity, 0); 
}


// The number of lightning events before IRQ is set high. The window of time
// before thie number of detected lightning events is 15 minutes. 
// Number of lightning strikes can be 1,5,9, and 16. 
void Qwiic_AS3935::lightningThreshold(uint8_t _strikes)
{

  if(_strikes != 5 | _strikes != 9 | _strikes != 16) 
    return; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(LIGHTNING); 
  _i2cPort->write(LIGHTNING &= ~((1<<5)|(1<<4)));

  if( _strikes == 5)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), 1, 4); 
    _i2cPort->write(LIGHTNING |= (1<<4));  
  if( _strikes == 9)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), 1, 5); 
    _i2cPort->write(LIGHTNING |= (1<<5)); 
  if( _strikes == 16)
    writeRegister(LIGHTNING, ((1<<5)|(1<<4)), (1<<5)|(1<<4), 4); 
    _i2cPort->write(LIGHTNING |= ((1<<5)|(1<<4)));
}


// No explanation of this register in the datasheet.
void Qwiic_AS3935::clearStatistics()
{
}
// When there is an event that exceeds the threshold, the register is written
// with the type of event. This consists of three messages: INT_NH (noise level too HIGH)
// which persists until it isn't, INT_D (disturber detected), INT_L(Lightning
// Interrupt).
uint8_t Qwiic_AS3935::readInterruptReg()
{
    //The first 4 bits of this register are the interrupt
    uint8_t __interValue; 
    _interValue = readRegister(INT_MASK_ANT, 15, 4); //Value 1111 or first 4 bits
    return(_interValue); 
}

// You'd want to unmask to help determine a good threshold,but I think in
// general this register should be off by default. It is on by manufacturer's
// default. 
void Qwiic_AS3935::maskDisturber(uint8_t _state)
{
  // Register 0x03 bit 5 masks disturbers, hich are determined by the IC to be
  // non-lightning events. 
  if(_state != 0 || _state != 1)
    return; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(INT_MASK_ANT); 
  _i2cPort->write(INT_MASK_ANT &= ~(1<<5)); 
  _i2cPort->write(INT_MASK_ANT |= (_state << 5);
  _i2cPort->endTransmission(); 
}

// Probably be kept to default but we'll determine through test. 
// I should probably determine best tuning given gain settings because
// datasheet doesn't give any. 
void Qwiic_AS3935::antennaTuning(uint8_t _divisionRatio)
{
  // The LCO_FDIV occupies bits [7:6]
  // Default for the Antenna is a division ratio of 16. 
  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(INT_MASK_ANT); 
  _i2cPort->write(INT_MASK_ANT &= ~((1<<7)|(1<<6))); 

  // Already cleared the register to the default above.
  if(_divisionRatio == 16) 
    return;
  if(_divisionRatio == 32) 
    _i2cPort->write(INT_MASK_ANT |= (1<<6)); 
  else if(_divisionRatio == 64) 
    _i2cPort->write(INT_MASK_ANT |= (1<<7)); 
  else if(_divisionRatio == 128) 
    _i2cPort->write(INT_MASK_ANT |= (1<<7)|(1<<6)); 
}
// This is a simple binary number of the distance in km to the front of the
// storm, not to the distance to lightning strike. 
uint8_t Qwiic_AS3935::distanceToStorm()
{
  uint8_t _dist = readRegister(DISTANCE, 1); 
  return(_dist); 
}

// The frequency of the oscillators can be sent over the IRQ pin. 
//  LCO -  Frequency of the Antenna
//  TRCO - Timer RCO Oscillators 1.1MHz
//  SRCO - System RCO at 32.768kHz
void Qwiic_AS3935::displayOscillatorSettings(_state, _osc)
{
  if(_state > 1 | _osc > 3) 
    return;

  //Pin 7 enables/disables diplaying the oscillator data on the IRQ Pin.
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(FREQ_DISP_IRQ); 

  if(_osc == 1){
    if(_state == 0);  
      _i2cPort->write(FREQ_DISP_IRQ,  
    if(_state == 1);  
  }
  if(_osc == 2){
    if(_state == 0);  
      return
    if(_state == 1);  
  }
  if(_osc == 3){
    if(_state == 0);  
      return
    if(_state == 1);  
  {    
  _i2cPort->write(FREQ_DISP_IRQ |= (1<<7)); 
}

Qwiic_AS3935::writeRegister(uint8_t _reg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition)
{
	_i2cPort->beginTransmission(_address); 
	_i2cPort->write(_reg);
  _i2cPort->write(_reg &= (~mask)); 
  _i2cPort->write(_reg |= _bits<<_startPosition); 
  _i2cPort->endTransmission(); 
}

uint8_t Qwiic_AS3935::readRegister(uint8_t reg, uint8_t _len)
{
    _i2cPort->beginTransmission(_address); 
    _i2cPort->write(reg); //Moves pointer to register in question and writes to it. 
    _i2cPort->endTransmission(false); //This tells the product 
    // Most cases this will be a single uint8_t except for Energy of Lightning
    // Register.
    _i2cPort->requestFrom(_address, _len); 
    uint8_t _regValue = _i2cPort->read(reg); 
    return(_regValue);
}
