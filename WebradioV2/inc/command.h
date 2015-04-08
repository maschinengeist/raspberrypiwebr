#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

enum
{
    CMD_UP = 1,
    CMD_DOWN,
    CMD_RIGHT,
    CMD_LEFT,
    CMD_MIDDLE,
    CMD_SETUP,
    CMD_POWER_SLEEP,
    CMD_POWER_OFF,
    CMD_SWITCH1,
    CMD_SWITCH2,
    CMD_SWITCH3,
    CMD_SWITCH4,
    CMD_SWITCH1_SET,
    CMD_SWITCH2_SET,
    CMD_SWITCH3_SET,
    CMD_SWITCH4_SET,
    CMD_DISPLAY_UPDATE
};



/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

void command_Get(void);

void command_DisplayUpdate(void);

INT_U8 command_Read(void);

void command_Clear(void);

#ifdef __cplusplus
        }
#endif

#endif


