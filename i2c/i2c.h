
#if !defined(__I2C_H__)
#define __I2C_H__

#ifdef __cplusplus
	extern "C" {
#endif

/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   D E C L A R A T I O N
*************************************************************************/

int i2c_Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size);

int i2c_Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size);

#ifdef __cplusplus
	}
#endif

#endif

/*** EOF ***/

