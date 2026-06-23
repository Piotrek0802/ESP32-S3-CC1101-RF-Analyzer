#include <Arduino.h>
#include "C1101_engine.h"

// Twoje nowe, zweryfikowane piny
constexpr uint8_t pinMosi = 7;
constexpr uint8_t pinMiso = 15;
constexpr uint8_t pinSck = 6;
constexpr uint8_t pinCs = 4;
constexpr uint8_t pinGdo0 = 5;
constexpr uint8_t pinGdo2 = 16;

C1101_engine Antena(pinMosi, pinMiso, pinSck, pinCs, pinGdo0, pinGdo2);

void setup()
{
  Serial.begin(115200);
  delay(2000);

  // Uruchamiamy sprzęt przez naszą klasę
  Antena.begin();
  Antena.startListening();

  Serial.println("--- ROZPOCZĘTO SKANOWANIE ETERU (433.92 MHz) ---");
}

void loop()
{
  // Sprawdzamy, czy pin danych GDO0 (GPIO 5) wszedł w stan wysoki
  if (Antena.isDataAvailable())
  {
    Antena.readData();
    unsigned long startTime = micros();

    // Mierzymy jak długo trwa ten impuls
    while (Antena.isDataAvailable())
    {
      delayMicroseconds(5);
    }

    unsigned long duration = micros() - startTime;

    // Ignorujemy ultra-krótkie szumy tła (krótsze niż 100 mikrosekund)
    if (duration > 100)
    {
      Serial.print("Impuls: ");
      Serial.print(duration);
      Serial.println(" us");
    }
  }
}