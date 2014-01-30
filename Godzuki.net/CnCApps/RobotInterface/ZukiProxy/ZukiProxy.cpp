// This is the main DLL file.

#include "stdafx.h"

#include "ZukiProxy.h"

using namespace Godzuki;

gCommandRouter myRouter;
extern gComms gMonitor;

void SensorBot::HelloWorld() { 
	System::Console::WriteLine("Hello, world");
}
void SensorBot::setup() { 
	myRouter.setup();
	gMonitor.setup();

	myRouter.AddCommandHandler( 99,1,0,1,receiveCommands,-1 );
	myRouter.AddCommandHandler( 50,1,0,-1,receiveCommands,-1 );
}
void SensorBot::routeCommand(int cmdID) {
	gCommandObject *newObj;
	if( cmdID == 1 )
		newObj = new gCommandObject(99,1,99,1,cmdID );
	else if( cmdID == 2 )
		newObj = new gCommandObject(50,1,99,1,cmdID );
	else if( cmdID == 3 )
		newObj = new gCommandObject( 50,1,3,1,2 );	// should sweep once...
	myRouter.RouteCommand( newObj );
}
void SensorBot::loop() {
	myRouter.ScanCommands();
}
void SensorBot::setClientHandler(ResponseHandler^ clientHandler) {
	// replace any existing handler with the one provided by the actual client...
	setHandler = clientHandler;
}

void Godzuki::receiveCommands( void *objRef, gCommandObject *cmdObj ) {
	// ok - when the comms subsystem responds with a command or response, 
	// this is where it should eventually get routed
	String^ s = gcnew String( (char *)cmdObj->payloadData );
	SensorBot::setHandler( cmdObj->commandID, cmdObj->rtnStatus, cmdObj->payloadSize, s );
}
