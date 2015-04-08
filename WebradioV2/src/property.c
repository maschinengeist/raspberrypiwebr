/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytypes.h"

#include "property.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/


#define PROPERTY_MAX_STRING_ENTRYS 12
#define PROPERTY_STATIONS_STRINGLENGTH 128

/*************************************************************************
*   S T A T I C
*************************************************************************/

static INT_U8 u08Volume = 60, u08StartVolume = 60;
static INT_U8 u08NewsState = 0, u08NewsStarttime = 0, u08NewsDurationtime = 0, u08NewsInterval = 0;
static INT_U8 u08Station = 1;
static char cNameMemory[PROPERTY_MAX_STRING_ENTRYS][PROPERTY_STATIONS_STRINGLENGTH];
static char cEmpty[2] = {""};

/************************************************************************
*/
/*! \brief Read a type of the propertys
*
*  \param[in]   u08Type --> Type to read
*
*  \return INT_U8 --> Value of propertys
*
*************************************************************************/
INT_U8 property_Read(INT_U8 u08Type)
{
   switch(u08Type)
   {
      case PROPERTY_VOLUME:
      return u08Volume;

      case PROPERTY_STARTVOLUME:
      return u08StartVolume;

      case PROPERTY_STATION:
      return u08Station;

      case PROPERTY_NEWS_STATE:
      return u08NewsState;

      case PROPERTY_NEWS_STARTTIME:
      return u08NewsStarttime;

      case PROPERTY_NEWS_DURATIONTIME:
      return u08NewsDurationtime;

      case PROPERTY_NEWS_INTERVAL:
      return u08NewsInterval;

      default:
      return 0;
   }
}

/************************************************************************
*/
/*! \brief Write a type of the propertys
*
*  \param[in]   u08Type --> Setting to write
*  \param[in]   u08Value --> Setting value
*
*************************************************************************/
void property_Write(INT_U8 u08Type, INT_U8 u08Value)
{
   switch(u08Type)
   {
      case PROPERTY_VOLUME:
         u08Volume = u08Value;
      break;

      case PROPERTY_STARTVOLUME:
         u08StartVolume = u08Value;
      break;

      case PROPERTY_STATION:
         u08Station = u08Value;
      break;

      case PROPERTY_NEWS_STATE:
         u08NewsState = u08Value;
      break;

      case PROPERTY_NEWS_STARTTIME:
         u08NewsStarttime = u08Value;
      break;

      case PROPERTY_NEWS_DURATIONTIME:
         u08NewsDurationtime = u08Value;
      break;

      case PROPERTY_NEWS_INTERVAL:
         u08NewsInterval = u08Value;
      break;

      default:
      break;
   }
}

/************************************************************************
*/
/*! \brief Write a sting into memory
*
*  \param[in]   u08Type --> Type of name
*  \param[in]   cName --> Sting to write
*
*************************************************************************/
void property_WriteString(INT_U8 u08Type, char* cName)
{
	switch(u08Type)
	{
		case PROPERTY_TEMPORARYNAME: 	u08Type = 0;	break;
		case PROPERTY_STATION: 			u08Type = 1;	break;
		case PROPERTY_STATION_MEMO1:  u08Type = 2;	break;
		case PROPERTY_STATION_MEMO2: 	u08Type = 3;	break;
		case PROPERTY_STATION_MEMO3: 	u08Type = 4;	break;
		case PROPERTY_STATION_MEMO4: 	u08Type = 5;	break;
		case PROPERTY_WORKING_PATH: 	u08Type = 6;	break;
		case PROPERTY_NEWS_STATION: 	u08Type = 7;	break;
		default: break;
	}

	if(u08Type < PROPERTY_MAX_STRING_ENTRYS)
	{
		if(strlen(cName) < PROPERTY_STATIONS_STRINGLENGTH)
		{
			strcpy(cNameMemory[u08Type], cName);
		}
		else
		{
			printf("string to long\n");
		}
	}
}

