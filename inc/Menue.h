/*! \file Menue.h
*/


#if !defined(__MENUE_H__)
#define __MENUE_H__

#ifdef __cplusplus
	extern "C" {
#endif


/**************************************************************************************
*   D E F I N E
***************************************************************************************/

enum MENUE
{
	MENUE_START = 0,
	MENUE_DIR,
	MENUE_PLAY,
	MENUE_CHANGE_STATION,
   MENUE_SETUP,
	MENUE_STATION,
	MENUE_NET_SETUP,
	MENUE_NET_SETUP_IP,
	MENUE_QUIT
} ;

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define STREAM_ENTRY_MAX_SIZE  64

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

typedef struct _WEBRADIO
{

   INT_U16  u16Position;
   /* information of network settings */
   NETWORK_SETTINGS NetworkSettings;
   /* information from stream */
   CHAR cStreamInfo[128];
   /* station name */
   CHAR cStation[128];
   /* full path of programm dir */
   CHAR cFullPath[256];
   /* Menue Backlight Timer */
   INT_U16  u16BacklightTimeout;
   /* Menue Timer */
   INT_U16  u16Timer;
   /* Current Menue Timeout */
   INT_U16   u16Timeout;
   /* Webradio setting */
   INT_U8   u08MenueState;
   /* Signal a new menue is select */
   INT_U8   u08MenueInit;
   /* Network settings */
   INT_U8   u08Network;
   /* Current Channel */
   INT_U8   u08Channel;
   /* Stor here the current Channel */
   INT_U8   u08OldChannel;
   /* New Station is select */
   INT_U8   u08NewStation;
   /* information of current selectet station position */
   INT_U8   u08MenueStationPosition;
   /* Maximum available Channels */
   INT_U8   u08MaxChannel;
   /* Current Volume (0-100) */
   INT_U8   u08Volume;
   /* Byte for toggeling */
   INT_U8   u08Toggle;
   /* Webradio setting */
   INT_U8   u08Run;

} WEBRADIO;


/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

void Menue_Select(WEBRADIO *pWebRadio);

void Menue_Play(WEBRADIO *pWebRadio);

void Menue_Station(WEBRADIO *pWebRadio);

void Menue_Settings(WEBRADIO *pWebRadio);

void Menue_Select(WEBRADIO *pWebRadio);


#ifdef __cplusplus
        }
#endif

#endif
