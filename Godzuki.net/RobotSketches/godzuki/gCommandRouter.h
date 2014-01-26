#ifndef COMMAND_ROUTER_H
#define COMMAND_ROUTER_H

#include "gRoutingDeviceIDs.h"
#include "gCommandObject.h"

typedef void cmdHandler( void *objRef, int commandID, int parameter );

#define CMD_METHOD_DEFINE(M)                static void M##Proxy(void *thisObj, int commandID, int paramID ); void M##Actually( int commandID, int paramID )
#define CMD_METHOD_IMPLEMENT(C,M)           void C::M##Proxy(void *thisObj, int commandID, int paramID ) { ((C *)thisObj)->M##Actually( commandID, paramID); } void C::M##Actually(int commandID, int paramID )

#define CMD_METHOD_REGISTER(Z,X,M)          router.AddCommandHandler( DEVICE_ID, instanceID, this, X,  (&Z::M##Proxy), -1 )
#define CMD_METHOD_REGISTER_TIMER(Z,X,M,T)  router.AddCommandHandler( DEVICE_ID, instanceID, this, X,  (&Z::M##Proxy), T )
#define CMD_METHOD_REGISTER_DEFAULT(Z,M)    router.AddCommandHandler( DEVICE_ID, instanceID, this, (&Z::M##Proxy) )

#define CMD_METHOD_DEREGISTER(X)            router.RemoveCommandHandler( DEVICE_ID, instanceID, X, -1 )
#define CMD_METHOD_DEREGISTER_TIMER(X,T)    router.RemoveCommandHandler( DEVICE_ID, instanceID, X, T )
#define CMD_METHOD_DEREGISTER_DEFAULT()     router.RemoveCommandHandler( DEVICE_ID, instanceID )

#define ROUTE_COMMAND(X,Y,Z,A)				pRouter->RouteCommand( gCommandObject(DEVICE_ID, instanceID, X, Y, Z, A, 0, 0 ) )

static const int ROUTER_NO_COMMAND       = 999;

class gCommandRouter {
public:
  gCommandRouter();
  void setup();
  void AddCommandHandler( int deviceID, int instanceID, void *objRef, cmdHandler thisHandler );  // default backstop
  void AddCommandHandler( int deviceID, int instanceID, void *objRef, int cmdID, cmdHandler thisHandler, long timer );

  void RemoveCommandHandler( int deviceID, int instanceID );  // default backstop
  void RemoveCommandHandler( int deviceID, int instanceID, int cmdID, long timer );
  
  void ScanCommands();
  gCommandObject *RouteCommand( gCommandObject objData );
};

#endif
