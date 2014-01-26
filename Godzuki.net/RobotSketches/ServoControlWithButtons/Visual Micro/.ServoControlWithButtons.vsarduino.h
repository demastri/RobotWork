#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Leonardo
#define __AVR_ATmega32U4__
#define 
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void init_status_led();
void light_status_led();
void clear_status_led();
void write_final_led_status();
void check_heartbeat();
void start_sweep();
void continue_sweep();
void stop_sweep();
void go_to_position( int newPos );
void go_to_center();
int get_debounced_key();
int get_key(unsigned int input);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\leonardo\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Projects\Workspaces\Robot\Godzuki.net\RobotSketches\ServoControlWithButtons\ServoControlWithButtons.ino"
#endif
