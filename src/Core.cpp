#include "Core.h"

wstring Core::GetCom(int num)
{
	if (num <= 10)
		return L"非酋";
	else if (num > 10 && num <= 20)
		return L"有点白的非酋";
	else if (num > 20 && num <= 50)
		return L"中等";
	else if (num > 50 && num <= 60)
		return L"中等偏上";
	else if (num > 60 && num <= 99)
		return L"欧皇";
	else if (num == 100)
		return L"极品欧皇";
}

void Core::onMessageRec(const string& msg)
{
	try
	{
		ParseMsg(msg); //解析数据
	}
	catch (const exception& e)
	{
		LOGGER->info("An error occured when running the service,error msg:{},exiting.", e.what());
	}
}

void Core::DeleteOutDateThread()
{
	while (true)
	{
		try
		{
			chrono::system_clock::time_point now = chrono::system_clock::now(); //获取当前时间
			time_t time = chrono::system_clock::to_time_t(now); //转换时间
			struct tm* t = localtime(&time); //转换为当地时间
			t->tm_hour = 0;
			t->tm_min = 0;
			t->tm_sec = 0;
			t->tm_mday += 1;

			auto nextDay = chrono::system_clock::from_time_t(mktime(t)); //转换时间
			auto duration = nextDay - chrono::system_clock::now(); //计算线程要休眠的时间

			this_thread::sleep_for(duration); //睡眠到次日的凌晨
			lock_guard<shared_mutex> lg(this->mLocker);
			this->mLuckNumMap.clear(); //清空表
		}
		catch (const exception& e)
		{
			LOGGER->info("An error occured when running the service,error msg:{},exiting.", e.what());
		}
	}
}

void Core::onClose()
{
	LOGGER->info("Connection lost.");
}

void Core::onOpen()
{
	LOGGER->info("Connected.");
}

void Core::ParseMsg(const string& msg)
{
	try
	{
		LOGGER->info("Received message: {}", msg);//FIXME
		json root = json::parse(msg);
		auto data = root["data"];

		if (data.contains("session"))
		{
			this->mData.Session(data["session"]);
		}
		else if (data["type"] == "GroupMessage") //判断为群组信息
		{
			LOGGER->info("test");

			string msg;
			for (auto& temp : data["messageChain"]) //解析数据链的信息
			{
				if (temp.contains("text")) //判断包含信息
				{
					msg = temp["text"];
				}
			}
			long long senderId = data["sender"]["id"];
			long long groupId = data["sender"]["group"]["id"];

			LOGGER->info(msg);//BUG
			wcout<<Util::StringToWstring(msg)<<endl;//FIXME
			wcout << flush;//FIXME

			if (msg == Util::WstringToString(L"测试")) //BUG 
			{
				srand(time(NULL)); //初始化随机数种子
				wstringstream ss;
				ss << L"您今日的幸运指数为: ";

				int num;
				shared_lock<shared_mutex> lg(this->mLocker);
				if (this->mLuckNumMap.find(senderId) != this->mLuckNumMap.end()) //判断是否已经获取过了
				{
					num = this->mLuckNumMap[senderId];
				}
				else
				{
					//没有获取过
					srand(time(NULL)); //初始化随机数种子
					int randNum = rand() % 101; //获取随机数
					this->mLuckNumMap.insert(make_pair(senderId, randNum)); //插入随机数
					num = randNum;
				}
				ss << to_wstring(num) << L"," << GetCom(num);

				SendGroupMsg(groupId, Util::WstringToString(ss.str().c_str()));
			}
		}
	}
	catch (const exception& e)
	{
		LOGGER->info("An error occured when running the service,error msg:{},exiting.", e.what());
	}
}

void Core::RunService()
{
	try
	{
		auto res = async(launch::async, &Core::DeleteOutDateThread, this);


		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("./config.properties", pt); //解析配置文件信息

		wss.onmessage = bind(&Core::onMessageRec, this, placeholders::_1);
		wss.onopen = bind(&Core::onOpen, this);
		wss.onclose = bind(&Core::onClose, this);

		reconn_setting_s rec;
		rec.cur_delay = 2000; //断开连接后2s重新连接
		wss.reconn_setting = &rec; //设置重新连接的对象
		wss.open(pt.get<string>("miraiUrl").c_str()); //开启连接

		getchar();
	}
	catch (const exception& e)
	{
		LOGGER->info("An error occured when running the service,error msg:{},exiting.", e.what());
		terminate(); //结束程序
	}
}

Core::Core(): LOGGER(spdlog::get("GetLuckyNum"))
{
}

void Core::SendGroupMsg(long long targetGroup, string text)
{
	json root, msg;
	msg["type"] = "Plain";
	msg["text"] = text;
	root["syncId"] = -1;
	root["command"] = "sendGroupMessage";
	root["subCommand"] = NULL;
	root["content"]["sessionKey"] = this->mData.Session();
	root["content"]["target"] = targetGroup;
	root["content"]["messageChain"].push_back(msg);
	wss.send(root.dump());
} //发送群组消息

void ProtectData::Session(string token)
{
	lock_guard<shared_mutex> lg(this->mLocker);
	this->mToken = token;
}

string ProtectData::Session()
{
	shared_lock<shared_mutex> lg(this->mLocker);
	return this->mToken;
}
