#include <Arduino.h>
#include "gDistanceSensor.h"
#include "gDistanceSensorCommands.h"

#include "gComms.h"
extern gComms gMonitor;

uint8_t EnPwmCmd[4]={0x44,0x22,0xbb,0x01};          // distance measure command
char *intToStr( int i );

gDistanceSensor::gDistanceSensor() {
	URPWM=3;                                        // PWM Output 0-25000us,every 50us represent 1cm
	URTRIG=5;                                       // PWM trigger pin
}

void gDistanceSensor::setup(int thisID, gCommandRouter *router, int trigPin, int pwmPin, int timeGap ) {
	instanceID = thisID;

	URTRIG = trigPin;
	URPWM = pwmPin;
	pinMode(URTRIG,OUTPUT);                            // A low pull on pin COMP/TRIG
	digitalWrite(URTRIG,HIGH);                         // Set to HIGH

	pinMode(URPWM, INPUT);                             // Sending Enable PWM mode command
	currentCommand = COMMAND_ID_GLOBAL_NONE;

	setContinuousTiming( timeGap );

	//for(int i=0;i<4;i++){                              // ### this is wrong... ok for serial comms...
	//	Serial.write(EnPwmCmd[i]);
	//}

	if( router != 0 )
		setupCommandListener( *router );
}

void gDistanceSensor::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gDistanceSensor, processCommand);
	if( readTimeGap > 0 ) {
		CMD_METHOD_REGISTER_TIMER(gDistanceSensor, COMMAND_ID_RANGER_CHECK_READING, continueReadings, readTimeGap);
	}
}

int gDistanceSensor::getDistance() {
	if( currentCommand == COMMAND_ID_RANGER_READ_SERIES && millis() - lastReadTime < readTimeGap )
		return -1;
	lastReadTime = millis();

	unsigned int Distance=0;
	digitalWrite(URTRIG, LOW); 
	digitalWrite(URTRIG, HIGH);                      // reading Pin PWM will output pulses

	unsigned long DistanceMeasured=pulseIn(URPWM,LOW);

	if(DistanceMeasured==50000){                     // the reading is invalid.
		Distance = -1;
	}
	else{
		Distance=DistanceMeasured/50;                  // every 50us low level stands for 1cm
	}
	return Distance;
}

void gDistanceSensor::setContinuousTiming(int gap ) {
	readTimeGap = gap;
}
CMD_METHOD_IMPLEMENT( gDistanceSensor, processCommand ) {
	if(cmdObj->commandID != currentCommand ) {
		switch( cmdObj->commandID ) {
		case COMMAND_ID_RANGER_READ_DISTANCE: 
			currentCommand = COMMAND_ID_RANGER_READ_DISTANCE;
			break;
		case COMMAND_ID_RANGER_READ_SERIES: 
			currentCommand = COMMAND_ID_RANGER_READ_SERIES;
			break;
		case COMMAND_ID_RANGER_END_SERIES: 
			currentCommand = COMMAND_ID_GLOBAL_NONE;
			break;
		default: 
			break;
		}
	}
}

CMD_METHOD_IMPLEMENT( gDistanceSensor, continueReadings) {
	int distance = -1;
	switch( currentCommand ) {
	case COMMAND_ID_GLOBAL_NONE:
		return;
	case COMMAND_ID_RANGER_READ_DISTANCE:
		currentCommand = COMMAND_ID_GLOBAL_NONE;
		break;
	}
	distance = getDistance();

	gMonitor.print(distance);
}


