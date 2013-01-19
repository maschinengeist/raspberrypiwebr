/*! \file Menue_OnOff.h
*/


#if !defined(__MENUE_ONOFF_H__)
#define __MENUE_ONOFF_H__

#ifdef __cplusplus
	extern "C" {
#endif


/**************************************************************************************
*   D E F I N E
***************************************************************************************/



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

void Menue_Start(void);

void Menue_On(WEBRADIO *pWebRadio);

void Menue_Off(WEBRADIO *pWebRadio);


#ifdef __cplusplus
        }
#endif

#endif