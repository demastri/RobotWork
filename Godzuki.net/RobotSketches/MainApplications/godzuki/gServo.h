#ifndef G_SERVO_H
#define G_SERVO_H

#include "gCommandRouter.h"

class gServo {
public:
	static const int DEVICE_ID = SERVO_DEVICE_ID;

private:
	static const int COMMAND_ID_CONTINUE_SWEEP		= 0x99;

	char cmdBuffer[5];

private:
	int SWEEP_TOP;
	int SWEEP_CENTER;
	int SWEEP_BOTTOM;
	int SWEEP_INCREMENT;
public:
	int pos;    // variable to store the servo position 
	int currentCommand;

	gServo();
	void setup( int pin, int thisID, gCommandRouter *router );

	CMD_METHOD_DEFINE(processCommand);
	CMD_METHOD_DEFINE(continueSweep);

private:
	Servo myBaseServo;  // create servo object to control a servo 

	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	unsigned long last_update_time;
	int sweep_direction;

	void startSweep();
	void stopSweep();
	void center();
	void moveTo( int newPos );
	
	uint8_t cmdBfr[10];
};

#endif
