#ifndef COMMAND_ROUTER_H
#define COMMAND_ROUTER_H

#include "gRoutingDeviceIDs.h"
#include "gCommandObject.h"
#include "gRouteTableEntry.h"

#define CMD_METHOD_DEFINE(M)                static void M##Proxy(void *thisObj, gCommandObject *cmdObj ); void M##Actually( gCommandObject *cmdObj )
#define CMD_METHOD_IMPLEMENT(C,M)           void C::M##Proxy(void *thisObj, gCommandObject *cmdObj ) { ((C *)thisObj)->M##Actually( cmdObj ); } void C::M##Actually(gCommandObject *cmdObj)

#define CMD_METHOD_REGISTER(Z,X,M)          router.AddCommandHandler( DEVICE_ID, instanceID, this, X,  (&Z::M##Proxy), -1 )
#define CMD_METHOD_REGISTER_TIMER(Z,X,M,T)  router.AddCommandHandler( DEVICE_ID, instanceID, this, X,  (&Z::M##Proxy), T )
#define CMD_METHOD_REGISTER_DEFAULT(Z,M)    router.AddCommandHandler( DEVICE_ID, instanceID, this, (&Z::M##Proxy) )

#define CMD_METHOD_DEREGISTER(X)            router.RemoveCommandHandler( DEVICE_ID, instanceID, X, -1 )
#define CMD_METHOD_DEREGISTER_TIMER(X,T)    router.RemoveCommandHandler( DEVICE_ID, instanceID, X, T )
#define CMD_METHOD_DEREGISTER_DEFAULT()     router.RemoveCommandHandler( DEVICE_ID, instanceID )

#define CMD_METHOD_DEREGISTER_ALL()			pRouter->RemoveAllCommandHandlers( DEVICE_ID, instanceID )

#define ROUTE_COMMAND(X,Y,Z,A)				pRouter->RouteCommand( gCommandObject::gCommandObjectFactory(-1, DEVICE_ID, instanceID, X, Y, Z, A, 0, 0 ) )
#define ROUTE_REPLY(X,S,P)					pRouter->RouteReply( cmdObj, X, S, P )

static const int ROUTER_NO_COMMAND       = 999;

class gCommandRouter {
public:
	gCommandRouter();
	void setup();
	void AddCommandHandler( int deviceID, int instanceID, void *objRef, cmdHandler thisHandler );  // default backstop
	void AddCommandHandler( int deviceID, int instanceID, void *objRef, int cmdID, cmdHandler thisHandler, long timer );

	void RemoveAllCommandHandlers( int deviceID, int instanceID );  // catchall backstop
	void RemoveCommandHandler( int deviceID, int instanceID );  // default backstop
	void RemoveCommandHandler( int deviceID, int instanceID, int cmdID, long timer );

	void ScanCommands();
	void RouteCommand( gCommandObject *objData );
	void RouteReply( gCommandObject *objData, unsigned char status, unsigned int rtnDataSize, void *rtnData );
	
	void DumpHandlerTree();
	void PrintRouteList(gRouteTableEntry *l);

private:
	void GenerateTimedCommands();
	void HandleCommandQueue();
	void HandleCommandResponses();

	gRouteTableEntry *FindRouteTable( gCommandObject *commandObj );

};

#endif
