#pragma once
class ITestCallBack {

public:
	virtual ~ITestCallBack() {};

	virtual void Call(char(*buf)[201]) = 0;

};

class ITestCallBack2 :public ITestCallBack
{
public:
	virtual ~ITestCallBack2() {};

	virtual void Call(char(*buf)[201])override;

	virtual void Call2(const char* buf) = 0;
};

class Test
{
public:
	Test();
	~Test();

	char buf[201];

	ITestCallBack * _callBack;

	void Run();

	void Release();

	void SetCallBack(ITestCallBack* callBack);
};


