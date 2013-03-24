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

#include "message.h"
#include "Menue.h"
#include "Menue_OnOff.h"
#include "Menue_Play.h"
#include "Mplayer.h"
#include "Directory.h"
#include "Webradio.h"

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

/* Menue Timer */
static INT_U16  u16TimeoutTime;
/* Webradio setting */
static INT_U8   u08MenueState;
/* Signal a new menue is select */
static INT_U8   u08MenueInit;

/************************************************************************
*/
/*! \fn int Menue_CheckTimeout(void);
*
*   \brief decrement timeout and check if an timeout occurre
*
*************************************************************************/
int Menue_CheckTimeout(void)
{
	if(u16TimeoutTime)
	{
		u16TimeoutTime--;
		return 0;
	}

	return 1;
}

/************************************************************************
*/
/*! \fn void Menue_SetTimeout(INT_U16 u16Timeout);
*
*   \brief set timeout
*
*************************************************************************/
void Menue_SetTimeout(INT_U16 u16Timeout)
{
   u16TimeoutTime = u16Timeout;
}

/************************************************************************
*/
/*! \fn void Menue_ChangeMenue(INT_U8 u08Menue);
*
*   \brief set new menue state
*
*************************************************************************/
void Menue_ChangeMenue(INT_U8 u08Menue)
{
	/* new menue and signal init */
	u08MenueState = u08Menue;
	u08MenueInit = 1;
}


/************************************************************************
*/
/*! \fn INT_U8 Menue_GetMenue(void)
*
*   \brief get current menue state
*
*************************************************************************/
INT_U8 Menue_GetMenue(void)
{
	return u08MenueState;
}

/************************************************************************
*/
/*! \fn int Menue_Init(INT_U16 u16Timeout);
*
*   \brief set new menue
*
*   \return 1 = initialization done, 0 = no initialization
*
*************************************************************************/
int Menue_Init(INT_U16 u16Timeout)
{
	/* check if a new menue is select */
	if(u08MenueInit == 1)
	{
		u08MenueInit = 0;
		u16TimeoutTime = u16Timeout;

		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		return 1;
	}

	return 0;
}

/*************************************************************************
*   M E N U E   S E T U P
*************************************************************************/
void Menue_Setup(void)
{
	static INT_U8 u08Position = 0;

	/* init new menue screen */
	if(Menue_Init(800))
	{
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);
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
                Menue_ChangeMenue(MENUE_OFF);
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
                Menue_ChangeMenue(MENUE_NEWSSTATION_SET_TIME);
            break;

            case 3:
                /* Ethernet Settings */
                Menue_ChangeMenue(MENUE_PLAY);
            break;

            default:
            break;
	    }
	}

	if(Menue_CheckTimeout())
	{
		/* Enter_play mode */
      Menue_ChangeMenue(MENUE_PLAY);
	}

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

		/* set timeout */
		Menue_SetTimeout(800);
	}
}

/*************************************************************************
*   N E W S   S T A T I O N   S E T   S T A T I O N
*************************************************************************/

void Menue_NewsStation_SetStation(void)
{
	static INT_U8 u08Position = 1, u08OldStation = 1;
	char cText[128];

	/* init new menue screen */
	if(Menue_Init(800))
	{
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		if((u08Position < 3) && (u08Position < Webradio_Get(MAX_STATION)))
		{
			u08Position++;
		}
		else
		{
			if((u08OldStation + u08Position) < Webradio_Get(MAX_STATION)) u08OldStation++;
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
		int iResult;

		/* Enter_play mode */
		Menue_ChangeMenue(MENUE_PLAY);

		Webradio_Set(NEWS_STATION, (u08OldStation + u08Position));

		/* add settings to path */
		sprintf(cText, "%s%s", "/WebRadio", SETTINGS_PATH);

		/* Clear Screen */
		HD44780_Clear();

		/* save the Webradio settings */
		iResult = Webradio_WriteSettings(cText);
		if(iResult < 0)
		{
			DB_PRINTF(("Settings write error\r\n"));
			HD44780_PrintStringXY("    save error    ", 1, 0);
		}
		else
		{
			HD44780_PrintStringXY("    save station    ", 1, 0);
		}

		sleep(2);

		/* Enter_setup mode */
		Menue_ChangeMenue(MENUE_PLAY);
	}

   if(Menue_CheckTimeout())
	{
		/* Enter_play mode */
		Menue_ChangeMenue(MENUE_PLAY);
	}

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
				memcpy(cText, DirEntries.cName, 19);
				cText[19] = 0;
				u08Length = (19 - (INT_U8) strlen(cText));

  				HD44780_PrintStringXY(cText, u08Loop, 1);

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

		/* set timeout */
		Menue_SetTimeout(800);
	}
}

