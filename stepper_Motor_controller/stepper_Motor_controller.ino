#include <AccelStepper.h>

// Define motor pins (GPIO5, GPIO4, GPIO0, GPIO2)
AccelStepper stepper(AccelStepper::FULL4WIRE, 5, 4, 0, 2);

// Joystick pin
const int joyY = A0;  // Y-axis for speed

// Direction buttons
const int leftButton = 14;  // GPIO14 (D5)
const int rightButton = 12; // GPIO12 (D6)

// Dead zone to avoid accidental movement
const int deadZone = 100;

void setup() {
  // Set up buttons as inputs
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  // Initialize motor
  stepper.setMaxSpeed(1000);  // Max RPM (adjust for your motor)
}

void loop() {
  // Read Y-axis for speed (0–1023)
  int yValue = analogRead(joyY);

  // Map Y-axis to speed (up = fast, down = slow)
  int speed = map(yValue, 0, 1023, 1000, 0);

  // Check direction buttons
  if (digitalRead(leftButton) == LOW) {  // LEFT → CCW
    speed = -speed;  // Negative speed = reverse
  } else if (digitalRead(rightButton) == LOW) {  // RIGHT → CW
    speed = speed;  // Positive speed = forward
  } else {  // No button pressed → STOP
    speed = 0;
  }

  // Update motor speed
  stepper.setSpeed(speed);
  stepper.runSpeed();
}