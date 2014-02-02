/*
* gMotorControl.cpp
*
*  Created on: Jan 13, 2014
*      Author: john
*/

extern bool itsCrayCray;

#include "gMotorControl.h"
#include "gMotorControlCommands.h"

#include "gComms.h"
extern gComms gMonitor;

gMotorControl::gMotorControl() {
	// TODO Auto-generated constructor stub

}

void gMotorControl::setup(int thisID, gCommandRouter *router ) {
	instanceID = thisID;
	if( router != 0 )
		setupCommandListener( *router );
	
	currentSpeed = 0;	
	if( !itsCrayCray ){
		gMotor::AFMS.begin();

		(myMotors[0] = new gMotor())->setup(1);
		(myMotors[1] = new gMotor())->setup(2);
		(myMotors[2] = new gMotor())->setup(3);
		(myMotors[3] = new gMotor())->setup(4);
	}

}
void gMotorControl::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gMotorControl, processCommand);
	pRouter = &router;
}

gMotorControl::~gMotorControl() {
	// TODO Auto-generated destructor stub
}

CMD_METHOD_IMPLEMENT(gMotorControl,processCommand) {
	switch( cmdObj->commandID ) {
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_FAST:
		setSpeeds(150);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW:
		setSpeeds(50);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED:
		setSpeeds(cmdObj->parameter);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP:
		currentSpeed += 50;
		currentSpeed %= 256;
		setSpeeds(currentSpeed);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_FWD:
		setDirections(FORWARD);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_REV:
		setDirections(BACKWARD);
		break;
	case COMMAND_ID_MOTORCONTROL_START:
		startAll();
		break;
	case COMMAND_ID_MOTORCONTROL_STOP:
		stopAll();
		break;
	}
}

void gMotorControl::setSpeeds(int thisSpeed) {
	currentSpeed = thisSpeed;
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			if( !itsCrayCray) myMotors[i]->setSpeed(thisSpeed);
}

void gMotorControl::setDirections(int thisDir) {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			if( !itsCrayCray) myMotors[i]->setDirection(thisDir);
}

void gMotorControl::startAll() {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			if( !itsCrayCray) myMotors[i]->go();
}

void gMotorControl::stopAll() {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			if( !itsCrayCray) myMotors[i]->stop();
}

