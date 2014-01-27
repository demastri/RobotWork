#include <Arduino.h>
#include "gCommandObject.h"

#include "gComms.h"
extern gComms gMonitor;

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	Init( srcdev, srcinst, dev, inst, cmd, param, paySize, payData );
}

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd ) {
	Init( srcdev, srcinst, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd, int param ) {
	Init( -1, -1, dev, inst, cmd, param, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd ) {
	Init( -1, -1, dev, inst, cmd, -1, 0, 0 );
}

void gCommandObject::Init( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	sourceDeviceID = srcdev;
	sourceInstanceID = srcinst;
	targetDeviceID = dev;
	targetInstanceID = inst;
	commandID = cmd;
	parameter = param;
	payloadSize = paySize;
	payloadData = payData;
}

void gCommandObject::print() {
	gMonitor.print("Dumping cmd object... <");
	gMonitor.print( targetDeviceID );
	gMonitor.print("> - <");
	gMonitor.print( targetInstanceID);
	gMonitor.print("> - <");
	gMonitor.print( commandID );
	gMonitor.print("> - <");
	gMonitor.print( parameter );
	gMonitor.print("> - ");
#if 0 
	if( cmdHandler == 0 )
		gMonitor.println("had some kind of handler...");
	else
		gMonitor.println("ruh roh");
#endif
}