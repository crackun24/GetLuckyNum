#include <iostream>
#include <glog/logging.h>
#include <io.h>
#include <direct.h>
#include "Core.h"

using namespace std;
using namespace google;

int main() {
	system("chcp 65001");//����ʹ��unicode �ַ�
	if (_access("./log", 0) == -1) //����ļ��в�����
		_mkdir("./log");           //�򴴽�

	InitGoogleLogging("LuckyNum");//��ʼ����־��¼
	FLAGS_log_dir = "./log"; //����log��־��ŵ�λ��
	FLAGS_colorlogtostderr = true;
	FLAGS_alsologtostderr = true;

	Core* core = new Core();
	core->RunService();//���з���

	return 0;
}