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
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW:
		setSpeeds(50);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED:
		setSpeeds(cmdObj->parameter);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP:
		currentSpeed += 50;
		currentSpeed %= 256;
		setSpeeds(currentSpeed);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_FWD:
		setDirections(FORWARD);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_REV:
		setDirections(BACKWARD);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_START:
		if( cmdObj->parameter > 0 ) { // embed everything into one command
			// 6 characters
			//  3 chars for motor control
				// first is 1-fwd,2-back,3-left,4-right
				// next 2 are 1 bit for each motor, bit 0 => motor 1..bit 3 => motor 4
			//  3 for speed - 0-255
			int paramDir = cmdObj->parameter / 100000;
			int paramMask = (cmdObj->parameter / 1000) % 100;
			int paramSpeed = cmdObj->parameter % 1000;
			for( int i=0; i<maxMotors; i++ ) {
				int thisDir = (paramDir == COMMAND_CONST_MOTORCONTROL_FORWARD ? FORWARD : 
					(paramDir == COMMAND_CONST_MOTORCONTROL_BACKWARD ? BACKWARD : 
					(paramDir == COMMAND_CONST_MOTORCONTROL_TURNLEFT ? (i<2 ? BACKWARD : FORWARD) : (i<2 ? FORWARD : BACKWARD) )));
				if( (paramMask & (1<<i)) ) {
					setDirection( thisDir, i );
					setSpeed( paramSpeed, i );
					start( i );
				}
			}
		}
		startAll();
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_STOP:
		stopAll();
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	}
}

void gMotorControl::setSpeeds(int thisSpeed) {
	currentSpeed = thisSpeed;
	for( int i=0; i<maxMotors; i++ )
		setSpeed(thisSpeed, i);
}
void gMotorControl::setSpeed(int thisSpeed, int motorID) {
	currentSpeed = thisSpeed;
	if( !itsCrayCray && myMotors[motorID] != 0 ) 
		myMotors[motorID]->setSpeed(thisSpeed);
}

void gMotorControl::setDirections(int thisDir) {
	for( int i=0; i<maxMotors; i++ )
		setDirection(thisDir, i);
}
void gMotorControl::setDirection(int thisDir, int motorID) {
	if( !itsCrayCray && myMotors[motorID] != 0 ) 
		myMotors[motorID]->setDirection(thisDir);
}

void gMotorControl::startAll() {
	for( int i=0; i<maxMotors; i++ )
		start( i );
}
void gMotorControl::start(int motorID) {
	if( myMotors[motorID] != 0 )
		if( !itsCrayCray ) 
			myMotors[motorID]->go();
}

void gMotorControl::stopAll() {
	for( int i=0; i<maxMotors; i++ )
		stop( i );
}
void gMotorControl::stop(int motorID) {
	if( !itsCrayCray && myMotors[motorID] != 0 ) 
		myMotors[motorID]->stop();
}

