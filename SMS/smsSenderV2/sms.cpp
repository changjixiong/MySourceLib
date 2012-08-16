#include "sms.h"
#include <memory.h>
#include <stdio.h>

#ifdef WIN32
#include "smsCode.h"
#endif

#define	WaitForDeviceTime	200
#define pduLength			512

int nsmsCount=0;
int nSMSHeadIndex=0;
#define	SMSMaxCount			5
#define SMSCapacity			10

bool WriteData(const void * lpBuffer,                
			   unsigned long nNumberOfBytesToWrite,     
			   unsigned long * lpNumberOfBytesWritten);

bool ReadData(void * lpBuffer,                
			  unsigned long nNumberOfBytesToRead,     
			  unsigned long * lpNumberOfBytesRead);

bool AT_SendMsg_text(const char *  msgCenterNo,
					 const char *  desTelNo,
					 const char *  msg);

#ifdef WIN32
bool AT_SendMsg(const char *  msgCenterNo,
				const char *  desTelNo,
				const char *  msg);
#endif

bool SendATCmd(const char * szATCmd);
void GetATCmd(char * pzCmd);

bool AT_Test(void);
bool AT_Set_CodeType(int nType);
bool AT_Set_Snfs(void);
bool AT_Set_Saic(void);
bool AT_Set_MsgCenter(const char *  msgCenterNo);

//=================以下函数需要根据不同平台重写=========================

#ifdef WIN32
#include <comutil.h>
HANDLE _handlePort=INVALID_HANDLE_VALUE;
#endif

bool WriteData(const void* lpBuffer,                
			   unsigned long nNumberOfBytesToWrite,     
			   unsigned long * lpNumberOfBytesWritten)
{

	#ifdef WIN32

		return (*lpNumberOfBytesWritten > 0
				&& WriteFile(_handlePort, // handle to file to write to
							 lpBuffer,              // pointer to data to write to file
							 nNumberOfBytesToWrite,              // number of bytes to write
							 lpNumberOfBytesWritten,NULL));	

	#else
		
		return FALSE;
	#endif
	
}

bool ReadData(void * lpBuffer,                
			  unsigned long nNumberOfBytesToRead,     
			  unsigned long * lpNumberOfBytesRead)
{
	#ifdef WIN32

		return ReadFile(_handlePort,  // handle of file to read
						 lpBuffer,               // pointer to buffer that receives data
						 nNumberOfBytesToRead,              // number of bytes to read
						 lpNumberOfBytesRead,                 // pointer to number of bytes read
						 NULL);         // pointer to structure for data

	#else

		return FALSE;

	#endif
}

