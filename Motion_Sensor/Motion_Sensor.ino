// Define pins
int pirPin = 2;  // PIR sensor output pin
int buzzerPin = 8;  // Buzzer pin
int blueLedPin = 9;  // Blue LED pin
int redLedPin = 10;  // Red LED pin
int durationPotPin = A0;
int volumePotPin = A1;

void setup() {
  pinMode(pirPin, INPUT);  // Set PIR pin as input
  pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output
  pinMode(blueLedPin, OUTPUT);  // Set blue LED pin as output
  pinMode(redLedPin, OUTPUT);  // Set red LED pin as output
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  int pirState = digitalRead(pirPin);  // Read PIR sensor state

  if (pirState == HIGH) {  // If motion is detected
    Serial.println("Motion detected!");  // Print to serial monitor
    
    // Turn on the red LED and turn off the blue LED
    digitalWrite(redLedPin, HIGH);  // Red LED on
    digitalWrite(blueLedPin, LOW);  // Blue LED off

    int durationPotValue = analogRead(durationPotPin);
    int volumePotValue = analogRead(volumePotPin);


    int durationOn = map(durationPotValue, 0, 1023, 50, 500);
    int volume = map(volumePotValue, 0, 1023, 50, 255);

    // Make the buzzer beep like an alarm
    for (int i = 0; i < 1; i++) {
      analogWrite(buzzerPin, volume);  // Turn on the buzzer
      delay(durationOn);  // Wait 200 milliseconds
      digitalWrite(buzzerPin, 0);  // Turn off the buzzer
      delay(durationOn);
    }
    
  } else {
    digitalWrite(buzzerPin, LOW);  // Ensure the buzzer is off when no motion is detected
    Serial.println("No motion");

    // Turn on the blue LED and turn off the red LED
    digitalWrite(blueLedPin, HIGH);  // Blue LED on
    digitalWrite(redLedPin, LOW);  // Red LED off
  }

  delay(300);  // Small delay for stability
}
