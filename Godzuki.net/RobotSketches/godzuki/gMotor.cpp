#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include "gMotor.h"

Adafruit_MotorShield gMotor::AFMS = Adafruit_MotorShield();

gMotor::gMotor() {
	myMotor = 0;
}

gMotor::~gMotor() {
	// TODO Auto-generated destructor stub
}

void gMotor::setup( int motorID ) {
	myMotor = AFMS.getMotor( motorID );
}

void gMotor::setSpeed( int thisSpeed ) {
    potentialSpeed = thisSpeed;
}

void gMotor::setDirection(int dir) {
    potentialDir = dir;
}

void gMotor::go() {
	myMotor->run(potentialDir);
	myMotor->setSpeed(potentialSpeed);
}

void gMotor::stop() {
	myMotor->setSpeed(0);
	myMotor->run(RELEASE);
}
