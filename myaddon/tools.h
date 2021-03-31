#ifndef TOOLS_H_
#define TOOLS_H_

#include <string.h>
#include <iostream>
using namespace std;
#include "../libSHtransportationcard/stru.h"


//#define _YYB_DEBUG
#ifdef _YYB_DEBUG
#define debug(...) {printf("[debug]  "); printf(__VA_ARGS__);}
#else
#define debug(...)
#endif


enum WarnOption{
	WO_SAVE2FILE		= 1,
	WO_MSG_SYS			= 2,
	WO_MSG_CASHIN		= 4,
	WO_MSG_CASHOUT		= 8,
	WO_MSG_CASHOUTTEN	= 16,
	WO_MSG_COININ		= 32,
	WO_MSG_COINOUTHALF	= 64,
	WO_MSG_COINOUTONE	= 128,
	WO_MSG_DB			= 256
};


/*int write_log(char*data,...);
#define warn(...) \
	do{\
	std::string temp;\
	char buf[40960] = {0};\
	sprintf(buf, __VA_ARGS__);\
	temp += buf;\
	temp += "          ";\
	temp += "{";\
	temp += __FUNCTION__;\
	temp += "}";\
	write_log("%s", temp.c_str());\
	}while(0)*/

#define MINFUNCTION(a, b) (a>b ? b : a)


string put_out_hex(unsigned char *data, int len);
#define hex_str(data, len)	(put_out_hex((unsigned char *)data, len).c_str())
void htonl(unsigned long host, char *net);
void ntohs1(unsigned char *net, unsigned short &host);

string get_EachModule_Initinfo(int err_code);
string get_CoinModule_Inifinfo(int code);
string getEcardCodeInfo(int ret, unsigned char code);

//lock函数指针类型
typedef int(*pKT_IELECTLOCK_init)(const char *i_arrPort, const char* logpath); 
typedef int(*pKT_IELECTLOCK_send_Unlocking)(); 
typedef void(*pKT_IELECTLOCK_close_serial)(); 
typedef void(*pKT_IELECTLOCK_cleanUp)();

typedef unsigned char byte;
typedef void (* icCard_poll_event_handler_t)(char* blockData);
//rfm
typedef bool(*pKT_IRFM_operateAntenna)(bool open); 
typedef char*(*pKT_IRFM_searchCard)(int type); 
typedef std::string(*pKT_RFM_preventConflict)(); 
typedef bool(*pKT_RFM_beer)(byte type); 
typedef std::string(*pKT_RFM_selectCard)(const std::string&serial); 
typedef bool(*pKT_RFM_verifyPassWord)(int type, byte blockNum); 
typedef std::string(*pKT_RFM_readBlock)(byte blockNum); 
typedef void(*pKT_IRFM_cleanUp)(); 
typedef void(*pKT_IRFM_Disable)(byte blockNum); 
typedef void(*pKT_IRFM_Enable)(byte blockNum); 
typedef int(*pKT_IRFM_init)(int port, const char *i_arrPort, byte **keyA_, byte **keyB_, icCard_poll_event_handler_t handler, const char* logpath); 
//rpd
typedef bool(*pKT_IRPD_Request)(int type, char *cardtype); 
typedef bool(*pKT_IRPD_Anticoll)(char *uid); 
typedef bool(*pKT_IRPD_Select)(char *i_uid); 
typedef bool(*pKT_IRPD_VerifyPassWord)(int type, byte blockNum); 
typedef bool(*pKT_IRPD_readBlock)(byte blockNum); 
typedef bool(*pKT_IRPD_Beep)(int time); 
typedef bool(*pKT_IRPD_cleanUp)(); 
typedef bool(*pKT_IRPD_init)(int port, const char *i_arrPort, byte **keyA_, byte **keyB_, icCard_poll_event_handler_t handler, const char* logpath); 
typedef bool(*pKT_IRPD_Disable)(byte blockNum); 
typedef bool(*pKT_IRPD_Enable)(byte blockNum); 
//MF800
typedef int (__stdcall *pKT_rf_init_com)(int port, long baud);
typedef int (__stdcall *pKT_rf_init_device_number)(unsigned short icdev);
typedef int (__stdcall *pKT_rf_get_device_number)(unsigned short *pIcdev);
typedef int (__stdcall *pKT_rf_light)(unsigned short icdev, unsigned char color);
typedef int (__stdcall *pKT_rf_antenna_sta)(unsigned short icdev, unsigned char model);
typedef int (__stdcall *pKT_rf_request)(unsigned short icdev, unsigned char model, unsigned short *pTagType);
typedef int (__stdcall *pKT_rf_anticoll)(unsigned short icdev, unsigned char bcnt, unsigned char *pSnr, unsigned char *pLen);
typedef int (__stdcall *pKT_rf_select)(unsigned short icdev, unsigned char *pSnr, unsigned char snrLen, unsigned char *pSize);
typedef int (__stdcall *pKT_rf_halt)(unsigned short icdev);
typedef int (__stdcall *pKT_rf_M1_authentication2)(unsigned short icdev, unsigned char model, unsigned char block, unsigned char *pKey);
typedef int (__stdcall *pKT_rf_M1_read)(unsigned short icdev, unsigned char block, unsigned char *pData, unsigned char *pLen);
typedef int (__stdcall *pKT_rf_M1_write)(unsigned short icdev, unsigned char block, unsigned char *pData);
typedef int (__stdcall *pKT_rf_M1_initval)(unsigned short icdev, unsigned char block, long value);
typedef int (__stdcall *pKT_rf_M1_readval)(unsigned short icdev, unsigned char block, long *pValue);
typedef int (__stdcall *pKT_rf_M1_decrement)(unsigned short icdev, unsigned char block, long value);
typedef int (__stdcall *pKT_rf_M1_increment)(unsigned short icdev, unsigned char block, long value);
typedef int (__stdcall *pKT_rf_M1_restore)(unsigned short icdev, unsigned char block);
typedef int (__stdcall *pKT_rf_M1_transfer)(unsigned short icdev, unsigned char block);
typedef int (__stdcall *pKT_rf_ClosePort)(void);
//traffic 
typedef int(*pKT_Itcard_init)(int port, const char *i_arrPort, const char* logpath, char industrycode, char* usercode, char deviceid, char *units, char* devicetime); 
typedef void(*pKT_Itcard_cleanUp)(); 
typedef int(*pKT_Itcard_readCard)(DATACARD &dc); 
typedef int(*pKT_Itcard_readCard_pr)(char rfu, READCARDPR &pr);
typedef int(*pKT_Itcard_operateCard)(char *datetime, char *serial, int money, int updateFlag, TRADEINFO &ti);

//printer
typedef void(*pKT_lib_printer_end)(); 
typedef bool(*pKT_lib_printer_init)(int port, const char *i_arrPort, int br, const char* logpath); 
typedef int(*pKT_lib_printer_get_status)(); 
typedef int(*pKT_lib_printer_cut)(int fullCut); 
typedef int(*pKT_lib_printer_set_strong)(bool bold); 
typedef int(*pKT_lib_printer_set_align)(int type); 
typedef int(*pKT_lib_printer_set_size)(int size); 
typedef int(*pKT_lib_printer_println)(char *content); 
typedef int(*pKT_lib_printer_printBarcode)(int epsontype, int height, int width, int pos, int align, char *content); 
typedef int(*pKT_lib_printer_printQr)(int Epsonver, char *content, int size, int errLevel); 
#endif

