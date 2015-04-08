
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "mytypes.h"
#include "debug.h"

#include "Init.h"
#include "HD44780.h"
#include "gpio.h"
#include "i2c.h"

/*************************************************************************
*   G L O B A L
*************************************************************************/

/*************************************************************************
*   F U N C T I O N S
*************************************************************************/

/**
*  \brief Init hardware and system setings
*
*/
void Init(void)
{
	int iResult;

	/* init rorary switchs ios */
	gpio_Init(17, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(22, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(21, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);

	iResult =  HD44780_Init(0);

	if(iResult < 0)
	{
		printf("Display Open error\r\n");
		exit(-1);
	}
}





