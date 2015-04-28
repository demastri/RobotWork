#include <Arduino.h>

#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>         

#include "gCamera.h"
#include "gCameraCommands.h"

gCamera::gCamera() : cameraconnection(11, 8), cam(&cameraconnection) {
	isReady = false;
}

void gCamera::setup(int thisID, gCommandRouter *router ) {
	mySDCard.setup(1, router, 10);

	if( !cam.begin() ) {
		isReady = false;
		gMonitor.println( "CamDead" );
		return;
	}
	isReady = true;


	instanceID = thisID;
	if( router != 0 )
		setupCommandListener( *router );

}
void gCamera::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gCamera, processCommand);
}

CMD_METHOD_IMPLEMENT(gCamera,processCommand) {
	switch( cmdObj->commandID ) {
	case 0: 
		break;
	}
}

