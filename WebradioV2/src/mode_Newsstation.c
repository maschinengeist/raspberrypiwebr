/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mytypes.h"

#include "mode.h"
#include "mode_Newsstation.h"
#include "mode_Power.h"
#include "mode_Play.h"
#include "mode_Settings.h"

#include "debug.h"
#include "station.h"
#include "property.h"
#include "Dogm204.h"
#include "Dogm204_UserInterface.h"
#include "mode.h"
#include "hwtime.h"
#include "command.h"
#include "station.h"
#include "Mplayer.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define MAX_SETTINGS 5

/*************************************************************************
*   S T A T I C
*************************************************************************/

static char cSelection[MAX_SETTINGS][20] =  {{"news      "},
															{"news station"},
															{"news start time"},
															{"news duration"},
															{"news interval"}};

static char cInterval[MAX_SETTINGS][20] =    {{"every 1/4 hour"},
															{"every 1/2 hour"},
															{"every 1 hour"},
															{""}};


/*************************************************************************
*
*************************************************************************/
void mode_NewsSelection(void)
{
   INT_U8 u08Loop;
   static INT_U8 u08Selection = 0, u08DisplayPosition = 0;

   /* Init with 10 minutes timeout */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* go into play menue */
			mode_Change(MODE_PLAY);
			Dogm204_DisplayWriteText(2, "                    ");

			/* save the Webradio settings */
			if(property_Save(property_ReadString(PROPERTY_WORKING_PATH)) < 0) printf("Settings write error\r\n");
      }
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
         command_DisplayUpdate();
         u08Selection = 0, u08DisplayPosition = 0;
      }

      default:
      {
         /* select the settings */
         if(command_Read() == CMD_MIDDLE)
         {
         	switch(u08Selection + u08DisplayPosition)
         	{
         		case 0:
         		{
						if(property_Read(PROPERTY_NEWS_STATE) == 0)
						{
							if(strcmp(property_ReadString(PROPERTY_NEWS_STATION), "") == 0)
							{
								Dogm204_Clear();
								Dogm204_PrintStringXY("  you must selsect  ",1 ,0);
								Dogm204_PrintStringXY("   a news station   ",2 ,0);
								sleep(2);
								Dogm204_Clear();
								command_DisplayUpdate();
							}
							else	property_Write(PROPERTY_NEWS_STATE, 1);
						}
						else property_Write(PROPERTY_NEWS_STATE, 0);
         		}
         		break;

         		case 1: mode_Change(MODE_NEWS_STATION); break;
         		case 2: mode_Change(MODE_NEWS_STARTTIME); break;
         		case 3: mode_Change(MODE_NEWS_DURATION); break;
         		case 4: mode_Change(MODE_NEWS_INTERVAL); break;
         		default: break;
         	}
         }

         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_DOWN)) mode_IncPosition(&u08DisplayPosition, &u08Selection,  4, 3);
         if((command_Read() == CMD_LEFT) || (command_Read() == CMD_UP)) mode_DecPosition(&u08DisplayPosition, &u08Selection, 0, 0);

         if(command_Read())
         {
         	mode_TimeoutTrigger();

				if(property_Read(PROPERTY_NEWS_STATE) == 1)
				{
					sprintf(&cSelection[0][5], "%s","[ on]");
				}
				else
				{
					sprintf(&cSelection[0][5], "%s","[off]");
					/* set state of news inactive */
					property_Write(PROPERTY_NEWS_START, 0);
				}

            for(u08Loop = 0 ; u08Loop < (DISPLAY_HIGH); u08Loop++)
            {
               Dogm204_DisplayWriteText((u08Loop), &cSelection[(u08Selection + u08Loop)][0]);

               if(u08Loop == u08DisplayPosition) Dogm204_PrintChar('>', (u08Loop), 0);
               else Dogm204_PrintChar(' ', (u08Loop), 0);
               Dogm204_DisplayPrint((u08Loop), 1);
            }
         }

         /* with switch4 go back to play mode */
			if(command_Read() == CMD_SWITCH4)
			{
				mode_Change(MODE_SETTINGS);

				/* save the Webradio settings */
				if(property_Save(property_ReadString(PROPERTY_WORKING_PATH)) < 0) printf("Settings write error\r\n");
			}

         if(hwtime_Check(TIME_500MS)) Dogm204_DisplayMove(u08DisplayPosition, 1, 19);
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_NewsStation(void)
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
			Dogm204_DisplayWriteText(2, "                    ");
      }
      break;

      case MODE_INIT:
      {
      	/* refresh the display data */
			Dogm204_Clear();
         command_DisplayUpdate();
      }
      default:
      {
         /* save station and connect */
         if(command_Read() == CMD_MIDDLE)
         {
				/* set news staion and change */
            property_WriteString(PROPERTY_NEWS_STATION, station_Read(u08ChangeStation + u08DisplayPosition));

				Dogm204_Clear();
				Dogm204_PrintStringXY("        save        ",1 ,0);
				Dogm204_PrintStringXY("    news station    ",2 ,0);
				sleep(1);

            mode_Change(MODE_NEWS_SELECTION);
         }

			/* select the news station */
         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_DOWN)) mode_IncPosition(&u08DisplayPosition, &u08ChangeStation, station_MaxEntries(), (DISPLAY_HIGH - 1));
         if((command_Read() == CMD_LEFT) || (command_Read() == CMD_UP)) mode_DecPosition(&u08DisplayPosition, &u08ChangeStation, 1, 0);

         /* short power button push */
         if(command_Read() == CMD_POWER_SLEEP) mode_Change(MODE_STANDBY);
         if(command_Read() == CMD_POWER_OFF) mode_Change(MODE_POWEROFF);

         if(command_Read())
         {
            mode_TimeoutTrigger();

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
			if(command_Read() == CMD_SWITCH4) mode_Change(MODE_NEWS_SELECTION);

			/* move display if name longer than 19 chars */
         if(hwtime_Check(TIME_500MS)) Dogm204_DisplayMove(u08DisplayPosition, 1, 19);
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_NewsStarttime(void)
{
   /* Init with 10 minutes timeout */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* go into play menue */
			mode_Change(MODE_PLAY);
			Dogm204_DisplayWriteText(2, "                    ");
      }
      break;

      case MODE_INIT:
      {
      	Dogm204_Clear();
      	Dogm204_PrintStringXY("     set the        ",1 ,0);
      	Dogm204_PrintStringXY("    start time      ",2 ,0);
      	sleep(1);
      	Dogm204_Clear();
         command_DisplayUpdate();
      }

      default:
      {
         /* with switch4 or switch middle go back to settings mode */
         if((command_Read() == CMD_MIDDLE) || (command_Read() == CMD_SWITCH4)) mode_Change(MODE_NEWS_SELECTION);

			/* change startvolume */
         if((command_Read() == CMD_UP) || (command_Read() == CMD_RIGHT))
         {
         	if(60 == property_Increment(PROPERTY_NEWS_STARTTIME, 60)) property_Write(PROPERTY_NEWS_STARTTIME, 0);
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_NEWS_STARTTIME));
         }

         if((command_Read() == CMD_DOWN) || (command_Read() == CMD_LEFT))
         {
				if(0 == property_Read(PROPERTY_NEWS_STARTTIME)) property_Write(PROPERTY_NEWS_STARTTIME, 59);
				else property_Decrement(PROPERTY_NEWS_STARTTIME, 0);
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_NEWS_STARTTIME));
         }

         if(command_Read())
         {
         	char cText[21];

         	mode_TimeoutTrigger();
         	Dogm204_PrintStringXY(" start time in HH:MM", 2 ,0);

         	sprintf(cText,"               HH:%02d", property_Read(PROPERTY_NEWS_STARTTIME));
         	Dogm204_PrintStringXY(cText, 3, 0);
         }
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_NewsDuration(void)
{
   /* Init with 10 minutes timeout */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* go into play menue */
			mode_Change(MODE_PLAY);
			Dogm204_DisplayWriteText(2, "                    ");
      }
      break;

		case MODE_INIT:
      {
      	Dogm204_Clear();
      	Dogm204_PrintStringXY("  set the duration  ",1 ,0);
      	Dogm204_PrintStringXY("    for the news    ",2 ,0);
      	sleep(1);
      	Dogm204_Clear();
         command_DisplayUpdate();
      }

      default:
      {
         /* with switch4 or switch middle go back to settings mode */
         if((command_Read() == CMD_MIDDLE) || (command_Read() == CMD_SWITCH4)) mode_Change(MODE_NEWS_SELECTION);

			/* change startvolume */
         if((command_Read() == CMD_UP) || (command_Read() == CMD_RIGHT))
         {
         	property_Increment(PROPERTY_NEWS_DURATIONTIME, 15);
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_NEWS_DURATIONTIME));
         }

         if((command_Read() == CMD_DOWN) || (command_Read() == CMD_LEFT))
         {
         	property_Decrement(PROPERTY_NEWS_DURATIONTIME, 1);
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_NEWS_DURATIONTIME));
         }

         if(command_Read())
         {
         	char cText[21];

         	mode_TimeoutTrigger();
         	Dogm204_PrintStringXY("   duration in HH:MM", 2 ,0);

         	sprintf(cText,"               HH:%02d", property_Read(PROPERTY_NEWS_DURATIONTIME));
         	Dogm204_PrintStringXY(cText, 3, 0);
         }
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_NewsInterval(void)
{
	INT_U8 u08Loop;
   static INT_U8 u08Selection = 0, u08DisplayPosition = 0;

   /* Init with 10 minutes timeout */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* go into play menue */
			mode_Change(MODE_PLAY);
			Dogm204_DisplayWriteText(2, "                    ");
      }
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
         command_DisplayUpdate();
      }

      default:
      {
         /* select the settings */
         if(command_Read() == CMD_MIDDLE)
         {
         	switch(u08Selection + u08DisplayPosition)
         	{
         		case 0: mode_Change(MODE_NEWS_SELECTION); property_Write(PROPERTY_NEWS_INTERVAL, 15); break;
         		case 1: mode_Change(MODE_NEWS_SELECTION); property_Write(PROPERTY_NEWS_INTERVAL, 30); break;
         		case 2: mode_Change(MODE_NEWS_SELECTION); property_Write(PROPERTY_NEWS_INTERVAL, 60); break;
         		default: break;
         	}
         }

         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_DOWN)) mode_IncPosition(&u08DisplayPosition, &u08Selection,  2, 2);
         if((command_Read() == CMD_LEFT) || (command_Read() == CMD_UP)) mode_DecPosition(&u08DisplayPosition, &u08Selection, 0, 0);

         if(command_Read())
         {
         	mode_TimeoutTrigger();

            for(u08Loop = 0 ; u08Loop < (DISPLAY_HIGH); u08Loop++)
            {
               Dogm204_DisplayWriteText((u08Loop), &cInterval[(u08Selection + u08Loop)][0]);

               if(u08Loop == u08DisplayPosition) Dogm204_PrintChar('>', (u08Loop), 0);
               else Dogm204_PrintChar(' ', (u08Loop), 0);
               Dogm204_DisplayPrint((u08Loop), 1);
            }
         }

         /* with switch4 go back to play mode */
			if(command_Read() == CMD_SWITCH4)
			{
				mode_Change(MODE_SETTINGS);
			}

         if(hwtime_Check(TIME_500MS)) Dogm204_DisplayMove(u08DisplayPosition, 1, 19);
      }
   }
}
