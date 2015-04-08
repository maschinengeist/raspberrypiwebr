
#ifndef __HD44780_H__
#define __HD44780_H__

#ifdef __cplusplus
	extern "C" {
#endif


/*************************************************************************
*   D E F I N E
*************************************************************************/

#define  HD44780_BACKLIGHT_OFF   0
#define  HD44780_BACKLIGHT_ON    1

/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

RESULT HD44780_Init(INT_U8 u08Mode);

void HD44780_WriteCommand(INT_U8 u08Command);

void HD44780_PrintStringXY(CHAR* cText, INT_U8 u08X, INT_U8 u08Y);

void HD44780_PrintString(CHAR* cText);

void HD44780_PrintChar(CHAR cChar, INT_U8 u08X, INT_U8 u08Y);

void HD44780_Clear(void);

void HD44780_Goto(INT_U8 u08Address);

void HD44780_Backlight(INT_U8 u08On);


#ifdef __cplusplus
	}
#endif

#endif
