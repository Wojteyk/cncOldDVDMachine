#include <Arduino.h>
#include "Servo.h"
#include <avr/pgmspace.h>
#include "connection.hpp"
#include "plotter.hpp"

//    		##### PIN CONFIGURATION  #####

// motor x
int motorXin1 = 12;
int motorXin2 = 9;
int motorXin3 = 10;
int motorXin4 = 11;

// motr y
int motorYin1 = 4;
int motorYin2 = 5;
int motorYin3 = 6;
int motorYin4 = 7;

// head
int servoPin = 3;

//          ### Hardwear ###

Motors motorX(motorXin1, motorXin2, motorXin3, motorXin4, 65);

Motors motorY(motorYin1, motorYin2, motorYin3, motorYin4, 65);

Servo mazaki;

Plotter CnCek(motorX, motorY, mazaki);

SerialCom serialPort(9600, CnCek);

void setup()
{

	serialPort.begin();

	pinMode(servoPin, OUTPUT);
	mazaki.attach(servoPin);

	mazaki.write(10);
	delay(200);
}

void loop()
{

	switch (CnCek.workFlag)
	{
	case Plotter::Waiting:
		serialPort.readSerial();   // waitng for action from python app
		break;
	case Plotter::Reading:			// uplod map
		delay(100);
		serialPort.readBmp();
		break;
	case Plotter::Working:		// start printig map but also listen for stop
		serialPort.readSerial();
		CnCek.update();
		break;
	case Plotter::Done:			// send comand that printig is done and set ploter to waitnig
		serialPort.sendStop();
		CnCek.workFlag = Plotter::Waiting;
		break;
	default:
		break;
	}
}
