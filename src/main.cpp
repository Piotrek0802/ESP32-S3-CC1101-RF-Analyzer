#include <Arduino.h>
#include "C1101Engine.h"
#include "RFReceiver.h"

constexpr uint8_t pinMosi = 7;
constexpr uint8_t pinMiso = 15;
constexpr uint8_t pinSck = 6;
constexpr uint8_t pinCs = 4;
constexpr uint8_t pinGdo0 = 5;
constexpr uint8_t pinGdo2 = 16;

C1101Engine Antena(pinMosi, pinMiso, pinSck, pinCs, pinGdo0, pinGdo2);


void setup()
{
    Serial.begin(115200);

    Antena.begin();
    Antena.startListening();

    Serial.println("--- ROZPOCZĘTO SKANOWANIE ETERU (433.92 MHz) ---");

    Antena.readData();
}

void loop()
{
}