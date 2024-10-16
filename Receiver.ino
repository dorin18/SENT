#include <Arduino.h>
#include <SENT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 500;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

// Definește pinii pentru SENT și motor pas cu pas
const byte SENT_PIN = 2;
SENT sent(SENT_PIN, READ_SENT);

int pole1[] ={0,0,0,0, 0,1,1,1, 0};
int pole2[] ={0,0,0,1, 1,1,0,0, 0};
int pole3[] ={0,1,1,1, 0,0,0,0, 0};
int pole4[] ={1,1,0,0, 0,0,0,1, 0};
int poleStep = 0;

// Initializează LCD-ul cu adresa I2C
LiquidCrystal_I2C lcd2(0x27, 16, 2);

void setup() {
  sent.begin(500); 
  Serial.begin(115200);
  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(0, 0);
  lcd2.print("Waiting for data");

}

void loop() {
  if (sent.available()) {
    uint16_t speedValue = 0, temperatureValue = 0, sentstatus = 0;
    sent.read(&speedValue, &temperatureValue, &sentstatus);

    myStepper.setSpeed(10);
    myStepper.step(speedValue /*stepsPerRevolution*/);
    // Convert temperature value back to float
    float temperature = temperatureValue / 100.0;

    // Afișează valoarea pe al doilea LCD
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("Speed:");
    lcd2.print(speedValue);
    lcd2.print(" RPM");

    // Afișează temperatura pe LCD
    lcd2.setCursor(0, 1);
    lcd2.print("Temp: ");
    lcd2.print(temperature);
    lcd2.print(" C");
  }
}
