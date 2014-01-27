#ifndef ROUTE_TABLE_LIST_H
#define ROUTE_TABLE_LIST_H

struct RouteTableList {
public:
	RouteTableList( int _deviceID, int _instanceID, void *_objRef, int _cmdID, cmdHandler _thisHandler, long timerVal, long now ) {
		deviceID = _deviceID;
		instanceID = _instanceID;
		objRef = _objRef;
		cmdID = _cmdID;
		thisHandler = _thisHandler;
		reTriggerInMills = timerVal;
		nextTrigger = (timerVal > 0 ? (now + timerVal) : -1);
		nextEntry = prevEntry = 0;
	};
	RouteTableList( int _deviceID, int _instanceID, void *_objRef, cmdHandler _thisHandler ) {
		deviceID = _deviceID;
		instanceID = _instanceID;
		objRef = _objRef;
		cmdID = -1;
		thisHandler = _thisHandler;
		reTriggerInMills = -1;
		nextTrigger = -1;
		nextEntry = prevEntry = 0;
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

#endif