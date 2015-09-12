/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>

#include "mytypes.h"

#include "mode.h"
#include "mode_Power.h"
#include "mode_Play.h"
#include "mode_Settings.h"
#include "mode_Newsstation.h"
#include "mode_Alarmclock.h"

#include "station.h"
#include "hwtime.h"
#include "command.h"
#include "mode.h"
#include "io.h"


/*************************************************************************
*   S T A T I C
*************************************************************************/

static INT_U8 u08CurrentMode = MODE_POWERON;
static INT_U8 u08ModeState = 0;

static INT_U16 u16CurrentTimeout = 0, u16ModeTimeout = 0;
static INT_U8 u08ModeInit = 0;


/************************************************************************
*/
/*! \brief Set current timeout and give init and timeout state back
*
*   \param[in] u16Timeout --> Timeout in seconds
*   \return INT_U8 --> MODE_INIT only after mode change was called,
*                      MODE_TIMEOUT when timeout occure
*                      MODE_NORMAL when nothing happends
*
*************************************************************************/
INT_U8 mode_Init(INT_U16 u16Timeout)
{
   if(u08ModeInit == 0)
   {
      u08ModeInit = 1;
      u16CurrentTimeout = u16ModeTimeout = u16Timeout;

      return MODE_INIT;
   }
   else
   {
      /* Count timeout down */
      if(hwtime_Check(TIME_1SEK))
      {
	      if(u16ModeTimeout)
	      {
		      u16ModeTimeout--;
	      }
         else
         {
            return MODE_TIMEOUT;
         }
      }
   }

   return MODE_NORMAL;
}

void mode_TimeoutTrigger(void)
{
   u16ModeTimeout = u16CurrentTimeout;
}

/************************************************************************
*/
/*! \brief Change the current mode
*
*   \param[in] u08Mode --> Mode to set, see mode.h
*
*************************************************************************/
void mode_Change(INT_U8 u08Mode)
{
   u08CurrentMode = u08Mode;
   u08ModeInit = 0;
}

/************************************************************************
*/
/*! \brief Write current state for statemachine
*
*   \param[in] u08State --> State to set, see mode.h
*
*************************************************************************/
void mode_WriteState(INT_U8 u08State)
{
   u08ModeState = u08State;
}

/************************************************************************
*/
/*! \brief Read current state for statemachine
*
*   \return INT_U8 --> State to set, see mode.h
*
*************************************************************************/
INT_U8 mode_ReadState(void)
{
   return u08ModeState;
}


/************************************************************************
*/
/*! \brief Get current mode
*
*   \return INT_U8 --> Current mode, see mode.h
*
*************************************************************************/
INT_U8 mode_Read(void)
{
   return u08CurrentMode;
}

/************************************************************************
*/
/*! \brief Get current mode
*
*   \return INT_U8 --> Current mode, see mode.h
*
*************************************************************************/
void mode_IncPosition(INT_U8* p08Position1, INT_U8* p08Position2, INT_U8 u08MaxRange, INT_U8 u08BorderToIncPosition2)
{
	if((p08Position1[0] + p08Position2[0]) < u08MaxRange)
	{
		if(p08Position1[0] < u08BorderToIncPosition2) p08Position1[0]++;
		else p08Position2[0]++;
	}
}

void mode_DecPosition(INT_U8* p08Position1, INT_U8* p08Position2, INT_U8 u08MinRange, INT_U8 u08BorderToDecPosition2)
{
	if((p08Position1[0] + p08Position2[0]) > u08MinRange)
	{
		if(p08Position1[0] > u08BorderToDecPosition2) p08Position1[0]--;
		else p08Position2[0]--;
	}
}

void mode_Inc(INT_U8* p08Value, INT_U8 u08MaxValue)
{
	if(p08Value[0] < u08MaxValue) p08Value[0]++;
}

void mode_Dec(INT_U8* p08Value, INT_U8 u08MinValue)
{
	if(p08Value[0] > u08MinValue) p08Value[0]--;
}

/*************************************************************************
*
*************************************************************************/
void mode(void)
{
   /* Read current commands */
   hwtime_Get();
   command_Get();

	switch(mode_Read())
	{
      case MODE_POWERON:
         mode_Poweron();
		break;

		case MODE_PLAY:
         mode_Play();
		break;

		case MODE_STATION:
         mode_Station();
		break;

		case MODE_CHANGE_STATION:
         mode_ChangeStation();
		break;

		case MODE_SETTINGS:
         mode_Selection();
		break;

		case MODE_SETTINGS_STARTVOLUME:
         mode_Startvolume();
		break;

		case MODE_SETTINGS_STATION_NAME:
         mode_StationName();
		break;

		case MODE_SETTINGS_STATION_ADDRESS:
         mode_StationAddress();
		break;

		case MODE_SETTINGS_NETWORK_RSSI:
         mode_NetworkRssi();
		break;

		case MODE_SETTINGS_NETWORK_KEY:
         mode_NetworkKey();
		break;

		case MODE_SETTINGS_STATION_CLEAR:
         mode_StationClear();
		break;

		case MODE_SETTINGS_NAME_EDIT:
         mode_NameEdit();
		break;

		case MODE_NEWS_SELECTION:
         mode_NewsSelection();
		break;

		case MODE_NEWS_STATION:
         mode_NewsStation();
		break;

		case MODE_NEWS_STARTTIME:
         mode_NewsStarttime();
		break;

		case MODE_NEWS_DURATION:
         mode_NewsDuration();
		break;

		case MODE_NEWS_INTERVAL:
         mode_NewsInterval();
		break;

		case MODE_ALARM_SELECTION:
         mode_AlarmSelection();
		break;

		case MODE_ALARM_TIME:
         mode_AlarmTime();
		break;

      case MODE_POWEROFF:
         mode_Poweroff();
		break;

      case MODE_STANDBY:
         mode_Standby();
		break;

      case MODE_SLEEP:
         mode_Sleep();
		break;


		default:
		break;
	}

   /* clear all commands */
   command_Clear();
   hwtime_Clear();
}


