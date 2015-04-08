/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>

#include "SerialCom.h"

/*************************************************************************
*   G L O B A L
*************************************************************************/

static int iFdPort;

SerialCom::SerialCom()
{
	iFdPort = 0;
}


/************************************************************************
*/
/*!
*  \fn int SerialCom::Open(char *cPath, speed_t Speed)
*
*  \brief Open serial port
*
*  \param cPath  --> string for device path
*  \param speed          --> required baud rate (B9600, B19200, B115200...)
*
*  \return int --> file descriptor of the opened device
*
*
*************************************************************************/
int SerialCom::Open(char *cPath, speed_t Speed)
{

	struct termios tio;

	if ((iFdPort = open(cPath, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1)
	{
		fprintf(stderr, "open error\n");
	}

	if (tcgetattr(iFdPort, &tio) == -1)                        // port opening failed?
	   fprintf(stderr," Can't open %s\n", cPath);

	if(cfsetispeed(&tio, Speed) == -1)                                      // unable to set input baudrate?
	   printf(" serial_open(): unable to set input baudrate!\n");

	if(cfsetospeed(&tio, Speed) == -1)                                      // unable to set output baudrate?
		printf(" serial_open(): unable to set output baudrate!\n");

	cfmakeraw(&tio);                                                        // set the terminal attributes

	if(tcflush(iFdPort, TCIOFLUSH) == -1)                      // unable to discard pending data?
		printf(" serial_open(): unable to flush data!\n");

	if(tcsetattr(iFdPort, TCSANOW, &tio) == -1)                // unable to set terminal attributes
	{
		printf(" serial_open(): unable to set new attributes!\n");
		printf(" error:%s\n",strerror(errno));
	}

	return iFdPort;                                            // return the file descriptor
}


/************************************************************************
*/
/*!
*  \fn int SerialCom::Write(int fd, char* cBuffer, int iSize)
*
*  \brief Write to serial port
*
*  \param fd  		--> file descriptor
*  \param cBuffer --> string for serial interface
*  \param iSize   --> size of data
*
*  \return int --> file descriptor of the opened device
*
*
*************************************************************************/
int SerialCom::Write(char* cBuffer, int iSize)
{
	return write(iFdPort, cBuffer, iSize);
}

/************************************************************************
*/
/*!
*  \fn int SerialCom::Write(int fd, char* cBuffer, int iSize)
*
*  \brief Write to serial port
*
*  \param fd  		--> file descriptor
*  \param cBuffer --> string for serial interface
*  \param iSize   --> size data to be read
*
*  \return int --> size of read data
*
*
*************************************************************************/
int SerialCom::Read(char* cBuffer, int iSize)
{
	return read(iFdPort, cBuffer, iSize);
}

/************************************************************************
*/
/*!
*  \fn int SerialCom::Close(void)
*
*  \brief Close serial port
*
*  \return int --> file descriptor of the opened device
*
*************************************************************************/
int SerialCom::Close(void)
{
	return close(iFdPort);
}


SerialCom::~SerialCom()
{
	//dtor
}
