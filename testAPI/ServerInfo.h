#pragma once
#include <string>
#include <vector>
using namespace std;
struct ServerInfo
{
	//��������Ϣ
	string ip;
	int trade_server_port;
	string commu_key;
	string depart_code;
};

struct AccountInfo
{
	//�˻���Ϣ
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
	int op_direction;	//0���룬1����
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
	double NewPrice;	//��ǰ�ɽ���
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