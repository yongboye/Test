// testglog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <map>
#include <boost/thread.hpp>

#include "../glog/gLog.h"
#pragma comment(lib, "../Debug/glog.lib")

int main()
{
	/*struct RepeaCommand {
		//boost::posix_time::ptime t;
		int    times;
		string command;
		int busid;
		char lock; //'0'kai '1'guan
	};

	map<string, RepeaCommand> repeaCommand; //锁重发指令
	RepeaCommand rc;
	//rc.t = boost::posix_time::microsec_clock::local_time() + boost::posix_time::seconds(60);
	rc.times = 0;
	rc.command = "(C010)";
	rc.busid = 1;
	rc.lock = '0';
	repeaCommand["01"] = rc;
	RepeaCommand& rc2 = repeaCommand["01"];
	//rc2.busid = 2;

	rc.times = 3;
	rc.command = "(C020)";
	rc.busid = 2;
	rc.lock = '0';
	repeaCommand["02"] = rc;

	for (map<string, RepeaCommand>::iterator it = repeaCommand.begin(); it != repeaCommand.end(); ++it)
	{ 
		cout << it->first << endl;
		cout << it->second.busid << endl;
	}

	for (map<string, RepeaCommand>::iterator it = repeaCommand.begin(); it != repeaCommand.end();)
	{
		if (it->second.times >= 3)
		{
			repeaCommand.erase(it++);
			continue;
		}
		++it;
	}

	for (map<string, RepeaCommand>::iterator it = repeaCommand.begin(); it != repeaCommand.end(); ++it)
	{
		cout << it->first << endl;
		cout << it->second.busid << endl;
	}

	map<int, string> is;
	is[1] = "aaa";
	is[2] = "bbb";
	is[1] = "ccc";
	is[3] = "ddd";
	cout << is.size() << endl;
	for (map<int, string>::iterator it = is.begin(); it != is.end(); it++)
		cout << "int=" << it->first << "," << "string=" << it->second << endl;*/








	LOGINIT("./logs", ""); //testglog.exe
	LOGINFO("First Number: %d.", 0);
	LOGWARN("First Number: %d.", 1);
	LOGERROR("First Number: %d.", 2);
	boost::this_thread::sleep(boost::posix_time::milliseconds(30000));
	LOGINFO("First Number: %d.", 0);
	LOGWARN("First Number: %d.", 1);
	LOGERROR("First Number: %d.", 2);

	system("PAUSE");
    return 0;
}

