/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "mytypes.h"
#include "debug.h"
#include "command.h"
#include "hwtime.h"
#include "io.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define COMMAND_TIMEOUT    200
#define COMMAND_REPEATTCOUNTTIME 80
#define COMMAND_REPEATTIME 10

/*************************************************************************
*   G L O B A L
*************************************************************************/

#ifdef SIMULANT

static int iKey = 0;
static INT_U8 u08Command = 0;

#else

static INT_U8 u08ButtonPush = 0;
static INT_U16 u16Timeout = COMMAND_TIMEOUT;
static INT_U16 u16RepeatTime = COMMAND_REPEATTIME;
static INT_U16 u16RepeatCount = COMMAND_REPEATTCOUNTTIME;
static INT_U8 u08Command = 0, u08CurrentCommand = 0;

#endif


#ifdef SIMULANT

/************************************************************************
*/
/*! \brief Get a button or rc 5 command
*
*************************************************************************/


void *command_Thread(void *arg)
{
	struct termios oldt, newt;

	do
	{
		tcgetattr( STDIN_FILENO, &oldt );
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr( STDIN_FILENO, TCSANOW, &newt );
		iKey = getchar();
		//printf("%d\n",iKey);

		tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
		usleep(10000);
	}
	while(1);
}

void command_Get(void)
{
	static int iStart = 1;

	if(iStart)
	{
	   pthread_t ThreadID;

		iStart = 0;

		if(pthread_create(&ThreadID, NULL, command_Thread, NULL) < 0)
		{
			DB_PRINTF(("Thread create errorr\n"));
		}
	}

	if(iKey)
	{
		switch(iKey)
		{
			case 56: u08Command = CMD_UP; 				break;
			case 50: u08Command = CMD_DOWN; 				break;
			case 54: u08Command = CMD_RIGHT; 			break;
			case 52: u08Command = CMD_LEFT; 				break;
			case 53: u08Command = CMD_MIDDLE; 			break;
			case 43: u08Command = CMD_SETUP; 			break;
			case 10: u08Command = CMD_POWER_SLEEP; 	break;
			case 44: u08Command = CMD_POWER_OFF; 		break;
			case 55: u08Command = CMD_SWITCH1; 	   	break;
			case 57: u08Command = CMD_SWITCH2; 	 		break;
			case 49: u08Command = CMD_SWITCH3; 	 		break;
			case 51: u08Command = CMD_SWITCH4; 		 	break;
			case 113: u08Command = CMD_SWITCH1_SET; 	break;
			case 119: u08Command = CMD_SWITCH2_SET; 	break;
			case 101: u08Command = CMD_SWITCH3_SET; 	break;
			case 114: u08Command = CMD_SWITCH4_SET; 	break;
			default: break;
		}

		iKey = 0;
	}
}

#else

