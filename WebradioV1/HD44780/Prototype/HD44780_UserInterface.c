
#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "HD44780_UserInterface.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   F U N C T I O N
*************************************************************************/

/***********************************************************************
*/
/*! \fn RESULT HD44780_UserInterface_InitGPIOs(void)
*
*   \brief Initialize the GPIOs for the Display
*
*
*   \return SMA_OK    --> GPIOs are initialize
*   \return SMA_ERROR --> an error occur during initializing
*
*************************************************************************/
RESULT HD44780_UserInterface_InitGPIOs(void)
{
   RESULT Result = SB_ERROR;

   return Result;
}

/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SetRS(INT_U8 u08State)
*
*   \brief Set or clear the RS (Register Select) pin at the display
*
*   \param u08State --> 0 = RS pin islow
*   \param          --> 1 = RS pin is high
*
*************************************************************************/
void HD44780_UserInterface_SetRS(INT_U8 u08State)
{

}

/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SetE(INT_U8 u08State)
*
*   \brief Set or clear the E (Enable) pin at the display
*
*   \param u08State --> 0 = E pin is low
*   \param          --> 1 = E pin is high
*
*************************************************************************/
void HD44780_UserInterface_SetE(INT_U8 u08State)
{

}

/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SetRW(INT_U8 u08State)
*
*   \brief Set or clear the RW (Read Write) pin at the display
*
*   \param u08State --> 0 = RW pin is low (write aktiv)
*   \param          --> 1 = RW pin is high (read aktiv)
*
*************************************************************************/
void HD44780_UserInterface_SetRW(INT_U8 u08State)
{

}

/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SetDataLine(INT_U8 u08Data)
*
*   \brief Set Data Pins at the Interface
*
*   \param u08Data --> in 8bit modus data bits 0 to 7 are aktiv
*   \param         --> in 4bit modus data bits 4 to 7 are aktiv
*   \param             and must connect to the display lines 4 to 7
*
*************************************************************************/
void HD44780_UserInterface_SetDataLine(INT_U8 u08Data)
{

}


/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SetBacklight(INT_U8 u08Backlight)
*
*   \brief Set Data Pins at the Interface
*
*   \param u08Backlight --> 0 = backlight off, 1= backlight on
*
*************************************************************************/
void HD44780_UserInterface_SetBacklight(INT_U8 u08Backlight)
{

}



/***********************************************************************
*/
/*! \fn void HD44780_UserInterface_SleepMs(INT_U16 u16SleepInMs)
*
*   \brief Sleep in milliseconds
*
*   \param u16Sleep --> Sleep time in milliseconds
*
*************************************************************************/
void HD44780_UserInterface_SleepMs(INT_U16 u16SleepInMs)
{

}

#ifdef __cplusplus
	}
#endif


