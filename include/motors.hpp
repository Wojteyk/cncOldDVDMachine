#include <Arduino.h>

#pragma once

class Motors
{
public:
    Motors(int in1_, int in2_, int in3_, int in4_, int maxSteps_);

private:
    int in1;
    int in2;
    int in3;
    int in4;

private:
    void step1();

    void step2();

    void step3();

    void step4();

public:
    int maxSteps;

    void rotateRight(int numberOfSteps);

    void rotateLeft(int numberOfSteps);
};