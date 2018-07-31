#include "TapQuoteAPI.h"
#include "TapAPIError.h"
#include "Quote.h"
#include "QuoteConfig.h"
#include "SocketServer.h"


#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char* argv[])
{
	//ȡ��API�İ汾��Ϣ
	cout << GetTapQuoteAPIVersion() << endl;

	auto config = QuoteConfig::GetInstance();
	config->Init();

	SocketServer server(QuoteConfig::GetInstance()->m_port);


	//����APIʵ��
	TAPIINT32 iResult = TAPIERROR_SUCCEED;
	TapAPIApplicationInfo stAppInfo;
	strcpy(stAppInfo.AuthCode, QuoteConfig::GetInstance()->m_authCode.c_str());
	strcpy(stAppInfo.KeyOperationLogPath, "");
	ITapQuoteAPI *pAPI = CreateTapQuoteAPI(&stAppInfo, iResult);
	if (NULL == pAPI){
		cout << "����APIʵ��ʧ�ܣ������룺" << iResult <<endl;
		return -1;
	}

	//�趨ITapQuoteAPINotify��ʵ���࣬�����첽��Ϣ�Ľ���
	Quote objQuote;
	objQuote.SetSocketServer(&server);
	pAPI->SetAPINotify(&objQuote);
	

	//�������ԣ���������
	objQuote.SetAPI(pAPI);

	objQuote.RunTest();
	return 0;
}