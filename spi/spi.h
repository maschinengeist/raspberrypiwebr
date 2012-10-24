
#if !defined(__SPI_H__)
#define __SPI_H__

/*************************************************************************
*   D E C L A R A T I O N
*************************************************************************/

int spi_Open(CHAR* cPath, INT_U32 u32Speed, INT_U8 u08Bits, INT_U8 u08Mode);

void spi_Close(int iSpi);

INT_S16 spi_ReadWrite(int iSpi, INT_U8* p08SendBuffer, INT_U8* p08ReceiveBuffer, INT_U16 u16Size);

#endif


