/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mytypes.h"

#include "gpio.h"
#include "i2c.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define				I2C_WAIT_TIME			160
#define				I2C_CLOCK    			3
#define				I2C_DATA    			2

/*************************************************************************
*   G L O B A L
*************************************************************************/

void i2c_wait(unsigned long int loop)
{	while(loop--);
}

void i2c_start(void)
{
	gpio_Direction(I2C_DATA, GPIO_DIRECTION_OUT);
   gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_OUT);

}

void i2c_stop(void)
{
	gpio_Direction(I2C_DATA, GPIO_DIRECTION_OUT);
   gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_IN);					// clock auf high danach änderung auf bus nicht erlaubt
	gpio_Direction(I2C_DATA, GPIO_DIRECTION_IN);				// änderung während clock auf high --> i2c stopbedingung
}

INT_U8 i2c_readack(void)
{
	INT_U8 u08Ack;
	gpio_Direction(I2C_DATA, GPIO_DIRECTION_IN);
	gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_IN);			// clock auf high danach ack bit abfragen
	u08Ack = gpio_Read(I2C_DATA);
   gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_OUT);
				// clock auf low danach änderung auf bus erlaubt
	gpio_Direction(I2C_DATA, GPIO_DIRECTION_OUT);
   return u08Ack;
}


INT_U8 i2c_write(INT_U8 u08Value)
{
   INT_U8 u08Loop, u08Ack;

   for(u08Loop = 0 ; u08Loop < 8 ; u08Loop++)
	{
		if(u08Value & 0x80) 	gpio_Direction(I2C_DATA, GPIO_DIRECTION_IN);
		else 	gpio_Direction(I2C_DATA, GPIO_DIRECTION_OUT);

		u08Value <<= 1;			// linksschieben des control bytes

		gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_IN);
      gpio_Direction(I2C_CLOCK, GPIO_DIRECTION_OUT);

	}

   u08Ack = i2c_readack();

#if 1
   if(u08Ack) printf("NOACK\r\n");
   //else printf("ACK\r\n");
#endif

   return u08Ack;
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
	INT_U16 u16Loop;

	u16Address <<= 1;

	i2c_start();

	i2c_write((INT_U8)u16Address);

	for(u16Loop = 0 ; u16Loop < u16Size ; u16Loop++)
	{
		i2c_write(p08Data[u16Loop]);
	}

	i2c_stop();

}

