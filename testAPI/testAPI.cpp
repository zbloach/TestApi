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
	cout <<"��ȡ��������:"<< fileAdress.c_str() << endl;
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
bool testAPI::ComputeBuyStockNum(double retainedf,double position, int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list)
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
		double realMoney = acinfo.totalValue - retainedf;
		if (realMoney < 0)
		{
			realMoney = 0;
		}
		double totalValue = realMoney * 0.95 * position;
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
				if (iter->second <= 100)
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
	//AccountInfo ac;
	//StockPrice sp;
	//this->tdApi.QueryAccountMoney(ac);
	StockPrice sp;
	int i = 0;
	vector<string> stock_list;
	stock_list.push_back("002394");
	stock_list.push_back("600060");
	stock_list.push_back("600350");
	stock_list.push_back("600684");
	stock_list.push_back("000581");
	stock_list.push_back("000726");
	stock_list.push_back("000936");
	stock_list.push_back("600548");
	stock_list.push_back("600761");
	stock_list.push_back("601107");
	while (i < 1000)
	{
		for (int j = 0; j < stock_list.size(); j++)
		{
			tdApi.get_price(stock_list[j], sp);
		}
		i++;
	}
	
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
			stocks_list.clear();
			if (exg_num == 0)
			{
				stocks_list.push_back(iter->second);
			} 
			else
			{
				exg_stocks_last = iter->second - exg_stocks * (exg_num - 1);
				for (int i = 0; i < exg_num; i++)
				{
					if (i == exg_num - 1)
						stocks_list.push_back(exg_stocks_last);
					else
						stocks_list.push_back(exg_stocks);
				}
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

	time_t time_temp;
	struct tm* tm_temp = NULL;
	//��¼��������
	int jilu_min = 0;

	do
	{
		tm_now = localtime(&time_now);
		//�ǽ���ʱ��
		if (!Toolkit::T_isExgTme(time_now))
		{
			cout << "ʱ�䣺 " << tm_now->tm_hour << ":" << tm_now->tm_min << ":" << tm_now->tm_sec << endl;
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
							//��ȡ�۸�
							if (!tdApi.get_price(sell_iter->first, sp))
							{
								Sleep(3600000);
								return false;
							}
							//��ȡʱ��
							time(&time_temp);
							tm_temp = localtime(&time_temp);
							cout << "ʱ�䣺 " << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
							cout << "��" << ser_num << "������:" << sell_iter->first << "  " << not_exg_num << " ��" << endl;
							//9������5λ�ã�0����ļ۸�1��������
							tdApi.sell_stock(sell_iter->first, not_exg_num, sp.v_prices[9][0], vtbh);
						}
						//��һֻ��Ʊ
						sell_iter++;
					}

					//���������б��еĹ�Ʊ
					if (buy_iter != buy_list_num.end())
					{
						if (ser_num < buy_iter->second.size())
						{
							//��������ֵ
							not_exg_num = buy_iter->second[ser_num];
							error_time = 0;

							if (!tdApi.get_price(buy_iter->first, sp))
							{
								Sleep(3600000);
								return false;
							}
							//��ȡʱ��
							time(&time_temp);
							tm_temp = localtime(&time_temp);
							cout << "ʱ�䣺 " << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
							cout << "��" << ser_num << "������:" << buy_iter->first << "  " << not_exg_num << " ��" << endl;
							//0������5λ�ã�0����ļ۸�1��������
							tdApi.buy_stock(buy_iter->first, not_exg_num, sp.v_prices[0][0], vtbh);
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
				time(&time_temp);
				tm_temp = localtime(&time_temp);
				if (jilu_min != tm_temp->tm_min)
				{
					//�������ʱ��
					cout << "ʱ�䣺 " << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
					jilu_min = tm_temp->tm_min;
				}
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

double testAPI::return_value()
{
	AccountInfo acinfo;
	tdApi.QueryAccountMoney(acinfo);
	return acinfo.totalValue;
}

int main()
{
	vector<double> money_list;
	cout << "�汾�ţ�" << "1_2" << endl;
	//ip��ַ�б�
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
	//��Ӧ�Ķ˿ں�
	int port = iter->second;

	//���ֲֹ�Ʊ��Ŀ
	int positionNum = 10;
	//һ�ν��׽��
	int ExgValue = 22000;
	//ÿ�ν��׼��ʱ��  ��λ��
	int ExgPerSeconds = 400;

	cout << "�����뽻�׼��ʱ��(��λ����)..." << endl;
	cin >> ExgPerSeconds;
	//���ײ�λ
	double position = 1;
	cout << "�ֲֹ�Ʊ��Ŀ��" << positionNum << endl;
	cout << "���ν��׽�" << ExgValue << endl;
	cout << "���׼��ʱ�䣺" << ExgPerSeconds <<"��"<< endl;
	Sleep(1000);
	bool result_1 = false;

	//����1_ExgFile_zyj�˻�
	
	testAPI tapi_1;
	result_1 = tapi_1.Init(selectedIP, port, "", "0", "309219037550", "651086");
	if (result_1)
	{
		money_list.push_back(tapi_1.return_value());
		//�˻������ʽ�
		double Retained_funds = 50000;
		vector<string> sell_list_1, buy_list_1;
		//tapi_1.test();
		tapi_1.readExgList("D:\\ExgFile\\1_ExgFile_zyj.txt", buy_list_1, sell_list_1);
		map<string, int> m_buy_list, m_sell_list;
		tapi_1.ComputeBuyStockNum(Retained_funds, position, positionNum, buy_list_1, m_buy_list);
		tapi_1.ComputeSellStockNum(sell_list_1, m_sell_list);

		cout << endl;
		cout << "������Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_sell_list.begin(); iter_0 != m_sell_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;
		cout << "�����Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_buy_list.begin(); iter_0 != m_buy_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;

		map<string, vector<int>> map_v_buy, map_v_sell;
		map_v_buy = tapi_1.ComputeBuyPerMin(m_buy_list, ExgValue);
		map_v_sell = tapi_1.ComputeSellPerMin(m_sell_list, ExgValue);
		tapi_1.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell);
		cout << "309219037550" << "�������" << endl;
		if (!(buy_list_1.empty() && buy_list_1.empty()))
		{
			cout << "�ȴ���һ���˻�����" << endl;
			Sleep(ExgPerSeconds * 1000);
		}
	}
	tapi_1.~testAPI();
	cout << endl;

	//����4_ExgFile_zb�˻�
	testAPI tapi;
	result_1 = tapi.Init(selectedIP, port, "", "0", "309719208370", "651086");
	if (result_1)
	{
		money_list.push_back(tapi.return_value());
		double Retained_funds = 50000;
		vector<string> sell_list, buy_list;
		tapi.readExgList("D:\\ExgFile\\4_ExgFile_zb.txt", buy_list, sell_list);
		map<string, int> m_buy_list, m_sell_list;
		tapi.ComputeBuyStockNum(Retained_funds,position, positionNum, buy_list, m_buy_list);
		tapi.ComputeSellStockNum(sell_list, m_sell_list);

		cout << endl;
		cout << "������Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_sell_list.begin(); iter_0 != m_sell_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;
		cout << "�����Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_buy_list.begin(); iter_0 != m_buy_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;

		map<string, vector<int>> map_v_buy, map_v_sell;
		map_v_buy = tapi.ComputeBuyPerMin(m_buy_list, ExgValue);
		map_v_sell = tapi.ComputeSellPerMin(m_sell_list, ExgValue);

		tapi.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell);
		cout << "309719208370" << "�������" << endl;
		if (!(buy_list.empty() && sell_list.empty()))
		{
			cout << "�ȴ���һ���˻�����" << endl;
			Sleep(ExgPerSeconds * 1000);
		}
	}
	//����
	tapi.~testAPI();
	cout << endl;
	
	//����8_ExgFile_jcp�˻�
	testAPI tapi_2;
	result_1 = tapi_2.Init(selectedIP, port, "", "0", "309219088510", "651086");
	if (result_1)
	{
		money_list.push_back(tapi_2.return_value());
		double Retained_funds = 200000;
		vector<string> sell_list, buy_list;
		tapi_2.readExgList("D:\\ExgFile\\8_ExgFile_jcp.txt", buy_list, sell_list);
		map<string, int> m_buy_list, m_sell_list;
		tapi_2.ComputeBuyStockNum(Retained_funds,position, positionNum, buy_list, m_buy_list);
		tapi_2.ComputeSellStockNum(sell_list, m_sell_list);

		cout << endl;
		cout << "������Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_sell_list.begin(); iter_0 != m_sell_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;
		cout << "�����Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_buy_list.begin(); iter_0 != m_buy_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;

		map<string, vector<int>> map_v_buy, map_v_sell;
		map_v_buy = tapi_2.ComputeBuyPerMin(m_buy_list, ExgValue);
		map_v_sell = tapi_2.ComputeSellPerMin(m_sell_list, ExgValue);

		tapi_2.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell);
		cout << "309219088510" << "�������" << endl;
		if (!(buy_list.empty() && sell_list.empty()))
		{
			cout << "�ȴ���һ���˻�����" << endl;
			Sleep(ExgPerSeconds * 1000);
		}
	}
	//����
	tapi_2.~testAPI();
	cout << endl;
	
	//����10_ExgFile_zjj�˻�

	testAPI tapi_3;
	result_1 = tapi_3.Init(selectedIP, port, "", "0", "309219171085", "651086");
	if (result_1)
	{
		money_list.push_back(tapi_3.return_value());
		double Retained_funds = 0;
		vector<string> sell_list, buy_list;
		tapi_3.readExgList("D:\\ExgFile\\10_ExgFile_zjj.txt", buy_list, sell_list);
		map<string, int> m_buy_list, m_sell_list;
		tapi_3.ComputeBuyStockNum(Retained_funds, position, positionNum, buy_list, m_buy_list);
		tapi_3.ComputeSellStockNum(sell_list, m_sell_list);

		cout << endl;
		cout << "������Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_sell_list.begin(); iter_0 != m_sell_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;
		cout << "�����Ʊ�б�:" << endl;
		for (map<string, int>::iterator iter_0 = m_buy_list.begin(); iter_0 != m_buy_list.end(); iter_0++)
		{
			cout << iter_0->first << ":" << iter_0->second << endl;
		}
		cout << endl;

		map<string, vector<int>> map_v_buy, map_v_sell;
		map_v_buy = tapi_3.ComputeBuyPerMin(m_buy_list, ExgValue);
		map_v_sell = tapi_3.ComputeSellPerMin(m_sell_list, ExgValue);

		tapi_3.ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell);
		cout << "309219171085" << "�������" << endl;
	}
	//����
	tapi_3.~testAPI();
	cout << endl;
	vector<double>::iterator iter0;
	int i = 0;
	double total_value = 0;
	for (iter0 = money_list.begin(); iter0 != money_list.end(); iter0++, i++)
	{
		cout << std::fixed << money_list[i] << endl;
		total_value = total_value + money_list[i];
	}
	cout << total_value << endl;
	Sleep(3600000);
	return 0;
}


