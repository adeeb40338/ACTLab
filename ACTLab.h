// If statement to make sure the ACTLab Library is not
// included more than once.

#ifndef ACTLab_h
#define ACTLab_h

// Include the Arduino Library - to access the standard
// types and constants of the Arduino language. Also include
// the Ethernet Library to access Ethernet Shield methods.

#include <Arduino.h>
#include <Ethernet.h>
#include <SD.h>

// Declare (and define) the ACTLabClass class.
class ACTLabClass {
	// Public methods and properties.
	public:
		// Constructor method.
		ACTLabClass();
		
		// Public methods.
		// Configuration methods:
		void	rig(String rig);
		void	MAC(byte b0,byte b1,byte b2,byte b3,byte b4,byte b5);
		void	SDBuffer(int arg);
		void	server(byte b0,byte b1,byte b2,byte b3);
		void	HTTP(int arg);
		void	SDPin(int arg);
		void	serialMessages(int arg);
		// Core ethernet methods:
		void	start();
		bool	checkForInstruction();
		int		getExperimentNumber();
		double	getParameter(int number);
		void	submitData(double time, double reference, double input, double output);
		// SD buffer methods:
		void	SDBuffer_clear();
		void	SDBuffer_add(double time, double reference, double input, double output);
		void	SDBuffer_submit(); // !!!!!!!!!!
		
		// Public properties.
	
	// Private methods and properties.
	private:
		// Private methods.
		void	_ethernetClient(char url[], char params[]);
		void	_serialPrint(char str[]);
		void	_serialPrintln(char str[]);
		
		// Private properties.
		char	_rig[16];
		byte	_MAC[6];
		int		_SDBuffer;			// 0 = OFF; 1 = ON; Default - 0.
		byte	_server[4];
		int		_HTTP;				// 0 = GET; 1 = POST; Default - 1.
		int		_SDPin;				// 4 - SD CS is on pin 4 on E.S.
		int		_serial;			// 0 = OFF; 1 = ON; Default - 0.
		int		_experiment;
		double	_parameters[10];
};

// Code to remove the need to initialize an ACTLab
// object in sketch.

extern ACTLabClass ACTLab;

#endif