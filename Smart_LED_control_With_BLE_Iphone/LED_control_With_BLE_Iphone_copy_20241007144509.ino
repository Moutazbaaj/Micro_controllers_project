#include <SoftwareSerial.h>

// Pin assignments for LEDs
const int ledRed = 2;
const int ledYellow = 3;
const int ledGreen = 4;
const int ledBlue = 5;
const int ledW = 8;

// Define HM-19 Bluetooth pins
const int rxPin = 10;
const int txPin = 11;

SoftwareSerial btSerial(rxPin, txPin);  // Initialize software serial for HM-19

void setup() {
  // Set LED pins as outputs
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledW, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
  btSerial.begin(9600);    // Initialize Bluetooth communication
  Serial.println("Waiting for Bluetooth connection...");

  // Initially turn off all LEDs
  turnOffAllLeds();
}

void loop() {
  // Bluetooth control
  if (btSerial.available()) {
    char command = btSerial.read();

    switch (command) {
      case '1':
        turnOnAllLeds();
        Serial.println("All Lights On");
        break;
      case '0':
        turnOffAllLeds();
        Serial.println("All Lights Off");
        break;
      case 'r':
        toggleLed(ledRed);
        Serial.println("Red LED toggled");
        break;
      case 'g':
        toggleLed(ledGreen);
        Serial.println("Green LED toggled");
        break;
      case 'b':
        toggleLed(ledBlue);
        Serial.println("Blue LED toggled");
        break;
      case 'y':
        toggleLed(ledYellow);
        Serial.println("Yellow LED toggled");
        break;
      case 'w':
        toggleLed(ledW);
        Serial.println("White LED toggled");
        break;
    }
  }
  delay(100);
}

// Function to turn on all LEDs
void turnOnAllLeds() {
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledW, HIGH);
}

// Function to turn off all LEDs
void turnOffAllLeds() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledW, LOW);
}

// Function to toggle a specific LED
void toggleLed(int ledPin) {
  digitalWrite(ledPin, !digitalRead(ledPin));
}
