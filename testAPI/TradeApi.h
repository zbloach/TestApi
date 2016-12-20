#pragma once
#include "stdio.h"
#include <iostream>
#include "ServerInfo.h"
#include "TradeSpi.h"
#include "Toolkit.h"
#include <map>
using namespace std;



class TradeApi
{
public:
	TradeApi();

	~TradeApi();
	//

	//virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//virtual void OnFrontConnected();

	void Init(const ServerInfo& info);//初始化成员
	//登录
	bool Login(string m_userid, string password);
	//查询账户总概况
	bool QueryAccountMoney(struct AccountInfo& ac_info);
	//查询股东代码信息
	bool QueryShareholderCode();
	//查询股票持仓
	bool QueryPosition(map<string,int>& stock_position);
	//查询某只股票持仓
	bool QueryStockPosition(string stockid,int& stock_num);
	//查询当日委托
	bool QueryEntrust(vector<EnstrustInfo>& EntrustList);
	//查询未成交委托
	bool QueryNotDealEntrust(vector<EnstrustInfo>& EntrustList);
	//根据委托编号查委托状态
	bool QueryEntrustByid(string Entrustid, EnstrustInfo& entrust);
	//根据委托编号查寻委托是否成交
	//返回未成交数量
	int QueryIsExgByid(string Entrustid);
	//买入股票
	bool buy_stock(string stock_id, int stock_num,double price,string& result);
	//卖出股票
	bool sell_stock(string stock_id, int stock_num, double price,string& result);
	//撤销委托
	bool cancel_order(string Entrustid);
	//获取盘口五档价格
	bool get_price(string stock_id, StockPrice& stockprice);
	//腾讯获取盘口五档价格
	bool get_price_tx(string stock_id, StockPrice& stockprice);
	//返回错误信息

	//const ErrorMessage& LastError() const;
	//判断信息是否合法
	//bool IsErrorRespInfo(CThostFtdcRspInfoField * pRspInfo);
	//返回一个API
	//CThostFtdcTraderApi* GetTradeApi();
	//查询登录状态
	//int GetLoginState(){ return m_iLoginState; }
private:
	struct ServerInfo m_serverinfo;
	struct UserInfo m_userinfo;
	//持仓
	//map<string, int> m_position;
	//开盘价，昨收价，五档价
	//map<string, vector<vector<double>>> m_price;

	//string m_

	TradeSpi* tradespi;

};
