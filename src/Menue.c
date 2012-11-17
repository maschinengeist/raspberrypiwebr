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

#include "mytypes.h"

#include "Station.h"
#include "gpio.h"
#include "message.h"
#include "Network.h"
#include "Menue.h"
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
*   S T A R T   M E N U E
*************************************************************************/

void Menue_Start(WEBRADIO *pWebRadio)
{
	int iResult, iTimeout = 10, iStation;
	INT_U8 u08Sequenz;
	char cFullPath[256];

	pWebRadio->u08Volume = 10;
	pWebRadio->u08MenueStationPosition = 1;
	pWebRadio->u08InitStation = 1;

	/* init message */
	Message_Init(0, 1, 2);

	HD44780_Clear();
	HD44780_Backlight(1);
   HD44780_PrintStringXY("   start webradio", 1, 0);

#if 0
	if(-1 == Network_Up())
	{
		printf("Can't start network\r\n");
	}
#endif

#ifdef SIMULANT
	/* get full path */
	if(0 == realpath(".", pWebRadio->cFullPath))
	{
		printf("Can't find the current path\r\n");
		exit(0);
	}
	/* add settings to path */
	sprintf(cFullPath, "%s%s",pWebRadio->cFullPath, SETTINGS_PATH);
#else
	sprintf(pWebRadio->cFullPath, "/WebRadio");
#endif

	sprintf(cFullPath, "/WebRadio%s", SETTINGS_PATH);
	/* read setting of the webradio */
	iResult = Settings_Read(cFullPath, pWebRadio);
	if(iResult < 0)
	{
		printf("Can't find Settings\r\n");
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
			printf("Can't set volume\r\n");
			exit(0);
		}

		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_CHANGE_STATION;
		pWebRadio->u08MenueInit = 1;
	}
	else
	{
		printf("Can't find Station.txt\r\n");
		exit(0);
	}

    u08Sequenz = 0;

    while(iTimeout--)
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

        usleep(250000);
    }
}

/*************************************************************************
*   P L A Y   M E N U E
*************************************************************************/

