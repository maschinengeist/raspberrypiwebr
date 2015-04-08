/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mytypes.h"

#include "mode.h"
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

#define STRINGLENGHT_STATION_NAME 34
#define STRINGLENGHT_STATION_ADDRESS 127
#define STRINGLENGHT_NETWORK_RSSI 63
#define STRINGLENGHT_NETWORK_KEY 127

extern void settings_NameEdit(void);

/*************************************************************************
*   S T A T I C
*************************************************************************/

static char cSelection[MAX_SETTINGS][20] =  {{"add new stations"},
															{"clear stations"},
															{"set start volume"},
															{"set news station"},
															{"network settings"}};

static INT_U8 u08NextMode = MODE_PLAY;

/* variale for name edit */
static INT_U8 u08AlphabetOffset = 0, u08AlphabetPosition = 0, u08CharToggle = 0;
static INT_U8 u08NameOffset = 0, u08NamePosition = 0, u08MaxNameSize = 34;
static INT_U8 u08AlphabetSelection = 0, u08AlphabetOffsetTable[4] = {0, 16, 33, 65};
static char cAlphabet[128], cInputName[128];

/*************************************************************************
*
*************************************************************************/
void mode_Selection(void)
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
      	Dogm204_PrintStringXY("       chose        ",1 ,0);
      	Dogm204_PrintStringXY("   your settings    ",2 ,0);
			sleep(1);
         command_DisplayUpdate();
      }

      default:
      {
         /* select the settings */
         if(command_Read() == CMD_MIDDLE)
         {
         	switch(u08Selection + u08DisplayPosition)
         	{
         		case 0: mode_Change(MODE_SETTINGS_STATION_NAME); break;
         		case 1: mode_Change(MODE_SETTINGS_STATION_CLEAR); break;
         		case 2: mode_Change(MODE_SETTINGS_STARTVOLUME); break;
         		case 3: mode_Change(MODE_NEWS_SELECTION); break;
         		case 4: mode_Change(MODE_SETTINGS_NETWORK_RSSI); break;
         		default: break;
         	}
         }

         if((command_Read() == CMD_RIGHT) || (command_Read() == CMD_DOWN)) mode_IncPosition(&u08DisplayPosition, &u08Selection,  4, 3);
         if((command_Read() == CMD_LEFT) || (command_Read() == CMD_UP)) mode_DecPosition(&u08DisplayPosition, &u08Selection, 0, 0);

         if(command_Read())
         {
         	mode_TimeoutTrigger();

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
				mode_Change(MODE_PLAY);
				Dogm204_DisplayWriteText(2, "                    ");
			}

         if(hwtime_Check(TIME_500MS)) Dogm204_DisplayMove(u08DisplayPosition, 1, 19);
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_Startvolume(void)
{
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
      	Dogm204_Clear();
      	Dogm204_PrintStringXY("     set your       ",1 ,0);
      	Dogm204_PrintStringXY("    startvolume     ",2 ,0);
      	sleep(1);
      	Dogm204_Clear();
         command_DisplayUpdate();
      }

      default:
      {
         /* with switch4 or switch middle go back to settings mode */
         if((command_Read() == CMD_MIDDLE) || (command_Read() == CMD_SWITCH4)) mode_Change(MODE_SETTINGS);

			/* change startvolume */
         if((command_Read() == CMD_UP) || (command_Read() == CMD_RIGHT))
         {
         	property_Increment(PROPERTY_STARTVOLUME, 100);
         	Mplayer_Volume(property_Increment(PROPERTY_STARTVOLUME, 100));
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_STARTVOLUME));
         }

         if((command_Read() == CMD_DOWN) || (command_Read() == CMD_LEFT))
         {
         	property_Decrement(PROPERTY_STARTVOLUME, 50);
         	Mplayer_Volume(property_Decrement(PROPERTY_STARTVOLUME, 50));
         	property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_STARTVOLUME));
         }

         if(command_Read())
         {
         	mode_TimeoutTrigger();
         	Dogm204_PrintStringXY("        start volume", 2 ,0);

         	Dogm204_DisplayProgressBar((float) (property_Read(PROPERTY_VOLUME) - 50) * 2.0 , 10, 3, 10);
         	Dogm204_PrintChar(0x15, 3 ,8);
         }
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_StationName(void)
{
	INT_U8 u08Loop;

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
			Dogm204_Clear();
			Dogm204_PrintStringXY("      edit the      ",1 ,0);
			Dogm204_PrintStringXY("    station name    ",2 ,0);
			sleep(1);
			Dogm204_Clear();
			Dogm204_PrintStringXY("[<]  [>]  clear next",3 ,0);

			u08AlphabetOffset = u08AlphabetPosition = 0;
			u08NameOffset = u08NamePosition = 0;
			u08AlphabetSelection = 0;

			/* Max leght of name */
			u08MaxNameSize = STRINGLENGHT_STATION_NAME;

			memset(cAlphabet, ' ', sizeof(cAlphabet));
			memset(cInputName, ' ', sizeof(cInputName));

			for(u08Loop = 0; u08Loop < 0x5e; u08Loop++)
			{
				cAlphabet[u08Loop] = u08Loop + 0x20;
			}
			command_DisplayUpdate();

			u08NextMode = MODE_SETTINGS_STATION_ADDRESS;
      }

      default:
      {
         /*  */
         mode_NameEdit();

         if(command_Read() == CMD_SWITCH4)
         {
         	INT_U8 u08Loop;

         	for(u08Loop = (INT_U8)(strlen(cInputName) - 1); u08Loop > 0 ; u08Loop--)
         	{
         		/* add nulltermination at end of name */
         		if(cInputName[u08Loop] ==' ') cInputName[u08Loop]  = 0;
         		else break;
         	}

         	property_WriteString(PROPERTY_TEMPORARYNAME, cInputName);

				mode_Change(MODE_SETTINGS_STATION_ADDRESS);
         }
      }
   }
}


