///
/// Input Router main loop - initially brought over from arduino IDE
///

#include "stdafx.h"

extern unsigned long millis();
#include <conio.h>
#include <iostream>

#include <chrono>

#include "../../../RobotSketches/libraries/ZukiLib/gComms.h"
extern Godzuki::gComms gMonitor;

#include "SimpleObject.h"
gCommandRouter myRouter;
gCommandRouter *pRouter=0;
SimpleObject *pMyObject;

long startTimer;
char *prompt = "\
 1 - Route Something Cmd\n\
 2 - Route N/A cmd\n\
 d - dump handler tree\n\
 r - request status\n\
 s - setup obj\n\
 x - teardown timer handler\n\
 X - teardown obj\n\
 q - quit\nCommand:";

extern int DEFAULT_DEVICE_ID;
extern int DEFAULT_INSTANCE_ID;

void receiveCommands( void *objRef, gCommandObject *cmdObj ) {
	if( cmdObj->isReply )
		printf( "\nReceived a reply!\n status <%d>\n data <%.*s>\n", (int)cmdObj->rtnStatus, (char *)cmdObj->payloadSize, (char *)cmdObj->payloadData);
	else
		printf( "\nReceived a command response...\nexpecting a string got\n status <%d>\n size <%d>\n data <%.*s>\n", (int)cmdObj->rtnStatus, (int)cmdObj->payloadSize, (int)cmdObj->payloadSize, (char *)cmdObj->payloadData);
}

void receiveStatusCommands( void *objRef, gCommandObject *cmdObj ) {
	ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 17, "Status reply here");
}

void setup() {
	DEFAULT_DEVICE_ID = TEST_COMMAND_ROUTER_APP_DEVICE_ID;
	DEFAULT_INSTANCE_ID = 1;

	pRouter = &myRouter;
	myRouter.setup();
	pMyObject = new SimpleObject();
	pMyObject->setup(7, &myRouter);  // actually includes an intended 1000 ms repeated request to "do something else"...
	startTimer = millis();
	printf( prompt );

	myRouter.AddCommandHandler( DEFAULT_DEVICE_ID, DEFAULT_INSTANCE_ID, 0, receiveCommands );  // default backstop
	myRouter.AddCommandHandler( GODZUKI_SENSOR_PLATFORM_DEVICE_ID, 1, 0, COMMAND_ID_GLOBAL_REQUEST_STATUS, receiveStatusCommands, -1 );
}

using namespace std;
int loop() {
	int DEVICE_ID = DEFAULT_DEVICE_ID;
	int instanceID = DEFAULT_INSTANCE_ID;

	char buffer[128];

	if (_kbhit())
	{
		cin.getline(buffer, sizeof(buffer));
		switch (buffer[0] )
		{
		case '1':
			printf( "Routing do something cmd\n" );
			ROUTE_COMMAND(SIMPLE_OBJECT_DEVICE_ID,7,SimpleObject::COMMAND_ID_DO_SOMETHING,100);
			break;
		case'2':
			printf( "Routing unknown cmd\n" );
			ROUTE_COMMAND(SIMPLE_OBJECT_DEVICE_ID,7,247,500);
			break;
		case'd':
			printf( "Dumping handler tree\n" );
			myRouter.DumpHandlerTree();
			break;
		case'r':
			printf( "Requesting status\n" );
			ROUTE_COMMAND(GODZUKI_SENSOR_PLATFORM_DEVICE_ID,1,COMMAND_ID_GLOBAL_REQUEST_STATUS,-1);
			break;
		case's':
			if( pMyObject == 0 ) {
				printf( "Setup object\n" );
				pMyObject = new SimpleObject();
				pMyObject->setup(7, &myRouter);  // actually includes an intended 1000 ms repeated request to "do something else"...
			}
			else
				printf( "Object already setup...\n" );
			break;
		case'x':
			printf( "Teardown object timer\n" );
			pMyObject->teardownTimer( &myRouter );
			break;
		case'X':
			printf( "Teardown object\n" );
			if( pMyObject != 0 ) {
				delete pMyObject;
				pMyObject = 0;
			}
			else
				printf( "Object already torn down...\n" );
			break;
		case'q':
			printf( "Quit app...\n" );
			return 1;	// not arduino compatible
		default:
			printf( "Don't know what you meant...\n" );
			break;
		}
		printf( prompt );
	}


	myRouter.ScanCommands();

	//std::chrono::milliseconds dura( 50 );
	//std::this_thread::sleep_for( dura );

	return 0;
}
