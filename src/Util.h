#pragma once

#include <iostream>

using namespace std;

class Util
{
private:
public:
	static string WstringToWstring(const wstring &str);
	static wstring StringToWstring(const string &str);
	static wstring GetCom(int num);
};