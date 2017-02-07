#include "TradeApi.h"
#include "DataApi.h"

TradeApi::TradeApi()
{
	tradespi = new TradeSpi();
	m_serverinfo = { "", 0, "", "" };
	m_userinfo = { "", "", "","" };
}

bool TradeApi::Login(string userid, string password)
{
	m_userinfo.userid = userid;
	m_userinfo.password = password;
	bool result;
	int error_num = 0, sleeptime = 0;
	do 
	{
		if (error_num > 0)
		{
			sleeptime = error_num + 200;
			Sleep(sleeptime);
			cout << userid << ":第"<<error_num<<"次重新登录" << endl;
		}
		if (m_userinfo.userid == "309719208370")
		{
			result = tradespi->JL_Login_0(m_serverinfo.ip.c_str(), m_serverinfo.trade_server_port, userid.c_str(),
				password.c_str(), m_serverinfo.commu_key.c_str(), m_serverinfo.depart_code.c_str());
		}
		if (m_userinfo.userid == "309219037550")
		{
			result = tradespi->JL_Login_1(m_serverinfo.ip.c_str(), m_serverinfo.trade_server_port, userid.c_str(),
				password.c_str(), m_serverinfo.commu_key.c_str(), m_serverinfo.depart_code.c_str());
		}
		if (m_userinfo.userid == "309219088510")
		{
			result = tradespi->JL_Login_2(m_serverinfo.ip.c_str(), m_serverinfo.trade_server_port, userid.c_str(),
				password.c_str(), m_serverinfo.commu_key.c_str(), m_serverinfo.depart_code.c_str());
		}
		if (m_userinfo.userid == "309219171085")
		{
			result = tradespi->JL_Login_3(m_serverinfo.ip.c_str(), m_serverinfo.trade_server_port, userid.c_str(),
				password.c_str(), m_serverinfo.commu_key.c_str(), m_serverinfo.depart_code.c_str());
		}
		error_num++;
		if (error_num > 10)
		{
			cout << userid << ":登录失败，放弃登录" << endl;
			break;
		}
	} while (result == false);
	
	if (result)	//登录成功，记录股东代码
	{
		result = this->QueryShareholderCode();
		cout << userid << ":登陆成功" << endl;
	}
	return result;
}

void TradeApi::Init(const ServerInfo& info)
{
	m_serverinfo = info;
}

bool TradeApi::QueryAccountMoney(struct AccountInfo& ac_info)
{
	ac_info = { 0, 0, 0 };
	char* info = "";
	int error_num = 0;
	int sleepTime = 0;
	while (strlen(info) == 0)
	{
		//出错
		if (error_num > 0)
		{
			//重新登录
			//this->Login(m_userinfo.userid, m_userinfo.password);
			sleepTime = sleepTime + 200;
			Sleep(sleepTime);
			cout <<m_userinfo.userid<< ":第" << error_num << "次查询账户资金" << endl;
		}

		if (m_userinfo.userid == "309719208370")
		{
			info = tradespi->JL_QueryData_0(m_userinfo.userid.c_str(), accountInfo_id);
		}
		if (m_userinfo.userid == "309219037550")
		{
			info = tradespi->JL_QueryData_1(m_userinfo.userid.c_str(), accountInfo_id);
		}
		if (m_userinfo.userid == "309219088510")
		{
			info = tradespi->JL_QueryData_2(m_userinfo.userid.c_str(), accountInfo_id);
		}
		if (m_userinfo.userid == "309219171085")
		{
			info = tradespi->JL_QueryData_3(m_userinfo.userid.c_str(), accountInfo_id);
		}
		if (error_num > 50)
		{
			cout<<m_userinfo.userid << ":账户资金查询失败,放弃查询" << endl;
			return false;
		}
		error_num++;
	}
	
	if (strlen(info) != 0)
	{
		string S_info = info;
		vector<string> v_info = Toolkit::T_split(S_info, "|");
		ac_info.totalMoney = atof(v_info[26].c_str()) + atof(v_info[20].c_str());
		ac_info.totalStock = atof(v_info[30].c_str());
		ac_info.totalValue = atof(v_info[31].c_str()) + atof(v_info[20].c_str());
		return true;
	}
	return false;
}

TradeApi::~TradeApi()
{

}