/************************************************************************
*/
/*! \brief Read a sting into memory
*
*  \param[in]   u08Type --> Type of name
*  \return      cName --> Sting to read or 0 for no string
*
*************************************************************************/
char* property_ReadString(INT_U8 u08Type)
{
	switch(u08Type)
	{
		case PROPERTY_TEMPORARYNAME: 	u08Type = 0;	break;
		case PROPERTY_STATION: 			u08Type = 1;	break;
		case PROPERTY_STATION_MEMO1:  u08Type = 2;	break;
		case PROPERTY_STATION_MEMO2: 	u08Type = 3;	break;
		case PROPERTY_STATION_MEMO3: 	u08Type = 4;	break;
		case PROPERTY_STATION_MEMO4: 	u08Type = 5;	break;
		case PROPERTY_WORKING_PATH: 	u08Type = 6;	break;
		case PROPERTY_NEWS_STATION: 	u08Type = 7;	break;
		default: break;
	}

	if(u08Type < PROPERTY_MAX_STRING_ENTRYS)
	{
		return cNameMemory[u08Type];
	}
	else
	{
		return cEmpty;
	}
}

/************************************************************************
*/
/*! \brief Increment the property until to max
*
*  \param[in]   u08Type --> Type to Increment
*  \param[in]   u08MaxValue --> Until this max value
*
*  \return INT_U8 -> New incremented value
*
*************************************************************************/
INT_U8 property_Increment(INT_U8 u08Type, INT_U8 u08MaxValue)
{
   INT_U8* pValue = 0;

   switch(u08Type)
   {
      case PROPERTY_VOLUME:
         pValue = &u08Volume;
      break;

      case PROPERTY_STARTVOLUME:
         pValue = &u08StartVolume;
      break;

      case PROPERTY_STATION:
         pValue = &u08Station;
      break;

      case PROPERTY_NEWS_STARTTIME:
         pValue = &u08NewsStarttime;
      break;

      case PROPERTY_NEWS_DURATIONTIME:
         pValue = &u08NewsDurationtime;
      break;

      case PROPERTY_NEWS_INTERVAL:
         pValue = &u08NewsInterval;
      break;

      default:
      return 0;
   }

   if(pValue > 0)
   {
      if(pValue[0] < u08MaxValue) pValue[0]++;
   }

   return pValue[0];
}

/************************************************************************
*/
/*! \brief Decrement the property until to min
*
*  \param[in]   u08Type --> Type to Increment
*  \param[in]   u08MinValue --> This is the min value
*
*  \return INT_U8 -> New incremented value
*
*************************************************************************/
INT_U8 property_Decrement(INT_U8 u08Type, INT_U8 u08MinValue)
{
   INT_U8* pValue = 0;

   switch(u08Type)
   {
      case PROPERTY_VOLUME:
         pValue = &u08Volume;
      break;

      case PROPERTY_STARTVOLUME:
         pValue = &u08StartVolume;
      break;

      case PROPERTY_STATION:
         pValue = &u08Station;
      break;

      case PROPERTY_NEWS_STARTTIME:
         pValue = &u08NewsStarttime;
      break;

      case PROPERTY_NEWS_DURATIONTIME:
         pValue = &u08NewsDurationtime;
      break;

      case PROPERTY_NEWS_INTERVAL:
         pValue = &u08NewsInterval;
      break;

      default:
      return 0;
   }

   if(pValue > 0)
   {
      if(pValue[0] > u08MinValue) pValue[0]--;
   }

   return pValue[0];
}


