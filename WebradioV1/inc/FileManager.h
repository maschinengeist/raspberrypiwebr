/*! \file FileManager.h
*/

#if !defined(__FILE_MANAGER_H__)
#define __FILE_MANAGER_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define MAX_SIZE_STATION    32
#define MAX_SIZE_GENRE      16
#define MAX_SIZE_HTTP_PATH  128

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

typedef struct _STATION_ENTRY
{
	int iNumber;
	char cStation[MAX_SIZE_STATION];
	char cGenre[MAX_SIZE_GENRE];
	char cStream[MAX_SIZE_HTTP_PATH];

} STATION_ENTRY;

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int FileManager_MaxEntries(char* cFile);

int FileManager_Read(char* cFile, STATION_ENTRY* StationEntry, int iNumber);

int FileManager_Write(char* cFile, STATION_ENTRY* StationEntry, int iNumber);

int FileManager_Append(char* cFile, STATION_ENTRY StationEntry);




#ifdef __cplusplus
        }
#endif

#endif
