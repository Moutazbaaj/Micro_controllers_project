/*
 * SD Card Tester
 * 
 * Copyright (C) 2024 Moutaz Baaj
 * 
 * Licensed under the MIT License.
 * You may obtain a copy of the License at
 * 
 *     https://opensource.org/licenses/MIT
 * 
 * This program is distributed "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * express or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose, and noninfringement.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Author: Moutaz Baaj
 * Date: 2024-12-19
 */



#include <SD.h>
#include <SPI.h>
#include "SSD1306Wire.h"

#define SD_CS_PIN 5
#define SPEAKER_PIN 25

// Button settings
#define BUTTON_PIN_1 12

#define BUTTON_PIN_2 14

static unsigned long lastPress1 = 0;
static unsigned long lastPress2 = 0;
unsigned long debounceDelay = 200;
  
SSD1306Wire display(0x3C, 21, 22);

void beep() {
  tone(SPEAKER_PIN, 1000, 200);
  delay(300);
}

void displayStartup() {

  display.clear();
  const char* text = "SD Tester";
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  display.drawString(x, 0, text);
  display.drawString(22, 22, "Insert an SD Card");
  display.drawString(12, 38, "Hold Button A to Start");
  display.display();
  beep();
}

void displayCenteredText(const String& text) {
  display.clear();
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  int16_t y = (display.getHeight() - 10) / 2;
  display.drawString(x, y, text);
  display.display();
}

void displayMenu() {
  display.clear();
  const char* text = "Menu:";
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  display.drawString(x, 0, text);
  display.drawString(0, 18, "A- Card Info");
  display.drawString(0, 30, "B- New Test");
  display.drawString(0, 48, "SD can be safely removed");
  display.display();
  beep();
}

void displayCardInfo() {
  uint8_t cardType = SD.cardType();
  uint64_t cardSize = SD.cardSize() / (1024 * 1024); 
  uint64_t usedSpace = SD.usedBytes() / (1024 * 1024); 
  uint64_t freeSpace = (SD.totalBytes() - SD.usedBytes()) / (1024 * 1024);

  String cardTypeStr;
  switch (cardType) {
    case CARD_MMC:  cardTypeStr = "MMC"; break;
    case CARD_SD:   cardTypeStr = "SD"; break;
    case CARD_SDHC: cardTypeStr = "SDHC"; break;
    default:        cardTypeStr = "UNKNOWN"; break;
  }

  display.clear();
  const char* text = "Card Info:";
  int16_t x = (display.getWidth() - display.getStringWidth(text)) / 2;
  display.drawString(x, 0, text);
  display.drawString(0, 12, "Type: " + cardTypeStr);
  display.drawString(0, 24, "Size: " + String(cardSize) + " MB");
  display.drawString(0, 36, "Used: " + String(usedSpace) + " MB");
  display.drawString(0, 48, "Free: " + String(freeSpace) + " MB");
  display.display();
  beep();
}

void setup() {
  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  displayStartup();
  beep();

  // Wait for BUTTON_PIN_1 press before continuing
  while (digitalRead(BUTTON_PIN_1) == HIGH) { }

  // Display SD Initialization Message
  displayCenteredText("Initializing SD card...");
  beep();
  delay(2000);

  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    display.clear();
    display.drawString(28, 22, "SD init failed!");
    display.drawString(8, 38, "Hold Button B to Restart");
    display.display();
    beep();
    return;
  }
  Serial.println("SD card initialized.");
  displayCenteredText("SD initialized.");
  beep();
  delay(2000);

  // Run the SD card test
  displayCenteredText("Testing SD card...");
  beep();
  delay(2000);

  // Write and Read Test
  File testFile = SD.open("/SDTester.txt", FILE_WRITE);
  if (testFile) {
    testFile.println("SD card test successful.");
    testFile.close();
    displayCenteredText("Write Test Passed");
    beep();
    delay(3000);

    testFile = SD.open("/test.txt");
    if (testFile) {
      displayCenteredText("Read Test Passed");
      beep();
      delay(3000);
      testFile.close();
    } else {
      display.clear();
      display.drawString(22, 22, "Read Test Failed!");
      display.drawString(8, 38, "Hold Button B to Restart");
      display.display();
      beep();
      return;
    }
    
    displayCenteredText("Test Completed");
    beep();
    delay(3000);

  } else {
    display.clear();
    display.drawString(22, 22, "Write Test Failed!");
    display.drawString(8, 38, "Hold Button B to Restart");
    display.display();
    beep();
    return;
  }

  // Display SD Card Info
  displayMenu();
  beep();
  delay(2000);
}

void loop() {


  if (digitalRead(BUTTON_PIN_1) == LOW && (millis() - lastPress1) > debounceDelay) {
    lastPress1 = millis();
    displayCardInfo();  // Display SD card information
    delay(100);
  }

  if (digitalRead(BUTTON_PIN_2) == LOW && (millis() - lastPress2) > debounceDelay) {
    lastPress2 = millis();
    Serial.println("Restarting ESP32...");
    displayCenteredText("Restarting...");
    beep();
    delay(1000);
    ESP.restart();
  }
}
