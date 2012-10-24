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

#if 0

int main(int argc, char **argv, char **envp)
{
	struct pollfd fdset[2];
	int nfds = 2;
	int gpio_fd, timeout, rc;
	char *buf[MAX_BUF];
	unsigned int gpio;
	int len;

    gpio_Init(22, GPIO_DIRECTION_IN, GPIO_EDGE_RISING);

	gpio_fd = gpio_Open(22);

	timeout = POLL_TIMEOUT;

	while (1) {
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;

		fdset[1].fd = gpio_fd;
		fdset[1].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}

		if (rc == 0) {
			printf(".");
		}

		if (fdset[1].revents & POLLPRI) {
			len = read(fdset[1].fd, buf, MAX_BUF);
			printf("\npoll() GPIO %d interrupt occurred\n", gpio);
		}

		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}

		fflush(stdout);
	}

	return 0;
}

#endif
