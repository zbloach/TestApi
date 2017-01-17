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
	bool ComputeBuyStockNum(double retainedf, double position, int positionNum, vector<string> v_buy_list, map<string, int>& m_buy_list,double min_exgMoney);
	bool ComputeSellStockNum(vector<string> v_sell_list,map<string,int>& m_sell_list);
	//��������ֲָ��������ʽ����Ӧ�е�����
	//deriction 1 ���˵��������ڵ�ǰ������-1 ���˵�����С�ڵ�ǰ����,0 ����Ҫ�κα䶯
	bool ComputeFitStockNum(double retainedf, double position, int positionNum, map<string, int>& fit_map, int& deriction,double min_exgMoney);
	//����Ӧ�õ����Ĺ�Ʊ����
	bool ComputeExgToFitStockNum(int deriction, map<string, int> fit_map, map<string, int>& toFit_map, double min_exgMoney);
	//�ϲ�������������б�
	bool MergeExgList(int deriction, map<string, int>& map_buy_list, map<string, int>& map_sell_list, map<string, int> toFit_map);


	//perMin��perNMin���ӽ���һ��,ExgNum���״���
	bool ExgPerMin(int perSeconds, int ExgValue, map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min, int max_ser, vector<time_t> exgtime_list);
	
	int ComputePerSeconds(map<string, vector<int>> buy_list_num, map<string, vector<int>> sell_list_num, int add_min, int& max_ser, double part_time);

	vector<time_t> ComputeExgtime_list(int perSeconds,int add_min,int max_ser);

	map<string, vector<int>> ComputeBuyPerMin(map<string, int> m_buy_list, double ExgValue);
	map<string, vector<int>> ComputeSellPerMin(map<string, int> m_sell_list, double ExgValue);

	void test();

	//�깺�¹�
	bool BuyNewStock();

	double return_value();
	//��ͨ�µ������ֲ���
	//reduce  true��������ʽ���٣��򽵵͵�ǰ���еĹ�Ʊ����
	int init_exg(string ip, int port, vector<string> id_list, vector<string> key_list,vector<string> exgfile_list, vector<double> Retained_funds_list, 
		double position, int positionNum, int add_min, double min_exgMoney, double part_time, double ExgValue, bool reduce, bool buyNewStock);
	//������λ�����ֻ�
private:
	//vector<string> m_sell_list;
	//vector<string> m_buy_list;
	//map<string, int> m_buy_map;
	//map<string, int> m_sell_map;
	TradeApi tdApi;
	struct ServerInfo svinfo;
};