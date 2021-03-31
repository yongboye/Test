//ICT
#ifndef CASH_IN_V77E_H__
#define CASH_IN_V77E_H__

#include "sdkaddon.h"
#include "cashin.h"

typedef int(*pKT_v77e_enable)(); 
typedef int(*pKT_v77e_disable)(); 
typedef int(*pKT_v77e_getStatus)(); 
typedef int(*pKT_v77e_reset)(); 
typedef int(*pKT_v77e_set_notes_accept)(unsigned short notes); 
typedef int(*pKT_v77e_getSoftInformation)(char* o_info); 
typedef int(*pKT_v77e_programFirmware)(const char* name); 
typedef int(*pKT_v77e_init)(const char* port, const char* logpath); 
typedef void(*pKT_v77e_bind_poll_event_handler)(v77e_poll_event_handler_t handler); 
typedef void(*pKT_v77e_close_serial)(); 
typedef void(*pKT_v77e_poll)(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int *sendStatus); 

class cash_in_v77e : public Icash_in
{
public:
	cash_in_v77e(cash_event_callback_t cash_event_handler_v77e, const char* port, int accept_);
	virtual ~cash_in_v77e();

	virtual char* test();
	virtual void destructObj();
	virtual char *cash_in_enable();
	virtual char *cash_in_disable();	
	virtual char *cash_in_reset();
	virtual char *cash_in_check();
	virtual char *cash_in_set_notes_accept(int cash_type, bool accept, bool finish);
	virtual char *cash_in_get_Dataset_Version();
	virtual char *cash_in_get_Firmware_Version();
	virtual char *cash_in_upgrade_Firmware_Program(const char *fileName, const char *i_arrPort);
	virtual void setParam(void *param);
	
	virtual int cash_in_init(const char* logpath, const char* path);	
	virtual void cash_in_closeport();
	virtual bool cash_in_sync1(bool bRead);
	virtual bool cash_in_poll1();
	virtual void cash_in_poll2(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int* sendStatus);
	
	static void cash_in_v77e_callback(unsigned char event_type, int param, int len);
	
	string getErrorMsg(int errcode);
	
	int cash_in_loadDll(const char* path);
	BOOL cash_in_FreeLib();

private:
	static cash_in_v77e *pThis; 
	char v77e_ret_buf[1024];
	cash_event_callback_t					cash_event_handler_v77e;
	int m_repeat_event;
	unsigned char m_last_event_type;
	char comPort[100];
	int m_cash_accept_1;
	DEVPARAMS *devParam;
	
	HINSTANCE hDll;
	pKT_v77e_enable 					kt_v77e_enable;
	pKT_v77e_disable					kt_v77e_disable;
	pKT_v77e_getStatus					kt_v77e_getStatus;
	pKT_v77e_reset						kt_v77e_reset;
	pKT_v77e_set_notes_accept			kt_v77e_set_notes_accept;
	pKT_v77e_getSoftInformation			kt_v77e_getSoftInformation;
	pKT_v77e_programFirmware			kt_v77e_programFirmware;
	pKT_v77e_init						kt_v77e_init;
	pKT_v77e_bind_poll_event_handler	kt_v77e_bind_poll_event_handler;
	pKT_v77e_close_serial				kt_v77e_close_serial;
	pKT_v77e_poll						kt_v77e_poll;
};

#endif