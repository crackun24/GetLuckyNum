#ifndef CORE_H
#define CORE_H

#include <chrono>
#include <future>
#include <nlohmann/json.hpp>
#include <hv/WebSocketClient.h>
#include <shared_mutex>
#include <iostream>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <random>

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

	hv::WebSocketClient wss;
	string GetCom(int num);//����ָ����ȡ����
	void onMessageRec(const string& msg);//������Ϣ�Ļص�����
	void DeleteOutDateThread();//ɾ���Ѿ���ʱ�����ֵ��߳�
	void onClose();//�Ͽ�����ʱִ�еĻص�����
	void onOpen();//���ӳɹ�ִ�еĻص�����
	void ParseMsg(const string& msg);//�������ݵ���Ϣ
	string GbkToUtf8(const char* src_str);
	string Utf8ToGbk(const char* src_str);
	void SendGroupMsg(long long targetGroup, string text);

public:
	void RunService();//���з���
	Core();
};

#endif // !CORE_H
