
#if !defined(__WEBRADIO_H__)
#define __WEBRADIO_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define STREAM_INFO_LENGTH 128
#define NEWS_STATION_OFF 0xff

enum WEBRADIO
{
	VOLUME = 0,
	MAX_VOLUME,
   STATION,
   MAX_STATION,
   NEWS_STATION,
   NEWS_SAVE_STATION,
	START_TIME,
	INTERVAL_TIME,
	DURATION_TIME
};

/**************************************************************************************
*   D E C L A R A T I O N
***************************************************************************************/

void Init(void);

INT_U8 Webradio_Plus(INT_U8 u08Type);

INT_U8 Webradio_Minus(INT_U8 u08Type);

void Webradio_Set(INT_U8 u08Type, INT_U8 u08Value);

INT_U8 Webradio_Get(INT_U8 u08Type);

void Webradio_SaveStation(void);

INT_U8 Webradio_LoadStation(void);

int Webradio_WriteSettings(char* cPath);

int Webradio_ReadSettings(char* cPath);

char* Webradio_GetPath(void);

char* Webradio_GetStation(INT_U8 u08Position);

char* Webradio_GetStreamInfo(INT_U8 u08Position);

#ifdef __cplusplus
   }
#endif

#endif


