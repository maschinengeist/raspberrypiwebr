#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "mytypes.h"
#include "debug.h"

#include "Init.h"
#include "Menue.h"
#include "mode.h"

#include "Dogm204.h"
#include "Dogm204_UserInterface.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/


/****************************************************************
 * Main
 ****************************************************************/

int main(void)
{
	Init();

	do
	{
		mode();

		usleep(50000);
	}
	while(1);

   return 0;
}

/* debug interface */
void DebugLog(CHAR *pText)
{
	static int iFirstDebug = 1;
	static FILE *fp;

	if(iFirstDebug)
	{
		iFirstDebug = 0;
		/* open file */
		fp = fopen("/tmp/WebRadioLog", "w");
		/* got to first char */
		fseek(fp, 0, SEEK_SET);
		/* close file */
		fclose(fp);
	}

	/* open file */
	fp = fopen("/tmp/WebRadioLog", "a");
	/* if open failed create default file */
	if(fp > 0)
	{
		/* print message */
		fprintf(fp, "%s", pText);
		/* close file */
		fclose(fp);
	}
}

void DebugPrintf(CHAR *pText)
{
	printf("%s", pText);
}

