#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned count = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Startup");
  lcd.setCursor(0, 1);
  lcd.print("Successful!");
  delay(2000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print(count);
  count++;
  delay(500);
}