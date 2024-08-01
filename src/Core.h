#ifndef CORE_H
#define CORE_H

#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <future>
#include <nlohmann/json.hpp>
#include <hv/WebSocketClient.h>
#include <shared_mutex>
#include <iostream>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <random>
#include "Util.h"
#include "ImgGenerator.h"

using namespace nlohmann;
using namespace spdlog;
using namespace std;

class ProtectData {
private:
	shared_mutex mLocker;
	string mToken;
public:
	void Session(const string token);
	string Session();
};//�̰߳�ȫ������
class Core {
private:
	ProtectData mData;
	shared_mutex mLocker;
	map<long long, int>mLuckNumMap;//���ֶ��ձ�
	string mSession;//session
	shared_ptr<logger>LOGGER;
	P_ImgGenerator mImgGenerator;

	hv::WebSocketClient wss;
	wstring GetCom(int num);//����ָ����ȡ����
	void onMessageRec(const string& msg);//������Ϣ�Ļص�����
	void DeleteOutDateThread();//ɾ���Ѿ���ʱ�����ֵ��߳�
	void onClose();//�Ͽ�����ʱִ�еĻص�����
	void onOpen();//���ӳɹ�ִ�еĻص�����
	void ParseMsg(const string& msg);//�������ݵ���Ϣ
	void SendGroupMsg(long long targetGroup, string text);

public:
	void RunService();//���з���
	Core();
};

#endif // !CORE_H
