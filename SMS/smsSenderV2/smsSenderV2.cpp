#include<stdio.h>
#include "sms.h"
#include <memory.h>
#include <windows.h>

//PDUģʽ �������Ĳ����мӺţ����շ����벻���мӺ�
//Textģʽ �������ı����мӺţ����շ�����
int main()
{	
	//char * pzPhoneNo="8615802047291";
	//char * pzText = "hello \r\nindex \r15\n16";//123456789d123456789e123456789f123456789g123456789h";
	//char * pzPDU = "�ַ����ĳ�����010�ַ����ĳ�����020�ַ����ĳ�����030�ַ����ĳ�����040�ַ����ĳ�����050�ַ����ĳ�����060�ַ����ĳ�����070";


	//�򿪴���,���ò����ʵȵ�
	if (OpenComm("COM1")==false || SetComm()==false)
	{
		printf("open device failed\r\n");
		CloseComm();
		return -1;
	}

	int nError=0;
	char szMsg[CodeLen];

	//��ʼ��
// 	if (smsInit(SMS_TEXT)==false)
// 	{
// 		CloseComm();
// 		printf("smsInit failed\r\n");
// 		return -1;
// 	}

//	ReadMsg(szMsg,SMS_TEXT,11);
	SIMSpace();

// 	while(1)
// 	{
// 		printf("-------------------------\r\n");
// 		if (GetNewMsg())
// 		{
// 			DealNewMsg();
// 			
// 			for (int i=1;i<=10;i++)
// 			{
// 				memset(szMsg, 0, CodeLen);
// 				ReadMsg(szMsg,SMS_TEXT,i);
// 				printf("%s\r\n", szMsg);
// 			}
// 			
// 			SIMSpace();
// 			
// 		}
// 		Sleep(500);
// 	}

	CloseComm();

	return 0;
}
