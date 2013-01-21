/*! \file Menue.c
*/

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <linux/types.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "mytypes.h"
#include "debug.h"

#include "Station.h"
#include "gpio.h"
#include "message.h"
#include "Network.h"
#include "Menue.h"
#include "Menue_OnOff.h"
#include "Menue_Play.h"
#include "Mplayer.h"
#include "Settings.h"
#include "Directory.h"
#include "amixer.h"

#include "HD44780.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/

#define NETWORK_PATH "Network.conf"

#define SETTINGS_PATH "/Settings"

#define STATION_PATH "/Station"

/*************************************************************************
*   G L O B A L
*************************************************************************/

/*************************************************************************
*   M E N U E   S E T U P
*************************************************************************/
void Menue_Setup(WEBRADIO *pWebRadio)
{
	static INT_U8 u08Position = 0;

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		HD44780_Backlight(1);

		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);

		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16BacklightTimeout = 2000;
		pWebRadio->u16Timeout = 800;

		u08Position = 0;
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		if(u08Position < 3) u08Position++;

		if(u08Position == 2 ) u08Position = 3;
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		if(u08Position > 0) u08Position--;
		if(u08Position == 2 ) u08Position = 1;
	}

	if(Message_Read() == MESSAGE_KEY_SETUP)
	{
	    switch(u08Position)
	    {
            case 0:
                /* Quit webradio */
                pWebRadio->u08MenueState = MENUE_OFF;
                pWebRadio->u08MenueInit = 1;
            break;

            default:
            break;
	    }
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
	    switch(u08Position)
	    {
            case 1:
                /* News station setup */
                pWebRadio->u08MenueState = MENUE_NEWSSTATION_SET_STATION;
                pWebRadio->u08MenueInit = 1;
            break;

            case 2:
                /* Wlan Settings */
                pWebRadio->u08MenueState = MENUE_NET_SETUP;
                pWebRadio->u08MenueInit = 1;
            break;

            case 3:
                /* Ethernet Settings */
                pWebRadio->u08MenueState = MENUE_PLAY;
                pWebRadio->u08MenueInit = 1;
            break;

            default:
            break;
	    }
	}

    if(pWebRadio->u16Timeout == 0)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_PLAY;
		pWebRadio->u08MenueInit = 1;
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	/* was there some actions */
	if(Message_Read())
	{
		INT_U8 u08Loop;

		HD44780_PrintStringXY("Off                ", 0, 1);
		HD44780_PrintStringXY("News Station       ", 1, 1);
		HD44780_PrintStringXY("                   ", 2, 1);
		HD44780_PrintStringXY("Back               ", 3, 1);

		for(u08Loop = 0 ; u08Loop < 4 ; u08Loop++)
		{
			if(u08Position == u08Loop) HD44780_PrintStringXY(">", u08Loop, 0);
			else  HD44780_PrintStringXY(" ", u08Loop, 0);
		}

		/* set timeout of start value */
		pWebRadio->u16Timeout = 800;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}

/*************************************************************************
*   N E W S   S T A T I O N   S E T   S T A T I O N
*************************************************************************/

void Menue_NewsStation_SetStation(WEBRADIO *pWebRadio)
{
	static INT_U8 u08Position = 1, u08OldStation = 1;

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;

		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		if((u08Position < 3) && (u08Position < pWebRadio->u08MaxChannel))
		{
			u08Position++;
		}
		else
		{
			if((u08OldStation + u08Position) < pWebRadio->u08MaxChannel) u08OldStation++;
		}
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		if(u08Position > 1)
		{
			u08Position--;
		}
		else
		{
			if(u08OldStation > 1) u08OldStation--;
		}
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_NEWSSTATION_SET_TIME;
      pWebRadio->u08MenueInit = 1;

		pWebRadio->u08NewsStatio = u08OldStation + u08Position;
	}

   if(pWebRadio->u16Timeout == 0)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_PLAY;
		pWebRadio->u08MenueInit = 1;
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	/* was there some actions */
	if(Message_Read())
	{
		INT_U8 u08Loop, u08Length;
		DIRECTORY_CONTENTS DirEntries;
		HD44780_PrintStringXY("Select News Station ", 0, 0);

		for(u08Loop = 1 ; u08Loop < 4 ; u08Loop++)
		{
			if(u08Position == u08Loop) HD44780_PrintStringXY(">", u08Loop, 0);
			else  HD44780_PrintStringXY(" ", u08Loop, 0);


			if(Directory_ReadEntrie(u08OldStation + u08Loop, &DirEntries) == 1)
			{
				memcpy(&pWebRadio->cStation, DirEntries.cName, 19);
				pWebRadio->cStation[19] = 0;
				u08Length = (19 - (INT_U8) strlen(pWebRadio->cStation));

  				HD44780_PrintStringXY(pWebRadio->cStation, u08Loop, 1);

				while(u08Length--)
				{
					HD44780_PrintString(" ");
				}
			}
			else
			{
				HD44780_PrintStringXY("                   ", u08Loop, 1);
			}
		}

		/* set timeout of start value */
		pWebRadio->u16Timeout = 800;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}

