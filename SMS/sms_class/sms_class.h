#ifndef SMS_HEAD
#define SMS_HEAD

#include <string>
using namespace std;


#define Data_buffer_length  1000
// 用户信息编码方式
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

#define SMS_PDU			0
#define	SMS_TEXT		1
#define	waitForReturnCMD 100

const int WaitForSMSSendOKTime=100;
const int pduLength=4096;
const int UnicodeLength=2*pduLength;

//缓冲区结构
struct Data_buffer
{
	BYTE b_data[Data_buffer_length];
	unsigned long b_len;
	BYTE *pHead,*pTail;
	
	void Init(const char * pzChar)
	{
		memset(b_data,0,Data_buffer_length);
		memcpy(b_data,pzChar,strlen(pzChar));
		pHead=b_data;
		b_len=strlen(pzChar);
		pTail=pHead+b_len;
	}
	
	void Init()
	{
		memset(b_data,0,Data_buffer_length);
		pHead=b_data;
		b_len=Data_buffer_length;
		pTail=pHead+b_len;
	}
	
	void Clear()
	{
		b_len=0;
		pHead=NULL;
		pTail=NULL;
		memset(b_data,0,Data_buffer_length);
	}
};

// 短消息参数结构，编码/解码共用
// 其中，字符串以'\0'结尾
typedef struct {
	char SCA[16];			// 短消息服务中心号码(SMSC地址)
	char TPA[16];			// 目标号码或回复号码(TP-DA或TP-RA)
	char TP_PID;			// 用户信息协议标识(TP-PID)
	char TP_DCS;			// 用户信息编码方式(TP-DCS)
	char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用到
	char TP_UD[160];		// 原始用户信息(编码前或解码后的TP-UD)
	short index;			// 短消息序号，在读取时用到
} SM_PARAM;

int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);

class CSMS
{
public:
	CSMS();
	~CSMS();
	bool AT_Test();
	bool AT_Set_CodeType(int nType);
	bool AT_Set_Snfs();
	bool AT_Set_Saic();
	bool SendATCmd(const char * szATCmd);
	bool SendATCmd(const string &  strATCmd);
	bool AT_Set_MsgCenter(const string & msgCenterNo);
	bool AT_SendMsg(const string & msgCenterNo,
							const string & desTelNo,
							const string & msg);
	bool AT_SendMsg_text(const string & msgCenterNo,
						const string & desTelNo,
						const string & msg);

	bool OpenComm(const char * pzPortName);
	bool CloseComm();

	bool SetComm(const DCB & dcb);
	bool SetComm();
	bool SetCommName(const string & strCommName)
	{
		_strComName=strCommName;
		return true;
	}

	bool WriteData(
					LPCVOID lpBuffer,                
					DWORD nNumberOfBytesToWrite,     
					LPDWORD lpNumberOfBytesWritten
					)
	{
		if (*lpNumberOfBytesWritten > 0)
		{
			if (WriteFile(_handlePort, // handle to file to write to
				lpBuffer,              // pointer to data to write to file
				nNumberOfBytesToWrite,              // number of bytes to write
				lpNumberOfBytesWritten,NULL) == 0)      // pointer to number of bytes written
			{			
				return FALSE;
			}
			return TRUE;
		}
		return FALSE;
	}

	bool ReadData(LPVOID lpBuffer,                
					DWORD nNumberOfBytesToRead,     
					LPDWORD lpNumberOfBytesRead)
	{
		if (ReadFile(_handlePort,  // handle of file to read
			lpBuffer,               // pointer to buffer that receives data
			nNumberOfBytesToRead,              // number of bytes to read
			lpNumberOfBytesRead,                 // pointer to number of bytes read
			NULL) == 0)              // pointer to structure for data
		{
			return FALSE;
		}
		return TRUE;
	}

	int SendMsg(const string & strMsg, const string & strDestPhone,const string & strSMSCenter,const string & strSMSMode,bool bChecked=FALSE)
	{
		int nCodeType;

		if (0==strncmp("text",strSMSMode.c_str(),strSMSMode.length()))
		{
			nCodeType=SMS_TEXT;
		}
		else if (0==strncmp("pdu",strSMSMode.c_str(),strSMSMode.length()))
		{
			nCodeType=SMS_PDU;
		}


		if (bChecked==false)
		{
			int nCheckError=0;

			nCheckError=SMSDevicCheck(strSMSMode);

			if (nCheckError)
			{
				return nCheckError;
			}
		}		

		if (AT_Set_MsgCenter(strSMSCenter)==FALSE)
		{
			CloseComm();
			return -5;
		}

		switch (nCodeType)
		{
		case SMS_PDU:
			if (AT_SendMsg(strSMSCenter,strDestPhone,strMsg)==FALSE)
			{
				CloseComm();
				return -6;
			}
			break;
		case SMS_TEXT:
			if (AT_SendMsg_text(strSMSCenter,strDestPhone,strMsg)==FALSE)
			{
				CloseComm();
				return -6;
			}
			break;
		default:
			CloseComm();
			return -6;
			break;
		}


		return 0;
	}

	int SendMsg(const string & strMsg, const string & strDestPhone,const string & strSMSMode)
	{
		return SendMsg(strMsg , strDestPhone , _SMSCenter,strSMSMode);
	}

	int SMSDevicCheck(const string & strSMSMode)
	{
		int nCodeType;
		
		if (0==strncmp("text",strSMSMode.c_str(),strSMSMode.length()))
		{
			nCodeType=SMS_TEXT;
		}
		else if (0==strncmp("pdu",strSMSMode.c_str(),strSMSMode.length()))
		{
			nCodeType=SMS_PDU;
		}

		if (_bComOpened==false)
		{
			OpenComm(_strComName.c_str());
			SetComm();
		}
		if (AT_Test()==FALSE)
		{
			CloseComm();
			return -1;
		}
		
		if (AT_Set_CodeType(nCodeType)==FALSE)
		{
			CloseComm();
			return -2;
		}
		
		if (AT_Set_Snfs()==FALSE)
		{
			CloseComm();
			return -3;
		}
		
		if (AT_Set_Saic()==FALSE)
		{
			CloseComm();
			return -4;
		}

		return 0;
	}	

	bool ComIsOpened()
	{
		return _bComOpened;
	}

	bool ComIsSetted()
	{
		return _bComSetted;
	}

	void GetATCmd(string & strCmd);


private:
	DCB _config;
	HANDLE _handlePort;

	Data_buffer  s_buf;
    Data_buffer  r_buf;  
	string _SMSCenter;

	bool _bComOpened;
	bool _bComSetted;
	string _strComName;
	
};

#endif