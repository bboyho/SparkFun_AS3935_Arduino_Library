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
    //Default Settings here and then many functions below to change those
    //settings, number of lightning strikes, watchdog threshold, masking
    //disturbers, antenna tuning, indoor/outdoor operation and the noise 
    //level threshold (AFE_GAIN BOOST)
}

//Need functions for resetting the noise level(indoor/outdoor), lightning threshold, and
//(un)masking disturbers, distance to storm, power down mode
//
//Questionable additions: LCO/TRCO/SRCO on IRQ, clear statistics, frequency
//division ration for antenna tuning
//
// This can be exported through the IRQ pin to an MCU to tune the respective
// frequencies:
//  LCO -  Frequency of the Antenna
//  TRCO - Timer RCO Oscillators 1.1MHz
//  SRCO - System RCO at 32.768kHz
Qwiic_AS3935::setNoiseLevel()
{
}

Qwiic_AS3935::lightningThreshold()
{
}

Qwiic_AS3935::distanceToLightning()
{
}

