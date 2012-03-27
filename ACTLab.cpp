// Include the Arduino Library - to access the standard
// types and constants of the Arduino language. Also include
// the Ethernet Library to access Ethernet Shield methods.

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
	_HTTP = 1;
	_dataBufferLocation = 1;
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

// ACTLab.rig()

void ACTLabClass::rig (String rig) {
	rig.toCharArray(_rig,16);
}

// ACTLab.HTTP()

void ACTLabClass::HTTP (int arg) {
	if (arg==0||arg==1) {_HTTP = arg;};
}

// ACTLab.startEthernet()

void ACTLabClass::startEthernet () {
	_serialPrintln("Starting ethernet.");
	//Ethernet.begin(_MAC);
	if (Ethernet.begin(_MAC) == 0) {
		Serial.println("Failed to start ethernet.");
		// For now SD card is not in use.
		pinMode(4,OUTPUT); digitalWrite(4,HIGH);
	} else { _serialPrintln("Ethernet started.");};
}

// ACTLab.checkForInstruction()

bool ACTLabClass::checkForInstruction () {
	// Initialize a static (persistant) EthernetClient object, called client.
	static EthernetClient client;		
	
	// Variable declarations.
	bool rtn = false;
	bool whileBool = true;
	int status_experiment = 0;		// 0 = Not started. 1 = Started. 2 = Ended.
	int status_parameters = 0;		// 0 = Not started. 1 = Started. 2 = Ended.
	char experimentBuffer[8] = {};
	char parametersBuffer[128] = {};
	char *p_pos_1;
	char *p_pos_2 = parametersBuffer;
	char *p_pos_3;
	char *p_param;
	int _parametersPos = 0;
	
	// Connect to server.
	_serialPrintln("Connecting to server.");
	if (client.connect(_server, 80)) { // Port 80 is the default for HTTP.
		_serialPrintln("Connected to server.");
		
		// Build up the http request's parameters.
		char paramsBuffer[60] = {}; // Nice conservative buffer size.
		strcat(paramsBuffer,"rig=");
		strcat(paramsBuffer,_rig);
		strcat(paramsBuffer,"&action=check");
		strcat(paramsBuffer,"\0");
		String paramsEncoded = paramsBuffer;
		paramsEncoded.replace("+","%2B");
		paramsEncoded.replace("-","%2D");
		
		// Decide whether to send a GET or POST HTTP request.
		if (_HTTP==0) {
			// Send a HTTP GET request.
			client.print("GET ");
			client.print("http://actlab.comli.com/application/scripts/instruction.php?");
			client.print(paramsEncoded);
			client.println(" HTTP/1.0");
			client.println();
		} else {
			// Send a HTTP POST request - Default (should be anyway).
			client.print("POST ");
			client.print("http://actlab.comli.com/application/scripts/instruction.php");
			client.println(" HTTP/1.0");
			client.println("Content-Type: application/x-www-form-urlencoded");
			client.print("Content-Length: ");
			client.println(paramsEncoded.length());
			client.println();
			client.print(paramsEncoded); // ! - Does this need to be encoded?
			client.println();
		};
		
		while(whileBool){
			// Check if there are incomming bytes from the server.
			if (client.available()) {
				// Get the next character.
				char c = client.read();
				
				// Check if the character is not a bracket.
				if (c!='{'&&c!='}'&&c!='['&&c!=']') {
					// Upgrade c char to string in a temp variable.
					char temp[2] = {c,'\0'};
					
					// If reading experiment.
					if (status_experiment==1) {strcat(experimentBuffer,temp);};
					// If reading parameters.
					if (status_parameters==1) {strcat(parametersBuffer,temp);};
				};
				
				// If c = experiment bracket start.
				if (c=='{') {status_experiment=1;};
				// If c = experiment bracket end.
				if (c=='}') {status_experiment=2;};
				// If c = parameters bracket start.
				if (c=='[') {status_parameters=1;};
				// If c = parameters bracket end.
				if (c==']') {status_parameters=2;rtn = true;};
			};
			
			// If the server has disconnected, stop the while loop.
			if (!client.connected()) {whileBool = false;};
		};
		
		// If instructions successfully recieved, process them.
		if (rtn) {
			// Convert the experiment number from string to double (then to int via equality).
			_experiment = strtod(experimentBuffer,&p_pos_1);
			
			// Clear the _parameters array.
			for (int i = 0; i < 10; i++) {_parameters[i] = NULL;};
			
			// Process the parameters to doubles.
			while ((p_param = strtok_r(p_pos_2, ",", &p_pos_2)) != NULL) { // Delimiter: comma.
				_parameters[_parametersPos] = strtod(p_param,&p_pos_3);
				_parametersPos ++;
			};
		};
		
		// Disconnect from server.
		client.stop();
		client.flush();
		_serialPrintln("Disconnected from server.");
		
		// If instructions successfully recieved, let server know.
		if (rtn) {
			// Connect to server(2).
			_serialPrintln("Connecting to server(2).");
			if (client.connect(_server, 80)) { // Port 80 is the default for HTTP.
				_serialPrintln("Connected to server(2).");
				
				// Build up the http request's parameters.
				char paramsBuffer[60] = {}; // Nice conservative buffer size.
				strcat(paramsBuffer,"rig=");
				strcat(paramsBuffer,_rig);
				strcat(paramsBuffer,"&action=received");
				strcat(paramsBuffer,"\0");
				String paramsEncoded = paramsBuffer;
				paramsEncoded.replace("+","%2B");
				paramsEncoded.replace("-","%2D");
				
				// Decide whether to send a GET or POST HTTP request.
				if (_HTTP==0) {
					// Send a HTTP GET request.
					client.print("GET ");
					client.print("http://actlab.comli.com/application/scripts/instruction.php?");
					client.print(paramsEncoded);
					client.println(" HTTP/1.0");
					client.println();
				} else {
					// Send a HTTP POST request - Default (should be anyway).
					client.print("POST ");
					client.print("http://actlab.comli.com/application/scripts/instruction.php");
					client.println(" HTTP/1.0");
					client.println("Content-Type: application/x-www-form-urlencoded");
					client.print("Content-Length: ");
					client.println(paramsEncoded.length());
					client.println();
					client.print(paramsEncoded); // ! - Does this need to be encoded?
					client.println();
				};
				
				// Disconnect from server.
				client.stop();
				client.flush();
				_serialPrintln("Disconnecting from server(2).");
			}
			// Could not connect to server.
			else {_serialPrintln("Connection to server failed(2).");};
		};
	}
	// Could not connect to server.
	else {_serialPrintln("Connection to server failed.");};
	
	// Return outcome.
	return rtn;
}

