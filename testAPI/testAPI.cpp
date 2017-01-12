#include "testAPI.h"

#include "ThostFtdcTraderApi.h"

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
	cout <<"读取交易数据:"<< fileAdress.c_str() << endl;
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
//positionNum   股票只数
//min_exgMoney  最小交易金额
//position 交易仓位，1全仓,0.5半仓
bool testAPI::ComputeBuyStockNum(double retainedf,double position, int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list,double min_exgMoney)
{
	//记录最新价格信息
	map<string, double> map_sp;
	//计算买入数量
	m_buy_list.clear();
	AccountInfo acinfo;
	if (v_buy_list.size() <= 0)
	{
		cout << "没有买入股票数据" << endl;
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
			if (!tdApi.get_price_tx(v_buy_list[i], sp))
			{
				return false;
			}

			price = 0;
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;
			map_sp.insert(make_pair(v_buy_list[i], price));
			stockNum = 0;
			stockNum = (int)(totalValue / positionNum / price / 100)* 100;
			m_buy_list.insert(make_pair(v_buy_list[i], stockNum));
			i++;
		}
		//根据持仓再更新一次买入列表
		map<string, int> now_position;
		tdApi.QueryPosition(now_position);
		if (now_position.size() == 0)
		{
			cout << "账户没有任何持仓股票" << endl;
			return true;
		}
		map<string, int>::iterator iter = m_buy_list.begin();
		map<string, int>::iterator find_iter, temp_iter;
		for (iter; iter != m_buy_list.end(); )
		{
			find_iter = now_position.find(iter->first);
			if (find_iter != now_position.end())
			{
				//更新买入股票数量
				iter->second = iter->second - find_iter->second;
				if (iter->second * map_sp.at(iter->first) < min_exgMoney)
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
	cout << "计算买入股票数量失败" << endl;
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
	
	//this->tdApi.get_price_tx("600350",sp);
	string re;
	//tdApi.sell_stock("600350", 100, 6.0, re);
	cout << "xxxx" << endl;
}


map<string, vector<int>> testAPI::ComputeBuyPerMin(map<string, int> m_buy_list, double ExgValue)
{
	map<string, vector<int>> m_buy_vector;
	if (m_buy_list.size() > 0)
	{
		map<string, int>::iterator iter = m_buy_list.begin();
		int exg_stocks, exg_stocks_last;
		vector<int> stocks_list;
		StockPrice sp;
		//交易股票数目，股票估算价格
		double price;
		//交易次数
		int exg_num;
		for (iter; iter != m_buy_list.end(); iter++)
		{
			if (!tdApi.get_price_tx(iter->first, sp))
			{
				m_buy_vector.clear();
				return m_buy_vector;
			}
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;
			//交易股票数目
			exg_stocks = 100 * (int)(ExgValue / price / 100);
			//交易次数，只取整数次
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

map<string, vector<int>> testAPI::ComputeSellPerMin(map<string, int> m_sell_list, double ExgValue)
{
	map<string, vector<int>> m_sell_vector;
	if (m_sell_list.size() > 0)
	{
		int exg_stocks, exg_stocks_last;
		vector<int> stocks_list;
		StockPrice sp;
		//交易股票数目，股票估算价格
		double price;
		//交易次数
		int exg_num;
		map<string, int>::iterator iter = m_sell_list.begin();
		for (iter; iter != m_sell_list.end(); iter++)
		{
			if (!tdApi.get_price_tx(iter->first, sp))
			{
				m_sell_vector.clear();
				return m_sell_vector;
			}
			if (sp.NewPrice == 0)
				price = sp.LastClose;
			else
				price = sp.NewPrice;

			//交易股票数目
			exg_stocks = 100 * (int)(ExgValue / price / 100);
			//交易次数，只取整数次
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

bool testAPI::ExgPerMin(int perSeconds, int ExgValue, map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min,int max_ser,vector<time_t> exgtime_list)
{
	//判断交易列表
	int exg_ser = 0;
	int sell_size = sell_list_num.size(), buy_size = buy_list_num.size();
	if (sell_size == 0 && buy_size == 0)
	{
		cout << "没有任何交易信息" << endl;
		return true;
	}
	//序号
	//计算需要交易的最大次数
	int ser_num = 0, time_num = 0;
	int max_ser_num = max_ser;
	/*
	map<string, vector<int>>::iterator exg_ter;
	for (exg_ter = buy_list_num.begin(); exg_ter != buy_list_num.end(); exg_ter++)
	{
		max_ser_num = max(max_ser_num, exg_ter->second.size());
	}

	for (exg_ter = sell_list_num.begin(); exg_ter != sell_list_num.end(); exg_ter++)
	{
		max_ser_num = max(max_ser_num, exg_ter->second.size());
	}
	*/
	//开始交易判断
	time_t time_now, time_exg, time_start;	//当前时间，下次交易时间

	map<string, vector<int>>::iterator buy_iter, sell_iter;
	StockPrice sp;
	//委托编号
	string vtbh;
	//未成交股票数量
	int not_exg_num = 0;
	//初始化时间
	//当前最新时间
	time(&time_now);
	//开始时间
	time_start = time_now + floor(perSeconds / 2);
	time_exg = time_start;
	int error_time = 0, max_error_time = 10;

	struct tm* tm_now = NULL;
	struct tm* tm_exg = NULL;

	time_t time_temp;
	struct tm* tm_temp = NULL;
	//记录分钟数字
	int jilu_min = 0;
	int time_state = -1;
	do 
	{
		//刷新时间
		time(&time_now);
		//到达交易时间
		if (time_now >= exgtime_list[ser_num])
		{

			sell_iter = sell_list_num.begin();
			buy_iter = buy_list_num.begin();
			for (sell_iter, buy_iter; sell_iter != sell_list_num.end() || buy_iter != buy_list_num.end();)
			{
				//遍历卖出列表中的股票,一次遍历所有股票，执行其中的一次交易次数
				//if (exg_ser < sell_size)
				//卖出列表的股票还没遍历完
				if (sell_iter != sell_list_num.end())
				{
					//实际交易的次数是否超过总交易次数
					if (ser_num < sell_iter->second.size())
					{
						//交易量初值
						not_exg_num = sell_iter->second[ser_num];
						error_time = 0;
						//获取价格
						if (!tdApi.get_price_tx(sell_iter->first, sp))
						{
							Sleep(3600000);
							return false;
						}
						//获取时间
						time(&time_temp);
						tm_temp = localtime(&time_temp);
						cout << "时间： " << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
						cout << "第" << ser_num << "次卖出:" << sell_iter->first << "  " << not_exg_num << " 股" << endl;
						//9代表买5位置，0代表的价格，1代表数量
						//tdApi.sell_stock(sell_iter->first, not_exg_num, sp.v_prices[9][0], vtbh);
					}
					//下一只股票
					sell_iter++;
				}

				//遍历买入列表中的股票
				if (buy_iter != buy_list_num.end())
				{
					if (ser_num < buy_iter->second.size())
					{
						//交易量初值
						not_exg_num = buy_iter->second[ser_num];
						error_time = 0;

						if (!tdApi.get_price_tx(buy_iter->first, sp))
						{
							Sleep(3600000);
							return false;
						}
						//获取时间
						time(&time_temp);
						tm_temp = localtime(&time_temp);
						cout << "时间： " << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
						cout << "第" << ser_num << "次买入:" << buy_iter->first << "  " << not_exg_num << " 股" << endl;
						//0代表卖5位置，0代表的价格，1代表数量
						//tdApi.buy_stock(buy_iter->first, not_exg_num, sp.v_prices[0][0], vtbh);
					}
					//下一只股票
					buy_iter++;
				}
			}

			ser_num++;
		}
		else
		{
			tm_now = localtime(&time_now);
			if (tm_now->tm_sec == 0)
			{
				cout << "时间： " << tm_now->tm_hour << ":" << tm_now->tm_min << ":" << tm_now->tm_sec << endl;
				cout << "非交易时段" << endl;
				Sleep(1000);
			}
			
		}
	} while (ser_num < max_ser_num);

	cout << "交易完毕" << endl;
	return true;
}

double testAPI::return_value()
{
	AccountInfo acinfo;
	tdApi.QueryAccountMoney(acinfo);
	return acinfo.totalValue;
}
//计算交易之间的间隔时间，单位秒
int testAPI::ComputePerSeconds(map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min, int& max_ser,double part_time)
{
	//序号
	//计算需要交易的最大次数
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

	max_ser = max_ser_num;

	time_t time_now;
	//初始化时间
	//当前最新时间
	time(&time_now);
	//总的交易持续时间
	int exg_seconds = 0;
	int time_state = Toolkit::T_isExgTme(time_now, add_min);
	if (time_state == 0)
	{
		exg_seconds = (4 * 3600 - add_min * 60) * part_time;
	}
	
	tm* tm_now = localtime(&time_now);
	tm tm_end = *tm_now;
	time_t time_end;
	//time_t time_start0, time_end0, time_start1, time_end1;
	tm_end.tm_hour = 15;
	tm_end.tm_min = 0;
	tm_end.tm_sec = 0;
	time_end = mktime(&tm_end);

	if (time_state == 1)
	{
		exg_seconds = (time_end - time_now - 1.5 * 3600 - add_min * 60) * part_time;
	}

	if (time_state == 2)
	{
		exg_seconds = (2 * 3600 - add_min * 60) * part_time;
	}

	if (time_state == 3)
	{
		exg_seconds = (time_end - time_now) * part_time;
	}

	if (time_state == 4 || max_ser_num == 0)
	{
		return 36000;
	}
	return floor(exg_seconds / max_ser_num);
}

vector<time_t> testAPI::ComputeExgtime_list(int perSeconds, int add_min, int max_ser)
{
	vector<time_t> reslut_list;
	if (max_ser == 0)
	{
		return reslut_list;
	}
	
	time_t time_now, time_startexg, time_start0,time_start1;
	tm tm_start;
	//初始化时间
	//当前最新时间
	time(&time_now);
	tm* tm_now = localtime(&time_now);
	tm tm_start0 = *tm_now;
	tm_start0.tm_hour = 9;
	tm_start0.tm_min = 30 + add_min;
	tm_start0.tm_sec = 0;
	
	//上午开盘时间
	time_start0 = mktime(&tm_start0);

	//tm_start = *tm_now;
	//上午开盘之前
	int time_state = Toolkit::T_isExgTme(time_now, add_min);
	if (time_state == 0)
	{
		time_startexg = time_start0 + floor(perSeconds / 2);
	}
	//上午已经开盘
	if (time_state == 1)
	{
		time_startexg = time_now + floor(perSeconds / 2);
		//开始交易时间为午盘休息
		if (Toolkit::T_isExgTme(time_startexg, add_min) == 2)
		{
			time_startexg = time_now + floor(perSeconds / 2) + 1.5 * 3600;
		}
	}
	
	tm tm_start1 = *tm_now;
	tm_start1.tm_hour = 13;
	tm_start1.tm_min = 00 + add_min;
	tm_start1.tm_sec = 0;
	//下午开盘时间
	time_start1 = mktime(&tm_start1);
	//午盘休息
	if (time_state == 2)
	{
		time_startexg = time_start1 + floor(perSeconds / 2);
	}
	

	//下午已经开盘
	if (time_state == 3)
	{
		time_startexg = time_now + floor(perSeconds / 2);
	}

	//下午已经收盘
	if (time_state == 4)
	{
		return reslut_list;
	}
	reslut_list.push_back(time_startexg);
	tm* tm_temp = localtime(&time_startexg);
	cout << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
	while (reslut_list.size() < max_ser)
	{
		time_startexg = time_startexg + perSeconds;
		if (Toolkit::T_isExgTme(time_startexg, add_min) == 2)
		{
			time_startexg = time_startexg + floor(perSeconds / 2) + 1.5 * 3600;
		}
		tm* tm_temp = localtime(&time_startexg);
		cout << tm_temp->tm_hour << ":" << tm_temp->tm_min << ":" << tm_temp->tm_sec << endl;
		reslut_list.push_back(time_startexg);
	}
	return reslut_list;
}

int testAPI::Ini_exg(string ip, int port, vector<string> id_list, vector<string> key_list, vector<string> exgfile_list, vector<double> Retained_funds_list,
	double position, int positionNum, double ExgValue, double min_exgMoney, int add_min, double part_time)
{
	bool result_1 = false;
	//文件操作
	//记录市值数据
	ofstream fileout("D:\\ExgFile\\total_value.txt", ios::trunc);
	if (!fileout)
	{
		cout << "Create file failure...\n";
		exit(0);
	}

	//获取hs300指数的最新价格
	TradeApi tradeapi_0;
	StockPrice sp;
	tradeapi_0.get_price_tx("000300", sp);
	fileout << sp.NewPrice << endl;
	vector<double> money_list;
	for (int i = 0; i < id_list.size(); i++)
	{
		
		result_1 = this->Init(ip, port, "", "0", id_list[i], key_list[i]);
		if (result_1)
		{
			money_list.push_back(this->return_value());
			vector<string> sell_list_1, buy_list_1;
			this->readExgList(exgfile_list[i], buy_list_1, sell_list_1);
			map<string, int> m_buy_list, m_sell_list;
			this->ComputeBuyStockNum(Retained_funds_list[i], position, positionNum, buy_list_1, m_buy_list, min_exgMoney);
			this->ComputeSellStockNum(sell_list_1, m_sell_list);

			cout << endl;
			cout << "卖出股票列表:" << endl;
			for (map<string, int>::iterator iter_0 = m_sell_list.begin(); iter_0 != m_sell_list.end(); iter_0++)
			{
				cout << iter_0->first << ":" << iter_0->second << endl;
			}
			cout << endl;
			cout << "买入股票列表:" << endl;
			for (map<string, int>::iterator iter_0 = m_buy_list.begin(); iter_0 != m_buy_list.end(); iter_0++)
			{
				cout << iter_0->first << ":" << iter_0->second << endl;
			}
			cout << endl;

			map<string, vector<int>> map_v_buy, map_v_sell;
			map_v_buy = this->ComputeBuyPerMin(m_buy_list, ExgValue);
			map_v_sell = this->ComputeSellPerMin(m_sell_list, ExgValue);
			int max_ser;
			int ExgPerSeconds = this->ComputePerSeconds(map_v_buy, map_v_sell, add_min, max_ser, part_time);
			vector<time_t> exgtime_list = this->ComputeExgtime_list(ExgPerSeconds, add_min, max_ser);
			if (!exgtime_list.empty())
				this->ExgPerMin(ExgPerSeconds, ExgValue, map_v_buy, map_v_sell, add_min, max_ser, exgtime_list);
			cout << id_list[i] << "交易完成" << endl;
			if (i != id_list.size() - 1 && !(exgtime_list.empty()))
			{
				cout << "等待下一个账户交易" << endl;
				Sleep(floor(ExgPerSeconds / 2));
			}
		}
		//this->~testAPI();
		cout << endl;
	}

	vector<double>::iterator iter0;
	int i = 0;
	double total_value = 0;
	for (iter0 = money_list.begin(); iter0 != money_list.end(); iter0++, i++)
	{
		cout << std::fixed << money_list[i] << endl;
		fileout << std::fixed << money_list[i] << endl;
		total_value = total_value + money_list[i];
	}
	cout << total_value << endl;


	Sleep(3600000);
	//文件关闭
	fileout.close();
	return 0;
}

int main()
{
	cout << "版本号：" << "1_2" << endl;
	//ip地址列表
	map<string, int> map_ip_p;
	map_ip_p.insert(make_pair("202.69.19.56", 7738));
	map_ip_p.insert(make_pair("116.228.52.78", 7738));
	map_ip_p.insert(make_pair("202.69.19.56", 7708));
	map_ip_p.insert(make_pair("116.228.52.78", 7708));
	map_ip_p.insert(make_pair("124.117.242.37", 7738));
	map_ip_p.insert(make_pair("124.117.242.38", 7738));
	map_ip_p.insert(make_pair("202.69.19.56", 80));
	map_ip_p.insert(make_pair("116.228.52.71", 80));

	cout << "连接测试..." << endl;
	vector<string> v_ip;
	map<string, int>::iterator iter = map_ip_p.begin();
	for (iter; iter != map_ip_p.end();iter++)
	{
		v_ip.push_back(iter->first);
	}
	string selectedIP = Toolkit::T_selectIp(v_ip);
	cout <<"最佳IP地址："<< selectedIP << endl;
	if (selectedIP.length() == 0)
	{
		cout << "IP地址有误" << endl;
		return 0;
	}
	iter = map_ip_p.find(selectedIP);
	//相应的端口号
	int port = iter->second;

	//最大持仓股票数目
	int positionNum = 10;
	//一次交易金额
	double ExgValue = 22000;
	//最小买入金额
	//暂定最小买入金额为3000
	//目的是为了过滤较小的仓差交易，增大交易费用
	double min_exgMoney = 3000;
	//每次交易间隔时间  单位秒
	//int ExgPerSeconds = 36000;
	//
	int add_min = 4;
	cout << "单账户占用交易时间份数(1==》所有交易时间，0.5==》一半时间)：" << endl;
	double part_time = 1;
	cin >> part_time;
	//交易仓位
	double position = 1;
	cout << "持仓股票数目：" << positionNum << endl;
	cout << "单次交易金额：" << ExgValue << endl;
	cout << "最小买入金额：" << min_exgMoney << endl;
	//cout << "交易间隔时间：" << ExgPerSeconds <<"秒"<< endl;
	Sleep(1000);
	

	
	//帐号列表，密码列表，交易信息列表
	vector<string> id_list,key_list,exgfile_list;
	id_list.push_back("309219037550");
	id_list.push_back("309719208370");
	id_list.push_back("309219088510");
	id_list.push_back("309219171085");

	key_list.push_back("651086");
	key_list.push_back("651086");
	key_list.push_back("651086");
	key_list.push_back("651086");

	exgfile_list.push_back("D:\\ExgFile\\1_ExgFile_zyj.txt");
	exgfile_list.push_back("D:\\ExgFile\\4_ExgFile_zb.txt");
	exgfile_list.push_back("D:\\ExgFile\\8_ExgFile_jcp.txt");
	exgfile_list.push_back("D:\\ExgFile\\10_ExgFile_zjj.txt");

	vector<double> Retained_funds_list;
	Retained_funds_list.push_back(250000);
	Retained_funds_list.push_back(250000);
	Retained_funds_list.push_back(250000);
	Retained_funds_list.push_back(250000);

	testAPI tapi_1;
	tapi_1.Ini_exg(selectedIP, port, id_list, key_list, exgfile_list, Retained_funds_list, position, positionNum, ExgValue, min_exgMoney,add_min,part_time);
	return 0;
}


