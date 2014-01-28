#include "stdafx.h"
#include "SimpleObject.h"

SimpleObject::SimpleObject() {
	pRouter = 0;
	instanceID = -1;
}

void SimpleObject::setup(int thisID, gCommandRouter *router ) {
	instanceID = thisID;
	if( (pRouter=router) != 0 )
		setupCommandListener( *router );
}
void SimpleObject::teardown(gCommandRouter *router) {
	if( (pRouter=router) != 0 )
		teardownCommandListener( *router );
}
void SimpleObject::teardownTimer(gCommandRouter *pRouter) {
	if( pRouter != 0 ) {
		gCommandRouter &router = *pRouter;
		CMD_METHOD_DEREGISTER_TIMER(COMMAND_ID_DO_SOMETHING_ELSE, 3000);
	}
}

void SimpleObject::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(SimpleObject, processCommand);
	CMD_METHOD_REGISTER(SimpleObject, COMMAND_ID_DO_SOMETHING,      doSomething);
	//CMD_METHOD_REGISTER(SimpleObject, COMMAND_ID_DO_SOMETHING_ELSE, doSomethingElse);
	CMD_METHOD_REGISTER_TIMER(SimpleObject, COMMAND_ID_DO_SOMETHING_ELSE, doSomethingElse, 3000);
}

void SimpleObject::teardownCommandListener( gCommandRouter &router ) {
	CMD_METHOD_DEREGISTER_DEFAULT();
	CMD_METHOD_DEREGISTER(COMMAND_ID_DO_SOMETHING);
	//CMD_METHOD_DEREGISTER(COMMAND_ID_DO_SOMETHING_ELSE);
	CMD_METHOD_DEREGISTER_TIMER(COMMAND_ID_DO_SOMETHING_ELSE, 3000);
}

CMD_METHOD_IMPLEMENT(SimpleObject,processCommand) {
	printf( "I did something by default...\n" );

	ROUTE_COMMAND( SIMPLE_OBJECT_DEVICE_ID, instanceID, COMMAND_ID_DO_SOMETHING, 111 );

}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomething) {
	// put the doSomething code here - the method to register and route to the correct object is done automagically...
	printf( "I did something...\n" );
}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomethingElse) {
	// put the doSomethingElse code here - the method to register and route to the correct object is done automagically...
	printf( "I did something...else\n" );
}

