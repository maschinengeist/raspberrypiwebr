
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "smadef.h"
#include "debug.h"
#include "uart.h"


#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   G L O B A L
*************************************************************************/

/*************************************************************************
*   F U N C T I O N S
*************************************************************************/

/**
*  \brief Write data to an given uart
*
*  \param[in]  cPath       Uart path, for example "/dev/ttyS0"
*  \param[in]  u32Speed    Speed for uart, for example 9600
*
*  \param[out] pOut        parameter description 3
*
*  \return     handle to com port
*/
int uart_Open(CHAR* cPath, SMA_U32 u32Speed)
{
   int iUartPort = 0, pins;
	struct termios tio;
	speed_t speed = B9600;

   /* get baudrate for speed_t */
   switch(u32Speed)
   {
      case 300:
         speed = B300;
      break;

      case 600:
         speed = B600;
      break;

      case 1200:
         speed = B1200;
      break;

      case 2400:
         speed = B2400;
      break;

      case 4800:
         speed = B4800;
      break;

      case 9600:
         speed = B9600;
      break;

      case 19200:
         speed = B19200;
      break;

      case 38400:
         speed = B38400;
      break;

      case 57600:
         speed = B57600;
      break;

      case 115200:
         speed = B115200;
      break;

      case 230400:
         speed = B230400;
      break;

      default:
      break;
   }


	if ((iUartPort = open(cPath, O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY)) == -1)
	{
		DB_PRINTF(("uart open error\r\n"));
		return iUartPort;
	}

	if (tcgetattr(iUartPort, &tio) == -1)                        // port opening failed?
	   fprintf(stderr," Can't open %s\n", cPath);

	if(cfsetispeed(&tio, speed) == -1)                                      // unable to set input baudrate?
	   DB_PRINTF((" serial_open(): unable to set input baudrate!\n"));

	if(cfsetospeed(&tio, speed) == -1)                                      // unable to set output baudrate?
		DB_PRINTF((" serial_open(): unable to set output baudrate!\n"));

	cfmakeraw(&tio);                                                        // set the terminal attributes

	/* enable flow control for rts and cts */
	tio.c_cflag |= CRTSCTS;

	if(tcflush(iUartPort, TCIOFLUSH) == -1)                      // unable to discard pending data?
		DB_PRINTF((" serial_open(): unable to flush data!\n"));


	if(tcsetattr(iUartPort, TCSANOW, &tio) == -1)                // unable to set terminal attributes
	{
		DB_PRINTF((" serial_open(): unable to set new attributes!\n"));
		DB_PRINTF((" error:%s\n",strerror(errno)));
	}

   /* Activate DTR and RTS lines */
   ioctl(iUartPort, TIOCMGET, &pins);
   pins |= (TIOCM_DTR | TIOCM_RTS);
   ioctl(iUartPort, TIOCMSET, &pins);

	return iUartPort;
}

/**
*  \brief Close the open uart
*
*  \param[in]  iUart       Handle to uart port
*
*  \return     none
*************************************************************************/
void uart_Close(int iUart)
{
   close(iUart);
}

/**
*  \brief Write data to an given uart
*
*  \param[in]  iUart       Handle to uart port
*  \param[in]  p08Data     Pointer to data
*  \param[in]  s16Size     Size of data to send
*
*  \return     SMA_S16     Count data were sent to or -1 for an error
*/
SMA_S16 uart_Write(int iUart, SMA_U8* p08Data, SMA_S16 s16Size)
{
   int iWriteSize;

   /* write data to uart and return number of bytes write */
   iWriteSize = write(iUart, p08Data, s16Size);

   return (SMA_S16) iWriteSize;
}


/***********************************************************************
 */
/*! \fn SMA_S16 uart_Read(int iUart, SMA_U8* p08Data, SMA_S16 s16Size)
 *
 *   \brief Read data from an given uart
 *
 *   \param iUart --> Handle to uart port
 *   \param p08Data --> Pointer to data
 *   \param s16Size --> Size of data to read
 *
 *   \return SMA_S16 --> Count data were read to or -1 for an error
 *
 *************************************************************************/
SMA_S16 uart_Read(int iUart, SMA_U8* p08Data, SMA_S16 s16Size)
{
   int iWriteSize;

   /* read data to uart and return number of bytes read */
   iWriteSize = read(iUart, p08Data, s16Size);

   return (SMA_S16) iWriteSize;
}


/***********************************************************************
 */
/*! \fn RESULT uart_SetRTS(int iUart, SMA_U8 u08State)
 *
 *   \brief Set or clear RTS
 *
 *   \param iUart --> Handle to uart port
 *   \param u08State --> 1 set RTS, or 0 clear RTS
 *
 *   \return RESULT --> SMA_OK all is fine, or SMA_ERROR uart can't open
 *
 *************************************************************************/
RESULT uart_SetRTS(int iUart, SMA_U8 u08State)
{
   int iState;

   /* if u08State true, activate RTS */
   iState = TIOCM_RTS;
   if(u08State) ioctl(iUart, TIOCMBIS, &iState);
   else ioctl (iUart, TIOCMBIC, &iState);

   return SMA_OK;
}



