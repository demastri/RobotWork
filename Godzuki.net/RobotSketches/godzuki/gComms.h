#ifndef G_COMMS_H
#define G_COMMS_H

#include "gCommandObject.h"

// NOTE - gComms now sets up both the USB Serial port and the radio
// be sure to set this up first!!

class gComms {
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

#endif
