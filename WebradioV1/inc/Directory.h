
#if !defined(__DIRECTORY_H__)
#define __DIRECTORY_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define DIR_TYPE_FOLDER 1
#define DIR_TYPE_FILE   2
#define DIR_TYPE_MP3    3
#define DIR_TYPE_MCU    4
#define DIR_TYPE_PLS		5

/**************************************************************************************
*   S T R U C T
***************************************************************************************/


typedef struct _DIRECTORY_CONTENTS
{
   /* pointer to Name of entrie */
   char *cName;
   /* Type of entrie */
   int  iType;

} DIRECTORY_CONTENTS;

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int Directory_Open(char *cPath);

int Directory_ReadEntrie(int iEntrie, DIRECTORY_CONTENTS* pDirEntries);

int Directory_MaxEntries(void);

#ifdef __cplusplus
        }
#endif

#endif
