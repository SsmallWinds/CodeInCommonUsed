#include <iostream>
#include "databasemysql.h"
#include <string>

using namespace std;

int main()
{

  CDatabaseMysql conn;

  conn.Initialize("139.224.45.109", "root", "1234qwer", "ps_quant");

  string sql("INSERT INTO trading_date(trading_date) VALUES(NOW())");

  conn.Execute(sql.c_str());

  return 0;
}