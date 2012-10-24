
#if !defined(__STATION_H__)
#define __STATION_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define MAX_SIZE_STATION    40
#define MAX_SIZE_STREAM_PATH  128

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

typedef struct _STATION_ENTRY
{
	int iNumber;
	char cStation[MAX_SIZE_STATION];
	char cStream[MAX_SIZE_STREAM_PATH];

} STATION_ENTRY;

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int Station_Open(char* cFile);

int Station_Read(STATION_ENTRY* StationEntry, int iNumber);

int Station_Write(STATION_ENTRY* StationEntry, int iNumber);

int Station_Append(STATION_ENTRY StationEntry);

void Station_Close(void);




#ifdef __cplusplus
        }
#endif

#endif
