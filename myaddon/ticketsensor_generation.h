//ICT
#ifndef SENSOR_GENERATION_H__
#define SENSOR_GENERATION_H__

#include "sdkaddon.h"
#include "sensor.h"

typedef int(*pKT_ITICKET_init)(const char *i_arrPort, ticket_sensor_stus_event_handler_t handler, const char* logpath); 
typedef int(*pKT_ITICKET_enable)(bool benable); 
typedef int(*pKT_ITICKET_send_GetStus)(char* o_bcdstr); 
typedef void(*pKT_ITICKET_read_GetStus)(); 
typedef void(*pKT_ITICKET_close_serial)(); 
typedef void(*pKT_ITICKET_cleanUp)(); 

class sensor_generation : public Isensor
{
public:
	sensor_generation(ticketsensor_stus_event_handler_t sensor_event_handler_generation, const char* port);
	sensor_generation();
	virtual ~sensor_generation();

	virtual char *sensor_enable();
	virtual char *sensor_disable();	
	virtual char *sensor_get_Status();
	virtual void setParam(void *param);
	virtual void sensor_read_Status();
	virtual void sensor_closeport();
	virtual int  sensor_init(const char* logpath, const char* path);

	int sensor_loadDll(const char* path);
	BOOL sensor_FreeLib();
	
private:
	static sensor_generation *pThis; 
	static void sensor_callback(int method, char* stusData);
	ticketsensor_stus_event_handler_t					sensor_event_handler_generation;
	char comPort[100];
	DEVPARAMS *devParam;
	char sensor_ret_buf[1024];
	
	HINSTANCE hDll;
	pKT_ITICKET_init			kt_ITICKET_init;
	pKT_ITICKET_enable	 		kt_ITICKET_enable;
	pKT_ITICKET_send_GetStus  	kt_ITICKET_send_GetStus;
	pKT_ITICKET_read_GetStus	kt_ITICKET_read_GetStus;
	pKT_ITICKET_close_serial	kt_ITICKET_close_serial;
	pKT_ITICKET_cleanUp			kt_ITICKET_cleanUp;
};

#endif