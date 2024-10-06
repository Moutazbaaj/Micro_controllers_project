#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900
#define TS_MINY 70
#define TS_MAXY 920

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Pin assignments for LEDs
const int ledYellow = 10;       // Red LED connected to pin 2
const int ledRed = 11;    // Yellow LED connected to pin 3
const int ledGreen = 12;     // Green LED connected to pin 4
const int ledBlue = 13;      // Blue LED connected to pin 5
const int ledW = 8;         // White LED connected to pin 8

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 40

// Declare startX as a global variable
int startX;

// Variables to track LED states and last touch time
bool ledRedState = false;
bool ledGreenState = false;
bool ledBlueState = false;
bool ledYellowState = false;

// Track the last time a button was pressed
unsigned long lastTouchTime = 0;
const int debounceDelay = 300; // Debounce delay in milliseconds

void setup(void) {
    Serial.begin(9600);
    Serial.println(F("Button Example!"));
  
    tft.reset();
  
    uint16_t identifier = tft.readID();
    if(identifier == 0x9341) {
        Serial.println(F("Found ILI9341 LCD driver"));
    } else {
        Serial.println(F("Unknown LCD driver chip"));
        identifier = 0x9341; // Default to ILI9341
    }

    tft.begin(identifier);
    tft.setRotation(1); // Set rotation to landscape
    tft.fillScreen(WHITE);
  
    // Calculate starting position for centering buttons
    startX = (tft.width() - (2 * BUTTON_WIDTH + 20)) / 2; // Centering with space between buttons
    int topY = 50; // Y position for top buttons
    int bottomY = topY + BUTTON_HEIGHT + 30; // Y position for middle buttons
  
    // Draw two buttons on top
    drawButton(startX, topY, BUTTON_WIDTH, BUTTON_HEIGHT, YELLOW, "1 Yellow");
    drawButton(startX + BUTTON_WIDTH + 10, topY, BUTTON_WIDTH, BUTTON_HEIGHT, BLUE, "2 Blue");

    // Draw two buttons below
    drawButton(startX, bottomY, BUTTON_WIDTH, BUTTON_HEIGHT, GREEN, "3 Green");
    drawButton(startX + BUTTON_WIDTH + 10, bottomY, BUTTON_WIDTH, BUTTON_HEIGHT, RED, "4 Red");
  
    // Set LED pins as outputs
    pinMode(ledRed, OUTPUT);  
    pinMode(ledYellow, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledW, OUTPUT);    // Set white LED pin as output

    // Initially turn off all LEDs
    turnOffAllLeds();
}

void drawButton(int x, int y, int w, int h, uint16_t color, const char* label) {
    int radius = 20; // Set the radius for the rounded corners
    tft.fillRoundRect(x, y, w, h, radius, color); // Draw button with rounded corners
    tft.drawRoundRect(x, y, w, h, radius, BLACK); // Draw button border with rounded corners

    // Move text position below the button
    tft.setTextColor(BLACK); // Set text color to black to contrast with white background
    tft.setCursor(x + 10, y + h + 10); // Adjust cursor position for the label below the button
    tft.print(label); // Print the button label
}



#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop() {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        // Scale the touch coordinates
        p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
        p.y = tft.height() - map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
        
        unsigned long currentTime = millis(); // Get the current time

        // Check button presses
        if (currentTime - lastTouchTime > debounceDelay) { // Check for debounce
            lastTouchTime = currentTime; // Update last touch time

            if (p.y >= 50 && p.y <= 50 + BUTTON_HEIGHT) { // Row for top buttons
                if (p.x >= startX && p.x <= startX + BUTTON_WIDTH) {
                    ledBlueState = !ledBlueState;  // Toggle Blue LED state
                    digitalWrite(ledBlue, ledBlueState ? HIGH : LOW); // Set LED state
                    Serial.println(ledBlueState ? "Blue LED On" : "Blue LED Off");
                   // turnOffOtherLeds(ledBlue);
                } else if (p.x >= startX + BUTTON_WIDTH + 10 && p.x <= startX + (BUTTON_WIDTH + 10) * 2) {
                    ledRedState = !ledRedState;  // Toggle Red LED state
                    digitalWrite(ledRed, ledRedState ? HIGH : LOW); // Set LED state
                    Serial.println(ledRedState ? "Red LED On" : "Red LED Off");
                    //turnOffOtherLeds(ledRed);
                }
            } else if (p.y >= (50 + BUTTON_HEIGHT + 30) && p.y <= (50 + BUTTON_HEIGHT + 30 + BUTTON_HEIGHT)) { // Row for middle buttons
                if (p.x >= startX && p.x <= startX + BUTTON_WIDTH) {
                    ledYellowState = !ledYellowState;  // Toggle Yellow LED state
                    digitalWrite(ledYellow, ledYellowState ? HIGH : LOW); // Set LED state
                    Serial.println(ledYellowState ? "Yellow LED On" : "Yellow LED Off");
                    //turnOffOtherLeds(ledYellow);
                } else if (p.x >= startX + BUTTON_WIDTH + 10 && p.x <= startX + (BUTTON_WIDTH + 10) * 2) {
                    ledGreenState = !ledGreenState;  // Toggle Green LED state
                    digitalWrite(ledGreen, ledGreenState ? HIGH : LOW); // Set LED state
                    Serial.println(ledGreenState ? "Green LED On" : "Green LED Off");
                   // turnOffOtherLeds(ledGreen);
                }
            }
        }
    }
}
/*
// Function to turn off all LEDs except the one that is turned on
void turnOffOtherLeds(int activeLed) {
    if (activeLed != ledRed) digitalWrite(ledRed, LOW);
    if (activeLed != ledYellow) digitalWrite(ledYellow, LOW);
    if (activeLed != ledGreen) digitalWrite(ledGreen, LOW);
    if (activeLed != ledBlue) digitalWrite(ledBlue, LOW);
}
*/
// Function to turn off all LEDs
void turnOffAllLeds() {
    digitalWrite(ledRed, LOW);     // Turn off Red LED
    digitalWrite(ledYellow, LOW);  // Turn off Yellow LED
    digitalWrite(ledGreen, LOW);   // Turn off Green LED
    digitalWrite(ledBlue, LOW);    // Turn off Blue LED
}
