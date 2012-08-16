#include<stdio.h>
#include "sms.h"
#include <memory.h>
#include <windows.h>

//PDU模式 短信中心不能有加号，接收方号码不能有加号
//Text模式 短信中心必须有加号，接收方不限
int main()
{	
	//char * pzPhoneNo="8615802047291";
	//char * pzText = "hello \r\nindex \r15\n16";//123456789d123456789e123456789f123456789g123456789h";
	//char * pzPDU = "字符串的长度是010字符串的长度是020字符串的长度是030字符串的长度是040字符串的长度是050字符串的长度是060字符串的长度是070";


	//打开串口,设置波特率等等
	if (OpenComm("COM1")==false || SetComm()==false)
	{
		printf("open device failed\r\n");
		CloseComm();
		return -1;
	}

	int nError=0;
	char szMsg[CodeLen];

	//初始化
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
