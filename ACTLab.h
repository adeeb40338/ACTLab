// If statement to make sure the ACTLab Library is not
// included more than once.

#ifndef ACTLab_h
#define ACTLab_h

// Include the Arduino Library - to access the standard
// types and constants of the Arduino language.

#include <Arduino.h>
#include <Ethernet.h>

// Declare (and spec out) the ACTLabClass class.
class ACTLabClass {
	// Public methods and properties.
	public:
		// Constructor method.
		ACTLabClass();
		
		// Public methods.
		void MAC(byte b0,byte b1,byte b2,byte b3,byte b4,byte b5);
		void server(byte b0,byte b1,byte b2,byte b3);
		void rig(String rig);
		void HTTP(int arg);
		void startEthernet();
		void submitData(double time,double input, double output);
		void serial(int arg);
		
		// Public properties.
	
	// Private methods and properties.
	private:
		// Private methods.
		void _serialPrint(char str[]);
		void _serialPrintln(char str[]);
		
		// Private properties.
		byte _MAC[6];
		byte _server[4];
		char _rig[16];
		int _HTTP;			// 0 = GET; 1 = POST; Default - 1.
		int _serial;		// 0 = OFF; 1 = ON; Default - 0.
};

// Code to remove the need to initialize an ACTLab
// object in sketch.

extern ACTLabClass ACTLab;

#endif