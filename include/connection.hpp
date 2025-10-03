#include "Arduino.h"
#include "plotter.hpp"
#pragma once



class SerialCom
{
private:
    int baudSpeed;

    Plotter &CnCek;

public:

    SerialCom(int speedBaud, Plotter &machine);

    uint8_t index = 0;

    void begin();

    void sendStop();

    void readBmp();

    void readSerial();
};
