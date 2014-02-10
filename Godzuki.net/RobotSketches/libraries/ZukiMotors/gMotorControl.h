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
	CMD_METHOD_DEFINE(checkEncoderTarget);

private:
	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int currentSpeed;

	const static int maxMotors = 4;
	static gMotor *(myMotors[4]);
	void setSpeed( int thisSpeed, int motorID);
	void setSpeeds( int thisSpeed );
	void setDirection(int thisDir, int motorID);
	void setDirections( int thisDir );
	void start(int motorID);
	void startAll();
	static void stop(int motorID);
	static void stopAll();
	
	uint8_t rtnSpdBfr[9];
	uint8_t rtnClkBfr[11];
	double mmsPerClick;  // 204mm/rev (65mm dia), 10 blades = 20 int = 10.2 mm/click

	static volatile unsigned long leftTotalClicks;
	static volatile unsigned long rightTotalClicks;

	bool updateSpeeds;
	long instSpeedUpdateTime;
	double leftInstantaneousSpeed;
	double rightInstantaneousSpeed;
	static bool encoderTargetsSet;
	static bool encoderTargetsMet;
	gCommandObject *leftTargetCmd;
	gCommandObject *rightTargetCmd;
	static long leftTarget;
	static long rightTarget;
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
