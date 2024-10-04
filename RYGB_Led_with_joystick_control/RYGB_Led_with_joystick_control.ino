const int ledRed = 2;
const int ledYellow = 3;
const int ledGreen = 4;
const int ledBlue = 5;

const int joyX = A0;
const int joyY = A1;

int joyR = 6;

const int buttonPin = 7; 

bool randomMode = false;
bool lightsOn = false; 
unsigned long startTime; 

void setup() {
  pinMode(ledRed, OUTPUT);  
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(joyR, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP); 

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
  int buttonToggleState = digitalRead(buttonPin);

  static bool lastButtonState = HIGH; 
  if (buttonToggleState == LOW && lastButtonState == HIGH) {
    lightsOn = !lightsOn; 
    Serial.println(lightsOn ? "Lights On" : "Lights Off");
    delay(200); 
  }
  lastButtonState = buttonToggleState; 

  // Handle random mode
  if (buttonState == LOW && !randomMode && lightsOn) { // Check if lights are on
    randomMode = true;
    startTime = millis();
    Serial.println("RandomMode On");
  }

  if (randomMode) {
    if (millis() - startTime < 5000) {
      randomBlink();
    } else {
      randomMode = false;
      turnOffAllLeds();
    }
    return;
  }

  // Only control LEDs if lights are on
  if (lightsOn) {
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
  }

  delay(100);
}

void randomBlink() {
  int redState = random(0, 2);
  int yellowState = random(0, 2);
  int greenState = random(0, 2);
  int blueState = random(0, 2);

  digitalWrite(ledRed, redState);
  digitalWrite(ledYellow, yellowState);
  digitalWrite(ledGreen, greenState);
  digitalWrite(ledBlue, blueState);

  delay(200);
}

void turnOffAllLeds() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
}
