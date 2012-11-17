/*! \file Settings.c
*/

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   I N C L U D E
*************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mytypes.h"

#include "Station.h"
#include "Network.h"
#include "Menue.h"
#include "Settings.h"


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
/*! \fn int Network_ReadSettings(char* cPath, NETWORK_SETTINGS *pSettings)
*
*   \brief read the network settings like ip subnetmask and dhcp settings
*          from a giveb  config file
*
*   \param cPath --> Path to config file
*   \param pSettings --> Pointer to settings for the network
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int Settings_Read(char* cPath, WEBRADIO *pWebRadio)
{
	#define SETTINGS_INPUT_SIZE 128

	int iValue, iSize;
	static FILE *fpSettings;

	/* open file */
	fpSettings = fopen(cPath, "r");

	/* if open failed create default file */
	if(fpSettings > 0)
	{
		/* if seek failed return with an error */
		if(fseek(fpSettings, 0, SEEK_SET) < 0) return -1;

		iSize = fscanf(fpSettings, "volume=%d\n", &iValue);
		if(iSize == 0) return -1;

		pWebRadio->u08Volume = (INT_U8)iValue;

		iSize = fscanf(fpSettings, "station=%d\n", &iValue);
		if(iSize == 0) return -1;

		pWebRadio->u08Station = (INT_U8)iValue;

		fclose(fpSettings);

		return 1;
	}

	return -1;
}


/************************************************************************
*/
/*! \fn int Network_WriteSettings(char* cPath, NETWORK_SETTINGS *pSettings)

*   \brief write the network settings like ip subnetmask and dhcp settings to config file
*
*   \param cPath --> Path to config file
*   \param pSettings --> Pointer to settings for the network
*
*   \return int --> -1 no error, 1 write settings
*
*************************************************************************/
int Settings_Write(char* cPath, WEBRADIO *pWebRadio)
{
	static FILE *fpSettings;

	fpSettings = fopen(cPath, "w");

	/* open file */
	if(fpSettings > 0)
	{

		/* if seek failed return with an error */
		if(fseek(fpSettings, 0, SEEK_SET) < 0) return -1;

		fprintf(fpSettings, "volume=%d\n", (int)pWebRadio->u08Volume);

		fprintf(fpSettings, "station=%d\n", (int)pWebRadio->u08Station);

		fclose(fpSettings);
	}
	else
	{
		return -1;
	}

	return 1;
}


#ifdef __cplusplus
   }
#endif

