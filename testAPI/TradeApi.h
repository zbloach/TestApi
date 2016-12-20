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

	void Init(const ServerInfo& info);//��ʼ����Ա
	//��¼
	bool Login(string m_userid, string password);
	//��ѯ�˻��ܸſ�
	bool QueryAccountMoney(struct AccountInfo& ac_info);
	//��ѯ�ɶ�������Ϣ
	bool QueryShareholderCode();
	//��ѯ��Ʊ�ֲ�
	bool QueryPosition(map<string,int>& stock_position);
	//��ѯĳֻ��Ʊ�ֲ�
	bool QueryStockPosition(string stockid,int& stock_num);
	//��ѯ����ί��
	bool QueryEntrust(vector<EnstrustInfo>& EntrustList);
	//��ѯδ�ɽ�ί��
	bool QueryNotDealEntrust(vector<EnstrustInfo>& EntrustList);
	//����ί�б�Ų�ί��״̬
	bool QueryEntrustByid(string Entrustid, EnstrustInfo& entrust);
	//����ί�б�Ų�Ѱί���Ƿ�ɽ�
	//����δ�ɽ�����
	int QueryIsExgByid(string Entrustid);
	//�����Ʊ
	bool buy_stock(string stock_id, int stock_num,double price,string& result);
	//������Ʊ
	bool sell_stock(string stock_id, int stock_num, double price,string& result);
	//����ί��
	bool cancel_order(string Entrustid);
	//��ȡ�̿��嵵�۸�
	bool get_price(string stock_id, StockPrice& stockprice);
	//��Ѷ��ȡ�̿��嵵�۸�
	bool get_price_tx(string stock_id, StockPrice& stockprice);
	//���ش�����Ϣ

	//const ErrorMessage& LastError() const;
	//�ж���Ϣ�Ƿ�Ϸ�
	//bool IsErrorRespInfo(CThostFtdcRspInfoField * pRspInfo);
	//����һ��API
	//CThostFtdcTraderApi* GetTradeApi();
	//��ѯ��¼״̬
	//int GetLoginState(){ return m_iLoginState; }
private:
	struct ServerInfo m_serverinfo;
	struct UserInfo m_userinfo;
	//�ֲ�
	//map<string, int> m_position;
	//���̼ۣ����ռۣ��嵵��
	//map<string, vector<vector<double>>> m_price;

	//string m_

	TradeSpi* tradespi;

};
