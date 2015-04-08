
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "mytypes.h"

#include "Dogm204.h"
#include "Dogm204_UserInterface.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define DOGM_COMMAND 0x00
#define DOGM_DATA    0x40
#define DOGM_CONBIT  0x80

/*************************************************************************
*   G L O B A L
*************************************************************************/

static INT_U8 u08Character[8*8] ={0x00,0x00,0x00,0x10,0x10,0x10,0x1f,0x00,
											 0x00,0x00,0x00,0x18,0x18,0x18,0x1f,0x00,
											 0x00,0x00,0x00,0x1c,0x1c,0x1c,0x1f,0x00,
											 0x00,0x00,0x00,0x1e,0x1e,0x1e,0x1f,0x00,
											 0x00,0x00,0x00,0x1f,0x1f,0x1f,0x1f,0x00,
											 0x00,0x00,0x0c,0x13,0x11,0x11,0x11,0x1f,
											 0x10,0x18,0x1c,0x1e,0x1c,0x18,0x10,0x00,
											 0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x00};

/*************************************************************************
*   S T A T I C   F U N K T I O N
*************************************************************************/

/***********************************************************************
*/
/*! \fn void Dogm204_WriteCommand(INT_U8 u08Command)
*
*   \brief write a command to the display
*
*   \param u08Command --> command to send
*
*************************************************************************/
static void Dogm204_WriteCommand(INT_U8 u08Command)
{
   INT_U8 u08Data[2];

   /* enter command mode */
   u08Data[0] = DOGM_COMMAND;
   u08Data[1] = u08Command;
   Dogm204_UserInterface_WriteI2C(&u08Data[0], 2);

   Dogm204_UserInterface_SleepUs(100);
}

/***********************************************************************
*/
/*! \fn void Dogm204_WriteData(INT_U8 u08Data, INT_U8 u08Size)
*
*   \brief write a data to the display
*
*   \param p08Data --> pointer to data to send
*   \param u08Size --> size of data to send
*
*************************************************************************/
static void Dogm204_WriteData(INT_U8* p08Data, INT_U8 u08Size)
{
	#define DATA_BUFFER_SIZE 20
   INT_U8 u08Data[DATA_BUFFER_SIZE + 1], u08Position;

   /* enter data mode */
   u08Data[0] = DOGM_DATA;
   u08Position = 0;

   do
   {
		/* copy data*/
		if(u08Size > DATA_BUFFER_SIZE)
		{
			memcpy(&u08Data[1], &p08Data[u08Position], DATA_BUFFER_SIZE);
			Dogm204_UserInterface_WriteI2C(&u08Data[0], DATA_BUFFER_SIZE + 1);

			u08Size -= DATA_BUFFER_SIZE;
			u08Position += DATA_BUFFER_SIZE;
		}
		else
		{
			memcpy(&u08Data[1], &p08Data[u08Position], u08Size);
			Dogm204_UserInterface_WriteI2C(&u08Data[0], u08Size + 1);

			u08Size = 0;
		}
   }
   while(u08Size);
}

/*************************************************************************
*   F U N K T I O N S
*************************************************************************/

/***********************************************************************
*/
/*! \fn RESULT Dogm204_Init(void)
*
*   \brief Initialize the dohm display.
*
*   \return SB_OK    --> display is initialize
*   \return SB_ERROR --> an error occur during initializing
*
*************************************************************************/
RESULT Dogm204_Init(void)
{
	INT_U8 u08Data;
   RESULT Result = SB_ERROR;

   /* init gpios */
   Result = Dogm204_UserInterface_InitGPIOs();

   if(Result == SB_ERROR) return Result;

   /* reset display */
   Dogm204_UserInterface_SetReset(0);
   Dogm204_UserInterface_SleepMs(50);
   Dogm204_UserInterface_SetReset(1);

   Dogm204_UserInterface_SleepMs(100);

   /* funktion set */
   Dogm204_WriteCommand(0x3a);
   /* extended funktion set */
   Dogm204_WriteCommand(0x09);
   /* entry mode set */
   Dogm204_WriteCommand(0x06);
   /* bias set */
   Dogm204_WriteCommand(0x1e);
   /* funktion set */
   Dogm204_WriteCommand(0x39);
   /* internal osc */
   Dogm204_WriteCommand(0x1b);
   /* follower control */
   Dogm204_WriteCommand(0x6e);
   /* power control */
   Dogm204_WriteCommand(0x57);
   /* contrast set */
   Dogm204_WriteCommand(0x7f);
   /* funktion set */
   Dogm204_WriteCommand(0x38);
   /* display on cursor off */
   Dogm204_WriteCommand(0x0c);
   /* select rom b */
   Dogm204_WriteCommand(0x3a);
   u08Data = 8;
   Dogm204_WriteData(&u08Data, 1);
	Dogm204_WriteCommand(0x38);
   /* clear display */
   Dogm204_WriteCommand(0x01);
   /* load user data for progress bar */
   Dogm204_WriteCharacterData(u08Character, sizeof(u08Character));

   return Result;
}

/***********************************************************************
*/
/*! \brief Write own characters to display
*
*   \param p08Data --> Pointer to character data
*   \param u08Size --> Size to write
*
*************************************************************************/
void Dogm204_WriteCharacterData(INT_U8* p08Data, INT_U8 u08Size)
{
   /* set cgram address */
   Dogm204_WriteCommand(0x38);
   Dogm204_WriteCommand(0x40);

   /* write data to cgram */
   Dogm204_WriteData(p08Data, u08Size);
}

