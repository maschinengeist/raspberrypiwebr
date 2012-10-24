
#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "Station.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/

static FILE *fpStation = 0;

/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/

/************************************************************************
*/
/*! \fn int Stations_Open(char* cFile)

*   \brief open an file with the stations return the number of entrys
*
*   \return int --< number of entries or -1 in case of an error
*
*************************************************************************/
int Station_Open(char* cFile)
{
	int iEntrys = 0;
	char cText[128];
	char* pResult;

	/* open file */
	if(fpStation == 0)
	{
		fpStation = fopen((const char*)cFile, "r+");
	}

	/* check file is open */
	if(fpStation < 0) return -1;

	/* if seek failed return with an error */
	if(fseek(fpStation, 0, SEEK_SET) < 0) return -1;

	do
	{
		pResult = fgets(cText, 128, fpStation);

		/* if first entry station ? */
		if(memcmp("STATION=", &cText[0], strlen("STATION=")) == 0)
		{
			pResult = fgets(cText, 128, fpStation);

			/* if second entry STREAM ? */
			if(memcmp("STREAM=", &cText[0], strlen("STREAM=")) == 0)
			{
				/* then we have a valid entrie */
				iEntrys++;
			}
			else
			{
				/* we have an error in this file */
				return -1;
			}
		}

	} while(pResult != 0);

	return iEntrys;
}

/************************************************************************
*/
/*! \fn int FileManager_Read(STATION_ENTRY* StationEntry, int iNumber)

*   \brief Read an entry selectet by number
*
*   \return int --> 1 entrie is filed, or -1 entry not found or an error occure
*
*************************************************************************/
int Station_Read(STATION_ENTRY* StationEntry, int iNumber)
{
	int iLoop, iLen;
	char* pResult;
	char cText[128];

	/* iNumber must be one */
	if(iNumber == 0) return -1;

	/* check file is open */
	if(fpStation < 0) return -1;

	/* if seek failed return with an error */
	if(fseek(fpStation, 0, SEEK_SET) < 0) return -1;

	/* read unused entries */
	for(iLoop = 0 ; iLoop < (iNumber - 1) ; iLoop++)
	{
		pResult = fgets(cText, 128, fpStation);
		pResult = fgets(cText, 128, fpStation);

		/* reach end of file ? */
		if(pResult == 0) return -1;
	}

	/* now read entries */
	pResult = fgets(cText, 128, fpStation);

	if(memcmp("STATION=", &cText[0], strlen("STATION=")) == 0)
	{
		iLen = strlen(cText) - strlen("STATION=");

		if(iLen < MAX_SIZE_STATION)
		{
			strcpy(StationEntry->cStation, &cText[strlen("STATION=")]);
			iLen = strlen(StationEntry->cStation);

			/* remove carry return and new line */
			if(StationEntry->cStation[iLen - 1] == 13) StationEntry->cStation[iLen - 1] = 0;
			if(StationEntry->cStation[iLen - 1] == 10) StationEntry->cStation[iLen - 1] = 0;
			if(StationEntry->cStation[iLen - 2] == 13) StationEntry->cStation[iLen - 2] = 0;
			if(StationEntry->cStation[iLen - 2] == 10) StationEntry->cStation[iLen - 2] = 0;
		}
		else
		{
			memcpy(StationEntry->cStation, &cText[strlen("STATION=")], MAX_SIZE_STATION);
			StationEntry->cStation[MAX_SIZE_STATION - 1] = 0;

			/* remove carry return and new line */
			if(StationEntry->cStation[MAX_SIZE_STATION - 2] == 13) StationEntry->cStation[MAX_SIZE_STATION - 2] = 0;
			if(StationEntry->cStation[MAX_SIZE_STATION - 2] == 10) StationEntry->cStation[MAX_SIZE_STATION - 2] = 0;
			if(StationEntry->cStation[MAX_SIZE_STATION - 3] == 13) StationEntry->cStation[MAX_SIZE_STATION - 3] = 0;
			if(StationEntry->cStation[MAX_SIZE_STATION - 3] == 10) StationEntry->cStation[MAX_SIZE_STATION - 3] = 0;
		}

		pResult = fgets(cText, 128, fpStation);

		/* if second entry STREAM ? */
		if(memcmp("STREAM=", &cText[0], strlen("STREAM=")) == 0)
		{
			iLen = strlen(cText);

			if(iLen < MAX_SIZE_STREAM_PATH)
			{
				strcpy(StationEntry->cStream, &cText[strlen("STREAM=")]);
			}
			else
			{
				memcpy(StationEntry->cStream, &cText[strlen("STREAM=")], MAX_SIZE_STREAM_PATH);
				StationEntry->cStream[MAX_SIZE_STREAM_PATH - 1] = 0;
			}
		}
		else
		{
			/* we have an error in this file */
			return -1;
		}
	}

	return 1;
}

/************************************************************************
*/
/*! \fn int FileManager_Write(STATION_ENTRY* StationEntry, int iNumber)

*   \brief Read an entry selectet by number
*
*   \return int --> 1 entrie is filed, or -1 entry not found or an error occure
*
*************************************************************************/
int Station_Write(STATION_ENTRY* StationEntry, int iNumber)
{
	return -1;
}

/************************************************************************
*/
/*! \fn int FileManager_Append(STATION_ENTRY* StationEntry, int iNumber)

*   \brief Append an return the position in the file
*
*   \return int --> nummber of entry in file, or -1 entry not found or an error occure
*
*************************************************************************/
int Station_Append(STATION_ENTRY StationEntry)
{
	return -1;
}


void Stations_Close(void)
{
	fclose(fpStation);
}




#ifdef __cplusplus
   }
#endif

