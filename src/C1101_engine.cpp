#include "C1101_engine.h"

#include <ELECHOUSE_CC1101_SRC_DRV.h>

C1101_engine::C1101_engine(uint8_t pinMosi, uint8_t pinMiso, uint8_t pinSck, uint8_t pinCs, uint8_t pinGdo0, uint8_t pinGdo2)
    : _pinMosi(pinMosi),
      _pinMiso(pinMiso),
      _pinSck(pinSck),
      _pinCs(pinCs),
      _pinGdo0(pinGdo0),
      _pinGdo2(pinGdo2)
{
}

C1101_engine::~C1101_engine()
{
}

void C1101_engine::begin()
{
    ELECHOUSE_cc1101.setSpiPin(_pinSck, _pinMiso, _pinMosi, _pinCs);
    ELECHOUSE_cc1101.Init();
    
    ELECHOUSE_cc1101.setMHZ(433.92);
    ELECHOUSE_cc1101.setModulation(2); 
    
    ELECHOUSE_cc1101.setRxBW(250.00); 
    
    ELECHOUSE_cc1101.setCrc(0);
    ELECHOUSE_cc1101.setSyncMode(0);
    
    pinMode(_pinGdo0, INPUT);
    
    Serial.println("CC1101: Silnik radiowy zainicjalizowany poprawnie.");
}

void C1101_engine::startListening()
{
    ELECHOUSE_cc1101.SetRx();
    Serial.println("CC1101: Radio zostalo przelaczone w tryb ODBIORU (RX).");
}
void C1101_engine::stopListening()
{
    ELECHOUSE_cc1101.setSidle();
    Serial.println("CC1101: Radio zostalo uspione (IDLE).");
}
bool C1101_engine::isDataAvailable()
{
    return (digitalRead(_pinGdo0) == HIGH);
}
void C1101_engine::readData()
{
    uint8_t buffer[64] = {0};
    
    uint8_t len = ELECHOUSE_cc1101.ReceiveData(buffer);
    
    Serial.print("CC1101 flaga była TRUE. Funkcja ReceiveData zwrocila dlugosc: ");
    Serial.println(len);
    
    if (len > 0) {
        Serial.print("Dane (HEX): ");
        for (int i = 0; i < len; i++) {
            Serial.printf("%02X ", buffer[i]);
        }
        Serial.println();
    }
    Serial.println("---");
}