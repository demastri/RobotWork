/*
* gMotorControl.cpp
*
*  Created on: Jan 13, 2014
*      Author: john
*/

extern bool itsCrayCray;

#ifndef PinChangeInt_h
#define LIBCALL_PINCHANGEINT
#include <PinChangeInt.h>
#endif

#include "gMotorControl.h"
#include "gMotorControlCommands.h"

#include "gComms.h"
extern gComms gMonitor;

bool gMotorControl::encoderTargetsSet = false;
bool gMotorControl::encoderTargetsMet = false;
long gMotorControl::leftTarget = -1;
long gMotorControl::rightTarget = -1;
volatile unsigned long gMotorControl::leftTotalClicks = 0;
volatile unsigned long gMotorControl::rightTotalClicks = 0;
volatile long gMotorControl::leftAggregateClicks = 0;
volatile long gMotorControl::rightAggregateClicks = 0;
int gMotorControl::leftDir = 0;
int gMotorControl::rightDir = 0;
gMotor *(gMotorControl::myMotors[4]);

int nbrCalSpeeds[2]		= { 0, 0 };
int pwmSpeed[2][10]		= {{ 0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,0,0,0,0,0,0 }};
int linearSpeed[2][10]	= {{ 0,0,0,0,0,0,0,0,0,0 }, { 0,0,0,0,0,0,0,0,0,0 }};
uint8_t spdVecBfr[30];

gMotorControl::gMotorControl() {
	// TODO Auto-generated constructor stub
	mmsPerClick = 10.2;
	gMotorControl::leftTotalClicks = gMotorControl::rightTotalClicks = 0;
	instSpeedUpdateTime = 1000;
	leftAggregateClicks=rightAggregateClicks=0;
	leftLastAggregateClicks=rightLastAggregateClicks=0;
	lastSpeedCalcTime=0;
	leftDir=rightDir=FORWARD;

	leftTargetCmd = rightTargetCmd = 0;
}

void gMotorControl::setup(int thisID, gCommandRouter *router, int leftEncoderPin, int rightEncoderPin ) {
	instanceID = thisID;

	if( leftEncoderPin >= 0 )
		PCintPort::attachInterrupt(leftEncoderPin, updateLeft,CHANGE);
	if( rightEncoderPin >= 0 )
		PCintPort::attachInterrupt(rightEncoderPin , updateRight,CHANGE);
	updateSpeeds = ( leftEncoderPin >= 0 ) || ( rightEncoderPin >= 0 );

	currentSpeed = 0;	
	if( !itsCrayCray ){
		gMotor::AFMS.begin();

		(myMotors[0] = new gMotor())->setup(1);
		(myMotors[1] = new gMotor())->setup(2);
		(myMotors[2] = new gMotor())->setup(3);
		(myMotors[3] = new gMotor())->setup(4);
	}
	if( router != 0 )
		setupCommandListener( *router );
}
void gMotorControl::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gMotorControl, processCommand);
	CMD_METHOD_REGISTER_TIMER(gMotorControl, COMMAND_ID_MOTORCONTROL_UPDATE_SPEEDS, calculateSpeeds, instSpeedUpdateTime);
	//CMD_METHOD_REGISTER_TIMER(gMotorControl, COMMAND_ID_MOTORCONTROL_GET_ENCODER, processCommand, 1980);
	//CMD_METHOD_REGISTER_TIMER(gMotorControl, COMMAND_ID_MOTORCONTROL_PULL_SPEEDS, processCommand, 1520);

	pRouter = &router;
}

gMotorControl::~gMotorControl() {
	// TODO Auto-generated destructor stub
}