/************************************************************************
*/
/*! \brief read the webrado settings from a config file
*
*   \param cPath --> Path to config file
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int property_Load(char* cPath)
{
	int iSize;
	static FILE *fpPropertys;
	char cFullPath[PROPERTY_STATIONS_STRINGLENGTH];
	char* cString;

	iSize = strlen(cPath) + strlen("/Settings");

	if(iSize < PROPERTY_STATIONS_STRINGLENGTH)
	{
		sprintf(cFullPath,"%s/Settings", cPath);

		/* open file */
		fpPropertys = fopen(cFullPath, "r");

		/* if open failed create default file */
		if(fpPropertys > 0)
		{
			/* if seek failed return with an error */
			if(fseek(fpPropertys, 0, SEEK_SET) < 0) return -1;

			cString = fgets(cFullPath, strlen("start volume= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			property_Write(PROPERTY_STARTVOLUME, (INT_U8)atol(cFullPath));

			/* news propertys */
			cString = fgets(cFullPath, strlen("news state= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			property_Write(PROPERTY_NEWS_STATE, (INT_U8)atol(cFullPath));

			cString = fgets(cFullPath, strlen("news starttime= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			property_Write(PROPERTY_NEWS_STARTTIME, (INT_U8)atol(cFullPath));

			cString = fgets(cFullPath, strlen("news duration= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			property_Write(PROPERTY_NEWS_DURATIONTIME, (INT_U8)atol(cFullPath));

			cString = fgets(cFullPath, strlen("news interval= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			property_Write(PROPERTY_NEWS_INTERVAL, (INT_U8)atol(cFullPath));

			/* string propertys */
			cString = fgets(cFullPath, strlen("station= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_STATION, cFullPath);

			cString = fgets(cFullPath, strlen("station memo1= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_STATION_MEMO1, cFullPath);

			cString = fgets(cFullPath, strlen("station memo2= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_STATION_MEMO2, cFullPath);

			cString = fgets(cFullPath, strlen("station memo3= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_STATION_MEMO3, cFullPath);

			cString = fgets(cFullPath, strlen("station memo4= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_STATION_MEMO4, cFullPath);

			cString = fgets(cFullPath, strlen("news station= "), fpPropertys);
			if(cString == NULL) return -1;
			cString = fgets(cFullPath, PROPERTY_STATIONS_STRINGLENGTH, fpPropertys);
			if(cString == NULL) return -1;
			cFullPath[strlen(cFullPath) - 1] = 0;
			property_WriteString(PROPERTY_NEWS_STATION, cFullPath);
			fclose(fpPropertys);

			return 1;
		}
		else
		{
			/* no file available, create new file with default propertys */
			property_Write(PROPERTY_STARTVOLUME, 60);
			property_Write(PROPERTY_NEWS_STATE, 0);
			property_Write(PROPERTY_NEWS_STARTTIME, 0);
			property_Write(PROPERTY_NEWS_DURATIONTIME, 5);
			property_Write(PROPERTY_NEWS_INTERVAL, 0);
			property_WriteString(PROPERTY_STATION, "Live Irland.m3u");
			property_WriteString(PROPERTY_STATION_MEMO1, "");
			property_WriteString(PROPERTY_STATION_MEMO2, "");
			property_WriteString(PROPERTY_STATION_MEMO3, "");
			property_WriteString(PROPERTY_STATION_MEMO4, "");
			property_WriteString(PROPERTY_NEWS_STATION, "");

			/* save the default settings */
			return property_Save(cPath);
		}
	}
	return -1;
}


/************************************************************************
*/
/*! \brief write the settings to a config file
*
*   \param cPath --> Path to config file
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int property_Save(char* cPath)
{
	int iSize;
	static FILE *fpPropertys;
	char cFullPath[PROPERTY_STATIONS_STRINGLENGTH];

	iSize = strlen(cPath) + strlen("/Settings");

	if(iSize < PROPERTY_STATIONS_STRINGLENGTH)
	{
		sprintf(cFullPath,"%s/Settings", cPath);

		fpPropertys = fopen(cFullPath, "w");

		/* open file */
		if(fpPropertys > 0)
		{
			/* if seek failed return with an error */
			if(fseek(fpPropertys, 0, SEEK_SET) < 0) return -1;

			fprintf(fpPropertys, "start volume=%d\n", (int)property_Read(PROPERTY_STARTVOLUME));
			fprintf(fpPropertys, "news state=%d\n", (int)property_Read(PROPERTY_NEWS_STATE));
			fprintf(fpPropertys, "news starttime=%d\n", (int)property_Read(PROPERTY_NEWS_STARTTIME));
			fprintf(fpPropertys, "news duration=%d\n", (int)property_Read(PROPERTY_NEWS_DURATIONTIME));
			fprintf(fpPropertys, "news interval=%d\n", (int)property_Read(PROPERTY_NEWS_INTERVAL));
			fprintf(fpPropertys, "station=%s\n", property_ReadString(PROPERTY_STATION));
			fprintf(fpPropertys, "station memo1=%s\n", property_ReadString(PROPERTY_STATION_MEMO1));
			fprintf(fpPropertys, "station memo2=%s\n", property_ReadString(PROPERTY_STATION_MEMO2));
			fprintf(fpPropertys, "station memo3=%s\n", property_ReadString(PROPERTY_STATION_MEMO3));
			fprintf(fpPropertys, "station memo4=%s\n", property_ReadString(PROPERTY_STATION_MEMO4));
			fprintf(fpPropertys, "news station=%s\n", property_ReadString(PROPERTY_NEWS_STATION));
			fclose(fpPropertys);

			return 1;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

