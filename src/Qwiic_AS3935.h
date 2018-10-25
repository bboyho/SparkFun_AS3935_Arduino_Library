
#include "Wire.h"
#include "SPI.h"

#ifndef AS3935.h
#define AS3935.h

enum AS3935_Register {
  //Most, if not all of these registers have many functions. 

	AFE_GAIN          = 0x00,
  THRESHOLD         = 0x01,
  LIGHTNING         = 0x02,
  INT_MASK          = 0x03,
  ENERGY_LIGHT_LSB  = 0x04,
  ENERGY_LIGHT_MSB  = 0x05,
  ENERGY_LIGHT_MMSB = 0x06,
  DISTANCE          = 0x07,
  FREQ_IRQ          = 0x08,

};

#define NOISE_TO_HIGH 0x01 //B00000001
#define DISTURBER_DETECT 0x04 //B00000100
#define LIGHTNING 0x08 //B00001000
#define INDOOR  0x12
#define OUTDOOR  0xE
#define GAIN_MASK 0x3E
#define DISTANCE_MASK 0x1F
class Qwiic_AS3935
{

};
#endif