void *command_Thread(void *arg)
{
	do
	{
		if(io_Get(TASTER1) && io_Get(TASTER2) && io_Get(TASTER3) && io_Get(TASTER4) &&
			io_Get(TASTER_UP) && io_Get(TASTER_DOWN) && io_Get(TASTER_LEFT) && io_Get(TASTER_RIGHT) &&
			io_Get(TASTER_MIDDLE) && (!io_Get(TASTER_POWER)))
		{
			u08ButtonPush = 0;
			u16Timeout = COMMAND_TIMEOUT;
			u16RepeatTime = COMMAND_REPEATTIME;
			u16RepeatCount = COMMAND_REPEATTCOUNTTIME;

			if(u08CurrentCommand)
			{
				if(u08CurrentCommand == CMD_SWITCH1) 		u08Command = CMD_SWITCH1;
				if(u08CurrentCommand == CMD_SWITCH2) 		u08Command = CMD_SWITCH2;
				if(u08CurrentCommand == CMD_SWITCH3) 		u08Command = CMD_SWITCH3;
				if(u08CurrentCommand == CMD_SWITCH4) 		u08Command = CMD_SWITCH4;
				if(u08CurrentCommand == CMD_MIDDLE)  		u08Command = CMD_MIDDLE;
				if(u08CurrentCommand == CMD_POWER_SLEEP) 	u08Command = CMD_POWER_SLEEP;

				u08CurrentCommand = 0;
			}
		}
		else
		{
			if(u08ButtonPush == 0)
			{
				if(io_Get(TASTER_UP) == IO_OFF) u08Command = CMD_UP;
				else if(io_Get(TASTER_DOWN) == IO_OFF) u08Command = CMD_DOWN;
				if(io_Get(TASTER_RIGHT) == IO_OFF) u08Command = CMD_RIGHT;
				else if(io_Get(TASTER_LEFT) == IO_OFF) u08Command = CMD_LEFT;

				else if(io_Get(TASTER_MIDDLE) == IO_OFF) u08CurrentCommand = CMD_MIDDLE;

				else if(io_Get(TASTER_POWER) == IO_ON) u08CurrentCommand = CMD_POWER_SLEEP;

				else if(io_Get(TASTER1) == IO_OFF) u08CurrentCommand = CMD_SWITCH1;
				else if(io_Get(TASTER2) == IO_OFF) u08CurrentCommand = CMD_SWITCH2;
				else if(io_Get(TASTER3) == IO_OFF) u08CurrentCommand = CMD_SWITCH3;
				else if(io_Get(TASTER4) == IO_OFF) u08CurrentCommand = CMD_SWITCH4;

				u08ButtonPush = 1;
			}
			else
			{
				if(u16RepeatCount) u16RepeatCount--;
				else
				{
					if(u16RepeatTime) u16RepeatTime--;
					else
					{
						u16RepeatTime = COMMAND_REPEATTIME;

						if(io_Get(TASTER_UP) == IO_OFF) u08Command = CMD_UP;
						else if(io_Get(TASTER_DOWN) == IO_OFF) u08Command = CMD_DOWN;
						else if(io_Get(TASTER_RIGHT) == IO_OFF) u08Command = CMD_RIGHT;
						else if(io_Get(TASTER_LEFT) == IO_OFF) u08Command = CMD_LEFT;
					}
				}

				if(u16Timeout)
				{
					u16Timeout--;

					if(u16Timeout == 1)
					{
						if(io_Get(TASTER1) == IO_OFF) u08Command = CMD_SWITCH1_SET;
						else if(io_Get(TASTER2) == IO_OFF) u08Command = CMD_SWITCH2_SET;
						else if(io_Get(TASTER3) == IO_OFF) u08Command = CMD_SWITCH3_SET;
						else if(io_Get(TASTER4) == IO_OFF) u08Command = CMD_SWITCH4_SET;
						else if(io_Get(TASTER_MIDDLE) == IO_OFF) u08Command = CMD_SETUP;
						else if(io_Get(TASTER_POWER) == IO_ON) u08Command = CMD_POWER_OFF;
						u16Timeout = 0;
						u08CurrentCommand = 0;
					}
				}
			}
		}
		usleep(10000);
	}
	while(1);
}


/************************************************************************
*/
/*! \brief Get a button or rc 5 command
*
*************************************************************************/
void command_Get(void)
{
	static int iStart = 1;

	if(iStart)
	{
	   pthread_t ThreadID;

		iStart = 0;

		if(pthread_create(&ThreadID, NULL, command_Thread, NULL) < 0)
		{
			DB_PRINTF(("Thread create errorr\n"));
		}
	}
}

#endif

/************************************************************************
*/
/*! \brief Set Command for display update
*
*************************************************************************/
void command_DisplayUpdate(void)
{
   u08Command = CMD_DISPLAY_UPDATE;
}

/************************************************************************
*/
/*! \brief Read the command
*
*   \return INT_U8 --> 0 = no command or > 0 command ocurre
*
*************************************************************************/
INT_U8 command_Read(void)
{
   return u08Command;
}

/************************************************************************
*/
/*! \brief Clear the command
*
*************************************************************************/
void command_Clear(void)
{
   u08Command = 0;
}
