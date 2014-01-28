///
/// Input Router main loop - initially brought over from arduino IDE
///

#include "stdafx.h"

extern unsigned long millis();
#include <conio.h>
#include <iostream>

#include <chrono>
#include <thread>

#include "../../RobotSketches/godzuki/gComms.h"
gComms gMonitor;

#include "SimpleObject.h"
gCommandRouter myRouter;
gCommandRouter *pRouter=0;
SimpleObject *pMyObject;

long startTimer;
char *prompt = " 1 - Route Something Cmd\n 2 - Route N/A cmd\n d - dump handler tree\n s - setup obj\n x - teardown timer handler\n X - teardown obj\n q - quit\nCommand:";

extern int DEFAULT_DEVICE_ID = TEST_COMMAND_ROUTER_APP_DEVICE_ID;
extern int DEFAULT_INSTANCE_ID = 1;

void receiveCommands( void *objRef, gCommandObject *cmdObj ) {
	printf( "\nReceived a command response...\nexpecting a string got\n status <%d>\n size <%d>\n data <%s>\n", (int)cmdObj->rtnStatus, (int)cmdObj->payloadSize, (char *)cmdObj->payloadData);
}

void setup() {
	pRouter = &myRouter;
	myRouter.setup();
	pMyObject = new SimpleObject();
	pMyObject->setup(7, &myRouter);  // actually includes an intended 1000 ms repeated request to "do something else"...
	startTimer = millis();
	printf( prompt );

	myRouter.AddCommandHandler( DEFAULT_DEVICE_ID, DEFAULT_INSTANCE_ID, 0, receiveCommands );  // default backstop
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
			//pRouter->RouteCommand( gCommandObject(DEVICE_ID, instanceID, X, Y, Z, A, 0, 0 ) )
			//gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData );
			//myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, SimpleObject::COMMAND_ID_DO_SOMETHING, 100 );
			break;
		case'2':
			printf( "Routing unknown cmd\n" );
			ROUTE_COMMAND(SIMPLE_OBJECT_DEVICE_ID,7,247,500);
			//myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, -1, 500 );
			break;
		case'd':
			printf( "Dumping handler tree\n" );
			myRouter.DumpHandlerTree();
			//myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, -1, 500 );
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
			printf( "Teardown all handlers\n" );
			delete pMyObject;
			pMyObject = 0;
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

	std::chrono::milliseconds dura( 50 );
	std::this_thread::sleep_for( dura );

	return 0;
}
