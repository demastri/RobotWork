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

	void setup(int thisID, gCommandRouter *router );
	CMD_METHOD_DEFINE(processCommand);

private:
	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	int currentSpeed;

	const static int maxMotors = 4;
	gMotor *(myMotors[4]);
	void setSpeeds( int thisSpeed );
	void setDirections( int thisDir );
	void startAll();
	void stopAll();
};

#endif /* GMOTORCONTROL_H_ */
