#ifndef WIN32
#include <Arduino.h>
#endif

#include "gCommandRouter.h"

#ifndef WIN32
#include "gComms.h"
extern gComms gMonitor;
#endif

#ifdef WIN32
#include <stdio.h>
using namespace std;

extern unsigned long millis();
class winComms {
public:
	void println( char *s ) { printf( "%s\n", s ); }
	void print( char *s ) { printf( "%s", s  ); }
	void print( int i ) { printf( "%d", i ); }
};
static winComms gMonitor;
#endif

static int devicesNOTtoMonitor[] = {HEARTBEAT_DEVICE_ID, -1};
static int devicesNOTtoRoute[] = {-1, SERVO_DEVICE_ID, -1};

RouteTableList *gCommandRouter::listBase = 0;

gCommandRouter::gCommandRouter() {
}

void gCommandRouter::setup() {
}

void gCommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, cmdHandler thisHandler ) {
	RouteTableList *nextID = new RouteTableList( deviceID, instanceID, objRef, thisHandler );
	nextID->nextEntry = listBase;
	if( listBase != 0 )
		listBase->prevEntry = nextID;
	listBase = nextID;
}

void gCommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, int cmdID, cmdHandler thisHandler, long timer ) {
	RouteTableList *nextID = new RouteTableList( deviceID, instanceID, objRef, cmdID, thisHandler, timer, millis() );
	nextID->nextEntry = listBase;
	if( listBase != 0 )
		listBase->prevEntry = nextID;
	listBase = nextID;
}

void gCommandRouter::RemoveCommandHandler( int deviceID, int instanceID ) {
	RouteTableList *curEntry = listBase;

	while( curEntry != 0 ) {
		if( deviceID == curEntry->deviceID && instanceID == curEntry->instanceID && curEntry->cmdID == -1) {
			if( curEntry->prevEntry == 0 ) // first one
				listBase = curEntry->nextEntry;
			else
				curEntry->prevEntry->nextEntry = curEntry->nextEntry;

			if( curEntry->nextEntry != 0 ) // last one
				curEntry->nextEntry->prevEntry = curEntry->prevEntry;

			RouteTableList *delEntry = curEntry;
			curEntry = curEntry->nextEntry;
			delete delEntry;      
		}
		else
			curEntry = curEntry->nextEntry;
	}
}

void gCommandRouter::RemoveCommandHandler( int deviceID, int instanceID, int cmdID, long timer ) {
	RouteTableList *curEntry = listBase;
	RouteTableList *defEntry = 0;

	while( curEntry != 0 ) {
		if( deviceID == curEntry->deviceID && instanceID == curEntry->instanceID && cmdID == curEntry->cmdID) {
			if( curEntry->prevEntry == 0 ) // first one
				listBase = curEntry->nextEntry;
			else
				curEntry->prevEntry->nextEntry = curEntry->nextEntry;

			if( curEntry->nextEntry != 0 ) // last one
				curEntry->nextEntry->prevEntry = curEntry->prevEntry;
			RouteTableList *delEntry = curEntry;
			curEntry = curEntry->nextEntry;
			delete delEntry;      
		}
		else
			curEntry = curEntry->nextEntry;
	}
}

void gCommandRouter::ScanCommands() {
	RouteTableList *curEntry = listBase;

	long now = millis();
	while( curEntry != 0 ) {
		if( curEntry->reTriggerInMills > 0 && curEntry->nextTrigger <= now ) {
			RouteCommand( gCommandObject(curEntry->deviceID, curEntry->instanceID, curEntry->cmdID, now) );
			curEntry->nextTrigger += curEntry->reTriggerInMills;
		}
		curEntry = curEntry->nextEntry;
	}
}

//void gCommandRouter::RouteCommand( int _deviceID, int _instanceID, int cmdID, long cmdParameter ) {
gCommandObject *gCommandRouter::RouteCommand( gCommandObject objData ) {
	int _deviceID = objData.targetDeviceID;
	int _instanceID = objData.targetInstanceID;
	int cmdID = objData.commandID;
	long cmdParameter = objData.parameter;

	bool monitorMe = true;

	for( int *pi=devicesNOTtoMonitor; *pi>=0; pi++ )
		if( _deviceID == *pi )
			monitorMe = false;

	for( int *pi=devicesNOTtoRoute; *pi>=0; pi++ )
		if( _deviceID == *pi ) {
			objData.print();
			return 0;
		}

	RouteTableList *curEntry = listBase;
	RouteTableList *defEntry = 0;
	bool hadHandler = false;

	while( curEntry != 0 ) {
		if( _deviceID == curEntry->deviceID && _instanceID == curEntry->instanceID && cmdID == curEntry->cmdID ) {
			if( monitorMe ) {
				gMonitor.println( "Cmd (about to be) handled explicitly" );
				objData.print();
			}
			curEntry->thisHandler( curEntry->objRef, cmdID, cmdParameter );
			hadHandler = true;    
		}
		if( _deviceID == curEntry->deviceID && _instanceID == curEntry->instanceID ) {
			defEntry = curEntry;
		}
		curEntry = curEntry->nextEntry;
	}
	if( defEntry && !hadHandler ) {
		if( monitorMe ) {
			gMonitor.println( "Cmd (about to be) handled by default" );
			objData.print();
		}
		defEntry->thisHandler( defEntry->objRef, cmdID, cmdParameter );
	}
	if( !defEntry && !hadHandler ) {
		gMonitor.println( "I had nowhere to send this command..." );
		objData.print();
	}
	return 0;
}

