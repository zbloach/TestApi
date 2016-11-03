#include "testAPI.h"

testAPI::testAPI()
{

}

bool testAPI::Init(string ip, int tradeport, string txmm, string yeb, string userid, string password)
{
	svinfo = { ip, tradeport, txmm, yeb };
	this->tdApi.Init(svinfo);
	bool result = tdApi.Login(userid, password);
	return result;
}

bool testAPI::readExgList(string fileAdress, vector<string>& v_buy_list, vector<string>& v_sell_list)
{
	v_buy_list.clear();
	v_sell_list.clear();
	fstream f;
	f.open(fileAdress.c_str(), ios::in);
	cout << fileAdress.c_str() << endl;
	if (f)
	{
		string s0,s2;
		while (f >> s0)
		{
			f >> s2;
			if (atoi(s2.c_str()) > 0)
			{
				if (find(v_buy_list.cbegin(), v_buy_list.cend(), s0) == v_buy_list.cend())
				{
					v_buy_list.push_back(s0);
				}
			} 
			else
				if (atoi(s2.c_str()) < 0)
				{
				if (find(v_sell_list.cbegin(), v_sell_list.cend(), s0) == v_sell_list.cend())
					{
						v_sell_list.push_back(s0);
					}
				}
		}
		return true;
	}
	return false;
}

testAPI::~testAPI()
{
	
}
//positionNum   ��Ʊֻ��
bool testAPI::ComputeBuyStockNum(int positionNum,vector<string> v_buy_list,map<string , int>& m_buy_list)
{
	//������������
	m_buy_list.clear();
	AccountInfo acinfo;
	if (v_buy_list.size() <= 0)
	{
		cout << "û�������Ʊ����" << endl;
		return true;
	}
	if (tdApi.QueryAccountMoney(acinfo))
	{
		double totalValue = acinfo.totalValue * 0.95;
		int i = 0;
		StockPrice sp;
		int stockNum;
		double price;
		while (i < v_buy_list.size())
		{
			if (!tdApi.get_price(v_buy_list[i], sp))
			{
				return false;
			}

			price = 0;
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;
			stockNum = 0;
			stockNum = (int)(totalValue / positionNum / price / 100)* 100;
			m_buy_list.insert(make_pair(v_buy_list[i], stockNum));
			i++;
		}
		//���ݳֲ��ٸ���һ�������б�
		map<string, int> now_position;
		tdApi.QueryPosition(now_position);
		if (now_position.size() == 0)
		{
			cout << "�˻�û���κγֲֹ�Ʊ" << endl;
			return true;
		}
		map<string, int>::iterator iter = m_buy_list.begin();
		map<string, int>::iterator find_iter, temp_iter;
		for (iter; iter != m_buy_list.end(); )
		{
			find_iter = now_position.find(iter->first);
			if (find_iter != now_position.end())
			{
				//���������Ʊ����
				iter->second = iter->second - find_iter->second;
				if (iter->second <= 0)
				{
					temp_iter = iter;
					iter = m_buy_list.erase(temp_iter);
				}
				else
					iter++;
			}
			else
				iter++;
		}
		return true;
	}
	cout << "���������Ʊ����ʧ��" << endl;
	return false;
}

bool testAPI::ComputeSellStockNum(vector<string> v_sell_list, map<string, int>& m_sell_list)
{
	m_sell_list.clear();
	map<string, int> stock_position;
	if (tdApi.QueryPosition(stock_position))
	{
		int i = 0, v_size = v_sell_list.size();
		map<string,int>::iterator it;
		while (i < v_size)
		{
			it = stock_position.find(v_sell_list[i]);
			if( it != stock_position.end() && it->second > 0)
			{
				m_sell_list.insert(make_pair(it->first, it->second));
			}
			i++;
		}
		return true;
	}
	return false;
}

void testAPI::test()
{
	//this->tdApi.cancel_order("97456978");
	//vector<EnstrustInfo> x;
	//EnstrustInfo xx;
	//this->tdApi.QueryEntrustByid("97482325", xx);
	//this->readExgList()
	//this->ComputeBuyPerMin()
	//this->tdApi.cancel_order("97488957");
	AccountInfo ac;
	StockPrice sp;
	this->tdApi.QueryAccountMoney(ac);
	//this->tdApi.get_price("600350",sp);
	string re;
	//tdApi.sell_stock("600350", 100, 6.0, re);
	cout << "xxxx" << endl;
}


