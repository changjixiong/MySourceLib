#ifndef SMSCODE_HEADER
#define	SMSCODE_HEADER

// ����Ϣ�����ṹ������/���빲��
// ���У��ַ�����'\0'��β
typedef struct {
	char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[160];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	short index;			// ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);

#endif




















