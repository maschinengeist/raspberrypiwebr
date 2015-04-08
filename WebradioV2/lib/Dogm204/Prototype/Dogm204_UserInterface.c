

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "Dogm204_UserInterface.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

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
   RESULT Result = SB_ERROR;

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

}



