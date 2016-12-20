#pragma once
#ifndef _TRADESPI_
#define _TRADESPI_
#endif
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
	typedef bool(__stdcall* Login_0)(const char* IP, int Port,const char* User,const char* PassWord,const char* TXPass,const char* Yyb);
	typedef char*(__stdcall* QueryData_0)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice_0)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder_0)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder_0)(const char* User, const char* Entrustid, int jys);

	HMODULE HMODULE2;
	typedef bool(__stdcall* Login_1)(const char* IP, int Port, const char* User, const char* PassWord, const char* TXPass, const char* Yyb);
	typedef char*(__stdcall* QueryData_1)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice_1)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder_1)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder_1)(const char* User, const char* Entrustid, int jys);

	HMODULE HMODULE3;
	typedef bool(__stdcall* Login_2)(const char* IP, int Port, const char* User, const char* PassWord, const char* TXPass, const char* Yyb);
	typedef char*(__stdcall* QueryData_2)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice_2)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder_2)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder_2)(const char* User, const char* Entrustid, int jys);

	HMODULE HMODULE4;
	typedef bool(__stdcall* Login_3)(const char* IP, int Port, const char* User, const char* PassWord, const char* TXPass, const char* Yyb);
	typedef char*(__stdcall* QueryData_3)(const char* User, int Category);
	typedef char*(__stdcall* GetPrice_3)(const char* Gpdm);
	typedef char*(__stdcall* SendOrder_3)(int Fx, const char* User, const char* Gddm, const char*  Gpdm, int Quantity, float Price);
	typedef char*(__stdcall* CancelOrder_3)(const char* User, const char* Entrustid, int jys);
public:
	Login_0 JL_Login_0;
	SendOrder_0 JL_SendOrder_0;
	GetPrice_0 JL_GetPrice_0;
	QueryData_0 JL_QueryData_0;
	CancelOrder_0 JL_CancelOrder_0;

	Login_1 JL_Login_1;
	SendOrder_1 JL_SendOrder_1;
	GetPrice_1 JL_GetPrice_1;
	QueryData_1 JL_QueryData_1;
	CancelOrder_1 JL_CancelOrder_1;

	Login_2 JL_Login_2;
	SendOrder_2 JL_SendOrder_2;
	GetPrice_2 JL_GetPrice_2;
	QueryData_2 JL_QueryData_2;
	CancelOrder_2 JL_CancelOrder_2;

	Login_3 JL_Login_3;
	SendOrder_3 JL_SendOrder_3;
	GetPrice_3 JL_GetPrice_3;
	QueryData_3 JL_QueryData_3;
	CancelOrder_3 JL_CancelOrder_3;
};