CMD_METHOD_IMPLEMENT(gMotorControl,processCommand) {
	gCommandRouter &router = *pRouter;
	int outLeft;
	int outRight;
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
			// 7 characters
			//  1 character for linear or pwm speed
			//  3 chars for motor control
			// first is 1-fwd,2-back,3-left,4-right, 5-stop
			// next 2 are 1 bit for each motor, bit 0 => motor 1..bit 3 => motor 4
			//  3 for speed - 0-255
			int linear = cmdObj->parameter / 1000000;
			int paramDir = (cmdObj->parameter / 100000) % 10;
			int paramMask = (cmdObj->parameter / 1000) % 100;
			int paramSpeed = cmdObj->parameter % 1000;		// either 0-255 (linear==0) or 0-999 mm/s (linear==1)

			int leftSpeed  = linear ? findActSpeed( paramSpeed, 0 ) : paramSpeed;
			int rightSpeed = linear ? findActSpeed( paramSpeed, 1 ) : paramSpeed;

			leftDir = ((paramDir == COMMAND_CONST_MOTORCONTROL_FORWARD || paramDir == COMMAND_CONST_MOTORCONTROL_TURNRIGHT) ? FORWARD : BACKWARD);
			rightDir = ((paramDir == COMMAND_CONST_MOTORCONTROL_FORWARD || paramDir == COMMAND_CONST_MOTORCONTROL_TURNLEFT) ? FORWARD : BACKWARD);

			for( int i=0; i<maxMotors; i++ ) {
				int thisDir = (paramDir == COMMAND_CONST_MOTORCONTROL_FORWARD ? FORWARD : 
					(paramDir == COMMAND_CONST_MOTORCONTROL_BACKWARD ? BACKWARD : 
					(paramDir == COMMAND_CONST_MOTORCONTROL_TURNLEFT ? (i<2 ? BACKWARD : FORWARD) : (i<2 ? FORWARD : BACKWARD) )));
				if( (paramMask & (1<<i)) ) {
					setDirection( thisDir, i );
					setSpeed( (i<2 ? leftSpeed : rightSpeed), i );
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
	case COMMAND_ID_MOTORCONTROL_PULL_SPEEDS:
		outLeft = leftInstantaneousSpeed * mmsPerClick;
		outRight = rightInstantaneousSpeed * mmsPerClick;
		gCommandObject::PlaceInStrBfr( rtnSpdBfr, outLeft,  4, 0 );
		gCommandObject::PlaceInStrBfr( rtnSpdBfr, outRight,  4, 4 );
		gCommandObject::PlaceInStrBfr( rtnSpdBfr, "\0",  1, 8 );
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,8,rtnSpdBfr);
		break;
	case COMMAND_ID_MOTORCONTROL_GET_ENCODER:
		gCommandObject::PlaceInStrBfr( rtnClkBfr, leftAggregateClicks,  5, 0 );
		gCommandObject::PlaceInStrBfr( rtnClkBfr, rightAggregateClicks,  5, 5 );
		gCommandObject::PlaceInStrBfr( rtnClkBfr, "\0",  1, 10 );
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,10,rtnClkBfr);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_LEFT_TARGET_ABS:
		leftTarget = cmdObj->parameter;
		if( leftTargetCmd )
			delete leftTargetCmd;
		leftTargetCmd = new gCommandObject( cmdObj );
		if( !encoderTargetsSet ) {
			encoderTargetsSet = true;
			CMD_METHOD_REGISTER_TIMER(gMotorControl,COMMAND_ID_MOTORCONTROL_CHECK_TARGETS,checkEncoderTarget,200);
		}
		encoderTargetsMet = false;
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_RIGHT_TARGET_ABS:
		rightTarget = cmdObj->parameter;
		if( rightTargetCmd )
			delete rightTargetCmd;
		rightTargetCmd = new gCommandObject( cmdObj );
		if( !encoderTargetsSet ) {
			encoderTargetsSet = true;
			CMD_METHOD_REGISTER_TIMER(gMotorControl,COMMAND_ID_MOTORCONTROL_CHECK_TARGETS,checkEncoderTarget,200);
		}
		encoderTargetsMet = false;
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_LEFT_TARGET_REL:
		leftTarget = leftAggregateClicks + cmdObj->parameter;
		if( leftTargetCmd )
			delete leftTargetCmd;
		leftTargetCmd = new gCommandObject( cmdObj );
		if( !encoderTargetsSet ) {
			encoderTargetsSet = true;
			CMD_METHOD_REGISTER_TIMER(gMotorControl,COMMAND_ID_MOTORCONTROL_CHECK_TARGETS,checkEncoderTarget,200);
		}
		encoderTargetsMet = false;
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_RIGHT_TARGET_REL:
		rightTarget = rightAggregateClicks + cmdObj->parameter;
		if( rightTargetCmd )
			delete rightTargetCmd;
		rightTargetCmd = new gCommandObject( cmdObj );
		if( !encoderTargetsSet ) {
			encoderTargetsSet = true;
			CMD_METHOD_REGISTER_TIMER(gMotorControl,COMMAND_ID_MOTORCONTROL_CHECK_TARGETS,checkEncoderTarget,200);
		}
		encoderTargetsMet = false;
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_CLEAR_TARGETS:
		encoderTargetsSet = false;
		break;
	case COMMAND_ID_MOTORCONTROL_CLEAR_CALIBRATION:
		clearSpeedVectors();
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_SET_CALIBRATION:
		updateSpeedVectors(cmdObj->parameter);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		break;
	case COMMAND_ID_MOTORCONTROL_CHECK_CALIBRATION :
				int outSize=0;
		uint8_t *outStr = showCalibData(&outSize, cmdObj->parameter/1000, cmdObj->parameter%1000);
		ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,outSize,outStr);
	}
}

