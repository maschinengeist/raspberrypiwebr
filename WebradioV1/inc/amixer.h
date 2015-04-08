/*! \file amixer.h
*/

#if !defined(__AMIXER_H__)
#define __AMIXER_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int amixer_Volume(INT_U8 u08Device, INT_U8 u08VolumePercent);

int amixer_Balance(INT_U8 u08Device, INT_U8 u08VolumeRight, INT_U8 u08VolumeLeft);

#ifdef __cplusplus
        }
#endif

#endif
