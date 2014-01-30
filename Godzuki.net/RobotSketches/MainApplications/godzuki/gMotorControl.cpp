/*
* gMotorControl.cpp
*
*  Created on: Jan 13, 2014
*      Author: john
*/

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
	gMotor::AFMS.begin();

	(myMotors[0] = new gMotor())->setup(1);
	(myMotors[1] = new gMotor())->setup(2);
	(myMotors[2] = new gMotor())->setup(3);
	(myMotors[3] = new gMotor())->setup(4);

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
		gMonitor.println("  motor controller - Setting fast speed");
		setSpeeds(150);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW:
		gMonitor.println("  motor controller - Setting slower speed");
		setSpeeds(50);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED:
		gMonitor.print("  motor controller - setting speed ");
		gMonitor.print(cmdObj->parameter);
		gMonitor.println("");
		setSpeeds(cmdObj->parameter);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP:
		currentSpeed += 50;
		currentSpeed %= 256;
		gMonitor.print("  motor controller - bumping speed (mod 255) to ");
		gMonitor.print(currentSpeed);
		gMonitor.println("");
		setSpeeds(currentSpeed);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_FWD:
		gMonitor.println("  motor controller - Setting forward dir");
		setDirections(FORWARD);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_DIR_REV:
		gMonitor.println("  motor controller - Setting backwards dir");
		setDirections(BACKWARD);
		break;
	case COMMAND_ID_MOTORCONTROL_START:
		gMonitor.println("  motor controller - Start!");
		startAll();
		break;
	case COMMAND_ID_MOTORCONTROL_STOP:
		gMonitor.println("  motor controller - Stop!");
		stopAll();
		break;
	}
}

void gMotorControl::setSpeeds(int thisSpeed) {
	currentSpeed = thisSpeed;
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			myMotors[i]->setSpeed(thisSpeed);
}

void gMotorControl::setDirections(int thisDir) {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			myMotors[i]->setDirection(thisDir);
}

void gMotorControl::startAll() {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			myMotors[i]->go();
}

void gMotorControl::stopAll() {
	for( int i=0; i<maxMotors; i++ )
		if( myMotors[i] != 0 )
			myMotors[i]->stop();
}

