/*! \file message.h
*/

#if !defined(__MESSAGE_H__)
#define __MESSAGE_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

enum MESSAGES
{
	MESSAGE_NOT_AVAILABLE = 0,
   MESSAGE_KEY_RIGHT,
   MESSAGE_KEY_LEFT,
   MESSAGE_KEY_MIDDLE,
   MESSAGE_KEY_SETUP,
   MESSAGE_REFRESH_SCREEN
};

/**************************************************************************************
*   D E C L A R A T I O N
***************************************************************************************/

int Message_Init(void);

void Message_Get(void);

int Message_Read(void);

void Message_Set(int iMessage);

void Message_Clear(void);

#ifdef __cplusplus
        }
#endif

#endif
