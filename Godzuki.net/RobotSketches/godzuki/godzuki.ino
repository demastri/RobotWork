#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "utility/Adafruit_PWMServoDriver.h"

////////////////////

///////  command router definitions
#include "gCommandRouter.h"
gCommandRouter myRouter;

///////  servo control definitions
#include "gServo.h"
#include "gServoCommands.h"
static gServo myServo;  // initializes and attaches the servo on pin 9 to the servo object 

///////  heartbeat and status control definitions
#include "gHBStatus.h"
#include "gHBStatusCommands.h"
static gHBStatus myHBStatus;

///////  kbd input and button control definitions
#include "gInputs.h"
gInputs myInputs;

///////  ultrasonic sensor definitions
#include "gDistanceSensor.h"
//gDistanceSensor myDistanceSensor;
unsigned long lastDistanceTime=0;

///////  communications control definitions
#include "gComms.h"
extern gComms gMonitor;

#include "gMotorControl.h"
gMotorControl gMotors;

extern int DEFAULT_DEVICE_ID;
extern int DEFAULT_INSTANCE_ID;

////////////////////
void setup() { 
	DEFAULT_DEVICE_ID = GODZUKI_SENSOR_PLATFORM_DEVICE_ID;
	DEFAULT_INSTANCE_ID = 1;

	myRouter.setup();

	gMonitor.setup(true);
	myHBStatus.setup(1, &myRouter);
	myServo.setup(9, 1, &myRouter);
	gMotors.setup(1, &myRouter);
	myInputs.setup(1, &myRouter);
	//myDistanceSensor.setup(1, &myRouter, 5, 6, 500);
} 

void loop() {
	int DEVICE_ID = -1;
	int instanceID = 1;

	gCommandRouter *pRouter = &myRouter;

	ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_LOOP_INIT, -1 );

	int kbdParam = -1;
	int new_command_input = myInputs.ReadCommand( kbdParam );

	// handle any radio stuff
	gMonitor.processCommand(new_command_input, kbdParam );

	// handle ALL the command stuff
	myRouter.ScanCommands();

	// handle maintenance stuff
	ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_LOOP_FINALLY, -1 );
}
