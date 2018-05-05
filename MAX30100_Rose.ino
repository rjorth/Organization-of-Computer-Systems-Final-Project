#include <elapsedMillis.h>

/*
  Arduino-MAX30100 oximetry / heart rate integrated sensor library
  Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

bool pin_state = 1; 
elapsedMillis pulse_t = 0; 

uint32_t tsLastReport = 0;
const int ledPin = 9;
int times = 0; 

void setup()
{
  Serial.begin(9600);

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }

  pinMode(ledPin, OUTPUT);
  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  for (int i = 0; i < 10; i++) {
    pox.update();


    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      Serial.print("Heart rate:");
      int bpm = pox.getHeartRate();
      

    }

  }
  pulse_t = 0; 
}

void loop()
{
  // Make sure to call update as fast as possible
  pox.update();


  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    int bpm = pox.getHeartRate();
    Serial.println(bpm);
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    tsLastReport = millis();
    times = (60000 / bpm / 2); 

  }
  if (pulse_t > times) {
    blink_this(); 
    pulse_t = 0;
  }
}

void blink_this() {

  pin_state = !pin_state; 
  digitalWrite(ledPin, pin_state);
  


}

//   int times = onBeatDetected();
//    Serial.println(times);
//    digitalWrite(ledPin, HIGH);
//  //Serial.println("HIGH");
//    delay(times);
//  //digitalWrite(ledPin, LOW);
//  //delay(times);
//    digitalWrite(ledPin, LOW);
//    delay(times);
//  //pox.setOnBeatDetectedCallback(onBeatDetected);
