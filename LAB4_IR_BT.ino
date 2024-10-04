#include <IRremote.hpp>
#include <SoftwareSerial.h> // Include SoftwareSerial library for Bluetooth communication

// Pin assignments
const int rcvPin = 13; // IR receiver pin
const int relayPin = 2; // Relay pin
const int numLEDs = 8; // Number of LEDs
const int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10}; // LED pins
const int dipPin1 = A0; // DIP switch pin 1
const int dipPin2 = A1; // DIP switch pin 2
const int blueLedPin = 12; // Blue LED for IR mode
const int whiteLedPin = 11; // White LED for Bluetooth mode
const int btTxPin = 1; // Bluetooth TX pin
const int btRxPin = 0; // Bluetooth RX pin
const int btKeyPin = A2; // Bluetooth KEY/EN pin for AT command mode

// IR remote button values
const unsigned long BUTTON_ON = 4077715200; // Button 1 
const unsigned long BUTTON_OFF = 3877175040; // Button 2 
const unsigned long BUTTON_RUN_LEDS = 2707357440; // Button 3
const unsigned long BUTTON_TURN_OFF_LEDS = 4144561920; // Button 4

// Bluetooth commands
char bluetoothCommand;

IRrecv irrecv(rcvPin);
decode_results results;
SoftwareSerial bluetooth(btRxPin, btTxPin); // RX, TX for Bluetooth module

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600); // Initialize Bluetooth serial communication
  irrecv.enableIRIn(); // Start the IR receiver

  // Set the KEY pin as output for Bluetooth module mode control
  pinMode(btKeyPin, OUTPUT);

  // Set the KEY pin LOW for normal operation (HIGH would put it in AT command mode)
  digitalWrite(btKeyPin, LOW);

  // Set up LED pins
  for (int i = 0; i < numLEDs; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Set up relay and LED indicator pins
  pinMode(relayPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);
  
  // Set up DIP switch pins
  pinMode(dipPin1, INPUT);
  pinMode(dipPin2, INPUT);
}

void loop() {
  int dipSwitchValue = (digitalRead(dipPin1) << 1) | digitalRead(dipPin2); // Read DIP switch value

  if (dipSwitchValue == 0b01) { 
    // Mode 01: Follow IR remote
    digitalWrite(blueLedPin, HIGH);  // Blue LED ON
    digitalWrite(whiteLedPin, LOW);  // White LED OFF
    handleIRRemote(); // Handle IR commands
  }
  else if (dipSwitchValue == 0b10) { 
    // Mode 10: Follow Bluetooth module
    digitalWrite(blueLedPin, LOW);   // Blue LED OFF
    digitalWrite(whiteLedPin, HIGH); // White LED ON
    handleBluetooth();  // Handle Bluetooth commands
  }
}

void handleIRRemote() {
  if (irrecv.decode()) {
    auto value = irrecv.decodedIRData.decodedRawData;

    switch (value) {
      case BUTTON_ON: // Button 1
        turnRelayOn();
        break;

      case BUTTON_OFF: // Button 2
        turnRelayOff();
        break;

      case BUTTON_RUN_LEDS: // Button 3
        runLEDs();
        break;

      case BUTTON_TURN_OFF_LEDS: // Button 4
        turnOffLEDs();
        break;

      default:
        Serial.println(value); // Print unrecognized button value
    }
    irrecv.resume(); // Receive the next value
  }
}

void handleBluetooth() {
  if (bluetooth.available() > 0) {
    bluetoothCommand = bluetooth.read(); // Read Bluetooth command

    switch (bluetoothCommand) {
      case 'A':
        turnRelayOn();
        break;

      case 'B':
        turnRelayOff();
        break;

      case 'C':
        runLEDs();
        break;

      case 'D':
        // Do nothing for 'D'
        break;

      default:
        Serial.println("Unknown command");
    }
  }
}

void runLEDs() {
  for (int i = 0; i < numLEDs; ++i) {
    digitalWrite(ledPins[i], HIGH);
    delay(200); // Adjust the delay as needed
    digitalWrite(ledPins[i], LOW);
  }
}

void turnOffLEDs() {
  for (int i = 0; i < numLEDs; ++i) {
    digitalWrite(ledPins[i], LOW);
  }
}

void turnRelayOn() {
  digitalWrite(relayPin, HIGH); // Turn relay on
}

void turnRelayOff() {
  digitalWrite(relayPin, LOW); // Turn relay off
}

