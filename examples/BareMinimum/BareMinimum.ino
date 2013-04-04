// Library includes.
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ACTLab.h>

void setup() {
  // Begin the serial connection with a 9600 baud.
  Serial.begin(9600);
  
  // Configure ACTLab.
  ACTLab.rig("tr");
  ACTLab.MAC(0x90,0xA2,0xDA,0x00,0x7F,0xAB);
  ACTLab.SDBuffer(0);              // 1 = Intending to use SD Buffer. [Default: 0]
  ACTLab.serialMessages(1);        // 1 = Print ACTLab progress messages to serial. [Default: 0]
  
  // Start the ACTLab Library.
  ACTLab.start();
}

void loop() {
}
