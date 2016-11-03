#pragma once
#include "stdio.h"
#include <iostream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace std;

class TradeSpi
{
public:
	TradeSpi();

	~TradeSpi();



private:
	HMODULE HMODULE1;
	typedef bool(__stdcall* Login)(const char* IP, int Port,const char* User,const char* PassWord,const char* TXPass,const char* Yyb);
	typedef char*(__stdcall* QueryData)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder)(const char* User, const char* Entrustid,int jys);

	HMODULE HMODULE2;
	typedef bool(__stdcall* Login_1)(const char* IP, int Port, const char* User, const char* PassWord, const char* TXPass, const char* Yyb);
	typedef char*(__stdcall* QueryData_1)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice_1)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder_1)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder_1)(const char* User, const char* Entrustid, int jys);
public:
	Login JL_Login;
	SendOrder JL_SendOrder;
	GetPrice JL_GetPrice;
	QueryData JL_QueryData;
	CancelOrder JL_CancelOrder;

	Login_1 JL_Login_1;
	SendOrder_1 JL_SendOrder_1;
	GetPrice_1 JL_GetPrice_1;
	QueryData_1 JL_QueryData_1;
	CancelOrder_1 JL_CancelOrder_1;
};
