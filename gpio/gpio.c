
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "gpio.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   F U N C T I O N
*************************************************************************/

/***********************************************************************
 */
/*! \fn RESULT gpio_Init(INT_U8 u08GpioPin, INT_U8 u08Direction, INT_U8 u08Edge)
 *
 *   \brief Init an gpio
 *
 *   \param u08GpioPin --> the gpio nummber to init
 *   \param u08Direction --> the gpio direction, can be GPIO_DIRECTION_IN or GPIO_DIRECTION_OUT
 *   \param u08Edge --> the gpio edge, can be GPIO_EDGE_NONE, GPIO_EDGE_FALLING,  GPIO_EDGE_RISING or GPIO_EDGE_BOTH
 *
 *   \return RESULT --> SB_OK = gpio is now available or SB_ERROR = th gpio is not available for the user
 *
 *************************************************************************/
RESULT gpio_Init(INT_U8 u08GpioPin, INT_U8 u08Direction, INT_U8 u08Edge)
{
	RESULT Result = SB_ERROR;
	FILE* fd_gpio = 0;

	/* create a variable to store path and value */
	CHAR cGpioPath[64], cGpioPort[8];


	/* Using sysfs we need to write "37" to /sys/class/gpio/export
	   This will create the folder /sys/class/gpio/gpio37 */

	sprintf(cGpioPort, "%d", u08GpioPin);

	if ((fd_gpio = fopen("/sys/class/gpio/export", "ab")) == 0)
	{
		DB_PRINTF(("Cannot open export file --> %d \n", u08GpioPin));
	}
	else
	{
		/* go to begin of file */
		rewind(fd_gpio);

		/* export the pin an make him available */
		(void)fwrite(cGpioPort, 1, strlen(cGpioPort), fd_gpio);
		fclose(fd_gpio);

		sprintf(cGpioPath, "/sys/class/gpio/gpio%d/direction", u08GpioPin);

		if ((fd_gpio = fopen(cGpioPath, "rb+")) == 0)
		{
			DB_PRINTF(("Cannot open direction file --> %d\n", u08GpioPin));
		}
		else
		{
			/* go to begin of file */
			rewind(fd_gpio);

			if(u08Direction == GPIO_DIRECTION_IN)
			{
				fwrite("in", 1, strlen("in"), fd_gpio);

				fclose(fd_gpio);

                sprintf(cGpioPath, "/sys/class/gpio/gpio%d/edge", u08GpioPin);

                if ((fd_gpio = fopen(cGpioPath, "rb+")) == 0)
                {
                    DB_PRINTF(("Cannot open direction file --> %d\n", u08GpioPin));
                }
                else
                {
                    /* go to begin of file */
                    rewind(fd_gpio);

                    if(u08Edge == GPIO_EDGE_NONE)
                    {
                        fwrite("none", 1, strlen("none"), fd_gpio);
                        Result = SB_OK;
                    }

                    if(u08Edge == GPIO_EDGE_FALLING)
                    {
                        fwrite("falling", 1, strlen("falling"), fd_gpio);
                        Result = SB_OK;
                    }

                    if(u08Edge == GPIO_EDGE_RISING)
                    {
                        fwrite("rising", 1, strlen("rising"), fd_gpio);
                        Result = SB_OK;
                    }

                    if(u08Edge == GPIO_EDGE_BOTH)
                    {
                        fwrite("both", 1, strlen("both"), fd_gpio);
                        Result = SB_OK;
                    }

                    fclose(fd_gpio);
                }
			}

			if(u08Direction == GPIO_DIRECTION_OUT)
			{
				fwrite("out", 1, strlen("out"), fd_gpio);
				fclose(fd_gpio);

				Result = SB_OK;
			}
		}
	}

	return Result;
}

/***********************************************************************
 */
/*! \fn RESULT gpio_Write(INT_U8 u08GpioPin, INT_U8* p08Value)
 *
 *   \brief Set state of the gpio
 *
 *   \param u08GpioPin --> the gpio nummber to init
 *   \param u08State --> the state of the pin 0 = gpio is low, or 1 gpio is high
 *
 *   \return RESULT --> SMA_OK = gpio is set or SMA_ERROR = th gpio is not available for the user
 *
 *************************************************************************/
RESULT gpio_Write(INT_U8 u08GpioPin, INT_U8 u08Value)
{
	RESULT Result = SB_ERROR;
	FILE* fd_gpio = 0;

	CHAR cGpioPath[64];

	sprintf(cGpioPath, "/sys/class/gpio/gpio%d/value", u08GpioPin);

	if ((fd_gpio = fopen(cGpioPath, "rb+")) == 0)
	{
		DB_PRINTF(("Cannot open value file --> %d\n", u08GpioPin));
	}
	else
	{
		if(u08Value == 0)
		{
			fwrite("0", 1, strlen("0"), fd_gpio);
			Result = SB_OK;
		}
		else
		{
			fwrite("1", 1, strlen("1"), fd_gpio);
			Result = SB_OK;
		}

		fclose(fd_gpio);
	}

	return Result;
}


/***********************************************************************
 */
/*! \fn INT_U8 gpio_Read(INT_U8 u08GpioPin)
 *
 *   \brief Read state of the gpio
 *
 *   \param u08GpioPin --> the gpio nummber to read
 *
 *   \return INT_U8 --> State of pin
 *
 *************************************************************************/
INT_U8 gpio_Read(INT_U8 u08GpioPin)
{
	FILE* fd_gpio = 0;
	INT_U8 u08State;

	CHAR cGpioPath[64], cState[8];

	sprintf(cGpioPath, "/sys/class/gpio/gpio%d/value", u08GpioPin);

	if ((fd_gpio = fopen(cGpioPath, "rb+")) == 0)
	{
		DB_PRINTF(("Cannot open value file --> %d\n", u08GpioPin));
	}
	else
	{
		fread(cState, 1, 1, fd_gpio);
		if(cState[0] == '1') u08State = 1;
		else u08State = 0;

		fclose(fd_gpio);
	}

	return u08State;
}



/***********************************************************************
 */
/*! \fn int gpio_Open(INT_U8 u08GpioPin)
 *
 *   \brief Open the gpio
 *
 *   \param u08GpioPin --> the gpio nummber to open
 *
 *************************************************************************/
int gpio_Open(INT_U8 u08GpioPin)
{
	int fd_gpio = 0;

	CHAR cGpioPath[64];

	sprintf(cGpioPath, "/sys/class/gpio/gpio%d/value", u08GpioPin);

	if ((fd_gpio = open(cGpioPath, O_RDONLY | O_NONBLOCK) == 0))
	{
		DB_PRINTF(("Cannot open gpio --> %d\n", u08GpioPin));
	}

	return fd_gpio;
}










