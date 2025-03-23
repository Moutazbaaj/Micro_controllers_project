#include "SSD1306Wire.h"

#define BATTERY_ADC_PIN 34
SSD1306Wire display(0x3C, 21, 22);

// Resistor values (in ohms)
const float R1 = 10000.0; // 10k
const float R2 = 20000.0; // Two 10k resistors in series

// ADC reference voltage (ESP32 ADC range is 0 - 3.3V)
const float ADC_REF_VOLTAGE = 3.3;

// ADC resolution (12-bit ADC: 0 - 4095)
const int ADC_MAX_VALUE = 4095;

// Battery voltage calculation
float getBatteryVoltage() {
  int rawADC = analogRead(BATTERY_ADC_PIN);
  float voltageAtPin = (rawADC * ADC_REF_VOLTAGE) / ADC_MAX_VALUE;
  float batteryVoltage = voltageAtPin * ((R1 + R2) / R2);
  return batteryVoltage;
}

// Convert battery voltage to percentage
int getBatteryPercentage(float voltage) {
  if (voltage >= 4.2) return 100;
  else if (voltage >= 4.0) return map(voltage * 100, 400, 420, 80, 100);
  else if (voltage >= 3.8) return map(voltage * 100, 380, 400, 60, 80);
  else if (voltage >= 3.6) return map(voltage * 100, 360, 380, 40, 60);
  else if (voltage >= 3.4) return map(voltage * 100, 340, 360, 20, 40);
  else if (voltage >= 3.2) return map(voltage * 100, 320, 340, 0, 20);
  else return 0; // Below 3.2V is considered 0%
}

// Draw battery icon with percentage
void drawBatteryIcon(int percentage) {
  // Icon position and size
  int x = display.getWidth() - 20; // Right corner 40
  int y = 0;
  int width = 28; //30
  int height = 10; //10

  // Draw battery outline
  display.drawRect(x, y, width, height);

  // Draw battery terminal
  display.fillRect(x + width, y + 3, 2, 6);

  // Calculate fill width based on percentage
  int fillWidth = map(percentage, 0, 100, 0, width - 2);

  // Draw fill
  display.fillRect(x + 1, y + 1, fillWidth, height - 2);

  // Show percentage
  String percentageText = String(percentage) + "%";
  display.drawString(x - display.getStringWidth(percentageText) - 5, y + 2, percentageText);
}

void setup() {
  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  analogReadResolution(12); // Set ADC resolution to 12-bit
  pinMode(BATTERY_ADC_PIN, INPUT);
}

void loop() {
  float batteryVoltage = getBatteryVoltage();
  int batteryPercentage = getBatteryPercentage(batteryVoltage);

  // Clear the display
  display.clear();

  // Display battery icon and percentage
  drawBatteryIcon(batteryPercentage);

  // Display other information (optional)
  display.drawString(0, 0, "Battery Info:");
  display.drawString(0, 20, "Voltage: " + String(batteryVoltage, 2) + "V");
  display.drawString(0, 35, "Level: " + String(batteryPercentage) + "%");

  display.display();

  // Print battery info to Serial Monitor
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // 2 decimal places
  Serial.print(" V | Battery Level: ");
  Serial.print(batteryPercentage);
  Serial.println("%");

  delay(2000); // Update every 2 seconds
}