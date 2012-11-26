#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "mytypes.h"
#include "debug.h"

#include "message.h"
#include "Init.h"
#include "Mplayer.h"
#include "Network.h"
#include "Menue.h"





/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/


/* static global settings of the webradio */
static WEBRADIO WebRadio;


/****************************************************************
 * Main
 ****************************************************************/

int main(void)
{
	printf("start webradio  \r\n");
	printf("build 26.11.2012\r\n");

	Init();

	WebRadio.u08MenueState = 0;

	do
	{
		Menue_Select(&WebRadio);

		usleep(10000);
	}
	while(1);

   return 0;
}

void DebugPrintf(CHAR *pText)
{
	printf(pText);
}

