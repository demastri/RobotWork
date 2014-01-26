#include "gCommandObject.h"

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	Init( srcdev, srcinst, dev, inst, cmd, param, paySize, payData );
}

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd ) {
	Init( srcdev, srcinst, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd, int param ) {
	Init( -1, -1, dev, inst, cmd, param, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd ) {
	Init( -1, -1, dev, inst, cmd, -1, 0, 0 );
}

void gCommandObject::Init( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	sourceDeviceID = srcdev;
	sourceInstanceID = srcinst;
	targetDeviceID = dev;
	targetInstanceID = inst;
	commandID = cmd;
	parameter = param;
	payloadSize = paySize;
	payloadData = payData;
}