#include <Arduino.h>
#include "SimpleObject.h"

SimpleObject::SimpleObject() {
	instanceID = -1;
}

void SimpleObject::setup(int thisID, CommandRouter *router ) {
	instanceID = thisID;
	if( router != 0 )
		setupCommandListener( *router );
}
void SimpleObject::teardown(CommandRouter *router) {
	if( router != 0 )
		teardownCommandListener( *router );
}

void SimpleObject::setupCommandListener( CommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(SimpleObject, processCommandProxy);
	CMD_METHOD_REGISTER(SimpleObject, COMMAND_ID_DO_SOMETHING,      doSomething);
	//CMD_METHOD_REGISTER(SimpleObject, COMMAND_ID_DO_SOMETHING_ELSE, doSomethingElse);
	CMD_METHOD_REGISTER_TIMER(SimpleObject, COMMAND_ID_DO_SOMETHING_ELSE, doSomethingElse, 1000);
}

void SimpleObject::teardownCommandListener( CommandRouter &router ) {
	CMD_METHOD_DEREGISTER_DEFAULT();
	CMD_METHOD_DEREGISTER(COMMAND_ID_DO_SOMETHING);
	//CMD_METHOD_DEREGISTER(COMMAND_ID_DO_SOMETHING_ELSE);
	CMD_METHOD_DEREGISTER_TIMER(COMMAND_ID_DO_SOMETHING_ELSE, 1000);
}

CMD_METHOD_IMPLEMENT(SimpleObject,processCommand) {
	Serial.println( "I did something by default..." );
}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomething) {
	// put the doSomething code here - the method to register and route to the correct object is done automagically...
	Serial.println( "I did something..." );
}

CMD_METHOD_IMPLEMENT(SimpleObject,doSomethingElse) {
	// put the doSomethingElse code here - the method to register and route to the correct object is done automagically...
	Serial.println( "I did something...else" );
}

