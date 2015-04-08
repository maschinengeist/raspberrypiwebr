#ifndef __IO_H__
#define __IO_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

enum
{
    LED_TEST = 1,
    LED_BACKLIGHT,
    EADOGM_RESET,
    TASTER1,
    TASTER2,
    TASTER3,
    TASTER4,
    TASTER_UP,
    TASTER_DOWN,
	 TASTER_LEFT,
	 TASTER_RIGHT,
	 TASTER_MIDDLE,
	 TASTER_POWER,
	 POWER_OFF,
    USB_POWER
};

enum
{
    IO_OFF = 0,
    IO_ON
};


/*************************************************************************
*   P R O T O T Y P E
*************************************************************************/

void io_Init(void);

void io_Set(INT_U8 u08IO, INT_U8 u08State);

INT_U8 io_Get(INT_U8 u08IO);

#ifdef __cplusplus
        }
#endif

#endif


