/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "mytypes.h"
#include "hwtime.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/

static INT_U8 u08Time500Ms = 4;
static INT_U8 u08Time1Sek = 1, u08Time = 0, u08TimeBitsRead = 0;

static volatile INT_U8 u08IrqTime = 0, u08MsTrigger = 0;

/************************************************************************
*/
/*! \brief Basic trigger function, call this function in your hw interrupt
*
*************************************************************************/
static void hwtime_IrqTrigger(void)
{
   u08MsTrigger = 1;

	u08IrqTime |= TIME_100MS;

	if(!u08Time500Ms--)
	{
		u08Time500Ms = 4;
		u08IrqTime |= TIME_500MS;

		if(!u08Time1Sek--)
		{
			u08Time1Sek = 1;
			u08IrqTime |= TIME_1SEK;
		}
	}
}

/************************************************************************
*/
/*! \brief the timmer thread
*
*************************************************************************/
void *TimerThread(void *arg)
{
	do
	{
		hwtime_IrqTrigger();
		usleep(100000);
	}
	while(1);
}

/************************************************************************
*/
/*! \brief start the timer thread
*
*************************************************************************/
void hwtime_Init(void)
{
	pthread_t ThreadID;
	pthread_create(&ThreadID, NULL, TimerThread, NULL);
}


/************************************************************************
*/
/*! \fn hwtime_Set(INT_U8 u08TimeBits)
*
*   \brief set time bits
*
*   \param[in] u08TimeBits --> Time bits to set
*
*************************************************************************/
void hwtime_Get(void)
{
   u08Time = u08IrqTime;
}

/************************************************************************
*/
/*! \fn INT_U8 hwtime_Check(INT_U8 u08TimeBits)
*
*   \brief check current time
*
*   \param[in] u08TimeBits --> Time bits to check
*
*   \return 1 is time bit is true otherwise zero
*
*************************************************************************/
INT_U8 hwtime_Check(INT_U8 u08TimeBits)
{
   if(u08Time & u08TimeBits)
   {
      u08TimeBitsRead |= u08TimeBits;
      return 1;
   }
   return 0;
}


/************************************************************************
*/
/*! \fn INT_U8 hwtime_CheckTime(INT_U8 u08TimeBits)
*
*   \brief check current time
*
*   \param[in] u08TimeBits --> Time bits to check
*
*   \return 1 is time bit is true otherwise zero
*
*************************************************************************/
void hwtime_Clear(void)
{
   u08IrqTime &= ~u08TimeBitsRead;
   u08TimeBitsRead = 0;
}

/************************************************************************
*/
/*! \fn void hwtime_Sleep(INT_U16 u16Time)
*
*   \brief sleep time in milliseconds
*
*   \param[in] u16Time --> time in milliseconds
*
*************************************************************************/
void hwtime_Sleep(INT_U16 u16Time)
{
   while(u16Time--)
   {
      /* wait for one millisecond tick */
      while(!u08MsTrigger);
      u08MsTrigger = 0;
   }
}


/************************************************************************
*/
/*! \fn void hwtime_SleepUs(INT_U16 u16Time)
*
*   \brief sleep time in microseconds
*
*   \param[in] u16Time --> time in microseconds
*
*************************************************************************/
void hwtime_SleepUs(INT_U16 u16Time)
{

}

