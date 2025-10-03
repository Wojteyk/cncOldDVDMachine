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

    Plotter(Motors &x, Motors &y, Servo &pen);

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

public:

    void stopWorking();

    void correctHeadPosition(int val);

    void penUp();

    void penDown();

    void moveX(int n);

    void moveY(int n);

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

    void update();

private:

    bool getPixelPRGM(int x, int y);

    bool getPixel(int x, int y);

    void drawPoint(bool isTrue);
};
