#include <Arduino.h>

#include "gServoCommands.h"
#include "gComms.h"

const long USB_BAUD_RATE = 57600;
const int MAX_SETUP_WAIT_TIME = 1000;

gComms::gComms() {
}
void gComms::setup(boolean defaultToRadio, int baudRate) {
	writeToRadio = defaultToRadio;
	Serial1.begin(baudRate);             //Set serial baud rate to 9600

	Serial.begin(57600);
	int curSetupWaitTime = 0;
	while (!Serial && curSetupWaitTime < MAX_SETUP_WAIT_TIME) {
		delay(20); // Wait untilSerial is ready - Leonardo
		curSetupWaitTime += 20;
	}
	Serial.println("Godzuki alive and well...");
}
void gComms::setup(boolean defaultToRadio) {
	setup( defaultToRadio, 9600 );
}
void gComms::setup() {
	setup( true, 9600 );
}
void gComms::processCommand(int newCommand, int cmdParam ) {
	switch( newCommand ) {
	case FOLLOW_SERIAL: 
		writeToRadio = cmdParam;   // 1 = radio, 0 = usb
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
	Serial1.print(s);
}

void gComms::printlnToRadio( char *s ) {
	Serial1.println(s);
}

void gComms::printToUSB( char *s ) {
	Serial.print(s);
}

void gComms::printlnToUSB( char *s ) {
	Serial.println(s);
}
char *gComms::intToStr( int i ) {
	boolean isNeg = (i<0);
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

