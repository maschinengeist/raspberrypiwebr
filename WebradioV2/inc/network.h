/*! \file Network.h
*/

#if !defined(__NETWORK_H__)
#define __NETWORK_H__

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************************
*   D E F I N E
***************************************************************************************/

#define DHCP_ENABLE  1
#define DHCP_DISABLE 0

/**************************************************************************************
*   S T R U C T
***************************************************************************************/

typedef struct _NETWORK_SETTINGS
{
   INT_U8 u08IpAddress[4];
   INT_U8 u08IpNetmask[4];
   INT_U8 u08IpGateway[4];
   INT_U8 u08DhcpOn;

} NETWORK_SETTINGS;

/**************************************************************************************
*   P R O T O T Y P E
***************************************************************************************/

int network_ReadSettings(char* cPath, NETWORK_SETTINGS *pSettings);

int network_WriteSettings(char* cPath, NETWORK_SETTINGS *pSettings);

int network_Up(void);

int network_Down(void);

#ifdef __cplusplus
        }
#endif

#endif
