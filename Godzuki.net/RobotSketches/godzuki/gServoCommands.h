#ifndef G_SERVO_COMMANDS_H
#define G_SERVO_COMMANDS_H

static const int COMMAND_ID_SERVO_CENTER			= 0x01;
static const int COMMAND_ID_SERVO_SWEEP_ONCE		= 0x02;
static const int COMMAND_ID_SERVO_SWEEP_CONTINUOUS	= 0x03;
static const int COMMAND_ID_SERVO_SWEEP_STOP		= 0x04;
static const int COMMAND_ID_SERVO_SET_POSITION		= 0x05;
static const int COMMAND_ID_SERVO_READ_POSITION		= 0x06;

const int FOLLOW_SERIAL    = 900; // no button - 'f'

static const int COMMAND_ID_SERVO_NO_COMMAND       = 999;

#endif
