//#include "afxmt.h"
//#include <io.h> 
//#include <direct.h> 
#include <stdio.h>
#include "tools.h"
#include <boost/thread.hpp>

string put_out_hex(unsigned char *data, int len)
{
	string out;
	
	char buf[30] = {0};
	for(int i = 0; i < len; ++i)
	{
		memset(buf, 0, sizeof(buf));
		//sprintf_s(buf, 30, "%02x", data[i]);
		sprintf(buf, "%02x", data[i]);
		out += buf;
		out += " ";
	}

	return out;
}
void htonl(unsigned long host, char *net)
{
	for (int i=0; i<4; i++){
		net[i] = (host >> (8 * (3 - i))) & 0xff;
	}
}
void ntohs1(unsigned char *net, unsigned short &host)
{
	host = 0;
	for (int i=0; i<2; i++){
		host += ((unsigned char)net[i]) << (8 * (1 - i));
	}
}


//CCriticalSection	critical_section1;
//boost::mutex sec1;
//bool log_open = true;
/*(char*data,...)
{ 
	//critical_section1.Lock();
	boost::mutex::scoped_lock lock(sec1);

	char	prj_path[300] = {0};		//�����·��
	char	szBuffer[40960] = {0};
	char	tmp[40960] = {0};
	char	szTime[32] = {0};
	va_list pArguments;
	static int size=0;
	int c;
	GetCurrentDirectory(MAX_PATH, prj_path);	
	time_t t		= time(NULL);
	struct tm *st	= localtime(&t);

	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", st->tm_year+1900, st->tm_mon+1, st->tm_mday, st->tm_hour, st->tm_min, st->tm_sec);

	va_start(pArguments, data);
	c = vsprintf(tmp, data, pArguments);
	sprintf(szBuffer, "%s :: %s", szTime, tmp);

	if (log_open)
	{			
		char szDirectName[300];
		sprintf(szDirectName, "%s\\logs", prj_path);
		if(_access(szDirectName, 0) != 0)
		{
			_mkdir(szDirectName);
		}	
		sprintf(szDirectName, "%s\\logs\\%04d%02d%02d", prj_path, st->tm_year+1900, st->tm_mon+1, st->tm_mday);
		if(_access(szDirectName, 0) != 0)
		{
			_mkdir(szDirectName);
		}	
		char szFileName[300];
		sprintf(szFileName, "%s\\nv9_%04d%02d%02d.txt", szDirectName, st->tm_year+1900, st->tm_mon+1, st->tm_mday);
		FILE *fp=fopen(szFileName,"a+");
		if (fp==NULL)
		{
			goto show;
		}
		fprintf(fp,"%s\n", szBuffer);
		fclose(fp);	
	}

show:
	//critical_section1.Unlock();
	return 0;
}*/

