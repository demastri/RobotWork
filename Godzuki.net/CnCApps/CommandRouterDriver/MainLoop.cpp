///
/// Input Router main loop - initially brought over from arduino IDE
///

#include "stdafx.h"

extern unsigned long millis();
#include <conio.h>
#include <iostream>

#include <chrono>
#include <thread>

#include "CommandRouter.h"
CommandRouter myRouter;

#include "SimpleObject.h"
SimpleObject myObject;

long startTimer;

void setup() {
	myRouter.setup();
	myObject.setup(7, &myRouter);  // actually includes an intended 1000 ms repeated request to "do something else"...
	startTimer = millis();
}

int loop() {
using namespace std;

	char buffer[128];

	if (_kbhit())
	{
		cin.getline(buffer, sizeof(buffer));
		switch (buffer[0] )
		{
		case '1':
			myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, SimpleObject::COMMAND_ID_DO_SOMETHING, 100 );
			break;
		case'2':
			myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, -1, 500 );
			break;
		case's':
			myObject.setup( 7, &myRouter );
			break;
		case'x':
			//if( millis() - startTimer > 30000 )
			myObject.teardown( &myRouter );
			break;
		case'q':
			return 1;	// not arduino compatible
		default:
			break;
		}
	}


	myRouter.ScanCommands();

	std::chrono::milliseconds dura( 50 );
	std::this_thread::sleep_for( dura );

	return 0;
}
