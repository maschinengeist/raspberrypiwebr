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

#include "message.h"
#include "Menue.h"
#include "Menue_Play.h"
#include "Mplayer.h"
#include "Directory.h"
#include "Webradio.h"

#include "HD44780.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/

/* the offset for your country */
#define COUNTRY_TIME_OFFSET 1

/*************************************************************************
*   G L O B A L
*************************************************************************/

/*************************************************************************
*   M E N U E   P L A Y
*************************************************************************/

void Menue_Play(void)
{
	int iResult, iLength;
	#define MAX_SONG_LENGTH 21
	#define MAX_DISPLAY_STRING_LENGTH 16

	time_t rawtime;
	struct tm * timeinfo;

	char cCurrentSong[MAX_SONG_LENGTH + 1], cText[256];

	static INT_U8 u08SongPosition, u08DisplayCounter = 25;
	static INT_U8 u08Start = 1, u08StopTime = 0;


	INT_U8 u08Position = 0, u08Loop, u08Minuten;

	/* get time */
	time (&rawtime);
	timeinfo = gmtime(&rawtime);

	/* init new menue screen */
	if(Menue_Init(0))
	{
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);

		strcpy(cText, Webradio_GetStation(0));

		iLength = strlen(cText);
		/* get legth of station text */
		if(iLength > MAX_DISPLAY_STRING_LENGTH)
		{
			char* pStringPosition[8];
			int iLoop, iLoopNext;
			char cDisplay[2][32];

			/* set pointer to zero */
			memset(pStringPosition, 0x00, sizeof(pStringPosition));

			pStringPosition[0] = &cText[0];

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
					if((strlen(&cDisplay[0][0]) + strlen(pStringPosition[iLoop])) < MAX_DISPLAY_STRING_LENGTH )
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
					if((strlen(&cDisplay[1][0]) + strlen(pStringPosition[iLoop])) < MAX_DISPLAY_STRING_LENGTH)
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
			HD44780_PrintStringXY(Webradio_GetStation(0), 0, 0);
		}

		sprintf(cText, " %02d:%02d", (timeinfo->tm_hour+1)%24, timeinfo->tm_min);
		HD44780_PrintStringXY(cText, 0, 14);
	}

	/* get stream info and clear info when new info receive */
	iResult = Mplayer_GetStreamInfo(Webradio_GetStreamInfo(0), STREAM_INFO_LENGTH);
	if(iResult > 0) HD44780_PrintStringXY("                    ", 3, 0);

 	if(Menue_CheckTimeout())
 	{
 		/* get stream info and clear info when new info receive */
 		iResult = Mplayer_GetStreamInfo(Webradio_GetStreamInfo(0), STREAM_INFO_LENGTH);
 		if(iResult > 0) HD44780_PrintStringXY("                    ", 3, 0);

		if(u08DisplayCounter)
		{
			u08DisplayCounter--;
		}
		else
		{
			u08DisplayCounter = 25;

			/* moving info */
			if(strlen(Webradio_GetStreamInfo(0)) < MAX_SONG_LENGTH)
			{
				strcpy(cCurrentSong, Webradio_GetStreamInfo(0));
			}
			else
			{
				INT_U8 u08Null = 1;
				u08Position = u08SongPosition;

				for(u08Loop = 0 ; u08Loop < MAX_SONG_LENGTH ; u08Loop++)
				{
					 CHAR cChar;
					 cChar = *Webradio_GetStreamInfo(u08Loop + u08Position);

					 if(cChar && u08Null)
					 {
						  cCurrentSong[u08Loop] = *Webradio_GetStreamInfo(u08Loop + u08SongPosition);
					 }
					 else
					 {
						  if(u08Null)
						  {
								u08Null = 0;
								u08Position = 0;
								cCurrentSong[u08Loop++] = 32;
						  }

						  cCurrentSong[u08Loop] = *Webradio_GetStreamInfo(u08Position++);
					 }
				}
			}

			u08SongPosition++;

			if(u08SongPosition >= strlen(Webradio_GetStreamInfo(0))) u08SongPosition = 0;

			cCurrentSong[20] = 0;
			HD44780_PrintStringXY(cCurrentSong, 3, 0);

			cCurrentSong[MAX_SONG_LENGTH - 1] = 0;

			/* winter or summertime */
			if(timeinfo->tm_isdst > 0)
			{
				/* wintertime */
				sprintf(cText, " %02d:%02d", (timeinfo->tm_hour + COUNTRY_TIME_OFFSET)%24, timeinfo->tm_min);
			}
			else
			{
				/* summertime plus one hour */
				sprintf(cText, " %02d:%02d", (timeinfo->tm_hour + COUNTRY_TIME_OFFSET + 1)%24, timeinfo->tm_min);
			}

			HD44780_PrintStringXY(cText, 0, 14);
		}
 	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		INT_U8 u08Volume = Webradio_Plus(VOLUME);
		/* display volume */
		sprintf(cText, "          Volume:%3d", u08Volume);
		HD44780_PrintStringXY(cText, 3, 0);

		Mplayer_Volume(u08Volume * 3);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		INT_U8 u08Volume = Webradio_Minus(VOLUME);
		/* display volume */
		sprintf(cText, "          Volume:%3d", u08Volume);
		HD44780_PrintStringXY(cText, 3, 0);

		Mplayer_Volume(u08Volume * 3);
	}

	/* Enter_play mode */
	if(Message_Read() == MESSAGE_KEY_MIDDLE) Menue_ChangeMenue(MENUE_STATION);

	/* Enter_play mode */
	if(Message_Read() == MESSAGE_KEY_SETUP) Menue_ChangeMenue(MENUE_SETUP);

	/* was there some actions the set timeout of start value */
	if(Message_Read()) Menue_SetTimeout(200);

	/* check news station time */
	u08Minuten = (INT_U8)timeinfo->tm_min;

	if(!(Webradio_Get(START_TIME) == 0xff))
	{
		if(u08Minuten >= Webradio_Get(START_TIME))
		{
			u08Minuten -= Webradio_Get(START_TIME);

			if((u08Minuten % Webradio_Get(INTERVAL_TIME)) == 0)
			{
				if(u08Start == 1)
				{
					u08Start = 0;
					u08StopTime = u08Minuten + Webradio_Get(DURATION_TIME);

					Webradio_Set(NEWS_SAVE_STATION, Webradio_Get(STATION));

					if(!(Webradio_Get(STATION) == Webradio_Get(NEWS_STATION)))
					{
						/* set news station */
						Webradio_Set(STATION, Webradio_Get(NEWS_STATION));
						/* switch to new station */
						Menue_ChangeMenue(MENUE_CHANGE_STATION);
					}
				}
			}

			if(u08Start == 0)
			{
				if(u08Minuten == u08StopTime)
				{
					u08Start = 1;
					Webradio_Set(STATION, Webradio_Get(NEWS_SAVE_STATION));

					if(!(Webradio_Get(STATION) == Webradio_Get(NEWS_STATION)))
					{
						/* switch to new station */
						Menue_ChangeMenue(MENUE_CHANGE_STATION);
					}
				}
			}
		}
	}
}

