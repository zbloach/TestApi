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
#include <map>
#include "DataApi.h"
using namespace std;

class Toolkit
{
public:
	Toolkit();

	~Toolkit();

	static vector<string> T_split(string str, string pattern);
	static int T_stockMarket(string stockcode);
	static bool T_isNum(string str);
	static int T_isExgTme(const time_t testtime);
	static int T_ComputeExgMin(int time_state,int add_min);
	static string T_selectIp(vector<string> v_ip);
	static map<string, vector<double>> T_readNewStock();
	//contern true  包括子串   false 不包括子串
	static string T_cut_after(string str_0, string str_sub,bool contern);
	static string T_cut_before(string str_0, string str_sub);
	static string T_getContext(string str_0, string start_str, string end_str);
//public:
//	static DataApi dataApi;
};

