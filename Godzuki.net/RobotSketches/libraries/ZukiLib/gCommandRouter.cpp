#ifndef WIN32
#include <Arduino.h>
#endif

#include "gCommandRouter.h"
#include "gListNode.h"

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


static gList handlerList;
static gList commandList;
static gList commandResponseList;

gCommandRouter::gCommandRouter() {
}

void gCommandRouter::setup() {
}

void gCommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, cmdHandler thisHandler ) {
	gRouteTableEntry *nextID = new gRouteTableEntry( deviceID, instanceID, objRef, thisHandler );
	handlerList.Add( nextID );
}

void gCommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, int cmdID, cmdHandler thisHandler, long timer ) {
	gRouteTableEntry *nextID = new gRouteTableEntry( deviceID, instanceID, objRef, cmdID, thisHandler, timer, millis() );
	handlerList.Add( nextID );
}

void gCommandRouter::RemoveAllCommandHandlers( int deviceID, int instanceID ) 
{
	while( !handlerList.isEmpty() ) {
		delete (gRouteTableEntry *)handlerList.PopFirst();
	}
}

void gCommandRouter::RemoveCommandHandler( int deviceID, int instanceID ) 
{
	RemoveCommandHandler( deviceID, instanceID, -1, -1 );
}

void gCommandRouter::RemoveCommandHandler( int deviceID, int instanceID, int cmdID, long timer ) {
	for( gListNode *curNode=handlerList.First(); curNode != 0; curNode=handlerList.Next(curNode) ) {
		gRouteTableEntry *curEntry = (gRouteTableEntry *)curNode->GetObject();
		if( deviceID == curEntry->deviceID && instanceID == curEntry->instanceID && 
			(cmdID == -1 || cmdID == curEntry->cmdID) && (timer == -1 || timer == curEntry->reTriggerInMills) ) {
			handlerList.RemoveNode(curNode);
			delete curEntry;
			return;
		}
	}
}

void gCommandRouter::ScanCommands() {
	HandleTimedCommands();
	HandleCommandResponses();
	ExecuteCommandQueue();
}

void gCommandRouter::ExecuteCommandQueue() {

	while( !commandList.isEmpty() ) {
		gCommandObject *cmd = (gCommandObject *)commandList.PopFirst();
		bool monitorMe = true;
		for( int *pi=devicesNOTtoMonitor; *pi>=0; pi++ )
			if( cmd->targetDeviceID == *pi )
				monitorMe = false;
		for( int *pi=devicesNOTtoRoute; *pi>=0; pi++ ) {
			if( cmd->targetDeviceID == *pi ) {
				//gMonitor.println( "Not Routing Command:" );
				cmd->print();
				return;
			}
		}

		gRouteTableEntry *pHandler = FindRouteTable( cmd );
		if( pHandler != 0 ) {
			pHandler->thisHandler( pHandler->objRef, cmd );
		} else {
			// broadcast over network for remote handler
			if( cmd->cmdSrc == -1 ) {
#ifdef WIN32
				size_t thisSize;
				uint8_t *cmdStr = cmd->ToCommandString(&thisSize);
				cmd->commandID = COMMAND_ID_GLOBAL_BROADCAST;
				cmd->isReply = true;
				RouteReply( cmd, GLOBAL_COMMAND_STATUS_OK, gComms::strlen( (char *)cmdStr ), (void *)cmdStr );
#else
				gMonitor.BroadcastCommand( cmd, 0 );
				gMonitor.BroadcastCommand( cmd, 1 );
#endif
			}
			if( cmd->cmdSrc == 0 ) {
				gMonitor.BroadcastCommand( cmd, 1 );
			}
			if( cmd->cmdSrc == 1 ) {
				gMonitor.BroadcastCommand( cmd, 0 );
			}
		}
		if( !cmd->isReply )
			delete cmd;
	}
}
void gCommandRouter::HandleCommandResponses() {
	while( !commandResponseList.isEmpty() ) {
		gCommandObject *resp = (gCommandObject *)commandResponseList.PopFirst();
		bool monitorMe = true;
		for( int *pi=devicesNOTtoMonitor; *pi>=0; pi++ )
			if( resp->sourceDeviceID == *pi )
				monitorMe = false;
		for( int *pi=devicesNOTtoRoute; *pi>=0; pi++ ) {
			if( resp->sourceDeviceID == *pi ) {
				//gMonitor.println( "Not Routing Command:" );
				resp->print();
				return;
			}
		}

		gRouteTableEntry *pHandler = FindRouteTable( resp );
		if( pHandler != 0 ) {
			pHandler->thisHandler( pHandler->objRef, resp );
		} else {
			// broadcast over network for remote target
			//gMonitor.BroadcastCommand( commandResponses, 0 );
			gMonitor.BroadcastCommand( resp, 1 );
		}
		delete resp;
	}
}

gRouteTableEntry *gCommandRouter::FindRouteTable( gCommandObject *commandObj ) {
	gRouteTableEntry *defEntry = 0;
	for( gListNode *curNode=handlerList.First(); curNode != 0; curNode=handlerList.Next(curNode) ) {
		gRouteTableEntry *curEntry = (gRouteTableEntry *)curNode->GetObject();
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
	}
	return defEntry;
}

void gCommandRouter::HandleTimedCommands() {
	long now = millis();
	for( gListNode *curNode=handlerList.First(); curNode != 0; curNode=handlerList.Next(curNode) ) {
		gRouteTableEntry *curEntry = (gRouteTableEntry *)curNode->GetObject();
		if( curEntry->reTriggerInMills > 0 && curEntry->nextTrigger <= now ) {
			// add timed command to the list for execution
			commandList.Add( new gCommandObject(-1, DEFAULT_DEVICE_ID, DEFAULT_INSTANCE_ID, curEntry->deviceID, curEntry->instanceID, curEntry->cmdID, now,0,0) );
			curEntry->nextTrigger += curEntry->reTriggerInMills;
		}
	}
}


//void gCommandRouter::RouteCommand( int _deviceID, int _instanceID, int cmdID, long cmdParameter ) {
void gCommandRouter::RouteCommand( gCommandObject *objData ) {
	commandList.Add( objData );
}
void gCommandRouter::RouteReply( gCommandObject *objData, unsigned char status, unsigned int rtnDataSize, void *rtnData ) {
	gCommandObject *respData = objData->InitReply( status, rtnDataSize, rtnData );
	commandResponseList.Add( respData );
}

void gCommandRouter::DumpHandlerTree() {
	for( gListNode *curNode=handlerList.First(); curNode != 0; curNode=handlerList.Next(curNode) ) {
		gRouteTableEntry *curEntry = (gRouteTableEntry *)curNode->GetObject();
		PrintRouteList(curEntry);
	}
}
char *breakStr = "> - <";
void gCommandRouter::PrintRouteList(gRouteTableEntry *l) {
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

