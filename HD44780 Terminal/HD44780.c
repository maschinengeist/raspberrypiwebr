
#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "string.h"
#include "stdio.h"

#include "mytypes.h"
#include "HD44780.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define CLEARSCR          "\x1B[2J\x1B[;H"    //!< Clear screen.
#define CLEAREOL          "\x1B[K"            //!< Clear end of line.
#define CLEAREOS          "\x1B[J"            //!< Clear end of screen.
#define CLEARLCR          "\x1B[0K"           //!< Clear line cursor right.
#define CLEARLCL          "\x1B[1K"           //!< Clear line cursor left.
#define CLEARELN          "\x1B[2K"           //!< Clear entire line.
#define CLEARCDW          "\x1B[0J"           //!< Clear cursor down.
#define CLEARCUP          "\x1B[1J"           //!< Clear cursor up.
#define GOTOYX            "\x1B[%.2d;%.2dH"   //!< Set cursor to (y, x).
#define INSERTMOD         "\x1B[4h"           //!< Insert mode.
#define OVERWRITEMOD      "\x1B[4l"           //!< Overwrite mode.
#define DELAFCURSOR       "\x1B[K"            //!< Erase from cursor to end of line.
#define CRLF              "\r\n"              //!< Carriage Return + Line Feed.
//! @}

/*! \name VT100 Cursor Commands
 */
//! @{
#define CURSON            "\x1B[?25h"         //!< Show cursor.
#define CURSOFF           "\x1B[?25l"         //!< Hide cursor.
//! @}

/*! \name VT100 Character Commands
 */
//! @{
#define NORMAL            "\x1B[0m"           //!< Normal.
#define BOLD              "\x1B[1m"           //!< Bold.
#define UNDERLINE         "\x1B[4m"           //!< Underline.
#define BLINKING          "\x1B[5m"           //!< Blink.
#define INVVIDEO          "\x1B[7m"           //!< Inverse video.
//! @}

/*! \name VT100 Color Commands
 */
//! @{
#define CL_BLACK          "\033[22;30m"       //!< Black.
#define CL_RED            "\033[22;31m"       //!< Red.
#define CL_GREEN          "\033[22;32m"       //!< Green.
#define CL_BROWN          "\033[22;33m"       //!< Brown.
#define CL_BLUE           "\033[22;34m"       //!< Blue.
#define CL_MAGENTA        "\033[22;35m"       //!< Magenta.
#define CL_CYAN           "\033[22;36m"       //!< Cyan.
#define CL_GRAY           "\033[22;37m"       //!< Gray.
#define CL_DARKGRAY       "\033[01;30m"       //!< Dark gray.
#define CL_LIGHTRED       "\033[01;31m"       //!< Light red.
#define CL_LIGHTGREEN     "\033[01;32m"       //!< Light green.
#define CL_YELLOW         "\033[01;33m"       //!< Yellow.
#define CL_LIGHTBLUE      "\033[01;34m"       //!< Light blue.
#define CL_LIGHTMAGENTA   "\033[01;35m"       //!< Light magenta.
#define CL_LIGHTCYAN      "\033[01;36m"       //!< Light cyan.
#define CL_WHITE          "\033[01;37m"       //!< White.

#define BC_BLACK           "\033[01;40m"       //!< Black.

#if 0
                                       40 black background
                                       41 red background
                                       42 green background
                                       43 yellow background
                                       44 blue background
                                       45 magenta background
                                       46 cyan background
                                       47 white background
                                       49 default background
#endif

/*************************************************************************
*   G L O B A L
*************************************************************************/


/*************************************************************************
*   S T A T I C   F U N K T I O N
*************************************************************************/

static INT_U8 Terminal[8][40], u08PosX = 0, u08PosY = 0;



void HD44780_Print(void)
{
	INT_U8 u08Loop;

	printf(GOTOYX,1 ,1);
	for(u08Loop = 0 ; u08Loop < 20 ; u08Loop++)
	{
		printf("%c", Terminal[0][u08Loop]);
	}

	printf(GOTOYX,2 ,1);
	for(u08Loop = 0 ; u08Loop < 20 ; u08Loop++)
	{
		printf("%c", Terminal[1][u08Loop]);
	}

	printf(GOTOYX,3 ,1);
	for(u08Loop = 0 ; u08Loop < 20 ; u08Loop++)
	{
		printf("%c", Terminal[2][u08Loop]);
	}

	printf(GOTOYX,4 ,1);
	for(u08Loop = 0 ; u08Loop < 20 ; u08Loop++)
	{
		printf("%c", Terminal[3][u08Loop]);
	}

	printf("\r\n\r\n\r\n\r\n\r\n\r\n");
}


