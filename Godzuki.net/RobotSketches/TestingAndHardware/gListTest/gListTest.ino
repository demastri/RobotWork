

#include <Servo.h>
#include <ZukiLib.h>
#include <gServoCommands.h>
#include <gServo.h>
#include <gRoutingDeviceIDs.h>
#include <gRouteTableEntry.h>
#include <gListNode.h>
#include <gInputs.h>
#include <gHBStatusCommands.h>
#include <gHBStatus.h>
#include <gDistanceSensorCommands.h>
#include <gDistanceSensor.h>
#include <gCommsCommands.h>
#include <gComms.h>
#include <gCommandRouter.h>
#include <gCommandObject.h>

const long RADIO_BAUD_RATE = 19200;
const int MAX_SETUP_WAIT_TIME = 2000;

gList baseList;
extern char *welcomeString;
int refObject;

void setup()
{

	/* add setup code here */
	int curSetupWaitTime = 0;
	Serial1.begin(RADIO_BAUD_RATE);             //Set serial baud rate to 9600
	while (!Serial1 && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}
}

void loop()
{

	/* add main program code here */
	Serial1.println( welcomeString );

	if( baseList.isEmpty() )
		Serial1.println( "fixed gList is empty" );
	else
		Serial1.println( "fixed gList is NOT empty ??" );

	gList *otherList = new gList();
	if( otherList->isEmpty() )
		Serial1.println( "alloc'd gList is empty" );
	else
		Serial1.println( "alloc'd gList is NOT empty ??" );

	while( 1 ) {
		Serial1.print( "gList Size:" );
		Serial1.println( baseList.Size() );
		for( int i=0; i<10; i++ ) {
			baseList.Add(&refObject);
			Serial1.print( "gList Size:" );
			Serial1.println( baseList.Size() );
		}
		baseList.PopFirst();
		Serial1.print( "gList Size:" );
		Serial1.println( baseList.Size() );

		baseList.RemoveNode( baseList.First() );
		Serial1.print( "gList Size:" );
		Serial1.println( baseList.Size() );

		for( int i=0; i<8; i++ ) {
			baseList.Remove(&refObject);
			Serial1.print( "gList Size:" );
			Serial1.println( baseList.Size() );
		}
	}

}
