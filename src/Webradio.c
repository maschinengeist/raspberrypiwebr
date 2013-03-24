
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "mytypes.h"
#include "debug.h"

#include "Webradio.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define MAX_VOLUME 30

/*************************************************************************
*   G L O B A L
*************************************************************************/

/* stream info like titel and artist */
static CHAR cStreamInfo[STREAM_INFO_LENGTH];
/* station name */
static CHAR cStation[128];
/* full path of programm dir */
static CHAR cFullPath[256];
/* Current Station */
static INT_U8   u08Station;
/* Stor here the current Station */
static INT_U8   u08OldStation;
/* The news station when switch to news */
static INT_U8   u08NewsStation;
/* Storage when switch to news */
static INT_U8   u08NewsSaveStation;
/* The start time to switch from current station to the news */
static INT_U8   u08NewsStartTime = 0;
/* The interval time for example every 30 minutes */
static INT_U8   u08NewsIntervalTime = 10;
/* The news time duration for example 5 minutes*/
static INT_U8   u08NewsDurationTime = 1;
/* Maximum available Stationss */
static INT_U8   u08MaxStations;
/* Current Volume (0-100) */
static INT_U8   u08Volume;

/*************************************************************************
*   F U N C T I O N S
*************************************************************************/

/************************************************************************
*/
/*! \fn INT_U8 Webradio_Plus(INT_U8 u08Type)
*
*   \brief increase
*
*   \return current
*
*************************************************************************/
INT_U8 Webradio_Plus(INT_U8 u08Type)
{
	INT_U8 u08Value = 0;

	switch(u08Type)
	{
		case VOLUME:
		{
			if(u08Volume < MAX_VOLUME) u08Volume++;
			u08Value = u08Volume;
		}
		break;

		case STATION:
		{

		}
		break;

		case START_TIME:
		{
			if((u08NewsStartTime < 59) || (u08NewsStartTime == NEWS_STATION_OFF))
			{
				u08NewsStartTime += 1;
			}

			u08Value = u08NewsStartTime;
		}
		break;

		case INTERVAL_TIME:
		{
			if(u08NewsIntervalTime < 30)
			{
				u08NewsIntervalTime += 5;
			}

			u08Value = u08NewsIntervalTime;
		}
		break;

		case DURATION_TIME:
		{
			if(u08NewsDurationTime < u08NewsIntervalTime)
			{
				u08NewsDurationTime += 1;
			}

			u08Value = u08NewsDurationTime;
		}
		break;

		default:
		break;
	}

	return u08Value;
}

/************************************************************************
*/
/*! \fn INT_U8 Webradio_Minus(INT_U8 u08Type)
*
*   \brief decrease
*
*   \return current
*
*************************************************************************/
INT_U8 Webradio_Minus(INT_U8 u08Type)
{
	INT_U8 u08Value = 0;

	switch(u08Type)
	{
		case VOLUME:
		{
			if(u08Volume > 0) u08Volume--;
			u08Value = u08Volume;
		}
		break;

		case STATION:
		{

		}
		break;

		case START_TIME:
		{
			if((u08NewsStartTime >= 0) && (!(u08NewsStartTime == NEWS_STATION_OFF)))
			{
				u08NewsStartTime -= 1;
			}

			u08Value = u08NewsStartTime;
		}
		break;

		case INTERVAL_TIME:
		{
			if(u08NewsIntervalTime > 10)
			{
				u08NewsIntervalTime -= 5;
			}

			u08Value = u08NewsIntervalTime;
		}
		break;

		case DURATION_TIME:
		{
			if(u08NewsDurationTime > 1)
			{
				u08NewsDurationTime -= 1;
			}

			u08Value = u08NewsDurationTime;
		}
		break;

		default:
		break;
	}

	return u08Value;
}

/**
*  \brief
*
*/
void Webradio_Set(INT_U8 u08Type, INT_U8 u08Value)
{
	switch(u08Type)
	{
		case VOLUME:
		{
			u08Volume = u08Value;
		}
		break;

		case STATION:
		{
			u08Station = u08Value;
		}
		break;

		case NEWS_SAVE_STATION:
		{
			u08NewsSaveStation = u08Value;
		}
		break;

		case MAX_STATION:
		{
			u08MaxStations = u08Value;
		}
		break;

		case NEWS_STATION:
		{
			u08NewsStation = u08Value;
		}
		break;

		default:
		break;
	}
}

