#ifndef WIN32
#include <Arduino.h>
#endif

#include "gComms.h"
#ifdef WIN32
#include <stdio.h>
using namespace std;
using namespace Godzuki;
#endif

const long RADIO_BAUD_RATE = 19200;
const long USB_BAUD_RATE = 115200;		// 115200
const int MAX_SETUP_WAIT_TIME = 2000;
char gComms::refString[20];

char *welcomsString = "Godzuki alive and well...";


gComms::gComms() {
}

void gComms::setup() {
	setup( -1, 0, true, RADIO_BAUD_RATE );
}
void gComms::setup(int thisID, gCommandRouter *router, bool defaultToRadio) {
	setup( thisID, router, defaultToRadio, RADIO_BAUD_RATE );
}
void gComms::setup(int thisID, gCommandRouter *router, bool defaultToRadio, int baudRate) {
	writeToRadio = defaultToRadio;
#ifndef WIN32
	int curSetupWaitTime = 0;
	Serial1.begin(baudRate);             //Set serial baud rate to 9600
	while (!Serial1 && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}

	curSetupWaitTime = 0;
	Serial.begin(USB_BAUD_RATE);
	while (!Serial && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}


	if( Serial ) {
		Serial.print(welcomsString);
		Serial.println((char *)(Serial1 ? "radio too" : "radio dead") );
	}
	if( Serial1 ) {
		Serial1.print(welcomsString);
		Serial1.println((char *)(Serial ? "USB too" : "USB dead") );
	}
#endif
	if( router != 0 )
		setupCommandListener( *router );
}
void gComms::setupCommandListener( gCommandRouter &router ) {
	CMD_METHOD_REGISTER_DEFAULT(gComms, processCommand);

	pRouter = &router;
}


void gComms::BroadcastCommand( gCommandObject *cmdObj, int dest ) {
	// this will be where all the magic happens :)
	size_t outSize;
	uint8_t *cmdString = cmdObj->ToCommandString( &outSize );

	if( dest == 0 )
		printToUSB( cmdString, outSize );
	else if( dest == 1 )
		printToRadio( cmdString, outSize );
}

gCommandObject *gComms::UnpackCommandString( char *s, int src ) {
	gCommandObject *cmdObj =  new gCommandObject();
	cmdObj->sourceDeviceID		= (s[1]-'0')*10 + (s[2]-'0');
	cmdObj->sourceInstanceID	= (s[3]-'0')*10 + (s[4]-'0');
	cmdObj->targetDeviceID		= (s[5]-'0')*10 + (s[6]-'0');
	cmdObj->targetInstanceID	= (s[7]-'0')*10 + (s[8]-'0');
	cmdObj->commandID			= (s[9]-'0')*10 + (s[10]-'0');
	cmdObj->parameter			= (s[12]-'0')*1000000L + (s[13]-'0')*100000L +(s[14]-'0')*10000L + (s[15]-'0')*1000L +(s[16]-'0')*100L+(s[17]-'0')*10+(s[18]-'0');
	if( s[11] == '-' )
		cmdObj->parameter = -cmdObj->parameter;
	else
		cmdObj->parameter += (s[11]-'0')*10000000L;
	cmdObj->cmdSrc = src;

	return cmdObj;
}

CMD_METHOD_IMPLEMENT(gComms,processCommand) {
	switch( cmdObj->commandID ) {
	case COMMAND_ID_COMMS_FOLLOW_SERIAL: 
		writeToRadio = (cmdObj->parameter == 1);   // 1 = radio, 0 = usb
		print( "Switching to follow monitor on " );
		println( (char *)(writeToRadio ? "radio" : "USB") );
		break;
	default:
		break;
	} 
}

void gComms::print( const uint8_t *s, size_t t ) {
#ifndef WIN32
	if( writeToRadio )
		printToRadio( s, t );
	else
		printToUSB( s, t );
#endif
}
void gComms::printToRadio( const uint8_t *s, size_t t ) {
#ifndef WIN32
	Serial1.write(s, t);
#endif
}
void gComms::printToUSB( const uint8_t *s, size_t t ) {
#ifndef WIN32
	Serial.write(s, t);
#endif
}

void gComms::print( char *s ) {
	if( writeToRadio )
		printToRadio( s );
	else
		printToUSB( s );
}

void gComms::print( long i ) {
	if( writeToRadio )
		printToRadio( intToStr(i) );
	else
		printToUSB( intToStr(i) );
}
void gComms::println( char *s ) {
	if( writeToRadio )
		printlnToRadio( s );
	else
		printlnToUSB( s );
}

void gComms::println( long i ) {
	if( writeToRadio )
		printlnToRadio( intToStr(i) );
	else
		printlnToUSB( intToStr(i) );
}
void gComms::printToRadio( char *s ) {
#ifdef WIN32
	printf("%s", s);
#else
	Serial1.print(s);
#endif
}

void gComms::printlnToRadio( char *s ) {
#ifdef WIN32
	printf("%s\n", s);
#else
	Serial1.println(s);
#endif
}

void gComms::printToUSB( char *s ) {
#ifdef WIN32
	printf("%s", s);
#else
	Serial.println(s);
#endif
}

void gComms::printlnToUSB( char *s ) {
#ifdef WIN32
	printf("%s\n", s);
#else
	Serial.println(s);
#endif
}
char *gComms::intToStr( long i ) {
	bool isNeg = (i<0);
	int nextChar=0;
	if( isNeg )
		i = -i;
	while( i > 0 ) {
		int convChar = i % 10;
		refString[nextChar++] = (convChar+'0');
		i = i / 10L;
	}
	if( isNeg ) {
		refString[nextChar++] = '-';
	}
	if(nextChar == 0 ) {
		refString[nextChar++] = '0';
	}
	refString[nextChar] = '\0';

	for( int x=0; x<nextChar; x++ ) {
		if(x >= nextChar-x-1 )
			break;
		char a = refString[x];
		char b = refString[nextChar-x-1];
		refString[x] = b;
		refString[nextChar-x-1] = a;
	}

	return refString;
}

