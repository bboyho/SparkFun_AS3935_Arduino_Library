#include <SPI.h>
#include <Wire.h>
#include <Sparkfun_AS3935.h>

// 0x03 is default, but the address can also be 0x02, 0x01, or 0x00
// Adjust the address jumpers on the underside of the product. 
#define AS3935_ADDR 0x03 
#define INDOOR 0x12 
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01
#define NOISE_FLOOR 0x02
#define WATCHDOG_THRESH 0x02
#define SPIKE 0x02


// Instance of our lightning detector.
SparkFun_AS935 lightning;

// Interrupt pin for lightning detection 
const uint8_t lightningInt = 1; 
uint8_t intVal; 
uint8_t distance; 

void setup()
{
  // When lightning is detected the interrupt pin goes HIGH.
  pinMode(lightningInt, INPUT); 

  Serial.begin(9600); 
  Serial.println("AS3935 Franklin Lightning Detector"); 

  Wire.begin(); // Begin Wire before lightning sensor. 
  lightning.begin(); // Initialize the sensor. 
  
  // Valid spike rejection values range from 0-13 
  lightning.spikeReduction(++SPIKE); 
  lightning.maskDisturber(true); 
  // The lightning detector defaults to an indoor setting (less
  // gain/sensitivity), if you plan on using this outdoors 
  // uncomment the following line:
  //lightning.setIndoorOutdoor(OUTDOOR); 
  
}

void loop()
{
  if(digitalRead(lightningInt) == HIGH){
    // Hardware has alerted us to an event, now we read the interrupt register
    // to see exactly what it is. 
    intVal = lightning.readInterruptReg();
    if(intVal == NOISE_INT){
      Serial.println("Noise."); 
    }
    else if(intVal == DISTURBER_INT){
      Serial.println("Disturber."); 
    }
    else if(intVal == LIGHTNING_INT){
      Serial.println("Lightning Strike Detected!"); 
      // Lightning! Now how far away is it? Distance estimation takes into
      // account previously seen events. 
      distance = lightning.distanceToStorm(); 
      Serial.print("Approximately: "); 
      Serial.print(distance); 
      Serial.print("km away!"); 
    }
  }
}

// This function helps to adjust the sensor to your environment. More
// environmental noise leads to more false positives. If you see lots of noise
// events, try calling this function.  
void reduceNoise(){
  ++NOISE_FLOOR; // Manufacturer's default is 2 with a max of 7. 
  if(NOISE_FLOOR > 7){
    Serial.println("Noise floor is at max!"); 
    return;
  }
  Serial.println("Increasing the event threshold.")
  lightning.setNoiseLevel(NOISE_FLOOR):  
}
