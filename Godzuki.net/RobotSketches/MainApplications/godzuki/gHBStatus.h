#ifndef G_HBSTATUS_H
#define G_HBSTATUS_H

#include "gCommandRouter.h"

class gHBStatus {
public:
	static const int DEVICE_ID = HEARTBEAT_DEVICE_ID;

	gHBStatus();
	void setup(int thisID, gCommandRouter *router );

	CMD_METHOD_DEFINE(loopInit);
	CMD_METHOD_DEFINE(setStatus);
	CMD_METHOD_DEFINE(clearStatus);
	CMD_METHOD_DEFINE(loopFinally);

private:
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int STATUS_LED_PIN;
	unsigned long HEARTBEAT_FREQUENCY;
	unsigned long HEARTBEAT_WIDTH;
	unsigned long last_heartbeat_on;
	int ledState;
	int printHeartbeatStatus;

	void checkHeartbeat();
};

#endif
