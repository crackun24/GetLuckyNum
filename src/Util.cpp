#include "Util.h"

string Util::WstringToWstring(const wstring& str)
{
	return string(str.begin(), str.end());
}

wstring Util::StringToWstring(const string& str)
{
	return wstring(str.begin(), str.end());
}

wstring Util::GetCom(int num)
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

