#include <Arduino.h>
#include <SENT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>


const byte SENT_PIN = 2;
const int POT_PIN = A0;
const int DHT_PIN = A1;
#define DHTTYPE DHT11

SENT sent(SENT_PIN, SEND_SENT);
dht DHT;

// Inițializează LCD-ul cu adresa I2C
LiquidCrystal_I2C lcd1(0x27, 16, 2);

void setup() {
  pinMode(SENT_PIN, OUTPUT);
  sent.begin(500);
  lcd1.init();
  lcd1.backlight();
}

void loop() {
  int potValue = analogRead(POT_PIN);
  int speedValue = map(potValue, 0, 1023, 0, 200);

  DHT.read11(DHT_PIN);
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  // Afișează valoarea pe primul LCD
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Pot Value:");
  lcd1.setCursor(0, 1);
  lcd1.print(speedValue);
  lcd1.print(" RPM");

  // Trimite valoarea prin protocolul SENT
  sent.send(speedValue, static_cast<uint16_t>(temperature * 100)); // Trimite viteza și temperatura

  delay(500);
}
