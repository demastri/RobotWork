#ifndef WIN32
#include <Arduino.h>
#endif

#ifdef WIN32
#include <stdio.h>
using namespace std;
#endif

#include "gServoCommands.h"
#include "gComms.h"

const long USB_BAUD_RATE = 9600;
const int MAX_SETUP_WAIT_TIME = 2000;

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

