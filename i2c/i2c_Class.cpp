/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

#include "mytypes.h"
#include "i2c_Class.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   G L O B A L
*************************************************************************/

/***********************************************************************
 */
/*! \fn int Constructor
 *
  *
 *************************************************************************/
i2c_Class::i2c_Class()
{
	printf("Constructor\n");
}

/***********************************************************************
 */
/*! \fn int i2c_Class::Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
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
int i2c_Class::Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
{
   int i2c_file;


   printf("Write: %s, 0x%x, 0x%x, 0x%x\n", cDevice, u16Address, p08Data, u16Size);





#if 0
   if((i2c_file = open(cDevice, O_RDWR)) < 0)
   {
      printf(("Failed to open the bus\r\n"));
      return -1;
   }

   if(ioctl(i2c_file, I2C_SLAVE, u16Address) < 0)
   {
      printf(("Failed to acquire bus access and/or talk to slave.\r\n"));
      close(i2c_file);
      return -1;
   }

   if(write(i2c_file, p08Data, u16Size) != u16Size)
   {
      /* ERROR HANDLING: i2c transaction failed */
      printf(("Failed to write to the i2c bus.\r\n"));
      close(i2c_file);
      return -1;
   }

   close(i2c_file);
#endif
	return 1;
}

/***********************************************************************
 */
/*! \fn int i2c_Class::Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
 *
 *   \brief Read from the i2c port
 *
 *   \param cDevice --> the i2c device path for Example /dev/i2c-0
 *   \param u16Address --> the addres of the i2c device
 *   \param p08Data --> pointer to data to send
 *   \param u16Size --> size of data to send
 *
 *   \return int --> 1 = write successful to device or -1 = error durring write
 *
 *************************************************************************/
int i2c_Class::Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size)
{
   int i2c_file;

   printf("Read: %s, 0x%x, 0x%x, 0x%x\n", cDevice, u16Address, p08Data, u16Size);


#if 0
   if((i2c_file = open(cDevice, O_RDWR)) < 0)
   {
      printf(("Failed to open the bus\r\n"));
      return -1;
   }

   if(ioctl(i2c_file, I2C_SLAVE, u16Address) < 0)
   {
      printf(("Failed to acquire bus access and/or talk to slave.\r\n"));
      close(i2c_file);
      return -1;
   }

   if(read(i2c_file, p08Data, u16Size) != u16Size)
   {
      /* ERROR HANDLING: i2c transaction failed */
      printf(("Failed to read to the i2c bus.\r\n"));
      close(i2c_file);
      return -1;
   }

   close(i2c_file);

#endif
	return 1;
}

/***********************************************************************
 */
/*! \fn int Deconstructor
 *
  *
 *************************************************************************/
i2c_Class::~i2c_Class()
{
	printf("Deconstructor\n");
}
