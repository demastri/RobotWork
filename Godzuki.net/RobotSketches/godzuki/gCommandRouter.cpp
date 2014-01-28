#ifndef WIN32
#include <Arduino.h>
#endif

#include "gCommandRouter.h"

#include "gComms.h"
extern gComms gMonitor;
extern unsigned long millis();

static int devicesNOTtoMonitor[] = {HEARTBEAT_DEVICE_ID, -1};
static int devicesNOTtoRoute[] = {-1, SERVO_DEVICE_ID, -1};

RouteTableList *gCommandRouter::listBase = 0;

gCommandRouter::gCommandRouter() {
	commandList = 0;
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

void gCommandRouter::RemoveAllCommandHandlers( int deviceID, int instanceID ) {
	RouteTableList *curEntry = listBase;

	while( curEntry != 0 ) {
		if( deviceID == curEntry->deviceID && instanceID == curEntry->instanceID ) {
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
	HandleTimedCommands();
	ExecuteCommandQueue();
}

void gCommandRouter::ExecuteCommandQueue() {

	while( commandList != 0 ) {

		bool monitorMe = true;
		for( int *pi=devicesNOTtoMonitor; *pi>=0; pi++ )
			if( commandList->targetDeviceID == *pi )
				monitorMe = false;
		for( int *pi=devicesNOTtoRoute; *pi>=0; pi++ ) {
			if( commandList->targetDeviceID == *pi ) {
				gMonitor.println( "Not Routing Command:" );
				commandList->print();
				return;
			}
		}

		RouteTableList *pHandler = FindRouteTable( commandList );
		if( pHandler != 0 ) {
			pHandler->thisHandler( pHandler->objRef, commandList->commandID, commandList->parameter );
		} else {
			gMonitor.println( "I had nowhere to send this command..." );
			commandList->print();
		}
		DequeueCommand( commandList );
	}
}

RouteTableList *gCommandRouter::FindRouteTable( gCommandObject *commandObj ) {
	RouteTableList *curEntry = listBase;
	RouteTableList *defEntry = 0;
	int handlerCount = 0;

	while( curEntry != 0 && ++ handlerCount < 50 ) {
		if( commandObj->targetDeviceID == curEntry->deviceID && commandObj->targetInstanceID == curEntry->instanceID && commandObj->commandID == curEntry->cmdID ) {
			return curEntry;
		}
		if( commandObj->targetDeviceID == curEntry->deviceID && commandObj->targetInstanceID == curEntry->instanceID ) {
			defEntry = curEntry;
		}
		curEntry = curEntry->nextEntry;
	}
	return defEntry;
}

void gCommandRouter::HandleTimedCommands() {
	RouteTableList *curEntry = listBase;
	int handlerCount = 0;

	long now = millis();
	while( curEntry != 0 && ++handlerCount < 50 ) {
		if( curEntry->reTriggerInMills > 0 && curEntry->nextTrigger <= now ) {
			// add timed command to the list for execution
			QueueCommand( new gCommandObject(curEntry->deviceID, curEntry->instanceID, curEntry->cmdID, now) );
			curEntry->nextTrigger += curEntry->reTriggerInMills;
		}
		curEntry = curEntry->nextEntry;
	}
}

void gCommandRouter::QueueCommand( gCommandObject *objData ) {
	// add this to the end of the ringtailed command list
	if( commandList == 0 ) {
		objData->nextEntry = objData->prevEntry = commandList = objData;
	} else {
		objData->nextEntry = commandList;
		objData->prevEntry = commandList->prevEntry;
		commandList->prevEntry->nextEntry = objData;
		commandList->prevEntry = objData;
	}
}

void gCommandRouter::DequeueCommand( gCommandObject *objData ) {
	// pop this off of the ringtailed command list
	if( objData->nextEntry == objData->prevEntry && objData->nextEntry == objData ) {
		// last one in the queue
		commandList = 0;
	} else {
		objData->nextEntry->prevEntry = objData->prevEntry;
		objData->prevEntry->nextEntry = objData->nextEntry;
		commandList = objData->nextEntry;
	}
	delete objData;
}

//void gCommandRouter::RouteCommand( int _deviceID, int _instanceID, int cmdID, long cmdParameter ) {
gCommandObject *gCommandRouter::RouteCommand( gCommandObject *objData ) {
	QueueCommand( objData );
	return 0;
}

void gCommandRouter::DumpHandlerTree() {
	RouteTableList *curEntry = listBase;
	int handlerCount = 0;

	long now = millis();
	while( curEntry != 0 && ++handlerCount < 50 ) {
		curEntry->print();
		curEntry = curEntry->nextEntry;
	}
}

