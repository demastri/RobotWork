#include <SdFat.h>

extern bool itsCrayCray;

#include "gSDCard.h"
#include "gSDCardCommands.h"

#include "gComms.h"
extern gComms gMonitor;

gSDCard::gSDCard() {
	isReady = false;
}

void gSDCard::setup(int thisID, gCommandRouter *router, int chipSelect ) {
	instanceID = thisID;
	if( !sd.begin((CSPin=chipSelect), SPI_HALF_SPEED)) {
		isReady = false;
		gMonitor.println( "SDead" );
		return;
	}
	isReady = true;
	if( router != 0 )
		setupCommandListener( *router );
}

void gSDCard::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gSDCard, processCommand);
	pRouter = &router;
}

CMD_METHOD_IMPLEMENT(gSDCard, processCommand) {
	switch( cmdObj->commandID ) {
	case COMMAND_ID_SDCARD_INIT: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_GET_DIR: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_GET_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_DEL_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_FAIL, 10, (void *)"Some value" );
		break;
	case COMMAND_ID_SDCARD_PUSH_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_OPEN_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_WRITE_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_CLOSE_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	case COMMAND_ID_SDCARD_UPDATE_FILE: 
		ROUTE_REPLY( GLOBAL_COMMAND_STATUS_OK, 0, 0 );
		break;
	default: 
		break;
	}
}