map<string, vector<int>> testAPI::ComputeBuyPerMin(map<string, int> m_buy_list, int ExgValue)
{
	map<string, vector<int>> m_buy_vector;
	if (m_buy_list.size() > 0)
	{
		map<string, int>::iterator iter = m_buy_list.begin();
		int exg_stocks, exg_stocks_last;
		vector<int> stocks_list;
		StockPrice sp;
		//���׹�Ʊ��Ŀ����Ʊ����۸�
		double price;
		//���״���
		int exg_num;
		for (iter; iter != m_buy_list.end(); iter++)
		{
			if (!tdApi.get_price(iter->first, sp))
			{
				m_buy_vector.clear();
				return m_buy_vector;
			}
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;
			//���׹�Ʊ��Ŀ
			exg_stocks = 100 * (int)(ExgValue / price / 100);
			//���״�����ֻȡ������
			exg_num = (int)iter->second / exg_stocks;
			exg_stocks_last = iter->second - exg_stocks * (exg_num - 1);
			stocks_list.clear();
			for (int i = 0; i < exg_num; i++)
			{
				if (i == exg_num - 1)
					stocks_list.push_back(exg_stocks_last);
				else
					stocks_list.push_back(exg_stocks);
			}
			m_buy_vector.insert(make_pair(iter->first, stocks_list));
		}
	}
	return m_buy_vector;
}

map<string, vector<int>> testAPI::ComputeSellPerMin(map<string, int> m_sell_list, int ExgValue)
{
	map<string, vector<int>> m_sell_vector;
	if (m_sell_list.size() > 0)
	{
		int exg_stocks, exg_stocks_last;
		vector<int> stocks_list;
		StockPrice sp;
		//���׹�Ʊ��Ŀ����Ʊ����۸�
		double price;
		//���״���
		int exg_num;
		map<string, int>::iterator iter = m_sell_list.begin();
		for (iter; iter != m_sell_list.end(); iter++)
		{
			if (!tdApi.get_price(iter->first, sp))
			{
				m_sell_vector.clear();
				return m_sell_vector;
			}
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;

			//���׹�Ʊ��Ŀ
			exg_stocks = 100 * (int)(ExgValue / price / 100);
			//���״�����ֻȡ������
			exg_num = (int)iter->second / exg_stocks;
			exg_stocks_last = iter->second - exg_stocks * (exg_num - 1);
			stocks_list.clear();
			if (exg_num == 0)
			{
				stocks_list.push_back(iter->second);
			}
			else
			{
				for (int i = 0; i < exg_num; i++)
				{
					if (i == exg_num - 1)
						stocks_list.push_back(exg_stocks_last);
					else
						stocks_list.push_back(exg_stocks);
				}

			}
			m_sell_vector.insert(make_pair(iter->first, stocks_list));
		}
	}
	return m_sell_vector;
}

