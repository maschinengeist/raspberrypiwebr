/*! \file message.h
*/

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include "SDL.h"
#include "mytypes.h"
#include "message.h"

/*************************************************************************
*   D E F I N E
*************************************************************************/

#define USERINTERFACE_GPIOB_ROTATION_RIGHT   (1<<9)
#define USERINTERFACE_GPIOB_ROTATION_LEFT    (1<<8)
#define USERINTERFACE_GPIOB_ROTATION_MIDDLE  (1<<7)

/*************************************************************************
*   G L O B A L
*************************************************************************/

static INT_U8 u08Message;

/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/

/************************************************************************
*/
/*! \fn INT_U8 Message_Get(void)

*   \brief read rotation and state of middle pin
*
*   \return INT_U08 --> 0x00 = no change, 0x01 = rotate right
*   \return 0x02 = rotate left, 0x80 = swicht middle
*
*************************************************************************/
INT_U8 Message_Get(void)
{
	// message processing loop
	SDL_Event event;

	if(SDL_PollEvent(&event))
	{
	   if(event.type == SDL_KEYDOWN)
	   {
	      u08Message = (INT_U8) event.key.keysym.scancode;
	   }
	}

	if(u08Message)
	{
      switch (u08Message)
      {
         /* up */
         case 71:
            return MESSAGE_CH1_RIGHT;
         break;

         /* down */
         case 79:
            return MESSAGE_CH1_LEFT;
         break;

         /* enter */
         case 75:
            return MESSAGE_CH1_MIDDLE;
         break;

         /* up */
         case 73:
            return MESSAGE_CH2_RIGHT;
         break;

         /* down */
         case 81:
            return MESSAGE_CH2_LEFT;
         break;

         /* enter */
         case 77:
            return MESSAGE_CH2_MIDDLE;
         break;

         /* button 1*/
         case 2:
            return MESSAGE_TASTER_1;
         break;

         /* button 2*/
         case 3:
            return MESSAGE_TASTER_2;
         break;

         /* button 3*/
         case 4:
            return MESSAGE_TASTER_3;
         break;

         /* button 4*/
         case 5:
            return MESSAGE_TASTER_4;
         break;

         /* button 4*/
         case 6:
            return MESSAGE_TASTER_5;
         break;
		}
	}

  	return 0;
}

/************************************************************************
*/
/*! \fn INT_U8 Message_Clear(void)

*   \brief read rotation and state of middle pin
*
*   \return INT_U08 --> 0x00 = no change, 0x01 = rotate right
*   \return 0x02 = rotate left, 0x80 = swicht middle
*
*************************************************************************/
void Message_Clear(void)
{
   u08Message = 0;
}


#ifdef __cplusplus
   }
#endif

