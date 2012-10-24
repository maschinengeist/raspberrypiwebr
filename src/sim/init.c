/*! \file init.c
*/

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <SDL.h>

#include <stdio.h>
#include <SDL_thread.h>

#include "mytypes.h"
#include "init.h"
#include "time.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define  BSP_GPIOC_BLUE                           DEF_BIT_08    /* LED. */
#define  BSP_GPIOC_GREEN                          DEF_BIT_09    /* LED. */
#define  BSP_GPIOC_YELLOW                         DEF_BIT_06    /* LED. */

/*************************************************************************
*   G L O B A L
*************************************************************************/



/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/
extern int TimeThread( void *ptr);

extern void  IntDMAChannel2(void);
extern int DmaThread( void *ptr);

/************************************************************************
*/
/*! \fn void init(void)
*
*   \brief init the controller
*
*   \return void
*
*************************************************************************/
void Init(void)
{
	SDL_Thread *thread;
	printf("\nCreate Time Thread\r\n");

	// Simply create a thread
	thread = SDL_CreateThread(TimeThread, (void *)NULL);
	// Simply create a thread
	thread = SDL_CreateThread(DmaThread, (void *)NULL);
}

int DmaThread( void *ptr )
{
	while(1)
	{
		/* trigger time */
   	IntDMAChannel2();
		SDL_Delay(100);
	}
}

// Very simple thread - counts 0 to 9 delaying 50ms between increments
int TimeThread( void *ptr )
{
	while(1)
	{
		/* trigger time */
   	Time_IrqSet100MS();
		SDL_Delay(100);
	}
}

#ifdef __cplusplus
   }
#endif

