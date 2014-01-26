#ifndef G_INPUTS_H
#define G_INPUTS_H

#include "gCommandRouter.h"

class gInputs {
public:
	static const int DEVICE_ID = INPUT_HANDLER_DEVICE_ID;

	gInputs();
	void setup(int thisID, gCommandRouter *router );
	int ReadCommand( int &param );
private:
	int instanceID;
	gCommandRouter *pRouter;

	int adc_key_in;
	int NUM_KEYS;
	int key;
	int oldkey;
	int showKeyStateOnLED;
	int returnKey;

	int get_key(unsigned int input);
};

#endif
