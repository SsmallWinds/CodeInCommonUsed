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
	//取得API的版本信息
	cout << GetTapQuoteAPIVersion() << endl;

	auto config = QuoteConfig::GetInstance();
	config->Init();

	SocketServer server(QuoteConfig::GetInstance()->m_port);


	//创建API实例
	TAPIINT32 iResult = TAPIERROR_SUCCEED;
	TapAPIApplicationInfo stAppInfo;
	strcpy(stAppInfo.AuthCode, QuoteConfig::GetInstance()->m_authCode.c_str());
	strcpy(stAppInfo.KeyOperationLogPath, "");
	ITapQuoteAPI *pAPI = CreateTapQuoteAPI(&stAppInfo, iResult);
	if (NULL == pAPI){
		cout << "创建API实例失败，错误码：" << iResult <<endl;
		return -1;
	}

	//设定ITapQuoteAPINotify的实现类，用于异步消息的接收
	Quote objQuote;
	objQuote.SetSocketServer(&server);
	pAPI->SetAPINotify(&objQuote);
	

	//启动测试，订阅行情
	objQuote.SetAPI(pAPI);

	objQuote.RunTest();
	return 0;
}