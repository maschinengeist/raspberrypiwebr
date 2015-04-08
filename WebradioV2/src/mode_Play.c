/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "mytypes.h"

#include "debug.h"
#include "Mplayer.h"
#include "station.h"
#include "property.h"
#include "Dogm204.h"
#include "Dogm204_UserInterface.h"

#include "hwtime.h"
#include "command.h"
#include "mode.h"
#include "mode_Play.h"
#include "io.h"


/*************************************************************************
*   S T A T I C
*************************************************************************/

static INT_U8 u08NewsStart = 0, u08NewsStopTime = 0;

/*************************************************************************
*   P L A Y   M O D U S
*************************************************************************/
void mode_Play(void)
{
   char cText[128];
   INT_U8 u08Minuten = 0;

   /* no timeout */
   switch(mode_Init(0))
   {
   	/* we have no timeout */
      case MODE_TIMEOUT:
      break;

      case MODE_INIT:
      {
      	int iLenght, iLoop;

			Dogm204_Clear();
			command_DisplayUpdate();

			/* set volume */
			(void) Mplayer_Volume(property_Read(PROPERTY_VOLUME));

			/* read current station name */
			strcpy(cText, property_ReadString(PROPERTY_STATION));
			iLenght = strlen(cText);

			/* cut extension .m3u from name */
			if(iLenght >= 4)
			{
				iLenght -= 4;
				cText[iLenght] = 0;
			}

			if(iLenght <= 14)
			{
				/* Text fit in display */
				Dogm204_PrintStringXY(cText, 0, 0);
			}
			else
			{
				int iPosition = 0;

				/* split text if grater than the display size */
				for(iLoop = 0; iLoop < iLenght; iLoop++)
				{
					if(cText[iLoop] == 0x20)
					{
						if((iLoop >= iPosition) && (iLoop < 20)) iPosition = iLoop;
					}
				}

				if(iPosition) cText[iPosition] = 0;

				Dogm204_PrintStringXY(&cText[0], 0, 0);
				Dogm204_PrintStringXY(&cText[iPosition +1], 1, 0);
			}
      }

      default:
      {
			/* get stream info and clear info when new info receive */
			if(Mplayer_GetStreamInfo(cText, sizeof(cText)) == 1)
			{
				/* load stream info into display buffer */
				Dogm204_DisplayWriteText(2, cText);
			}

         if(command_Read() == CMD_SETUP) mode_Change(MODE_SETTINGS);

			/* select a new station */
         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_LEFT) || command_Read() == CMD_MIDDLE) mode_Change(MODE_STATION);

			/* short station memo */
         if((command_Read() == CMD_SWITCH1) || (command_Read() == CMD_SWITCH2) || (command_Read() == CMD_SWITCH3) || (command_Read() == CMD_SWITCH4))
         {
         	INT_U8 u08Memo = PROPERTY_STATION_MEMO1;
         	char* cStation = 0;

         	switch(command_Read())
         	{
         		case CMD_SWITCH1:	u08Memo = PROPERTY_STATION_MEMO1; break;
         		case CMD_SWITCH2:	u08Memo = PROPERTY_STATION_MEMO2; break;
         		case CMD_SWITCH3:	u08Memo = PROPERTY_STATION_MEMO3; break;
         		case CMD_SWITCH4:	u08Memo = PROPERTY_STATION_MEMO4; break;
         		default:	break;
         	}

				/* set new staion and change */
				cStation = property_ReadString(u08Memo);
				if(cStation[0] == 0)
				{
					Dogm204_PrintStringXY("    no station      ", 0, 0);
					Dogm204_PrintStringXY("  in memory, push   ", 1, 0);
					Dogm204_PrintStringXY("   button long to   ", 2, 0);
					Dogm204_PrintStringXY("  save the station  ", 3, 0);
					sleep(3);
					mode_Change(MODE_PLAY);
				}
				else
				{
					property_WriteString(PROPERTY_STATION, cStation);
					mode_Change(MODE_CHANGE_STATION);
				}
         }

         if((command_Read() == CMD_SWITCH1_SET) || (command_Read() == CMD_SWITCH2_SET) || (command_Read() == CMD_SWITCH3_SET) || (command_Read() == CMD_SWITCH4_SET))
         {
         	INT_U8 u08Memo = PROPERTY_STATION_MEMO1;

         	switch(command_Read())
         	{
         		case CMD_SWITCH1_SET:	u08Memo = PROPERTY_STATION_MEMO1; break;
         		case CMD_SWITCH2_SET:	u08Memo = PROPERTY_STATION_MEMO2; break;
         		case CMD_SWITCH3_SET:	u08Memo = PROPERTY_STATION_MEMO3; break;
         		case CMD_SWITCH4_SET:	u08Memo = PROPERTY_STATION_MEMO4; break;
         		default:	break;
         	}

				/* set new staion and change */
            property_WriteString(u08Memo, property_ReadString(PROPERTY_STATION));
				Dogm204_PrintStringXY("save station to memo", 3, 0);
				sleep(2);
				Dogm204_PrintStringXY("                    ", 3, 0);
         }

         /* short power button push */
         if(command_Read() == CMD_POWER_SLEEP) mode_Change(MODE_STANDBY);

         /* long power button push */
         if(command_Read() == CMD_POWER_OFF) mode_Change(MODE_POWEROFF);

         /* volume control */
         if(command_Read() == CMD_UP)
         {
         	property_Increment(PROPERTY_VOLUME, 100);
         	Mplayer_Volume(property_Increment(PROPERTY_VOLUME, 100));
         }

         if(command_Read() == CMD_DOWN)
         {
         	property_Decrement(PROPERTY_VOLUME, 0);
         	Mplayer_Volume(property_Decrement(PROPERTY_VOLUME, 50));
         }

			/* volume display */
         if(command_Read())
         {
         	Dogm204_DisplayProgressBar((float) (property_Read(PROPERTY_VOLUME) - 50) * 2.0 , 10, 3, 10);
         	Dogm204_PrintChar(0x15, 3 ,8);
         }

			if(hwtime_Check(TIME_500MS))
			{
				/* the offset for your country */
				#define COUNTRY_TIME_OFFSET 1

				time_t rawtime;
				struct tm * timeinfo;
				Dogm204_DisplayMove(2, 0, 20);

				/* get time */
				time (&rawtime);
				timeinfo = gmtime(&rawtime);

				/* winter or summertime */
				if(timeinfo->tm_isdst > 0) sprintf(cText, " %02d:%02d", (timeinfo->tm_hour + COUNTRY_TIME_OFFSET)%24, timeinfo->tm_min);
				else sprintf(cText, " %02d:%02d", (timeinfo->tm_hour + COUNTRY_TIME_OFFSET + 1)%24, timeinfo->tm_min);
				Dogm204_PrintStringXY(cText, 0, 14);

				/* move display stream info */
				Dogm204_DisplayMove(2, 0, 20);

				/* check time for the news station */
				if(property_Read(PROPERTY_NEWS_STATE) == 1)
				{
					INT_U8 u08Loop, u08StartTime[4];

					for(u08Loop = 0 ; u08Loop < 4 ; u08Loop++)
					{
						u08StartTime[u08Loop] = property_Read(PROPERTY_NEWS_STARTTIME) + (u08Loop * property_Read(PROPERTY_NEWS_INTERVAL));
						/* wrap around ? */
						if(u08StartTime[u08Loop] > 59) u08StartTime[u08Loop] -= 60;
						if(u08StartTime[u08Loop] > 59) u08StartTime[u08Loop] -= 60;
						if(u08StartTime[u08Loop] > 59) u08StartTime[u08Loop] -= 60;
					}

					/* check news station time */
					u08Minuten = (INT_U8)timeinfo->tm_min;

					if((u08Minuten == u08StartTime[0]) || (u08Minuten == u08StartTime[1]) || (u08Minuten == u08StartTime[2]) || (u08Minuten == u08StartTime[3]))
					{
						if(u08NewsStart == 0)
						{
							u08NewsStart = 1;

							Dogm204_Clear();
							Dogm204_PrintStringXY("    !!! NEWS !!!    ", 1, 0);
							sleep(2);

							u08NewsStopTime = u08Minuten + property_Read(PROPERTY_NEWS_DURATIONTIME);

							/* wrap around ? */
							if(u08NewsStopTime > 59) u08NewsStopTime -= 60;

							/* save old station */
							property_WriteString(PROPERTY_TEMPORARYNAME, property_ReadString(PROPERTY_STATION));

							/* set new staion and change */
							property_WriteString(PROPERTY_STATION, property_ReadString(PROPERTY_NEWS_STATION));
							mode_Change(MODE_CHANGE_STATION);
						}
					}

					if(u08Minuten == u08NewsStopTime)
					{
						if(u08NewsStart == 1)
						{
							u08NewsStart = 0;

							Dogm204_Clear();
							Dogm204_PrintStringXY("  !!! NEWS END !!!  ", 1, 0);
							sleep(2);

							/* set new staion and change */
							property_WriteString(PROPERTY_STATION, property_ReadString(PROPERTY_TEMPORARYNAME));
							mode_Change(MODE_CHANGE_STATION);
						}
					}
				}
			}
      }
      break;
   }
}


