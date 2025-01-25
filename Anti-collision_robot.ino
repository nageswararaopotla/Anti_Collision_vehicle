#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define lcd_address 0x27 // Update this with the correct I2C address from the scanner sketch
#define led_pin 2 // D0 in ESP8266
#define trig_pin 14 // D5 in ESP8266
#define echo_pin 12 // D6 in ESP8266

#define US_ERROR -1
#define US_NO_OBSTACLE -2

LiquidCrystal_I2C lcd(lcd_address, 16, 2); // Initialize LCD object with 16x2 dimensions

int distance;

void setup() {
  Serial.begin(115200);

  pinMode(led_pin, OUTPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight

  // Clear the LCD at the beginning
  lcd.clear();
}

void loop() {
  distance = measureDistance();

  // Print distance to Serial Monitor for debugging
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= 100 && distance != US_ERROR) {
    digitalWrite(led_pin, HIGH); // Turn on LED
    displayAlert(); // Display "ALERT" when distance is within the range
  } else if (distance > 60 && distance != US_ERROR) {
    digitalWrite(led_pin, LOW); // Turn off LED
    displayParkingMessage(); // Display "You can park easily" when distance is greater than 60 cm
  } else {
    digitalWrite(led_pin, LOW); // Turn off LED
    clearLCD(); // Clear the LCD when LED is off
  }

  delay(100);
}

void clearLCD() {
  lcd.clear(); // Clear the LCD
}

int measureDistance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  unsigned long duration = pulseIn(echo_pin, HIGH, 30000); // timeout after 30ms

  if (duration == 0) return US_ERROR; // timeout

  int distance_cm = duration * 0.034 / 2;

  if (distance_cm > 400 || distance_cm <= 0) return US_NO_OBSTACLE; // out of range

  return distance_cm;
}

void displayAlert() {
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to first line
  lcd.print("ALERT"); // Display "ALERT" on the LCD

  // Print to Serial Monitor for debugging
  Serial.println("ALERT");
}

void displayParkingMessage() {
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to first line
  lcd.print("You can park"); // Display "You can park" on the LCD
  lcd.setCursor(0, 1); // Set cursor to second line
  lcd.print("easily"); // Display "easily" on the LCD

  // Print to Serial Monitor for debugging
  Serial.println("You can park easily");
}
