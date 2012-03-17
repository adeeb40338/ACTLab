
// If statement to make sure the ACTLab Library is not
// included more than once.

#ifndef ACTLab_h
#define ACTLab_h

// Include the Arduino Library - to access the standard
// types and constants of the Arduino language.

#include <Arduino.h>

// Declare (and spec out) the ACTLabClass class.

class ACTLabClass {
	
	// Public methods and properties.
	
	public:
	
		// Constructor method.
		
		ACTLabClass();
		
		// Public methods.
		
		void serialTest();
		
		// Public properties.
		
		int qwerty;
	
	// Private methods and properties.
	
	
	
};

extern ACTLabClass ACTLab;

#endif