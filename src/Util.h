#pragma once

#include <iostream>
#include <codecvt>

using namespace std;

class Util
{
private:
public:
	static string WstringToString(const wstring &str);
	static wstring StringToWstring(const string &str);
	static wstring GetCom(int num);
};