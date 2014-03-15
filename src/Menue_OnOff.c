/*! \file Menue_OnOff.c
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
#include "Mplayer.h"
#include "Directory.h"
#include "Webradio.h"

#include "HD44780.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/


/*************************************************************************
*   S T A R T U P   M E N U E
*************************************************************************/

/*************************************************************************
*   M E N U E   O N
*************************************************************************/

void Menue_On(void)
{
	int iResult, iStation;
	char cFullPath[256];

	HD44780_Backlight(1);

	Webradio_Set(VOLUME, 10);

	/* init message */
	Message_Init();

#ifdef SIMULANT
	/* get full path */
	if(0 == realpath(".", Webradio_GetPath()))
	{
		DB_PRINTF(("Can't find the current path\r\n"));
		exit(0);
	}
	/* add settings to path */
	sprintf(cFullPath, "%s%s",Webradio_GetPath(), SETTINGS_PATH);
#else
	sprintf(Webradio_GetPath(), "/WebRadio");
	sprintf(cFullPath, "/WebRadio%s", SETTINGS_PATH);
#endif

	/* read setting of the webradio */
	iResult = Webradio_ReadSettings(cFullPath);
	if(iResult < 0)
	{
		DB_PRINTF(("Can't find Settings\r\n"));
		exit(0);
	}

	/* add station to path */
	sprintf(cFullPath, "%s%s",Webradio_GetPath(), STATION_PATH);

	iStation = Directory_Open(cFullPath);
	if(iStation > 0)
	{
		/* set max station */
		Webradio_Set(MAX_STATION, (INT_U8) iStation);

		/* start mplayer in slave mode */
		Mplayer_Start();

		/* set volume */
		iResult = Mplayer_Volume(Webradio_Get(VOLUME) * 3);
		if(iResult < 0)
		{
			DB_PRINTF(("Can't set volume\r\n"));
			exit(0);
		}

		/* Enter_play mode */
		Menue_ChangeMenue(MENUE_CHANGE_STATION);
	}
	else
	{
		DB_PRINTF(("Can't find Station.txt\r\n"));
		exit(0);
	}
}

/*************************************************************************
*   M E N U E   O F F
*************************************************************************/
void Menue_Off(void)
{
	/* init new menue screen */
	int iResult, iTimeout = 10;
	INT_U8 u08Sequenz = 0;

	char cFullPath[256];

	/* init new menue screen */
	if(Menue_Init(200))
	{
		HD44780_Backlight(1);
		HD44780_PrintStringXY("      goodbye      ", 1, 0);

		while(iTimeout--)
		{
			switch (u08Sequenz++)
			{
				case 0: HD44780_PrintStringXY("    *         ", 2, 0); break;
				case 1: HD44780_PrintStringXY("     *        ", 2, 0); break;
				case 2: HD44780_PrintStringXY("      *       ", 2, 0); break;
				case 3: HD44780_PrintStringXY("       *      ", 2, 0); break;
				case 4: HD44780_PrintStringXY("        *     ", 2, 0); break;
				case 5: HD44780_PrintStringXY("         *    ", 2, 0); break;
				case 6: HD44780_PrintStringXY("          *   ", 2, 0); break;
				case 7: HD44780_PrintStringXY("           *  ", 2, 0); break;
				case 8: HD44780_PrintStringXY("            * ", 2, 0); break;
				case 9: HD44780_PrintStringXY("             *", 2, 0); u08Sequenz = 0; break;
				default:
				break;
			}

			usleep(250000);
		}

		/* backlight off and clear screen */
		HD44780_Clear();
		HD44780_Backlight(0);
		HD44780_PrintStringXY("     i'am sleep     ", 1, 0);

		/* add settings to path */
		sprintf(cFullPath, "%s%s",Webradio_GetPath(), SETTINGS_PATH);
		/* save the Webradio settings */
		iResult = Webradio_WriteSettings(cFullPath);
		if(iResult < 0)
		{
			printf("Settings write error\r\n");
		}

		Mplayer_Close();

		sleep(1);
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		printf("key start\r\n");
		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		usleep(200000);
		HD44780_Backlight(1);

      Menue_ChangeMenue(MENUE_ON);
	}
}


#ifdef __cplusplus
        }
#endif

