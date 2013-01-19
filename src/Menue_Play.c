/*! \file Menue_Play.c
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
*   M E N U E   P L A Y
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

		/* set timeout of start value */
		pWebRadio->u08MenueInit = 0;
		pWebRadio->u16BacklightTimeout = 2000;
		pWebRadio->u16Timeout = 0;

		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);

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

	/* get stream info and clear info when new info receive */
	iResult = Mplayer_GetStreamInfo(pWebRadio->cStreamInfo, sizeof(pWebRadio->cStreamInfo));
	if(iResult > 0) HD44780_PrintStringXY("                    ", 3, 0);

	if(u08DisplayCounter)
	{
		u08DisplayCounter--;
	}
	else
	{
		time_t rawtime;
		struct tm * timeinfo;
		char cTime[32];

		u08DisplayCounter = 25;

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

		/* get time */
		time (&rawtime);
		timeinfo = localtime(&rawtime);

		sprintf(cTime, " %02d:%02d", (timeinfo->tm_hour+1)%24, timeinfo->tm_min);
		HD44780_PrintStringXY(cTime, 0, 14);
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

	/* was there some actions */
	if(Message_Read())
	{
		/* set timeout of start value */
		pWebRadio->u16Timeout = 200;
		pWebRadio->u16BacklightTimeout = 2000;
	}
}

/*************************************************************************
*   M E N U E   C H A N G E   S T A T I O N
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
		pWebRadio->u16Timeout = 1000;

		if(Directory_ReadEntrie((int)pWebRadio->u08Station, &DirEntries) == 1)
		{
			DB_PRINTF(("Directory_ReadEntrie\n"));
			/* load current stations name */
			if(DirEntries.iType == DIR_TYPE_MCU)
			{
				int iLoop = 0;
				char cEscapeStation[256];
				char* pText = pWebRadio->cStation;

				DB_PRINTF(("DirEntries.iType\n"));

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
				DB_PRINTF(("cEscapeStation\n"));

				if(pWebRadio->u08InitStation)
				{
					pWebRadio->u08InitStation = 0;

					/* play station with mplayer */
					sprintf(cText,"loadlist %s/Station/%s.m3u\n",pWebRadio->cFullPath, cEscapeStation);
					Mplayer_PlayFile(cText);
					DB_PRINTF(("Mplayer_PlayFile\n"));
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

		DB_PRINTF(("Mplayer_GetStreamInfo befor\n"));

		/* get stream info and clear info when new info receive */
		iResult = Mplayer_GetStreamInfo(pWebRadio->cStreamInfo, sizeof(pWebRadio->cStreamInfo));

		DB_PRINTF(("Mplayer_GetStreamInfo\n"));

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

	/* decrement menue timeout */
	if(pWebRadio->u16Timeout)
	{
		pWebRadio->u16Timeout--;
	}
	else
	{
		/* Enter_play mode */
		pWebRadio->u08MenueState = MENUE_PLAY;
		pWebRadio->u08MenueInit = 1;
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

#ifdef __cplusplus
        }
#endif

