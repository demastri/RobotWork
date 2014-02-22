#include <Arduino.h>
#include "gHBStatus.h"

#undef USB_MONITOR_CMDS
#undef RADIO_MONITOR_CMDS
#undef REPORT_BFR_SIZE

#include "gRoutingDeviceIDs.h"
#include "gHBStatusCommands.h"

#ifdef USB_MONITOR_CMDS
#include "gMotorControlCommands.h"
#include "gDistanceSensorCommands.h"
#endif
#ifdef RADIO_MONITOR_CMDS
#include "gMotorControlCommands.h"
#include "gDistanceSensorCommands.h"
#endif

extern const int gButtonMappingCmds[];
extern const int gButtonMappingDevs[];
extern const int gButtonMappingIDs[];

#include "gInputs.h"
#include "gComms.h"
extern gComms gMonitor;
#include "gCommandRouter.h"

static const unsigned int adc_key_val[5] = { 30, 150, 360, 535, 760 };
extern int DEFAULT_INSTANCE_ID;

gInputs::gInputs() {
	adc_key_in = 0;
	NUM_KEYS = 5;
	key = -1;
	oldkey = -1;
	showKeyStateOnLED = 1;
	returnKey = ROUTER_NO_COMMAND;
}

void gInputs::setup(int thisID, gCommandRouter *router ) {
	instanceID = thisID;
	if( router != 0 )
		setupCommandListener( *router );
	pRouter = router;
}

void gInputs::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gInputs, processCommand);
	router.AddCommandHandler( GODZUKI_SENSOR_PLATFORM_DEVICE_ID, DEFAULT_INSTANCE_ID, this, COMMAND_ID_GLOBAL_REQUEST_STATUS,  (gInputs::processCommandProxy), -1 );
	pRouter = &router;
}
CMD_METHOD_IMPLEMENT(gInputs,processCommand) {
	switch( cmdObj->commandID ) {
	case COMMAND_ID_GLOBAL_REQUEST_STATUS:
#ifdef REPORT_BFR_SIZE
		if( Serial1.bfrSize() == 64 ) {
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 2, (void *)"64" );
		}
		else if( Serial1.bfrSize() == 256 ) {
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 3, (void *)"256" );
		}
		else {
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 2, (void *)"-1" );
		}
#else
		for( int i=0; i<1; i++ ) {
			Serial1.print("-");
			Serial1.print(i);
			Serial1.print("-");
			gCommandObject *newObj = gCommandObject::gCommandObjectFactory( cmdObj );
			ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0);
			cmdObj = newObj;
		}
#endif
		break;
	}
}

char serialCmd[30];
int cmdSize = -1;
char ser1Cmd[30];
int ser1cmdPtr = -1;

