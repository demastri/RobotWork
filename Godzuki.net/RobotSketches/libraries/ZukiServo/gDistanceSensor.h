#ifndef G_DISTANCE_SENSOR_H
#define G_DISTANCE_SENSOR_H

#include "gCommandRouter.h"

class gDistanceSensor {
public:
	static const int DEVICE_ID = DISTANCE_SENSOR_DEVICE_ID;

	gDistanceSensor();

	void setup(int thisID, gCommandRouter *router, int trigPin, int pwmPin, int timeGap );
	
	CMD_METHOD_DEFINE(processCommand);
	CMD_METHOD_DEFINE(continueReadings);

	void setContinuousTiming(int gap );
	int getDistance();

private:
	static const int COMMAND_ID_RANGER_CHECK_READING = 0x99;

	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int URPWM;                                        // PWM Output 0-25000us,every 50us represent 1cm
	int URTRIG;                                       // PWM trigger pin
	unsigned long lastReadTime;
	unsigned long readTimeGap;
	int currentCommand;

	void continueReadings();
};

#endif
