

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytypes.h"
#include "station.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define MAX_ENTRIES 64
#define MAX_STRING_LENGTH 64

/*************************************************************************
*   G L O B A L
*************************************************************************/

static int iMaxEntries = 0;

static char* cStations[MAX_ENTRIES];
static char cEmpty[2] = {""};

/************************************************************************
*/
/*! \brief open an directory and load all m3u entries
*
*   \param cPath --> path to directory
*
*   \return int --> amount of entries or -1 open error
*
*************************************************************************/
int station_Load(char *cPath)
{
	int iLoop, iEntries, iFilteredEntries;
	struct dirent **namelist;

	/* clear entrie counts */
	iMaxEntries = 0;

	if(chdir(cPath) < 0)
	{
		printf("chdir error\n");
	}

	getcwd(cPath, 255);

   iEntries = scandir(cPath, &namelist, 0, alphasort);

	if (iEntries < 0)
	{
		perror("scandir");
		return -1;
	}
   else
	{
		/* free used entries */
		for(iLoop = 0 ; iLoop < MAX_ENTRIES ; iLoop++)
		{
			if(cStations[iLoop] > 0)
			{
				free(cStations[iLoop]);
				cStations[iLoop] = 0;
			}
		}

		/* set entries to zero */
		iFilteredEntries = 0;

		for(iLoop = 0 ; iLoop < iEntries ; iLoop++)
		{
			/* get memory space for name */
			cStations[iFilteredEntries] = (char*) malloc(MAX_STRING_LENGTH);

			if(cStations[iFilteredEntries] > 0)
			{
				/* load onle m3u files no folder */
				if((!(namelist[iLoop]->d_name[0] == '.')) && (!(namelist[iLoop]->d_type == DT_DIR)))
				{
					/* her a error to fix */
					if(strstr(namelist[iLoop]->d_name, ".m3u") > 0)
					{
						if(strlen(namelist[iLoop]->d_name) < MAX_STRING_LENGTH)
						{
							/* copy name */
							strcpy(cStations[iFilteredEntries], namelist[iLoop]->d_name);
							iFilteredEntries++;
						}
					}
				}
			}
			else
			{
				printf("Malloc error\r\n");
			}

		   free(namelist[iLoop]);
		}

		free(namelist);
   }

   iMaxEntries = iFilteredEntries;

	return iFilteredEntries;
}

/************************************************************************
*/
/*! \brief read station entrie
*
*   \param iEntrie --> number to read
*
*   \return char* --> Pointer to name of entrie or NULL for no entrie
*
*************************************************************************/
char* station_Read(int iEntrie)
{
	if((iEntrie > 0) && (iEntrie <= iMaxEntries))
	{
		return cStations[iEntrie - 1];
	}

	return &cEmpty[0];
}

/************************************************************************
*/
/*! \brief clear a station
*
*   \param cPath   --> path to station
*   \param iEntrie --> number of station to clear
*
*   \return int --> 1 = station is cleard, -1 error during clear
*
*************************************************************************/
int station_Clear(char *cPath, int iEntrie)
{
   FILE* fp;
   char cCommand[128];
   char cEscapeStation[128];
	char* pText = cCommand;

   if((cCommand > 0) && (cEscapeStation > 0))
   {
		int iLoop = 0;

		/* copy station name */
		strcpy(cCommand, station_Read(iEntrie));

		memset(cEscapeStation, 0x00 , sizeof(cEscapeStation));

		/* escape spaces in string */
		while(*pText)
		{
			/* escape spaces in string */
			if(*pText == ' ')
			{
				cEscapeStation[iLoop++] = 92;
				cEscapeStation[iLoop++] = 32;
			}
			else
			{
				cEscapeStation[iLoop++] = *pText;
			}

			pText++;
		}

		cEscapeStation[iLoop] = 0;

   	sprintf(cCommand, "rm %s/%s\n", cPath, cEscapeStation);
		/* create a pipe for rm station */
		fp = popen(cCommand, "r");

		/* pipe successfully opened? */
		if(fp > 0)
		{
			/* sleep until clear */
			sleep(1);
			/* reload list */
			if(station_Load(cPath) > 0) return 1;
		}

		return -1;
   }
}

/************************************************************************
*/
/*! \brief Write a new station
*
*   \param cPath --> path to directory
*
*   \return int --> 1 all is ok or -1 write error
*
*************************************************************************/
int station_Write(char *cPath, char *cStationName, char *cStationAddress)
{
	static FILE *fpSettings;
	char* cFullPath;

	cFullPath = malloc(strlen(cPath) + strlen(cStationName) + 16);
	sprintf(cFullPath, "%s/%s.m3u", cPath, cStationName);

	fpSettings = fopen(cFullPath, "w+");

	/* open file */
	if(fpSettings > 0)
	{
		/* if seek failed return with an error */
		if(fseek(fpSettings, 0, SEEK_SET) < 0) return -1;
		/* write station address */
		fprintf(fpSettings, "%s\n", cStationAddress);
		/* Close file */
		fclose(fpSettings);
		/* reload stations */
		(void)station_Load(cFullPath);
	}
	else
	{
		free(cFullPath);
		return -1;
	}

	free(cFullPath);
	return 1;
}

/************************************************************************
*/
/*! \brief return amount of entries
*
*   \return int --> amount of entries
*
*************************************************************************/
int station_MaxEntries(void)
{
	return iMaxEntries;
}

