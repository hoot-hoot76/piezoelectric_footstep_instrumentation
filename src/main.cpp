#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int voltagePin = A0;

const int stepPin = A1;

const float capacitor = 0.000010;

const float R1 = 100000.0;
const float R2 = 10000.0;
int rawValue = analogRead(voltagePin);

const float VREF = 5.0;

int steps = 0;
int previousStepValue = 0;

unsigned long lastStepTime = 0;
const unsigned long stepDelay = 500;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("STARTING");
  lcd.setCursor(0, 1);
  lcd.print("UP...");
  delay(2000);
  pinMode(voltagePin, INPUT);
  pinMode(stepPin, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Footstep Energy");
  lcd.setCursor(0, 1);
  lcd.print("Generator");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {

  int rawValue = analogRead(voltagePin);

  float dividerVoltage = rawValue * VREF / 1023.0;

  float capacitorVoltage =
    dividerVoltage * (R1 + R2) / R2;

  
  float energy = 0.5 * capacitor *
                 capacitorVoltage *
                 capacitorVoltage;

  
  float energy_mJ = energy * 1000.0;

  int stepValue = analogRead(stepPin);

  if (stepValue > 100 &&
      previousStepValue <= 100 &&
      millis() - lastStepTime > stepDelay) {

      steps++;
      lastStepTime = millis();
  }

  previousStepValue = stepValue;
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(capacitorVoltage, 2);
  lcd.print(" E:");
  lcd.print(energy_mJ, 1);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Steps: ");
  lcd.print(steps);
  lcd.print("        "); 

  Serial.print("Piezo: ");
  Serial.print(stepValue);
  Serial.print(" | Voltage: ");
  Serial.print(capacitorVoltage, 2);
  Serial.print(" V | Energy: ");
  Serial.print(energy_mJ, 2);
  Serial.print(" mJ | Steps: ");
  Serial.println(steps);
  Serial.print("RAW A0: ");
  Serial.println(rawValue); 
  delay(100);
}
