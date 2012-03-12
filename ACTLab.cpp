#include <WProgram.h>
#include <ACTLab.h>

void ACTLabClass::dot(int _pin)
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void ACTLabClass::dash(int _pin)
{
  digitalWrite(_pin, HIGH);
  delay(1500);
  digitalWrite(_pin, LOW);
  delay(250);
}

ACTLabClass ACTLab;