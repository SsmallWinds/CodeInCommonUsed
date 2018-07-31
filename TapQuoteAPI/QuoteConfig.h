#ifndef QUOTE_CONFIG_H
#define QUOTE_CONFIG_H

#include "tinyxml2.h"
#include "TapQuoteAPI.h" 
#include <string>
#include <vector>
#include <iostream>

using namespace tinyxml2;
using std::string;
using std::vector;
using std::cout;
using std::endl;


//行情IP地址与端口 联通
#define DEFAULT_IP		("61.163.243.173")
#define DEFAULT_PORT	(7171)


extern bool isdisconnect;

//#define CONFIG_PATH "C://Users//Administrator//Desktop//外盘9.0TapQuoteAPI//TapQuotAPIDemo//demo//TapAPI_Demo//windows//TapQuoteAPI_Demo//TapQuoteConfig.xml"
#define CONFIG_PATH "TapQuoteConfig.xml"

//用户名密码
#define DEFAULT_USERNAME	("ES")
#define DEFAULT_PASSWORD	("123456")

struct  IntrumentStruct
{
	string echangeNo;
	char commodityType;
	string commodityNo;
	string contractno1;
};

class QuoteConfig {
public:
	static QuoteConfig * GetInstance();
	

	QuoteConfig() {

	};

	~QuoteConfig() {

	};

	void Init() {

		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(CONFIG_PATH) != XMLError::XML_SUCCESS)
		{
			cout << "read config file [" << CONFIG_PATH << "] error!" << endl;
			return;
		}
		else
		{
			cout << "read config file [" << CONFIG_PATH << "] success!" << endl;
		}

		XMLElement* root = doc.RootElement();

		XMLElement* tap_ip = root->FirstChildElement("tap_ip");
		if (tap_ip != nullptr)
		{
			m_tapIp = tap_ip->GetText();
		}

		XMLElement* tap_port = root->FirstChildElement("tap_port");
		if (tap_port != nullptr)
		{
			m_tapPort = tap_port->IntText();
		}

		XMLElement* auth_code = root->FirstChildElement("auth_code");
		if (auth_code != nullptr)
		{
			m_authCode = auth_code->GetText();
		}

		XMLElement* user_id = root->FirstChildElement("user_id");
		if (user_id != nullptr)
		{
			m_userId = user_id->GetText();
		}

		XMLElement* psw = root->FirstChildElement("psw");
		if (psw != nullptr)
		{
			m_psw = psw->GetText();
		}


		XMLElement* port = root->FirstChildElement("port");
		if (port != nullptr)
		{
			m_port = port->IntText();
		}


		XMLElement* key = root->FirstChildElement("key");
		if (key != nullptr)
		{
			m_key = key->GetText();
		}

		XMLElement* instruments = root->FirstChildElement("instruments");
		if (instruments != nullptr)
		{
			auto code = instruments->FirstChildElement();
			while (code != nullptr)
			{
				IntrumentStruct ins;

				ins.echangeNo = code->Attribute("exchangeNo");
				ins.commodityType = code->Attribute("commodityType")[0];
				ins.commodityNo = code->Attribute("commodityno");
				ins.contractno1 = code->Attribute("contractno1");

				m_codes.push_back(ins);
				code = code->NextSiblingElement();
			}
		}
		cout << "InstrumentCount:" << m_codes.size() << endl;
	};

public:
	string m_tapIp;
	int m_tapPort;
	string m_authCode;
	string m_userId;
	string m_psw;
	int m_port;
	string m_key;
	vector<IntrumentStruct> m_codes;
};


#endif // QUOTE_CONFIG_H
