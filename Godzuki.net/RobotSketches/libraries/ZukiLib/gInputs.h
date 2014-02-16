#ifndef G_INPUTS_H
#define G_INPUTS_H

#include "gCommandRouter.h"

class gInputs {
public:
	static const int DEVICE_ID = INPUT_HANDLER_DEVICE_ID;

	gInputs();
	void setup(int thisID, gCommandRouter *router );
	int ReadCommand( int &param );
	int showKeyStateOnLED;
private:
	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );
	CMD_METHOD_DEFINE(processCommand);

	int adc_key_in;
	int NUM_KEYS;
	int key;
	int oldkey;
	int returnKey;

	int get_key(unsigned int input);
};

#endif
