#ifndef WIN32
#include <Arduino.h>
#endif

#include "gComms.h"
#ifdef WIN32
#include <stdio.h>
using namespace std;
using namespace ZukiProxy;
#endif

const long USB_BAUD_RATE = 9600;
const int MAX_SETUP_WAIT_TIME = 2000;
char gComms::refString[20];

gComms::gComms() {
}

void gComms::setup(bool defaultToRadio, int baudRate) {
	writeToRadio = defaultToRadio;
#ifndef WIN32
	int curSetupWaitTime = 0;
	Serial1.begin(baudRate);             //Set serial baud rate to 9600
	while (!Serial1 && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}

	curSetupWaitTime = 0;
	Serial.begin(115200);
	while (!Serial && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}

	if( Serial ) {
		Serial.println("Godzuki alive and well...");
		if( Serial1 )
			Serial.println("radio available as well");
		else
			Serial.println("radio dead!!");
	}
	if( Serial1 ) {
		Serial1.println("Godzuki alive and well...");
		if( Serial )
			Serial1.println("USb available as well");
		else
			Serial1.println("USB dead!!");
	}
#endif
}

void gComms::BroadcastCommand( gCommandObject *cmdObj ) {
	// this will be where all the magic happens :)
	char *cmdString = cmdObj->ToCommandString();

	print( "Eventually going out to the radio - <" );
	print( cmdString );
	println( ">" );

	print( cmdString );
}

gCommandObject *gComms::UnpackCommandString( char *s ) {
	gCommandObject *cmdObj =  new gCommandObject();
	cmdObj->sourceDeviceID		= (s[1]-'0')*10 + (s[2]-'0');
	cmdObj->sourceInstanceID	= (s[3]-'0')*10 + (s[4]-'0');
	cmdObj->targetDeviceID		= (s[5]-'0')*10 + (s[6]-'0');
	cmdObj->targetInstanceID	= (s[7]-'0')*10 + (s[8]-'0');
	cmdObj->commandID			= (s[9]-'0')*10 + (s[10]-'0');
	cmdObj->parameter			= (s[12]-'0')*10000 + (s[13]-'0')*1000 +(s[14]-'0')*100 + (s[15]-'0')*10 +(s[16]-'0');
	if( s[11] == '-' )
		cmdObj->parameter = -cmdObj->parameter;
	else
		cmdObj->parameter += (s[11]-'0')*100000;
	cmdObj->isLocal = false;

	return cmdObj;
}



void gComms::setup(bool defaultToRadio) {
	setup( defaultToRadio, 9600 );
}
void gComms::setup() {
	setup( true, 9600 );
}
void gComms::processCommand(int newCommand, int cmdParam ) {
	switch( newCommand ) {
	case FOLLOW_SERIAL: 
		writeToRadio = (cmdParam == 1);   // 1 = radio, 0 = usb
		print( "Switching to follow monitor on " );
		println( (char *)(writeToRadio ? "radio" : "USB") );
		break;
	default:
		break;
	} 
}

void gComms::print( char *s ) {
	if( writeToRadio )
		printToRadio( s );
	else
		printToUSB( s );
}

void gComms::print( int i ) {
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

void gComms::println( int i ) {
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
char *gComms::intToStr( int i ) {
	bool isNeg = (i<0);
	int nextChar=0;
	if( isNeg )
		i = -i;
	while( i > 0 ) {
		int convChar = i % 10;
		refString[nextChar++] = (convChar+'0');
		i = i / 10;
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

