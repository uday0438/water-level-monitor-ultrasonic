#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
 * Real-Time Water Level Monitoring System
 * ---------------------------------------
 * Hardware:
 * - Arduino Uno
 * - HC-SR04 Ultrasonic Sensor
 * - I2C LCD 16x2 (Address 0x27 or 0x3F)
 * - Buzzer
 * 
 * Connections:
 * - Trig: D9, Echo: D10
 * - Buzzer: D8
 * - LCD: SDA(A4), SCL(A5)
 */

// Initialize LCD (Address, Columns, Rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_PIN 8

// Configuration Parameters
const int tankHeight = 100;   // Maximum tank height in cm
const int threshold = 80;     // Alert threshold in percentage (%)
const int offset = 2;         // Calibration offset in cm

// --- Ultrasonic Sensor Functions ---

/**
 * Gets a single raw distance reading from the ultrasonic sensor.
 * @return distance in cm, or -1 if timeout occurred.
 */
long getDistanceRaw() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // pulseIn returns duration in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout

  if (duration == 0) return -1; // No echo received

  // distance = (time * speed of sound) / 2
  // speed of sound = 0.034 cm/us
  long distance = (duration * 0.034) / 2;
  return distance;
}

/**
 * Gets a filtered distance reading by averaging multiple samples.
 * @return averaged distance in cm, or -1 if all readings failed.
 */
long getDistanceFiltered() {
  long readings[5];
  int validCount = 0;

  for (int i = 0; i < 5; i++) {
    long d = getDistanceRaw();
    if (d > 2 && d < 400) { // Valid range for HC-SR04
      readings[validCount++] = d;
    }
    delay(50);
  }

  if (validCount == 0) return -1;

  long sum = 0;
  for (int i = 0; i < validCount; i++) {
    sum += readings[i];
  }

  return sum / validCount;
}

// --- Setup ---

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println(F("Water Level Monitor Starting..."));

  // Initialize Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initial Splash Screen
  lcd.setCursor(0, 0);
  lcd.print(F(" WATER MONITOR "));
  lcd.setCursor(0, 1);
  lcd.print(F("   INITIALIZING  "));
  delay(2000);
  lcd.clear();
}

// --- Main Loop ---

void loop() {
  long distance = getDistanceFiltered();

  if (distance == -1) {
    // Sensor error handling
    lcd.setCursor(0, 0);
    lcd.print(F(" SENSOR ERROR!  "));
    lcd.setCursor(0, 1);
    lcd.print(F(" CHECK WIRING   "));
    
    Serial.println(F("❌ Sensor error detected."));
    digitalWrite(BUZZER_PIN, LOW);
    delay(2000);
    return;
  }

  // Apply calibration offset
  distance = distance - offset;
  if (distance < 0) distance = 0;
  if (distance > tankHeight) distance = tankHeight;

  // Calculate Water Level Percentage
  // Formula: level (%) = ((tankHeight - distance) / tankHeight) * 100
  int waterLevel = ((tankHeight - distance) * 100) / tankHeight;

  // Update Display & Serial
  if (waterLevel >= threshold) {
    // Tank Full Alert
    lcd.setCursor(0, 0);
    lcd.print(F("  TANK FULL!    "));
    lcd.setCursor(0, 1);
    lcd.print(F("Level: "));
    lcd.print(waterLevel);
    lcd.print(F("%     "));

    digitalWrite(BUZZER_PIN, HIGH);
    
    Serial.print(F("⚠️ ALERT: Tank Full ("));
    Serial.print(waterLevel);
    Serial.println(F("%)"));
  } else {
    // Normal Monitoring
    digitalWrite(BUZZER_PIN, LOW);

    lcd.setCursor(0, 0);
    lcd.print(F("Dist: "));
    lcd.print(distance);
    lcd.print(F("cm      "));

    lcd.setCursor(0, 1);
    lcd.print(F("Level: "));
    lcd.print(waterLevel);
    lcd.print(F("%      "));

    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.print(F(" cm | Level: "));
    Serial.print(waterLevel);
    Serial.println(F(" %"));
  }

  delay(1000); // 1 second delay between updates
}