int gInputs::ReadCommand(int &param) {
	while( cmdSize >= 0 && Serial.available()) {
		int kbdKey = Serial.read();
		serialCmd[cmdSize++] = (char)kbdKey;

		if( kbdKey == '#' ) {
			if( cmdSize == 20 ) {
				// done with command
				gMonitor.writeToRadio = 0;   // 1 = radio, 0 = usb

				gCommandObject *outObj = gComms::UnpackCommandString(serialCmd, 1);
				serialCmd[cmdSize++] = '\0';

				Serial.print( "RCS" );
			} else {
				// ask for a resend here
				gCommandObject *newObj = gCommandObject::gCommandObjectFactory( 
					0, DEVICE_ID, instanceID, CNC_APP_DEVICE_ID, 1, COMMAND_ID_GLOBAL_NONE, 0,0,0 );
				pRouter->RouteReply( newObj, GLOBAL_COMMAND_STATUS_RESEND, 0, 0 );

				Serial.print( "BAD CMD" );
			}
			Serial.println(ser1Cmd);
			cmdSize = -1;

			pRouter->RouteCommand( outObj );
		}
		if( cmdSize > 30 ) {
			Serial.print( "Unk" );
			Serial.println( serialCmd );
			cmdSize = -1;
		}
	}
	if (Serial.available()) {
		int kbdKey = Serial.read();
		switch (kbdKey) {
		case '!':
			if( cmdSize < 0 ) {
				cmdSize = 0;
				serialCmd[cmdSize++] = (char)kbdKey;
			}
			break;
#ifdef USB_MONITOR_CMDS
		case 'h':
			pRouter->DumpHandlerTree();
			return ROUTER_NO_COMMAND;
		case '>':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_FAST, -1 );
			return ROUTER_NO_COMMAND;
		case '<':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW, -1 );
			return ROUTER_NO_COMMAND;
		case 'b':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP, -1 );
			return ROUTER_NO_COMMAND;
		case ':':
			param = 0;
			while (Serial.available()) {
				kbdKey = Serial.read();
				if( '0' <= kbdKey && kbdKey <= '9' )
					param = 10 * (param) + (kbdKey - '0');
			}
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED, param );
			return ROUTER_NO_COMMAND;
		case '{':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_DIR_REV, -1 );
			return ROUTER_NO_COMMAND;
		case '}':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_DIR_FWD, -1 );
			return ROUTER_NO_COMMAND;
		case ',':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_STOP, -1 );
			return ROUTER_NO_COMMAND;
		case '.':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_START, -1 );
			return ROUTER_NO_COMMAND;

		case 'c':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_CENTER, -1 );
			return ROUTER_NO_COMMAND;
		case 'r':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_READ_POSITION, -1 );
			return ROUTER_NO_COMMAND;
		case 's':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_CONTINUOUS, -1 );
			return ROUTER_NO_COMMAND;
		case 'o':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_ONCE, -1 );
			return ROUTER_NO_COMMAND;
		case 'x':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_STOP, -1 );
			return ROUTER_NO_COMMAND;
		case 'd':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_READ_DISTANCE, -1 );
			return ROUTER_NO_COMMAND;
		case 'u':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_READ_SERIES, -1 );
			return ROUTER_NO_COMMAND;
		case 'k':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_END_SERIES, -1 );
			return ROUTER_NO_COMMAND;
		case 'g':
			param = 0;
			while (Serial.available()) {
				kbdKey = Serial.read();
				if( '0' <= kbdKey && kbdKey <= '9' )
					param = 10 * (param) + (kbdKey - '0');
			}
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SET_POSITION, param );
			return ROUTER_NO_COMMAND;
		case 'f':
			ROUTE_COMMAND( COMMS_DEVICE_ID, 1, COMMAND_ID_COMMS_FOLLOW_SERIAL, 0 );
			return ROUTER_NO_COMMAND;
		default:
			break;
