
#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "HD44780.h"
#include "HD44780_UserInterface.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/


/*************************************************************************
*   S T A T I C   F U N K T I O N
*************************************************************************/

static void HD44780_WriteData4BitMode(INT_U8 u08Data);

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
   RESULT Result = SB_ERROR;

   /* init gpios */
   Result = HD44780_UserInterface_InitGPIOs();
   if(Result == SB_ERROR) return Result;

   /* set RS, E and R/W to default state */
   HD44780_UserInterface_SetRS(0);
   HD44780_UserInterface_SetE(0);
   HD44780_UserInterface_SetRW(0);

   /* wait more than 40ms */
   HD44780_UserInterface_SleepMs(100);

   /* display initialize, enter 8 bit mode */
   HD44780_UserInterface_SetDataLine(0x30);
   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetE(0);

   /* wait more than 4.1ms and enter 8 bit mode */
   HD44780_UserInterface_SleepMs(5);
   HD44780_UserInterface_SetDataLine(0x30);

   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetE(0);

   /* wait more than 100us and enter 8 bit mode */
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetDataLine(0x30);

   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetE(0);

   /* got into 4 bit mode */
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetDataLine(0x20);

   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SleepMs(2);
   HD44780_UserInterface_SetE(0);

   /* now we are in the 4 bit mode*/
   HD44780_WriteData4BitMode(0x28);
   HD44780_UserInterface_SleepMs(2);
   /* display off */
   HD44780_WriteData4BitMode(0x08);
   HD44780_UserInterface_SleepMs(2);
   /* clear display */
   HD44780_WriteData4BitMode(0x01);
   HD44780_UserInterface_SleepMs(2);
   /* entry mode set */
   HD44780_WriteData4BitMode(0x06);
   HD44780_UserInterface_SleepMs(2);
   /* display on, cursor off */
   HD44780_WriteData4BitMode(0x0c);
   HD44780_UserInterface_SleepMs(2);
   Result = SB_OK;

   return Result;
}



/***********************************************************************
*/
/*! \fn void HD44780_WriteCommand(INT_U8 u08Command)
*
*   \brief write a command to the display
*
*   \param u08Command --> command to send
*
*************************************************************************/
void HD44780_WriteCommand(INT_U8 u08Command)
{
   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   HD44780_WriteData4BitMode(u08Command);
}

/***********************************************************************
*/
/*! \fn void HD44780_WriteData4BitMode(INT_U8 u08Data)
*
*   \brief write data in 4bit mode to the display
*
*   \param u08Data --> data to write, will be send in to steps.
*
*************************************************************************/
static void HD44780_WriteData4BitMode(INT_U8 u08Data)
{
   /* transfere high nibble */
   HD44780_UserInterface_SetDataLine(u08Data);

   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SetE(0);

   /* transfere low nibble */
   HD44780_UserInterface_SetDataLine((u08Data << 4));

   HD44780_UserInterface_SetE(1);
   HD44780_UserInterface_SetE(0);
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
   /* enter data mode */
   HD44780_UserInterface_SetRS(1);

   while(*cText)
   {
      HD44780_WriteData4BitMode(*cText++);
   }
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
   switch(u08X)
   {
      case 0:
         u08X = 0x00;
      break;

      case 1:
         u08X = 0x40;
      break;

      case 2:
         u08X = 0x14;
      break;

      case 3:
         u08X = 0x54;
      break;

      default:
         u08X = 0x00;
      break;
   }

	/* add y position */
   if(u08Y < 20) u08X |= u08Y;

   /* set bit for set address */
   u08X |= 0x80;

   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   /* set command ddram address */
   HD44780_WriteData4BitMode(u08X);

   /* enter data mode */
   HD44780_UserInterface_SetRS(1);

   while(*cText)
   {
      HD44780_WriteData4BitMode(*cText++);
   }
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
   switch(u08X)
   {
      case 0:
         u08X = 0x00;
      break;

      case 1:
         u08X = 0x40;
      break;

      case 2:
         u08X = 0x14;
      break;

      case 3:
         u08X = 0x54;
      break;

      default:
         u08X = 0x00;
      break;
   }

	/* add y position */
   if(u08Y < 20) u08X |= u08Y;

   /* set bit for set address */
   u08X |= 0x80;

   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   /* set command ddram address */
   HD44780_WriteData4BitMode(u08X);

   /* enter data mode */
   HD44780_UserInterface_SetRS(1);

	HD44780_WriteData4BitMode(cChar);
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
   switch(u08X)
   {
      case 0:
         u08X = 0x00;
      break;

      case 1:
         u08X = 0x40;
      break;

      case 2:
         u08X = 0x14;
      break;

      case 3:
         u08X = 0x54;
      break;

      default:
         u08X = 0x00;
      break;
   }

	/* add y position */
   if(u08Y < 20) u08X |= u08Y;

   /* set bit for set address */
   u08X |= 0x80;

   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   HD44780_WriteData4BitMode(u08X);
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
   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   /* clear display */
   HD44780_WriteData4BitMode(0x01);

   HD44780_UserInterface_SleepMs(1);
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
   /* enter command mode */
   HD44780_UserInterface_SetRS(0);

   /* set command ddram address */
   u08Address |= 0x80;

   HD44780_WriteData4BitMode(u08Address);
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
      HD44780_UserInterface_SetBacklight(1);
   }
   else
   {
      HD44780_UserInterface_SetBacklight(0);
   }
}

#ifdef __cplusplus
	}
#endif