bool testAPI::ExgPerMin(int perSeconds, int ExgValue, map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num)
{
	//�жϽ����б�
	int exg_ser = 0;
	int sell_size = sell_list_num.size(), buy_size = buy_list_num.size();
	if (sell_size == 0 && buy_size == 0)
	{
		cout << "û���κν�����Ϣ" << endl;
		return true;
	}
	//���
	//������Ҫ���׵�������
	int ser_num = 0, time_num = 0;
	int max_ser_num = 0;
	map<string, vector<int>>::iterator exg_ter;
	for (exg_ter = buy_list_num.begin(); exg_ter != buy_list_num.end(); exg_ter++)
	{
		max_ser_num = max(max_ser_num, exg_ter->second.size());
	}

	for (exg_ter = sell_list_num.begin(); exg_ter != sell_list_num.end(); exg_ter++)
	{
		max_ser_num = max(max_ser_num, exg_ter->second.size());
	}

	//��ʼ�����ж�
	time_t time_now, time_exg, time_start;	//��ǰʱ�䣬�´ν���ʱ��

	map<string, vector<int>>::iterator buy_iter, sell_iter;
	StockPrice sp;
	//ί�б��
	string vtbh;
	//δ�ɽ���Ʊ����
	int not_exg_num = 0;
	//��ʼ��ʱ��
	//��ǰ����ʱ��
	time(&time_now);
	//��ʼʱ��
	time_start = time_now;
	time_exg = time_start;
	int error_time = 0, max_error_time = 10;

	struct tm* tm_now = NULL;
	struct tm* tm_exg = NULL;

	do
	{
		tm_now = localtime(&time_now);
		//cout << "����ʱ�䣺 " << tm_now->tm_hour<<":"<<tm_now->tm_min<<":"<<tm_now->tm_sec << endl;
		//�ǽ���ʱ��
		if (!Toolkit::T_isExgTme(time_now))
		{
			cout << "����ʱ�䣺 " << tm_now->tm_hour << ":" << tm_now->tm_min << ":" << tm_now->tm_sec << endl;
			//�ǽ���ʱ��
			cout << "�ǽ���ʱ��" << endl;
			Sleep(15000);
			//���ÿ�ʼʱ��
			time_num = 0;
			//���¸���ʱ��
			time(&time_now);
			time_start = time_now;
		}
		else //����ʱ��
		{
			//������һ�ν���ʱ�䣬��ʼ����
			if (time_now >= time_exg)
			{

				sell_iter = sell_list_num.begin();
				buy_iter = buy_list_num.begin();
				//for (exg_ser = 0; exg_ser < sell_size || exg_ser < buy_size; exg_ser++)
				for (sell_iter, buy_iter; sell_iter != sell_list_num.end() || buy_iter != buy_list_num.end();)
				{
					//���������б��еĹ�Ʊ,һ�α������й�Ʊ��ִ�����е�һ�ν��״���
					//if (exg_ser < sell_size)
					//�����б�Ĺ�Ʊ��û������
					if (sell_iter != sell_list_num.end())
					{
						//ʵ�ʽ��׵Ĵ����Ƿ񳬹��ܽ��״���
						if (ser_num < sell_iter->second.size())
						{
							//��������ֵ
							not_exg_num = sell_iter->second[ser_num];
							error_time = 0;
							//do
							//{
							//��ȡ�۸�
							if (!tdApi.get_price(sell_iter->first, sp))
							{
								return false;
							}
							//9������5λ�ã�0����ļ۸�1��������
							tdApi.sell_stock(sell_iter->first, not_exg_num, sp.v_prices[9][0], vtbh);
							cout << "��" << ser_num << "������:" << sell_iter->first << "  " << not_exg_num << " ��"<< endl;;
							//not_exg_num = tdApi.QueryIsExgByid(vtbh);
							//error_time++;
							//if (error_time > max_error_time)
							//{
							//	cout << sell_iter->first << ":���׳���" << endl;
							//	return false;
							//}
							//} while (not_exg_num != 0);//����ί��û�гɽ����������µ�
						}
						//��һֻ��Ʊ
						sell_iter++;
					}

					//���������б��еĹ�Ʊ
					//if (exg_ser < buy_size)
					if (buy_iter != buy_list_num.end())
					{
						if (ser_num < buy_iter->second.size())
						{
							//��������ֵ
							not_exg_num = buy_iter->second[ser_num];
							error_time = 0;
							//do
							//{
							//��ȡ�۸�
							if (!tdApi.get_price(buy_iter->first, sp))
							{
								return false;
							}
							//0������5λ�ã�0����ļ۸�1��������
							tdApi.buy_stock(buy_iter->first, not_exg_num, sp.v_prices[0][0], vtbh);
							cout << "��" << ser_num << "������:" << buy_iter->first << "  " << not_exg_num << " ��" << endl;
							//not_exg_num = tdApi.QueryIsExgByid(vtbh);
							//error_time++;
							//if (error_time > max_error_time)
							//{
							//	cout << sell_iter->first << ":���׳���" << endl;
							//	return false;
							//}
							//} while (not_exg_num != 0);//����ί��û�гɽ����������µ�

						}
						//��һֻ��Ʊ
						buy_iter++;
					}
				}
				//���һ�ֽ���
				ser_num++;
				//��һ�ν���ʱ��
				time_num++;
				cout << endl;
			}
			else //û������һ�ν���ʱ�䣬�����ȴ������µ�ǰʱ��
			{
				//cout << "�ȴ���һ�ν�������" << endl;
			}
			//���ֽ�����ɣ�������һ�ν���ʱ��
			
			//���¸���ʱ��
			time(&time_now);
		}
		//�´ν���ʱ��
		time_exg = time_start + ser_num * perSeconds;
		tm_exg = localtime(&time_exg);
		//cout << "�´ν���ʱ�䣺 " << tm_exg->tm_hour << ":" << tm_exg->tm_min << ":" << tm_exg->tm_sec << endl;
		if (time_now - time_exg > (ser_num + 1)*perSeconds)
		{
			//Խ��һ������ʱ�䣬���ÿ�ʼʱ��
			time_start = time_now;
		}
	} while (ser_num < max_ser_num);
	
	cout << "�������" << endl;
	return true;
}