/*************************************************************************
*   M E N U E   C H A N G E   S T A T I O N
*************************************************************************/

void Menue_ChangeStation(void)
{
	int iResult;
	static INT_U8 u08Sequenz = 0, u08ChangeSequenz = 0, u08TimeCount = 25, u08Trys = 0;

	DIRECTORY_CONTENTS DirEntries;

	char cText[128];

	switch(u08ChangeSequenz)
	{
		case 0:
		{
			Menue_Init(1200);

			HD44780_PrintStringXY("  connect station", 1, 0);
			u08Sequenz = 0;

			/* trys to zero */
			u08Trys = 0;

			/* next state, prepare connection */
			u08ChangeSequenz = 1;
		}
		break;

		case 1:
		{
			if(Directory_ReadEntrie((int)Webradio_Get(STATION), &DirEntries) == 1)
			{
				/* load current stations name */
				if(DirEntries.iType == DIR_TYPE_MCU)
				{
					int iLoop = 0;
					char cEscapeStation[256];
					char* pText = cText;

					/* copy station name */
					strcpy(cText, DirEntries.cName);

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

					/* play station with mplayer */
					sprintf(cText,"loadlist %s/Station/%s.m3u\n",Webradio_GetPath(), cEscapeStation);
					Mplayer_PlayFile(cText);
				}
			}

			/* next state, wait for connection */
			u08ChangeSequenz = 2;
		}
		break;

		case 2:
		{
			if(u08TimeCount)
			{
				u08TimeCount--;
			}
			else
			{
				u08TimeCount = 25;

				/* get stream info and clear info when new info receive */
				iResult = Mplayer_GetStreamInfo(cText, sizeof(cText));

				if(iResult == 1)
				{
					/* copy new stream info */
					strcpy(Webradio_GetStreamInfo(0), cText);

					/* read current station name */
					if(Directory_ReadEntrie(Webradio_Get(STATION), &DirEntries) == 1)
					{
						strcpy(Webradio_GetStation(0), DirEntries.cName);
					}

					Mplayer_Volume(Webradio_Get(VOLUME) * 3);

					/* reset state */
					u08ChangeSequenz = 0;

					/* and go into play menue */
					Menue_ChangeMenue(MENUE_PLAY);
				}
				else
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
				}
			}

			/* check menue timeout */
			if(Menue_CheckTimeout())
			{
				u08Trys++;

				if(u08Trys == 3)
				{
					/* no connection possible, leave connection */
					u08ChangeSequenz = 4;
				}
				else
				{
					/* timeout occure, try it again */
					u08ChangeSequenz = 3;
					/* set timeout again */
					Menue_SetTimeout(1200);
				}
			}
		}
		break;

		case 3:
		{
			/* no change get old channel */
			Webradio_LoadStation();

			/* Clear Screen */
			HD44780_Clear();
			HD44780_PrintStringXY("      station       ", 1, 0);
			HD44780_PrintStringXY("   not available    ", 2, 0);
			sleep(2);
			HD44780_PrintStringXY("      connect       ", 1, 0);
			HD44780_PrintStringXY("  to last station   ", 2, 0);
			sleep(2);

			HD44780_Clear();
			HD44780_PrintStringXY("  connect station", 1, 0);

			/* next state, connect again */
			u08ChangeSequenz = 1;
		}
		break;

		case 4:
		{
			/* Clear Screen */
			HD44780_Clear();
			HD44780_PrintStringXY("      station       ", 1, 0);
			HD44780_PrintStringXY("   not available    ", 2, 0);
			sleep(2);

			/* clear info */
			sprintf(cText,"                    ");

			/* copy new stream info */
			strcpy(Webradio_GetStreamInfo(0), cText);

			/* and go into play menue */
			Menue_ChangeMenue(MENUE_PLAY);

			/* next state, connect again */
			u08ChangeSequenz = 0;
		}
		break;

		default:
		break;
	}

	if(Message_Read() == MESSAGE_KEY_RIGHT)
	{
		Mplayer_Volume(Webradio_Plus(VOLUME) * 3);
	}

	if(Message_Read() == MESSAGE_KEY_LEFT)
	{
		Mplayer_Volume(Webradio_Minus(VOLUME) * 3);
	}

	if(Message_Read() == MESSAGE_KEY_MIDDLE)
	{
		/* select new station */
		Menue_ChangeMenue(MENUE_STATION);

		/* reset state */
		u08ChangeSequenz = 0;
	}
}

