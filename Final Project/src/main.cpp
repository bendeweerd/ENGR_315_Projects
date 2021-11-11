#include <Arduino.h>
#include <LiquidCrystal.h>

#define THERMISTORPIN A0
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000

int samples[NUMSAMPLES];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  analogReference(EXTERNAL);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Startup");
  lcd.setCursor(0, 1);
  lcd.print("Successful!");
  delay(500);
}

void loop() {
  //compute average value of 5 thermistor readings
  uint8_t i;
  float average;

  for(i = 0; i < NUMSAMPLES; i++){
    samples[i] = analogRead(THERMISTORPIN);
    delay(10);
  }

  average = 0;
  for(i = 0; i < NUMSAMPLES; i++){
    average += samples[i];
  }
  average /= NUMSAMPLES;

 // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C

  float fahrenheit = (steinhart * 1.8) + 32.0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Average Temp:");
  lcd.setCursor(0,1);
  lcd.print(fahrenheit);

  delay(250);
}