/***********************************************************************
*/
/*! \fn RESULT HD44780_Init(INT_U8 u08Mode)
*
*   \brief Initialize a HD 44780 compatible Display in 8 or 4 bit mode.
*
*   \param u08Mode --> 0 = 8bit mode aktiv
*   \param         --> 1 = 4bit mode aktiv
*
*   \return SB_OK    --> display is initialize
*   \return SB_ERROR --> an error occur during initializing
*
*************************************************************************/
RESULT HD44780_Init(INT_U8 u08Mode)
{
	RESULT Result = SB_OK;

	u08PosX = u08PosY = 0;
	memset(Terminal, 0x20, sizeof(Terminal));

	printf(CLEARSCR);

	printf(GOTOYX,0 ,0);

	HD44780_Print();

	return Result;
}




/***********************************************************************
*/
/*! \fn void HD44780_WriteString(CHAR* cText)
*
*   \brief write a string to the display
*
*   \param cText --> Nullterminated string to send
*
*************************************************************************/
void HD44780_PrintString(CHAR* cText)
{
   while(*cText)
   {
      Terminal[u08PosX][u08PosY] = *cText++;

      u08PosY++;
      if(u08PosY > 40) u08PosX++;
      if(u08PosX > 8)
		{
			u08PosX = u08PosY = 0;
		}
   }

   HD44780_Print();
}

/***********************************************************************
*/
/*! \fn void HD44780_WriteStringXY(CHAR* cText, INT_U8 u08X, INT_U8 u08Y)
*
*   \brief write a string to the display
*
*   \param cText --> Nullterminated string to send
*
*************************************************************************/
void HD44780_PrintStringXY(CHAR* cText, INT_U8 u08X, INT_U8 u08Y)
{
	u08PosX = u08X;
	u08PosY = u08Y;

   while(*cText)
   {
      Terminal[u08PosX][u08PosY] = *cText++;

      u08PosY++;
      if(u08PosY > 40) u08PosX++;
      if(u08PosX > 8)
		{
			u08PosX = u08PosY = 0;
		}
   }

   HD44780_Print();
}

/***********************************************************************
*/
/*! \fn void HD44780_PrintChar(CHAR cChar, INT_U8 u08X, INT_U8 u08Y)
*
*   \brief write a string to the display
*
*   \param cText --> Nullterminated string to send
*
*************************************************************************/
void HD44780_PrintChar(CHAR cChar, INT_U8 u08X, INT_U8 u08Y)
{
	u08PosX = u08X;
	u08PosY = u08Y;

	Terminal[u08PosX][u08PosY] = cChar;

	u08PosY++;
	if(u08PosY > 40) u08PosX++;
	if(u08PosX > 8)
	{
		u08PosX = u08PosY = 0;
	}

   HD44780_Print();
}


/***********************************************************************
*/
/*! \fn void HD44780_GotoXY(INT_U8 u08X, INT_U8 u08Y)
*
*   \brief goto a address specified by X, Y
*
*   \param u08X --> 0 to 3
*   \param u08Y --> 0 to 19
*
*************************************************************************/
void HD44780_GotoXY(INT_U8 u08X, INT_U8 u08Y)
{
	u08PosX = u08X;
	u08PosY = u08Y;
}


/***********************************************************************
*/
/*! \fn void HD44780_Clear(void)
*
*   \brief clear display
*
*************************************************************************/
void HD44780_Clear(void)
{
	u08PosX = u08PosY = 0;
	memset(Terminal, 0x20, sizeof(Terminal));

   HD44780_Print();
}




/***********************************************************************
*/
/*! \fn void HD44780_Goto(u08Address)
*
*   \brief goto a address specified by u08Address
*
*   \param u08Address --> Address of DDRAM
*
*************************************************************************/
void HD44780_Goto(INT_U8 u08Address)
{

}


/***********************************************************************
*/
/*! \fn HD44780_Backlight(INT_U8 u08On)
*
*   \brief switch backlight on or off
*
*   \param u08On --> HD44780_BACKLIGHT_ON or HD44780_BACKLIGHT_OFF
*
*************************************************************************/
void HD44780_Backlight(INT_U8 u08On)
{
	/* check mode */
	if(u08On == HD44780_BACKLIGHT_ON)
	{
		//printf(BC_BLACK);
		//printf(CL_WHITE);
	}
	else
	{

	}
}

#ifdef __cplusplus
	}
#endif