/*************************************************************************
*   M E N U E   S T A T I O N
*************************************************************************/
void Menue_Station(void)
{
	static INT_U8 u08Position = 0, u08OldStation = 1;
	char cText[128];

	/* init new menue screen */
	if(Menue_Init(800))
	{
		/* send message to refresh the display */
		Message_Set(MESSAGE_REFRESH_SCREEN);

		/* save old channel */
		Webradio_SaveStation();
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
		/* get current channel */
		if(!(Webradio_Get(STATION) == u08OldStation + u08Position))
		{
			Webradio_Set(STATION, (u08OldStation + u08Position));

			Webradio_Set(NEWS_SAVE_STATION, (u08OldStation + u08Position));
			/* switch to new station */
			Menue_ChangeMenue(MENUE_CHANGE_STATION);
		}
		else
		{
			/* don't switch got to play */
			Menue_ChangeMenue(MENUE_PLAY);
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
		INT_U8 u08Loop, u08Length;
		DIRECTORY_CONTENTS DirEntries;

		for(u08Loop = 0 ; u08Loop < 4 ; u08Loop++)
		{
			if(u08Position == u08Loop) HD44780_PrintStringXY(">", u08Loop, 0);
			else  HD44780_PrintStringXY(" ", u08Loop, 0);

			if(Directory_ReadEntrie(u08OldStation + u08Loop, &DirEntries) == 1)
			{
				memcpy(cText, DirEntries.cName, 19);
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

		/* set timeout of start value */
		Menue_SetTimeout(800);
	}
}

#ifdef __cplusplus
        }
#endif

