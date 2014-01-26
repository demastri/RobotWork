#include "CommandRouter.h"
CommandRouter myRouter;

#include "SimpleObject.h"
SimpleObject myObject;

long startTimer;
bool didCmds = false;

void setup() {
	myRouter.setup();
	myObject.setup(7, &myRouter);  // actually includes an intended 1000 ms repeated request to "do something else"...
	startTimer = millis();
}

void loop() {
	Serial.println( "routing a command..." );

	if( millis() - startTimer > 10000 && !didCmds) {
		didCmds = true;
		myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, SimpleObject::COMMAND_ID_DO_SOMETHING, 100 );
		myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, -1, 500 );
	}

	Serial.println( "scanning commands..." );
	myRouter.ScanCommands();
	delay(100);

	if( millis() - startTimer > 30000 ) {
		myObject.teardown( &myRouter );
		myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, SimpleObject::COMMAND_ID_DO_SOMETHING, 100 );
		myRouter.RouteCommand( SimpleObject::DEVICE_ID, 7, -1, 500 );
	}
}