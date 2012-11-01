

#ifdef __cplusplus
	extern "C" {
#endif

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
#include "Directory.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define MAX_ENTRIES 128
#define MAX_STRING_LENGTH 64

/*************************************************************************
*   G L O B A L
*************************************************************************/



static char cFilterMp3[] = ".mp3";
static char cFilterM3U[] = ".m3u";
static char cFilterPls[] = ".pls";

static int iMaxEntries = 0;


static DIRECTORY_CONTENTS iDirEntries[MAX_ENTRIES];

typedef struct _DIRECTORY_FILTER
{
	char* cFilter;
	int iFilterType;
} DIRECTORY_FILTER;

/* init filters tabel*/
DIRECTORY_FILTER FilterList[] = {{cFilterMp3, DIR_TYPE_MP3}, {cFilterM3U, DIR_TYPE_MCU}, {cFilterPls, DIR_TYPE_PLS}};


/************************************************************************
*/
/*! \fn int Directory_Open(char *cPath)

*   \brief open an directory and read all entries specified by filters
*
*   \param cPath --> path to directory
*
*   \return int --> amount of entries or -1 open error
*
*************************************************************************/
int Directory_Open(char *cDir)
{
	int iLoop, iEntries, iFilteredEntries;
	struct dirent **namelist;
	INT_U8 u08FilterLoop;

	/* clear entrie counts */
	iMaxEntries = 0;

	chdir(cDir);

	getcwd(cDir, 255);

   iEntries = scandir(cDir, &namelist, 0, alphasort);

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
			if(iDirEntries[iLoop].cName > 0)
			{
				free(iDirEntries[iLoop].cName);
				iDirEntries[iLoop].cName = 0;
			}
		}

		/* set entries to zero */
		iFilteredEntries = 0;

		for(iLoop = 0 ; iLoop < iEntries ; iLoop++)
		{
			/* get memory space for name */
			iDirEntries[iFilteredEntries].cName = (char*) malloc(256);

			if(iDirEntries[iFilteredEntries].cName > 0)
			{
				if(!(namelist[iLoop]->d_name[0] == '.'))
				{
					strcpy(iDirEntries[iFilteredEntries].cName, namelist[iLoop]->d_name);

					if(namelist[iLoop]->d_type == DT_DIR)
					{
						iDirEntries[iFilteredEntries].iType = DIR_TYPE_FOLDER;
						iFilteredEntries++;
					}
					else
					{

						for(u08FilterLoop = 0 ; u08FilterLoop < 2; u08FilterLoop++)
						{
							/* her a error to fix */
							if(strstr(namelist[iLoop]->d_name, FilterList[u08FilterLoop].cFilter) > 0)
							{
								/* copy name */
								strcpy(iDirEntries[iFilteredEntries].cName, namelist[iLoop]->d_name);
								/* and cut extension */
								iDirEntries[iFilteredEntries].cName[strlen(namelist[iLoop]->d_name) - strlen(FilterList[u08FilterLoop].cFilter)] = 0;
								/* now set type of file */
								iDirEntries[iFilteredEntries].iType = FilterList[u08FilterLoop].iFilterType;
								/* increment the entries and quit */
								iFilteredEntries++;
								break;
							}
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
/*! \fn char* Directory_ReadEntrie(int iEntrie)

*   \brief read entrie
*
*   \param iEntrie --> number to read
*   \param pDirEntries --> pointer to entrie,
*
*   \return char* --> Pointer to name of entrie
*
*************************************************************************/
int Directory_ReadEntrie(int iEntrie, DIRECTORY_CONTENTS* pDirEntries)
{

	if((iEntrie > 0) && (iEntrie <= iMaxEntries))
	{
		memcpy(pDirEntries, &iDirEntries[iEntrie - 1], sizeof(DIRECTORY_CONTENTS));
		return 1;
	}

	return -1;
}

/************************************************************************
*/
/*! \fn int Directory_MaxEntries(void)

*   \brief return amount of entries
*
*   \return int --> amount of entries
*
*************************************************************************/
int Directory_MaxEntries(void)
{
	return iMaxEntries;
}

