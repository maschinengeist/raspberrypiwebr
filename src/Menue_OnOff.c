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

#include "Station.h"
#include "gpio.h"
#include "message.h"
#include "Network.h"
#include "Menue.h"
#include "Menue_OnOff.h"
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
*   S T A R T U P   M E N U E
*************************************************************************/

void Menue_Start(void)
{
	int iCountdown = 30 * 4;
	char cText[32];
	INT_U8 u08Sequenz = 0;

	HD44780_Clear();
   HD44780_PrintStringXY("   start webradio", 1, 0);
	HD44780_Backlight(1);

   u08Sequenz = 0;

	while(iCountdown--)
	{
	  switch (u08Sequenz++)
	  {
			case 0:
				 HD44780_PrintStringXY("     *         ", 2, 0);
			break;

			case 1:
				 HD44780_PrintStringXY("      *       ", 2, 0);
			break;

			case 2:
				 HD44780_PrintStringXY("       *       ", 2, 0);
			break;

			case 3:
				 HD44780_PrintStringXY("        *      ", 2, 0);
			break;

			case 4:
				 HD44780_PrintStringXY("         *     ", 2, 0);
			break;

			case 5:
				 HD44780_PrintStringXY("          *   ", 2, 0);
			break;

			case 6:
				 HD44780_PrintStringXY("           *   ", 2, 0);
			break;

			case 7:
				 HD44780_PrintStringXY("            *  ", 2, 0);
			break;

			case 8:
				 HD44780_PrintStringXY("             * ", 2, 0);
			break;

			case 9:
				 HD44780_PrintStringXY("              *", 2, 0);
				 u08Sequenz = 0;
			break;

			default:
			break;
	  }

	  sprintf(cText, "in %3d seconds", ((iCountdown--) / 4));
	  HD44780_PrintStringXY(cText, 3, 3);

	  usleep(250000);
	}
}

/*************************************************************************
*   M E N U E   O N
*************************************************************************/

void Menue_On(WEBRADIO *pWebRadio)
{
	int iResult, iStation;
	char cFullPath[256];

	pWebRadio->u08Volume = 10;
	pWebRadio->u08MenueStationPosition = 1;
	pWebRadio->u08InitStation = 1;

	/* init message */
	Message_Init(0, 1, 2);

#ifdef SIMULANT
	/* get full path */
	if(0 == realpath(".", pWebRadio->cFullPath))
	{
		DB_PRINTF(("Can't find the current path\r\n"));
		exit(0);
	}
	/* add settings to path */
	sprintf(cFullPath, "%s%s",pWebRadio->cFullPath, SETTINGS_PATH);
#else
	sprintf(pWebRadio->cFullPath, "/WebRadio");
	sprintf(cFullPath, "/WebRadio%s", SETTINGS_PATH);
#endif

	/* read setting of the webradio */
	iResult = Settings_Read(cFullPath, pWebRadio);
	if(iResult < 0)
	{
		DB_PRINTF(("Can't find Settings\r\n"));
		exit(0);
	}

	/* add station to path */
	sprintf(cFullPath, "%s%s",pWebRadio->cFullPath, STATION_PATH);

	iStation = Directory_Open(cFullPath);
	if(iStation > 0)
	{
		/* set max station */
		pWebRadio->u08MaxChannel = (INT_U8) iStation;

		/* start mplayer in slave mode */
		Mplayer_Start();

		/* set volume */
		iResult = Mplayer_Volume(pWebRadio->u08Volume * 3);
		if(iResult < 0)
		{
			DB_PRINTF(("Can't set volume\r\n"));
			exit(0);
		}

		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_CHANGE_STATION;
		pWebRadio->u08MenueInit = 1;
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
void Menue_Off(WEBRADIO *pWebRadio)
{
	/* init new menue screen */
	int iResult, iTimeout = 10;
	INT_U8 u08Sequenz = 0;

	char cFullPath[256];

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16BacklightTimeout = 2000;
		pWebRadio->u16Timeout = 200;

		/* backlight on */
		HD44780_Clear();
		HD44780_Backlight(1);
		HD44780_PrintStringXY("   exit webradio", 1, 0);

		 while(iTimeout--)
		 {
			  switch (u08Sequenz++)
			  {
					case 0:
						 HD44780_PrintStringXY("    *         ", 2, 0);
					break;

					case 1:
						 HD44780_PrintStringXY("     *       ", 2, 0);
					break;

					case 2:
						 HD44780_PrintStringXY("      *       ", 2, 0);
					break;

					case 3:
						 HD44780_PrintStringXY("       *      ", 2, 0);
					break;

					case 4:
						 HD44780_PrintStringXY("        *     ", 2, 0);
					break;

					case 5:
						 HD44780_PrintStringXY("         *   ", 2, 0);
					break;

					case 6:
						 HD44780_PrintStringXY("          *   ", 2, 0);
					break;

					case 7:
						 HD44780_PrintStringXY("           *  ", 2, 0);
					break;

					case 8:
						 HD44780_PrintStringXY("            * ", 2, 0);
					break;

					case 9:
						 HD44780_PrintStringXY("             *", 2, 0);
						 u08Sequenz = 0;
					break;

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
		sprintf(cFullPath, "%s%s",pWebRadio->cFullPath, SETTINGS_PATH);
		/* save the Webradio settings */
		iResult = Settings_Write(cFullPath, pWebRadio);
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

      pWebRadio->u08MenueState = MENUE_ON;
		pWebRadio->u08MenueInit = 1;
	}
}


#ifdef __cplusplus
        }
#endif

