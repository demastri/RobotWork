#include <Arduino.h>

#include "gRoutingDeviceIDs.h"
#include "gServoCommands.h"
#include "gHBStatusCommands.h"
#include "gMotorControlCommands.h"
#include "gDistanceSensorCommands.h"

const int gButtonMappingCmds[] = {COMMAND_ID_SERVO_SWEEP_ONCE,	COMMAND_ID_SERVO_SWEEP_STOP,	COMMAND_ID_SERVO_CENTER,	COMMAND_ID_SERVO_SWEEP_CONTINUOUS,	COMMAND_ID_SERVO_READ_POSITION };
const int gButtonMappingDevs[] = {SERVO_DEVICE_ID,				SERVO_DEVICE_ID,				SERVO_DEVICE_ID,			SERVO_DEVICE_ID,					SERVO_DEVICE_ID };
const int gButtonMappingIDs[] =	 {1,							1,								1,							1,									1 };


#include "gInputs.h"

#include "gComms.h"

#include "gCommandRouter.h"

static const unsigned int adc_key_val[5] = { 30, 150, 360, 535, 760 };

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
	//	if( router != 0 )
	//		setupCommandListener( *router );
	pRouter = router;
}

int gInputs::ReadCommand(int &param) {
	if (Serial.available()) {
		int kbdKey = Serial.read();
		switch (kbdKey) {
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
			param = 0;
			return FOLLOW_SERIAL;
		default:
			break;
		}
	}
	char serialCmd[20];
	int cmdSize = 0;

	if (Serial1.available()) {
		int kbdKey = Serial1.read();
		switch (kbdKey) {
		case '!':
			serialCmd[cmdSize++] = (char)kbdKey;
			delay(5);
			while (Serial1.available()) {
				kbdKey = Serial1.read();
				serialCmd[cmdSize++] = (char)kbdKey;
				delay(5);
				if( kbdKey == '#' )
					break;
			}
			serialCmd[cmdSize++] = '\0';
			Serial1.print( "Remote Command String..." );
			Serial1.println( serialCmd );
			if( kbdKey == '#' ) {
				pRouter->RouteCommand( gComms::UnpackCommandString(serialCmd) );
			}
			break;
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
			param = 1;
			return FOLLOW_SERIAL;
		default:
			break;
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
			ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_CLEAR_STATUS, -1 );
		else
			ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_SET_STATUS, -1 );
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
