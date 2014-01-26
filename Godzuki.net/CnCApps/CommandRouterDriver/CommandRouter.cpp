#include "stdafx.h"
#include "CommandRouter.h"
extern unsigned long millis();
struct RouteTableList {
public:
  RouteTableList( int _deviceID, int _instanceID, void *_objRef, int _cmdID, cmdHandler _thisHandler, long timerVal ) {
    deviceID = _deviceID;
    instanceID = _instanceID;
    objRef = _objRef;
    cmdID = _cmdID;
    thisHandler = _thisHandler;
    reTriggerInMills = timerVal;
    nextTrigger = (timerVal > 0 ? millis() + timerVal : -1);
    nextEntry = prevEntry = 0;
  };
  RouteTableList( int _deviceID, int _instanceID, void *_objRef, cmdHandler _thisHandler ) {
    deviceID = _deviceID;
    instanceID = _instanceID;
    objRef = _objRef;
    cmdID = -1;
    thisHandler = _thisHandler;
    nextEntry = prevEntry = 0;
    reTriggerInMills = -1;
    nextTrigger = -1;
  };

  RouteTableList *nextEntry;
  RouteTableList *prevEntry;

  int deviceID;
  int instanceID;
  void *objRef;
  int cmdID;
  cmdHandler *thisHandler;
  long reTriggerInMills;
  long nextTrigger;
};

RouteTableList *listBase = 0;

CommandRouter::CommandRouter() {
}

void CommandRouter::setup() {
}

void CommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, cmdHandler thisHandler ) {
  RouteTableList *nextID = new RouteTableList( deviceID, instanceID, objRef, thisHandler );
  nextID->nextEntry = listBase;
  if( listBase != 0 )
	  listBase->prevEntry = nextID;
  listBase = nextID;
}

void CommandRouter::AddCommandHandler( int deviceID, int instanceID, void *objRef, int cmdID, cmdHandler thisHandler, long timer ) {
  RouteTableList *nextID = new RouteTableList( deviceID, instanceID, objRef, cmdID, thisHandler, timer );
  nextID->nextEntry = listBase;
  if( listBase != 0 )
	listBase->prevEntry = nextID;
  listBase = nextID;
}

void CommandRouter::RemoveCommandHandler( int deviceID, int instanceID ) {
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

void CommandRouter::RemoveCommandHandler( int deviceID, int instanceID, int cmdID, long timer ) {
  RouteTableList *curEntry = listBase;
  
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

void CommandRouter::ScanCommands() {
  RouteTableList *curEntry = listBase;
  
  long now = millis();
  while( curEntry != 0 ) {
    if( curEntry->reTriggerInMills > 0 && curEntry->nextTrigger <= now ) {
      RouteCommand( curEntry->deviceID, curEntry->instanceID, curEntry->cmdID, now );
      curEntry->nextTrigger += curEntry->reTriggerInMills;
    }
	curEntry = curEntry->nextEntry;
  }
}

void CommandRouter::RouteCommand( int _deviceID, int _instanceID, int cmdID, long cmdParameter ) {
  RouteTableList *curEntry = listBase;
  RouteTableList *defEntry = 0;
  bool hadHandler = false;
  
  while( curEntry != 0 ) {
    if( _deviceID == curEntry->deviceID && _instanceID == curEntry->instanceID && cmdID == curEntry->cmdID ) {
      curEntry->thisHandler( curEntry->objRef, cmdID, cmdParameter );
      hadHandler = true;    
    }
    if( _deviceID == curEntry->deviceID && _instanceID == curEntry->instanceID ) {
      defEntry = curEntry;
    }
    curEntry = curEntry->nextEntry;
  }
  if( defEntry && !hadHandler )
    defEntry->thisHandler( defEntry->objRef, cmdID, cmdParameter );
  if( !defEntry && !hadHandler )
    printf( "I had nowhere to send this command...\n" );
}

