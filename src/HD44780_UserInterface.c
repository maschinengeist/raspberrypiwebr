
#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <unistd.h>
#include <stdio.h>

#include "mytypes.h"
#include "HD44780_UserInterface.h"
#include "i2c.h"
#include "debug.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/


/*************************************************************************
*   G L O B A L
*************************************************************************/

static INT_U8 u08PortValue = 0;
static INT_U8 u08Mode = 0;
static INT_U8 u08RsState = 0;

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
   RESULT iResult = SB_ERROR;

	/* set port to zero */
	iResult = i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);

	/* read data from port */
	////iResult = i2c_Read("/dev/i2c-0", 0x20, &u08Buffer[1], 1);

   return iResult;
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
    /* clear or set rs*/
    if(u08State) u08PortValue |= 0x02;
    else u08PortValue &= ~0x02;

    (void)i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);
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
    /* clear or set e */
    if(u08State) u08PortValue |= 0x08;
    else u08PortValue &= ~0x08;

    (void)i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);
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
    /* clear or set rw*/
    if(u08State) u08PortValue |= 0x04;
    else u08PortValue &= ~0x04;

    (void)i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);
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
	/* clear or set upper bits */
	u08PortValue &= 0x0f;
	u08PortValue |= (u08Data & 0xf0);

	/* write port bits */
   (void)i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);
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
    /* clear or set rw*/
    if(u08Backlight) u08PortValue |= 0x01;
    else u08PortValue &= ~0x01;

    (void)i2c_Write("/dev/i2c-0", 0x20, &u08PortValue, 1);
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
	usleep((int)u16SleepInMs * 1000);
}


#ifdef __cplusplus
	}
#endif