void Menue_Play(WEBRADIO *pWebRadio)
{
	int iResult, iLength;
	#define MAX_SONG_LENGTH 21

	char cCurrentSong[MAX_SONG_LENGTH + 1], cText[256];

	static INT_U8 u08SongPosition, u08DisplayCounter = 25;
	INT_U8 u08Position = 0, u08Loop;

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		HD44780_Backlight(1);

		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16BacklightTimeout = 2000;
		pWebRadio->u16Timeout = 0;


		iLength = strlen(pWebRadio->cStation);
		if(iLength > 20)
		{
			char* pStringPosition[8];
			int iLoop, iLoopNext;
			char cDisplay[2][32];

			memset(pStringPosition, 0x00, sizeof(pStringPosition));

			pStringPosition[0] = &pWebRadio->cStation[0];

			for(iLoop = 0 ; iLoop < 7 ; iLoop++)
			{
				pStringPosition[1 + iLoop] = strstr(pStringPosition[iLoop], " ") + 1;

				*(pStringPosition[iLoop] -1) = 0;

				if(pStringPosition[1 + iLoop] == (char*)1)
				{
					break;
				}
			}

			memset(&cDisplay[0][0], 0x00, sizeof(cDisplay));

			iLoopNext = 8;
			for(iLoop = 0 ; iLoop < 8 ; iLoop++)
			{
				if(pStringPosition[iLoop])
				{
					if((strlen(&cDisplay[0][0]) + strlen(pStringPosition[iLoop])) < 20 )
					{
						if(pStringPosition[iLoop])
						{
							if(iLoop == 0)
							{
								sprintf(&cDisplay[0][0], "%s ", pStringPosition[iLoop]);
							}
							else
							{
								sprintf(&cDisplay[0][0], "%s%s ", &cDisplay[0][0], pStringPosition[iLoop]);
							}
						}
					}
					else
					{
						iLoopNext = iLoop;
						break;
					}
				}
			}

			for(iLoop = iLoopNext ; iLoop < 8 ; iLoop++)
			{
				if(pStringPosition[iLoop] > (char*)1)
				{
					if((strlen(&cDisplay[1][0]) + strlen(pStringPosition[iLoop])) < 20 )
					{
						if(pStringPosition[iLoop])
						{
							if(iLoop == iLoopNext)
							{
								sprintf(&cDisplay[1][0], "%s ", pStringPosition[iLoop]);
							}
							else
							{
								sprintf(&cDisplay[1][0], "%s%s ", &cDisplay[1][0], pStringPosition[iLoop]);
							}
						}
					}
					else
					{
						break;
					}
				}
			}

			HD44780_PrintStringXY(&cDisplay[0][0], 0, 0);
			HD44780_PrintStringXY(&cDisplay[1][0], 1, 0);

		}
		else
		{
			HD44780_PrintStringXY(pWebRadio->cStation, 0, 0);
		}
	}

	if(pWebRadio->u16Timeout == 0)
	{
		/* get stream info and clear info when new info receive */
		iResult = Mplayer_GetStreamInfo(pWebRadio->cStreamInfo, sizeof(pWebRadio->cStreamInfo));
		if(iResult > 0) HD44780_PrintStringXY("                    ", 3, 0);

		if(u08DisplayCounter)
		{
			u08DisplayCounter--;
		}
		else
		{
			u08DisplayCounter = 35;

			/* moving info */
			if(strlen(pWebRadio->cStreamInfo) < MAX_SONG_LENGTH)
			{
				strcpy(cCurrentSong, pWebRadio->cStreamInfo);
			}
			else
			{
				INT_U8 u08Null = 1;
				u08Position = u08SongPosition;

				for(u08Loop = 0 ; u08Loop < MAX_SONG_LENGTH ; u08Loop++)
				{
					 CHAR cChar;
					 cChar = pWebRadio->cStreamInfo[u08Loop + u08Position];

					 if(cChar && u08Null)
					 {
						  cCurrentSong[u08Loop] = pWebRadio->cStreamInfo[u08Loop + u08SongPosition];
					 }
					 else
					 {
						  if(u08Null)
						  {
								u08Null = 0;
								u08Position = 0;
								cCurrentSong[u08Loop++] = 32;
						  }

						  cCurrentSong[u08Loop] = pWebRadio->cStreamInfo[u08Position++];
					 }
				}
			}

			u08SongPosition++;

			if(u08SongPosition >= strlen(pWebRadio->cStreamInfo)) u08SongPosition = 0;

			cCurrentSong[20] = 0;
			HD44780_PrintStringXY(cCurrentSong, 3, 0);

			cCurrentSong[MAX_SONG_LENGTH - 1] = 0;
		}
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		if(pWebRadio->u08Volume < 30) pWebRadio->u08Volume++;
		/* display volume */
		sprintf(cText, "          Volume:%3d", pWebRadio->u08Volume);
		HD44780_PrintStringXY(cText, 3, 0);

		Mplayer_Volume(pWebRadio->u08Volume * 3);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		if(pWebRadio->u08Volume > 0) pWebRadio->u08Volume--;
		/* display volume */
		sprintf(cText, "          Volume:%3d", pWebRadio->u08Volume);
		HD44780_PrintStringXY(cText, 3, 0);

		Mplayer_Volume(pWebRadio->u08Volume * 3);
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_STATION;
		pWebRadio->u08MenueInit = 1;
	}

	if(Message_Read() == MESSAGE_KEY_SETUP)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_SETUP;
		pWebRadio->u08MenueInit = 1;
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)
	{
		pWebRadio->u16Timeout--;
	}

	/* was there some actions */
	if(Message_Read())
	{
		/* set timeout of start value */
		pWebRadio->u16Timeout = 200;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}


/*************************************************************************
*   C H A N G E   S T A T I O N   M E N U E
*************************************************************************/

