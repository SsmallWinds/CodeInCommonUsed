#include "Quote.h"
#include "TapAPIError.h"
#include "QuoteConfig.h"
//#include <Windows.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "QuoteConfig.h"

using namespace std;

Quote::Quote(void) :
	m_pAPI(NULL),
	m_bIsAPIReady(false),
	m_pServer(NULL)
{
}


Quote::~Quote(void)
{
}


void Quote::SetSocketServer(SocketServer * pServer)
{
	m_pServer = pServer;
}

void Quote::SetAPI(ITapQuoteAPI *pAPI)
{
	m_pAPI = pAPI;
}


void Quote::RunTest()
{
	if (NULL == m_pAPI) {
		cout << "Error: m_pAPI is NULL." << endl;
		return;
	}

	TAPIINT32 iErr = TAPIERROR_SUCCEED;


	//设定服务器IP、端口
	iErr = m_pAPI->SetHostAddress(QuoteConfig::GetInstance()->m_tapIp.c_str(), QuoteConfig::GetInstance()->m_tapPort);
	if (TAPIERROR_SUCCEED != iErr) {
		cout << "SetHostAddress Error:" << iErr << endl;
		return;
	}

	//登录服务器
	TapAPIQuoteLoginAuth stLoginAuth;
	memset(&stLoginAuth, 0, sizeof(stLoginAuth));
	strcpy(stLoginAuth.UserNo, QuoteConfig::GetInstance()->m_userId.c_str());
	strcpy(stLoginAuth.Password, QuoteConfig::GetInstance()->m_psw.c_str());
	stLoginAuth.ISModifyPassword = APIYNFLAG_NO;
	stLoginAuth.ISDDA = APIYNFLAG_NO;
	iErr = m_pAPI->Login(&stLoginAuth);
	if (TAPIERROR_SUCCEED != iErr) {
		cout << "Login Error:" << iErr << endl;
		return;
	}


	//等待APIReady
	m_Event.WaitEvent();
	if (!m_bIsAPIReady) {
		return;
	}

	/*m_uiSessionID = 0;
	m_pAPI->QryCommodity(&m_uiSessionID);
*/

	auto& codes = QuoteConfig::GetInstance()->m_codes;

	for (auto& code : codes) {
		TapAPIContract stContract;
		memset(&stContract, 0, sizeof(stContract));
		strcpy(stContract.Commodity.ExchangeNo, code.echangeNo.c_str());
		stContract.Commodity.CommodityType = code.commodityType;
		strcpy(stContract.Commodity.CommodityNo, code.commodityNo.c_str());
		strcpy(stContract.ContractNo1, code.contractno1.c_str());
		stContract.CallOrPutFlag1 = TAPI_CALLPUT_FLAG_NONE;
		stContract.CallOrPutFlag2 = TAPI_CALLPUT_FLAG_NONE;
		m_uiSessionID = 0;
		auto iErr = m_pAPI->SubscribeQuote(&m_uiSessionID, &stContract);
		if (TAPIERROR_SUCCEED != iErr) {
			cout << "SubscribeQuote Error:" << iErr << " [" << code.commodityNo << code.contractno1 << "] " << endl;
			break;
		}
	}

	/*
	m_uiSessionID = 0;
	TapAPICommodity com;
	memset(&com, 0, sizeof(com));
	strcpy(com.ExchangeNo,DEFAULT_EXCHANGE_NO);
	strcpy(com.CommodityNo,DEFAULT_COMMODITY_NO);
	com.CommodityType =DEFAULT_COMMODITY_TYPE;
	m_pAPI->QryContract(&m_uiSessionID,&com);



	//订阅行情
	TapAPIContract stContract;
	memset(&stContract, 0, sizeof(stContract));
	strcpy(stContract.Commodity.ExchangeNo, DEFAULT_EXCHANGE_NO);
	stContract.Commodity.CommodityType = DEFAULT_COMMODITY_TYPE;
	strcpy(stContract.Commodity.CommodityNo, DEFAULT_COMMODITY_NO);
	strcpy(stContract.ContractNo1, DEFAULT_CONTRACT_NO);
	stContract.CallOrPutFlag1 = TAPI_CALLPUT_FLAG_NONE;
	stContract.CallOrPutFlag2 = TAPI_CALLPUT_FLAG_NONE;
	m_uiSessionID = 0;
	iErr = m_pAPI->SubscribeQuote(&m_uiSessionID, &stContract);
	if(TAPIERROR_SUCCEED != iErr) {
		cout << "SubscribeQuote Error:" << iErr <<endl;
		return;
	}
	*/

	while (true) {
		m_Event.WaitEvent();
	}
}



