

#ifndef __HD44780_USERINTERFACE_H__
#define __HD44780_USERINTERFACE_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

RESULT HD44780_UserInterface_InitGPIOs(void);

void HD44780_UserInterface_SetRS(INT_U8 u08State);

void HD44780_UserInterface_SetE(INT_U8 u08State);

void HD44780_UserInterface_SetRW(INT_U8 u08State);

void HD44780_UserInterface_SetDataLine(INT_U8 u08Data);

void HD44780_UserInterface_SleepMs(INT_U16 u16SleepInMs);

void HD44780_UserInterface_SetBacklight(INT_U8 u08Backlight);

#ifdef __cplusplus
	}
#endif

#endif

