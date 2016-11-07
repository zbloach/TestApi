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
	//��ȡ�µ��б�
	bool readExgList(string fileAdress,vector<string>& v_sell_list,vector<string>& v_buy_list);
	//���չ̶����Ӽ���µ�
	//��������������ױ�ϸ�ֵĴ���
	
	//����������Ʊ������
	//stockVolume�ֲֳַɵķ���
	//position ��λ  0.5  ��֣�1 ȫ��
	//Retained funds �����ʽ���
	bool ComputeBuyStockNum(double retainedf,double position,int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list);
	bool ComputeSellStockNum(vector<string> v_sell_list,map<string,int>& m_sell_list);
	//perMin��perNMin���ӽ���һ��,ExgNum���״���
	
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