/*************************************************************************
*   M O D E   S T A T I O N
*************************************************************************/
void mode_Station(void)
{
   INT_U8 u08Loop;
   static INT_U8 u08ChangeStation = 1, u08DisplayPosition = 0;

   /* Init with 10 minutes timeout */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* go into play menue */
			mode_Change(MODE_PLAY);
      }
      break;

      case MODE_INIT:
      {
      	/* refresh the display data */
         command_DisplayUpdate();
      }
      default:
      {
         /* save station and connect */
         if(command_Read() == CMD_MIDDLE)
         {
				/* set new staion and change */
            property_WriteString(PROPERTY_STATION, station_Read(u08ChangeStation + u08DisplayPosition));
            mode_Change(MODE_CHANGE_STATION);
         }

			/* select the station */
         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_DOWN))
         {
            mode_TimeoutTrigger();
				mode_IncPosition(&u08DisplayPosition, &u08ChangeStation, station_MaxEntries(), (DISPLAY_HIGH - 1));
         }

         if((command_Read() == CMD_LEFT) || (command_Read() == CMD_UP))
         {
            mode_TimeoutTrigger();
				mode_DecPosition(&u08DisplayPosition, &u08ChangeStation, 1, 0);
         }

         /* short power button push */
         if(command_Read() == CMD_POWER_SLEEP) mode_Change(MODE_STANDBY);

         /* long power button push */
         if(command_Read() == CMD_POWER_OFF) mode_Change(MODE_POWEROFF);

         if(command_Read())
         {
            for(u08Loop = 0 ; u08Loop < (DISPLAY_HIGH); u08Loop++)
            {
            	int iLength;
            	char cText[64];

            	memset(cText, 0 , sizeof(cText));
            	iLength = strlen(station_Read((u08ChangeStation + u08Loop)));

			      /* cut extension .m3u from name */
            	if((iLength >= 4) && (iLength < sizeof(cText)))
            	{
            		memcpy(cText, station_Read(u08ChangeStation + u08Loop), (iLength - 4));
            	}

               Dogm204_DisplayWriteText(u08Loop, cText);

               if(u08Loop == u08DisplayPosition) Dogm204_PrintChar('>', u08Loop, 0);
               else Dogm204_PrintChar(' ', u08Loop, 0);
               Dogm204_DisplayPrint(u08Loop, 1);
            }
         }

         /* with switch4 go back to play mode */
			if(command_Read() == CMD_SWITCH4)
			{
				mode_Change(MODE_PLAY);
				Dogm204_DisplayWriteText(2, "                    ");
			}

			/* move display if name longer than 19 chars */
         if(hwtime_Check(TIME_500MS)) Dogm204_DisplayMove(u08DisplayPosition, 1, 19);
      }
   }
}


