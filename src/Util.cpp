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
		return L"����";
	else if (num > 10 && num <= 20)
		return L"�е�׵ķ���";
	else if (num > 20 && num <= 50)
		return L"�е�";
	else if (num > 50 && num <= 60)
		return L"�е�ƫ��";
	else if (num > 60 && num <= 99)
		return L"ŷ��";
	else if (num == 100)
		return L"��Ʒŷ��";
}
