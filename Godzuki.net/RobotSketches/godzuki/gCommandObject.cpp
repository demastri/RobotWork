#ifndef WIN32
#include <Arduino.h>
#endif

#include "gCommandObject.h"
#include "gComms.h"

#ifdef WIN32
using namespace ZukiProxy;
#endif
gComms gMonitor;

extern unsigned long millis();

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	Init( srcdev, srcinst, dev, inst, cmd, param, paySize, payData );
}

gCommandObject::gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd ) {
	Init( srcdev, srcinst, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd, int param ) {
	Init( -1, -1, dev, inst, cmd, param, 0, 0 );
}

gCommandObject::gCommandObject( int dev, int inst, int cmd ) {
	Init( -1, -1, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject() {
	Init( -1, -1, -1, -1, -1, -1, 0, 0 );
}

void gCommandObject::Init( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	sourceDeviceID = srcdev;
	sourceInstanceID = srcinst;
	targetDeviceID = dev;
	targetInstanceID = inst;
	commandID = cmd;
	parameter = param;
	payloadSize = paySize;
	payloadData = payData;
	isReply = false;
	isLocal = true;

	nextEntry = prevEntry = 0;
}
gCommandObject::gCommandObject( gCommandObject *rhs ) {
	sourceDeviceID = rhs->sourceDeviceID;
	sourceInstanceID = rhs->sourceInstanceID;
	targetDeviceID = rhs->targetDeviceID;
	targetInstanceID = rhs->targetInstanceID;
	commandID = rhs->commandID;
	parameter = rhs->parameter;
	payloadSize = rhs->payloadSize;
	payloadData = rhs->payloadData;
	isReply = rhs->isReply;

	nextEntry = prevEntry = 0;
}
gCommandObject *gCommandObject::InitReply( unsigned char status, long paySize, void *payData ) { 
	gCommandObject *outObj = new gCommandObject(this);

	outObj->rtnStatus = status; 
	outObj->payloadSize = paySize; 
	outObj->payloadData = payData; 
	outObj->isReply = true; 
	return outObj;
}
char cmdStrBfr[50];
char *gCommandObject::ToCommandString() {
	//   !ssiimmxxyyzzzzzz#  - look in the evernote blog, but it's pretty self evident...

	PlaceInStrBfr( cmdStrBfr, "!",              1,  0 );
	PlaceInStrBfr( cmdStrBfr, sourceDeviceID,   2,  1 );
	PlaceInStrBfr( cmdStrBfr, sourceInstanceID, 2,  3 );
	PlaceInStrBfr( cmdStrBfr, targetDeviceID,   2,  5 );
	PlaceInStrBfr( cmdStrBfr, targetInstanceID, 2,  7 );
	PlaceInStrBfr( cmdStrBfr, commandID,        2,  9 );
	PlaceInStrBfr( cmdStrBfr, parameter,        6, 11 );
	PlaceInStrBfr( cmdStrBfr, "#",              1, 17 );
	PlaceInStrBfr( cmdStrBfr, "\0",             1, 18 );

	return cmdStrBfr;
}
void gCommandObject::PlaceInStrBfr( char *s, char *ss, int size, int loc ) {
	for( int i=0; i<size; i++ )
		*(s+loc+i) = ss[i];
}

void gCommandObject::PlaceInStrBfr( char *s, int data, int size, int loc ) {
	if( data < 0 ) {
		PlaceInStrBfr( s, "-", 1, loc++ );
		size--;
		data = -data;
	}

	char *myBfr = gComms::intToStr( data );
	int thisStrlen = gComms::strlen( myBfr );
	for( int i=0; i<size-thisStrlen; i++ )
		PlaceInStrBfr( s, "0", 1, loc+i );
	PlaceInStrBfr( s, myBfr, thisStrlen, loc+size-thisStrlen );
}


void gCommandObject::print() {
	gMonitor.print("Dumping cmd object... <");
	gMonitor.print( isReply ? sourceDeviceID : targetDeviceID );
	gMonitor.print("> - <");
	gMonitor.print( isReply ? sourceInstanceID : targetInstanceID);
	gMonitor.print("> - <");
	gMonitor.print( commandID );
	gMonitor.print("> - <");
	gMonitor.print( parameter );
	gMonitor.println(">");
#if 0 
	if( cmdHandler == 0 )
		gMonitor.println("had some kind of handler...");
	else
		gMonitor.println("ruh roh");
#endif
}