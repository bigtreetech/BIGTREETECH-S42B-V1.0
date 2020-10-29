/*
 * Serial messages
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 */

// Value ID's used with value messages
enum SERIAL_MSG_READVALUE_SOURCE
{
    SERIAL_MSG_READVALUE_SOURCE_STATUS  = 0,
    SERIAL_MSG_READVALUE_SOURCE_ANGLE   = 1,
    SERIAL_MSG_READVALUE_SOURCE_ANGERROR= 2,
};

// Status bitfield
enum SERIAL_MSG_STATUS
{
    SERIAL_MSG_STATUS_ENABLED           = 0,            // 0 = Disabled, 1 = Enabled
    SERIAL_MSG_STATUS_MODE              = 1,            // 0 = Open loop, 1 = Closed loop
};

// Parameter ID's used with parameter messages
enum SERIAL_MSG_PARAM_SOURCE
{
    SERIAL_MSG_PARAM_SOURCE_CALFLAG     = 0,
    SERIAL_MSG_PARAM_SOURCE_KP          = 1,
    SERIAL_MSG_PARAM_SOURCE_KI          = 2,
    SERIAL_MSG_PARAM_SOURCE_KD          = 3,
    SERIAL_MSG_PARAM_SOURCE_CURRENT     = 4,
    SERIAL_MSG_PARAM_SOURCE_STEPSIZE    = 5,
    SERIAL_MSG_PARAM_SOURCE_ENDIR       = 6,
    SERIAL_MSG_PARAM_SOURCE_MOTORDIR    = 7,
    
};

// Command ID's used with command messages
enum SERIAL_MSG_COMMAND
{
    SERIAL_MSG_COMMAND_STEP             = 0,            // Software version of a pulse on the STEP pin
    SERIAL_MSG_COMMAND_STEP_FORWARD     = 1,            // Used for testing
    SERIAL_MSG_COMMAND_STEP_BACK        = 2,            // Used for testing
    SERIAL_MSG_COMMAND_MOVE             = 3,            // Move number of steps provided as uint16 (parameter1 and parameter2)
    SERIAL_MSG_COMMAND_STORAGE_SAVE     = 5,            // Save parameters in RAM to Flash
    SERIAL_MSG_COMMAND_MODE_ENABLE      = 10,           // Software version of EN pin activated
    SERIAL_MSG_COMMAND_MODE_DISABLE     = 11,           // Software version of EN pin deactivated
    SERIAL_MSG_COMMAND_MODE_CLOSELOOP   = 12,
    SERIAL_MSG_COMMAND_MODE_OPENLOOP    = 13,
    SERIAL_MSG_COMMAND_STREAM_ANGLE     = 20,           // Starts/stop streaming angle values via UART
    SERIAL_MSG_COMMAND_JUMP_BOOTLOADER  = 30,           // Jump to bootloader in system memory
};

#define SERIAL_MSG_READVALUE        1
struct Serial_Msg_ReadValue
{
    uint8_t source;
}__attribute__((packed));


#define SERIAL_MSG_GETPARAM         2
struct Serial_Msg_GetParam
{
    uint8_t source;
}__attribute__((packed));  


#define SERIAL_MSG_SETPARAM         3
struct Serial_Msg_SetParam
{
    uint8_t source;
    int16_t value;
}__attribute__((packed));  


#define SERIAL_MSG_COMMAND          4
struct Serial_Msg_Command
{
    uint8_t command;
    uint8_t parameter1;
    uint8_t parameter2;
    uint8_t parameter3;
    uint8_t parameter4;
}__attribute__((packed));  


#define SERIAL_MSG_PARAM_KP         10
struct Serial_Msg_Param_Kp
{
    int16_t kp;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_KI         11
struct Serial_Msg_Param_Ki
{
    int16_t ki;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_KD         12
struct Serial_Msg_Param_Kd
{
    int16_t kd;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_CURRENT    13
struct Serial_Msg_Param_Current
{
    // Divide by 8, multiply by 100
    uint8_t current;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_STEPSIZE   14
struct Serial_Msg_Param_StepSize
{
    uint8_t size;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_ENDIR      15
struct Serial_Msg_Param_ENDir
{
    uint8_t direction;
}__attribute__((packed));


#define SERIAL_MSG_PARAM_MOTORDIR   16
struct Serial_Msg_Param_MotorDirection
{
    uint8_t direction;
}__attribute__((packed));


#define SERIAL_MSG_STATUS           20
struct Serial_Msg_Status
{
    uint8_t statusBitField;
}__attribute__((packed));


#define SERIAL_MSG_ANGLE            21
struct Serial_Msg_Angle
{
    float angle;
}__attribute__((packed));


#define SERIAL_MSG_ANGERROR         22
struct Serial_Msg_AngleError
{
    float error;
}__attribute__((packed));