/*************************************************************************
*   N E W S   S T A T I O N   S E T   T I M E
*************************************************************************/
void Menue_NewsStation_SetTime(void)
{
	int iResult;
	static INT_U8 u08Position = 1;
	char cText[256];

	/* init new menue screen */
	if(Menue_Init(800))
	{
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		switch(u08Position)
		{
			case 1:
			{
				Webradio_Plus(START_TIME);
			}
			break;

			case 2:
			{
				Webradio_Plus(INTERVAL_TIME);
			}
			break;

			case 3:
			{
				Webradio_Plus(DURATION_TIME);
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
				Webradio_Minus(START_TIME);
			}
			break;

			case 2:
			{
				Webradio_Minus(INTERVAL_TIME);
			}
			break;

			case 3:
			{
				Webradio_Minus(DURATION_TIME);
			}
			break;

			default:
			break;
		}
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		u08Position++;

		if((u08Position == 2) && (Webradio_Get(START_TIME) == NEWS_STATION_OFF))
		{
			u08Position = 1;

			/* add settings to path */
			sprintf(cText, "%s%s", "/WebRadio", SETTINGS_PATH);
			/* save the Webradio settings */
			iResult = Webradio_WriteSettings(cText);
			if(iResult < 0)
			{
				DB_PRINTF(("Settings write error\r\n"));
			}

			/* Clear Screen */
			HD44780_Clear();
			HD44780_PrintStringXY("    switch news    ", 1, 0);
			HD44780_PrintStringXY("    station off    ", 2, 0);

			sleep(2);

			/* Enter_setup mode */
			Menue_ChangeMenue(MENUE_PLAY);
		}

		if(u08Position == 4)
		{
			u08Position = 1;

			/* Enter_setup mode */
			Menue_ChangeMenue(MENUE_NEWSSTATION_SET_STATION);
		}
	}

   if(Menue_CheckTimeout())
	{
		/* Enter_play mode */
		Menue_ChangeMenue(MENUE_PLAY);
	}

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

		if(Webradio_Get(START_TIME) == NEWS_STATION_OFF)
		{
			HD44780_PrintStringXY("News off           ", 1, 1);
		}
		else
		{
			sprintf(cText,"Start    hh:%02d", Webradio_Get(START_TIME));
			HD44780_PrintStringXY(cText, 1, 1);
		}

		sprintf(cText,"Interval %02d min", Webradio_Get(INTERVAL_TIME));
		HD44780_PrintStringXY(cText, 2, 1);

		sprintf(cText,"Duration %02d min", Webradio_Get(DURATION_TIME));
		HD44780_PrintStringXY(cText, 3, 1);

		/* set timeout */
		Menue_SetTimeout(800);
	}
}

void Menue_Select(void)
{
	/* read last mesage and enter menue */
	Message_Get();

	switch(Menue_GetMenue())
	{
		case MENUE_ON:
			Menue_On();
		break;

		case MENUE_PLAY:
			Menue_Play();
		break;

		case MENUE_CHANGE_STATION:
			Menue_ChangeStation();
		break;

		case MENUE_SETUP:
			Menue_Setup();
		break;

		case MENUE_STATION:
			Menue_Station();
		break;

		case MENUE_NEWSSTATION_SET_STATION:
			 Menue_NewsStation_SetStation();
		break;

		case MENUE_NEWSSTATION_SET_TIME:
			 Menue_NewsStation_SetTime();
		break;

      case MENUE_OFF:
			Menue_Off();
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

