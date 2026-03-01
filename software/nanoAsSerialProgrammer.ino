/* ----- Arduino Nano as Serial Programmer -----

    Using a counterfeit (non-original) Arduino Nano as a serial programmer 
    proved challenging. Standard approaches, such as uploading an empty sketch 
    or connecting the reset pin to ground, didn't work. To avoid waiting another 
    two weeks for replacement parts, I developed the following method to use
    the Nano as a serial programmer.

    ⚠ Important: Many serial devices operate at TTL voltage levels. To prevent 
    damage to your target device, use a voltage divider on the Arduino Nano's 
    D9 pin. Follow this wiring configuration:

    • Connect Arduino Nano D8 (RX) → TX pin of serial device
    • Connect Arduino Nano D9 (TX) → R1-1
    • Connect R1-2 and R2-1 → RX pin of serial device
    • Connect R2-2 → GND
    - R1 = 1k, R2 = 2k

    Note: The Arduino Nano's RX pin accepts TTL-level signals directly, so 
    no voltage divider is needed on this line.

    Hardware connections:
    - Arduino D8 (AltSoftSerial RX) → TX pin of serial device
    - Arduino D9 (AltSoftSerial TX) → RX pin of serial device (via voltage divider)
 */

#include <AltSoftSerial.h>

AltSoftSerial deviceSerial;   // RX on D8, TX on D9 (fixed pins for AltSoftSerial)

// Configure baud rates here for easy changes
const long PC_BAUD = 9600;        // USB serial to PC
const long DEVICE_BAUD = 9600;    // Communication with serial device
const int BOOT_DELAY = 15000;     // Delay for module initialization (ms)

void setup() {
  Serial.begin(PC_BAUD);
  deviceSerial.begin(DEVICE_BAUD);
  
  // Wait for serial ports to initialize
  delay(100);
  
  Serial.println(F("AltSoftSerial Serial bridge started"));
  Serial.print(F("Waiting "));
  Serial.print(BOOT_DELAY / 1000);
  Serial.println(F(" seconds for module to initialize..."));
  
  delay(BOOT_DELAY);  // Critical! Give module time to boot
  
  Serial.println(F("Ready. Type in Serial Monitor to send to module."));
}

void loop() {
  // Forward from serial device to PC
  while (deviceSerial.available()) {
    Serial.write(deviceSerial.read());
  }
  
  // Forward from PC to serial device
  while (Serial.available()) {
    deviceSerial.write(Serial.read());
  }
  
  // Small delay to prevent overwhelming slower devices
  delay(1);
}
