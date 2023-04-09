#include "Core.h"

string Core::GetCom(int num) {
    if (num <= 10)
        return "����";
    else if (num > 10 && num <= 20)
        return "�е�׵ķ���";
    else if (num > 20 && num <= 50)
        return "�е�";
    else if (num > 50 && num <= 60)
        return "�е�ƫ��";
    else if (num > 60 && num <= 99)
        return "ŷ��";
    else if (num == 100)
        return "��Ʒŷ��";

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
            chrono::system_clock::time_point now = chrono::system_clock::now();//��ȡ��ǰʱ��
            time_t time = chrono::system_clock::to_time_t(now);//ת��ʱ��
            struct tm *t = localtime(&time);//ת��Ϊ����ʱ��
            t->tm_hour = 0;
            t->tm_min = 0;
            t->tm_sec = 0;
            t->tm_mday += 1;

            auto nextDay = chrono::system_clock::from_time_t(mktime(t));//ת��ʱ��
            auto duration = nextDay - chrono::system_clock::now();//�����߳�Ҫ���ߵ�ʱ��

            this_thread::sleep_for(duration);//˯�ߵ����յ��賿
            lock_guard<shared_mutex> lg(this->mLocker);
            this->mLuckNumMap.clear();//��ձ�
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
        else if (data["type"] == "GroupMessage")//�ж�ΪȺ����Ϣ
        {
            string msg;
            for (auto &temp: data["messageChain"])//��������������Ϣ
            {
                if (temp.contains("text"))//�жϰ�����Ϣ
                {
                    msg = temp["text"];
                }
            }
            long long senderId = data["sender"]["id"];
            long long groupId = data["sender"]["group"]["id"];

            if (msg == GbkToUtf8("����ָ��")) {
                srand(time(NULL));//��ʼ�����������
                stringstream ss;
                ss << "�����յ�����ָ��Ϊ: ";
                int num;
                shared_lock<shared_mutex> lg(this->mLocker);
                if (this->mLuckNumMap.find(senderId) != this->mLuckNumMap.end())//�ж��Ƿ��Ѿ���ȡ����
                {
                    num = this->mLuckNumMap[senderId];
                } else {//û�л�ȡ��
                    srand(time(NULL));//��ʼ�����������
                    int randNum = rand() % 101;//��ȡ�����
                    this->mLuckNumMap.insert(make_pair(senderId, randNum));//���������
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

        libProp::Config conf = libProp::Config::Parse("./config.properties");//���������ļ���Ϣ

        wss.onmessage = bind(&Core::onMessageRec, this, placeholders::_1);
        wss.onopen = bind(&Core::onOpen, this);
        wss.onclose = bind(&Core::onClose, this);

        reconn_setting_s rec;
        rec.cur_delay = 2000;//�Ͽ����Ӻ�2s��������
        wss.reconn_setting = &rec;//�����������ӵĶ���
        wss.open(conf["miraiUrl"].as<string>().c_str());//��������

        getchar();
    }
    catch (const exception &e) {
        LOG(ERROR) << "An error occured when running the service,error msg: " << e.what() << ",exiting.";
        terminate();//��������
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
}//����Ⱥ����Ϣ

void ProtectData::Session(string token) {
    lock_guard<shared_mutex> lg(this->mLocker);
    this->mToken = token;
}

string ProtectData::Session() {
    shared_lock<shared_mutex> lg(this->mLocker);
    return this->mToken;
}
