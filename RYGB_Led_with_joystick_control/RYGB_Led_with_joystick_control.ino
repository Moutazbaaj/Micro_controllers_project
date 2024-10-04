const int ledRed = 2;
const int ledYellow = 3;
const int ledGreen = 4;
const int ledBlue = 5;

const int joyX = A0;
const int joyY = A1;

int joyR = 6;

bool randomMode = false;
unsigned long startTime; 



void setup() {

pinMode(ledRed, OUTPUT);  
pinMode(ledYellow, OUTPUT);
pinMode(ledGreen, OUTPUT);
pinMode(ledBlue, OUTPUT);
pinMode(joyR, INPUT_PULLUP);

Serial.begin(9600);


digitalWrite(ledRed, LOW);
digitalWrite(ledYellow, LOW);
digitalWrite(ledGreen, LOW);
digitalWrite(ledBlue, LOW);

}

void loop() {

int xValue = analogRead(joyX);
int yValue = analogRead(joyY);
int buttonState = digitalRead(joyR);

if (buttonState == LOW && !randomMode) {
  randomMode = true;
  startTime = millis();
   Serial.println("RandomMode On");
}

if (randomMode) {
  if (millis() - startTime < 5000) {
    randomBlink();
  } else {
    randomMode = false;

   digitalWrite(ledRed, LOW);
   digitalWrite(ledYellow, LOW);
   digitalWrite(ledGreen, LOW);
   digitalWrite(ledBlue, LOW);

  }
  return;
}

if (xValue < 300) {
  digitalWrite(ledRed, HIGH);
  Serial.println("Red Light On");
} else {
  digitalWrite(ledRed, LOW);

}

if (xValue > 700) {
  digitalWrite(ledYellow, HIGH);
  Serial.println("Yellow Light On");
} else {
  digitalWrite(ledYellow, LOW);

}

if (yValue < 300) {
  digitalWrite(ledGreen, HIGH);
  Serial.println("Green Light On");
} else {
  digitalWrite(ledGreen, LOW);

}

if (yValue > 700) {
  digitalWrite(ledBlue, HIGH);
  Serial.println("Blue Light On");
} else {
  digitalWrite(ledBlue, LOW);

}

delay(100);
}

void randomBlink() {
  // Generate random numbers (0 or 1) for each LED
  int redState = random(0, 2);    // Randomly turn Red LED on/off
  int yellowState = random(0, 2); // Randomly turn Yellow LED on/off
  int greenState = random(0, 2);  // Randomly turn Green LED on/off
  int blueState = random(0, 2);   // Randomly turn Blue LED on/off

  // Set the LEDs to random states
  digitalWrite(ledRed, redState);
  digitalWrite(ledYellow, yellowState);
  digitalWrite(ledGreen, greenState);
  digitalWrite(ledBlue, blueState);

  delay(200); // Small delay for the blinking effect
}