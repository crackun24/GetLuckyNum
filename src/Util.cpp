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

