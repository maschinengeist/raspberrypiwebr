/*! \file FileManager.c
*/

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

#include "FileManager.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/


/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/

/************************************************************************
*/
/*! \fn int FileManager_MaxEntries(char* cFile)

*   \brief open an file and return the number of entrys
*
*   \return int --< number of entries or -1 in case of an error
*
*************************************************************************/
int FileManager_MaxEntries(char* cFile)
{
    FILE *fpStation;
    int iCurrentNumber, iEntrys = 0;
    char cText[128];
    char* pResult;

	fpStation = fopen((const char*)cFile, "r+");

    /* if seek failed return with an error */
    if(fseek(fpStation, 0, SEEK_SET) < 0) return -1;

    do
    {
        pResult = fgets(cText, 128, fpStation);

        if(memcmp("ENTRY=", &cText[0], strlen("ENTRY=")) == 0)
        {
            sscanf(&cText[strlen("ENTRY=")], "%d", &iCurrentNumber);

            iEntrys++;
            if(!(iCurrentNumber == iEntrys))
            {
                fclose(fpStation);
                return -1;
            }

        }

        pResult = fgets(cText, 128, fpStation);
        pResult = fgets(cText, 128, fpStation);
        pResult = fgets(cText, 128, fpStation);


    } while(pResult != 0);

    fclose(fpStation);
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
int FileManager_Read(char* cFile, STATION_ENTRY* StationEntry, int iNumber)
{
    FILE *fpStation;
    int iCurrentNumber = 0;
    char* pResult;
    char cText[128];

    fpStation = fopen((const char*)cFile, "r+");

    /* if seek failed return with an error */
    if(fseek(fpStation, 0, SEEK_SET) < 0) return -1;

    do
    {
        pResult = fgets(cText, 128, fpStation);

        if(memcmp("ENTRY=", &cText[0], strlen("ENTRY=")) == 0)
        {
            sscanf(&cText[strlen("ENTRY=")], "%d", &iCurrentNumber);
        }

        if(iCurrentNumber == iNumber)
        {
            StationEntry->iNumber = iCurrentNumber;

            pResult = fgets(cText, 128, fpStation);
            if(memcmp("STATION=", &cText[0], strlen("ENTRY=")) == 0)
            {
                strcpy(StationEntry->cStation, &cText[strlen("STATION=")]);
            }

            pResult = fgets(cText, 128, fpStation);
            if(memcmp("GENRE=", &cText[0], strlen("GENRE=")) == 0)
            {
                strcpy(StationEntry->cGenre, &cText[strlen("GENRE=")]);
            }

            pResult = fgets(cText, 128, fpStation);
            if(memcmp("STREAM=", &cText[0], strlen("STREAM=")) == 0)
            {
                strcpy(StationEntry->cStream, &cText[strlen("STREAM=")]);
            }

            fclose(fpStation);
            return iCurrentNumber;
        }
        else
        {
            pResult = fgets(cText, 128, fpStation);
            pResult = fgets(cText, 128, fpStation);
            pResult = fgets(cText, 128, fpStation);
        }

    } while(pResult != 0);

    fclose(fpStation);
	return -1;
}

/************************************************************************
*/
/*! \fn int FileManager_Write(STATION_ENTRY* StationEntry, int iNumber)

*   \brief Read an entry selectet by number
*
*   \return int --> 1 entrie is filed, or -1 entry not found or an error occure
*
*************************************************************************/
int FileManager_Write(char* cFile, STATION_ENTRY* StationEntry, int iNumber)
{
    FILE *fpStation;

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
int FileManager_Append(char* cFile, STATION_ENTRY StationEntry)
{
    FILE *fpStation;

    int iMaxEntrys;

    char* pResult;
    char cText[128];

    iMaxEntrys = FileManager_MaxEntries(cFile);
    iMaxEntrys++;

    fpStation = fopen((const char*)cFile, "a");

    if(fpStation < 0) return -1;

    sprintf(cText, "ENTRY=%d\r", iMaxEntrys);
    fputs (cText,fpStation);
    sprintf(cText, "STATION=%s", StationEntry.cStation);
    fputs (cText,fpStation);
    sprintf(cText, "GENRE=%s", StationEntry.cGenre);
    fputs (cText,fpStation);
    sprintf(cText, "STREAM=%s", StationEntry.cStream);
    fputs (cText,fpStation);

    fclose(fpStation);
	return iMaxEntrys;
}





#ifdef __cplusplus
   }
#endif