void TAP_CDECL Quote::OnRspLogin(TAPIINT32 errorCode, const TapAPIQuotLoginRspInfo *info)
{
	if (TAPIERROR_SUCCEED == errorCode) {
		cout << "登录成功，等待API初始化..." << endl;
		m_bIsAPIReady = true;

	}
	else {
		cout << "登录失败，错误码:" << errorCode << endl;
		m_Event.SignalEvent();
	}
}

void TAP_CDECL Quote::OnAPIReady()
{
	cout << "API初始化完成" << endl;
	m_Event.SignalEvent();
}

void TAP_CDECL Quote::OnDisconnect(TAPIINT32 reasonCode)
{
	cout << "API断开,断开原因:" << reasonCode << endl;
}

void TAP_CDECL Quote::OnRspQryCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteCommodityInfo *info)
{
	//cout << __FUNCTION__ << isLast << endl;

	if (info) {
		cout << __FUNCTION__ << " [Commodity] " << info->Commodity.CommodityNo << " [CommodityType]" << info->Commodity.CommodityType << " [Exchangeno]" << info->Commodity.ExchangeNo << endl;

		if (info->Commodity.CommodityType != TAPI_COMMODITY_TYPE_FUTURES)
			return;

		m_uiSessionID = 0;
		TapAPICommodity com;
		memset(&com, 0, sizeof(com));
		strcpy(com.ExchangeNo, info->Commodity.ExchangeNo);
		strcpy(com.CommodityNo, info->Commodity.CommodityNo);
		com.CommodityType = info->Commodity.CommodityType;
		m_pAPI->QryContract(&m_uiSessionID, &com);
	}
	else
	{
		cout << __FUNCTION__ << " info is nullptr" << endl;
	}
}

void TAP_CDECL Quote::OnRspQryContract(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteContractInfo *info)
{
	cout << __FUNCTION__ << errorCode << endl;
	if (info)
	{
		cout << "合约:[Code]" << info->Contract.Commodity.CommodityNo << info->Contract.ContractNo1 << " [ContractNo2]" << info->Contract.ContractNo2 << endl;

		TapAPIContract stContract;
		memset(&stContract, 0, sizeof(stContract));
		strcpy(stContract.Commodity.ExchangeNo, info->Contract.Commodity.ExchangeNo);
		stContract.Commodity.CommodityType = info->Contract.Commodity.CommodityType;
		strcpy(stContract.Commodity.CommodityNo, info->Contract.Commodity.CommodityNo);
		strcpy(stContract.ContractNo1, info->Contract.ContractNo1);
		stContract.CallOrPutFlag1 = TAPI_CALLPUT_FLAG_NONE;
		stContract.CallOrPutFlag2 = TAPI_CALLPUT_FLAG_NONE;
		m_uiSessionID = 0;
		auto iErr = m_pAPI->SubscribeQuote(&m_uiSessionID, &stContract);
		if (TAPIERROR_SUCCEED != iErr) {
			cout << "SubscribeQuote Error:" << iErr << endl;
			return;
		}
	}
	else
	{
		cout << __FUNCTION__ << " info is nullptr" << endl;
	}
}


