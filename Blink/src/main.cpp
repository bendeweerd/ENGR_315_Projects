#include <Arduino.h>

#define LEDPin 7

void setup() {
  pinMode(LEDPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LEDPin, !digitalRead(LEDPin));
  delay(250);
}