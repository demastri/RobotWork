#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "../../../RobotSketches/MainApplications/godzuki/gRoutingDeviceIDs.h"
#include "../../../RobotSketches/MainApplications/godzuki/gCommandRouter.h"

class SimpleObject {
public:
	static const unsigned char DEVICE_ID = SIMPLE_OBJECT_DEVICE_ID;

	static const unsigned char COMMAND_ID_DO_SOMETHING		= 0x01;
	static const unsigned char COMMAND_ID_DO_SOMETHING_ELSE	= 0x02;

	static const unsigned char COMMAND_REPLY_OK				= 0x01;
	static const unsigned char COMMAND_REPLY_FAIL			= 0xFF;

	SimpleObject();
	~SimpleObject() {
		CMD_METHOD_DEREGISTER_ALL();
	}
	void setup(int thisID, gCommandRouter *router);
	void teardown(gCommandRouter *router);
	void teardownTimer(gCommandRouter *pRouter);

private:
	gCommandRouter *pRouter;
	int instanceID;
	static unsigned int strlen( char *s ) { int i=0; while( *(s+i) != '\0') i++; return i; };

	void setupCommandListener( gCommandRouter &router );
	void teardownCommandListener( gCommandRouter &router );

	CMD_METHOD_DEFINE(processCommand); // catchall method...

	CMD_METHOD_DEFINE(doSomething);
	CMD_METHOD_DEFINE(doSomethingElse);
};

#endif