/*************************************************************************
*   N E W S   S T A T I O N   S E T   T I M E
*************************************************************************/
void Menue_NewsStation_SetTime(WEBRADIO *pWebRadio)
{
	#define NEWS_STATION_OFF 0xff
	int iResult;
	static INT_U8 u08Position = 1;
	char cText[32];

	char cFullPath[256];

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;

		/* Clear Screen and switch backlight on */
		HD44780_Clear();

		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		switch(u08Position)
		{
			case 1:
			{
				if((pWebRadio->u08NewsStatioStartTime < 59) || (pWebRadio->u08NewsStatioStartTime == NEWS_STATION_OFF))
				{
					pWebRadio->u08NewsStatioStartTime += 1;
				}
			}
			break;

			case 2:
			{
				if(pWebRadio->u08NewsStatioIntervalTime < 30)
				{
					pWebRadio->u08NewsStatioIntervalTime += 5;
				}
			}
			break;

			case 3:
			{
				if(pWebRadio->u08NewsStatioDurationTime < pWebRadio->u08NewsStatioIntervalTime)
				{
					pWebRadio->u08NewsStatioDurationTime += 1;
				}
			}
			break;

			default:
			break;
		}
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		switch(u08Position)
		{
			case 1:
			{
				if((pWebRadio->u08NewsStatioStartTime >= 0) && (!(pWebRadio->u08NewsStatioStartTime == NEWS_STATION_OFF)))
				{
					pWebRadio->u08NewsStatioStartTime -= 1;
				}
			}
			break;

			case 2:
			{
				if(pWebRadio->u08NewsStatioIntervalTime > 10)
				{
					pWebRadio->u08NewsStatioIntervalTime -= 5;
				}
			}
			break;

			case 3:
			{
				if(pWebRadio->u08NewsStatioDurationTime > 1)
				{
					pWebRadio->u08NewsStatioDurationTime -= 1;
				}
			}
			break;

			default:
			break;
		}
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		u08Position++;

		if(u08Position == 4)
		{
			u08Position = 1;

			/* add settings to path */
			sprintf(cFullPath, "%s%s", "/WebRadio", SETTINGS_PATH);
			/* save the Webradio settings */
			iResult = Settings_Write(cFullPath, pWebRadio);
			if(iResult < 0)
			{
				DB_PRINTF(("Settings write error\r\n"));
			}

			/* Enter_play mode */
			pWebRadio->u08MenueState = MENUE_SETUP;
			pWebRadio->u08MenueInit = 1;
		}
	}

   if(pWebRadio->u16Timeout == 0)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_PLAY;
		pWebRadio->u08MenueInit = 1;
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	/* was there some actions */
	if(Message_Read())
	{
		INT_U8 u08Loop;
		HD44780_PrintStringXY("Set Time   ", 0, 0);

		for(u08Loop = 1 ; u08Loop < 4 ; u08Loop++)
		{
			if(u08Position == u08Loop) HD44780_PrintStringXY(">", u08Loop, 0);
			else  HD44780_PrintStringXY(" ", u08Loop, 0);
		}

		if(pWebRadio->u08NewsStatioStartTime == NEWS_STATION_OFF)
		{
			HD44780_PrintStringXY("News off           ", 1, 1);
		}
		else
		{
			sprintf(cText,"Start    hh:%02d",pWebRadio->u08NewsStatioStartTime);
			HD44780_PrintStringXY(cText, 1, 1);
		}

		sprintf(cText,"Interval %02d min",pWebRadio->u08NewsStatioIntervalTime);
		HD44780_PrintStringXY(cText, 2, 1);

		sprintf(cText,"Duration %02d min",pWebRadio->u08NewsStatioDurationTime);
		HD44780_PrintStringXY(cText, 3, 1);

		/* set timeout of start value */
		pWebRadio->u16Timeout = 800;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}

void Menue_Select(WEBRADIO *pWebRadio)
{
	/* read last mesage and enter menue */
	Message_Get();

	switch(pWebRadio->u08MenueState)
	{
		case MENUE_ON:
			Menue_On(pWebRadio);
		break;

		case MENUE_PLAY:
			Menue_Play(pWebRadio);
		break;

		case MENUE_CHANGE_STATION:
			Menue_ChangeStation(pWebRadio);
		break;

		case MENUE_SETUP:
			Menue_Setup(pWebRadio);
		break;

		case MENUE_STATION:
			Menue_Station(pWebRadio);
		break;

		case MENUE_NEWSSTATION_SET_STATION:
			 Menue_NewsStation_SetStation(pWebRadio);
		break;

		case MENUE_NEWSSTATION_SET_TIME:
			 Menue_NewsStation_SetTime(pWebRadio);
		break;

      case MENUE_OFF:
			Menue_Off(pWebRadio);
		break;

		default:
		break;
	}


   /* now clear all */
	Message_Clear();

}

#ifdef __cplusplus
        }
#endif

