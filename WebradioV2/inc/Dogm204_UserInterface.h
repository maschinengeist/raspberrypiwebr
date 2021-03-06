

#ifndef __DOGM204_USERINTERFACE_H__
#define __DOGM204_USERINTERFACE_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

RESULT Dogm204_UserInterface_InitGPIOs(void);

void Dogm204_UserInterface_SetReset(INT_U8 u08State);

void Dogm204_UserInterface_WriteI2C(INT_U8* p08Data, INT_U8 u08Size);

void Dogm204_UserInterface_SetBacklight(INT_U8 u08Backlight);

void Dogm204_UserInterface_SleepMs(INT_U16 u16SleepInMs);

void Dogm204_UserInterface_SleepUs(INT_U16 u16SleepInUs);

INT_U8 Dogm204_DisplayWriteText(INT_U8 u08Row, char* cText);

void Dogm204_DisplayMove(INT_U8 u08Row, INT_U8 u08Offset, INT_U8 u08WindowWidth);

void Dogm204_DisplayPrint(INT_U8 u08Row, INT_U8 u08Offset);

#ifdef __cplusplus
	}
#endif

#endif

