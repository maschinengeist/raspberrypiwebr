#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

enum
{
    PROPERTY_VOLUME = 0,
    PROPERTY_STARTVOLUME,
    PROPERTY_NEWS_STATE,
    PROPERTY_NEWS_START,
    PROPERTY_NEWS_STARTTIME,
    PROPERTY_NEWS_DURATIONTIME,
    PROPERTY_NEWS_INTERVAL,
    PROPERTY_ALARM_STATE,
    PROPERTY_ALARM_MINUTES,
    PROPERTY_ALARM_HOUR,
    PROPERTY_TEMPORARYNAME,
    PROPERTY_STATION,
    PROPERTY_STATION_MEMO1,
	 PROPERTY_STATION_MEMO2,
    PROPERTY_STATION_MEMO3,
	 PROPERTY_STATION_MEMO4,
    PROPERTY_WORKING_PATH,
    PROPERTY_NEWS_STATION
};

/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

INT_U8 property_Read(INT_U8 u08Type);

void property_Write(INT_U8 u08Type, INT_U8 u08Value);

void property_WriteString(INT_U8 u08Type, char* cName);

char* property_ReadString(INT_U8 u08Type);

INT_U8 property_Increment(INT_U8 u08Type, INT_U8 u08MaxValue);

INT_U8 property_Decrement(INT_U8 u08Type, INT_U8 u08MinValue);

int property_Load(char* cPath);

int property_Save(char* cPath);

#ifdef __cplusplus
        }
#endif

#endif


