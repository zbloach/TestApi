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
	bool ComputeBuyStockNum(double retainedf, double position, int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list,double min_exgMoney);
	bool ComputeSellStockNum(vector<string> v_sell_list,map<string,int>& m_sell_list);
	//计算各个持仓根据最新资金情况应有的数量
	//deriction 1 适宜的数量大于当前数量，-1 适宜的数量小于当前数量,0 不需要任何变动
	bool ComputeFitStockNum(double retainedf, double position, int positionNum, map<string, int>& fit_map, int& deriction,double min_exgMoney);
	//计算应该调整的股票数量
	bool ComputeExgToFitStockNum(int deriction, map<string, int> fit_map, map<string, int>& toFit_map, double min_exgMoney);
	//合并买入或者卖出列表
	bool MergeExgList(int deriction, map<string, int>& map_buy_list, map<string, int>& map_sell_list, map<string, int> toFit_map);


	//perMin隔perNMin分钟交易一次,ExgNum交易次数
	bool ExgPerMin(int perSeconds, int ExgValue, map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min, int max_ser, vector<time_t> exgtime_list);
	
	int ComputePerSeconds(map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min, int& max_ser, double part_time);

	vector<time_t> ComputeExgtime_list(int perSeconds,int add_min,int max_ser);

	map<string, vector<int>> ComputeBuyPerMin(map<string, int> m_buy_list, double ExgValue);
	map<string, vector<int>> ComputeSellPerMin(map<string, int> m_sell_list, double ExgValue);

	void test();

	//申购新股
	bool BuyNewStock();

	double return_value();
	//普通下单，换仓操作
	//reduce  true：如果总资金减少，则降低当前持有的股票数量
	int init_exg(string ip, int port, vector<string> id_list, vector<string> key_list,vector<string> exgfile_list, vector<double> Retained_funds_list, 
		double position, int positionNum, int add_min, double min_exgMoney, double part_time, double ExgValue, bool reduce, bool buyNewStock);
	//调整仓位，减仓或
private:
	//vector<string> m_sell_list;
	//vector<string> m_buy_list;
	//map<string, int> m_buy_map;
	//map<string, int> m_sell_map;
	TradeApi tdApi;
	struct ServerInfo svinfo;
};