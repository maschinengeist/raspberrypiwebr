/*! \file Mplayer.h
*/

#if !defined(__MPLAYER_H__)
#define __MPLAYER_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

enum MP_COMMANDS
{
	MP_VOLUME_PLUS = 1,
	MP_VOLUME_MINUS,
	MP_PLAY,
	MP_PAUSE,
	MP_STOP,
	MP_MUTE_ON,
	MP_MUTE_OFF,
	MP_QUIT
} ;

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define STREAM_ENTRY_MAX_SIZE  64

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

typedef struct _STREAM_ENTRY
{
	char cStation[STREAM_ENTRY_MAX_SIZE];
	char cArtist[STREAM_ENTRY_MAX_SIZE];
	char cTitle[STREAM_ENTRY_MAX_SIZE];
	char cAlbum[STREAM_ENTRY_MAX_SIZE];

} STREAM_ENTRY;





/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int Mplayer_Start(void);

int Mplayer_PlayFile(char* cFile);

int Mplayer_Volume(INT_U8 u08Volume);

int Mplayer_PlayCommand(int iCommand);

int Mplayer_GetStreamInfo(char* cStreamInfo, int iMaxSize);

void Mplayer_ReadStreamInfo(STREAM_ENTRY* pStreamEntry);

int Mplayer_Close(void);

#ifdef __cplusplus
        }
#endif

#endif
