// Include the Arduino Library - to access the standard
// types and constants of the Arduino language.

#include <Arduino.h>
#include <Ethernet.h>

// Include the ACTLab header file where the class methods
// and properties are declared.

#include <ACTLab.h>

// Constructor method.

ACTLabClass::ACTLabClass () {
	// Set private properties.
	MAC(0x90,0xA2,0xDA,0x00,0x7F,0xAB);
	_serial = 0;
}

// ACTLab.MAC()

void ACTLabClass::MAC (byte b0,byte b1,byte b2,byte b3,byte b4,byte b5) {
	_mac[0] = b0;
	_mac[1] = b1;
	_mac[2] = b2;
	_mac[3] = b3;
	_mac[4] = b4;
	_mac[5] = b5;
}

// ACTLab.startEthernetClient()

void ACTLabClass::startEthernet () {
	_serialPrintln("Starting Ethernet.");
	Ethernet.begin(_mac);
	_serialPrintln("Ethernet Started.");
}

// ACTLab.connect()

void ACTLabClass::connect () {
	
	byte server[] = { 31,170,160,87 };
	String params = "t=0&i=1&iE=2&o=3&oE=6";
	
	EthernetClient client;
	
	_serialPrintln("connecting...");
	
	if (client.connect(server, 80)) { // port 80 is the default for http
		_serialPrintln("connected");
		client.print("GET ");
		client.print("http://actlab.comli.com/application/scripts/recordData.php?");
		client.print("t=0&i=1&iE=2&o=3&oE=6");
		client.println(" HTTP/1.0");
		client.println();
		
		client.stop();
		client.flush();
		_serialPrintln("disconnecting.");
		
	} else {_serialPrintln("Connection Failed.");}
	
}

// ACTLab.serial()

void ACTLabClass::serial (int arg) {
	if (arg==0||arg==1) {_serial = arg;}
}

// ACTLab.serialTest()

void ACTLabClass::serialTest () {
	Serial.println("Serial Test!");
}

// ----------

void ACTLabClass::_serialPrint (char str[]) {
	if (_serial) {Serial.print(str);}
}

// ----------

void ACTLabClass::_serialPrintln (char str[]) {
	if (_serial) {Serial.println(str);}
}

// Initialize an ACTLab object.

ACTLabClass ACTLab;