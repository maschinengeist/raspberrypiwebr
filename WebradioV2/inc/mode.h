#ifndef __MODE_H__
#define __MODE_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define SLEEP_TIME 10 * 60

#define MODE_TIME		200
#define MP3_PLAYER_VERSION "   06.02.2015   "

/* Menue Start */
enum
{
   MODE_NORMAL = 0,
   MODE_INIT,
   MODE_TIMEOUT
};

/* Menue Start */
enum
{
    MODE_POWERON = 0,
    MODE_PLAY,
    MODE_VOLUME,
    MODE_STATION,
    MODE_CHANGE_STATION,

    MODE_SETTINGS,
    MODE_SETTINGS_STARTVOLUME,
    MODE_SETTINGS_STATION_NAME,
    MODE_SETTINGS_STATION_ADDRESS,
    MODE_SETTINGS_NETWORK_RSSI,
    MODE_SETTINGS_NETWORK_KEY,
    MODE_SETTINGS_STATION_CLEAR,
    MODE_SETTINGS_NAME_EDIT,

    MODE_NEWS_SELECTION,
    MODE_NEWS_STATE,
    MODE_NEWS_STATION,
    MODE_NEWS_STARTTIME,
    MODE_NEWS_DURATION,
    MODE_NEWS_INTERVAL,

    MODE_ALARM_SELECTION,
    MODE_ALARM_TIME,

    MODE_STANDBY,
    MODE_SLEEP,
    MODE_POWEROFF
};


/**************************************************************************
*   P R O T O T Y P E
***************************************************************************/

INT_U8 mode_Init(INT_U16 u16Timeout);

void mode_TimeoutTrigger(void);

void mode_Change(INT_U8 u08Mode);

void mode_WriteState(INT_U8 u08State);

INT_U8 mode_ReadState(void);

INT_U8 mode_Read(void);

void mode_IncPosition(INT_U8* p08Position1, INT_U8* p08Position2, INT_U8 u08MaxRange, INT_U8 u08BorderToIncPosition2);

void mode_DecPosition(INT_U8* p08Position1, INT_U8* p08Position2, INT_U8 u08MinRange, INT_U8 u08BorderToDecPosition2);

void mode_Inc(INT_U8* p08Value, INT_U8 u08MaxValue);

void mode_Dec(INT_U8* p08Value, INT_U8 u08MinValue);

void mode(void);

#ifdef __cplusplus
	}
#endif

#endif




