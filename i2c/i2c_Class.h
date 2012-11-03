
#if !defined(__I2C_CLASS_H__)
#define __I2C_CLASS_H__



/*************************************************************************
*   D E F I N E
*************************************************************************/

/*************************************************************************
*   D E C L A R A T I O N
*************************************************************************/

class i2c_Class
{
	public:
	i2c_Class();
	virtual ~i2c_Class();

	int Init(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size);
	int Read(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size);
	int Write(CHAR* cDevice, INT_U16 u16Address, INT_U8* p08Data, INT_U16 u16Size);

	protected:
	private:
};

#endif

/*** EOF ***/

