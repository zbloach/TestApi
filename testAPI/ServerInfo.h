#pragma once
#include <string>
#include <vector>
using namespace std;
struct ServerInfo
{
	//服务器信息
	string ip;
	int trade_server_port;
	string commu_key;
	string depart_code;
};

struct AccountInfo
{
	//账户信息
	double totalStock;
	double totalMoney;
	double totalValue;
};

struct UserInfo 
{
	string userid;
	string password;
	string shid;
	string szid;
};

struct EnstrustInfo
{
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
};

struct StockPrice
{
	string stockid;
	double LastClose;
	double TodayOpen;
	double NewPrice;	//当前成交价
	vector<vector<double>> v_prices;
};

enum queryid
{
	accountInfo_id = 104,
	todaySend_id = 1102,
	ShareholderCode_id = 1122,
	Position_id = 1114,
	Ensturst_id = 1102
};

enum errorCode
{
	NotEnophMoney = 413
};