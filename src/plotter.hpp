#include "Arduino.h"
#include "motors.hpp"
#include "Servo.h"

#pragma once

class Plotter
{
private:
    unsigned long penDelay = 90;

    uint8_t hooverAngle = 10;

    uint8_t drawingAngle = 29;

    Motors &motorX;

    Motors &motorY;

    Servo &mazaki;

    enum CurrentMachineState
    {
        Calculating,
        Wait_Pen_Down,
        Wait_Pen_Up,
        Move_Next,
        Finished
    };

    CurrentMachineState currentMachineState = Calculating;

    int curX = 0, curY = 0;

    const uint8_t stepSize = 1;

    unsigned long lastTime = 0;

public:
    Plotter(Motors &x, Motors &y, Servo &pen) : motorX(x), motorY(y), mazaki(pen) {}

    template<uint8_t WIDTH, uint8_t HEIGHT>
    struct Bitmap
    {
        static constexpr size_t SIZE = (WIDTH * HEIGHT + 7) / 8;  
        static constexpr uint8_t rowBytes = (WIDTH + 7) / 8;

        uint8_t data[SIZE];
        uint8_t width  = WIDTH;
        uint8_t height = HEIGHT;
        uint16_t size  = SIZE;
    };

    Bitmap<40,40> bmp;  // to change the size of the map please use this !!!!!!!!!!!!

    enum workingState
    {
        Waiting,
        Reading,
        Working,
        Done
    };

    workingState workFlag = Waiting;

    void stopWorking(){
        currentMachineState = Finished;
    }

    void correctHeadPosition(int val)
    {
        if (val > 0)
        {
            drawingAngle++;
        }
        else
        {
            drawingAngle--;
        }
    }

    void penUp()
    {
        mazaki.write(hooverAngle);
    }

    void penDown()
    {
        mazaki.write(drawingAngle);
    }

    void moveX(int n)
    {
        if (n > 0)
        {
            motorX.rotateRight(n);
        }
        else
        {
            motorX.rotateLeft(-n);
        }
    }

    void moveY(int n)
    {
        if (n > 0)
        {
            motorY.rotateRight(n);
        }
        else
        {
            motorY.rotateLeft(-n);
        }
    }

    // void loadBitmap()
    // {
    //     bitmap = bmp;
    //     bmpWidth = w;
    //     bmpHeight = h;
    //     curX = 0;
    //     curY = 0;
    //     currentMachineState = Calculating;
    //     rowBytes = (bmpWidth + 7) / 8;
    // }

    void update()
    {

        unsigned long now = millis();

        switch (currentMachineState)
        {
        case Calculating: // caluclate next index and make the right action
            if (curY >= bmp.height)
            {
                currentMachineState = Finished;
                return;
            }

            if (getPixel(curX, curY))
            { // if 1 put pen down
                penDown();
                lastTime = now;
                currentMachineState = Wait_Pen_Down;
            }
            else
            {
                currentMachineState = Move_Next; // otherwise move to the next point
            }

            break;

        case Wait_Pen_Down: // wait for the servo to move down
            if (now - lastTime > penDelay)
            {
                penUp();
                lastTime = now;
                currentMachineState = Wait_Pen_Up;
            }
            break;

        case Wait_Pen_Up: // wait for the srvo to move up
            if (now - lastTime > 80)
            {
                lastTime = now;
                currentMachineState = Move_Next;
            }
            break;

        case Move_Next: // move to the next line
            moveX(stepSize);
            curX++;
            if (curX >= bmp.width)
            {
                curX = 0;
                moveY(stepSize);
                moveX(-bmp.width);
                curY++;
            }

            // check if maximal range of mators is reached
            if (curX >= motorX.maxSteps || curY >= motorX.maxSteps)
            {
                currentMachineState = Finished;
            }
            else
            {
                currentMachineState = Calculating;
            }

            break;

        case Finished:
            curX = 0;
            curY = 0;
            workFlag = Done;
            break;

        default:
            break;
        }
    }

private:
    bool getPixelPRGM(int x, int y)
    {
        int index = y * bmp.rowBytes + x / 8;
        uint8_t byteVal = pgm_read_byte(&bmp.data[index]);
        return byteVal & (0x80 >> x % 8);
    }

    bool getPixel(int x, int y)
    {
        int byteIndex = y * bmp.rowBytes + (x / 8);
        int bitIndex = 7 - (x % 8);
        return (bmp.data[byteIndex] >> bitIndex) & 1;
    }

    void drawPoint(bool isTrue)
    {
        penDown();
        delay(penDelay);
        penUp();
    }
};