void setrts(int fd, int on)
{
   int controlbits;

   ioctl(fd, TIOCMGET, &controlbits);

   if (on)
   {
      controlbits |= TIOCM_RTS;
   } else {
      controlbits &= ~TIOCM_RTS;
   }
   ioctl(fd, TIOCMSET, &controlbits);
}

/***********************************************************************
 */
/*! \fn RESULT uart_SetDTR(int iUart, SMA_U8 u08State)
 *
 *   \brief Set or clear DTR
 *
 *   \param iUart --> Handle to uart port
 *   \param u08State --> 1 set DTR, or 0 clear DTR
 *
 *   \return RESULT --> SMA_OK all is fine, or SMA_ERROR uart can't open
 *
 *************************************************************************/
RESULT uart_SetDTR(int iUart, SMA_U8 u08State)
{
   int iState;

   /* if u08State true, activate DTR */
   iState = TIOCM_DTR;
   if(u08State) ioctl(iUart, TIOCMBIS, &iState);
   else ioctl (iUart, TIOCMBIC, &iState);

   return SMA_OK;
}


/***********************************************************************
 */
/*! \fn RESULT uart_ReadDCD(int iUart, SMA_U8* p08State)
 *
 *   \brief Read State of DCD
 *
 *   \param iUart --> Handle to uart port
 *   \param u08State --> Pointer to State,  1 = DCD set, or 0 = DCD clear
 *
 *   \return RESULT --> SMA_OK all is fine, or SMA_ERROR uart can't open
 *
 *************************************************************************/
RESULT uart_ReadDCD(int iUart, SMA_U8* p08State)
{
   int iState;

   /* read state of DCD pin */
   ioctl(iUart, TIOCMBIS, &iState);

   if(iState & TIOCM_CD) *p08State = 1;
   else *p08State = 0;

   return SMA_OK;
}

/**
*  \brief Write data to an given uart, read the data back and verify it
*
*  \param[in]  iUart         Handle to uart port
*  \param[in]  u16Size       Size of data to send
*  \param[in]  p16ErrorCount Pointer for an error count result
*  \param[in]  u16Timeout    Timeout im millseconds to abort the test
*
*  \return     RESULT     SMA_OK Test is passed without errors, SMA_ERROR Test failed, error in error count
*/
RESULT uart_MirrorTest(int iUart, SMA_U16 u16Size, SMA_U16* p16ErrorCount, SMA_U16 u16Timeout)
{
   RESULT Result = SMA_ERROR;
   int iSize;
   SMA_U8* p08SendData;
   SMA_U8* p08ReceiveData;
   SMA_U16 u16Loop, u16SizeRead, u16TimeoutCounter;

   /* check if uart open */
   if(iUart > 0)
   {
      /* allocate memory for serial test and return with error in case of no memory available */
      p08SendData = (SMA_U8 *) malloc(u16Size);
      if(p08SendData == 0)
      {
         free(p08SendData);
         return SMA_ERROR;
      }

      /* allocate memory for serial test and return with error in case of no memory available */
      p08ReceiveData = (SMA_U8 *) malloc(u16Size);
      if(p08ReceiveData == 0)
      {
         free(p08SendData);
         free(p08ReceiveData);
         return SMA_ERROR;
      }

      /* creat test pattern */
      for(u16Loop = 0 ; u16Loop < u16Size; u16Loop++)
      {
         /* pattern is simple counting up */
         p08SendData[u16Loop] = (SMA_U8) u16Loop;

         /* set receive data different to send data, to get all errors */
         p08ReceiveData[u16Loop] = (SMA_U8) u16Loop + 1;
      }

      /* read rest of uart data */
      iSize = uart_Read(iUart, &p08ReceiveData[0], u16Size);

      /* send data over uart */
      iSize = uart_Write(iUart, p08SendData, u16Size);

      if(iSize == -1)
      {
         /* free data */
         free(p08SendData);
         free(p08ReceiveData);

         DB_PRINTF(("Send failed\r\n"));
         return SMA_ERROR;
      }

      /* Set counter for receive data to zero */
      u16SizeRead = 0;

      /* set timeout */
      u16TimeoutCounter = u16Timeout;

      do
      {
         /* read data minus the befor reading data */
         iSize = uart_Read(iUart, &p08ReceiveData[u16SizeRead], (u16Size - u16SizeRead));

         if(iSize > 0)
         {
            u16SizeRead += (SMA_U16) iSize;
            u16TimeoutCounter = u16Timeout;
         }
         else
         {
            /* sleep 1 ms and count timeout down */
            usleep(1000);
            u16TimeoutCounter--;
         }
      }
      while((u16SizeRead < u16Size) && u16TimeoutCounter);

      /* verify data and increment error count */
      *p16ErrorCount = 0;

      for(u16Loop = 0 ; u16Loop < u16Size; u16Loop++)
      {
         /* check for errors */
         if(!(p08ReceiveData[u16Loop] == (u16Loop & 0x00ff)))
         {
            (*p16ErrorCount)++;
         }
      }

      /* all data read and no error then set result to ok */
      if((*p16ErrorCount == 0) && (u16SizeRead == u16Size))
      {
         Result = SMA_OK;
      }
      else
      {
         Result = SMA_ERROR;
      }

      free(p08SendData);
      free(p08ReceiveData);
   }

   return Result;
}




#ifdef __cplusplus
	}
#endif


