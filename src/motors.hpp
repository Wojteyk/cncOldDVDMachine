#include <Arduino.h>

#pragma once

class Motors
{
public:
    Motors(int in1_, int in2_, int in3_, int in4_, int maxSteps_) : in1(in1_), in2(in2_), in3(in3_), in4(in4_), maxSteps(maxSteps_)
    {
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(in3, OUTPUT);
        pinMode(in4, OUTPUT);
    }

private:
    int in1;
    int in2;
    int in3;
    int in4;

private:
    void step1()
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
    }

    void step2()
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
    }

    void step3()
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
    }

    void step4()
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
    }

public:
    int maxSteps;

    void rotateRight(int numberOfSteps)
    {
        for (int i = 0; i < numberOfSteps; i++)
        {
            delay(3);
            step1();
            delay(3);
            step2();
            delay(3);
            step3();
            delay(3);
            step4();
        }
    }

    void rotateLeft(int numberOfSteps)
    {
        for (int i = 0; i < numberOfSteps; i++)
        {
            delay(3);
            step4();
            delay(3);
            step3();
            delay(3);
            step2();
            delay(3);
            step1();
        }
    }
};