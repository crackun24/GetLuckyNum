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
};//线程安全的数据
class Core {
private:
	ProtectData mData;
	shared_mutex mLocker;
	map<long long, int>mLuckNumMap;//数字对照表
	string mSession;//session
	shared_ptr<logger>LOGGER;
	P_ImgGenerator mImgGenerator;

	hv::WebSocketClient wss;
	wstring GetCom(int num);//根据指数获取评价
	void onMessageRec(const string& msg);//接收消息的回调函数
	void DeleteOutDateThread();//删除已经超时的数字的线程
	void onClose();//断开连接时执行的回调函数
	void onOpen();//连接成功执行的回调函数
	void ParseMsg(const string& msg);//解析数据的信息
	void SendGroupMsg(long long targetGroup, string text);

public:
	void RunService();//运行服务
	Core();
};

#endif // !CORE_H
