#ifndef CORE_H
#define CORE_H

#include <glog/logging.h>
#include <chrono>
#include <future>
#include <nlohmann/json.hpp>
#include <hv/WebSocketClient.h>
#include <libProp.h>
#include <shared_mutex>
#include <iostream>

using namespace google;
using namespace nlohmann;
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