/**
*  \brief
*
*/
INT_U8 Webradio_Get(INT_U8 u08Type)
{
	INT_U8 u08Value = 0;

	switch(u08Type)
	{
		case VOLUME:
		{
			u08Value = u08Volume;
		}
		break;

		case STATION:
		{
			u08Value = u08Station;
		}
		break;

		case MAX_STATION:
		{
			u08Value = u08MaxStations;
		}
		break;

		case NEWS_STATION:
		{
			u08Value = u08NewsStation;
		}
		break;

		case NEWS_SAVE_STATION:
		{
			u08Value = u08NewsSaveStation;
		}
		break;

		case START_TIME:
		{
			u08Value = u08NewsStartTime;
		}
		break;

		case INTERVAL_TIME:
		{
			u08Value = u08NewsIntervalTime;
		}
		break;

		case DURATION_TIME:
		{
			u08Value = u08NewsDurationTime;
		}
		break;

		default:
		break;
	}

	return u08Value;
}

/************************************************************************
*/
/*! \fn void Webradio_SaveStation(void)
*
*   \brief temporary store station
*
*************************************************************************/
void Webradio_SaveStation(void)
{
	u08OldStation = u08Station;
}

/************************************************************************
*/
/*! \fn INT_U8 Webradio_LoadStation(void)
*
*   \brief load stored station back
*
*************************************************************************/
INT_U8 Webradio_LoadStation(void)
{
	u08Station = u08OldStation;
	return u08Station;
}


/************************************************************************
*/
/*! \fn int Webradio_ReadSettings(char* cPath)
*
*   \brief read the webrado setings from a config file
*
*   \param cPath --> Path to config file
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int Webradio_ReadSettings(char* cPath)
{
	#define SETTINGS_INPUT_SIZE 128

	int iValue, iSize;
	static FILE *fpSettings;

	/* open file */
	fpSettings = fopen(cPath, "r");

	/* if open failed create default file */
	if(fpSettings > 0)
	{
		/* if seek failed return with an error */
		if(fseek(fpSettings, 0, SEEK_SET) < 0) return -1;

		iSize = fscanf(fpSettings, "volume=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(VOLUME, (INT_U8)iValue);

		iSize = fscanf(fpSettings, "station=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(STATION, (INT_U8)iValue);

		iSize = fscanf(fpSettings, "news station=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(NEWS_STATION, (INT_U8)iValue);

		iSize = fscanf(fpSettings, "news station start time=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(START_TIME, (INT_U8)iValue);

		iSize = fscanf(fpSettings, "news station interval time=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(INTERVAL_TIME, (INT_U8)iValue);

		iSize = fscanf(fpSettings, "news station duration time=%d\n", &iValue);
		if(iSize == 0) return -1;
		Webradio_Set(DURATION_TIME, (INT_U8)iValue);

		fclose(fpSettings);

		return 1;
	}

	return -1;
}


/************************************************************************
*/
/*! \fn Webradio_WriteSettings(char* cPath)
*
*   \brief write the settings to a config file
*
*   \param cPath --> Path to config file
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int Webradio_WriteSettings(char* cPath)
{
	static FILE *fpSettings;

	fpSettings = fopen(cPath, "w");

	/* open file */
	if(fpSettings > 0)
	{
		/* if seek failed return with an error */
		if(fseek(fpSettings, 0, SEEK_SET) < 0) return -1;

		fprintf(fpSettings, "volume=%d\n", (int)Webradio_Get(VOLUME));

		fprintf(fpSettings, "station=%d\n", (int)Webradio_Get(STATION));

		fprintf(fpSettings, "news station=%d\n", (int)Webradio_Get(NEWS_STATION));

		fprintf(fpSettings, "news station start time=%d\n", (int)Webradio_Get(START_TIME));

		fprintf(fpSettings, "news station interval time=%d\n", (int)Webradio_Get(INTERVAL_TIME));

		fprintf(fpSettings, "news station duration time=%d\n", (int)Webradio_Get(DURATION_TIME));

		fclose(fpSettings);
	}
	else
	{
		return -1;
	}

	return 1;
}

/************************************************************************
*/
/*! \fn char* Webradio_GetPath(void);
*
*   \brief get pointer to path string
*
*   \return pointer to path string
*
*************************************************************************/
char* Webradio_GetPath(void)
{
	return &cFullPath[0];
}

/************************************************************************
*/
/*! \fn char* Webradio_GetStation(INT_U8 u08Position)
*
*   \brief get pointer to path string
*
*   \return pointer to path string
*
*************************************************************************/
char* Webradio_GetStation(INT_U8 u08Position)
{
	return &cStation[u08Position];
}

/************************************************************************
*/
/*! \fn char* Webradio_GetStreamInfo(INT_U8 u08Position)
*
*   \brief get pointer to stream info string
*
*   \return pointer to stream info string
*
*************************************************************************/
char* Webradio_GetStreamInfo(INT_U8 u08Position)
{
	return &cStreamInfo[u08Position];
}



