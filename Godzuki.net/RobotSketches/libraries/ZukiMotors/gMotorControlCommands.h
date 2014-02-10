#ifndef G_MOTORCONTROL_COMMANDS_H
#define G_MOTORCONTROL_COMMANDS_H

static const int COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW = 0x01;
static const int COMMAND_ID_MOTORCONTROL_SET_SPEED_FAST = 0x02;
static const int COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP = 0x07;
static const int COMMAND_ID_MOTORCONTROL_SET_SPEED		= 0x08;
static const int COMMAND_ID_MOTORCONTROL_SET_DIR_FWD    = 0x03;
static const int COMMAND_ID_MOTORCONTROL_SET_DIR_REV    = 0x04;
static const int COMMAND_ID_MOTORCONTROL_START		  	= 0x05;
static const int COMMAND_ID_MOTORCONTROL_STOP     		= 0x06;
static const int COMMAND_ID_MOTORCONTROL_PULL_SPEEDS  	= 0x09;
static const int COMMAND_ID_MOTORCONTROL_UPDATE_SPEEDS  = 0x0a;
static const int COMMAND_ID_MOTORCONTROL_GET_ENCODER 	= 0x0b;
static const int COMMAND_ID_MOTORCONTROL_SET_LEFT_TARGET    = 12;
static const int COMMAND_ID_MOTORCONTROL_SET_RIGHT_TARGET   = 13;
static const int COMMAND_ID_MOTORCONTROL_CLEAR_TARGETS      = 14;
static const int COMMAND_ID_MOTORCONTROL_CHECK_TARGETS		= 15;
static const int COMMAND_ID_MOTORCONTROL_TARGET_REACHED		= 16;

static const int COMMAND_CONST_MOTORCONTROL_FORWARD		= 0x01;
static const int COMMAND_CONST_MOTORCONTROL_BACKWARD	= 0x02;
static const int COMMAND_CONST_MOTORCONTROL_TURNLEFT	= 0x03;
static const int COMMAND_CONST_MOTORCONTROL_TURNRIGHT	= 0x04;
static const int COMMAND_CONST_MOTORCONTROL_STOP		= 0x05;

static const int COMMAND_CONST_MOTORCONTROL_MOTOR1		= 0x01;
static const int COMMAND_CONST_MOTORCONTROL_MOTOR2		= 0x02;
static const int COMMAND_CONST_MOTORCONTROL_MOTOR3		= 0x04;
static const int COMMAND_CONST_MOTORCONTROL_MOTOR4		= 0x08;
static const int COMMAND_CONST_MOTORCONTROL_MOTORS_ALL	= 0x0F;

#endif
