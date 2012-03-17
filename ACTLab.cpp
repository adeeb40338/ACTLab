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
	server(31,170,160,87);
	_serial = 0;
}

// ACTLab.MAC()

void ACTLabClass::MAC (byte b0,byte b1,byte b2,byte b3,byte b4,byte b5) {
	_MAC[0] = b0;
	_MAC[1] = b1;
	_MAC[2] = b2;
	_MAC[3] = b3;
	_MAC[4] = b4;
	_MAC[5] = b5;
}

// ACTLab.server()

void ACTLabClass::server (byte b0,byte b1,byte b2,byte b3) {
	_server[0] = b0;
	_server[1] = b1;
	_server[2] = b2;
	_server[3] = b3;
}

// ACTLab.startEthernet()

void ACTLabClass::startEthernet () {
	_serialPrintln("Starting Ethernet.");
	Ethernet.begin(_MAC);
	_serialPrintln("Ethernet Started.");
}

// ACTLab.submitData()

void ACTLabClass::submitData (double time,double input, double output) {
	// Initialize a static (persistant) EthernetClient object, called client.
	static EthernetClient client;
	
	// Declare the buffers for the three parameters.
	char param_time[12];
	char param_input[12];
	char param_output[12];
	
	// Convert the three parameters from doubles to an ASCII representation (/string).
	dtostre(time,param_time,3,0);
	dtostre(input,param_input,3,0);
	dtostre(output,param_output,3,0);
	
	// Build up the http request's parameters.
	char paramsBuffer[50] = {}; // Nice conservative buffer size.
	strcat(paramsBuffer,"t=");
	strcat(paramsBuffer,param_time);
	strcat(paramsBuffer,"&i=");
	strcat(paramsBuffer,param_input);
	strcat(paramsBuffer,"&o=");
	strcat(paramsBuffer,param_output);
	strcat(paramsBuffer,"\0");
	String paramsEncoded = paramsBuffer;
	paramsEncoded.replace("+","%2B");
	paramsEncoded.replace("-","%2D");
	
	// Serial output the parameters in their various forms for info.
	_serialPrint("time = ");_serialPrintln(param_time);
	_serialPrint("input = ");_serialPrintln(param_input);
	_serialPrint("output = ");_serialPrintln(param_output);
	_serialPrint("paramsBuffer = ");_serialPrintln(paramsBuffer);
	_serialPrint("paramsEncoded = ");if (_serial) {Serial.println(paramsEncoded);};
	
	// Connect to server.
	_serialPrintln("Connecting to server.");
	if (client.connect(_server, 80)) { // Port 80 is the default for HTTP.
		_serialPrintln("Connected to server.");
		
		// Send a HTTP GET request.
		client.print("GET ");
		client.print("http://actlab.comli.com/application/scripts/recordData.php?");
		client.print(paramsEncoded);
		client.println(" HTTP/1.0");
		client.println();
		
		// Disconnect from server.
		client.stop();
		client.flush();
		_serialPrintln("Disconnecting from server.");
		
	}
	// Could not connect to server.
	else {_serialPrintln("Connection to server failed.");};
}

// ACTLab.serial()

void ACTLabClass::serial (int arg) {
	if (arg==0||arg==1) {_serial = arg;};
}

// ----------

void ACTLabClass::_serialPrint (char str[]) {
	if (_serial) {Serial.print(str);};
}

// ----------

void ACTLabClass::_serialPrintln (char str[]) {
	if (_serial) {Serial.println(str);};
}

// Initialize an ACTLab object.

ACTLabClass ACTLab;