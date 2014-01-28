#ifndef ROUTE_TABLE_LIST_H
#define ROUTE_TABLE_LIST_H

typedef void cmdHandler( void *objRef, int commandID, int parameter );

#include "gComms.h"
extern gComms gMonitor;

struct RouteTableList {
public:
	RouteTableList( int _deviceID, int _instanceID, void *_objRef, int _cmdID, cmdHandler _thisHandler, long timerVal, unsigned long now ) {
		deviceID = _deviceID;
		instanceID = _instanceID;
		objRef = _objRef;
		cmdID = _cmdID;
		thisHandler = _thisHandler;
		reTriggerInMills = timerVal;
		nextTrigger = (timerVal > 0 ? now + timerVal : -1);
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

	void print() {
		gMonitor.print("Dumping cmd handler object... <");
		gMonitor.print( deviceID );
		gMonitor.print("> - <");
		gMonitor.print( instanceID );
		gMonitor.print("> - <");
		gMonitor.print( cmdID );
		gMonitor.print("> - <");
		gMonitor.print( reTriggerInMills );
		gMonitor.println(">");

	}
};

#endif