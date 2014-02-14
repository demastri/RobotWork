#ifndef ROUTE_TABLE_LIST_H
#define ROUTE_TABLE_LIST_H

typedef void cmdHandler( void *objRef, gCommandObject *cmdObj );

struct gRouteTableEntry {
public:
	gRouteTableEntry( int _deviceID, int _instanceID, void *_objRef, int _cmdID, cmdHandler _thisHandler, long timerVal, unsigned long now ) {
		deviceID = _deviceID;
		instanceID = _instanceID;
		objRef = _objRef;
		cmdID = _cmdID;
		thisHandler = _thisHandler;
		reTriggerInMills = timerVal;
		nextTrigger = (timerVal > 0 ? now + timerVal : -1);
	};
	gRouteTableEntry( int _deviceID, int _instanceID, void *_objRef, cmdHandler _thisHandler ) {
		deviceID = _deviceID;
		instanceID = _instanceID;
		objRef = _objRef;
		cmdID = -1;
		thisHandler = _thisHandler;
		reTriggerInMills = -1;
		nextTrigger = -1;
	};

	int deviceID;
	int instanceID;
	void *objRef;
	int cmdID;
	cmdHandler *thisHandler;
	long reTriggerInMills;
	long nextTrigger;

};

#endif