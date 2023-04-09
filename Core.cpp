#include "Core.h"

string Core::GetCom(int num) {
    if (num <= 10)
        return "非酋";
    else if (num > 10 && num <= 20)
        return "有点白的非酋";
    else if (num > 20 && num <= 50)
        return "中等";
    else if (num > 50 && num <= 60)
        return "中等偏上";
    else if (num > 60 && num <= 99)
        return "欧皇";
    else if (num == 100)
        return "极品欧皇";

}

void Core::onMessageRec(const string &msg) {
    try {
        ParseMsg(msg);
    }
    catch (const exception &e) {
        LOG(WARNING) << "An error occured when parsing the message,error msg: " << e.what();
    }
}

void Core::DeleteOutDateThread() {
    while (true) {
        try {
            chrono::system_clock::time_point now = chrono::system_clock::now();//获取当前时间
            time_t time = chrono::system_clock::to_time_t(now);//转换时间
            struct tm *t = localtime(&time);//转换为当地时间
            t->tm_hour = 0;
            t->tm_min = 0;
            t->tm_sec = 0;
            t->tm_mday += 1;

            auto nextDay = chrono::system_clock::from_time_t(mktime(t));//转换时间
            auto duration = nextDay - chrono::system_clock::now();//计算线程要休眠的时间

            this_thread::sleep_for(duration);//睡眠到次日的凌晨
            lock_guard<shared_mutex> lg(this->mLocker);
            this->mLuckNumMap.clear();//清空表
        }
        catch (const exception &e) {
            LOG(WARNING) << "An error occured when running the delete outdate thread,error msg: " << e.what();
        }
    }
}

void Core::onClose() {
    LOG(WARNING) << "Connection lost.";
}

void Core::onOpen() {
    LOG(INFO) << "Connected.";
}

void Core::ParseMsg(const string &msg) {
    try {
        json root = json::parse(msg);
        auto data = root["data"];

        if (data.contains("session"))
            this->mData.Session(data["session"]);
        else if (data["type"] == "GroupMessage")//判断为群组信息
        {
            string msg;
            for (auto &temp: data["messageChain"])//解析数据链的信息
            {
                if (temp.contains("text"))//判断包含信息
                {
                    msg = temp["text"];
                }
            }
            long long senderId = data["sender"]["id"];
            long long groupId = data["sender"]["group"]["id"];

            if (msg == GbkToUtf8("幸运指数")) {
                srand(time(NULL));//初始化随机数种子
                stringstream ss;
                ss << "您今日的幸运指数为: ";
                int num;
                shared_lock<shared_mutex> lg(this->mLocker);
                if (this->mLuckNumMap.find(senderId) != this->mLuckNumMap.end())//判断是否已经获取过了
                {
                    num = this->mLuckNumMap[senderId];
                } else {//没有获取过
                    srand(time(NULL));//初始化随机数种子
                    int randNum = rand() % 101;//获取随机数
                    this->mLuckNumMap.insert(make_pair(senderId, randNum));//插入随机数
                    num = randNum;
                }
                ss << to_string(num) << "," << GetCom(num);
                SendGroupMsg(groupId, GbkToUtf8(ss.str().c_str()));
            }
        }
    }
    catch (const exception &e) {
        LOG(WARNING) << "An error occured when parsing the data,error msg: " << e.what();
    }
}

void Core::RunService() {
    try {
        auto res = async(launch::async, &Core::DeleteOutDateThread, this);

        libProp::Config conf = libProp::Config::Parse("./config.properties");//解析配置文件信息

        wss.onmessage = bind(&Core::onMessageRec, this, placeholders::_1);
        wss.onopen = bind(&Core::onOpen, this);
        wss.onclose = bind(&Core::onClose, this);

        reconn_setting_s rec;
        rec.cur_delay = 2000;//断开连接后2s重新连接
        wss.reconn_setting = &rec;//设置重新连接的对象
        wss.open(conf["miraiUrl"].as<string>().c_str());//开启连接

        getchar();
    }
    catch (const exception &e) {
        LOG(ERROR) << "An error occured when running the service,error msg: " << e.what() << ",exiting.";
        terminate();//结束程序
    }

}

Core::Core() {
}

string Core::GbkToUtf8(const char *src_str) {
    int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
    wchar_t *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if (wstr) delete[] wstr;
    if (str) delete[] str;
    return strTemp;
}

string Core::Utf8ToGbk(const char *src_str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if (wszGBK) delete[] wszGBK;
    if (szGBK) delete[] szGBK;
    return strTemp;
}

void Core::SendGroupMsg(long long targetGroup, string text) {
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
}//发送群组消息

void ProtectData::Session(string token) {
    lock_guard<shared_mutex> lg(this->mLocker);
    this->mToken = token;
}

string ProtectData::Session() {
    shared_lock<shared_mutex> lg(this->mLocker);
    return this->mToken;
}
