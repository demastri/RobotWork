#include <Arduino.h>
#include "gHBStatus.h"
#include "gHBStatusCommands.h"

#include "gComms.h"
extern gComms gMonitor;

gHBStatus::gHBStatus() {
	STATUS_LED_PIN = 13;
	HEARTBEAT_FREQUENCY = 1500; //mills
	HEARTBEAT_WIDTH = 100; //mills
	last_heartbeat_on = 0;
	ledState = 0;
	printHeartbeatStatus = false;
}
void gHBStatus::setup(int thisID, gCommandRouter *router ) {
	instanceID = thisID;
	pinMode(STATUS_LED_PIN, OUTPUT);  //we'll use the debug LED to output a heartbeat
	if( router != 0 )
		setupCommandListener( *router );

}
void gHBStatus::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER(gHBStatus, COMMAND_ID_HBSTATUS_LOOP_INIT, loopInit);
	CMD_METHOD_REGISTER(gHBStatus, COMMAND_ID_HBSTATUS_SET_STATUS, setStatus);
	CMD_METHOD_REGISTER(gHBStatus, COMMAND_ID_HBSTATUS_CLEAR_STATUS, clearStatus);
	CMD_METHOD_REGISTER(gHBStatus, COMMAND_ID_HBSTATUS_LOOP_FINALLY, loopFinally);
}

CMD_METHOD_IMPLEMENT(gHBStatus,loopInit) {
	ledState = 2;  // at beginning of loop, noone's told me to do anything yet
}
CMD_METHOD_IMPLEMENT(gHBStatus,setStatus) {
	ledState = 1;
}
CMD_METHOD_IMPLEMENT(gHBStatus,clearStatus) {
	if( ledState == 2 )
		ledState = 0;
}
CMD_METHOD_IMPLEMENT(gHBStatus,loopFinally) {
	checkHeartbeat();
	if( ledState != 2 )
		digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
}

void gHBStatus::checkHeartbeat() {
	if( millis() - last_heartbeat_on >= 0 && millis() - last_heartbeat_on <= HEARTBEAT_WIDTH ) {  // should still be on...
		setStatusActually(-1,-1);
	}
	if( millis() - last_heartbeat_on > HEARTBEAT_WIDTH ) {
		if( printHeartbeatStatus ) {
			gMonitor.print("turning off at: ");
			gMonitor.println(millis());
		}
		clearStatusActually(-1,-1);
	}
	if( millis() - last_heartbeat_on > HEARTBEAT_FREQUENCY ) {
		if( printHeartbeatStatus ) {
			gMonitor.print("turning on at: ");
			gMonitor.println(millis());
		}
		last_heartbeat_on = millis();
		setStatusActually(-1,-1);
	}
}