string get_EachModule_Initinfo(int err_code)
{
	string str = "";
	switch(err_code)
	{		
	case 0:
		str = "Initialization succeeded";
		break;
	case 0x01:
		str = "Configuration file object is not instantiated";
		break;
	case 0x02:
		str = "Coin receiver serial port open failed";
		break;
	case 0x04:
		str = "One dollar coin change library load failed";
		break;
	case 0x08:
		str = "One-dollar coin-to-zero serial port open failure";
		break;
	case 0x10:
		str = "One dollar coin change reset failed";
		break;
	case 0x20:
		str = "50 cents change library load failed";
		break;
	case 0x40:
		str = "50 cents change string opening failed";
		break;
	case 0x80:
		str = "The change - and - reset of the pentagram failed.";
		break;
	case -50:
		str = "There is no UnionPay dll library";
		break;
	case -51:
		str = "There is no UnionPay dll library function";
		break;
	case -52:
		str = "UnionPay check-in failed";
		break;	
	case -53:
		str = "Note change string opening failed";
		break;
	case -54:
		str = "Cdm4000 banknote change diagnosis failed";
		break;
	case -55:
		str = "Cdm4000 banknote change reset failed";
		break;
	case -56:
		str = "Cdm4000 banknote change thickness setting failed";
		break;
	case -57:
		str = "Cdm4000 banknote change size setting failed";
		break;
	case -58:
		str = "E-card serial port open failure";
		break;
	case -59:
		str = "E card connection test failed";
		break;
	case -60:
		str = "The number of change cases in the F 53 note is incorrectly configured.";
		break;
	case -61:
		str = "F 53 notes change the first NCK.";
		break;
	case -62:
		str = "F53 banknote change the first ACK responds to other error instructions";
		break;
	case -63:
		str = "F 53 notes change the first ACK timeout.";
		break;
	case -64:
		str = "F 53 notes change the second NCK.";
		break;
	case -65:
		str = "F53 banknote change the second ACK responds to other error instructions";
		break;
	case -66:
		str = "F53 banknote change the second ACK timeout";
		break;
	case -67:
		str = "F53 banknote change en Q responds to other error instructions";
		break;
	case -68:
		str = "F53 banknote change en Q timeout";
		break;
	case -69:
		str = "F53 banknote change RecvDataStatu timeout";
		break;
	case -70:
		str = "F53 banknote change other";
		break;
	case -71:
		str = "The F 53 note was also incorrectly re - read.";
		break;
	case -72:
		str = "F53 note failed to find zero";
		break;
	case -73:
		str = "F53 banknote change reset status reading flag failed";
		break;
	case -74:
		str = "F53 banknote change did not recognize the corresponding device ID";
		break;
	case -75:
		str = "C scan B UnionPay sign-in failure";
		break;
	case -76:
		str = "Bank card and C scan B UnionPay both failed to sign in.";
		break;
	case -80:
		str = "Failed to read keya.txt";
		break;
	case -81:
		str = "Not instantiated, call failed";
		break;
	case -82:
		str = "Failed to open IC card reader serial port";
		break;
	case -83:
		str = "Failed to connect to IC card reader";
		break;
	case -84:
		str = "There is no E-card dll library";
		break;
	case -85:
		str = "There is no E-card dll library function";
		break;
	case -86:
		str = "There is no money - box mode.";
		break;
	case -87:
		str = "Failed to load cash box library";
		break;
	case -88:
		str = "Failed to switch USB mode when turned on";
		break;
	case -89:
		str = "Failed to load bullet box library MFCLibraryTpt200V1.dll";
		break;
	case -90:
		str = "Failed to load the MFCLibraryTpt200V1.dll function of the bullet box library";
		break;
	case -91:
		str = "Instantiation of form object or thread failed";
		break;
	case -92:
		str = "The end form pointer object is not instantiated";
		break;
	default:
		str = "other";
		break;
	}
	return str;
}

