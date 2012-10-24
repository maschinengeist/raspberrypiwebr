
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#include "mytypes.h"
#include "debug.h"
#include "spi.h"


/*************************************************************************
*   G L O B A L
*************************************************************************/

static struct spi_ioc_transfer Mesg;

/*************************************************************************
*   F U N C T I O N S
*************************************************************************/

/**
*  \brief Write data to an given uart
*
*  \param[in]  cPath       Uart path, for example "/dev/spidev1.0"
*  \param[in]  u32Speed    Speed for for spi
*  \param[in]  u08Bits     Number of bits, for example 8 or 16
*  \param[in]  u08Mode     Mode for Spi, SPI_MODE_0 to SPI_MODE_3 (0 to 3)
*
*  \return     handle to spi port
*/
int spi_Open(CHAR* cPath, INT_U32 u32Speed, INT_U8 u08Bits, INT_U8 u08Mode)
{
	int iSpi, iResult, iBits, iMode, iSpeed;

   iBits = (int)u08Bits;
   iSpeed = (int)u32Speed;
   iMode = (int)u08Mode;

   /* save setings for later access */
   Mesg.speed_hz = iSpeed;
   Mesg.bits_per_word = iBits;
   Mesg.delay_usecs = 100;

   /* Open path an check if available */
	iSpi = open(cPath, O_RDONLY);

	if(iSpi < 0)
	{
		DB_PRINTF(("Cannot open %s\r\n", cPath));
		return iSpi;
	}
	else
	{
	   /* Set mode for read and write */
		iResult = ioctl(iSpi, SPI_IOC_WR_MODE, &iMode);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_WR_MODE ioctl error\r\n"));
			return -1;
		}

		iResult = ioctl(iSpi, SPI_IOC_RD_MODE, &iMode);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_RD_MODE ioctl error\r\n"));
			return -1;
		}

	   /* Set bits for read and write */
		iResult = ioctl(iSpi, SPI_IOC_WR_BITS_PER_WORD, &iBits);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_WR_BITS_PER_WORD ioctl error\r\n"));
			return -1;
		}

		iResult = ioctl(iSpi, SPI_IOC_RD_BITS_PER_WORD, &iBits);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_RD_BITS_PER_WORD ioctl error\r\n"));
			return -1;
		}

	   /* Set speed for read and write */
      iResult = ioctl(iSpi, SPI_IOC_WR_MAX_SPEED_HZ, &iSpeed);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_WR_MAX_SPEED_HZ ioctl error\r\n"));
			return -1;
		}

		iResult = ioctl(iSpi, SPI_IOC_RD_MAX_SPEED_HZ, &iSpeed);
		if(iResult < 0)
		{
			DB_PRINTF(("SPI_IOC_RD_MAX_SPEED_HZ ioctl error\r\n"));
			return -1;
		}
	}

   return iSpi;
}

/**
*  \brief Close the open spi
*
*  \param[in]  iUart       Handle to spi
*
*  \return     none
*/
void spi_Close(int iSpi)
{
   close(iSpi);
}

/**
*  \brief Write data to an given spi
*
*  \param[in]  iSpi          handle to Spi device
*  \param[in]  p08SendBuffer Pointer to send buffer
*  \param[in]  u16Size       Size of data to send
*
*  \param[out] p08ReceiveBuffer   Pointer to receive buffer
*
*  \return     on error -1, on success size of data
*/
INT_S16 spi_ReadWrite(int iSpi, INT_U8* p08SendBuffer, INT_U8* p08ReceiveBuffer, INT_U16 u16Size)
{
   Mesg.tx_buf = (unsigned long) p08SendBuffer;
   Mesg.rx_buf = (unsigned long) p08ReceiveBuffer;
   Mesg.len = u16Size;

   if(ioctl(iSpi, SPI_IOC_MESSAGE(1), &Mesg) != -1)
   {
      return (INT_S16) u16Size;
   }
   else
   {
      DB_PRINTF(("Send failed\r\n"));
      return -1;
   }
}



