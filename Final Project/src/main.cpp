#include <Arduino.h>
#include <LiquidCrystal.h>

#define INTAKETHERMISTORPIN A0
#define ROOMTHERMISTORPIN A1

#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000

int intake_samples[NUMSAMPLES];
int room_samples[NUMSAMPLES];

unsigned long data_count = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Helper function to convert a measured resistance into temperature in Fahrenheit
float resistance_to_fahrenheit(float resistance){
  float steinhart;
  steinhart = resistance / THERMISTORNOMINAL;       // (R/Ro)
  steinhart = log(steinhart);                       // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                      // Invert
  steinhart -= 273.15;                              // convert absolute temp to C

  float fahrenheit = (steinhart * 1.8) + 32.0;
  return fahrenheit;
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  analogReference(EXTERNAL);
}

void loop() {
  //compute average value of 5 thermistor readings
  uint8_t i;
  float intake_average;
  float room_average;

  for(i = 0; i < NUMSAMPLES; i++){
    intake_samples[i] = analogRead(INTAKETHERMISTORPIN);
    room_samples[i] = analogRead(ROOMTHERMISTORPIN);
    delay(10);
  }

  intake_average = 0;
  room_average = 0;
  for(i = 0; i < NUMSAMPLES; i++){
    intake_average += intake_samples[i];
    room_average += room_samples[i];
  }
  intake_average /= NUMSAMPLES;
  room_average /= NUMSAMPLES;

  // convert the value to resistance
  intake_average = 1023 / intake_average - 1;
  intake_average = SERIESRESISTOR / intake_average;
  room_average = 1023 / room_average - 1;
  room_average = SERIESRESISTOR / room_average;
  
  // use helper function to convert resistance to temperature in F
  float intake_temp = resistance_to_fahrenheit(intake_average);
  float room_temp = resistance_to_fahrenheit(room_average);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Intake:");
  lcd.setCursor(8,0);
  lcd.print(intake_temp);
  lcd.setCursor(0,1);
  lcd.print("Room:");
  lcd.setCursor(8,1);
  lcd.print(room_temp);

  delay(950);

  data_count++;
  Serial.print(data_count);
  Serial.print(",");
  Serial.print(intake_temp);
  Serial.print(",");
  Serial.println(room_temp);

}