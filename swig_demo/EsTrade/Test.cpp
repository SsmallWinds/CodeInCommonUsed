#include "Test.h"

#include <string>

Test::Test()
{
	buf[0] = 't';
	buf[1] = 'e';
	buf[2] = 's';
	buf[3] = 't';
	buf[4] = '\0';
}


Test::~Test()
{
}

void Test::Run()
{
	for (int i = 0; i < 5; i++)
		_callBack->Call(&buf);
}

void Test::Release()
{
	delete this;
}

void Test::SetCallBack(ITestCallBack* callBack)
{
	_callBack = callBack;
}

void ITestCallBack2::Call(char(*buf)[201])
{
	Call2(*buf);
}
