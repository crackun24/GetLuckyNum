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
};//线程安全的数据
class Core {
private:
	ProtectData mData;
	shared_mutex mLocker;
	map<long long, int>mLuckNumMap;//数字对照表
	string mSession;//session
	hv::WebSocketClient wss;
	string GetCom(int num);//根据指数获取评价
	void onMessageRec(const string& msg);//接收消息的回调函数
	void DeleteOutDateThread();//删除已经超时的数字的线程
	void onClose();//断开连接时执行的回调函数
	void onOpen();//连接成功执行的回调函数
	void ParseMsg(const string& msg);//解析数据的信息
	string GbkToUtf8(const char* src_str);
	string Utf8ToGbk(const char* src_str);
	void SendGroupMsg(long long targetGroup, string text);
public:
	void RunService();//运行服务
	Core();
};

#endif // !CORE_H
