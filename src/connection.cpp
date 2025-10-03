#include "connection.hpp"



SerialCom::SerialCom(int speedBaud, Plotter &machine)
    : baudSpeed(speedBaud), CnCek(machine) {}

void SerialCom::begin()
{
        Serial.begin(baudSpeed);
}

void SerialCom::sendStop()
{
        Serial.println("STOP"); // confirmation for python
}

void SerialCom::readBmp()
{
            if (index == 0) {
        memset(CnCek.bmp.data, 0, CnCek.bmp.size);
        }
        Serial.println("READY"); // confirmation for python

        while (Serial.available() > 0 && index < CnCek.bmp.size)
        {
            CnCek.bmp.data[index++] = Serial.read();
        }
        if (index % 32 == 0)
        {
            Serial.println("NEXT"); // confirmation for Python
        }
        if (index >= CnCek.bmp.size - 1)
        {
            index = 0;
            CnCek.workFlag = Plotter::Working;
        }
    }

    void SerialCom::readSerial()
    {
        if (Serial.available() > 0)
        {
            char message = Serial.read();
            switch (message)
            {
            case 'U':   //up
                CnCek.moveX(1);
                break;
            case 'D':   //down
                CnCek.moveX(-1);
                break;
            case 'L':   //left
                CnCek.moveY(-1);
                break;
            case 'R':   //right
                CnCek.moveY(1);
                break;
            case 'S':   //start
                CnCek.workFlag = Plotter::Reading; // start working but first load map (thats why its named reading)
                break;
            case 'X':   //cancel
                CnCek.stopWorking(); // cancel printig sesion
                break;
            case 'H':   //move pen up
                CnCek.penUp();
                break;
            case 'Z':   //move pen down
                CnCek.penDown();
                break;
            case 'V':   //make printing head more preassure 
                CnCek.correctHeadPosition(1);
                break;
            case 'N':   //make priting head less preasure
                CnCek.correctHeadPosition(-1);
                break;
            default:
                break;
            }
        }
    }