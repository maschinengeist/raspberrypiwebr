

/*************************************************************************
*   I N C L U D E
*************************************************************************/
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mytypes.h"
#include "gpio.h"
#include "i2c.h"
#include "Dogm204.h"
#include "Dogm204_UserInterface.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/

#define DOGM_GPIO_RESET 12

#define DISPLAY_STRINGLENGTH 64

static char cDisplay[DISPLAY_HIGH][(DISPLAY_WIDTH * 2) + DISPLAY_STRINGLENGTH];
static int iDisplayStringLength[DISPLAY_HIGH] = {0, 0, 0, 0};

static INT_U8 u08DisplayMovePosition = DISPLAY_WIDTH;

/*************************************************************************
*   F U N C T I O N
*************************************************************************/

/***********************************************************************
*/
/*! \fn RESULT Dogm204_UserInterface_InitGPIOs(void)
*
*   \brief Initialize the GPIOs for the Display
*
*
*   \return SB_OK --> GPIOs are initialize
*   \return SB_ERROR --> an error occur during initializing
*
*************************************************************************/
RESULT Dogm204_UserInterface_InitGPIOs(void)
{
   RESULT Result = SB_OK;

   	/* init rorary switchs ios */
	gpio_Init(DOGM_GPIO_RESET, GPIO_DIRECTION_OUT, GPIO_EDGE_NONE);
	gpio_Init(4, GPIO_DIRECTION_OUT, GPIO_EDGE_NONE);
	gpio_Write(4, 1);
	gpio_Write(DOGM_GPIO_RESET, 1);

   return Result;
}

/***********************************************************************
*/
/*! \fn void Dogm204_UserInterface_SetReset(INT_U8 u08State)
*
*   \brief Set or clear the Reset pin at the display
*
*   \param u08State --> 0 = Reset pin is low (write aktiv)
*   \param          --> 1 = Reset pin is high (read aktiv)
*
*************************************************************************/
void Dogm204_UserInterface_SetReset(INT_U8 u08State)
{
		gpio_Write(DOGM_GPIO_RESET, u08State);
}

/***********************************************************************
*/
/*! \fn void Dogm204_UserInterface_WriteI2C(INT_U8 u08Data, INT_U8 u08Size)
*
*   \brief Write data to the i2c interface
*
*   \param p08Data --> Pointer to i2c data
*   \param u08Size --> Size of data to send
*
*************************************************************************/
void Dogm204_UserInterface_WriteI2C(INT_U8* p08Data, INT_U8 u08Size)
{
	int iSize;
	/* write data to bus */
	do
	{
		iSize = i2c_Write("/dev/i2c-1", 0x3c, p08Data, (INT_U16)u08Size);
		if(iSize == -1)
		{
			printf("i2c error\n");
			Dogm204_UserInterface_SleepMs(100);
		}
	}
   while(iSize == -1);
}


/***********************************************************************
*/
/*! \fn void Dogm204_UserInterface_SetBacklight(INT_U8 u08Backlight)
*
*   \brief Set backlight on or off
*
*   \param u08Backlight --> 0 = backlight off, 1= backlight on
*
*************************************************************************/
void Dogm204_UserInterface_SetBacklight(INT_U8 u08Backlight)
{
	if(u08Backlight) gpio_Write(4, 1);
	else gpio_Write(4, 0);
}



/***********************************************************************
*/
/*! \fn void Dogm204_UserInterface_SleepMs(INT_U16 u16SleepInMs)
*
*   \brief Sleep in milliseconds
*
*   \param u16Sleep --> Sleep time in milliseconds
*
*************************************************************************/
void Dogm204_UserInterface_SleepMs(INT_U16 u16SleepInMs)
{
	usleep((int)u16SleepInMs * 1000);
}

/***********************************************************************
*/
/*! \fn void Dogm204_UserInterface_SleepUs(INT_U16 u16SleepInUs)
*
*   \brief Sleep in microseconds
*
*   \param u16Sleep --> Sleep time in microseconds
*
*************************************************************************/
void Dogm204_UserInterface_SleepUs(INT_U16 u16SleepInUs)
{
	usleep((int)u16SleepInUs);
}


/***********************************************************************
*/
/*! \brief Write text into display buffer
*
*   \param[in] u08Row --> The row to load
*   \param[in] cText --> Pointer to Text
*
*   \return --> 1 = load text, 0 = text dosen't fit or no available row
*
*************************************************************************/
INT_U8 Dogm204_DisplayWriteText(INT_U8 u08Row, char* cText)
{
   iDisplayStringLength[u08Row] = strlen(cText);

   if((u08Row < DISPLAY_HIGH) && (iDisplayStringLength[u08Row] < DISPLAY_STRINGLENGTH - 1))
   {
      u08DisplayMovePosition = DISPLAY_WIDTH;
      memset(&cDisplay[u08Row][0], 0x20, (DISPLAY_WIDTH * 2) + DISPLAY_STRINGLENGTH);
      memcpy(&cDisplay[u08Row][DISPLAY_WIDTH], cText, iDisplayStringLength[u08Row]);
      return 1;
   }

   return 0;
}


/***********************************************************************
*/
/*! \brief Move the current display row one positonen left
*
*   \param[in] u08Row --> The row to move
*   \param[in] u08Offset --> The display offset (normaly null)
*   \param[in] u08WindowWidth --> The Window width, if textsize greater than the text will move
*
*************************************************************************/
void Dogm204_DisplayMove(INT_U8 u08Row, INT_U8 u08Offset, INT_U8 u08WindowWidth)
{
   char cText[(DISPLAY_WIDTH + 1)];

   /* check max offset */
   if(u08Offset > DISPLAY_WIDTH) return;

   cText[DISPLAY_WIDTH - u08Offset] = 0;
   memcpy(&cText[0], &cDisplay[u08Row][u08DisplayMovePosition], (DISPLAY_WIDTH - u08Offset));
   Dogm204_PrintStringXY(cText, u08Row, u08Offset);

   if(iDisplayStringLength[u08Row] > u08WindowWidth) u08DisplayMovePosition++;
   if(u08DisplayMovePosition > (iDisplayStringLength[u08Row] + DISPLAY_WIDTH)) u08DisplayMovePosition = 0;
}

/***********************************************************************
*/
/*! \brief Print the current display content to display
*
*   \param[in] u08Row --> The row to move
*   \param[in] u08Offset --> The display offset (normaly null)
*
*************************************************************************/
void Dogm204_DisplayPrint(INT_U8 u08Row, INT_U8 u08Offset)
{
   char cText[(DISPLAY_WIDTH + 1)];

   /* check max offset */
   if(u08Offset > DISPLAY_WIDTH) return;

   cText[DISPLAY_WIDTH - u08Offset] = 0;
   memcpy(&cText[0], &cDisplay[u08Row][u08DisplayMovePosition], (DISPLAY_WIDTH - u08Offset));
   Dogm204_PrintStringXY(cText, u08Row, u08Offset);
}