int main()
{
	map<string, int> map_ip_p;
	map_ip_p.insert(make_pair("202.69.19.56", 7738));
	map_ip_p.insert(make_pair("116.228.52.78", 7738));
	map_ip_p.insert(make_pair("202.69.19.56", 7708));
	map_ip_p.insert(make_pair("116.228.52.78", 7708));
	map_ip_p.insert(make_pair("124.117.242.37", 7738));
	map_ip_p.insert(make_pair("124.117.242.38", 7738));
	map_ip_p.insert(make_pair("202.69.19.56", 80));
	map_ip_p.insert(make_pair("116.228.52.71", 80));

	cout << "���Ӳ���..." << endl;
	vector<string> v_ip;
	map<string, int>::iterator iter = map_ip_p.begin();
	for (iter; iter != map_ip_p.end();iter++)
	{
		v_ip.push_back(iter->first);
	}
	string selectedIP = Toolkit::T_selectIp(v_ip);
	cout <<"���IP��ַ��"<< selectedIP << endl;
	if (selectedIP.length() == 0)
	{
		cout << "IP��ַ����" << endl;
		return 0;
	}
	iter = map_ip_p.find(selectedIP);

	//���ֲֹ�Ʊ��Ŀ
	int positionNum = 10;
	//һ�ν��׽��
	int ExgValue = 21000;
	//ÿ�ν��׼��ʱ��  ��λ��
	int ExgPerSeconds = 90;

	Sleep(2000);

	//�����ڶ����˻�
	
	testAPI tapi_1;
	bool result_1 = tapi_1.Init(selectedIP, iter->second, "", "0", "309219037550", "651086");
	if (result_1)
	{
		vector<string> sell_list_1, buy_list_1;
		//tapi_1.test();
		tapi_1.readExgList("D:\\ExgFile\\ExgFile_1.txt", buy_list_1, sell_list_1);
		map<string, int> m_buy_list_1, m_sell_list_1;
		tapi_1.ComputeBuyStockNum(positionNum, buy_list_1, m_buy_list_1);
		tapi_1.ComputeSellStockNum(sell_list_1, m_sell_list_1);
		map<string, vector<int>> map_v_buy_1, map_v_sell_1;
		map_v_buy_1 = tapi_1.ComputeBuyPerMin(m_buy_list_1, ExgValue);
		map_v_sell_1 = tapi_1.ComputeSellPerMin(m_sell_list_1, ExgValue);
		tapi_1.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy_1, map_v_sell_1);
	}
	tapi_1.~testAPI();
	
	
	
	//������һ���˻�
	testAPI tapi;
	result_1 = tapi.Init(selectedIP, iter->second, "", "0", "309719208370", "651086");
	if (result_1)
	{
		vector<string> sell_list, buy_list;
		tapi.readExgList("D:\\ExgFile\\ExgFile_1.txt", buy_list, sell_list);
		map<string, int> m_buy_list, m_sell_list;
		tapi.ComputeBuyStockNum(positionNum, buy_list, m_buy_list);
		tapi.ComputeSellStockNum(sell_list, m_sell_list);
		map<string, vector<int>> map_v_buy, map_v_sell;
		map_v_buy = tapi.ComputeBuyPerMin(m_buy_list, ExgValue);
		map_v_sell = tapi.ComputeSellPerMin(m_sell_list, ExgValue);

		tapi.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell);
	}
	//����
	tapi.~testAPI();


	return 0;
}