/***********************************************************************
*/
/*! \fn void Dogm204_WriteStringXY(CHAR* cText, INT_U8 u08X, INT_U8 u08Y)
*
*   \brief write a string to x,y position
*
*   \param cText --> Nullterminated string to send
*   \param[in] u08X --> 0 to 3
*   \param[in] u08Y --> 0 to 19
*
*************************************************************************/
void Dogm204_PrintStringXY(char* cText, INT_S8 s08X, INT_S8 s08Y)
{
	/* goto xy */
   Dogm204_Goto(s08X, s08Y);

   /* write data */
   Dogm204_WriteData((INT_U8*)cText, (INT_U8)strlen(cText));
}

/***********************************************************************
*/
/*! \fn void Dogm204_WriteStringXY(CHAR* cText, INT_U8 u08X, INT_U8 u08Y)
*
*   \brief write a string to x,y position
*
*   \param cText --> Nullterminated string to send
*
*************************************************************************/
void Dogm204_PrintString(char* cText)
{
   /* write data */
   Dogm204_WriteData((INT_U8*)cText, (INT_U8)strlen(cText));
}

/***********************************************************************
*/
/*! \fn void Dogm204_PrintChar(CHAR cChar, INT_S8 s08X, INT_S8 s08Y)
*
*   \brief write a char to x,y position
*
*   \param cText --> char to send
*   \param[in] u08X --> 0 to 3
*   \param[in] u08Y --> 0 to 19
*
*************************************************************************/
void Dogm204_PrintChar(char cChar, INT_S8 s08X, INT_S8 s08Y)
{
   /* goto xy */
   Dogm204_Goto(s08X, s08Y);

   /* write data */
   Dogm204_WriteData((INT_U8*)&cChar, 1);
}

/***********************************************************************
*/
/*! \fn void Dogm204_GotoXY(INT_U8 u08X, INT_U8 u08Y)
*
*   \brief goto a address specified by X, Y
*
*   \param u08X --> 0 to 3
*   \param u08Y --> 0 to 19
*
*************************************************************************/
void Dogm204_Goto(INT_S8 s08X, INT_S8 s08Y)
{
   if(s08X < 0) return;
   if(s08Y < 0) return;

   if(s08X > 3) return;
   if(s08Y > 19) return;

   switch(s08X)
   {
      case 0:
         s08X = 0x00;
      break;

      case 1:
         s08X = 0x20;
      break;

      case 2:
         s08X = 0x40;
      break;

      case 3:
         s08X = 0x60;
      break;

      default:
      break;
   }

	/* add y position */
   if(s08Y < 20) s08X += s08Y;

   /* set bit for set address */
   s08X |= 0x80;

   /* set command ddram address */
   Dogm204_WriteCommand(s08X);
}


/***********************************************************************
*/
/*! \fn void Dogm204_Clear(void)
*
*   \brief clear display
*
*************************************************************************/
void Dogm204_Clear(void)
{
   /* clear display */
   Dogm204_WriteCommand(0x01);
   Dogm204_UserInterface_SleepMs(2);
}

/***********************************************************************
*/
/*! \fn Dogm204_Backlight(INT_U8 u08On)
*
*   \brief switch backlight on or off
*
*   \param u08On --> Dogm204_BACKLIGHT_ON or Dogm204_BACKLIGHT_OFF
*
*************************************************************************/
void Dogm204_Backlight(INT_U8 u08On)
{
   /* check mode */
   if(u08On)
   {
      Dogm204_UserInterface_SetBacklight(1);
   }
   else
   {
      Dogm204_UserInterface_SetBacklight(0);
   }
}

/***********************************************************************
*/
/*! \brief Set the big front of the first lines
*
*   \param u08Positon --> 1 = Text wird oben groß angezeigt, 0 = Text wird unten groß angezeigt
*
*************************************************************************/
void Dogm204_BigFront(INT_U8 u08Positon)
{
   Dogm204_WriteCommand(0x3e);
   switch(u08Positon)
   {
   	case 0:
   		Dogm204_WriteCommand(0x18);
   	break;

   	case 1:
   	   Dogm204_WriteCommand(0x10);
   	break;

   	case 2:
   	   Dogm204_WriteCommand(0x10);
   	break;

   	default:
   	break;
   }
}

/***********************************************************************
*/
/*! \brief Set the small front of the first lines
*
*************************************************************************/
void Dogm204_SmallFront(void)
{
   Dogm204_WriteCommand(0x38);
}

/***********************************************************************
*/
/*! \brief Print progress to display
*
*   \param[in] fProgress --> 0 to 100 Progress in %
*   \param[in] u08Wide --> Wide in characters 1 to 20
*   \param[in] u08X --> 0 to 3
*   \param[in] u08Y --> 0 to 19
*
*************************************************************************/
void Dogm204_DisplayProgressBar(float fProgress, INT_U8 u08Wide, INT_S8 s08X, INT_S8 s08Y)
{
   float fPosition;
   char cText[21];

   INT_U8 u08Loop;

   if(u08Wide > 20) return;

   Dogm204_Goto(s08X, s08Y);

   memset(cText, 0x5f, (int)u08Wide);
   cText[u08Wide] = 0;

   fPosition = (fProgress / 100) * (float)(u08Wide * 5);

   for(u08Loop = 0 ; u08Loop < (INT_U8)fPosition ; u08Loop++)
   {
   	cText[(u08Loop / 5)] = (u08Loop % 5 );
   }

   Dogm204_WriteData((INT_U8*)cText, u08Wide);
}


