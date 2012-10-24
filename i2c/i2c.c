
/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "debug.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

#include "i2c.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   F U N C T I O N
*************************************************************************/

/***********************************************************************
 */
/*! \fn int i2c_Init(CHAR* cDevice, u08Address, p08Data)
 *
 *   \brief Init an gpio
 *
 *   \param u08GpioPin --> the gpio nummber to init
 *   \param u08Direction --> the gpio direction, can be GPIO_DIRECTION_IN or GPIO_DIRECTION_OUT
 *
 *   \return int --> 1 = gpio is now available or -1 = th gpio is not available for the user
 *
 *************************************************************************/
int i2c_Init(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
{
   return -1;
}

/***********************************************************************
 */
/*! \fn int i2c_Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
 *
 *   \brief Write to the i2c port
 *
 *   \param cDevice --> the i2c device path for Example /dev/i2c-0
 *   \param u16Address --> the addres of the i2c device
 *   \param p08Data --> pointer to data to send
 *   \param u16Size --> size of data to send
 *
 *   \return int --> 1 = write successful to device or -1 = error durring write
 *
 *************************************************************************/
int i2c_Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
{
   int i2c_file;

   if((i2c_file = open(cDevice, O_RDWR)) < 0)
   {
      DB_PRINTF(("Failed to open the %s bus\r\n"));
      return -1;
   }

   if(ioctl(i2c_file, I2C_SLAVE, u16Address) < 0)
   {
      DB_PRINTF(("Failed to acquire bus access and/or talk to slave.\r\n"));
      close(i2c_file);
      return -1;
   }

   if(write(i2c_file, p08Data, u16Size) != u16Size)
   {
      /* ERROR HANDLING: i2c transaction failed */
      DB_PRINTF(("Failed to write to the i2c bus.\r\n"));
      close(i2c_file);
      return -1;
   }

   close(i2c_file);
	return 1;
}

/***********************************************************************
 */
/*! \fn int i2c_Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
 *
 *   \brief Write to the i2c port
 *
 *   \param cDevice --> the i2c device path for Example /dev/i2c-0
 *   \param u16Address --> the addres of the i2c device
 *   \param p08Data --> pointer to data to send
 *   \param u16Size --> size of data to send
 *
 *   \return int --> 1 = write successful to device or -1 = error durring write
 *
 *************************************************************************/
int i2c_Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
{
   int i2c_file;

   if((i2c_file = open(cDevice, O_RDWR)) < 0)
   {
      DB_PRINTF(("Failed to open the %s bus\r\n"));
      return -1;
   }

   if(ioctl(i2c_file, I2C_SLAVE, u16Address) < 0)
   {
      DB_PRINTF(("Failed to acquire bus access and/or talk to slave.\r\n"));
      close(i2c_file);
      return -1;
   }

   if(read(i2c_file, p08Data, u16Size) != u16Size)
   {
      /* ERROR HANDLING: i2c transaction failed */
      DB_PRINTF(("Failed to read to the i2c bus.\r\n"));
      close(i2c_file);
      return -1;
   }

   close(i2c_file);
	return 1;
}








