/*
  This is a library for the ASM AS3935 Franklin Lightning Detector
  By: Elias Santistevan
  SparkFun Electronics
  Date: October, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
*/

#include "Qwiic_AS3935.h"

Qwiic_AS3935::Qwiic_AS3935(byte addr)
{
  //Need an I2C setting and an SPI setting.
  _address = addr
}

Qwiic_AS3935::begin()
{
  Wire.begin();//Start I2C
    //Default Settings here and then many functions below to change those
    //settings, number of lightning strikes, watchdog threshold, masking
    //disturbers, antenna tuning, indoor/outdoor operation and the noise 
    //level threshold (AFE_GAIN BOOST)
  return 1;
}

// Could I write a function (maybe in example code) that would try to determine that
// threshold?
//
// Questionable additions: LCO/TRCO/SRCO on IRQ, clear statistics, frequency
// division ration for antenna tuning
//
// This can be exported through the IRQ pin to an MCU to tune the respective
// frequencies:
//  LCO -  Frequency of the Antenna
//  TRCO - Timer RCO Oscillators 1.1MHz
//  SRCO - System RCO at 32.768kHz

// Why does the Arduino API guidelines contradict literally every other style
// guildline in the universe???
Qwiic_AS3935::spikeRejection()//better name: spike reduction?
{
  
}
//Will consume 1-2uA. If the board is powered down the the TRCO will need to be
//recalibrated. REG0x08[5] = 1, wait 2 ms, REG0x08[5] = 0.
Qwiic_AS3935::powerDown()
{
}
/
// Indoor and outdoor settings i.e. the antenna gain or sensitivity to events. 
Qwiic_AS3935::setAFEGain(byte _sensitivity)
{
  // Anttena gain is located in bits [5:1] 
  // Sensitivy settings range from 0 - 10
  // Datasheet specifies 9 as "indoor" (larger threshold) and outdoor with
  // greater sensitivity (lower threshold) as 7. 
  Wire.beginTransmission(_address); 
  Wire.write(THRESHOLD); 
  Wire.write(THRESHOLD &= ~(GAIN_MASK << 1)); 

  if(_sensitiviy > 10)
    return; 

  Wire.write(THRESHOLD |= (_sensitiviy << 1)); 
  Wire.endTransmission(); 

}

// You'd want to unmask to help determine a good threshold,but I think in
// general this register should be off by default. It is on by manufacturer's
// default. 
Qwiic_AS3935::maskDisturber(byte _state)
{
  // Register 0x03 bit 5 masks disturbers, hich are determined by the IC to be
  // non-lightning events. 
  if(_state != 0 || _state != 1)
    return; 

  Wire.beginTransmission(_address); 
  Wire.write(INT_MASK_ANT); 
  Wire.write(INT_MASK_ANT &= ~(1<<5)); 
  Wire.write(INT_MASK_ANT |= (_state << 5);
  Wire.endTransmission(); 
}

// When there is an event that exceeds the threshold, the register is written
// with the type of event. This consists of three messages: INT_NH (noise level too HIGH)
// which persists until it isn't, INT_D (disturber detected), INT_L(Lightning
// Interrupt).
Qwiic_AS3935::readInterruptReg()
{
    //The first 4 bits of this register are the interrupt
    byte __interValue; 
    _interValue = readRegister(INT_MASK_ANT, 15, 4); //Value 1111 or first 4 bits
    return(_interValue); 
}

// This is the floor that determines whether a distruber is a disturber 
// or actual lightning. This is dependent on the AFE gain and the watchdog
// sensitivity. 
Qwiic_AS3935::setNoiseLevel()
{
}

// The number of lightning events before IRQ is set high. The window of time
// before thie number of detected lightning events is 15 minutes. 
// Number of lightning strikes can be 1,5,9, and 16. 
Qwiic_AS3935::lightningThreshold(byte _strikes)
{
  //Bits [5:4] hold the lightning strike data. 
  Wire.beginTransmission(_address); 
  Wire.write(LIGHTNING); 
  // Clears the register which coincidentally also is the default: 1 lightning
  // strike.
  Wire.write(LIGHTNING &= ~((1<<5)|(1<<4)));
  if( _strikes == 5)
    Wire.write(LIGHTNING |= (1<<4));  
  if( _strikes == 9)
    Wire.write(LIGHTNING |= (1<<5)); 
  if( _strikes == 16)
    Wire.write(LIGHTNING |= ((1<<5)|(1<<4)));
}

// This is a simple binary number of the distance in km to the front of the
// storm, not to the distance to lightning strike. 
Qwiic_AS3935::distanceToStorm()
{
  byte _dist = readRegister(DISTANCE, 1); 
  return(_dist); 
}

// No explanation of this register in the datasheet.
Qwiic_AS3935::clearStatistics()
{
}

// Probably be kept to default but we'll determine through test. 
// I should probably determine best tuning given gain settings because
// datasheet doesn't give any. 
Qwiic_AS3935::antennaTuning(byte _divisionRatio)
{
  // The LCO_FDIV occupies bits [7:6]
  // Default for the Antenna is a division ratio of 16. 
  Wire.beginTransmission(_address); 
  Wire.write(INT_MASK_ANT); 
  Wire.write(INT_MASK_ANT &= ~((1<<7)|(1<<6))); 

  // Already cleared the register to the default above.
  if(_divisionRatio == 16) 
    return;
  if(_divisionRatio == 32) 
    Wire.write(INT_MASK_ANT |= (1<<6)); 
  else if(_divisionRatio == 64) 
    Wire.write(INT_MASK_ANT |= (1<<7)); 
  else if(_divisionRatio == 128) 
    Wire.write(INT_MASK_ANT |= (1<<7)|(1<<6)); 
}

// The frequency of the oscillators can be sent over the IRQ pin. 
Qwiic_AS3935::displayOscillatorSettings()
{
}

Qwiic_AS3935::writeRegister(byte reg)
{
	Wire.beginTransmission(_address); 
	Wire.write(reg);
  Wire.endTransmission(); 
}

Qwiic_AS3935::readRegister(byte reg, byte _len)
{
    Wire.beginTransmission(_address); 
    Wire.write(reg); //Moves pointer to register in question and writes to it. 
    Wire.endTransmission(false); //This tells the product 
    // Most cases this will be a single byte except for Energy of Lightning
    // Register.
    Wire.requestFrom(_address, _len); 
    byte _regValue = Wire.read(reg); 
    return(_regValue);
}