string get_CoinModule_Inifinfo(int code)
{
	string str;
	switch(code) 
	{
	case 0:
		str = "Initialization succeeded";
		break; 
	case 0x01:
		str = "Configuration file object is not instantiated";
		break; 
	case 0x02:
		str = "Coin receiver serial port open failed";
		break; 
	case 0x04:
		str = "The first coin search library failed to load";
		break; 
	case 0x08:
		str = "The first coin change string failed to open";
		break; 
	case 0x10:
		str = "The first coin change failed to reset";
		break; 
	case 0x20:
		str = "The second kind of coin search library failed to load";
		break; 
	case 0x40:
		str = "The second kind of coin change string opening failed";
		break; 
	case 0x80:
		str = "The second kind of coin failed to change and reset.";
		break; 
	case 0x0100:
		str = "The third kind of coin failed to load the zero library.";
		break; 
	case 0x0200:
		str = "The third kind of coin failed to open the change string.";
		break; 
	case 0x0400:
		str = "The third kind of coin failed to change and reset.";
		break; 
	case -1:
		str = "Failed to open serial port";
		break; 
	case -2:
		str = "Check that the change of coins has not passed";
		break;	
	case -3:
		str = "Check to see if the coin is received or not.";
		break;		
	case -4:
		str = "Chop object instantiation failed";
		break;		
	case -50:
		str = "No factory library exists[lf280-posinf.dll m100-umsapi.dll trans.dll]";
		break;		
	case -51:
		str = "There is no manufacturer library function[lf280-posinf.dll m100-umsapi.dll trans.dll]";
		break;	
	case -52:
		str = "Check in failed[lf280��lf290��M100]";
		break;		
	case -53:
		str = "Note change string opening failed";
		break;			
	case -54:
		str = "Cdm4000 banknote change diagnosis failed";
		break;			
	case -55:
		str = "Cdm4000 banknote change reset failed";
		break;			
	case -56:
		str = "Cdm4000 banknote change thickness setting failed";
		break;			
	case -57:
		str = "Cdm4000 banknote change size setting failed";
		break;			
	case -58:
		str = "E-card serial port open failure";
		break;		
	case -59:
		str = "E card connection test failed";
		break;		
	case -60:
		str = "The number of change cases in the F 53 note is incorrectly configured.";
		break;		
	case -61:
		str = "F 53 notes change the first NCK.";
		break;	
	case -62:
		str = "F53 banknote change the first ACK responds to other error instructions";
		break;	
	case -63:
		str = "F 53 notes change the first ACK timeout.";
		break;		
	case -64:
		str = "F 53 notes change the second NCK.";
		break;			
	case -65:
		str = "F53 banknote change the second ACK responds to other error instructions";
		break;	
	case -66:
		str = "F53 banknote change the second ACK timeout";
		break;		
	case -67:
		str = "F53 banknote change en Q responds to other error instructions";
		break;		
	case -68:
		str = "F53 banknote change en Q timeout";
		break;		
	case -69:
		str = "F53 banknote change RecvDataStatu timeout";
		break;			
	case -70:
		str = "F53 banknote change other";
		break;		
	case -71:
		str = "The F 53 note was also incorrectly re - read.";
		break;		
	case -72:
		str = "F53 banknote change reset failed";
		break;	
	case -73:
		str = "F53 banknote change reset status reading flag failed";
		break;	
	case -74:
		str = "The F53 paper currency has no identification to the corresponding device ID";
		break;	
	case -75:
		str = "C scan B UnionPay sign-in failure[M100]";
		break;		
	case -76:
		str = "Bank card and C scan B UnionPay both failed to sign in.[M100]";
		break;			
	}
	/*if(g_config->GetData().m_coin_type == 1)
	{
		//����
		switch(code) //-1�򿪴���ʧ�� -2���Ӳ������ûͨ�� -3���Ӳ�ҽ���ûͨ�� -4CHopper����ʵ����ʧ��
		{
		case -1:
			str = "�򿪴���ʧ��";
			break;
		case -2:
			str = "���Ӳ������ûͨ��(pay_out_by_num��0��5��)";
			break;
		case -3:
			str = "���Ӳ�ҽ���ûͨ��(disable)";
			break;
		case -4:
			str = "CHopper����ʵ����ʧ��";
			break;
		default:
			break;
		}
	}
	//��˳
	else if (g_config->GetData().m_coin_type == 2)
	{
		if ((code & 0x01) == 0x01){
			str += "g_configû��ʵ����;";
		}
		if ((code & 0x02) == 0x02){
			str += "��Ӳ�ҽ��մ���ʧ��;";
		}
		if ((code & 0x04) == 0x04){
			str += "������һԪӲ�������;";
		}
		if ((code & 0x08) == 0x08){
			str += "һԪ���㴮�ڴ�ʧ��;";
		}
		if ((code & 0x10) == 0x10){
			str += "һԪ���㸴λmk2Pollʧ��;";
		}
		if ((code & 0x20) == 0x20){
			str += "���������Ӳ�������;";
		}
		if ((code & 0x40) == 0x40){
			str += "������㴮�ڴ�ʧ��;";
		}
		if ((code & 0x80) == 0x80){
			str += "������㸴λmk2PollHalfʧ��;";
		}
	}
	else if(g_config->GetData().m_coin_type == 0)
	{
		warn(WO_MSG_SYS, "Ӳ���豸��ʼ��ʧ�� ret = %d [0:�ɹ� 0x01:���ö���δʵ�� 0x02:Ӳ�ҽ��մ���δ�� 0x1C:һԪӲ�Ҽ���ʧ�� 0xE0:������Ҽ���ʧ�� ]", code);
		//��׼��
		if ((code & 0x01) == 0x01)
		{
			str += "�����ļ�����δʵ����";
		}
		else if ((code & 0x02) == 0x02)
		{
			str += "Ӳ�ҽ��������ڴ�ʧ�ܣ�PS������Ӳ�ҽ�������Ӳ�����������д��ڴ�ʧ�ܣ�";
		}
		else if ((code & 0x04) == 0x04)
		{
			str += "һԪӲ����������ʧ��";
		}
		else if ((code & 0x08) == 0x08)
		{
			str += "һԪӲ�����㴮�ڴ�ʧ��";
		}
		else if ((code & 0x10) == 0x10)
		{
			str += "һԪӲ�����㸴λʧ��";
		}
		else if ((code & 0x20) == 0x20)
		{
			str += "���Ӳ����������ʧ��";
		}
		else if ((code & 0x40) == 0x40)
		{
			str += "���Ӳ�����㴮�ڴ�ʧ��";
		}
		else if ((code & 0x80) == 0x80)
		{
			str += "���Ӳ�����㸴λʧ��";
		}	
	}*/
	return str;
}

