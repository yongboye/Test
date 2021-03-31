#include <string>
#include <iostream>
#include "glog/logging.h"   // glog 头文件
#include "glog/raw_logging.h"

int main(int argc, char** argv)
{
    FLAGS_log_dir="./logs";   //设置log目录  没有指定则输出到控制台
    FLAGS_logtostderr = 1;  //输出到控制台
    google::InitGoogleLogging(argv[0]);    // 初始化program name

    std::string test = "this is test";
    int i = 2, number = 8;

    LOG(INFO) << "it is info";     // 打印log：“hello glog.  类似于C++ stream。
	
    LOG_IF(INFO, number > 10) << "number >  10"; 
    LOG_IF(INFO, number < 10) << "number <  10";
	
    for(i=0; i<20 ;i++){
        LOG_EVERY_N(INFO, 5) << "log i = " << i;
    }

    LOG(WARNING) << "It is error info"; 
    LOG(ERROR) << "It is error info"; 

    DLOG(INFO) << "it is debug mode";
	
    DLOG_IF(INFO, number > 10) << "debug number > 10";  

    RAW_LOG(INFO, "it is pthread log");


    return 0;
}
