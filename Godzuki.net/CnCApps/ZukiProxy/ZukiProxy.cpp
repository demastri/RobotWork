// This is the main DLL file.

#include "stdafx.h"

#include "ZukiProxy.h"

using namespace ZukiProxy;

gCommandRouter myRouter;
extern gComms gMonitor;


void SensorBot::HelloWorld() { 
	System::Console::WriteLine("Hello, world");
}
void SensorBot::setup() { 
	myRouter.setup();
	gMonitor.setup();

	myRouter.AddCommandHandler( 500,1,0,-1,receiveCommands,-1 );
}
void SensorBot::routeCommand(int cmdID) {
	gCommandObject *newObj = new gCommandObject( 500,1,cmdID );
	myRouter.RouteCommand( newObj );
}
void SensorBot::loop() {
	myRouter.ScanCommands();
}
void SensorBot::setClientHandler(ResponseHandler^ clientHandler) {
	// replace any existing handler with the one provided by the actual client...
	setHandler = clientHandler;
}

void ZukiProxy::receiveCommands( void *objRef, gCommandObject *cmdObj ) {
	// ok - when the comms subsystem responds with a command or response, 
	// this is where it should eventually get routed
	SensorBot::setHandler();
}
