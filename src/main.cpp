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
	system("chcp 65001");//����ʹ��unicode �ַ�

	if (_access("./log", 0) == -1) //����ļ��в�����
		_mkdir("./log");           //�򴴽�

		if (!filesystem::exists("./logs")) //�ж��ļ����Ƿ����
	{
		filesystem::create_directory("./logs"); //��������ڵĻ��ʹ���һ���ļ���
	}

	auto console_sink = make_shared<sinks::stdout_color_sink_mt>(); //����̨��־��¼��
	auto daily_sink = make_shared<sinks::daily_file_sink_mt>(".logs/log.log", 0, 0); //ÿ����־��¼��
	auto LOGGER = make_shared<logger>("GetLuckyNum", sinks_init_list{console_sink, daily_sink}); //��������־��¼��
	register_logger(LOGGER); //ע����־��¼��

	Core* core = new Core();
	core->RunService();//���з���

	return 0;
}