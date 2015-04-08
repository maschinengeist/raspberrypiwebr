
#ifndef __DOGM204_H__
#define __DOGM204_H__

#ifdef __cplusplus
	extern "C" {
#endif

#define 	DISPLAY_WIDTH 	20
#define 	DISPLAY_HIGH 	4

enum
{
	BACKLIGHT_OFF = 0,
   BACKLIGHT_ON
};

RESULT Dogm204_Init(void);

void Dogm204_WriteCharacterData(INT_U8* p08Data, INT_U8 u08Size);

void Dogm204_PrintStringXY(char* cText, INT_S8 s08X, INT_S8 s08Y);

void Dogm204_PrintString(char* cText);

void Dogm204_PrintChar(char cChar, INT_S8 s08X, INT_S8 s08Y);

void Dogm204_Clear(void);

void Dogm204_Goto(INT_S8 s08X, INT_S8 s08Y);

void Dogm204_Backlight(INT_U8 u08On);

void Dogm204_BigFront(INT_U8 u08Positon);

void Dogm204_SmallFront(void);

void Dogm204_DisplayProgressBar(float fProgress, INT_U8 u08Wide, INT_S8 s08X, INT_S8 s08Y);

#ifdef __cplusplus
	}
#endif

#endif
