#include "API.h"

MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_FIELD* fd;    //�ֶ�������  
char field[32][32];    //���ֶ�����ά����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���

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




