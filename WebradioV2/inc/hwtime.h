
#if !defined(__HWTIME_H__)
#define __HWTIME_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define TIME_100MS 0x02
#define TIME_500MS 0x04
#define TIME_1SEK  0x08

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

void hwtime_Init(void);

void hwtime_Get(void);

INT_U8 hwtime_Check(INT_U8 u08TimeBits);

void hwtime_Clear(void);

void hwtime_Sleep(INT_U16 u16Time);

void hwtime_SleepUs(INT_U16 u16Time);

#ifdef __cplusplus
        }
#endif

#endif