string getEcardCodeInfo(int ret, unsigned char code)
{
	string str = "";
	char buf[32] = { 0 };
	switch(ret)
	{
	case 0:
		str = "�ɹ� ";
		break;
	case -1:
		str = "�հ�ͷ��ʱ ";
		break;
	case -2:
		str = "��ͷ�� ";
		break;
	case -3:
		str = "�հ�β��ʱ ";
		break;
	case -4:
		str = "У��� ";
		break;
	case -5:
		str = "�������� ";
		break;
	}

	switch(code)
	{
	case 0x00:
		str += "Successful operation";
		break;
	case 0x01:
		str += "Card brushing error";
		break;
	case 0x02:
		str += "M1 card BCC error check";
		break;
	case 0x03:
		str += "Card misformat";
		break;
	case 0x04:
		str += "Non-system card";
		break;
	case 0x05:
		str += "Card not enabled";
		break;
	case 0x06:
		str += "Stop card";
		break;
	case 0x07:
		str += "Black card";
		break;
	case 0x08:
		str += "Card expiration period";
		break;
	case 0x09:
		str += "not sufficient funds";
		break;
	case 0x0a:
		str == "PSAM operation failed!";
		break;
	case 0x15:
		str += "Other errors";
		break;
		//7D 7E�����
	case 0x7D:
		str += "Transaction interface call exception";
		break;
	case 0x7E:
		str += "Cannot start thread";
		break;
	case 0x81:
		str += "overtime ";
		break;
	case 0x82:
		str += "Test error";
		break;
	case 0x83:
		str += "Instruction error";
		break;
	case 0x84:
		str += "Password error";
		break;
	case 0x9c: //
		str += "card return information";
		break;
	case 0x9d: //
		str += "Blacklist card data";
		break;
	case 0x9e: //
		str += "Successful transaction returns data";
		break;
	case 0xff:
		str += "Operation failure";
		break;
	default:
		sprintf(buf, "other code 0x%02x", code);
		str += buf;
		break;
	}

	return str;
}


