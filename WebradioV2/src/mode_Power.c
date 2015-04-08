/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
#include "mode_Power.h"
#include "io.h"


/*************************************************************************
*   S T A T I C
*************************************************************************/


/*************************************************************************
*   P L A Y E R   O N
*************************************************************************/
void mode_Poweron(void)
{
	int iResult;
	char cPath[128], cFullPath[128];

   /* no timeout */
   switch(mode_Init(0))
   {
      case MODE_TIMEOUT:
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
			Dogm204_Backlight(1);
			Dogm204_PrintStringXY("    raspberry pi    ", 0, 0);
			Dogm204_PrintStringXY("      webradio      ", 1, 0);
			Dogm204_PrintStringXY("  build 08.04.2015  ", 3, 0);

			/* switch power on usb port off */
			io_Set(USB_POWER, IO_ON);

#ifdef SIMULANT
			/* get full path */
			if(0 == realpath(".", cPath))
			{
				DB_PRINTF(("Can't find the current path\r\n"));
				exit(0);
			}

			/* add settings to path */
			sprintf(cFullPath, "%s%s", cPath, "/Settings");
#else
			sprintf(cPath, "/Webradio");
#endif
			/* write the current path */
			property_WriteString(PROPERTY_WORKING_PATH, cPath);

			/* read setting of the webradio */
			iResult = property_Load(cPath);
			if(iResult < 0)
			{
				DB_PRINTF(("Can't find Settings\r\n"));
				exit(0);
			}

			/* add station to path */
			sprintf(cFullPath, "%s/Station", cPath);

			if(station_Load(cFullPath) > 0)
			{
				/* start mplayer in slave mode */
				Mplayer_Start();

				/* copy startvolume to volume */
				property_Write(PROPERTY_VOLUME, property_Read(PROPERTY_STARTVOLUME));

				/* Enter_play mode */
				mode_Change(MODE_CHANGE_STATION);
			}
			else
			{
				DB_PRINTF(("Can't find Station.txt\r\n"));
				exit(0);
			}
      }

      default:
      break;
   }
}

/*************************************************************************
*   P L A Y E R   I N   S T A N D B Y
*************************************************************************/
void mode_Standby(void)
{
   /* Init with 2 seconds timeout befor enter sleep */
   switch(mode_Init(2))
   {
      case MODE_TIMEOUT:
      {
			/* backlight off and clear screen */
			Dogm204_Clear();

			/* save the Webradio settings */
			if(property_Save(property_ReadString(PROPERTY_WORKING_PATH)) < 0) printf("Settings write error\r\n");

			/* exit mplayer */
			Mplayer_Close();

			/* enter sleep mode */
			mode_Change(MODE_SLEEP);
      }
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
			Dogm204_PrintStringXY(" go into sleep mode", 1, 0);
      }

      default:
      break;
   }
}

/*************************************************************************
*   S W I T C H   P L A Y E R   O F F
*************************************************************************/
void mode_Poweroff(void)
{
   /* Init with 2 seconds timeout befor switch power off */
   switch(mode_Init(10))
   {
      case MODE_TIMEOUT:
      {
			/* power off, backlight off */
			Dogm204_Backlight(0);
			Dogm204_Clear();

			/* switch power for the raspberry pi off */
			io_Set(POWER_OFF, IO_ON);
			io_Set(POWER_OFF, IO_OFF);

			/* endless loop */
			while(1){sleep(1);}
      }
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
			Dogm204_PrintStringXY("  switch power off  ", 1, 0);
			sleep(1);

			/* save the Webradio settings */
			if(property_Save(property_ReadString(PROPERTY_WORKING_PATH)) < 0) printf("Settings write error\r\n");

			/* switch power on usb port off */
			io_Set(USB_POWER, IO_OFF);

			/* exit mplayer */
			Mplayer_Close();
      }

      default:
      break;
   }
}

/*************************************************************************
*   S W I T C H   P L A Y E R   O F F
*************************************************************************/
void mode_Sleep(void)
{
   /* no timeout */
   switch(mode_Init(0))
   {
      case MODE_TIMEOUT:
      break;

      case MODE_INIT:
      {
			Dogm204_Clear();
			Dogm204_PrintStringXY("     i'am sleep     ", 1, 0);
			sleep(1);
			Dogm204_Backlight(0);

			/* switch power on usb port off */
			io_Set(USB_POWER, IO_OFF);
      }

      default:
      {
      	/* short power button push */
			if(command_Read() == CMD_POWER_SLEEP) mode_Change(MODE_POWERON);

         /* long power button push */
         if(command_Read() == CMD_POWER_OFF)
         {
				Dogm204_PrintStringXY("  switch power off  ", 1, 0);
				sleep(5);

				/* switch power for the raspberry pi off */
				io_Set(POWER_OFF, IO_ON);
				io_Set(POWER_OFF, IO_OFF);

				/* endless loop */
				while(1){sleep(1);}
         }
      }
      break;
   }
}