CMD_METHOD_IMPLEMENT(gMotorControl, checkEncoderTarget) {
	gCommandRouter &router = *pRouter;
	if( encoderTargetsMet ) {
		encoderTargetsSet = false;
		encoderTargetsMet = false;
		CMD_METHOD_DEREGISTER_TIMER(COMMAND_ID_MOTORCONTROL_CHECK_TARGETS,200);
		if( leftTargetCmd )
		{
			stopAll();
			gCommandObject *cmdObj = leftTargetCmd;
			cmdObj->commandID = COMMAND_ID_MOTORCONTROL_TARGET_REACHED;
			ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		}
		else if( rightTargetCmd )
		{
			stopAll();
			gCommandObject *cmdObj = rightTargetCmd;
			cmdObj->commandID = COMMAND_ID_MOTORCONTROL_TARGET_REACHED;
			ROUTE_REPLY(GLOBAL_COMMAND_STATUS_OK,0,0);
		}
		// else noone was asking...
	}
}

int gMotorControl::findActSpeed( int mmPerSec, int isLeft ) {
	int outSpeed = 0;
	int i;
	for( i=0; i<nbrCalSpeeds[isLeft]; i++ )
		if( linearSpeed[isLeft][i] > mmPerSec )
			break;
	if( i==0 ) // first entry's too big...
	{
		if( nbrCalSpeeds[isLeft] == 0 )
			return 0;
		outSpeed = (pwmSpeed[isLeft][i]*mmPerSec) / linearSpeed[isLeft][i];
	}
	else if( i == nbrCalSpeeds[isLeft] ) // last one's still too small...
	{
		if( i == 1 )// there's only one value...
			outSpeed = pwmSpeed[isLeft][0] + ((pwmSpeed[isLeft][0]-0) * (mmPerSec - linearSpeed[isLeft][0])) / (linearSpeed[isLeft][0]-0);
		else
			outSpeed = pwmSpeed[isLeft][i-1] + ((pwmSpeed[isLeft][i-1]-pwmSpeed[isLeft][i-2]) * (mmPerSec - linearSpeed[isLeft][i-1])) / (linearSpeed[isLeft][i-1]-linearSpeed[isLeft][i-2]);
		if( outSpeed > 255 )
			outSpeed = 255;
	} 
	else 
	{ // middle value...
		outSpeed = pwmSpeed[isLeft][i-1] + ((pwmSpeed[isLeft][i]-pwmSpeed[isLeft][i-1]) * (mmPerSec - linearSpeed[isLeft][i-1])) / (linearSpeed[isLeft][i]-linearSpeed[isLeft][i-1]);
	}
	return outSpeed;
}

