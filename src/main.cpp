#include <iostream>
#include <io.h>
#include <direct.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/logger.h>
#include "Core.h"

using namespace std;
using namespace spdlog;

int main() {
	system("chcp 65001");//设置使用unicode 字符

	if (_access("./log", 0) == -1) //如果文件夹不存在
		_mkdir("./log");           //则创建

		if (!filesystem::exists("./logs")) //判断文件夹是否存在
	{
		filesystem::create_directory("./logs"); //如果不存在的话就创建一个文件夹
	}

	auto console_sink = make_shared<sinks::stdout_color_sink_mt>(); //控制台日志记录器
	auto daily_sink = make_shared<sinks::daily_file_sink_mt>(".logs/log.log", 0, 0); //每日日志记录器
	auto LOGGER = make_shared<logger>("GetLuckyNum", sinks_init_list{console_sink, daily_sink}); //创建主日志记录器
	register_logger(LOGGER); //注册日志记录器

	Core* core = new Core();
	core->RunService();//运行服务

	return 0;
}