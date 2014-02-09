#ifndef G_SDCARD_H
#define G_SDCARD_H

#include "gCommandRouter.h"

class gSDCard {
public:
	static const int DEVICE_ID = SD_CARD_DEVICE_ID;

	gSDCard();

	void setup(int thisID, gCommandRouter *router, int CS );
	
	CMD_METHOD_DEFINE(processCommand);

	bool isReady;

private:
	gCommandRouter *pRouter;
	SdFat sd;

	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int CSPin;
};

#endif
