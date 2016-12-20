#include "DataApi.h"
#include <afxinet.h>

string DataApi::readData_tx(string netAddress)
{
	CString temp_0 = "HttpClient";
	CInternetSession session(temp_0);
	CString cstr_url(netAddress.c_str());
	CHttpFile* pfile = (CHttpFile *)session.OpenURL(cstr_url);
	
	string result = "";

	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	if (dwStatusCode == HTTP_STATUS_OK)
	{
		CString content;
		CString data;
		while (pfile->ReadString(data))
		{
			//content += data + "\r\n";
			content += data;
		}
		content.TrimRight();

		LPCTSTR lpTexts = (LPCTSTR)content;

		//printf(" %s\n ", lpTexts);
		result = (LPSTR)lpTexts;
		//cout << result << endl;
		//printf(" %s\n ", (LPCTSTR)content);
		//cout << content << endl;
		//result = string((W2A)content));
	}
	pfile->Close();
	delete pfile;
	session.Close();

	return result;
}