// ACTLab.getExperimentNumber()

int ACTLabClass::getExperimentNumber () {
	return _experiment;
}

// ACTLab.getParameter()

double ACTLabClass::getParameter (int number) {
	return _parameters[number];
}

// ACTLab.submitData()

void ACTLabClass::submitData (double time, double reference, double input, double output) {
	// Initialize a static (persistant) EthernetClient object, called client.
	static EthernetClient client;
	
	// Declare the buffers for the three parameters.
	char param_time[12];
	char param_reference[12];
	char param_input[12];
	char param_output[12];
	
	// Convert the three parameters from doubles to an ASCII representation (/string).
	dtostre(time,param_time,3,0);
	dtostre(reference,param_reference,3,0);
	dtostre(input,param_input,3,0);
	dtostre(output,param_output,3,0);
	
	// Build up the http request's parameters.
	char paramsBuffer[60] = {}; // Nice conservative buffer size.
	strcat(paramsBuffer,"rig=");
	strcat(paramsBuffer,_rig);
	strcat(paramsBuffer,"&t=");
	strcat(paramsBuffer,param_time);
	strcat(paramsBuffer,"&r=");
	strcat(paramsBuffer,param_reference);
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
	_serialPrint("reference = ");_serialPrintln(param_reference);
	_serialPrint("input = ");_serialPrintln(param_input);
	_serialPrint("output = ");_serialPrintln(param_output);
	_serialPrint("paramsBuffer = ");_serialPrintln(paramsBuffer);
	_serialPrint("paramsEncoded = ");if (_serial) {Serial.println(paramsEncoded);};
	
	// Connect to server.
	_serialPrintln("Connecting to server.");
	if (client.connect(_server, 80)) { // Port 80 is the default for HTTP.
		_serialPrintln("Connected to server.");
		
		// Decide whether to send a GET or POST HTTP request.
		if (_HTTP==0) {
			// Send a HTTP GET request.
			client.print("GET ");
			client.print("http://actlab.comli.com/application/scripts/recordData.php?");
			client.print(paramsEncoded);
			client.println(" HTTP/1.0");
			client.println();
		} else {
			// Send a HTTP POST request - Default (should be anyway).
			client.print("POST ");
			client.print("http://actlab.comli.com/application/scripts/recordData.php");
			client.println(" HTTP/1.0");
			client.println("Content-Type: application/x-www-form-urlencoded");
			client.print("Content-Length: ");
			client.println(paramsEncoded.length());
			client.println();
			client.print(paramsEncoded); // ! - Does this need to be encoded?
			client.println();
		};
		
		// Disconnect from server.
		client.stop();
		client.flush();
		_serialPrintln("Disconnecting from server.");
	}
	// Could not connect to server.
	else {_serialPrintln("Connection to server failed.");};
}

// ACTLab.dataBufferLocation()

void ACTLabClass::dataBufferLocation (char location[]) { 
}

// ACTLab.clearDataBuffer()

void ACTLabClass::clearDataBuffer () { 
}

// ACTLab.addToDataBuffer()

void ACTLabClass::addToDataBuffer (double time, double reference, double input, double output) {
}

// ACTLab.submitDataBuffer()

void ACTLabClass::submitDataBuffer () {
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