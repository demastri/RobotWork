///////servo control definitions
#include <Arduino.h>
#include <Servo.h>

#include "gServo.h"
#include "gServoCommands.h"

#include "gComms.h"
extern gComms gMonitor;

gServo::gServo() {
	SWEEP_TOP = 135;
	SWEEP_CENTER = 90;
	SWEEP_BOTTOM = 45;

	SWEEP_INCREMENT = 5;
	sweep_direction = +SWEEP_INCREMENT;

	last_update_time = 0;
	pos = 0;    // variable to store the servo position 
	currentCommand = COMMAND_ID_GLOBAL_NONE;
}

void gServo::setup( int pin, int thisID, gCommandRouter *router ) {
	myBaseServo.attach( pin );
	instanceID = thisID;
	if( router != 0 )
		setupCommandListener( *router );

}
void gServo::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gServo, processCommand);
	CMD_METHOD_REGISTER_TIMER(gServo, COMMAND_ID_CONTINUE_SWEEP, continueSweep, 50);
	pRouter = &router;
}

void gServo::startSweep() {
	moveTo( SWEEP_BOTTOM );
	sweep_direction = SWEEP_INCREMENT;
	last_update_time = millis();
}

CMD_METHOD_IMPLEMENT(gServo,continueSweep) {
	if( currentCommand == COMMAND_ID_GLOBAL_NONE )
		return;
	int time_increment = 100;

	if( millis() - last_update_time < time_increment )
		return;

	last_update_time = millis();

	pos += sweep_direction;
	if( pos >= SWEEP_TOP ) {
		pos = SWEEP_TOP;
		sweep_direction = -SWEEP_INCREMENT;
	}
	if( pos <= SWEEP_BOTTOM ) {
		pos = SWEEP_BOTTOM;
		sweep_direction = SWEEP_INCREMENT;
		if( currentCommand == COMMAND_ID_SERVO_SWEEP_ONCE )
			stopSweep();
	}
	moveTo( pos );
}

void gServo::stopSweep() {
	currentCommand = COMMAND_ID_GLOBAL_NONE;
}

void gServo::moveTo( int newPos ) {
	newPos = min( max( newPos, SWEEP_BOTTOM ), SWEEP_TOP  );

	myBaseServo.write(newPos);               // tell servo to go to position in variable 'pos' 
	pos = newPos;
}

void gServo::center() {
	moveTo( SWEEP_CENTER );
}

CMD_METHOD_IMPLEMENT(gServo,processCommand) {
	char *outBfr;

	if( cmdObj->commandID != currentCommand ) {
		switch( cmdObj->commandID ) {
		case COMMAND_ID_SERVO_CENTER: 
			gMonitor.println("Going to center");
			center();
			currentCommand = COMMAND_ID_GLOBAL_NONE;
			break;
		case COMMAND_ID_SERVO_SWEEP_ONCE:
			gMonitor.println("Sweeping once");
			startSweep();
			currentCommand = cmdObj->commandID;
			break;
		case COMMAND_ID_SERVO_SWEEP_CONTINUOUS:
			gMonitor.println("Sweeping continuously");
			startSweep();
			currentCommand = cmdObj->commandID;
			break;
		case COMMAND_ID_SERVO_SWEEP_STOP:
			gMonitor.println("Stopping sweep");
			stopSweep();
			currentCommand = COMMAND_ID_GLOBAL_NONE;
			break;
		case COMMAND_ID_SERVO_SET_POSITION:
			gMonitor.print("Setting position to ");
			gMonitor.println(cmdObj->parameter);
			moveTo(cmdObj->parameter);
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
			currentCommand = COMMAND_ID_GLOBAL_NONE;
			break;
		case COMMAND_ID_SERVO_READ_POSITION:
			gMonitor.print("current servo position: ");    // do NOT update current_command...
			gMonitor.println(pos);                // let it keep doing whatever it was doing...
			gCommandObject::PlaceInStrBfr( cmdBfr, pos, 3, 0 );
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, gComms::strlen( (char *)cmdBfr ), cmdBfr );
			break;
		default:
			break;
		}
	}
}

