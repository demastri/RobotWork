#ifndef G_COMMS_H
#define G_COMMS_H

const int FOLLOW_SERIAL    = 900; // no button - 'f'

#include "gCommandObject.h"

// NOTE - gComms now sets up both the USB Serial port and the radio
// be sure to set this up first!!

#ifdef WIN32
namespace ZukiProxy
{
#endif


public class gComms {
public:
	gComms();
	bool writeToRadio;
	void setup(bool defaultToRadio, int baudRate);
	void setup(bool defaultToRadio);
	void setup();

	void BroadcastCommand( gCommandObject *cmdObj );

	void processCommand(int newCommand, int cmdParam );  
	void print( char *s );
	void println( char *s );
	void print( int i );
	void println( int i );
private:
	void printToRadio( char *s );
	void printlnToRadio( char *s );
	void printToUSB( char *s );
	void printlnToUSB( char *s );

	char *intToStr( int i );
	char refString[20];
};

#ifdef WIN32
}
#endif

#endif
