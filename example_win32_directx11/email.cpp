#include "Smtp.h"
#include "Smtp.h"
#include <iostream>
using namespace std;

void email(char email_target,char message)
{
    string email_Get, email_message;
    email_Get = email_target;
    email_message = message;

	CSmtp smtp(
		25,								/*smtp�˿�*/
		"smtp.163.com",					/*smtp��������ַ*/
		"zhu1725212799@163.com",	/*��������ַ*/
		"XXXXXXXXXX",					/*��������*/
        email_Get,	/*Ŀ�������ַ*/
		"�����û��ķ���",							/*����*/
        email_message		/*�ʼ�����*/
	);
	/**
	//��Ӹ���ʱע��,\һ��Ҫд��\\����Ϊת���ַ���Ե��
	string filePath("D:\\�γ���Ʊ���.doc");
	smtp.AddAttachment(filePath);
	*/

	/*�����Ե���CSmtp::DeleteAttachment����ɾ������������һЩ�������Լ���ͷ�ļ���!*/
	//filePath = "C:\\Users\\��ܲ��\\Desktop\\sendEmail.cpp";
	//smtp.AddAttachment(filePath);

	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "����1: �������粻��ͨ������ʧ��!" << endl;
		if (err == 2)
			cout << "����2: �û�������,��˶�!" << endl;
		if (err == 3)
			cout << "����3: �û����������˶�!" << endl;
		if (err == 4)
			cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
	}
	system("pause");
}