//打开,关闭及设置串口,
#ifdef WIN32
bool OpenComm(const char * pzPortName)
{
	#ifdef WIN32

		_handlePort = CreateFile(pzPortName,  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.
	
		return _handlePort!=INVALID_HANDLE_VALUE;

	#else

		return FALSE;

	#endif
}

bool CloseComm()
{
	#ifdef WIN32

		bool bReturn;
		if (_handlePort==INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
		
		bReturn=CloseHandle(_handlePort);
		_handlePort=INVALID_HANDLE_VALUE;
		return bReturn;	

	#else

		return FALSE;

	#endif
}

bool SetComm()
{
	#ifdef WIN32

		DCB dcb;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;
		dcb.BaudRate = CBR_9600;
		
		DCB _config;
		
		if (GetCommState(_handlePort,&_config) == 0)
		{
			return FALSE;
		}
		
		_config.BaudRate = dcb.BaudRate;    // Specify buad rate of communicaiton.
		_config.StopBits = dcb.StopBits;    // Specify stopbit of communication.
		_config.Parity = dcb.Parity;        // Specify parity of communication.
		_config.ByteSize = dcb.ByteSize;    // Specify  byte of size of communication.
		
		if (SetCommState(_handlePort,&_config) == 0)
		{
			return FALSE;
		}
		
		COMMTIMEOUTS comTimeOut;                   
		
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		
		SetCommTimeouts(_handlePort,&comTimeOut);
		return TRUE;

	#else
		
		return FALSE;
		
	#endif
}

bool GetNewMsg()
{	
	//这个函数判断收到新短信
	char szATCmd[CodeLen];
	char szOneMsg[CodeLen];
	char szCmdTemp[CodeLen];
	char * p=NULL;
	
	if (AT_Set_CodeType(SMS_PDU)==FALSE)
	{
		return FALSE;
	}
	
	sprintf(szATCmd, "AT+CMGL=0\r");
	SendATCmd(szATCmd);
	
	int nHaveTryTime=0;
	memset(szATCmd, 0, CodeLen);
	
	while(nHaveTryTime<WaitForSMSSendOKTime)
	{
		GetATCmd(szCmdTemp);
		strcat(szATCmd, szCmdTemp);
		
		if (strstr(szATCmd, "\r\nERROR\r\n")!=NULL)
		{
			return FALSE;
		}
		
		if (strstr(szATCmd, "\r\nOK\r\n")!=NULL)
		{
			break;
		}
		
		nHaveTryTime++;
	}
	
	if (strstr(szATCmd, "\r\nOK\r\n")==NULL)
	{
		return FALSE;
	}
	
	//AT+CMGL=0\r\r\nOK\r\n
	//AT+CMGL=0\r\r\n+CMGL:1,0,,21\r\nXXXX\r\n\r\nOK
	
	p=strstr(szATCmd,"\r\n")+2;
	
	memset(szOneMsg, 0, CodeLen);
	strncpy(szOneMsg, p, strstr(p, "\r\n") - p);		
	return strlen(szOneMsg)>2;
}
#endif

//=================以上函数需要根据不同平台重写=========================


//=================以下函数需要重写=========================
bool smsInit(int nCodeType)
{
	//删除短信,重置计数
	//如有需要,请重写

	//设备重启以后默认是PDU模式,如果需要TEXT模式,需要设置
	int i;
	if (AT_Set_CodeType(nCodeType)==FALSE)
	{
		return FALSE;
	}
	
	for (i=1;i<=SMSCapacity;i++)
	{
		if (deleteMsg(i)==FALSE)
		{
			return FALSE;
		}
	}
	
	nsmsCount=0;
	nSMSHeadIndex=1;
	return TRUE;

}


bool DealNewMsg()
{
	nsmsCount++;
	
	if (nsmsCount>=SMSMaxCount)
	{
		if (deleteMsg(nSMSHeadIndex)==FALSE)
		{
			return FALSE;
		}
		
		nsmsCount = SMSMaxCount -1;
		
		nSMSHeadIndex = (nSMSHeadIndex>=SMSMaxCount) ? 1 : nSMSHeadIndex+1;
		
	}
	
	//这里按照短信的内容,执行对应的操作
	printf("deal the cmd come from user sms\r\n");
	
	return TRUE;
}

//=================以上函数需要重写=========================


bool SendATCmd(const char * szATCmd)
{
	unsigned long b_len;
	b_len=strlen(szATCmd);

	return WriteData(szATCmd,b_len,&b_len);
}

void GetATCmd(char * pzCmd)
{
	Sleep(WaitForDeviceTime);
	unsigned long b_len;

	memset(pzCmd,0,CodeLen);
	b_len=CodeLen;

	ReadData(pzCmd,b_len,&b_len);
}

bool AT_Test()
{
	char szATCmdGet[CodeLen];

	if (SendATCmd("AT\r")==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);
	
	return (strcmp(szATCmdGet, "AT\r\r\nOK\r\n")==0 
			|| strstr(szATCmdGet, "OK"));		
	
}

bool AT_Set_CodeType(int nType)
{
	char szATCmdGet[CodeLen];
	char szATCmdSend[CodeLen];
	memset(szATCmdSend, 0, CodeLen);	

	switch (nType)
	{
		#ifdef WIN32
		case SMS_PDU:
		strcat(szATCmdSend, "AT+CMGF=0\r");
			break;
		#endif		
		case SMS_TEXT:
			strcat(szATCmdSend, "AT+CMGF=1\r");
			break;
		default:
			return FALSE;
			break;
	}
	
	if (SendATCmd(szATCmdSend)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);

	strcat(szATCmdSend, "\r\nOK\r\n");
	return (strcmp(szATCmdGet, szATCmdSend)==0 
			|| strstr(szATCmdGet, "OK"));	 
}

bool AT_Set_Snfs()
{
	char szATCmdGet[CodeLen];
	char szATCmdSend[CodeLen];
	memset(szATCmdSend, 0, CodeLen);
	strcat(szATCmdSend, "AT^SNFS=2\r");

	if (SendATCmd(szATCmdSend)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);

	strcat(szATCmdSend, "\r\nOK\r\n");
	return (strcmp(szATCmdGet, szATCmdSend)==0
			|| strstr(szATCmdGet, "OK"));
}

bool AT_Set_Saic()
{
	char szATCmdGet[CodeLen];
	char szATCmdSend[CodeLen];
	memset(szATCmdSend, 0, CodeLen);
	strcat(szATCmdSend, "AT^SAIC=2,2,2\r");
	if (SendATCmd(szATCmdSend)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);

	strcat(szATCmdSend, "\r\nOK\r\n");
	return (strcmp(szATCmdGet, szATCmdSend) ==0 
			|| strstr(szATCmdGet, "OK"));
}

bool AT_Set_MsgCenter(const char *  msgCenterNo)
{
	if (strlen(msgCenterNo) > 20)
	{
		return FALSE;
	}

	char szATCmdGet[CodeLen];
	char szATCmdSend[CodeLen];
	memset(szATCmdSend, 0, CodeLen);

	strcat(szATCmdSend, "AT+CSCA=\"");
	strcat(szATCmdSend, msgCenterNo);
	strcat(szATCmdSend, "\"\r");
	
	if (SendATCmd(szATCmdSend)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);

	strcat(szATCmdSend, "\r\nOK\r\n");
	return (strcmp(szATCmdGet, szATCmdSend)==0 
		||strstr(szATCmdGet, "OK"));

}


#ifdef WIN32
bool AT_SendMsg(const char *  msgCenterNo,
				const char *  desTelNo,
				const char *  msg)
{
	int nPduLength;		// PDU串长度
	unsigned char nSmscLength;	// SMSC串长度
	unsigned char m_pdu[pduLength];
	memset(m_pdu,0,pduLength);
	
	// 检查短消息内容是否空，或者超长	
// 	WCHAR wchar[UnicodeLength];
// 	int nCount = ::MultiByteToWideChar(CP_ACP, 0, msg, -1, wchar, UnicodeLength);
// 	if(nCount <= 1)
// 	{
// 		printf("pls input msg\r\n");	
// 		return FALSE;
// 	}
// 	else if(nCount > 70)		// 我们决定全部用UCS2编码，最大70个字符(半角/全角)
// 	{
// 		printf("msg is too long\r\n");		
// 		return FALSE;
// 	}

	SM_PARAM SmParam;
	
	memset(&SmParam, 0, sizeof(SM_PARAM));
	
	// 填充短消息结构
	strcpy(SmParam.SCA, msgCenterNo);
	strcpy(SmParam.TPA, desTelNo);
	strcpy(SmParam.TP_UD, msg);
	SmParam.TP_PID = 0;
	SmParam.TP_DCS = 8;
	
	// 发送短消息		
	nPduLength = gsmEncodePdu(&SmParam, (char*)m_pdu);	// 根据PDU参数，编码PDU串
	strcat((char*)m_pdu, "\x01a");		// 以Ctrl-Z结束
	
	gsmString2Bytes((char*)m_pdu, &nSmscLength, 2);	// 取PDU串中的SMSC信息长度
	nSmscLength++;		// 加上长度字节本身
	
	// 命令中的长度，不包括SMSC信息长度，以数据字节计	
	unsigned char szCmd[32];		// 命令串
	memset(szCmd,0,32);
	sprintf((char*)szCmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);	// 生成命令

	char szATCmdGet[CodeLen];

	if (SendATCmd((char *)szCmd)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);
	strcat((char*)szCmd, "\r\n> ");
	if (strcmp(szATCmdGet, (const char*)szCmd)!=0 
	&& strstr(szATCmdGet, ">")==NULL)
	{				
		return FALSE;
	}		

	//send
	int nhaveTrytime=0;

	char szATCmdTemp[CodeLen];
	
	if (SendATCmd((const char*)m_pdu)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdTemp);
	memset(szATCmdGet,0,CodeLen);
	strcat(szATCmdGet, szATCmdTemp);
	
	while (nhaveTrytime<WaitForSMSSendOKTime)
	{
		memset(szATCmdTemp, 0, CodeLen);
		GetATCmd(szATCmdTemp);
		strcat(szATCmdGet, szATCmdTemp);

		if (strstr(szATCmdGet, "OK") != NULL)
		{
			return TRUE;
		}

		nhaveTrytime++;	
	}

	return FALSE;
}
#endif

bool AT_SendMsg_text(const char *  msgCenterNo,
					const char *  desTelNo,
					const char *  msg)
{

	char szATCmdGet[CodeLen];	
	char szATCmdSend[CodeLen];

	if (AT_Set_MsgCenter(msgCenterNo)==FALSE)
	{
		return FALSE;
	}

	memset(szATCmdGet, 0, CodeLen);
	memset(szATCmdSend, 0, CodeLen);
	strcat(szATCmdSend, "AT+CMGS=\"");
	strcat(szATCmdSend, desTelNo);
	strcat(szATCmdSend, "\"\r");

	if (SendATCmd(szATCmdSend)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);
	strcat(szATCmdSend, "\r\n> ");
	if (strcmp(szATCmdGet,szATCmdSend)!=0 && strstr(szATCmdGet,">")==NULL)
	{				
		return FALSE;
	}		

	//send
	int nhaveTrytime=0;

	char szATCmdTemp[CodeLen];
	memset(szATCmdTemp, 0, CodeLen);
	strcat(szATCmdTemp, msg);
	strcat(szATCmdTemp, "\x01a");

	if (SendATCmd(szATCmdTemp)==FALSE)
	{
		return FALSE;
	}

	GetATCmd(szATCmdGet);

	while (nhaveTrytime<WaitForSMSSendOKTime)
	{
		GetATCmd(szATCmdTemp);
		strcat(szATCmdGet, szATCmdTemp);
		if (strstr(szATCmdGet,"OK")!=NULL)
		{
			return TRUE;
		}
		nhaveTrytime++;			
	}

	return FALSE;
}

int SMSDevicCheck()
{
	if (AT_Test()==FALSE)
	{
		return -1;
	}
	
	if (AT_Set_Snfs()==FALSE)
	{
		return -2;
	}
	
	if (AT_Set_Saic()==FALSE)
	{
		return -4;
	}
	
	return 0;
}	

int SendMsg(const char* pzMsg, 
			const char* pzDestPhone,
			const char* pzSMSCenter,
			int		nCodeType)
{
	
	if (AT_Test()==FALSE)
	{
		return -1;
	}		

	if (AT_Set_CodeType(nCodeType)==FALSE)
	{
		return -2;
	}
	
	switch (nCodeType)
	{
	#ifdef WIN32
	case SMS_PDU:
		if (AT_SendMsg(pzSMSCenter,pzDestPhone,pzMsg)==FALSE)
		{
			return -4;
		}
		break;
	#endif
	case SMS_TEXT:
		if (AT_SendMsg_text(pzSMSCenter,pzDestPhone,pzMsg)==FALSE)
		{
			return -5;
		}
		break;
	default:
		return -6;
		break;
	}	
	
	return 0;
}

int ReadMsg(char *pzMsg, int nCodeType, unsigned index)
{
	if (index==0)
	{
		return -1;
	}

	if (AT_Set_CodeType(nCodeType)==FALSE)
	{
		return -2;
	}

	char szATCmd[CodeLen];
	char szOneMsg[CodeLen];
	char szTemp[CodeLen];
	char *p=NULL;

	int nhaveTrytime=0;

	sprintf(szATCmd,"AT+CMGR=%d\r", index);
	SendATCmd(szATCmd);
	
	nhaveTrytime=0;
	memset(szATCmd, 0, CodeLen);
	while (nhaveTrytime<WaitForSMSSendOKTime)
	{
		GetATCmd(szTemp);
		strcat(szATCmd, szTemp);	
		
		if (strstr(szATCmd, "\r\nERROR\r\n")!=NULL)
		{
			return -3;
		}

		if (strstr(szATCmd, "\r\nOK\r\n")!=NULL)
		{
			break;
		}

		nhaveTrytime++;
	}

	if (strstr(szATCmd, "\r\nOK\r\n")==NULL)
	{
		return -4;
	}

	//完整消息 text模式    15\r\n 这个15就是数据
	//		AT+CMGR=1\r
	//		\r\n
	//		+CMGR: "REC READ","+8615802047291",,"12/05/25,14:32:48+32"\r\n
	//		15\r\n
	//		\r\n
	//		OK\r\n

	//		AT+CMGR=11\r
	//		\r\n
	//		+CMGR: 0,,0\r\n
	//		\r\n
	//		OK
	//		\r\n

	//完整消息 PDU模式    xxxx\r\n 这个xxxx就是数据
	//		AT+CMGR=1\r
	//		\r\n
	//		+CMGR: 1,,22\r\n
	//		xxxxxxxx\r\n
	//		\r\n
	//		OK\r\n

	p=strstr(strstr(szATCmd,"\r\n")+2,"\r\n")+2;
	
	memset(szOneMsg, 0, CodeLen);
	strncpy(szOneMsg, p, strstr(p, "\r\n") - p);		

	#ifdef WIN32
	if (nCodeType == SMS_PDU)
	{
		SM_PARAM sm_p;
		memset(&sm_p, 0, sizeof(sm_p));
		gsmDecodePdu(szOneMsg, &sm_p);
		sprintf(pzMsg, "%s", sm_p.TP_UD);
	}
	else
	{
		sprintf(pzMsg, "%s", szOneMsg);
	}
	#endif

	sprintf(pzMsg, "%s", szOneMsg);
	
	return 0;
}

int deleteMsg(unsigned index)
{
	if (index==0)
	{
		return -1;
	}

	char szATCmd[CodeLen];
	char sztemp[CodeLen];
	sprintf(szATCmd, "AT+CMGD=%d\r", index);

	if (SendATCmd(szATCmd)==FALSE)
	{
		return FALSE;
	}

	int nhaveTrytime=0;

	memset(szATCmd, 0, CodeLen);
	while(nhaveTrytime<WaitForSMSSendOKTime)
	{
		GetATCmd(sztemp);
		strcat(szATCmd, sztemp);

		if (strstr(szATCmd, "\r\nERROR\r\n")!=NULL)
		{
			return FALSE;
		}

		if (strstr(szATCmd, "\r\nOK\r\n")!=NULL)
		{
			return TRUE;
		}

		nhaveTrytime++;
	}
	return FALSE;
}

int SIMSpace()
{
	/*
	AT+CPMS?\r
	\r\n
	+CPMS: "SM",1,50,"SM",1,50,"SM",1,50\r\n
	\r\n
	OK\r\n
	*/
	char szATCmd[CodeLen];
	sprintf(szATCmd, "AT+CPMS?\r");
	SendATCmd(szATCmd);
	GetATCmd(szATCmd);

	printf(szATCmd);
	return 0;
}

// int listMsg()
// {
// 		char szATCmd[CodeLen];
// 		sprintf(szATCmd, "AT+CMGL=1\r");
// 		SendATCmd(szATCmd);
// 		GetATCmd(szATCmd);
// 		
// 		printf(szATCmd);
// 		return 0;
// }

