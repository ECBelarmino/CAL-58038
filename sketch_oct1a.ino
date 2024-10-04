#include <IRremote.hpp>

const int rcvPin = 11;

IRrecv irrecv(rcvPin);
decode_results results;

// Define LED pins
const int numLEDs = 8; // Number of LEDs
const int ledPins[] = {1, 2, 3, 4, 5, 6, 7, 8}; // Pins for LEDs

// Define relay pin
const int relayPin = 0;

// IR remote button values
const unsigned long BUTTON_ON = 3877175040; // Button 1 4077715200
const unsigned long BUTTON_OFF = 4077715200; // Button 2 3877175040
const unsigned long BUTTON_RUN_LEDS = 2707357440; // Button 3
const unsigned long BUTTON_TURN_OFF_LEDS = 4144561920; // Button 4

void setup() {
 Serial.begin(9600);
 irrecv.enableIRIn();

 // Set up LED pins
 for (int i = 0; i < numLEDs; ++i) {
  pinMode(ledPins[i], OUTPUT);
 }

 // Set up relay pin
 pinMode(relayPin, OUTPUT);
}

void loop() {
 if (irrecv.decode()) {
  auto value = irrecv.decodedIRData.decodedRawData;

  switch (value) {
   case BUTTON_ON: // Button 1
    turnRelayOn(); // Turn ON the bulb
    break;

   case BUTTON_OFF: // Button 2
    turnRelayOff(); // Turn OFF the bulb
    break;

   case BUTTON_RUN_LEDS: // Button 3
    runLEDs(); // Run the LED sequence
    break;

   case BUTTON_TURN_OFF_LEDS: // Button 4
    turnOffLEDs(); // Turn OFF the LEDs
    break;

   default:
    Serial.println(value); // Print unrecognized button value
  }
  irrecv.resume();
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