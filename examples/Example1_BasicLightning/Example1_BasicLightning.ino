#include <Wire.h>
#include <Sparkfun_AS3935.h>

Qwiic_AS3935 lightning;
void setup()
{
  Serial.begin(9600); 
  Serial.println("AS3935 Franklin Lightning Detector"); 

  Qwiic_AS3935.begin();
  
}

void loop()
{
}
