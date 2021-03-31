//ITL

#ifndef CASH_IN_JUNPENG_H__
#define CASH_IN_JUNPENG_H__

#include "sdkaddon.h"
#include "cashin.h"


typedef bool(*pKT_nv9_enable)(); 
typedef bool(*pKT_nv9_disable)(); 
typedef int(*pKT_nv9_getStatus)(); 
typedef bool(*pKT_nv9_reset)(); 
typedef bool(*pKT_nv9_set_notes_accept)(unsigned short notes); 
typedef bool(*pKT_nv9_getDatasetVersion)(char* oDatasetVer); 
typedef bool(*pKT_nv9_getFirmwareVersion)(char* oFirmwareVer); 
typedef bool(*pKT_nv9_programFirmware)(const char* name, int dwPort, const char *i_arrPort); 
typedef bool(*pKT_nv9_init)(int dwPort, const char *i_arrPort, const char* logpath); 
typedef void(*pKT_nv9_close_serial)(); 
typedef bool(*pKT_nv9_sync2)(bool bRead); 
typedef bool(*pKT_nv9_poll2)(); 
typedef void(*pKT_nv9_bind_poll_event_handler)(nv9_poll_event_handler_t handler); 
	
	
class cash_in_junpeng : public Icash_in
{
public:	
	cash_in_junpeng(cash_event_callback_t cash_event_handler_junpeng, const char* port, int accept_);
	virtual ~cash_in_junpeng();

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
	

	/******************库内部使用******************/	
	virtual int cash_in_init(const char* logpath, const char* path);	
	virtual void cash_in_closeport();
	virtual bool cash_in_sync1(bool bRead);
	virtual bool cash_in_poll1();
	virtual void cash_in_poll2(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int* sendStatus);
	static void cash_in_nv9_callback(unsigned char event_type, int param, int len); //->cash_event_handler_junpeng
	
	string getErrorMsg(int errcode);
	
	int cash_in_loadDll(const char* path);
	BOOL cash_in_FreeLib();

private:
	static cash_in_junpeng *pThis; //单例
	char junpeng_ret_buf[1024];	
	cash_event_callback_t cash_event_handler_junpeng;	
	int m_repeat_event;
	unsigned char m_last_event_type;
	char comPort[100];
	int m_cash_accept_1;
	DEVPARAMS *devParam;
	
	HINSTANCE hDll;
	pKT_nv9_enable 					kt_nv9_enable;
	pKT_nv9_disable					kt_nv9_disable;
	pKT_nv9_getStatus				kt_nv9_getStatus;
	pKT_nv9_reset					kt_nv9_reset;
	pKT_nv9_set_notes_accept		kt_nv9_set_notes_accept;
	pKT_nv9_getDatasetVersion		kt_nv9_getDatasetVersion;
	pKT_nv9_getFirmwareVersion		kt_nv9_getFirmwareVersion;
	pKT_nv9_programFirmware			kt_nv9_programFirmware;
	pKT_nv9_init					kt_nv9_init;
	pKT_nv9_close_serial			kt_nv9_close_serial;
	pKT_nv9_sync2					kt_nv9_sync2;
	pKT_nv9_poll2					kt_nv9_poll2;
	pKT_nv9_bind_poll_event_handler kt_nv9_bind_poll_event_handler;
};

#endif