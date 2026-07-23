#ifndef C1101_ENGINE_H
#define C1101_ENGINE_H

#include <Arduino.h>
// #include <vector>

constexpr uint32_t IMPULSE_BUFFER_SIZE = 16384; // min 64

struct HeaderPacket
{
    uint32_t firstImpulse;
    uint32_t readTime;
    uint16_t packetLength;
    int8_t dBm;
};

class C1101Engine
{
private:
    const uint8_t _pinMosi; // Master Out Slave In
    const uint8_t _pinMiso; // Master In Slave Out
    const uint8_t _pinSck;  // Serial Clock
    const uint8_t _pinCs;   // Chip Select
    const uint8_t _pinGdo0;
    const uint8_t _pinGdo2;

    long long lastTime = micros();

    uint16_t _listeningToTimeout = 10000;                 // microseconds 10^-6
    uint16_t _minSignalTime = 200, _maxSignalTime = 2000; // microseconds 10^-6

    float _mhz = 433.92;             // set frequency
    uint8_t _receiverBandwith = 100; // receiver bandwith +-
    uint8_t _modulation = 2;         // modulation OSK
    uint8_t _crc = 0;                // CRC check control 0-off 1-on
    uint8_t _syncMode = 0;           // sync word mode
    uint8_t _transmissionPower = 10; // min -20dBm max +10dBm

    uint32_t _data[IMPULSE_BUFFER_SIZE];
    HeaderPacket _headerPacket[IMPULSE_BUFFER_SIZE / 32];

    volatile uint32_t _dataHead = 0;
    uint32_t _dataTail = 0;

    volatile uint32_t _headerPacketHead = 0;
    uint32_t _headerPacketTail = 0;

    bool _isDebugON = false;
    bool _isDataFull = false;
    bool _isListeningTo = false;

public:
    C1101Engine(uint8_t pinMosi, uint8_t pinMiso, uint8_t pinSck, uint8_t pinCs, uint8_t pinGdo0, uint8_t pinGdo2);
    ~C1101Engine();

    void begin();
    void startListening();
    void stopListening();
    bool isDataAvailable();
    void readData(uint16_t listeningToTimeout = 10000); // variables: timeout
    void IRAM_ATTR readDataInterruptHandler();
};

#endif // C1101_ENGINE_H