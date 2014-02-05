/*
* gMotorControl.h
*
*  Created on: Jan 13, 2014
*      Author: john
*/

#ifndef GMOTORCONTROL_H_
#define GMOTORCONTROL_H_

#include "gCommandRouter.h"
#include "gMotor.h"

class gMotorControl {
public:
	static const int DEVICE_ID = MOTOR_CONTROL_DEVICE_ID;

	gMotorControl();
	virtual ~gMotorControl();

	void setup(int thisID, gCommandRouter *router, int leftEncoderPin, int rightEncoderPin );
	CMD_METHOD_DEFINE(processCommand);
	CMD_METHOD_DEFINE(calculateSpeeds);

private:
	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int currentSpeed;

	const static int maxMotors = 4;
	gMotor *(myMotors[4]);
	void setSpeed( int thisSpeed, int motorID);
	void setSpeeds( int thisSpeed );
	void setDirection(int thisDir, int motorID);
	void setDirections( int thisDir );
	void start(int motorID);
	void startAll();
	void stop(int motorID);
	void stopAll();
	
	uint8_t rtnSpdBfr[9];
	double mmsPerClick;  // 204mm/rev (65mm dia), 10 blades = 20 int = 10.2 mm/click

	static volatile unsigned long leftTotalClicks;
	static volatile unsigned long rightTotalClicks;

	bool updateSpeeds;
	long instSpeedUpdateTime;
	double leftInstantaneousSpeed;
	double rightInstantaneousSpeed;
	static volatile long leftAggregateClicks;
	static volatile long rightAggregateClicks;
	volatile long leftLastAggregateClicks;
	volatile long rightLastAggregateClicks;
	volatile long lastSpeedCalcTime;

	static int leftDir;
	static int rightDir;

	static void updateLeft();
	static void updateRight();

};

#endif /* GMOTORCONTROL_H_ */