bool TradeApi::QueryShareholderCode()
{
	char* info = NULL;
	if (m_userinfo.userid == "309719208370")
	{
		info = tradespi->JL_QueryData_0(m_userinfo.userid.c_str(), ShareholderCode_id);
	}
	if (m_userinfo.userid == "309219037550")
	{
		info = tradespi->JL_QueryData_1(m_userinfo.userid.c_str(), ShareholderCode_id);
	}
	if (m_userinfo.userid == "309219088510")
	{
		info = tradespi->JL_QueryData_2(m_userinfo.userid.c_str(), ShareholderCode_id);
	}
	if (m_userinfo.userid == "309219171085")
	{
		info = tradespi->JL_QueryData_3(m_userinfo.userid.c_str(), ShareholderCode_id);
	}
	if (strlen(info) != 0)
	{
		string S_info = info;
		vector<string> v_info = Toolkit::T_split(S_info, "|");
		m_userinfo.szid = v_info[11];
		m_userinfo.shid = v_info[19];
		return true;
	}
	else
		return false;
}

bool TradeApi::QueryPosition(map<string, int>& stock_position)
{
	stock_position.clear();
	char* info = NULL;
	if (m_userinfo.userid == "309719208370")
	{
		info = tradespi->JL_QueryData_0(m_userinfo.userid.c_str(), Position_id);
	}
	
	if (m_userinfo.userid == "309219037550")
	{
		info = tradespi->JL_QueryData_1(m_userinfo.userid.c_str(), Position_id);
	}

	if (m_userinfo.userid == "309219088510")
	{
		info = tradespi->JL_QueryData_2(m_userinfo.userid.c_str(), Position_id);
	}

	if (m_userinfo.userid == "309219171085")
	{
		info = tradespi->JL_QueryData_3(m_userinfo.userid.c_str(), Position_id);
	}
	if (strlen(info) != 0)
	{
		string S_info = info;
		vector<string> v_info = Toolkit::T_split(S_info, "|");
		int i = 25, j = 37, v_size = v_info.size();
		map<string, int>::iterator it;
		while (j < v_size)
		{
			it = stock_position.find(v_info[i]);
			if (it == stock_position.end())
			{
				//抛开没有持仓的股票   还有新股
				if (atoi(v_info[j].c_str()) != 0 && atoi(v_info[j].c_str()) != 1000 && atoi(v_info[j].c_str()) != 500)
				{
					stock_position.insert(make_pair(v_info[i], atoi(v_info[j].c_str())));
				}
			}
			else
			{
				if (atoi(v_info[j].c_str()) != 0)
				{
					it->second = atoi(v_info[j].c_str());
				} 
				else
				{
					//删除
					stock_position.erase(it);
				}
				
			}

			i += 23;
			j += 23;
		}
		//cout << S_info << endl;
		return true;
	}
	else
		return false;
}


bool TradeApi::QueryEntrust(vector<EnstrustInfo>& EntrustList)
{
	EntrustList.clear();
	string info;
	if (m_userinfo.userid == "309719208370")
	{
		info = tradespi->JL_QueryData_0(m_userinfo.userid.c_str(), Ensturst_id);
	}
	if (m_userinfo.userid == "309219037550")
	{
		info = tradespi->JL_QueryData_1(m_userinfo.userid.c_str(), Ensturst_id);
	} 
	if (m_userinfo.userid == "309219088510")
	{
		info = tradespi->JL_QueryData_2(m_userinfo.userid.c_str(), Ensturst_id);
	}
	if (m_userinfo.userid == "309219171085")
	{
		info = tradespi->JL_QueryData_3(m_userinfo.userid.c_str(), Ensturst_id);
	}
	if (info.length() != 0)
	{
		vector<string> v_info = Toolkit::T_split(info, "|");
		EnstrustInfo ei;
		int v_size = v_info.size();
		int k = 26,i = 0;
		while ((42 + i * k) < v_size)
		{
			ei = { "", "", "", 0, 0, 0, 0, 0, 0, 0, "" };
			ei.s_time = v_info[28 + i * k];
			ei.Enstrustid = v_info[29 + i * k];
			ei.stockid = v_info[30 + i * k];
			ei.op_direction = atoi(v_info[32 + i * k].c_str());
			ei.EntrustNum = atoi(v_info[36 + i * k].c_str());
			ei.EntrustPrice = atof(v_info[35 + i * k].c_str());
			ei.DealNum = atoi(v_info[39 + i * k].c_str());
			ei.DealPrice = atof(v_info[38 + i * k].c_str());
			ei.DealValue = atof(v_info[40 + i * k].c_str());
			ei.CancelNum = atoi(v_info[41 + i * k].c_str());
			ei.CancelSymbol = v_info[42 + i * k];
			EntrustList.push_back(ei);
			i++;
		}
		if (EntrustList.size() > 1)
			std::reverse(EntrustList.begin(), EntrustList.end());
		return true;
	}
	return false;
}

