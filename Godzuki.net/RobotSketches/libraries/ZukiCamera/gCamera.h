#ifndef G_CAMERA_H
#define G_CAMERA_H

#include "gCommandRouter.h"

#include "gComms.h"
extern gComms gMonitor;

#include <SdFat.h>
#include "gSDCard.h"
#include "gSDCardCommands.h"

class gCamera {
public:
	static const int DEVICE_ID = CAMERA_DEVICE_ID;
	bool isReady;

	gCamera();
	void setup(int thisID, gCommandRouter *router );

	CMD_METHOD_DEFINE(processCommand);

private:
	int instanceID;
	void setupCommandListener( gCommandRouter &router );
	
	SoftwareSerial cameraconnection;
	Adafruit_VC0706 cam;
	gSDCard mySDCard;
};

#endif
