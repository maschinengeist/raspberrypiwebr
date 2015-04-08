/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "mytypes.h"
#include "gpio.h"
#include "io.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define GPIO_LED_TEST 			22
#define GPIO_LED_BACKLIGHT 	4
#define GPIO_EADOGM_RESET 		12
#define GPIO_TASTER1 			5
#define GPIO_TASTER2 			6
#define GPIO_TASTER3				27
#define GPIO_TASTER4				17
#define GPIO_TASTER_UP			21
#define GPIO_TASTER_DOWN		18
#define GPIO_TASTER_LEFT		19
#define GPIO_TASTER_RIGHT		26
#define GPIO_TASTER_MIDDLE    20
#define GPIO_TASTER_POWER     16
#define GPIO_USB_POWER        22
#define GPIO_POWER_OFF        13


/*************************************************************************
*   G L O B A L
*************************************************************************/

/************************************************************************
*/
/*! \brief Set io's to default state
*
*************************************************************************/
void io_Init(void)
{
	/* output */
	gpio_Init(GPIO_LED_TEST, GPIO_DIRECTION_OUT, GPIO_EDGE_NONE);
	gpio_Init(GPIO_USB_POWER, GPIO_DIRECTION_OUT, GPIO_EDGE_NONE);
	gpio_Init(GPIO_POWER_OFF, GPIO_DIRECTION_OUT, GPIO_EDGE_NONE);

	/* input */
	gpio_Init(GPIO_TASTER1, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER2, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER3, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER4, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_UP, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_DOWN, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_LEFT, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_RIGHT, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_MIDDLE, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
	gpio_Init(GPIO_TASTER_POWER, GPIO_DIRECTION_IN, GPIO_EDGE_NONE);
}

/************************************************************************
*/
/*! \brief Set io's on or off
*
*   \param u08IO --> io to set
*   \param u08State --> IO_OFF, IO_ON
*
*************************************************************************/
void io_Set(INT_U8 u08IO, INT_U8 u08State)
{
   /* Speed LEDs */
   switch(u08IO)
   {
      case LED_TEST:
      {
         if(u08State == 0) gpio_Write(GPIO_LED_TEST, 0);
         else gpio_Write(GPIO_LED_TEST, 1);
      }
      break;

      case LED_BACKLIGHT:
      {
         if(u08State == 0) gpio_Write(GPIO_LED_BACKLIGHT, 0);
         else gpio_Write(GPIO_LED_BACKLIGHT, 1);
      }
      break;

      case EADOGM_RESET:
      {
         if(u08State == 0) gpio_Write(GPIO_EADOGM_RESET, 0);
         else gpio_Write(GPIO_EADOGM_RESET, 1);
      }
      break;

      case USB_POWER:
      {
         if(u08State == 0) gpio_Write(GPIO_USB_POWER, 0);
         else gpio_Write(GPIO_USB_POWER, 1);
      }
      break;

      case POWER_OFF:
      {
         if(u08State == 0) gpio_Write(GPIO_POWER_OFF, 0);
         else gpio_Write(GPIO_POWER_OFF, 1);
      }
      break;

      default:
      break;
   }
}


/************************************************************************
*/
/*! \brief Get the io state
*
*   \param u08IO --> io state to get
*   \return INT_U8 --> 0 = io 0ff, 1 = io on
*
*************************************************************************/
INT_U8 io_Get(INT_U8 u08IO)
{
   /* Speed LEDs */
   switch(u08IO)
   {
      case TASTER1:
      {
         if(gpio_Read(GPIO_TASTER1) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER2:
      {
         if(gpio_Read(GPIO_TASTER2) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER3:
      {
         if(gpio_Read(GPIO_TASTER3) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER4:
      {
         if(gpio_Read(GPIO_TASTER4) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_UP:
      {
         if(gpio_Read(GPIO_TASTER_UP) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_DOWN:
      {
         if(gpio_Read(GPIO_TASTER_DOWN) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_LEFT:
      {
         if(gpio_Read(GPIO_TASTER_LEFT) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_RIGHT:
      {
         if(gpio_Read(GPIO_TASTER_RIGHT) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_MIDDLE:
      {
         if(gpio_Read(GPIO_TASTER_MIDDLE) == 0) return 0;
         else return 1;
      }
      break;

      case TASTER_POWER:
      {
         if(gpio_Read(GPIO_TASTER_POWER) == 0) return 0;
         else return 1;
      }
      break;

      default:
      break;
   }

   return 0;
}


