// CommandRouterDriver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

extern void setup();
extern int loop();

extern unsigned long millis();

extern unsigned long millis() {

	return GetTickCount();
}

int _tmain(int argc, _TCHAR* argv[])
{
	setup();

	for( ;; ) {
		if( loop() )
			return 0;
	}
	return 0;
}

