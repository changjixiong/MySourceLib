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

//�յ��¶���,�豸��������������
//  "\r\n+CMTI: "SM",19\r\n"

//PDUģʽ �������Ĳ����мӺţ����շ����벻���мӺ�
//Textģʽ �������ı����мӺţ����շ�����


#ifdef WIN32
bool OpenComm(const char * pzPortName);
bool CloseComm(void);
bool SetComm(void);
#endif


int SendMsg(const char* pzMsg, 
			const char* pzDestPhone,
			const char* pzSMSCenter,
			int		nCodeType);

int SMSDevicCheck(char *pzMsg);		//�����豸
int SIMSpace();//��ӡSIM���Ĵ洢���,������Ҫ,���Դ��ַ����л�ȡ���ÿռ���ܿռ�

int ReadMsg(char *pzMsg, int nCodeType, unsigned index);	//��������Ϊindex(��1��ʼ)����
int deleteMsg(unsigned index);	//ɾ��������Ϊindex(��1��ʼ)�Ķ���

bool smsInit(int nCodeType);		

bool DealNewMsg(void);	//�����¶��Ű���������
bool GetNewMsg(void);	//����Ƿ����¶���

#endif