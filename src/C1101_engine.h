#ifndef C1101_ENGINE_H
#define C1101_ENGINE_H

#include <Arduino.h>
#include <vector>

class C1101_engine
{
private:
    const uint8_t _pinMosi; // Master Out Slave In
    const uint8_t _pinMiso; // Master In Slave Out
    const uint8_t _pinSck;  // Serial Clock
    const uint8_t _pinCs;   // Chip Select
    const uint8_t _pinGdo0; // Główny pin przerwania danych
    const uint8_t _pinGdo2; // Pomocniczy pin danych

    std::vector<std::vector<uint32_t>> data;
    std::vector<int16_t> signaldBm;

    uint16_t minSignalTime = 200, maxSignalTime = 2000;
    uint16_t vectorSize=128;
    bool _isDebugON = true;

public:
    C1101_engine(uint8_t pinMosi, uint8_t pinMiso, uint8_t pinSck, uint8_t pinCs, uint8_t pinGdo0, uint8_t pinGdo2);
    ~C1101_engine();

    void begin();
    void startListening();
    void stopListening();
    bool isDataAvailable();
    void readData();
    void readRawData();
};

#endif // C1101_ENGINE_H