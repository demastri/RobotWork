#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "../../../RobotSketches/libraries/ZukiLib/gRoutingDeviceIDs.h"
#include "../../../RobotSketches/libraries/ZukiLib/gCommandRouter.h"

class SimpleObject {
public:
	static const unsigned char DEVICE_ID = SIMPLE_OBJECT_DEVICE_ID;

	static const unsigned char COMMAND_ID_DO_SOMETHING		= 0x01;
	static const unsigned char COMMAND_ID_DO_SOMETHING_ELSE	= 0x02;

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
