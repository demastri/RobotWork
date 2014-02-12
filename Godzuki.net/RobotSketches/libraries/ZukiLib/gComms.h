#ifndef G_COMMS_H
#define G_COMMS_H

const int FOLLOW_SERIAL    = 900; // no button - 'f'

#include "gCommandRouter.h"
#include "gCommandObject.h"
#include "gCommsCommands.h"

// NOTE - gComms now sets up both the USB Serial port and the radio
// be sure to set this up first!!

#ifdef WIN32
namespace Godzuki
{
	public class gComms
#else
class gComms
#endif
{	
public:
	static const int DEVICE_ID = COMMS_DEVICE_ID;

	gComms();
	bool writeToRadio;
	void setup(int thisID, gCommandRouter *router, bool defaultToRadio, int baudRate);
	void setup(int thisID, gCommandRouter *router, bool defaultToRadio);
	void setup();
	CMD_METHOD_DEFINE(processCommand);

	void BroadcastCommand( gCommandObject *cmdObj, int dest );
	static gCommandObject *UnpackCommandString( char *s, int src );

	void print( const uint8_t *s, size_t t );
	void print( char *s );
	void println( char *s );
	void print( long i );
	void println( long i );

	static char *intToStr( long i );
	static char refString[20];
	static unsigned int strlen( char *s ) { int i=0; while( *(s+i) != '\0') i++; return i; };

private:
	gCommandRouter *pRouter;
	int instanceID;
	void setupCommandListener( gCommandRouter &router );

	void printToRadio( const uint8_t *s, size_t t );
	void printToUSB( const uint8_t *s, size_t t );
	void printToRadio( char *s );
	void printlnToRadio( char *s );
	void printToUSB( char *s );
	void printlnToUSB( char *s );
};

#ifdef WIN32
}
#endif

#endif
