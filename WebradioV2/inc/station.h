#ifndef __STATION_H__
#define __STATION_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/


/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

int station_Load(char *cPath);

char* station_Read(int iEntrie);

int station_Clear(char *cPath, int iEntrie);

int station_Write(char *cPath, char *cStationName, char *cStationAddress);

int station_MaxEntries(void);

#ifdef __cplusplus
        }
#endif

#endif


