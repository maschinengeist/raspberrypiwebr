/*! \file amixer.c
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
#include "amixer.h"


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
/*! \fn int amixer_Volume(INT_U8 u08Device, INT_U8 u08VolumeProzent)

*   \brief set volume with amixer in percent
*
*   \param u08Device --> Number of control
*
*   \param u08VolumePercent --> Volume from 0 to 100
*
*   \return int --> -1 volume set error, 1 volume is set
*
*************************************************************************/
int amixer_Volume(INT_U8 u08Device, INT_U8 u08VolumePercent)
{
	FILE* fp;
   char cVolume[64];

   /* linit volume to 100 */
   if(u08VolumePercent > 100) u08VolumePercent = 100;

	sprintf(cVolume, "amixer -c %d -- sset Speaker Playback Volume %d\%\r\n", (int)u08Device, (int)u08VolumePercent);

	/* create a pipe for sending volume to amixer */
	fp = popen(cVolume, "r");

	/* pipe successfully open */
	if(fp != NULL)
	{
		fclose(fp);
	}
	else
	{
		printf("set volume error\r\n");
		return -1;
	}

	return 1;
}

/************************************************************************
*/
/*! \fn int amixer_Volume(INT_U8 u08Device, INT_U8 u08VolumeProzent)

*   \brief set volume with amixer in percent
*
*   \param u08Device --> Number of control
*
*   \param u08VolumeRight --> Right volume from 0 to 100
*
*   \param u08VolumeLeft --> Right volume from 0 to 100
*
*   \return int --> -1 volume set error, 1 volume is set
*
*************************************************************************/
int amixer_Balance(INT_U8 u08Device, INT_U8 u08VolumeRight, INT_U8 u08VolumeLeft)
{
	FILE *fp;
   char cVolume[64];

   /* linit volume to 100 */
   if(u08VolumeRight > 100) u08VolumeRight = 100;
	if(u08VolumeLeft > 100)  u08VolumeLeft  = 100;

	sprintf(cVolume, "amixer -c %d sset Line,0 %d\%,%d\% unmute cap", (int)u08Device, (int)u08VolumeRight, (int)u08VolumeLeft);

	/* create a pipe for sending volume to amixer */
	fp = popen(cVolume, "r");

	/* pipe successfully open */
	if(fp != NULL)
	{
		fclose(fp);
	}
	else
	{
		printf("set volume error\r\n");
		return -1;
	}

	return 1;
}

#ifdef __cplusplus
   }
#endif

