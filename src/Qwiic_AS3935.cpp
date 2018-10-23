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
  //Need an I2C setting and an SPI setting.
}

Qwiic_AS3935::begin()
{
  Wire.begin();//Start I2C
    //Default Settings here and then many functions below to change those
    //settings, number of lightning strikes, watchdog threshold, masking
    //disturbers, antenna tuning, indoor/outdoor operation and the noise 
    //level threshold (AFE_GAIN BOOST)
}

// Could I write a function (maybe in example code) that would try to determine that
// threshold?
//
//Questionable additions: LCO/TRCO/SRCO on IRQ, clear statistics, frequency
//division ration for antenna tuning
//
// This can be exported through the IRQ pin to an MCU to tune the respective
// frequencies:
//  LCO -  Frequency of the Antenna
//  TRCO - Timer RCO Oscillators 1.1MHz
//  SRCO - System RCO at 32.768kHz

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
Qwiic_AS3935::setAFEGain()
{
}

// You'd want to unmask to help determine a good threshold.
Qwiic_AS3935::maskDisturber()
{
}

// When there is an event that exceeds the threshold, the register is written
// with the type of event. This consists of three messages: INT_NH (noise level too HIGH)
// which persists until it isn't, INT_D (disturber detected), INT_L(Lightning
// Interrupt).
Qwiic_AS3935::readInterruptReg()
{
}

// This is the floor that determines whether a distruber is a disturber 
// or actual lightning. This is dependent on the AFE gain and the watchdog
// sensitivity. 
Qwiic_AS3935::setNoiseLevel()
{
}

// The number of lightning events before IRQ is set high. The window of time
// before thie number of detected lightning events is 15 minutes. 
Qwiic_AS3935::lightningThreshold()
{
}

// This is a simple binary number of the distance in km to the front of the
// storm, not to the distance to lightning strike. 
Qwiic_AS3935::distanceToStorm()
{
}

// No explanation of this register in the datasheet.
Qwiic_AS3935::clearStatistics()
{
}

// This will be kept private since the antenna values are fixed.This will
// probably be kept to default but we'll determine through test. 
Qwiic_AS3935::antennaTuning()
{
}

// The frequency of the oscillators can be sent over the IRQ pin. 
Qwiic_AS3935::displayOscillatorSettings()
{
}
