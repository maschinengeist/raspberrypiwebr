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
	printf("Start\r\n");

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

void SVT_DebugCall(CHAR *pText)
{
	printf(pText);
}

