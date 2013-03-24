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
	MENUE_ON = 0,
	MENUE_DIR,
	MENUE_PLAY,
	MENUE_CHANGE_STATION,
   MENUE_SETUP,
	MENUE_STATION,
	MENUE_NEWSSTATION_SET_STATION,
	MENUE_NEWSSTATION_SET_TIME,
	MENUE_OFF
} ;

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define STREAM_ENTRY_MAX_SIZE  64

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

void Menue_ChangeMenue(INT_U8 u08Menue);

INT_U8 Menue_GetMenue(void);

int Menue_Init(INT_U16 u16Timeout);

int Menue_CheckTimeout(void);

void Menue_SetTimeout(INT_U16 u16Timeout);

void Menue_On(void);

void Menue_Select(void);

void Menue_Settings(void);

void Menue_NewsStation_SetStation(void);

void Menue_NewsStation_SetTime(void);

void Menue_Off(void);


#ifdef __cplusplus
        }
#endif

#endif
