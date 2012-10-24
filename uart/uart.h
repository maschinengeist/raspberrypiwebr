
#if !defined(__UART_H__)
#define __UART_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E C L A R A T I O N
*************************************************************************/

int uart_Open(CHAR* cPath, SMA_U32 u32Speed);

void uart_Close(int iUart);

SMA_S16 uart_Write(int iUart, SMA_U8* p08Data, SMA_S16 s16Size);

SMA_S16 uart_Read(int iUart, SMA_U8* p08Data, SMA_S16 s16Size);

RESULT uart_SetRTS(int iUart, SMA_U8 u08State);

void setrts(int fd, int on);

RESULT uart_SetDTR(int iUart, SMA_U8 u08State);

RESULT uart_ReadDCD(int iUart, SMA_U8* p08State);

RESULT uart_MirrorTest(int iUart, SMA_U16 u16Size, SMA_U16* p16ErrorCount, SMA_U16 u16Timeout);

#ifdef __cplusplus
	}
#endif

#endif

/*** EOF ***/

