#ifndef RF_RECEIVER_H
#define RF_RECEIVER_H

#include <Arduino.h>
#include <vector>

class RFReceiver {
private:

public:
    RFReceiver();
    ~RFReceiver();

    void readRawData();
};

#endif // RF_RECEIVER_H