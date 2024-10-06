// Pin assignments for LEDs
const int ledRed = 2;       // Red LED connected to pin 2
const int ledYellow = 3;    // Yellow LED connected to pin 3
const int ledGreen = 4;     // Green LED connected to pin 4
const int ledBlue = 5;      // Blue LED connected to pin 5
const int ledW = 8;         // White LED connected to pin 8

// Joystick pin assignments
const int joyX = A0;        // X-axis of the joystick connected to analog pin A0
const int joyY = A1;        // Y-axis of the joystick connected to analog pin A1

int joyR = 6;               // Joystick button (click) connected to pin 6

// Button pin assignment
const int buttonPin = 7;    // Button to toggle LEDs connected to pin 7

// Global variables to manage the states
bool randomMode = false;    // Variable to manage random LED blinking mode
bool lightsOn = false;      // Variable to indicate if the main lights (Red, Yellow, Green, Blue) are on or off
bool ledWOn = false;        // Variable to track the state of the white LED (on or off)
unsigned long startTime;    // Variable to store the time when random mode started

void setup() {
  // Set LED pins as outputs
  pinMode(ledRed, OUTPUT);  
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledW, OUTPUT);    // Set white LED pin as output

  // Set joystick button and toggle button as inputs with pull-up resistors
  pinMode(joyR, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP); 

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initially turn off all LEDs
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledW, LOW);  // White LED is off initially
}

void loop() {
  // Read joystick X and Y positions
  int xValue = analogRead(joyX);  // X-axis reading
  int yValue = analogRead(joyY);  // Y-axis reading
  
  // Read joystick button state
  int buttonState = digitalRead(joyR);

  // Read the state of the button used for toggling lights on/off
  int buttonToggleState = digitalRead(buttonPin);

  // Track the last state of the toggle button to detect button press transitions
  static bool lastButtonState = HIGH; 
  
  // Toggle the white LED (ledW) when the button is pressed
  if (buttonToggleState == LOW && lastButtonState == HIGH) {
    ledWOn = !ledWOn;  // Toggle the state of the white LED
    digitalWrite(ledW, ledWOn ? HIGH : LOW);  // Turn white LED on or off based on the state
    Serial.println(ledWOn ? "W On" : "W Off");  // Log white LED state to Serial Monitor
    delay(200);  // Debounce delay to prevent multiple triggers
  }

  // Toggle the main lights (Red, Yellow, Green, Blue) when the button is pressed
  if (buttonToggleState == LOW && lastButtonState == HIGH) {
    lightsOn = !lightsOn;  // Toggle the state of the main LEDs
    Serial.println(lightsOn ? "Lights On" : "Lights Off");  // Log the main lights state to Serial Monitor
    delay(200);  // Debounce delay to prevent multiple triggers
  }
  
  // Update the last button state to the current one for detecting press transitions
  lastButtonState = buttonToggleState; 

  // Handle random mode
  if (buttonState == LOW && !randomMode && lightsOn) {  // Check if lights are on and random mode is not active
    randomMode = true;          // Activate random mode
    startTime = millis();       // Record the start time of random mode
    Serial.println("RandomMode On");  // Log to Serial Monitor
  }

  // If random mode is active
  if (randomMode) {
    if (millis() - startTime < 5000) {  // If less than 5 seconds have passed
      randomBlink();  // Call the function to randomly blink LEDs
    } else {
      randomMode = false;  // Deactivate random mode after 5 seconds
      turnOffAllLeds();    // Ensure all LEDs are off
    }
    return;  // Exit the loop to prevent further code execution until random mode is done
  }

  // Only control individual LEDs based on joystick position if the lights are on
  if (lightsOn) {
    // Control Red LED based on X-axis joystick position
    if (xValue < 300) {
      digitalWrite(ledRed, HIGH);  // Turn on Red LED if joystick is moved left
      Serial.println("Red Light On");
    } else {
      digitalWrite(ledRed, LOW);   // Turn off Red LED
    }

    // Control Yellow LED based on X-axis joystick position
    if (xValue > 700) {
      digitalWrite(ledYellow, HIGH);  // Turn on Yellow LED if joystick is moved right
      Serial.println("Yellow Light On");
    } else {
      digitalWrite(ledYellow, LOW);   // Turn off Yellow LED
    }

    // Control Green LED based on Y-axis joystick position
    if (yValue < 300) {
      digitalWrite(ledGreen, HIGH);  // Turn on Green LED if joystick is moved up
      Serial.println("Green Light On");
    } else {
      digitalWrite(ledGreen, LOW);   // Turn off Green LED
    }

    // Control Blue LED based on Y-axis joystick position
    if (yValue > 700) {
      digitalWrite(ledBlue, HIGH);  // Turn on Blue LED if joystick is moved down
      Serial.println("Blue Light On");
    } else {
      digitalWrite(ledBlue, LOW);   // Turn off Blue LED
    }
  }

  delay(100);  // Short delay to prevent rapid flickering of LEDs
}

// Function to randomly blink the LEDs
void randomBlink() {
  int redState = random(0, 2);    // Randomly choose on/off state for Red LED
  int yellowState = random(0, 2); // Randomly choose on/off state for Yellow LED
  int greenState = random(0, 2);  // Randomly choose on/off state for Green LED
  int blueState = random(0, 2);   // Randomly choose on/off state for Blue LED

  digitalWrite(ledRed, redState);     // Set the Red LED to random state
  digitalWrite(ledYellow, yellowState);  // Set the Yellow LED to random state
  digitalWrite(ledGreen, greenState);    // Set the Green LED to random state
  digitalWrite(ledBlue, blueState);      // Set the Blue LED to random state

  delay(200);  // Small delay between blinks
}

// Function to turn off all LEDs
void turnOffAllLeds() {
  digitalWrite(ledRed, LOW);     // Turn off Red LED
  digitalWrite(ledYellow, LOW);  // Turn off Yellow LED
  digitalWrite(ledGreen, LOW);   // Turn off Green LED
  digitalWrite(ledBlue, LOW);    // Turn off Blue LED
}