#include "Util.h"

string Util::WstringToString(const wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(str);
}

wstring Util::StringToWstring(const string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes(str);
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
