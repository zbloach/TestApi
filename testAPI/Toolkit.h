#pragma once
#include "stdio.h"
#include <string>
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <time.h>
#include <iostream>
#include "ping.h"
using namespace std;

class Toolkit
{
public:
	Toolkit();

	~Toolkit();

	static vector<string> T_split(string str, string pattern);
	static int T_stockMarket(string stockcode);
	static bool T_isNum(string str);
	static bool T_isExgTme(const time_t testtime);
	static string T_selectIp(vector<string> v_ip);
};