/*************************************************************************
*   M O D E   C H A N G E   S T A T I O N
*************************************************************************/

void mode_ChangeStation(void)
{
	char cText[128];
	static INT_U8 u08Sequenz = 0, u08SequenzCount = 0, u08ConectionCount = 99;

   /* Init with 60 seconds timeout */
   switch(mode_Init(60))
   {
      case MODE_TIMEOUT:
      {
			/* Clear Screen */
			Dogm204_Clear();
			Dogm204_PrintStringXY("      station       ", 1, 0);
			Dogm204_PrintStringXY("   not available    ", 2, 0);
		   Dogm204_DisplayWriteText(2, "station not available, try it again");
			sleep(2);

			/* start with 99, the connection begin immediately */
			u08ConectionCount = 99;

			/* and go into play menue */
			mode_Change(MODE_PLAY);
      }
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
			Dogm204_PrintStringXY("  connect station   ", 1, 0);
		   Dogm204_DisplayWriteText(2, "no stream info");

			/* start with 99, the connection begin immediately */
			u08ConectionCount = 99;

			/* get old stream info bevor connect */
			(void)Mplayer_GetStreamInfo(cText, sizeof(cText));
      }

      default:
      {
         /* with switch4 go back to play mode */
			if((command_Read() == CMD_MIDDLE) || (command_Read() == CMD_SWITCH4))
			{
				mode_Change(MODE_PLAY);
				Dogm204_DisplayWriteText(2, "                    ");
			}

			/* select new station */
         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_LEFT)) mode_Change(MODE_STATION);

         /* short power button push */
         if(command_Read() == CMD_POWER_SLEEP) mode_Change(MODE_STANDBY);

         /* long power button push */
         if(command_Read() == CMD_POWER_OFF) mode_Change(MODE_POWEROFF);

			/* get stream info and clear info when new info receive */
			if(Mplayer_GetStreamInfo(cText, sizeof(cText)) == 1)
			{
				/* load stream info into display buffer */
				Dogm204_DisplayWriteText(2, cText);
				/* and go into play menue */
				mode_Change(MODE_PLAY);
			}
			else
			{
				/* connect animation */
				if(hwtime_Check(TIME_100MS))
				{
					u08ConectionCount++;
					/* connect every 10 seconds */
					if(u08ConectionCount > 99)
					{
						int iLoop = 0;
						char cEscapeStation[256];
						char* pText = cText;

						u08ConectionCount = 0;

						/* copy station name */
						strcpy(cText, property_ReadString(PROPERTY_STATION));
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
						sprintf(cText,"%s/Station/%s", property_ReadString(PROPERTY_WORKING_PATH), cEscapeStation);
						Mplayer_PlayFile(cText);
					}

					u08SequenzCount++;
					if(u08SequenzCount == 2)
					{
						u08SequenzCount = 0;

						switch (u08Sequenz++)
						{
							case 0:  Dogm204_PrintStringXY("   *                ", 2, 0); break;
							case 1:  Dogm204_PrintStringXY("    *               ", 2, 0); break;
							case 2:  Dogm204_PrintStringXY("     *              ", 2, 0); break;
							case 3:  Dogm204_PrintStringXY("      *             ", 2, 0); break;
							case 4:  Dogm204_PrintStringXY("       *            ", 2, 0); break;
							case 5:  Dogm204_PrintStringXY("        *           ", 2, 0); break;
							case 6:  Dogm204_PrintStringXY("         *          ", 2, 0); break;
							case 7:  Dogm204_PrintStringXY("          *         ", 2, 0); break;
							case 8:  Dogm204_PrintStringXY("           *        ", 2, 0); break;
							case 9:  Dogm204_PrintStringXY("            *       ", 2, 0); break;
							case 10: Dogm204_PrintStringXY("             *      ", 2, 0); break;
							case 11: Dogm204_PrintStringXY("              *     ", 2, 0); break;
							case 12: Dogm204_PrintStringXY("               *    ", 2, 0); u08Sequenz = 0; break;
							default:
							break;
						}
					}
				}
			}
      }
      break;
   }
}


