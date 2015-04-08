/*! \file message.c
*/



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

#include "message.h"
#include "gpio.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define USERINTERFACE_MESSAGE_MIDDLE  21
#define USERINTERFACE_MESSAGE_RIGHT   18
#define USERINTERFACE_MESSAGE_LEFT    17

#define MESSAGE_LOCK pthread_mutex_lock(&MessageMutex)
#define MESSAGE_UNLOCK pthread_mutex_unlock(&DisplayMutex)


/*************************************************************************
*   G L O B A L
*************************************************************************/

static int iMessage, iCurrentMessage;
static pthread_mutex_t MessageMutex;

/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/

#ifdef SIMULANT
INT_U16 u16RotarySwitch(void)
{

	INT_U16 u16CurrentState;
	struct termios oldt, newt;
	int ch;

	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	u16CurrentState = 0;

	switch(ch)
	{
	    printf("%d\n", ch);
		case 67:
			u16CurrentState |= 0x0001;
		break;

		case 68:
			u16CurrentState |= 0x0002;
		break;

		case 69:
			u16CurrentState |= 0x0004;
		break;

		case 65:
			u16CurrentState |= 0x0008;
		break;

		default:
		break;
	}

   return u16CurrentState;
}


void *MessageThread(void *arg)
{
	INT_U16 u16CurrentState;

	do
	{
		u16CurrentState = u16RotarySwitch();

		pthread_mutex_lock(&MessageMutex);

		/* read button state */
		if(u16CurrentState == 0x0004)
		{
			iMessage = MESSAGE_KEY_MIDDLE;
		}

		if(u16CurrentState == 0x0001)
		{
			iMessage = MESSAGE_KEY_RIGHT;
		}

		if(u16CurrentState == 0x0002)
		{
			iMessage = MESSAGE_KEY_LEFT;
		}

		if(u16CurrentState == 0x0008)
		{
			iMessage = MESSAGE_KEY_SETUP;
		}

		pthread_mutex_unlock(&MessageMutex);

		usleep(1000);
	}
	while(1);
}

#else

INT_U16 u16RotarySwitch(void)
{
	INT_U16 u16CurrentState;

	u16CurrentState = 0;
	if(gpio_Read(17) == 0) u16CurrentState |= 0x0001;
	if(gpio_Read(21) == 0) u16CurrentState |= 0x0002;
	if(gpio_Read(22) == 0) u16CurrentState |= 0x0004;

   return u16CurrentState;
}


void *MessageThread(void *arg)
{
	static INT_U16 u16OldState = 0;
	static INT_U16 u16Timeout = 100;
	INT_U16 u16CurrentState;

	do
	{
		u16CurrentState = u16RotarySwitch();

		/* read button state */
		if(u16CurrentState & 0x0004)
		{
			if(!(u16OldState & 0x0004))
			{
				u16OldState |= 0x0004;
			}
			else
			{
				if(u16Timeout)
				{
					if(u16Timeout > 1)
					{
						u16Timeout--;
					}
					else
					{

						u16Timeout = 100;
						iMessage = MESSAGE_KEY_SETUP;
					}
				}
			}
		}
		else
		{
		    if(u16OldState & 0x0004)
			{
				u16OldState &= ~0x0004;
				iMessage = MESSAGE_KEY_MIDDLE;
			}

			u16Timeout = 100;
		}

		/* read rotary swiches */
		u16CurrentState &= 0x03;

		pthread_mutex_lock(&MessageMutex);

		if((u16OldState & 0x03) == 0x00)
		{
			if(u16CurrentState == 2)
			{
				iMessage = MESSAGE_KEY_RIGHT;
			}

			if(u16CurrentState == 1)
			{
				iMessage = MESSAGE_KEY_LEFT;
			}
		}

		if((u16OldState & 0x03) == 0x03)
		{
			if(u16CurrentState == 1)
			{
				iMessage = MESSAGE_KEY_RIGHT;
			}

			if(u16CurrentState == 2)
			{
				iMessage = MESSAGE_KEY_LEFT;
			}
		}

		/* save current state */
		u16OldState &= 0x0004;
		u16OldState |=  (u16CurrentState & 0x0003);

		pthread_mutex_unlock(&MessageMutex);

		usleep(5000);
	}
	while(1);
}

#endif



#if 1
/************************************************************************
*/
/*! \fn int Message_Init(void)
*
*   \brief create thread to poll gpios
*
*   \return
*
*************************************************************************/
int Message_Init(void)
{
	pthread_t ThreadID;

	pthread_create(&ThreadID, NULL, MessageThread, NULL);
	return pthread_mutex_init(&MessageMutex, NULL);
}

#else





#endif


/************************************************************************
*/
/*! \fn void Message_Get(void)
*
*   \brief read an message
*
*   \return int --> 0 = no Message
*
*************************************************************************/
void Message_Get(void)
{
	pthread_mutex_lock(&MessageMutex);
	iCurrentMessage = iMessage;
	iMessage = 0;
	pthread_mutex_unlock(&MessageMutex);
}

/************************************************************************
*/
/*! \fn int Message_Read(void)
*
*   \brief read an message
*
*   \return int --> 0 = no Message
*
*************************************************************************/
int Message_Read(void)
{
   return iCurrentMessage;
}


/************************************************************************
*/
/*! \fn void Message_Set(int iMessage)
*
*   \brief set an message
*
*   \return int --> 0 = no Message
*
*************************************************************************/
void Message_Set(int iMessage)
{
   iCurrentMessage = iMessage;
}


/************************************************************************
*/
/*! \fn void Message_Clear(void)
*
*   \brief clear an pending message
*
*************************************************************************/
void Message_Clear(void)
{
	iMessage = 0;
}



