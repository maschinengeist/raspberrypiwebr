/*! \file Network.c
*/


/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytypes.h"

#include "network.h"


/*************************************************************************
*   D E F I N E
*************************************************************************/


/*************************************************************************
*   G L O B A L
*************************************************************************/

/*************************************************************************
*   F O R W A R D   D E C L A R A T I O N
*************************************************************************/


/************************************************************************
*/
/*! \brief read the network settings like ip subnetmask and dhcp settings
*          from a giveb  config file
*
*   \param cPath --> Path to config file
*   \param pSettings --> Pointer to settings for the network
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int network_ReadSettings(char* cPath, NETWORK_SETTINGS *pSettings)
{
	#define NETWORK_INPUT_SIZE 128

	static FILE *fpNetwork;
	int iIP1, iIP2, iIP3, iIP4, iSize;
	char cText[NETWORK_INPUT_SIZE];
	char* pResult;

	/* open file */
	fpNetwork = fopen(cPath, "r");

	/* if open failed create default file */
	if(fpNetwork <= 0)
	{
		/* create default file */
		fpNetwork = fopen(cPath, "w");

		/* create file succesfull */
		if(fpNetwork <= 0)
		{
			/* create failed */
			return -1;
		}

		fprintf(fpNetwork, "auto lo\n");
		fprintf(fpNetwork, "iface lo inet loopback\n");
		fprintf(fpNetwork, "\n");

		fprintf(fpNetwork, "auto wlan0\n");
		fprintf(fpNetwork, "iface wlan0 inet dhcp\n");
		fprintf(fpNetwork, "wpa-conf /etc/wpa.conf\n");
		fprintf(fpNetwork, "\n");

		fprintf(fpNetwork, "auto eth0\n");
		fprintf(fpNetwork, "iface eth0 inet dhcp\n");

		/* clear all settings */
		memset(pSettings, 0x00, sizeof(NETWORK_SETTINGS));
		/* set dhcp to on */
		pSettings->u08DhcpOn = DHCP_ENABLE;

		/* close file */
		fclose(fpNetwork);
		return 1;
	}
	else
	{
		/* if seek failed return with an error */
		if(fseek(fpNetwork, 0, SEEK_SET) < 0) return -1;

		pResult = fgets(cText, NETWORK_INPUT_SIZE, fpNetwork);
		if(pResult == 0) return -1;

		/* if dhcp configured */
		if(0 == (memcmp(cText, "auto eth0", strlen("auto eth0"))))
		{
			/* clear all settings */
			memset(pSettings, 0x00, sizeof(NETWORK_SETTINGS));
			/* set dhcp to on */
			pSettings->u08DhcpOn = DHCP_ENABLE;
		}
		else
		{
			/* clear all settings */
			memset(pSettings, 0x00, sizeof(NETWORK_SETTINGS));

			/* read ip address settings */
			iSize = fscanf(fpNetwork, "address %d.%d.%d.%d\n", &iIP1, &iIP2, &iIP3, &iIP4);
			if(iSize == 0) return -1;

			/* get ip address */
			pSettings->u08IpAddress[0] = (INT_U8) iIP1;
			pSettings->u08IpAddress[1] = (INT_U8) iIP2;
			pSettings->u08IpAddress[2] = (INT_U8) iIP3;
			pSettings->u08IpAddress[3] = (INT_U8) iIP4;

			iSize = fscanf(fpNetwork, "netmask %d.%d.%d.%d\n", &iIP1, &iIP2, &iIP3, &iIP4);
			if(iSize == 0) return -1;

			/* get ip netmask */
			pSettings->u08IpNetmask[0] = (INT_U8) iIP1;
			pSettings->u08IpNetmask[1] = (INT_U8) iIP2;
			pSettings->u08IpNetmask[2] = (INT_U8) iIP3;
			pSettings->u08IpNetmask[3] = (INT_U8) iIP4;

			iSize = fscanf(fpNetwork, "gateway %d.%d.%d.%d\n", &iIP1, &iIP2, &iIP3, &iIP4);
			if(iSize == 0) return -1;

			/* get ip gateway */
			pSettings->u08IpGateway[0] = (INT_U8) iIP1;
			pSettings->u08IpGateway[1] = (INT_U8) iIP2;
			pSettings->u08IpGateway[2] = (INT_U8) iIP3;
			pSettings->u08IpGateway[3] = (INT_U8) iIP4;

			/* close file */
			fclose(fpNetwork);
		}
	}

	return 1;
}


/************************************************************************
*/
/*! \brief write the network settings like ip subnetmask and dhcp settings to config file
*
*   \param cPath --> Path to config file
*   \param pSettings --> Pointer to settings for the network
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int network_WriteSettings(char* cPath, NETWORK_SETTINGS *pSettings)
{
	static FILE *fpNetwork;

	fpNetwork = fopen(cPath, "w");

	/* open file */
	if(fpNetwork > 0)
	{

		/* if seek failed return with an error */
		if(fseek(fpNetwork, 0, SEEK_SET) < 0) return -1;

		if(pSettings->u08DhcpOn == DHCP_ENABLE)
		{
			fprintf(fpNetwork, "auto eth0\n");
			fprintf(fpNetwork, "iface eth0 inet dhcp\n");
		}
		else
		{
			fprintf(fpNetwork, "iface eth0 inet static\n");
			fprintf(fpNetwork, "address %d.%d.%d.%d\n", (int)pSettings->u08IpAddress[0], (int)pSettings->u08IpAddress[1], (int)pSettings->u08IpAddress[2], (int)pSettings->u08IpAddress[3]);
			fprintf(fpNetwork, "netmask %d.%d.%d.%d\n", (int)pSettings->u08IpNetmask[0], (int)pSettings->u08IpNetmask[1], (int)pSettings->u08IpNetmask[2], (int)pSettings->u08IpNetmask[3]);
			fprintf(fpNetwork, "gateway %d.%d.%d.%d\n", (int)pSettings->u08IpGateway[0], (int)pSettings->u08IpGateway[1], (int)pSettings->u08IpGateway[2], (int)pSettings->u08IpGateway[3]);
		}

		fclose(fpNetwork);
	}

	 return 1;
}

/************************************************************************
*/
/*! \brief Read Network Settings from File and start the connection
*
*   \param cPath --> Path to network config file
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int network_Up(void)
{

	FILE *fp;

	fp = popen("ifup wlan0\r\n", "r+");

	/* pipe successfully opened? */
	if(fp != NULL)
	{
		fclose(fp);
	}
	else
	{
		printf("ifup wlan0 error\r\n");
		return -1;
	}

	sleep(1);

	return 1;
}


/************************************************************************
*/
/*! \brief Switch off the wlan network
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int network_Down(void)
{
	FILE *fp;

	/* create fifo in temp */
	fp = popen("ifdown wlan0 ", "r+");

	/* pipe successfully opened? */
	if(fp != NULL)
	{
		fclose(fp);
	}
	else
	{
		printf("ifup wlan0 error\r\n");
		return -1;
	}

	return 1;
}


