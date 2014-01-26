#ifndef G_COMMS_H
#define G_COMMS_H

// NOTE - by default gInputs sets up the USB Serial port
// 1 - don't do it here as well
// 2 - be sure to setup gInputs first!!

class gComms {
public:
	gComms();
	boolean writeToRadio;
	void setup(boolean defaultToRadio, int baudRate);
	void setup(boolean defaultToRadio);
	void setup();

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
