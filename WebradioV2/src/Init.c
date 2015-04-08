
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "mytypes.h"
#include "hwtime.h"
#include "debug.h"

#include "Init.h"
#include "Dogm204.h"
#include "io.h"
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

	io_Init();

	iResult = Dogm204_Init();

	hwtime_Init();

	if(iResult < 0)
	{
		printf("Display Open error\r\n");
		exit(-1);
	}
}





