#ifndef SENSOR_H__
#define SENSOR_H__

typedef void(*ticketsensor_stus_event_handler_t)(void* extra, int method, char* stusData);
typedef void(*ticket_sensor_stus_event_handler_t)(int method, char* stusData);

class Isensor
{
public:
	Isensor(){};
	virtual ~Isensor() {};
	
	virtual char *sensor_enable() = 0;
	virtual char *sensor_disable() = 0;	
	virtual char *sensor_get_Status() = 0;
	virtual void setParam(void *param) = 0;
	virtual void sensor_read_Status() = 0;
	virtual void sensor_closeport() = 0;
	virtual int  sensor_init(const char* logpath, const char* path) = 0;
};

#endif