/*************************************************************************
*
*************************************************************************/
void mode_StationAddress(void)
{
	char cFullPath[128];
	INT_U8 u08Loop;

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
			Dogm204_Clear();
			Dogm204_PrintStringXY("     edit the       ",1 ,0);
			Dogm204_PrintStringXY(" station address    ",2 ,0);
			sleep(1);
			Dogm204_Clear();
			Dogm204_PrintStringXY("[<]  [>]  clear save",3 ,0);

			u08AlphabetOffset = u08AlphabetPosition = 0;
			u08NameOffset = u08NamePosition = 0;
			u08AlphabetSelection = 0;

			/* Max leght of address */
			u08MaxNameSize = STRINGLENGHT_STATION_ADDRESS;

			memset(cAlphabet, ' ', sizeof(cAlphabet));
			memset(cInputName, ' ', sizeof(cInputName));

			for(u08Loop = 0; u08Loop < 0x5e; u08Loop++)
			{
				cAlphabet[u08Loop] = u08Loop + 0x20;
			}
			command_DisplayUpdate();

			u08NextMode = MODE_SETTINGS;
      }

      default:
      {
         /*  */
         mode_NameEdit();

         if(command_Read() == CMD_SWITCH4)
         {
         	INT_U8 u08Loop;

         	for(u08Loop = (INT_U8)(strlen(cInputName) - 1); u08Loop > 0 ; u08Loop--)
         	{
         		/* add nulltermination at end of name */
         		if(cInputName[u08Loop] ==' ') cInputName[u08Loop]  = 0;
         		else break;
         	}

				/* add "Station" directory and save the station */
         	sprintf(cFullPath,"%s/Station", property_ReadString(PROPERTY_WORKING_PATH));
         	station_Write(cFullPath, property_ReadString(PROPERTY_TEMPORARYNAME), cInputName);

				Dogm204_Clear();
				Dogm204_PrintStringXY("    save station    ",1 ,0);
				Dogm204_PrintStringXY("                    ",2 ,0);
				sleep(1);

				mode_Change(MODE_SETTINGS);
         }
      }
   }
}

