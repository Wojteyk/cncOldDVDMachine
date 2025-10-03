#include "plotter.hpp"


Plotter::Plotter(Motors &x, Motors &y, Servo &pen) : motorX(x), motorY(y), mazaki(pen) {}


void Plotter::stopWorking()
{
        currentMachineState = Finished;
}

void Plotter::correctHeadPosition(int val)
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

void Plotter::penUp()
{
        mazaki.write(hooverAngle);
}

void Plotter::penDown()
{
        mazaki.write(drawingAngle);
}

void Plotter::moveX(int n)
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

void Plotter::moveY(int n)
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

void Plotter::update()
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

bool Plotter::getPixelPRGM(int x, int y)
{
        int index = y * bmp.rowBytes + x / 8;
        uint8_t byteVal = pgm_read_byte(&bmp.data[index]);
        return byteVal & (0x80 >> x % 8);
}

bool Plotter::getPixel(int x, int y)
{
        int byteIndex = y * bmp.rowBytes + (x / 8);
        int bitIndex = 7 - (x % 8);
        return (bmp.data[byteIndex] >> bitIndex) & 1;
}

void Plotter::drawPoint(bool isTrue)
{
        penDown();
        delay(penDelay);
        penUp();
}
