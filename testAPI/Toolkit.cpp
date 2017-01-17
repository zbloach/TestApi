#include "Toolkit.h"

vector<string> Toolkit::T_split(string str, string pattern)
{
	std::string::size_type pos;

	std::vector<std::string> result;

	str += pattern;//扩展字符串以方便操作 // 可以不用  

	int size = str.size();

	for (int i = 0; i < size; i++)

	{
		pos = str.find(pattern, i);

		if (pos < size)

		{
			std::string s = str.substr(i, pos - i);

			result.push_back(s);

			i = pos + pattern.size() - 1;
		}

	}

	return result;
}

int Toolkit::T_stockMarket(string stockcode)
{
	char* a = (char*)malloc(sizeof(stockcode.c_str()));
	strcpy(a, stockcode.c_str());
	if (a[0] == '6' || strcmp("000300", a) == 0 || a[0] == '7')
	{
		//上交所股票
		return 1;
	}
	else if (a[0] == '3' || a[0] == '0')
	{
		return 0;
	}
	else
		cout<<"无法判断是上交所还是深交所股票"<<endl;
		return -1;

}

bool Toolkit::T_isNum(string str)
{
	int size = str.length();
	char temp_c = '!';
	for (int i = 0; i < size; i++)
	{
		temp_c = str[i];
		if ((temp_c >= '0' && temp_c <= '9') || (temp_c >= 'a' && temp_c <= 'z') || (temp_c >= 'A' && temp_c <= 'Z'))
		{
			continue;
		}
		else
			return false;
		
	}
	return true;
	/*
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
	*/
}

int Toolkit::T_isExgTme(const time_t testtime)
{
	//time_t time_now;
	//time(&time_now);
	tm* tm_testtime = localtime(&testtime);
	//tm* tm_now = localtime(&time_now);
	tm tm_start0 = *tm_testtime;
	time_t time_start0,time_end0,time_start1,time_end1;
	tm_start0.tm_hour = 9;
	tm_start0.tm_min = 30;
	tm_start0.tm_sec = 0;
	time_start0 = mktime(&tm_start0);
	time_end0 = time_start0 + 2 * 3600 - 60;//抛开最后一分钟
	time_start1 = time_start0 + 3.5 * 3600;
	time_end1 = time_start1 + 2 * 3600 - 60;//抛开最后一分钟
	//开盘前
	if (testtime < time_start0)
	{
		return 0;
	}
	//上午开盘
	if (testtime >= time_start0 && testtime < time_end0)
	{
		return 1;
	}
	//中午休市
	if (testtime >= time_end0 && testtime < time_start1)
	{
		return 2;
	}
	//下午开盘
	if (testtime >= time_start1 && testtime < time_end1)
	{
		return 3;
	}
	//下午收盘
	if (testtime >= time_end1)
	{
		return 4;
	}
	/*
	if ((testtime >= time_start0 && testtime < time_end0) || (testtime >= time_start1 && testtime < time_end1))
	{
		//printf("交易时间\n");
		return true;
	}
	//double xx0 = difftime(time_end0, time_start0);
	//printf("非交易时间\n");
	//return false;
	*/
	return -1;
}

std::string Toolkit::T_selectIp(vector<string> v_ip)
{
	string selectedIp = "";
	CPing objPing;
	PingReply reply;
	vector<string>::iterator iter = v_ip.begin();
	
	int minTime = 1000000;

	if (v_ip.size() > 0)
	{
		int i = 0;
		for (iter; iter != v_ip.end(); iter++,i++)
		{
			char* tempchar = (char*)malloc(sizeof(v_ip[i]));
			strcpy(tempchar, v_ip[i].c_str());
			objPing.Ping(tempchar, &reply);
			printf("Reply from %s: bytes=%d time=%ldms TTL=%ld\n", tempchar, reply.m_dwBytes, reply.m_dwRoundTripTime, reply.m_dwTTL);
			if (reply.m_dwRoundTripTime < minTime && reply.m_dwRoundTripTime >= 0)
			{
				minTime = reply.m_dwRoundTripTime;
				selectedIp = v_ip[i];
			}
		}
		
	}
	return selectedIp;
}

map<string, vector<double>> Toolkit::T_readNewStock()
{
	map<string, vector<double>> result;
	string net_address = "http://stock.stockstar.com/ipo/";
	static DataApi dataApi;
	string context = dataApi.readData_tx(net_address);
	string temp_str = ">新股档案<";

	context = T_cut_after(context, temp_str,false);
	temp_str = "align=\"center\"><span class=\"red_color\">";
	
	do 
	{
		string code = T_getContext(context, "\"red_color\">", "</span>");
		int num = atof(T_getContext(context, "\"right\">", "</td>").c_str()) * 10000;
		context = T_cut_after(context, "\"right\">",false);
		double price = atof(T_getContext(context, "\"right\">", "</td>").c_str());
		vector<double> temp_vec;
		temp_vec.push_back(num);
		temp_vec.push_back(price);
		result.insert(make_pair(code, temp_vec));

		context = T_cut_after(context, temp_str, true);
	} while (context.length() != 0);
	
	return result;
}

std::string Toolkit::T_cut_after(string str_0, string str_sub,bool contern)
{
	int re_in = str_0.find(str_sub);
	if (re_in == -1)
	{
		return "";
	}
	if (contern)
	{
		return str_0.substr(re_in);
	}
	else
	{
		return str_0.substr(re_in + str_sub.length());
	}
	 
}

std::string Toolkit::T_cut_before(string str_0, string str_sub)
{
	int re_in = str_0.find(str_sub);
	if (re_in == -1)
	{
		return "";
	}
	return str_0.substr(0, re_in);
}

std::string Toolkit::T_getContext(string str_0, string start_str, string end_str)
{
	string str_1 = T_cut_after(str_0, start_str,false);
	return T_cut_before(str_1, end_str);
}
