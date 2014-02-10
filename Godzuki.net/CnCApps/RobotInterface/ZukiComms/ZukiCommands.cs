using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Godzuki
{
    public class ZukiCommands
    {
        public const int SIMPLE_OBJECT_DEVICE_ID             = 01;
        public const int HEARTBEAT_DEVICE_ID                 = 02;
        public const int SERVO_DEVICE_ID                     = 03;
        public const int MOTOR_CONTROL_DEVICE_ID             = 04;
        public const int INPUT_HANDLER_DEVICE_ID             = 05;
        public const int DISTANCE_SENSOR_DEVICE_ID           = 06;
        public const int CAMERA_DEVICE_ID                    = 07;
        public const int SDCARD_DEVICE_ID                    = 08;

        public const int CNC_APP_DEVICE_ID                   = 97;
        public const int TEST_COMMAND_ROUTER_APP_DEVICE_ID   = 98;
        public const int GODZUKI_SENSOR_PLATFORM_DEVICE_ID   = 99;

        // global command status values
        public const int GLOBAL_COMMAND_STATUS_OK            = 00;
        public const int GLOBAL_COMMAND_STATUS_FAIL          = 01;
        // global command values - 90+ reserved for global
        public const int COMMAND_ID_GLOBAL_NONE              = 97;
        public const int COMMAND_ID_GLOBAL_REQUEST_STATUS    = 98;
        public const int COMMAND_ID_GLOBAL_BROADCAST         = 99;

        // Heaartbeat commands
        public const int COMMAND_ID_HBSTATUS_LOOP_INIT       = 01;
        public const int COMMAND_ID_HBSTATUS_LOOP_FINALLY    = 02;
        public const int COMMAND_ID_HBSTATUS_SET_STATUS      = 03;
        public const int COMMAND_ID_HBSTATUS_CLEAR_STATUS    = 04;

        // Servo Commands
        public const int COMMAND_ID_SERVO_CENTER             = 01;
        public const int COMMAND_ID_SERVO_SWEEP_ONCE         = 02;
        public const int COMMAND_ID_SERVO_SWEEP_CONTINUOUS   = 03;
        public const int COMMAND_ID_SERVO_SWEEP_STOP         = 04;
        public const int COMMAND_ID_SERVO_SET_POSITION       = 05;
        public const int COMMAND_ID_SERVO_READ_POSITION      = 06;

        // motor control commands
        public const int COMMAND_ID_MOTORCONTROL_SET_SPEED_SLOW = 01;
        public const int COMMAND_ID_MOTORCONTROL_SET_SPEED_FAST = 02;
        public const int COMMAND_ID_MOTORCONTROL_SET_SPEED_BUMP = 07;
        public const int COMMAND_ID_MOTORCONTROL_SET_SPEED      = 08;
        public const int COMMAND_ID_MOTORCONTROL_SET_DIR_FWD    = 03;
        public const int COMMAND_ID_MOTORCONTROL_SET_DIR_REV    = 04;
        public const int COMMAND_ID_MOTORCONTROL_START          = 05;
        public const int COMMAND_ID_MOTORCONTROL_STOP           = 06;
        public const int COMMAND_ID_MOTORCONTROL_PULL_SPEEDS    =  9;
        public const int COMMAND_ID_MOTORCONTROL_UPDATE_SPEEDS  = 10;
        public const int COMMAND_ID_MOTORCONTROL_GET_ENCODER    = 11;
        public const int COMMAND_ID_MOTORCONTROL_SET_LEFT_TARGET    = 12;
        public const int COMMAND_ID_MOTORCONTROL_SET_RIGHT_TARGET   = 13;
        public const int COMMAND_ID_MOTORCONTROL_CLEAR_TARGETS = 14;
        public const int COMMAND_ID_MOTORCONTROL_CHECK_TARGETS = 15;
        public const int COMMAND_ID_MOTORCONTROL_TARGET_REACHED = 16;

        public const int COMMAND_CONST_MOTORCONTROL_FORWARD     = 0x01;
        public const int COMMAND_CONST_MOTORCONTROL_BACKWARD    = 0x02;
        public const int COMMAND_CONST_MOTORCONTROL_TURNLEFT    = 0x03;
        public const int COMMAND_CONST_MOTORCONTROL_TURNRIGHT   = 0x04;
        public const int COMMAND_CONST_MOTORCONTROL_STOP        = 0x05;

        public const int COMMAND_CONST_MOTORCONTROL_MOTOR1      = 0x01;
        public const int COMMAND_CONST_MOTORCONTROL_MOTOR2      = 0x02;
        public const int COMMAND_CONST_MOTORCONTROL_MOTOR3      = 0x04;
        public const int COMMAND_CONST_MOTORCONTROL_MOTOR4      = 0x08;
        public const int COMMAND_CONST_MOTORCONTROL_MOTORS_ALL  = 0x0F;

        // distance sensor commands
        public const int COMMAND_ID_RANGER_READ_DISTANCE    = 01;

        // camera commands
        public const int COMMAND_ID_CAMERA_SNAP_IMAGE       = 01;
        public const int COMMAND_ID_CAMERA_PULL_DIRECTORY   = 02;
        public const int COMMAND_ID_CAMERA_RETREIVE_FILE    = 03;

        // SD Card commands
        public const int COMMAND_ID_SDCARD_INIT             = 01;
        public const int COMMAND_ID_SDCARD_GET_DIR          = 02;
        public const int COMMAND_ID_SDCARD_GET_FILE         = 03;
        public const int COMMAND_ID_SDCARD_DEL_FILE         = 04;
        public const int COMMAND_ID_SDCARD_PUSH_FILE        = 05;
        public const int COMMAND_ID_SDCARD_OPEN_FILE        = 06;
        public const int COMMAND_ID_SDCARD_WRITE_FILE       = 07;
        public const int COMMAND_ID_SDCARD_CLOSE_FILE       = 08;
        public const int COMMAND_ID_SDCARD_UPDATE_FILE      = 09;

    }
}
