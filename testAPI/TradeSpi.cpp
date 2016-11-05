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
}

