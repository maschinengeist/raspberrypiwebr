#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <termios.h>

#if 0
class SerialCom
{
	public:
		SerialCom();
		virtual ~SerialCom();
		int Open(char *cPath, speed_t Speed);
		int Write(char* cBuffer, int iSize);
		int Read(char* cBuffer, int iSize);
		int Close(void);

	protected:
	private:

};
#endif
#endif // SERIALCOM_H