/*************************************************************************
*
*************************************************************************/
void mode_NetworkRssi(void)
{
	INT_U8 u08Loop;

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
			Dogm204_Clear();
			Dogm204_PrintStringXY("      edit the      ",1 ,0);
			Dogm204_PrintStringXY("  network rssi name ",2 ,0);
			sleep(1);
			Dogm204_Clear();
			Dogm204_PrintStringXY("[<]  [>]  clear next",3 ,0);

			u08AlphabetOffset = u08AlphabetPosition = 0;
			u08NameOffset = u08NamePosition = 0;
			u08AlphabetSelection = 0;

			/* Max leght of rssi name */
			u08MaxNameSize = STRINGLENGHT_NETWORK_RSSI;

			memset(cAlphabet, ' ', sizeof(cAlphabet));
			memset(cInputName, ' ', sizeof(cInputName));

			for(u08Loop = 0; u08Loop < 0x5e; u08Loop++)
			{
				cAlphabet[u08Loop] = u08Loop + 0x20;
			}
			command_DisplayUpdate();

			u08NextMode = MODE_SETTINGS_NETWORK_KEY;
      }

      default:
      {
         /*  */
         mode_NameEdit();

         if(command_Read() == CMD_SWITCH4)
         {
         	INT_U8 u08Loop;

         	for(u08Loop = (INT_U8)(strlen(cInputName) - 1); u08Loop > 0 ; u08Loop--)
         	{
         		/* add nulltermination at end of name */
         		if(cInputName[u08Loop] ==' ') cInputName[u08Loop]  = 0;
         		else break;
         	}

         	property_WriteString(PROPERTY_TEMPORARYNAME, cInputName);

				mode_Change(MODE_SETTINGS_NETWORK_KEY);
         }
      }
   }
}


/*************************************************************************
*
*************************************************************************/
void mode_NetworkKey(void)
{
	INT_U8 u08Loop;

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
			Dogm204_Clear();
			Dogm204_PrintStringXY("     edit the       ",1 ,0);
			Dogm204_PrintStringXY("    network key     ",2 ,0);
			sleep(1);
			Dogm204_Clear();
			Dogm204_PrintStringXY("[<]  [>]  clear save",3 ,0);

			u08AlphabetOffset = u08AlphabetPosition = 0;
			u08NameOffset = u08NamePosition = 0;
			u08AlphabetSelection = 0;

			/* Max leght of network key */
			u08MaxNameSize = STRINGLENGHT_NETWORK_KEY;

			memset(cAlphabet, ' ', sizeof(cAlphabet));
			memset(cInputName, ' ', sizeof(cInputName));

			for(u08Loop = 0; u08Loop < 0x5e; u08Loop++)
			{
				cAlphabet[u08Loop] = u08Loop + 0x20;
			}
			command_DisplayUpdate();

			u08NextMode = MODE_SETTINGS;
      }

      default:
      {
         /*  */
         mode_NameEdit();

         if(command_Read() == CMD_SWITCH4)
         {
         	INT_U8 u08Loop;

         	for(u08Loop = (INT_U8)(strlen(cInputName) - 1); u08Loop > 0 ; u08Loop--)
         	{
         		/* add nulltermination at end of name */
         		if(cInputName[u08Loop] ==' ') cInputName[u08Loop]  = 0;
         		else break;
         	}

         	//station_Write(property_ReadString(PROPERTY_WORKING_PATH), property_ReadString(PROPERTY_TEMPORARYNAME), cInputName);

				Dogm204_Clear();
				Dogm204_PrintStringXY("    save network    ",1 ,0);
				Dogm204_PrintStringXY("      settings      ",2 ,0);
				sleep(1);

				mode_Change(MODE_SETTINGS);
         }
      }
   }
}


/*************************************************************************
*
*************************************************************************/
void mode_StationClear(void)
{
	char cFullPath[128];
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
			Dogm204_Clear();
			Dogm204_PrintStringXY(" select the station ",1 ,0);
			Dogm204_PrintStringXY("      to clear      ",2 ,0);
         command_DisplayUpdate();
      }

      default:
      {
      	if(command_Read() == CMD_MIDDLE)
         {
				Dogm204_Clear();
				Dogm204_PrintStringXY("   clear station    ",1 ,0);
				Dogm204_PrintStringXY("                    ",2 ,0);
				sleep(1);

				/* add "Station" directory and clear the station */
         	sprintf(cFullPath,"%s/Station", property_ReadString(PROPERTY_WORKING_PATH));
				station_Clear(cFullPath, (u08DisplayPosition + u08ChangeStation));

				mode_Change(MODE_SETTINGS);
         }

      	if(command_Read() == CMD_SWITCH4)
         {
				mode_Change(MODE_SETTINGS);
         }

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

         if(command_Read())
         {
            for(u08Loop = 0 ; u08Loop < DISPLAY_HIGH; u08Loop++)
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
      }
   }
}


