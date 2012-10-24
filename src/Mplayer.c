/*! \file Mplayer.c
*/

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytypes.h"
#include "Mplayer.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/

#define MPLAYER_COMMAND_SIZE 256

/*************************************************************************
*   G L O B A L
*************************************************************************/

static FILE *fpMplayer;
static char cText[MPLAYER_COMMAND_SIZE];
static STREAM_ENTRY StreamEntry;
char ICY_Stream[] = "ICY Info: StreamTitle='The Fureys & Davey Arthur - When you were sweet sixteen (remastered)';StreamUrl='&artist=The%20Fureys%20%26%20Davey%20Arthur&title=When%20you%20were%20sweet%20sixteen%20%28remastered%29&album=Ultimate%20Irish%20Pub%20Songs%20%5BOriginal%20recording%20remastered%5D&duration=310727%';";


/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/

int Mplayer_WriteFifo(char* cCommand)
{
    FILE* fpFifo;

    /* file for sending to mplayer */
	fpFifo = fopen("/tmp/fifofile", "r+");

	/* file successfully opened? */
	if(fpFifo != NULL)
	{
        fputs(cCommand, fpFifo);
        fclose(fpFifo);

        return 1;
	}

	return -1;
}

/************************************************************************
*/
/*! \fn int Mplayer_Start(void)

*   \brief open mplayer in slave mode
*
*   \return int --> -1 open error, 1 mplayer is started
*
*************************************************************************/
int Mplayer_Start(void)
{
   int iFlag;

	/* create fifo in temp */
	fpMplayer = popen("mkfifo /tmp/fifofile", "r");

	/* pipe successfully opened? */
	if(fpMplayer != NULL)
	{
		fclose(fpMplayer);
	}
	else
	{
		printf("open error\r\n");
		return -1;
	}

	/* create a pipe for sending to mplayer */
	fpMplayer = popen("mplayer -slave -quiet -idle -input file=/tmp/fifofile\n", "r");

	/* make pipe non blocking */
	iFlag = fileno(fpMplayer);
	fcntl(iFlag, F_SETFL, O_NONBLOCK);

	/* wait for response */
	sleep(2);

	/* pipe successfully opened? */
	if(fpMplayer != NULL)
	{
		(void)fgets(cText, MPLAYER_COMMAND_SIZE, fpMplayer);

		/* if mplayer startet ? */
		if(0 == (memcmp(cText, "MPlayer", strlen("MPlayer"))))
		{
			return 1;
		}
		else
		{
			fclose(fpMplayer);
			printf("close mplayer\r\n");
			return -1;
		}
	}

	return -1;
}


/************************************************************************
*/
/*! \fn int Mplayer_GetStreamInfo(STREAM_ENTRY* pStreamEntry)

*   \brief get info of steam, station, titel, genre and so on
*
*   \return int --> -1 no info, 1 get stream info
*
*************************************************************************/
int Mplayer_GetStreamEntries(STREAM_ENTRY* pStreamEntry)
{
	char* pResult;
	char* pPosition, *pEntriePointer;
	unsigned char u08Loop;
	char cEscapeData[4], cSearchString[32];
	int iValue, iEntryCount = 0, iEntrySize = 0;

	pResult = fgets(cText, MPLAYER_COMMAND_SIZE, fpMplayer);
	if(pResult == 0) return -1;

	/* if mplayer startet ? */
	if(0 == (memcmp(cText, "ICY Info: StreamTitle='", strlen("ICY Info: StreamTitle='"))))
	{
		/* get max string length */
		//iTextLength = strlen(cText);

		/* set all entrys to zero */
		memset(&StreamEntry, 0x00, sizeof(StreamEntry));

		for(u08Loop = 0 ;u08Loop < 3 ; u08Loop++)
		{
			switch(u08Loop)
			{
				case 0:
				  	memset(cSearchString, 0x00, sizeof(cSearchString));
				  	memcpy(cSearchString ,"&artist=", strlen("&artist="));
				  	pEntriePointer = StreamEntry.cArtist;
				  	iEntrySize = sizeof(StreamEntry.cArtist);
				break;

				case 1:
				  	memset(cSearchString, 0x00, sizeof(cSearchString));
				  	memcpy(cSearchString ,"&title=", strlen("&titel="));
				  	pEntriePointer = StreamEntry.cTitle;
				  	iEntrySize = sizeof(StreamEntry.cTitle);
				break;

				case 2:
				  	memset(cSearchString, 0x00, sizeof(cSearchString));
				  	memcpy(cSearchString ,"&album=", strlen("&album="));
				  	pEntriePointer = StreamEntry.cAlbum;
				  	iEntrySize = sizeof(StreamEntry.cAlbum);
				break;

				default:
				break;
			}

			pPosition = strstr(cText, cSearchString);

			if(pPosition)
			{
				pPosition += strlen(cSearchString);
				iEntryCount = 0;

				do
				{
					if((iEntryCount + 1) < iEntrySize)
					{
						if(*pPosition == '%')
						{
							pPosition++;

							memcpy(cEscapeData, pPosition, 2);
							cEscapeData[3] = 0;

							sscanf(cEscapeData, "%x", &iValue);
							pPosition++;

							pEntriePointer[iEntryCount++] = iValue;
						}
						else if(*pPosition != '&')
						{
							pEntriePointer[iEntryCount++] = *pPosition;
						}
					}
				}
				while(*(pPosition++) != '&');
			}
		}

		/* copy stream */
		memcpy(pStreamEntry, &StreamEntry, sizeof(StreamEntry));

		return 1;
	}

	return -1;
}



