#include "cash_in_v77e.h"
#include <string.h>
#include "tools.h"
#include "Commands.h"
#include <json/json.h>
//#include <ICTcashin.h> //2021-03-09

cash_in_v77e* cash_in_v77e::pThis = NULL;

cash_in_v77e::cash_in_v77e(cash_event_callback_t cash_event_handler_v77e, const char* port, int accept_)
{
	devParam = NULL;
	pThis = this;
	v77e_ret_buf[0] = 0;
	this->cash_event_handler_v77e = cash_event_handler_v77e;
	m_repeat_event = 0;
	m_last_event_type = 0;
	strcpy(comPort, port);	
	m_cash_accept_1 = accept_;
}
cash_in_v77e::~cash_in_v77e()
{
	/*if(devParam != NULL){
		if(!devParam->exit) devParam->exit = true;
	}
	v77e_end();*/
	cash_in_FreeLib();
}

void cash_in_v77e::destructObj()
{
	/*if(devParam != NULL){
		if(!devParam->exit) devParam->exit = true;
	}
	v77e_end();*/
}

char* cash_in_v77e::test()
{
	return "hello cash_in_v77e";
}

char *cash_in_v77e::cash_in_enable()
{
	debug("cash_in_v77e::cash_in_enable run\n");

	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	int enable_flag = kt_v77e_enable();
	if (!enable_flag)
	{
		lib_info = "ʹ��ֽ�ҽ�����ʧ�ܣ�";
		root["result_code"] = -7;
		root["result_msg"] = lib_info;
	}
	else
	{
		if(devParam != NULL)
			devParam->enable = true;
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_enable����:%s", v77e_ret_buf);

	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_disable()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	int enable_flag = kt_v77e_disable();
	if (!enable_flag)
	{
		lib_info = "�ر�ֽ�ҽ�����ʧ��";
		root["result_code"] = -8;
		root["result_msg"] = lib_info;
	}
	else
	{
		if(devParam != NULL)
			devParam->enable = false;
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_disable����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_check()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	int ret = kt_v77e_getStatus();
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["error_code"] = ret;
		root["error_msg"] = getErrorMsg(ret);
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_check����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_reset()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";
	int reset_flag = kt_v77e_reset();
	if (!reset_flag)
	{
		lib_info = "��λֽ�ҽ�����ʧ��";
		root["result_code"] = -53;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_reset����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_set_notes_accept(int cash_type, bool accept, bool finish)
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";

	unsigned short channel = (unsigned short)(cash_type & 0xffff);
	int channel_flag = kt_v77e_set_notes_accept(channel);
	if (!channel_flag)
	{
		lib_info = "����ֽ�ҽ��յ����ʧ��";
		root["result_code"] = -9;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_set_notes_accept����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_get_Dataset_Version()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";

	char ver[100] = { 0 };
	int ret = 0; 
	if (!ret)
	{
		lib_info = "��ȡ���ݰ��汾ʧ��";
		root["result_code"] = -48;
		root["result_msg"] = lib_info;
		root["result_version"] = "";
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["result_version"] = ver;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_get_Dataset_Version����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_get_Firmware_Version()
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";

	char ver[100] = { 0 };
	int ret = kt_v77e_getSoftInformation(ver); //2019-11-29
	if (!ret)
	{
		lib_info = "��ȡ�̼��汾ʧ��";
		root["result_code"] = -49;
		root["result_msg"] = lib_info;
		root["result_version"] = "";
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
		root["result_version"] = ver;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_get_Firmware_Version����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

char *cash_in_v77e::cash_in_upgrade_Firmware_Program(const char *fileName, const char *i_arrPort)
{
	Json::Value root;
	Json::FastWriter write;
	std::string lib_info = "";

	int ret = kt_v77e_programFirmware(fileName);
	if (!ret)
	{
		lib_info = "�����̼�����ʧ��";
		root["result_code"] = -50;
		root["result_msg"] = lib_info;
	}
	else
	{
		lib_info = "success";
		root["result_code"] = 0;
		root["result_msg"] = lib_info;
	}
	std::string strWriter = "";
	strWriter = write.write(root);
	strcpy(v77e_ret_buf, strWriter.c_str());
	debug("cash_in_upgrade_Firmware_Program����:%s", v77e_ret_buf);
	return v77e_ret_buf;
}

void cash_in_v77e::setParam(void *param)
{
	devParam = (DEVPARAMS *)param;
}

int cash_in_v77e::cash_in_init(const char* logpath, const char* path)
{
	//warn(WO_MSG_CASHIN, "cash_in_init��ʼ����ʼ");

	//�ڶ������򿪴���
	int ret = cash_in_loadDll(path);
	if(ret == 0){
		return 0;
	}else if(ret == -1){
		return 0;
	}
	int res = kt_v77e_init(comPort, logpath);
	if (res)
	{
		//��ʼ���ɹ�
		//warn(WO_MSG_CASHIN, "cash_in_init��ʼ���򿪴���%d�ɹ�", serial);
		//��libv77e��Ҫ���õĻص�
		//cout<<"��libv77e��Ҫ���õĻص�"<<endl;
		kt_v77e_bind_poll_event_handler(cash_in_v77e_callback);
	}
	else
	{
		//��ʼ��ʧ��
		//warn(WO_MSG_CASHIN, "cash_in_init��ʼ��ʧ�ܣ�����%d", serial);
		return 0;
	}
	unsigned short note_accept = (unsigned short)m_cash_accept_1;
	debug("�򿪴���%d�ɹ�, note_accept=%d\n", serial, note_accept);
	res = kt_v77e_set_notes_accept(note_accept);
	if (res)
	{
		//��ʼ���ɹ�
		//warn(WO_MSG_CASHIN, "cash_in_init��ʼ���ɹ�������ֽ�ҽ������%d", note_accept);
	}
	else
	{
		//��ʼ��ʧ��
		//warn(WO_MSG_CASHIN, "cash_in_init��ʼ��ʧ�ܣ�����ֽ�ҽ������%dʧ��", note_accept);
		return 0;
	}
	return res;
}

void cash_in_v77e::cash_in_closeport()
{
	kt_v77e_close_serial();
}

bool cash_in_v77e::cash_in_sync1(bool bRead)
{
	return false;
}

bool cash_in_v77e::cash_in_poll1()
{
	return false;
}

void cash_in_v77e::cash_in_poll2(int *needLen, int *recvLen, int *recvLenLst, unsigned char* recvData, int* sendStatus)
{
	return kt_v77e_poll(needLen, recvLen, recvLenLst, recvData, sendStatus);
}

int cash_in_v77e::cash_in_loadDll(const char* path)
{
	char buf[1024];
	sprintf(buf, "%s/libICTcashin.dll", path);
	hDll = LoadLibrary(buf);
	if (hDll == NULL)
	{
		debug("������libICTcashin.dll");
		return 0;
	}

	kt_v77e_enable = (pKT_v77e_enable)GetProcAddress(hDll, "v77e_enable");
	kt_v77e_disable = (pKT_v77e_disable)GetProcAddress(hDll, "v77e_disable");
	kt_v77e_getStatus = (pKT_v77e_getStatus)GetProcAddress(hDll, "v77e_getStatus");
	kt_v77e_reset = (pKT_v77e_reset)GetProcAddress(hDll, "v77e_reset");
	kt_v77e_set_notes_accept = (pKT_v77e_set_notes_accept)GetProcAddress(hDll, "v77e_set_notes_accept");
	kt_v77e_getSoftInformation = (pKT_v77e_getSoftInformation)GetProcAddress(hDll, "v77e_getSoftInformation");
	kt_v77e_programFirmware = (pKT_v77e_programFirmware)GetProcAddress(hDll, "v77e_programFirmware");
	kt_v77e_init = (pKT_v77e_init)GetProcAddress(hDll, "v77e_init");
	kt_v77e_bind_poll_event_handler = (pKT_v77e_bind_poll_event_handler)GetProcAddress(hDll, "v77e_bind_poll_event_handler");
	kt_v77e_close_serial = (pKT_v77e_close_serial)GetProcAddress(hDll, "v77e_close_serial");
	kt_v77e_poll = (pKT_v77e_poll)GetProcAddress(hDll, "v77e_poll");
	if (!kt_v77e_enable || !kt_v77e_disable || !kt_v77e_getStatus || !kt_v77e_reset || !kt_v77e_set_notes_accept || !kt_v77e_getSoftInformation ||
	    !kt_v77e_programFirmware || !kt_v77e_init || !kt_v77e_bind_poll_event_handler || !kt_v77e_close_serial || !kt_v77e_poll)
	{
		debug("����libICTcashin.dll�⺯��ʧ�ܣ�");
		return -1;
	}
	return 1;
}
BOOL cash_in_v77e::cash_in_FreeLib()
{
	if (hDll != NULL)
	{
		return FreeLibrary(hDll);
	}
	return FALSE;
}

//��libv77e����,libv77e�������Ĳ�������(param*100)��c#�Ļص�
void cash_in_v77e::cash_in_v77e_callback(unsigned char event_type, int param, int len)
{
	char display[100];
	sprintf(display, "0x%02x(%d)", event_type, event_type);
	//cout<<"cash_in_v77e_callback event:" << display << " money:" << param << " type:" << len <<endl;
	int out_event_type = (char)event_type;
	int out_money = param;
	std::string out_event_msg;
	bool bUnknown = false;
	switch (out_event_type)
	{
	case -43:
	case 213:
		out_event_msg = "Credit Note";
		out_event_type = -43;
		break;
	case -47:
	case 209:
		out_event_msg = "Note Stacked";
		out_event_type = -47;
		break;
	case -45:
	case 211:
		out_event_msg = "Note Rejected";
		out_event_type = -45;
		break;
	case -48:
	case 208:
		out_event_msg = "Safe Note Jam";
		out_event_type = -48;
		break;
	case -81:
	case 175:
		out_event_msg = "Motor failure";
		out_event_type = -81;
		break;
	case -82:
	case 174:
		out_event_msg = "Checksum error";
		out_event_type = -82;
		break;
	case -83:
	case 173:
		out_event_msg = "Bill jam";
		out_event_type = -83;
		break;
	case -84:
	case 172:
		out_event_msg = "Bill remove";
		out_event_type = -84;
		break;
	case -85:
	case 171:
		out_event_msg = "Stacker open";
		out_event_type = -85;
		break;
	case -86:
	case 170:
		out_event_msg = "Sensor problem";
		out_event_type = -86;
		break;
	case -87:
	case 169:
		out_event_msg = "Bill fish";
		out_event_type = -87;
		break;
	case -88:
	case 168:
		out_event_msg = "Stacker problem";
		out_event_type = -88;
		break;
	case -89:
	case 167:
		out_event_msg = "Bill reject";
		out_event_type = -89;
		break;
	case -90:
	case 166:
		out_event_msg = "Invalid command";
		out_event_type = -90;
		break;
	case -91:
	case 165:
		out_event_msg = "reserved";
		out_event_type = -91;
		break;
	case -92:
	case 164:
		out_event_msg = "Response when error status is exclusion";
		out_event_type = -92;
		break;
	case -93:
	case 163:
		out_event_msg = "Bill acceptor enable status";
		out_event_type = -93;
		break;
	case -94:
	case 162:
		out_event_msg = "Bill acceptor inhibit status";
		out_event_type = -94;
		break;
	case -95:
	case 161:
		out_event_msg = "Bill acceptor busy";
		out_event_type = -95;
		break;
	case -96:
	case 160:
		out_event_msg = "Power supply on communication";
		out_event_type = -96;
		break;
	default:
		bUnknown = true;
		break;
	}
	if (bUnknown) {
		//cout<<"unknown out_event_type:"<<display<<endl;
		return;
	}
	char msg[100] = { 0 };
	strcpy(msg, out_event_msg.c_str());

	//���е�Ǯ�ĵ�λ���Ƿ�
	out_money *= 100;

	//�����ظ��¼�
	if (pThis->m_last_event_type == event_type && param == 0)
	{
		pThis->m_repeat_event++;
	}
	else
	{
		pThis->m_repeat_event = 0;
	}
	if (pThis->m_repeat_event >= 1)
	{
		//���ظ��¼�
		if (pThis->m_repeat_event == 1)
		{
			debug("callback: ...\n");
			//warn(WO_MSG_CASHIN, "callback: ...");
		}
	}
	else
	{
		//�����ظ��¼�
		//warn(WO_MSG_CASHIN, "callback: event:%s money:%d type:0X%02X(%d)", msg, out_money, event_type, out_event_type);
		debug("callback: event:%s money:%d type:0X%02X(%d)\n", msg, out_money, event_type, out_event_type);
	}
	pThis->m_last_event_type = event_type;



	if (pThis->cash_event_handler_v77e != NULL)
	{
		//����c#Ӧ�ó���Ļص�
		cout<<"out_event_type="<<out_event_type<<endl;
		(*pThis->cash_event_handler_v77e)(pThis->devParam, out_event_type, msg, out_money, len);
	}
	else
	{
		//warn(WO_MSG_CASHIN, "Ӧ�ó���û��ע��ص�����");
	}
	return;
}

//-100����������ʧ�� -101���̼������� -1δ֪״̬ -2��ʼ��ʧ�� -110���ڴ򿪳ɹ� -111���ڴ�ʧ�� -116�رմ��� -112����ʧ�� -114����ʧ�� 
//-43��� -47������� -45�˱ҽ��� -48���� -81������� -82����� -83bill jam -84bill remove -85Stacker open -86Sensor problem -87Bill fish -88Stacker problem -89Bill reject
//-90Invalid command -91reserved -92Response when error status is exclusion -93enable -94inhibit -95busy -96power on
string cash_in_v77e::getErrorMsg(int errcode)
{
	string str = "";
	switch (errcode) {
	case -100:
		str = "��������ʧ��";
		break;
	case -101:
		str = "�̼�������";
		break;
	case -1:
		str = "δ֪״̬";
		break;
	case -2:
		str = "��ʼ��ʧ��";
		break;
	case -110:
		str = "���ڴ򿪳ɹ�";
		break;
	case -111:
		str = "���ڴ�ʧ��";
		break;
	case -116:
		str = "�رմ���";
		break;
	case -112:
		str = "����ʧ��";
		break;
	case -114:
		str = "����ʧ��";
		break;
	case -43:
		str = "���";
		break;
	case -47:
		str = "�������";
		break;
	case -45:
		str = "�˱ҽ���";
		break;
	case -48:
		str = "����";
		break;
	case -81:
		str = "�������";
		break;
	case -82:
		str = "�����";
		break;
	case -83:
		str = "bill jam";
		break;
	case -84:
		str = "bill remove";
		break;
	case -85:
		str = "Stacker open";
		break;
	case -86:
		str = "Sensor problem";
		break;
	case -87:
		str = "Bill fish";
		break;
	case -88:
		str = "Stacker problem";
		break;
	case -89:
		str = "Bill reject";
		break;
	case -90:
		str = "Invalid command";
		break;
	case -91:
		str = "reserved";
		break;
	case -92:
		str = "Response when error status is exclusion";
		break;
	case -93:
		str = "enable";
		break;
	case -94:
		str = "inhibit";
		break;
	case -95:
		str = "busy";
		break;
	case -96:
		str = "power on";
		break;
	}
	return str;
}