// Include the Arduino Library - to access the standard
// types and constants of the Arduino language.

#include <Arduino.h>

// Include the ACTLab header file where the class methods
// and properties are declared.

#include <ACTLab.h>

// Constructor method.

ACTLabClass::ACTLabClass () {}

// Test method. Outputs a serial message. Serial need to
// already be setup.

void ACTLabClass::serialTest () {
	Serial.println("Serial Test!");
}

ACTLabClass ACTLab;