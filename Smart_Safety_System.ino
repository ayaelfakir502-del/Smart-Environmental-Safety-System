#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configuration for I2C LCD (20x4 characters)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// DHT11 Sensor Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin Definitions for RGB LED, Buzzer, Gas Sensor, and Button
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
const int buzzer = 8;
const int gasPin = A0;
const int buttonPin = 3;

// Global Variables for Status Tracking
int pStatus = 0;      // Tracks the acknowledgment (Mute) state
int hazardLevel = 0;  // 0: Secure, 1: Humidity, 2: Gas
bool btnState = HIGH;
bool lastBtnState = HIGH;

void setup() {
  // Initialize peripherals and start DHT sensor
  lcd.init();
  lcd.backlight();
  dht.begin();

  // Configure I/O pins
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up for the button
}

// Function to set LED color for Normal state (Secure)
void setNormal() {
  digitalWrite(redPin, HIGH); digitalWrite(greenPin, LOW); digitalWrite(bluePin, HIGH);
}

// Function to set LED color for Humidity Alert
void setHumidAlert() {
  digitalWrite(redPin, HIGH); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, LOW);
}

// Function to set LED color for Gas Hazard (Danger)
void setGasHazard() {
  digitalWrite(redPin, LOW); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, HIGH);
}

void loop() {
  // Read and map Gas sensor values to a scale of 0-1000
  int gasVal = map(analogRead(gasPin), 0, 1023, 0, 1000); 
  int h = (int)dht.readHumidity();    // Read Humidity
  int t = (int)dht.readTemperature(); // Read Temperature

  // Determine Hazard Level based on sensor thresholds
  if (gasVal > 700) {
    hazardLevel = 2; // Critical Gas Alert
  } else if (h > 70) {
    hazardLevel = 1; // High Humidity Warning
  } else {
    hazardLevel = 0; // Everything is Secure
    pStatus = 0;     // Reset mute status when secure
  }

  // Handle Button Input for Acknowledging (Muting) the Alarm
  btnState = digitalRead(buttonPin);
  if (btnState == LOW && lastBtnState == HIGH) { // Detect Button Press (Falling Edge)
    if (hazardLevel > 0 && pStatus < hazardLevel) {
      pStatus++; // Increment pStatus to mute the current alert level
    }
    delay(200); // Simple Debouncing
  }
  lastBtnState = btnState;

  // Update Information on LCD
  updateDisplay(t, h, gasVal);
  
  // --- Emergency Audio/Visual Logic ---
  if (hazardLevel == 2) { 
    setGasHazard();
    if (pStatus < 2) { // Play siren only if not acknowledged for Gas
      tone(buzzer, 1000); delay(150);
      tone(buzzer, 600);  delay(150);
    } else {
      noTone(buzzer); // Alarm muted by user
    }
  } 
  else if (hazardLevel == 1) { 
    setHumidAlert();
    if (pStatus < 1) { // Play short beeps only if not acknowledged for Humidity
      tone(buzzer, 500); delay(100);
      noTone(buzzer);    delay(100);
    } else {
      noTone(buzzer); // Alarm muted by user
    }
  } 
  else {
    setNormal();    // Normal operation
    noTone(buzzer);
  }

  delay(50); // Small delay for system stability
}

// Function to format and output data to the 20x4 LCD
void updateDisplay(int t, int h, int g) {
  lcd.setCursor(0, 0);
  lcd.print("Temp: "); lcd.print(t); lcd.write(223); lcd.print("C    ");
  lcd.setCursor(0, 1);
  lcd.print("Humid: "); lcd.print(h); lcd.print("%   ");
  lcd.setCursor(0, 2);
  lcd.print("Gas: "); lcd.print(g); lcd.print("     ");
  
  lcd.setCursor(15, 0);
  lcd.print("P:"); lcd.print(pStatus); 

  // Status message logic for clear visual feedback
  lcd.setCursor(0, 3);
  lcd.print("Status: ");
  if (hazardLevel == 0) lcd.print("SECURE      ");
  else if (hazardLevel == 1 && pStatus >= 1) lcd.print("HUMID MUTED ");
  else if (hazardLevel == 1) lcd.print("HUMID ALERT ");
  else if (hazardLevel == 2 && pStatus >= 2) lcd.print("GAS MUTED   ");
  else if (hazardLevel == 2) lcd.print("GAS HAZARD  ");
}
