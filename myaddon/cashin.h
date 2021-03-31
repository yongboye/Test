//纸币接收器接口类

#ifndef CASH_IN_H__
#define CASH_IN_H__

//standard、JUNPENG
typedef void (*cash_event_callback_t)(void* extra, int cash_in_event_code, const char *event_code_description, int money, int len);
typedef void (*nv9_poll_event_handler_t)(unsigned char event_type, int param, int len);
typedef void (*v77e_poll_event_handler_t)(unsigned char event_code, int money, int currency);

class Icash_in
{
public:
	Icash_in(){};
	virtual ~Icash_in() {};
	
	virtual char* test() = 0;
	virtual void destructObj() = 0;
	virtual char *cash_in_enable() = 0;
	virtual char *cash_in_disable() = 0;	
	virtual char *cash_in_reset() = 0;	
	virtual char *cash_in_check() = 0;
	virtual char *cash_in_set_notes_accept(int cash_type, bool accept, bool finish) = 0;
	virtual char *cash_in_get_Dataset_Version() = 0;
	virtual char *cash_in_get_Firmware_Version() = 0;
	virtual char *cash_in_upgrade_Firmware_Program(const char *fileName, const char *i_arrPort) = 0;
	virtual void setParam(void *param) = 0;
	virtual int   cash_in_init(const char* logpath, const char* path) = 0;
	virtual void  cash_in_closeport() = 0;	
	virtual bool cash_in_sync1(bool bRead) = 0;
	virtual bool cash_in_poll1() = 0;
	virtual void cash_in_poll2(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int* sendStatus) = 0;
};

#endif

