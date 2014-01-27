#include "stdafx.h"
#include "SimpleObject.h"

SimpleObject::SimpleObject() {
	instanceID = -1;
}

void SimpleObject::setup(int thisID, gCommandRouter *router ) {
  instanceID = thisID;
  if( router != 0 )
    setupCommandListener( *router );
}
void SimpleObject::teardown(gCommandRouter *router) {
  if( router != 0 )
    teardownCommandListener( *router );
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
}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomething) {
  // put the doSomething code here - the method to register and route to the correct object is done automagically...
  printf( "I did something...\n" );
}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomethingElse) {
  // put the doSomethingElse code here - the method to register and route to the correct object is done automagically...
  printf( "I did something...else\n" );
}