void TAP_CDECL Quote::OnRspSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteWhole *info)
{
	if (TAPIERROR_SUCCEED == errorCode)
	{
		cout << "行情订阅成功 ";
		if (NULL != info)
		{
			cout << info->DateTimeStamp << " "
				<< info->Contract.Commodity.ExchangeNo << " "
				<< info->Contract.Commodity.CommodityType << " "
				<< info->Contract.Commodity.CommodityNo << " "
				<< info->Contract.ContractNo1 << " "
				<< info->QLastPrice
				// ...		
				<< endl;
		}

	}
	else {
		cout << "行情订阅失败，错误码：" << errorCode << endl;
	}
}

void TAP_CDECL Quote::OnRspUnSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIContract *info)
{
	cout << __FUNCTION__ << " is called." << endl;
}

void TAP_CDECL Quote::OnRtnQuote(const TapAPIQuoteWhole *info)
{
	if (NULL != info)
	{
		stringstream ss;
		ss << info->DateTimeStamp << ','
			<< info->Contract.Commodity.ExchangeNo << ','
			<< info->Contract.Commodity.CommodityType << ','
			<< info->Contract.Commodity.CommodityNo << info->Contract.ContractNo1 << ','
			<< info->QPreClosingPrice << ','
			<< info->QPreSettlePrice << ','
			<< info->QPrePositionQty << ','
			<< info->QOpeningPrice << ','
			<< info->QLastPrice << ','
			<< info->QHighPrice << ','
			<< info->QHisHighPrice << ','
			<< info->QHisLowPrice << ','
			<< info->QLimitUpPrice << ','
			<< info->QLimitDownPrice << ','
			<< info->QTotalQty << ','
			<< info->QPositionQty << ','
			<< info->QAveragePrice << ','
			<< info->QClosingPrice << ','
			<< info->QSettlePrice << ','
			<< info->QLastQty << ','

			<< info->QBidPrice[0] << ','
			<< info->QBidPrice[1] << ','
			<< info->QBidPrice[2] << ','
			<< info->QBidPrice[3] << ','
			<< info->QBidPrice[4] << ','
			<< info->QBidPrice[5] << ','
			<< info->QBidPrice[6] << ','
			<< info->QBidPrice[7] << ','
			<< info->QBidPrice[8] << ','
			<< info->QBidPrice[9] << ','

			<< info->QBidQty[0] << ','
			<< info->QBidQty[1] << ','
			<< info->QBidQty[2] << ','
			<< info->QBidQty[3] << ','
			<< info->QBidQty[4] << ','
			<< info->QBidQty[5] << ','
			<< info->QBidQty[6] << ','
			<< info->QBidQty[7] << ','
			<< info->QBidQty[8] << ','
			<< info->QBidQty[9] << ','

			<< info->QAskPrice[0] << ','
			<< info->QAskPrice[1] << ','
			<< info->QAskPrice[2] << ','
			<< info->QAskPrice[3] << ','
			<< info->QAskPrice[4] << ','
			<< info->QAskPrice[5] << ','
			<< info->QAskPrice[6] << ','
			<< info->QAskPrice[7] << ','
			<< info->QAskPrice[8] << ','
			<< info->QAskPrice[9] << ','

			<< info->QAskQty[0] << ','
			<< info->QAskQty[1] << ','
			<< info->QAskQty[2] << ','
			<< info->QAskQty[3] << ','
			<< info->QAskQty[4] << ','
			<< info->QAskQty[5] << ','
			<< info->QAskQty[6] << ','
			<< info->QAskQty[7] << ','
			<< info->QAskQty[8] << ','
			<< info->QAskQty[9] << ','

			<< info->QInsideQty << ','
			<< info->QOutsideQty << ','
			<< info->QTurnoverRate << ','
			<< info->Q5DAvgQty << ','
			<< info->QPERatio << ','
			<< info->QTotalValue << ','
			<< info->QNegotiableValue << ','
			<< info->QPositionTrend << ','
			<< info->QChangeSpeed << ','
			<< info->QChangeRate << ','
			<< fixed << info->QChangeValue << ','
			<< info->QSwing << ','
			<< info->QTotalBidQty << ','
			<< info->QTotalAskQty << endl;

		ss.flush();
		m_pServer->Send(ss.str().c_str(), ss.str().size());
	}
}