bool TradeApi::QueryNotDealEntrust(vector<EnstrustInfo>& EntrustList)
{
	bool result = QueryEntrust(EntrustList);
	if (result)
	{
		int i = 0, v_size = EntrustList.size();
		vector<EnstrustInfo>::iterator temp_it;
		for (vector<EnstrustInfo>::iterator it = EntrustList.begin(); it != EntrustList.end();)
		{
			if (it->EntrustNum == it->DealNum)	//委托数量等于成交数量
			{
				temp_it = it;
				it = EntrustList.erase(temp_it);
			}
			else
			{
				if (it->CancelSymbol == "已撤")
				{
					temp_it = it;
					it = EntrustList.erase(temp_it);
				}
				else
				{
					it++;
				}
			}
		}
		return true;
	}
	return false;
}


bool TradeApi::buy_stock(string stock_id, int stock_num, double price, string& result)
{
	int stockMarket = Toolkit::T_stockMarket(stock_id);
	string ShareholderCode = "";
	//1上交所股票，0深交所股票
	if (stockMarket == 1)
	{
		ShareholderCode = m_userinfo.shid;
	}
	else
		if (stockMarket == 0)
		{
		ShareholderCode = m_userinfo.szid;
		}
		else
		{
			result = "股票代码有误";
			return false;
		}

	if (m_userinfo.userid == "309719208370")
	{
		result = tradespi->JL_SendOrder_0(0, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219037550")
	{
		result = tradespi->JL_SendOrder_1(0, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219088510")
	{
		result = tradespi->JL_SendOrder_2(0, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219171085")
	{
		result = tradespi->JL_SendOrder_3(0, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	bool b_res = Toolkit::T_isNum(result);
	if (b_res)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TradeApi::sell_stock(string stock_id, int stock_num, double price, string& result)
{
	int stockMarket = Toolkit::T_stockMarket(stock_id);
	string ShareholderCode = "";
	if (stockMarket == 1)
	{
		ShareholderCode = m_userinfo.shid;
	}
	else
		if (stockMarket == 0)
		{
			ShareholderCode = m_userinfo.szid;
		}
		else
		{
			result = "股票代码有误";
			return false;
		}

	if (m_userinfo.userid == "309719208370")
	{
		result = tradespi->JL_SendOrder_0(1, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219037550")
	{
		result = tradespi->JL_SendOrder_1(1, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219088510")
	{
		result = tradespi->JL_SendOrder_2(1, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	if (m_userinfo.userid == "309219171085")
	{
		result = tradespi->JL_SendOrder_3(1, m_userinfo.userid.c_str(), ShareholderCode.c_str(), stock_id.c_str(), stock_num, (float)price);
	}
	bool b_res = Toolkit::T_isNum(result);
	if (b_res)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TradeApi::get_price(string stock_id, StockPrice& stockprice)
{
	//cout << "获取" << stock_id << "价格数据" << endl;
	vector<vector<double>> v_prices0;
	stockprice = { "", 0, 0, 0, v_prices0 };
	string S_info = "";
	int error_num = 0, sleeptime = 0;
	vector<string> v_info;
	do 
	{
		if (error_num > 0)
		{
			if (error_num % 4 == 0)
			{
				//重新登录，并且查询价格信息
				//Sleep(2000);
				cout << "重新登录" << endl;
				this->Login(m_userinfo.userid, m_userinfo.password);
			}
			cout << m_userinfo.userid << ":第" << error_num << "次重复查询:" << stock_id << "价格信息" << endl;
			sleeptime = sleeptime + 2000;
			Sleep(sleeptime);
		}
		if (m_userinfo.userid == "309719208370")
		{
			S_info = tradespi->JL_GetPrice_0(stock_id.c_str());
		}
		if (m_userinfo.userid == "309219037550")
		{
			S_info = tradespi->JL_GetPrice_1(stock_id.c_str());
		}
		if (m_userinfo.userid == "309219088510")
		{
			S_info = tradespi->JL_GetPrice_2(stock_id.c_str());
		}
		if (m_userinfo.userid == "309219171085")
		{
			S_info = tradespi->JL_GetPrice_3(stock_id.c_str());
		}
		error_num++;
		if (error_num >= 20)
		{
			cout << stock_id << ":没有价格信息" << endl;
			return false;
		}
		v_info = Toolkit::T_split(S_info, "|");
	} while (v_info.size() < 37);
	
	vector<double> v_price;

	//清空价格向量元素
	v_price.clear();
	v_price.push_back(atof(v_info[20].c_str()));
	v_price.push_back(atof(v_info[25].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[19].c_str()));
	v_price.push_back(atof(v_info[24].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[18].c_str()));
	v_price.push_back(atof(v_info[23].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[17].c_str()));
	v_price.push_back(atof(v_info[22].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[16].c_str()));
	v_price.push_back(atof(v_info[21].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[6].c_str()));
	v_price.push_back(atof(v_info[11].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[7].c_str()));
	v_price.push_back(atof(v_info[12].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[8].c_str()));
	v_price.push_back(atof(v_info[13].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[9].c_str()));
	v_price.push_back(atof(v_info[14].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[10].c_str()));
	v_price.push_back(atof(v_info[15].c_str()));
	stockprice.v_prices.push_back(v_price);
	stockprice.stockid = stock_id;
	stockprice.LastClose = atof(v_info[2].c_str());
	stockprice.TodayOpen = atof(v_info[3].c_str());
	stockprice.NewPrice = atof(v_info[5].c_str());
	return true;
}

bool TradeApi::cancel_order(string Entrustid)
{
	EnstrustInfo etinfo;
	this->QueryEntrustByid(Entrustid , etinfo);
	int jys = Toolkit::T_stockMarket(etinfo.stockid);
	if (jys != -1)
	{
		string S_info;
		if (m_userinfo.userid == "309719208370")
		{
			S_info = tradespi->JL_CancelOrder_0(m_userinfo.userid.c_str(), Entrustid.c_str(), jys);
		}
		if (m_userinfo.userid == "309219037550")
		{
			S_info = tradespi->JL_CancelOrder_1(m_userinfo.userid.c_str(), Entrustid.c_str(), jys);
		}
		if (m_userinfo.userid == "309219088510")
		{
			S_info = tradespi->JL_CancelOrder_2(m_userinfo.userid.c_str(), Entrustid.c_str(), jys);
		}
		if (m_userinfo.userid == "309219171085")
		{
			S_info = tradespi->JL_CancelOrder_3(m_userinfo.userid.c_str(), Entrustid.c_str(), jys);
		}
		if (S_info == "ok")
		{
			//撤单成功
			return true;
		}
	}
	return false;
}

bool TradeApi::QueryStockPosition(string stockid, int& stock_num)
{
	map<string, int> stockposition;
	if (this->QueryPosition(stockposition))
	{
		map<string, int>::iterator it = stockposition.find(stockid);
		if (it != stockposition.end())
		{
			stock_num = it->second;
		}
		else
		{
			stock_num = 0;
		}
		return true;
	}
	return false;
}

string s_time;
string Enstrustid;
string stockid;
int op_direction;	//0买入，1卖出
int EntrustNum;
double EntrustPrice;
int DealNum;
double DealPrice;
double DealValue;
int CancelNum;
string CancelSymbol;

bool TradeApi::QueryEntrustByid(string Entrustid,EnstrustInfo& entrust)
{
	vector<EnstrustInfo> v_entrust;
	entrust = { "", "", "", -1, 0, 0, 0, 0, 0, 0, "" };
	if (QueryEntrust(v_entrust))
	{
		if (v_entrust.size() > 0)
		{
			vector<EnstrustInfo>::iterator iter;
			for (iter = v_entrust.begin(); iter != v_entrust.end(); iter++)
			{
				if (iter->Enstrustid == Entrustid)
				{
					entrust = { iter->s_time, iter->Enstrustid, iter->stockid, iter->op_direction, iter->EntrustNum, iter->EntrustPrice,
						iter->DealNum, iter->DealPrice, iter->DealValue, iter->CancelNum, iter->CancelSymbol };
				}
			}
		}
		return true;
	}
	return false;
}

int TradeApi::QueryIsExgByid(string Entrustid)
{
	EnstrustInfo enf;
	this->QueryEntrustByid(Entrustid, enf);
	return enf.EntrustNum - enf.DealNum;
}

bool TradeApi::get_price_tx(string stock_id, StockPrice& stockprice)
{
	//cout << "获取" << stock_id << "价格数据" << endl;
	vector<vector<double>> v_prices0;
	stockprice = { "", 0, 0, 0, v_prices0 };
	string S_info = "";
	int error_num = 0, sleeptime = 0;
	vector<string> v_info;

	string stock_market = "";
	int market_code = Toolkit::T_stockMarket(stock_id);
	if (market_code == 1)
	{
		stock_market = "sh";
	}
	else
		if (market_code == 0)
			stock_market = "sz";
		else
		{
			cout << "无法判断股票市场归属" << endl;
			return false;
		}

	DataApi dataApi;
	string net_address = "http://qt.gtimg.cn/q=" + stock_market + stock_id;
	S_info = dataApi.readData_tx(net_address);
	v_info = Toolkit::T_split(S_info, "~");

	vector<double> v_price;

	//清空价格向量元素
	v_price.clear();
	//价格排列顺序：卖五——卖一，买一——买五
	v_price.push_back(atof(v_info[27].c_str()));
	v_price.push_back(atof(v_info[28].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[25].c_str()));
	v_price.push_back(atof(v_info[26].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[23].c_str()));
	v_price.push_back(atof(v_info[24].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[21].c_str()));
	v_price.push_back(atof(v_info[22].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[19].c_str()));
	v_price.push_back(atof(v_info[20].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[9].c_str()));
	v_price.push_back(atof(v_info[10].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[11].c_str()));
	v_price.push_back(atof(v_info[12].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[13].c_str()));
	v_price.push_back(atof(v_info[14].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[15].c_str()));
	v_price.push_back(atof(v_info[16].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[17].c_str()));
	v_price.push_back(atof(v_info[18].c_str()));
	stockprice.v_prices.push_back(v_price);
	stockprice.stockid = stock_id;
	stockprice.LastClose = atof(v_info[4].c_str());
	stockprice.TodayOpen = atof(v_info[5].c_str());
	stockprice.NewPrice = atof(v_info[3].c_str());
	return true;
}

bool TradeApi::get_price_sina(string stock_id, StockPrice& stockprice)
{
	//cout << "获取" << stock_id << "价格数据" << endl;
	vector<vector<double>> v_prices0;
	stockprice = { "", 0, 0, 0, v_prices0 };
	string S_info = "";
	int error_num = 0, sleeptime = 0;
	vector<string> v_info;

	string stock_market = "";
	int market_code = Toolkit::T_stockMarket(stock_id);
	if (market_code == 1)
	{
		stock_market = "sh";
	}
	else
		if (market_code == 0)
			stock_market = "sz";
		else
		{
			cout << "无法判断股票市场归属" << endl;
			return false;
		}

	DataApi dataApi;
	string net_address = "http://hq.sinajs.cn/list=" + stock_market + stock_id;
	S_info = dataApi.readData_sina(net_address);
	v_info = Toolkit::T_split(S_info, ",");

	vector<double> v_price;

	//清空价格向量元素
	v_price.clear();
	//价格排列顺序：卖五——卖一，买一——买五
	v_price.push_back(atof(v_info[29].c_str()));
	v_price.push_back(atof(v_info[28].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[27].c_str()));
	v_price.push_back(atof(v_info[26].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[25].c_str()));
	v_price.push_back(atof(v_info[24].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[23].c_str()));
	v_price.push_back(atof(v_info[22].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[21].c_str()));
	v_price.push_back(atof(v_info[20].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[11].c_str()));
	v_price.push_back(atof(v_info[10].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[13].c_str()));
	v_price.push_back(atof(v_info[12].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[15].c_str()));
	v_price.push_back(atof(v_info[14].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[17].c_str()));
	v_price.push_back(atof(v_info[16].c_str()));
	stockprice.v_prices.push_back(v_price);
	v_price.clear();
	v_price.push_back(atof(v_info[19].c_str()));
	v_price.push_back(atof(v_info[18].c_str()));
	stockprice.v_prices.push_back(v_price);
	stockprice.stockid = stock_id;
	stockprice.LastClose = atof(v_info[2].c_str());
	stockprice.TodayOpen = atof(v_info[1].c_str());
	stockprice.NewPrice = atof(v_info[3].c_str());
	return true;
}



