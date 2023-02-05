#include <iostream>
#include <glog/logging.h>
#include <io.h>
#include <direct.h>
#include "Core.h"

using namespace std;
using namespace google;

int main() {
	system("chcp 65001");//设置使用unicode 字符
	if (_access("./log", 0) == -1) //如果文件夹不存在
		_mkdir("./log");           //则创建

	InitGoogleLogging("LuckyNum");//初始化日志记录
	FLAGS_log_dir = "./log"; //设置log日志存放的位置
	FLAGS_colorlogtostderr = true;
	FLAGS_alsologtostderr = true;

	Core* core = new Core();
	core->RunService();//运行服务

	return 0;
}