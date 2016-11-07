#pragma once
#include "stdio.h"
#include <iostream>
#include "TradeApi.h"
#include <time.h>
#include <fstream>
#include <map>


class testAPI
{
public:
	testAPI();
	bool Init(string ip, int tradeport, string txmm, string yeb, string userid, string password);
	~testAPI();
	//读取下单列表
	bool readExgList(string fileAdress,vector<string>& v_sell_list,vector<string>& v_buy_list);
	//按照固定分钟间隔下单
	//间隔分钟数，交易被细分的次数
	
	//计算买卖股票的数量
	//stockVolume持仓分成的分数
	//position 仓位  0.5  半仓，1 全仓
	//Retained funds 留存资金量
	bool ComputeBuyStockNum(double retainedf,double position,int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list);
	bool ComputeSellStockNum(vector<string> v_sell_list,map<string,int>& m_sell_list);
	//perMin隔perNMin分钟交易一次,ExgNum交易次数
	
	bool ExgPerMin(int perSeconds, int ExgValue, map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num);
	
	map<string, vector<int>> ComputeBuyPerMin(map<string, int> m_buy_list, int ExgValue);
	map<string, vector<int>> ComputeSellPerMin(map<string, int> m_sell_list, int ExgValue);
	void test();

private:
	//vector<string> m_sell_list;
	//vector<string> m_buy_list;
	//map<string, int> m_buy_map;
	//map<string, int> m_sell_map;
	TradeApi tdApi;
	struct ServerInfo svinfo;
};