#include "TradeSpi.h"

TradeSpi::TradeSpi()
{
	HMODULE1 = LoadLibrary(TEXT("JLAPI.dll"));
	JL_Login_0 = (Login_0)GetProcAddress(HMODULE1, "JL_Login");
	JL_QueryData_0 = (QueryData_0)GetProcAddress(HMODULE1, "JL_QueryData");
	JL_GetPrice_0 = (GetPrice_0)(GetProcAddress(HMODULE1, "JL_GetPrice"));
	JL_SendOrder_0 = (SendOrder_0)(GetProcAddress(HMODULE1, "JL_SendOrder"));
	JL_CancelOrder_0 = (CancelOrder_0)(GetProcAddress(HMODULE1, "JL_CancelOrder"));

	HMODULE2 = LoadLibrary(TEXT("309219037550.dll"));
	JL_Login_1 = (Login_1)GetProcAddress(HMODULE2, "JL_Login");
	JL_QueryData_1 = (QueryData_1)GetProcAddress(HMODULE2, "JL_QueryData");
	JL_GetPrice_1 = (GetPrice_1)(GetProcAddress(HMODULE2, "JL_GetPrice"));
	JL_SendOrder_1 = (SendOrder_1)(GetProcAddress(HMODULE2, "JL_SendOrder"));
	JL_CancelOrder_1 = (CancelOrder_1)(GetProcAddress(HMODULE2, "JL_CancelOrder"));

	HMODULE3 = LoadLibrary(TEXT("JLAPI_jcp.dll"));
	JL_Login_2 = (Login_2)GetProcAddress(HMODULE3, "JL_Login");
	JL_QueryData_2 = (QueryData_2)GetProcAddress(HMODULE3, "JL_QueryData");
	JL_GetPrice_2 = (GetPrice_2)(GetProcAddress(HMODULE3, "JL_GetPrice"));
	JL_SendOrder_2 = (SendOrder_2)(GetProcAddress(HMODULE3, "JL_SendOrder"));
	JL_CancelOrder_2 = (CancelOrder_2)(GetProcAddress(HMODULE3, "JL_CancelOrder"));

	HMODULE4 = LoadLibrary(TEXT("JLAPI_zjj.dll"));
	JL_Login_3 = (Login_3)GetProcAddress(HMODULE4, "JL_Login");
	JL_QueryData_3 = (QueryData_3)GetProcAddress(HMODULE4, "JL_QueryData");
	JL_GetPrice_3 = (GetPrice_3)(GetProcAddress(HMODULE4, "JL_GetPrice"));
	JL_SendOrder_3 = (SendOrder_3)(GetProcAddress(HMODULE4, "JL_SendOrder"));
	JL_CancelOrder_3 = (CancelOrder_3)(GetProcAddress(HMODULE4, "JL_CancelOrder"));
}

