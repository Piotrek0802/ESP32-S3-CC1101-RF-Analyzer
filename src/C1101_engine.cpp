#include "C1101_engine.h"

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <vector>

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

    ELECHOUSE_cc1101.setCrc(0);      // bez korekcji
    ELECHOUSE_cc1101.setSyncMode(0); // odczyt 0 i 1

    ELECHOUSE_cc1101.setPA(10); // max moc nadawania
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

    if (len > 0)
    {
        Serial.print("Dane (HEX): ");
        for (int i = 0; i < len; i++)
        {
            Serial.printf("%02X ", buffer[i]);
        }
        Serial.println();
    }
    Serial.println("---");
}
void C1101_engine::readRawData()
{
    long long time0 = micros();
    long long time1 = time0;

    uint8_t lastPrinted = 0;
    uint16_t i = 0;
    bool isFirst = true;

    data.resize(vectorSize);
    signaldBm.resize(vectorSize);
    for (size_t i = 0; i < data.size(); i++)
    {
        data.at(i).reserve(64);
    }

    //---MAIN----
    while (true)
    {
        if (i>=vectorSize)
        {
            Serial.println("Koniec miejsca na dane!");
            return;//tutaj jakis ciag dalszy moze jakies zaczecie od zera?
        }
        
        if (micros() - time0 > 2 * maxSignalTime && isFirst == false)
        {
            i++;
            isFirst = true;
        }

        if (isDataAvailable())
        {
            time1 = micros();

            if (!isFirst && time1 - time0 > minSignalTime && time1 - time0 < maxSignalTime)
            {
                data[i].push_back(time1 - time0);
            }
            else if (time1 - time0 > minSignalTime && time1 - time0 < maxSignalTime)
            {
                isFirst = false;
            }

            while (isDataAvailable())
            {
                delayMicroseconds(5);
            }
            if (micros() - time1 > minSignalTime && micros() - time1 < maxSignalTime)
            {
                data[i].push_back(micros() - time1);
                if (signaldBm[i]==0)
                {
                    signaldBm[i]=ELECHOUSE_cc1101.getRssi();
                }
            }
            time0 = micros();
        }

        //---EXIT---
        if (Serial.available() > 0)
        {
            char incomingChar = Serial.read();
            if (incomingChar == 'c')
            {
                Serial.println("Koniec nasluchiwania");
                return;
            }
        }

        //---DEBUGGING---
        if (_isDebugON)
        {
            if (i > lastPrinted)
            {
                Serial.printf("Dlugosc %u wektora: %u, dBm:%d\n", lastPrinted, data[lastPrinted].size(),signaldBm[lastPrinted]);
                for (size_t k = 0; k < data[lastPrinted].size(); k++)
                {
                    Serial.printf("%dus ", data[lastPrinted][k]);
                }
                Serial.println();
                lastPrinted++;
            }
        }
    }
}