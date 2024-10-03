#include <Servo.h> // Includes the Servo library

// Defines Trigger and Echo pins of the Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 10;

// Variables for the duration and the distance
long duration;
int distance;
Servo myServo; // Creates a servo object for controlling the servo motor

int currentAngle = 15; // Start angle
int targetAngle = 165; // Target angle
int increment = 1; // Increment to move the servo

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(11); // Defines on which pin the servo motor is attached
  myServo.write(currentAngle); // Set initial position of the servo
}

void loop() {
  // Move the servo continuously
  myServo.write(currentAngle); // Move the servo to the current angle
  
  // Allow some time for the servo to reach its position
  delay(15); // Adjust this delay for the desired speed of scanning

  // Calculate distance
  distance = calculateDistance();

  // Send the current degree and distance to the Serial Port
  Serial.print(currentAngle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");

  // Increment the angle
  currentAngle += increment;

  // Reverse direction at limits
  if (currentAngle >= 165) {
    increment = -1; // Change direction to decrease angle
  } 
  else if (currentAngle <= 15) {
    increment = 1; // Change direction to increase angle
  }
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  int distance = duration * 0.034 / 2; // Calculate the distance
  return distance;
}