/************************************************************************
*/
/*! \fn int Mplayer_GetStreamInfo(STREAM_ENTRY* pStreamEntry, int iMaxSize)

*   \brief get info of steam, station, titel, genre and so on
*
*   \return int --> -1 no info, 1 get stream info
*
*************************************************************************/
int Mplayer_GetStreamInfo(char* cStreamInfo, int iMaxSize)
{
	char* pResult;
	char cText[256];
	int iPosition;

	pResult = fgets(cText, 256, fpMplayer);
	if(pResult == 0) return -1;

	/* if mplayer startet ? */
	if(0 == (memcmp(cText, "ICY Info: StreamTitle='", strlen("ICY Info: StreamTitle='"))))
	{
		/* set all entrys to zero */
		memset(cStreamInfo, 0x00, iMaxSize);

		iPosition = strlen("ICY Info: StreamTitle='");

		while(cText[iPosition] != '\'')
		{
			*cStreamInfo = cText[iPosition++];
			cStreamInfo++;

			iMaxSize--;
			if(iMaxSize == 0) return -1;
		}

		return 1;
	}

	return -1;
}



/************************************************************************
*/
/*! \fn void Mplayer_ReadStreamInfo(STREAM_ENTRY* pStreamEntry)

*   \brief get info of steam, station, titel, genre and so on
*
*   \return int --> -1 no info, 1 get stream info
*
*************************************************************************/
void Mplayer_ReadStreamInfo(STREAM_ENTRY* pStreamEntry)
{
	memcpy(pStreamEntry, &StreamEntry, sizeof(StreamEntry));
}

/************************************************************************
*/
/*! \fn int Mplayer_PlayCommand(int iCommand)

*   \brief send play command like volume play paus to the mplayer
*
*   \return int --> -1 command not availabel, 1 execute command
*
*************************************************************************/
int Mplayer_PlayCommand(int iCommand)
{
	switch(iCommand)
	{
		case MP_VOLUME_PLUS:
			sprintf(cText, "volume 1\n");
		break;

		case MP_VOLUME_MINUS:
			sprintf(cText, "volume 0\n");
		break;

		case MP_PAUSE:
			sprintf(cText, "pause\n");
		break;

		case MP_STOP:
			sprintf(cText, "stop\n");
		break;

		case MP_MUTE_ON:
			sprintf(cText, "mute 1\n");
		break;

		case MP_MUTE_OFF:
			sprintf(cText, "mute 0\n");
		break;

		case MP_QUIT:
			sprintf(cText, "quit 1\n");
		break;

		default:
			return -1;
		break;
	}

	return Mplayer_WriteFifo(cText);
}

/************************************************************************
*/
/*! \fn int Mplayer_Volume(INT_U8 u08Volume)
*
*   \brief set the absolut volume
*
*   \param u08Volume --> Volume from 0 to 100
*
*   \return int --> -1 command not availabel, 1 execute command
*
*************************************************************************/
int Mplayer_Volume(INT_U8 u08Volume)
{
    char cText[64];

    sprintf(cText, "volume %d 1\n", (int)u08Volume);

	return Mplayer_WriteFifo(cText);
}
/************************************************************************
*/
/*! \fn int Mplayer_PlayFile(char* cFile)

*   \brief play an file or url
*
*   \return int --> -1 play error, 1 mplayer is playing
*
*************************************************************************/
int Mplayer_PlayFile(char* cFile)
{
	/* Write to Fifo */
	return Mplayer_WriteFifo(cFile);
}

/************************************************************************
*/
/*! \fn int Mplayer_Close(void)

*   \brief stop play and leave mplayer
*
*   \return int --> -1 play error, 1 mplayer is closed
*
*************************************************************************/
int Mplayer_Close(void)
{
   /* close pipe */
   fclose(fpMplayer);
   return 1;
}





#ifdef __cplusplus
   }
#endif

