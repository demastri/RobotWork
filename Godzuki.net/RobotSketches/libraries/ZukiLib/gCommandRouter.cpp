#ifndef WIN32
#include <Arduino.h>
#endif


#include "gCommandRouter.h"

#include "gComms.h"
#ifdef WIN32
using namespace Godzuki;
#endif
extern gComms gMonitor;
extern unsigned long millis();

static int devicesNOTtoMonitor[] = {HEARTBEAT_DEVICE_ID, -1};
static int devicesNOTtoRoute[] = {-1, SERVO_DEVICE_ID, -1};

int DEFAULT_DEVICE_ID = -1;
int DEFAULT_INSTANCE_ID = -1;


static RouteTableList *listBase = 0;
static gCommandObject *commandList = 0;
static gCommandObject *commandResponses = 0;
//RouteTableList *gCommandRouter::listBase = 0;
//gCommandObject *gCommandRouter::commandList = 0;
//gCommandObject *gCommandRouter::commandResponses = 0;


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
		if( deviceID == curEntry->deviceID && instanceID == curEntry->instanceID && cmdID == curEntry->cmdID && timer == curEntry->reTriggerInMills) {
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
	HandleCommandResponses();
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
				//gMonitor.println( "Not Routing Command:" );
				commandList->print();
				return;
			}
		}

		RouteTableList *pHandler = FindRouteTable( commandList );
		if( pHandler != 0 ) {
			pHandler->thisHandler( pHandler->objRef, commandList );
		} else {
			// broadcast over network for remote handler
			if( commandList->cmdSrc == -1 ) {
#ifdef WIN32
				size_t thisSize;
				uint8_t *cmdStr = commandList->ToCommandString(&thisSize);
				commandList->commandID = COMMAND_ID_GLOBAL_BROADCAST;
				commandList->isReply = true;
				RouteReply( commandList, GLOBAL_COMMAND_STATUS_OK, gComms::strlen( (char *)cmdStr ), (void *)cmdStr );
#else
				gMonitor.BroadcastCommand( commandList, 0 );
				gMonitor.BroadcastCommand( commandList, 1 );
#endif
			}
			if( commandList->cmdSrc == 0 ) {
				gMonitor.BroadcastCommand( commandList, 1 );
			}
			if( commandList->cmdSrc == 1 ) {
				gMonitor.BroadcastCommand( commandList, 0 );
			}
		}
		DequeueCommand( &commandList, commandList );
	}
}
void gCommandRouter::HandleCommandResponses() {
	while( commandResponses != 0 ) {
		bool monitorMe = true;
		for( int *pi=devicesNOTtoMonitor; *pi>=0; pi++ )
			if( commandResponses->sourceDeviceID == *pi )
				monitorMe = false;
		for( int *pi=devicesNOTtoRoute; *pi>=0; pi++ ) {
			if( commandResponses->sourceDeviceID == *pi ) {
				//gMonitor.println( "Not Routing Command:" );
				commandResponses->print();
				return;
			}
		}

		RouteTableList *pHandler = FindRouteTable( commandResponses );
		if( pHandler != 0 ) {
			pHandler->thisHandler( pHandler->objRef, commandResponses );
		} else {
			// broadcast over network for remote target
			gMonitor.BroadcastCommand( commandResponses, 0 );
			gMonitor.BroadcastCommand( commandResponses, 1 );
		}
		DequeueCommand( &commandResponses, commandResponses );
	}
}

RouteTableList *gCommandRouter::FindRouteTable( gCommandObject *commandObj ) {
	RouteTableList *curEntry = listBase;
	RouteTableList *defEntry = 0;
	int handlerCount = 0;

	while( curEntry != 0 && ++ handlerCount < 50 ) {
		if( !commandObj->isReply && commandObj->targetDeviceID == curEntry->deviceID && commandObj->targetInstanceID == curEntry->instanceID && commandObj->commandID == curEntry->cmdID ) {
			return curEntry;
		}
		if( !commandObj->isReply && commandObj->targetDeviceID == curEntry->deviceID && commandObj->targetInstanceID == curEntry->instanceID && curEntry->cmdID == -1 ) {
			defEntry = curEntry;
		}
		if( commandObj->isReply && commandObj->sourceDeviceID == curEntry->deviceID && commandObj->sourceInstanceID == curEntry->instanceID && commandObj->commandID == curEntry->cmdID ) {
			return curEntry;
		}
		if( commandObj->isReply && commandObj->sourceDeviceID == curEntry->deviceID && commandObj->sourceInstanceID == curEntry->instanceID && curEntry->cmdID == -1 ) {
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
			QueueCommand( &commandList, new gCommandObject(-1, DEFAULT_DEVICE_ID, DEFAULT_INSTANCE_ID, curEntry->deviceID, curEntry->instanceID, curEntry->cmdID, now,0,0) );
			curEntry->nextTrigger += curEntry->reTriggerInMills;
		}
		curEntry = curEntry->nextEntry;
	}
}

void gCommandRouter::QueueCommand( gCommandObject **head, gCommandObject *objData ) {
	// add this to the end of the ringtailed command list
	if( *head == 0 ) {
		objData->nextEntry = objData->prevEntry = *head = objData;
	} else {
		objData->nextEntry = *head;
		objData->prevEntry = (*head)->prevEntry;
		(*head)->prevEntry->nextEntry = objData;
		(*head)->prevEntry = objData;
	}
}

void gCommandRouter::DequeueCommand( gCommandObject **head, gCommandObject *objData ) {
	// pop this off of the ringtailed command list
	if( objData->nextEntry == objData->prevEntry && objData->nextEntry == objData ) {
		// last one in the queue
		*head = 0;
	} else {
		objData->nextEntry->prevEntry = objData->prevEntry;
		objData->prevEntry->nextEntry = objData->nextEntry;
		*head = objData->nextEntry;
	}
	delete objData;
}

//void gCommandRouter::RouteCommand( int _deviceID, int _instanceID, int cmdID, long cmdParameter ) {
void gCommandRouter::RouteCommand( gCommandObject *objData ) {
	QueueCommand( &commandList, objData );
}
void gCommandRouter::RouteReply( gCommandObject *objData, unsigned char status, unsigned int rtnDataSize, void *rtnData ) {
	gCommandObject *respData = objData->InitReply( status, rtnDataSize, rtnData );
	QueueCommand( &commandResponses, respData );
}

void gCommandRouter::DumpHandlerTree() {
	RouteTableList *curEntry = listBase;
	int handlerCount = 0;

	long now = millis();
	while( curEntry != 0 && ++handlerCount < 50 ) {
		PrintRouteList(curEntry);
		curEntry = curEntry->nextEntry;
	}
}
char *breakStr = "> - <";
void gCommandRouter::PrintRouteList(RouteTableList *l) {
	gMonitor.print("Dump <");
	gMonitor.print( l->deviceID );
	gMonitor.print(breakStr);
	gMonitor.print( l->instanceID );
	gMonitor.print(breakStr);
	gMonitor.print( l->cmdID );
	gMonitor.print(breakStr);
	gMonitor.print( l->reTriggerInMills );
	gMonitor.println(">");

}

