#include <linux/types.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "mytypes.h"
#include "debug.h"

#include "Station.h"
#include "Directory.h"
#include "message.h"
#include "Init.h"
#include "Mplayer.h"
#include "Network.h"
#include "Menue.h"

#include "gpio.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64


/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/

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

