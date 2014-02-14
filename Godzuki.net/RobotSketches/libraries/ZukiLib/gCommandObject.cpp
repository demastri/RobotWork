#include "gCommandObject.h"
#include "gComms.h"

#ifdef WIN32
using namespace Godzuki;
#endif
gComms gMonitor;

extern unsigned long millis();

gCommandObject::gCommandObject( int src, int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	Init( src, srcdev, srcinst, dev, inst, cmd, param, paySize, payData );
}

gCommandObject::gCommandObject( int src, int srcdev, int srcinst, int dev, int inst, int cmd ) {
	Init( src, srcdev, srcinst, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject( int src, int dev, int inst, int cmd, int param ) {
	Init( src, -1, -1, dev, inst, cmd, param, 0, 0 );
}

gCommandObject::gCommandObject( int src, int dev, int inst, int cmd ) {
	Init( src, -1, -1, dev, inst, cmd, -1, 0, 0 );
}

gCommandObject::gCommandObject() {
	Init( 0, -1, -1, -1, -1, -1, -1, 0, 0 );
}

void gCommandObject::Init( int src, int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData ) {
	sourceDeviceID = srcdev;
	sourceInstanceID = srcinst;
	targetDeviceID = dev;
	targetInstanceID = inst;
	commandID = cmd;
	parameter = param;
	payloadSize = paySize;
	payloadData = payData;
	isReply = false;
	cmdSrc = src;
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
	cmdSrc = rhs->cmdSrc;
}
gCommandObject *gCommandObject::InitReply( unsigned char status, long paySize, void *payData ) { 
	rtnStatus = status; 
	payloadSize = paySize; 
	payloadData = payData; 
	isReply = true; 
	return this;
}

#define OUT_BFR_SIZE 100
uint8_t cmdStrBfr[OUT_BFR_SIZE];
int currentStartPointer = 0;
uint8_t *gCommandObject::ToCommandString( size_t *t ) {
	int startPoint=currentStartPointer;
	//   !ssiimmxxyyzzzzzz#  - look in the evernote blog, but it's pretty self evident...
	if( !isReply ) {
		if( currentStartPointer + 20 >=  OUT_BFR_SIZE )
			startPoint = currentStartPointer = 0;
		*t = 20;
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "!",              1, 0 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), sourceDeviceID,   2, 1 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), sourceInstanceID, 2, 3 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), targetDeviceID,   2, 5 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), targetInstanceID, 2, 7 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), commandID,        2, 9 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), parameter,        8, 11 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "#",              1, 19 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "\0",             1, 20 );
		currentStartPointer += 20;
		return( &(cmdStrBfr[startPoint]) );
	} else { // &ssiimmxxyyccSSZZDDDDDDD# similar - repl param w/status, add size and data
		if( currentStartPointer + 18+payloadSize >=  OUT_BFR_SIZE )
			startPoint = currentStartPointer = 0;
		*t = 17+payloadSize+1;
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "&",					1,				0 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), sourceDeviceID,		2,				1 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), sourceInstanceID,		2,				3 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), targetDeviceID,		2,				5 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), targetInstanceID,		2,				7 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), commandID,				2,				9 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), rtnStatus,				2,				11 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), payloadSize,			4,				13 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), (char *)payloadData,	payloadSize,	17 );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "#",					1,				17+payloadSize );
		PlaceInStrBfr( &(cmdStrBfr[currentStartPointer]), "\0",					1,				17+payloadSize+1 );
		currentStartPointer += 18+payloadSize;
		return( &(cmdStrBfr[startPoint]) );
	}

	return 0;
}
void gCommandObject::PlaceInStrBfr( uint8_t *s, char *ss, int size, int loc ) {
	for( int i=0; i<size; i++ )
		*(s+loc+i) = ss[i];
}

void gCommandObject::PlaceInStrBfr( uint8_t *s, int data, int size, int loc ) {
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

extern char *breakStr;

void gCommandObject::print() {
	gMonitor.print("Dump cmd <");
	gMonitor.print( isReply ? sourceDeviceID : targetDeviceID );
	gMonitor.print(breakStr);
	gMonitor.print( isReply ? sourceInstanceID : targetInstanceID);
	gMonitor.print(breakStr);
	gMonitor.print( commandID );
	gMonitor.print(breakStr);
	gMonitor.print( parameter );
	gMonitor.println(">");
}