/*************************************************************************
*
*************************************************************************/
void mode_NameEdit(void)
{
   char cText[21];

	/*  */
	if(command_Read() == CMD_MIDDLE)
	{
		cInputName[u08NamePosition + u08NameOffset] = (0x20 + u08AlphabetOffset + u08AlphabetPosition);
		mode_IncPosition(&u08NamePosition, &u08NameOffset, u08MaxNameSize, (DISPLAY_WIDTH - 1));
	}

	if(command_Read() == CMD_SWITCH1)
	{
		mode_IncPosition(&u08NamePosition, &u08NameOffset, u08MaxNameSize, (DISPLAY_WIDTH - 1));

		if(cInputName[u08NamePosition + u08NameOffset] == 0)
		{
			cInputName[u08NamePosition + u08NameOffset] = (0x20 + u08AlphabetOffset + u08AlphabetPosition);
		}
	}

	if(command_Read() == CMD_SWITCH2)
	{
		mode_DecPosition(&u08NamePosition, &u08NameOffset, 0, 0);

		if(cInputName[u08NamePosition + u08NameOffset] == 0)
		{
			cInputName[u08NamePosition + u08NameOffset] = (0x20 + u08AlphabetOffset + u08AlphabetPosition);
		}
	}

	if(command_Read() == CMD_SWITCH3)
	{
		memset(cInputName, ' ', sizeof(cInputName));
		u08NamePosition = 0;

		memcpy(cText, &cInputName[u08NameOffset], 20);
		Dogm204_PrintStringXY(cText, 0 ,0);
	}

	if(command_Read() == CMD_UP)
	{
		mode_Inc(&u08AlphabetSelection, 3);
		u08AlphabetOffset = u08AlphabetOffsetTable[u08AlphabetSelection];
		u08AlphabetPosition = 0;
	}

	if(command_Read() == CMD_DOWN)
	{
		mode_Dec(&u08AlphabetSelection, 0);
		u08AlphabetOffset = u08AlphabetOffsetTable[u08AlphabetSelection];
		u08AlphabetPosition = 0;
	}

	if(command_Read() == CMD_RIGHT) mode_IncPosition(&u08AlphabetPosition, &u08AlphabetOffset, 0x5d, (DISPLAY_WIDTH - 1));

	if(command_Read() == CMD_LEFT) mode_DecPosition(&u08AlphabetPosition, &u08AlphabetOffset, 0, 0);

	if(command_Read())
	{
		mode_TimeoutTrigger();

		memset(cText, 0, sizeof(cText));
		memcpy(cText, &cInputName[u08NameOffset], 20);
		Dogm204_PrintStringXY(cText, 0 ,0);

		memset(cText, 0, sizeof(cText));
		memcpy(cText, &cAlphabet[u08AlphabetOffset], 20);
		Dogm204_PrintStringXY(cText, 1 ,0);

		memset(cText, ' ', DISPLAY_WIDTH);
		cText[(u08AlphabetPosition)] = '^';
		Dogm204_PrintStringXY(cText, 2 ,0);
	}

	if(hwtime_Check(TIME_500MS))
	{
		memset(cText, 0, sizeof(cText));
		memcpy(cText, &cInputName[u08NameOffset], 20);

		if(u08CharToggle)
		{
			u08CharToggle = 0;
			cText[(u08NamePosition)] = ' ';
			Dogm204_PrintStringXY(cText, 0 ,0);
		}
		else
		{
			u08CharToggle = 1;
			cText[(u08NamePosition)] = '-';
			Dogm204_PrintStringXY(cText, 0 ,0);
		}
	}
 }


