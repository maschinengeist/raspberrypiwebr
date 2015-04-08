
#if !defined(__GPIO_H__)
#define __GPIO_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

enum
{
   GPIO_DIRECTION_IN = 0,
   GPIO_DIRECTION_OUT
};


enum
{
    GPIO_EDGE_NONE = 0,
    GPIO_EDGE_FALLING,
    GPIO_EDGE_RISING,
    GPIO_EDGE_BOTH
};

/*************************************************************************
*   D E C L A R A T I O N
*************************************************************************/

RESULT gpio_Init(INT_U8 u08GpioPin, INT_U8 u08Direction, INT_U8 u08Edge);

RESULT gpio_Direction(INT_U8 u08GpioPin, INT_U8 u08Direction);

RESULT gpio_Write(INT_U8 u08GpioPin, INT_U8 u08Value);

INT_U8 gpio_Read(INT_U8 u08GpioPin);

int gpio_Open(INT_U8 u08GpioPin);


#ifdef __cplusplus
	}
#endif

#endif

/*** EOF ***/

