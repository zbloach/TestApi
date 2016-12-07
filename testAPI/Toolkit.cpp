#include "Toolkit.h"

vector<string> Toolkit::T_split(string str, string pattern)
{
	std::string::size_type pos;

	std::vector<std::string> result;

	str += pattern;//��չ�ַ����Է������ // ���Բ���  

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
	if (a[0] == '6')
	{
		//�Ͻ�����Ʊ
		return 1;
	}
	else if (a[0] == '3' || a[0] == '0')
	{
		return 0;
	}
	else
		cout<<"�޷��ж����Ͻ������������Ʊ"<<endl;
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

bool Toolkit::T_isExgTme(const time_t testtime)
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
	time_end0 = time_start0 + 2 * 3600;
	time_start1 = time_end0 + 1.5 * 3600;
	time_end1 = time_start1 + 2 * 3600;
	if ((testtime >= time_start0 && testtime < time_end0) || (testtime >= time_start1 && testtime < time_end1))
	{
		//printf("����ʱ��\n");
		return true;
	}
	//double xx0 = difftime(time_end0, time_start0);
	/*
	if (testtime <)
	{
	}
	*/
	//printf("�ǽ���ʱ��\n");
	return false;
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