void Menue_ChangeStation(WEBRADIO *pWebRadio)
{
	int iResult;

	static int iTimeCount = 25;

	static INT_U8 u08Sequenz = 0;

	DIRECTORY_CONTENTS DirEntries;

	char cText[128];

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		HD44780_Backlight(1);
		HD44780_PrintStringXY("  connect station", 1, 0);

		u08Sequenz = 0;

		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16Timeout = 50;

		if(Directory_ReadEntrie(pWebRadio->u08Station, &DirEntries) == 1)
		{
			/* load current stations name */
			if(DirEntries.iType == DIR_TYPE_MCU)
			{
				int iLoop = 0;
				char cEscapeStation[256];
				char* pText = pWebRadio->cStation;

				/* copy station name */
				strcpy(pWebRadio->cStation, DirEntries.cName);

				memset(cEscapeStation, 0x00 , sizeof(cEscapeStation));

				/* escape spaces in name */
				while(*pText)
				{
					/* escape spaces in string */
					if(*pText == ' ')
					{
						cEscapeStation[iLoop++] = 92;
						cEscapeStation[iLoop++] = 32;
					}
					else
					{
						cEscapeStation[iLoop++] = *pText;
					}

					pText++;
				}

				cEscapeStation[iLoop] = 0;

				if(pWebRadio->u08InitStation)
				{
					pWebRadio->u08InitStation = 0;

					/* play station with mplayer */
					sprintf(cText,"loadlist %s/Station/%s.m3u\n",pWebRadio->cFullPath, cEscapeStation);
					Mplayer_PlayFile(cText);
				}
			}
		}
	}

	if(iTimeCount)
	{
		iTimeCount--;
	}
	else
	{
		iTimeCount = 25;

		if(pWebRadio->u16Timeout > 1)
		{
			pWebRadio->u16Timeout--;
		}
		else
		{
			if(pWebRadio->u16Timeout)
			{
				pWebRadio->u16Timeout--;

				/* Clear Screen  and switch backlight on */
				HD44780_Clear();
				HD44780_PrintStringXY("      station       ", 1, 0);
				HD44780_PrintStringXY("   not available    ", 2, 0);
			}
		}

		/* get stream info and clear info when new info receive */
		iResult = Mplayer_GetStreamInfo(pWebRadio->cStreamInfo, sizeof(pWebRadio->cStreamInfo));
		if(iResult == 1)
		{
			pWebRadio->u08MenueState = MENUE_PLAY;
			pWebRadio->u08MenueInit = 1;
		}

		if(pWebRadio->u16Timeout)
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
		}
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_STATION;
		pWebRadio->u08MenueInit = 1;
	}

	if(Message_Read() == MESSAGE_KEY_SETUP)
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_SETUP;
		pWebRadio->u08MenueInit = 1;
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		if(pWebRadio->u08Volume < 29) pWebRadio->u08Volume++;

		Mplayer_Volume(pWebRadio->u08Volume * 3);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		if(pWebRadio->u08Volume > 0) pWebRadio->u08Volume--;

		Mplayer_Volume(pWebRadio->u08Volume * 3);
	}

	/* was there some actions */
	if(Message_Read())
	{
		/* set timeout of start value */
		pWebRadio->u16Timeout = 200;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}

