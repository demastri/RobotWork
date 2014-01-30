#include "stdafx.h"
#include <windows.h>

extern unsigned long millis();

extern unsigned long millis() {

	return GetTickCount();
}