void gMotorControl::clearSpeedVectors() {
	for( int i=0; i<2; i++ ) {
		nbrCalSpeeds[i] = 0;
		for( int j=0; j<10; j++ )
			pwmSpeed[i][j] = linearSpeed[i][j] = 0;
	}

}
void gMotorControl::updateSpeedVectors(long param) {
	bool isNeg = ( param < 0 );
	if( isNeg )
		param = -param;
	int isLeft = param / 1000000L;
	int thisPwmSpeed = (param / 1000L) % 1000;	// 0-255
	int thisActSpeed = param % 1000L;	// mm/sec
	pwmSpeed[isLeft][ nbrCalSpeeds[isLeft] ] = (isNeg ? -1 : 1)*thisPwmSpeed;
	linearSpeed[isLeft][ nbrCalSpeeds[isLeft] ] = (isNeg ? -1 : 1)*thisActSpeed;
	nbrCalSpeeds[isLeft]++;

	gMonitor.print(param);
	gMonitor.print(".");
	gMonitor.print(isLeft);
	gMonitor.print(".");
	gMonitor.print(thisPwmSpeed);
	gMonitor.print(".");
	gMonitor.print(thisActSpeed);
}

uint8_t *gMotorControl::showCalibData(int *outSize, int i, int j) {
	if( i<0 || j<0 || i>2 || j >= nbrCalSpeeds[i] ) {
		*outSize = 2;
		gCommandObject::PlaceInStrBfr( spdVecBfr, "-1",  2, 0 );
	} else {
		*outSize = 0;
		gCommandObject::PlaceInStrBfr( spdVecBfr, nbrCalSpeeds[i],  2, *outSize );
		*outSize += 2;
		gCommandObject::PlaceInStrBfr( spdVecBfr, (char *)(i==0?"<":"{"),  1, *outSize );
		*outSize += 1;
		gCommandObject::PlaceInStrBfr( spdVecBfr, pwmSpeed[i][j],  4, *outSize );
		*outSize += 4;
		gCommandObject::PlaceInStrBfr( spdVecBfr, ",",  1, *outSize );
		*outSize += 1;
		gCommandObject::PlaceInStrBfr( spdVecBfr, linearSpeed[i][j],  4, *outSize );
		*outSize += 4;
		gCommandObject::PlaceInStrBfr( spdVecBfr, (char *)(i==0?">":"}"),  1, *outSize );
		*outSize += 1;
	}
	return spdVecBfr;
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
	leftDir = rightDir = thisDir;
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

void gMotorControl::updateLeft()
{
	leftTotalClicks++;
	leftAggregateClicks += (leftDir==FORWARD ? 1 : -1);
	if( encoderTargetsSet && ((leftDir==FORWARD && leftAggregateClicks >= leftTarget) || (leftDir==BACKWARD && leftAggregateClicks <= leftTarget)) ) {
		encoderTargetsMet = true;
	}
}

void gMotorControl::updateRight()
{
	rightTotalClicks++;
	rightAggregateClicks += (rightDir==FORWARD ? 1 : -1);
	if( encoderTargetsSet && ((rightDir==FORWARD && rightAggregateClicks >= rightTarget) || (rightDir==BACKWARD && rightAggregateClicks <= rightTarget)) ) {
		encoderTargetsMet = true;
	}
}
CMD_METHOD_IMPLEMENT(gMotorControl,calculateSpeeds) {
	unsigned long now = millis();
	leftInstantaneousSpeed = (leftAggregateClicks-leftLastAggregateClicks) / ((now-lastSpeedCalcTime) / 1000.0);
	leftLastAggregateClicks = leftAggregateClicks;
	rightInstantaneousSpeed = (rightAggregateClicks-rightLastAggregateClicks) / ((now-lastSpeedCalcTime) / 1000.0);
	rightLastAggregateClicks = rightAggregateClicks;
	lastSpeedCalcTime = now;
}