/*************************************************************************
*   M E N U E   S T A T I O N
*************************************************************************/
void Menue_Station(WEBRADIO *pWebRadio)
{
	static INT_U8 u08Position = 0, u08OldStation = 1;

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* Clear Screen  and switch backlight on */
		HD44780_Clear();
		HD44780_Backlight(1);

		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);

		/* save old channel */
		pWebRadio->u08OldStation = pWebRadio->u08Station;

		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16Timeout = 800;
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
		if(u08Position > 0)
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
		pWebRadio->u08MenueState = MENUE_CHANGE_STATION;
      pWebRadio->u08MenueInit = 1;

		/* get current channel */
		if(!(pWebRadio->u08Station == u08OldStation + u08Position))
		{
          pWebRadio->u08InitStation = 1;
		}

		pWebRadio->u08Station = u08OldStation + u08Position;
	}

   if(pWebRadio->u16Timeout == 0)
	{
		/* no changel get old channel */
		pWebRadio->u08Station = pWebRadio->u08OldStation;
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_CHANGE_STATION;
		pWebRadio->u08MenueInit = 1;
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	/* was there some actions */
	if(Message_Read())
	{
		INT_U8 u08Loop, u08Length;
		DIRECTORY_CONTENTS DirEntries;

		for(u08Loop = 0 ; u08Loop < 4 ; u08Loop++)
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
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		if(u08Position > 0) u08Position--;
	}

	if(Message_Read() == MESSAGE_KEY_SETUP)
	{
	    switch(u08Position)
	    {
            case 0:
                /* Quit webradio */
                pWebRadio->u08MenueState = MENUE_QUIT;
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
		HD44780_PrintStringXY("Wlan Settings      ", 1, 1);
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
*   N E T W O R K   S E T U P   M E N U E
*************************************************************************/
void Menue_SetupNetwork(WEBRADIO *pWebRadio)
{
	char cText[32];

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16Timeout = 1000;

		/* backlight on */
		HD44780_Backlight(1);

		HD44780_Clear();
		HD44780_PrintStringXY("   Network Setup   ",0 ,0);

		Message_Set(MESSAGE_REFRESH_SCREEN);
	}

   /* enable or disable dhcp */
	if(Message_Read() == MESSAGE_KEY_RIGHT) pWebRadio->NetworkSettings.u08DhcpOn = DHCP_ENABLE;
	if(Message_Read() == MESSAGE_KEY_LEFT) pWebRadio->NetworkSettings.u08DhcpOn = DHCP_DISABLE;

	if(Message_Read() > MESSAGE_NOT_AVAILABLE)
	{
		if(pWebRadio->NetworkSettings.u08DhcpOn == DHCP_DISABLE)
		{
			sprintf(cText, ">IP:%03d.%03d.%03d.%03d",(int)pWebRadio->NetworkSettings.u08IpAddress[0]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[1]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[2]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[3]);
  			HD44780_PrintStringXY(cText, 2, 0);
			HD44780_PrintStringXY(" DHCP               ", 3, 0);
		}
		else
		{
			sprintf(cText, " IP:%03d.%03d.%03d.%03d",(int)pWebRadio->NetworkSettings.u08IpAddress[0]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[1]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[2]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[3]);
  			HD44780_PrintStringXY(cText, 2, 0);
			HD44780_PrintStringXY(">DHCP               ", 3, 0);
		}
	}

	if((Message_Read() == MESSAGE_KEY_MIDDLE) || (pWebRadio->u16Timeout == 0))
	{

		if(pWebRadio->NetworkSettings.u08DhcpOn == DHCP_DISABLE)
		{
		    /* Enter network mode */
            HD44780_PrintStringXY("   Set IP Address   ", 0, 0);
            HD44780_PrintStringXY("                    ", 1, 0);

            pWebRadio->u08MenueState = MENUE_NET_SETUP_IP;
		}
		else
		{
			/* save settings */

			/* Enter_play mode */
			pWebRadio->u08MenueState = MENUE_PLAY;
			pWebRadio->u08MenueInit = 1;
		}
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	/* was there some actions */
	if(Message_Read())
	{
		/* set timeout of start value */
		pWebRadio->u16Timeout = 1000;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}


/*************************************************************************
*   N E T W O R K   S E T U P   I P   M E N U E
*************************************************************************/
void Menue_NetworkSetupIP(WEBRADIO *pWebRadio)
{
	static INT_U8 u08NetworkIpPosition = 0;
	char cText[32];

	/* init new menue screen */
	if(pWebRadio->u08MenueInit)
	{
		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16BacklightTimeout = 2000;
		pWebRadio->u16Timeout = 1000;

		/* backlight on */
		HD44780_Backlight(1);
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
        /* enable dhcp */
		if (pWebRadio->NetworkSettings.u08IpAddress[u08NetworkIpPosition] < 255) pWebRadio->NetworkSettings.u08IpAddress[u08NetworkIpPosition]++;
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
        /* enable dhcp */
		if (pWebRadio->NetworkSettings.u08IpAddress[u08NetworkIpPosition] > 0) pWebRadio->NetworkSettings.u08IpAddress[u08NetworkIpPosition]--;
	}

	if(pWebRadio->u16Timer < 25)
	{
	    pWebRadio->u16Timer++;
	}
	else
	{
	    pWebRadio->u16Timer = 0;

	    if(pWebRadio->u08Toggle) pWebRadio->u08Toggle = 0;
        else pWebRadio->u08Toggle = 1;

	    if(pWebRadio->u08Toggle)
	    {
            INT_U8 u08Loop;
            HD44780_PrintStringXY(">IP:",2 ,0);

            for(u08Loop = 0 ; u08Loop < 4 ; u08Loop++)
            {
                if(u08Loop == u08NetworkIpPosition) HD44780_PrintString("   ");
                else
                {
                    sprintf(cText, "%03d",(int)pWebRadio->NetworkSettings.u08IpAddress[u08Loop]);
                    HD44780_PrintString(cText);
                }

                if(u08Loop < 3) HD44780_PrintString(".");
            }
	    }
	    else
	    {
            sprintf(cText, ">IP:%03d.%03d.%03d.%03d",(int)pWebRadio->NetworkSettings.u08IpAddress[0]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[1]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[2]
                                                    ,(int)pWebRadio->NetworkSettings.u08IpAddress[3]);

            HD44780_PrintStringXY(cText, 2, 0);
            HD44780_PrintStringXY(" DHCP               ", 3, 0);
	    }
	}

	if((Message_Read() == MESSAGE_KEY_RIGHT) || (Message_Read() == MESSAGE_KEY_LEFT))
	{
	    pWebRadio->u16Timer = 0;
	    pWebRadio->u08Toggle = 0;

        sprintf(cText, ">IP:%03d.%03d.%03d.%03d",(int)pWebRadio->NetworkSettings.u08IpAddress[0]
                                                ,(int)pWebRadio->NetworkSettings.u08IpAddress[1]
                                                ,(int)pWebRadio->NetworkSettings.u08IpAddress[2]
                                                ,(int)pWebRadio->NetworkSettings.u08IpAddress[3]);

        HD44780_PrintStringXY(cText, 2, 0);
        HD44780_PrintStringXY(" DHCP               ", 3, 0);
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
        if(u08NetworkIpPosition < 3)
        {
            u08NetworkIpPosition++;
        }
        else
        {
            u08NetworkIpPosition = 0;
        }
	}

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)	pWebRadio->u16Timeout--;

	if(pWebRadio->u16Timeout == 0)
	{
        /* ip position to start of ip */
		u08NetworkIpPosition = 0;

		/* save the settings */
		(void) Network_WriteSettings(NETWORK_PATH, &pWebRadio->NetworkSettings);

       /* Enter_play mode */
       pWebRadio->u08MenueState = MENUE_PLAY;
		 pWebRadio->u08MenueInit = 1;
	}

	/* was there some actions */
	if(Message_Read())
	{
		/* set timeout of start value */
		pWebRadio->u16Timeout = 1000;
		pWebRadio->u16BacklightTimeout = 2000;
		HD44780_Backlight(1);
	}
}

/*************************************************************************
*   Q U I T   M E N U E
*************************************************************************/
void Menue_Quit(WEBRADIO *pWebRadio)
{
	/* init new menue screen */
	int iResult, iTimeout = 10;
	INT_U8 u08Sequenz;

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

#if 0
		/* Network off */
		iResult = Network_Down();
		if(iResult < 0)
		{
			printf("Can't stop network\r\n");
		}
#endif

	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		printf("key start\r\n");
      pWebRadio->u08MenueState = MENUE_START;
		pWebRadio->u08MenueInit = 1;
	}
}


void Menue_Select(WEBRADIO *pWebRadio)
{
	/* read last mesage and enter menue */
	Message_Get();

	switch(pWebRadio->u08MenueState)
	{
		case MENUE_START:
			Menue_Start(pWebRadio);
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

		case MENUE_NET_SETUP:
			 Menue_SetupNetwork(pWebRadio);
		break;

		case MENUE_NET_SETUP_IP:
			 Menue_NetworkSetupIP(pWebRadio);
		break;

      case MENUE_QUIT:
			Menue_Quit(pWebRadio);
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

