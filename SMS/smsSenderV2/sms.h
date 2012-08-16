#ifndef SMS_HEAD
#define SMS_HEAD

#ifdef WIN32
#define SMS_PDU						0
#endif

#define	SMS_TEXT					1
#define CodeLen						512

#define WaitForSMSSendOKTime		5
#define MaxStoreMsgNum				20
//#define UnicodeLength				8192

//收到新短信,设备会上送如下数据
//  "\r\n+CMTI: "SM",19\r\n"

//PDU模式 短信中心不能有加号，接收方号码不能有加号
//Text模式 短信中心必须有加号，接收方不限


#ifdef WIN32
bool OpenComm(const char * pzPortName);
bool CloseComm(void);
bool SetComm(void);
#endif


int SendMsg(const char* pzMsg, 
			const char* pzDestPhone,
			const char* pzSMSCenter,
			int		nCodeType);

int SMSDevicCheck(char *pzMsg);		//测试设备
int SIMSpace();//打印SIM卡的存储情况,如有需要,可以从字符串中获取已用空间和总空间

int ReadMsg(char *pzMsg, int nCodeType, unsigned index);	//读索引号为index(从1开始)短信
int deleteMsg(unsigned index);	//删除索引号为index(从1开始)的短信

bool smsInit(int nCodeType);		

bool DealNewMsg(void);	//处理新短信包含的命令
bool GetNewMsg(void);	//检测是否有新短信

#endif