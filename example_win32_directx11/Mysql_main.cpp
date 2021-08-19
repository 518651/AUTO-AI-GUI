#include "API.h"

MYSQL* mysql = new MYSQL; //mysql连接  
MYSQL_FIELD* fd;    //字段列数组  
char field[32][32];    //存字段名二维数组  
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集  
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列  
char query[150]; //查询语句

int Connectmysql()
{
    mysql_init(mysql);
    if (!(mysql_real_connect(mysql, "36035183wz.zicp.vip", "auto_ai_admin", "auto_ai_by_518651", "auto_ai_text", 29691, NULL, 0)))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}




