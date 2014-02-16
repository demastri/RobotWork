#include <Servo.h>

#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>         
#include <SdFat.h>

////////////////////

///////  command router definitions
#include "gCommandRouter.h"
gCommandRouter myRouter;

///////  servo control definitions
#include "gServo.h"
#include "gServoCommands.h"
static gServo myServoPan;  // initializes and attaches the servo on pin 9 to the servo object 
static gServo myServoTilt;  // initializes and attaches the servo on pin 9 to the servo object 

///////  heartbeat and status control definitions
#include "gHBStatus.h"
#include "gHBStatusCommands.h"
static gHBStatus myHBStatus;

///////  kbd input and button control definitions
#include "gInputs.h"
gInputs myInputs;

///////  ultrasonic sensor definitions
//#include "gDistanceSensor.h"
//gDistanceSensor myDistanceSensor;1
//unsigned long lastDistanceTime=0;

///////  communications control definitions
#include "gComms.h"
extern gComms gMonitor;

//#include "gMotorControl.h"
//gMotorControl gMotors;

#include "gCamera.h"
#include "gCameraCommands.h"
gCamera myCamera;

extern int DEFAULT_DEVICE_ID;
extern int DEFAULT_INSTANCE_ID;

extern const int gButtonMappingCmds[]	= {COMMAND_ID_HBSTATUS_SET_STATUS,COMMAND_ID_HBSTATUS_SET_STATUS,COMMAND_ID_HBSTATUS_SET_STATUS,COMMAND_ID_HBSTATUS_SET_STATUS,COMMAND_ID_HBSTATUS_SET_STATUS};
extern const int gButtonMappingDevs[]	= {HEARTBEAT_DEVICE_ID,           HEARTBEAT_DEVICE_ID,           HEARTBEAT_DEVICE_ID,           HEARTBEAT_DEVICE_ID,           HEARTBEAT_DEVICE_ID           };
extern const int gButtonMappingIDs[]	= {1,                             1,                             1,                             1,                             1,                            };

////////////////////
void setup() { 
	DEFAULT_DEVICE_ID = GODZUKI_SENSOR_PLATFORM_DEVICE_ID;
	DEFAULT_INSTANCE_ID = 2;

	myRouter.setup();

	gMonitor.setup(1, &myRouter, true);
	myInputs.setup(1, &myRouter);
	myHBStatus.setup(1, &myRouter);
	myServoPan.setup(9, 1, &myRouter);
	myServoTilt.setup(6, 2, &myRouter);
	//gMotors.setup(1, &myRouter, 10, 11);
	//myDistanceSensor.setup(1, &myRouter, 5, 6, -1);
	myCamera.setup(1, &myRouter);
} 

void loop() {

	gCommandRouter *pRouter = &myRouter;

	myHBStatus.loopInitActually(0);
	//ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_LOOP_INIT, -1 );

	int kbdParam = -1;
	int new_command_input = myInputs.ReadCommand( kbdParam );

	// handle ALL the command stuff
	myRouter.ScanCommands();

	// handle maintenance stuff
	myHBStatus.loopFinallyActually(0);
	//ROUTE_COMMAND( HEARTBEAT_DEVICE_ID, 1, COMMAND_ID_HBSTATUS_LOOP_FINALLY, -1 );
	//delay( 50 );
}