#endif
		}
	}

	while( ser1cmdPtr >= 0 && Serial1.available()) {
		int kbdKey = Serial1.read();
		ser1Cmd[ser1cmdPtr++] = (char)kbdKey;

		if( kbdKey == '#' ) {
			if( cmdSize == 20 ) {
				// done with command
				gMonitor.writeToRadio = 1;   // 1 = radio, 0 = usb

				gCommandObject *outObj = gComms::UnpackCommandString(ser1Cmd, 1);
				ser1Cmd[ser1cmdPtr++] = '\0';

				Serial1.print( "RCS" );
			} else {
				// ask for a resend here
				gCommandObject *newObj = gCommandObject::gCommandObjectFactory( 
					1, DEVICE_ID, instanceID, CNC_APP_DEVICE_ID, 1, COMMAND_ID_GLOBAL_NONE, 0,0,0 );
				pRouter->RouteReply( newObj, GLOBAL_COMMAND_STATUS_RESEND, 0, 0 );
				Serial1.print( "BAD CMD" );
			}
			Serial1.println(ser1Cmd);
			ser1cmdPtr = -1;

			pRouter->RouteCommand( outObj );
		}
		if( cmdSize > 30 ) {
			Serial1.print( "Unk" );
			Serial1.println( ser1Cmd );
			ser1cmdPtr = -1;
		}
	}
	if (Serial1.available()) {
		int kbdKey = Serial1.read();
		switch (kbdKey) {
		case '!':
			if( ser1cmdPtr < 0 ) {
				ser1cmdPtr = 0;
				ser1Cmd[ser1cmdPtr++] = (char)kbdKey;
			}
			break;
#ifdef RADIO_MONITOR_CMDS
		case 'h':
			pRouter->DumpHandlerTree();
			return ROUTER_NO_COMMAND;
		case '>':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_FAST, -1 );
			return ROUTER_NO_COMMAND;
		case '<':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW, -1 );
			return ROUTER_NO_COMMAND;
		case 'b':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP, -1 );
			return ROUTER_NO_COMMAND;
		case ':':
			param = 0;
			delay(5);
			while (Serial1.available()) {
				kbdKey = Serial1.read();
				if( '0' <= kbdKey && kbdKey <= '9' )
					param = 10 * (param) + (kbdKey - '0');
				delay(5);
			}
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_SPEED, param );
			return ROUTER_NO_COMMAND;
		case '{':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_DIR_REV, -1 );
			return ROUTER_NO_COMMAND;
		case '}':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_SET_DIR_FWD, -1 );
			return ROUTER_NO_COMMAND;
		case ',':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_STOP, -1 );
			return ROUTER_NO_COMMAND;
		case '.':
			ROUTE_COMMAND( MOTOR_CONTROL_DEVICE_ID, 1, COMMAND_ID_MOTORCONTROL_START, -1 );
			return ROUTER_NO_COMMAND;

		case 'c':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_CENTER, -1 );
			return ROUTER_NO_COMMAND;
		case 'r':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_READ_POSITION, -1 );
			return ROUTER_NO_COMMAND;
		case 's':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_CONTINUOUS, -1 );
			return ROUTER_NO_COMMAND;
		case 'o':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_ONCE, -1 );
			return ROUTER_NO_COMMAND;
		case 'x':
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SWEEP_STOP, -1 );
			return ROUTER_NO_COMMAND;
		case 'd':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_READ_DISTANCE, -1 );
			return ROUTER_NO_COMMAND;
		case 'u':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_READ_SERIES, -1 );
			return ROUTER_NO_COMMAND;
		case 'k':
			ROUTE_COMMAND( DISTANCE_SENSOR_DEVICE_ID, 1, COMMAND_ID_RANGER_END_SERIES, -1 );
			return ROUTER_NO_COMMAND;
		case 'g':
			param = 0;
			delay(5);
			while (Serial1.available()) {
				kbdKey = Serial1.read();
				if( '0' <= kbdKey && kbdKey <= '9' )
					param = 10 * (param) + (kbdKey - '0');
				delay(5);
			}
			ROUTE_COMMAND( SERVO_DEVICE_ID, 1, COMMAND_ID_SERVO_SET_POSITION, param );
			return ROUTER_NO_COMMAND;
		case 'f':
			ROUTE_COMMAND( COMMS_DEVICE_ID, 1, COMMAND_ID_COMMS_FOLLOW_SERIAL, 1 );
			return ROUTER_NO_COMMAND;
		default:
			break;
#endif
		}
	}

	adc_key_in = analogRead(0);    // read the value from the sensor
	/* get the key */
	key = get_key(adc_key_in);    // convert into key press
	if (key != oldkey) {   // if keypress is detected
		delay(50);      // wait for debounce time
		adc_key_in = analogRead(0);    // read the value from the sensor
		key = get_key(adc_key_in);    // convert into key press
		if (key != oldkey) {
			oldkey = key;
			//      if (key >=0){    // old code to monitor the actual adc values
			//         Serial.println(adc_key_in, DEC);
			//      }
		}
	}
	if (showKeyStateOnLED) {
		if (key == -1)
		{
			gHBStatus::clearStatusProxy(0,0);
			//ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_CLEAR_STATUS, -1 );
		}
		else
		{
			gHBStatus::setStatusProxy(0,0);
			//ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_SET_STATUS, -1 );
		}
	}
	if (key == returnKey)
		return ROUTER_NO_COMMAND;
	if( 0 <= key && key <= 4 )
		ROUTE_COMMAND( gButtonMappingDevs[key], gButtonMappingIDs[key], gButtonMappingCmds[key], -1 );
	returnKey = key;
	return ROUTER_NO_COMMAND;
}

// Convert ADC value to key number
int gInputs::get_key(unsigned int input) {
	int k;
	for (k = 0; k < NUM_KEYS; k++) {
		if (input < adc_key_val[k]) {
			return k;
		}
	}
	if (k >= NUM_KEYS)
		k = -1;     // No valid key pressed
	return k;
}
