#include <StdAfx.h>
#include "sms.h"
#include <comutil.h>

// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ������ָ��
// ����: Ŀ�����ݳ���
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	for (int i = 0; i < nSrcLength; i += 2)
	{
		// �����4λ
		if ((*pSrc >= '0') && (*pSrc <= '9'))
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// �����4λ
		if ((*pSrc>='0') && (*pSrc<='9'))
		{
			*pDst |= *pSrc - '0';
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		//TRACE("%s",pSrc);
		pDst++;
		//TRACE("%s",pDst);
	}

	// ����Ŀ�����ݳ���
	return (nSrcLength / 2);
}

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// ����: pSrc - Դ����ָ��
//       nSrcLength - Դ���ݳ���
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�

	for (int i = 0; i < nSrcLength; i++)
	{
		*pDst++ = tab[*pSrc >> 4];		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';   

	// ����Ŀ���ַ�������
	return (nSrcLength * 2);
}

// 7bit����
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nChar;		// ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	unsigned char nLeft;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	// ѭ���ô�����̣�ֱ��Դ����������
	// ������鲻��8�ֽڣ�Ҳ����ȷ����
	while (nSrc < nSrcLength)
	{
		// ȡԴ�ַ����ļ���ֵ�����3λ
		nChar = nSrc & 7;

		// ����Դ����ÿ���ֽ�
		if(nChar == 0)
		{
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			nLeft = *pSrc;
		}
		else
		{
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			nLeft = *pSrc >> nChar;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����Ŀ�괮����
	return nDst;
}

// 7bit����
// ����: pSrc - Դ���봮ָ��
//       nSrcLength - Դ���봮����
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nByte;		// ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	unsigned char nLeft;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	
	// �����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;

	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength)
	{
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);

		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;

		// �޸��ֽڼ���ֵ
		nByte++;

		// ����һ������һ���ֽ�
		if(nByte == 7)
		{
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;

			// �����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ�괮����
	return nDst;
}

// 8bit����
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	// �򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	return nSrcLength;
}

// 8bit����
// ����: pSrc - Դ���봮ָ��
//       nSrcLength -  Դ���봮����
// ���: pDst -  Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	// �򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	// ����ַ����Ӹ�������
	*pDst = '\0';

	return nSrcLength;
}

// UCS2����
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE���ַ���Ŀ
	WCHAR wchar[128];	// UNICODE��������

	// �ַ���-->UNICODE��
	nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	// �ߵ��ֽڶԵ������
	for(int i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		// �������λ�ֽ�
		*pDst++ = wchar[i] & 0xff;		// �������λ�ֽ�
	}
//	TRACE("%s",wchar);
	// ����Ŀ����봮����
	return nDstLength * 2;
}

// UCS2����
// ����: pSrc - Դ���봮ָ��
//       nSrcLength -  Դ���봮����
// ���: pDst -  Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// UNICODE���ַ���Ŀ
	WCHAR wchar[128];	// UNICODE��������

	// �ߵ��ֽڶԵ���ƴ��UNICODE
	for(int i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	// �ȸ�λ�ֽ�
		wchar[i] |= *pSrc++;		// ���λ�ֽ�
	}

	// UNICODE��-->�ַ���
	nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);

	// ����ַ����Ӹ�������
	pDst[nDstLength] = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}

// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�

	// ���ƴ�����
	nDstLength = nSrcLength;

	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		// �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�
		*pDst++ = ch;		// �����ȳ��ֵ��ַ�
	}

	// Դ��������������
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';	// ��'F'
		nDstLength++;		// Ŀ�괮���ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�

	// ���ƴ�����
	nDstLength = nSrcLength;

	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		// �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�
		*pDst++ = ch;		// �����ȳ��ֵ��ַ�
	}

	// �����ַ���'F'��
	if(*(pDst-1) == 'F')
	{
		pDst--;
		nDstLength--;		// Ŀ���ַ������ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// ����: pSrc - ԴPDU����ָ��
// ���: pDst - Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
	int nLength;			// �ڲ��õĴ�����
	int nDstLength;			// Ŀ��PDU������
	unsigned char buf[256];	// �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	nLength = strlen(pSrc->SCA);	// SMSC��ַ�ַ����ĳ���	
	buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;	// SMSC��ַ��Ϣ����
//	TRACE("%c",buf[0]);
	buf[1] = 0x91;		// �̶�: �ù��ʸ�ʽ����
	nDstLength = gsmBytes2String(buf, pDst, 2);		// ת��2���ֽڵ�Ŀ��PDU��
//	TRACE("%s",pDst);
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);	// ת��SMSC���뵽Ŀ��PDU��
//	TRACE("%s",pDst);

	// TPDU�λ���������Ŀ���ַ��
	nLength = strlen(pSrc->TPA);	// TP-DA��ַ�ַ����ĳ���
	buf[0] = 0x11;					// �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	buf[1] = 0;						// TP-MR=0
	buf[2] = (char)nLength;			// Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	buf[3] = 0x91;					// �̶�: �ù��ʸ�ʽ����
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);		// ת��4���ֽڵ�Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);	// ת��TP-DA��Ŀ��PDU��

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	nLength = strlen(pSrc->TP_UD);	// �û���Ϣ�ַ����ĳ���
	buf[0] = pSrc->TP_PID;			// Э���ʶ(TP-PID)
	buf[1] = pSrc->TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2] = 0;						// ��Ч��(TP-VP)Ϊ5����
	if(pSrc->TP_DCS == GSM_7BIT)	
	{
		// 7-bit���뷽ʽ
		buf[3] = nLength;			// ����ǰ����
		nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;	// ת��TP-DA��Ŀ��PDU��
	}
	else if(pSrc->TP_DCS == GSM_UCS2)
	{
		// UCS2���뷽ʽ
		buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);	// ת��TP-DA��Ŀ��PDU��		
		nLength = buf[3] + 4;		// nLength���ڸö����ݳ���
	}
	else
	{
		// 8-bit���뷽ʽ
		buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);	// ת��TP-DA��Ŀ��PDU��
		nLength = buf[3] + 4;		// nLength���ڸö����ݳ���
	}
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);		// ת���ö����ݵ�Ŀ��PDU��

	// ����Ŀ���ַ�������
	return nDstLength;
}

// PDU���룬���ڽ��ա��Ķ�����Ϣ
// ����: pSrc - ԴPDU��ָ��
// ���: pDst - Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength;			// Ŀ��PDU������
	unsigned char tmp;		// �ڲ��õ���ʱ�ֽڱ���
	unsigned char buf[256];	// �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	gsmString2Bytes(pSrc, &tmp, 2);	// ȡ����
	tmp = (tmp - 1) * 2;	// SMSC���봮����
	pSrc += 4;			// ָ����ƣ�������SMSC��ַ��ʽ
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp);	// ת��SMSC���뵽Ŀ��PDU��
	pSrc += tmp;		// ָ�����

	// TPDU�λ�������
	gsmString2Bytes(pSrc, &tmp, 2);	// ȡ��������
	pSrc += 2;		// ָ�����

	// ȡ�ظ�����
	gsmString2Bytes(pSrc, &tmp, 2);	// ȡ����
	if(tmp & 1) tmp += 1;	// ������ż��
	pSrc += 4;			// ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ
	gsmSerializeNumbers(pSrc, pDst->TPA, tmp);	// ȡTP-RA����
	pSrc += tmp;		// ָ�����

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);	// ȡЭ���ʶ(TP-PID)
	pSrc += 2;		// ָ�����
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);	// ȡ���뷽ʽ(TP-DCS)
	pSrc += 2;		// ָ�����
	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);		// ����ʱ����ַ���(TP_SCTS) 
	pSrc += 14;		// ָ�����
	gsmString2Bytes(pSrc, &tmp, 2);	// �û���Ϣ����(TP-UDL)
	pSrc += 2;		// ָ�����
	if(pDst->TP_DCS == GSM_7BIT)	
	{
		// 7-bit����
		nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// ��ʽת��
		gsmDecode7bit(buf, pDst->TP_UD, nDstLength);	// ת����TP-DU
		nDstLength = tmp;
	}
	else if(pDst->TP_DCS == GSM_UCS2)
	{
		// UCS2����
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);			// ��ʽת��
		nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength);	// ת����TP-DU
	}
	else
	{
		// 8-bit����
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);			// ��ʽת��
		nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength);	// ת����TP-DU
	}

	// ����Ŀ���ַ�������
	return nDstLength;
}


//-------------------------------------------------

CSMS::CSMS()
{
	_bComOpened=FALSE;
	_bComSetted=FALSE;
	_SMSCenter="";
}


CSMS::~CSMS()
{

}

bool CSMS::SendATCmd(const char * szATCmd)
{
	s_buf.Init(szATCmd);
	if (WriteData(s_buf.pHead,s_buf.b_len,&s_buf.b_len)==true)
	{
		s_buf.Clear();
		return TRUE;
	}
	else
	{
		s_buf.Clear();
		return false;
	}
	
}

bool CSMS::SendATCmd(const string &  strATCmd)
{
	return SendATCmd(strATCmd.c_str());
}

void CSMS::GetATCmd(string & strCmd)
{
	r_buf.Init();
	ReadData(r_buf.pHead,r_buf.b_len,&r_buf.b_len);
	strCmd=(const char *)r_buf.pHead;
	r_buf.Clear();

}

bool CSMS::AT_Test()
{
	string strATCmdGet;
	string strATCmdSend;
	strATCmdSend="AT\r";
	if (SendATCmd(strATCmdSend)==TRUE)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\nOK\r\n" && strATCmdGet.find("OK")==string::npos)
		{				
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;	
	}

}

bool CSMS::AT_Set_CodeType(int nType)
{
	string strATCmdGet;
	string strATCmdSend;

	switch (nType)
	{
		case SMS_PDU:
			strATCmdSend="AT+CMGF=0\r";
			break;
		case SMS_TEXT:
			strATCmdSend="AT+CMGF=1\r";
			break;
		default:
			return false;
			break;
	}
	
	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\nOK\r\n" && strATCmdGet.find("OK")==string::npos)
		{				
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	 
}

bool CSMS::AT_Set_Snfs()
{
	string strATCmdGet;
	string strATCmdSend;
	strATCmdSend="AT^SNFS=2\r";
	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\nOK\r\n" && strATCmdGet.find("OK")==string::npos)
		{				
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

}
bool CSMS::AT_Set_Saic()
{
	string strATCmdGet;
	string strATCmdSend;
	strATCmdSend="AT^SAIC=2,2,2\r";

	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\nOK\r\n" && strATCmdGet.find("OK")==string::npos)
		{				
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

}

bool CSMS::AT_Set_MsgCenter(const string & msgCenterNo)
{
	//string strATCms;
	if (msgCenterNo.length() > 20)
	{
		return false;
	}

	string strATCmdGet;
	string strATCmdSend;
	strATCmdSend="AT+CSCA=\""+msgCenterNo+"\"\r";

	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\nOK\r\n" && strATCmdGet.find("OK")==string::npos)
		{				
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

}

bool CSMS::AT_SendMsg(const string & msgCenterNo,
									const string & desTelNo,
									const string & msg)
{
	int nPduLength;		// PDU������
	unsigned char nSmscLength;	// SMSC������
	TBYTE m_pdu[pduLength];
	memset(m_pdu,0,pduLength);
	
	// ������Ϣ�����Ƿ�գ����߳���
	CString strUnicode;
	WCHAR wchar[UnicodeLength];
	int nCount = ::MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, wchar, UnicodeLength);
	if(nCount <= 1)
	{
		AfxMessageBox("pls input msg");
		return false;
	}
	else if(nCount > 70)		// ���Ǿ���ȫ����UCS2���룬���70���ַ�(���/ȫ��)
	{
		AfxMessageBox("msg is too long");
		return false;
	}
	SM_PARAM SmParam;
	
	memset(&SmParam, 0, sizeof(SM_PARAM));
	
	// ȥ������ǰ��"+"
	string strSmsc=msgCenterNo;
	string strNumber=desTelNo;
    string strContent=msg;
	if(strSmsc[0] == '+')  strSmsc = strSmsc.substr(1);
	if(strNumber[0] == '+')  strNumber = strNumber.substr(1);
	
	// �ں���ǰ��"86"
	//if(strSmsc.substr(0,2) != "86")  strSmsc = "86" + strSmsc;
	//if(strNumber.substr(0,2) != "86")  strNumber = "86" + strNumber;
	
	// ������Ϣ�ṹ
	strcpy(SmParam.SCA, strSmsc.c_str());
	strcpy(SmParam.TPA, strNumber.c_str());
	strcpy(SmParam.TP_UD, strContent.c_str());
	SmParam.TP_PID = 0;
	SmParam.TP_DCS = 8;
	
	// ���Ͷ���Ϣ	
	
	nPduLength = gsmEncodePdu(&SmParam, (char*)m_pdu);	// ����PDU����������PDU��
	strcat((char*)m_pdu, "\x01a");		// ��Ctrl-Z����
	
	gsmString2Bytes((char*)m_pdu, &nSmscLength, 2);	// ȡPDU���е�SMSC��Ϣ����
	nSmscLength++;		// ���ϳ����ֽڱ���
	
	// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
	
	TBYTE cmd[16];		// ���
	memset(cmd,0,16);
	sprintf((char*)cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);	// ��������

	string strATCmdGet;
	string strATCmdSend;
	strATCmdSend=(char *)cmd;

	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\n> " && strATCmdGet.find(">")==string::npos)
		{				
			return false;
		}		
	}
	else
	{
		return false;
	}

	//send
	int nhaveTrytime=0;

	strATCmdGet;
	strATCmdSend=(const char*)m_pdu;
	string strATCmdTemp="";
	
	if (SendATCmd(strATCmdSend)==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdTemp);
		strATCmdGet=strATCmdTemp;
		while (nhaveTrytime<WaitForSMSSendOKTime)
		{
			Sleep(waitForReturnCMD*3);//�������賤һ��
			GetATCmd(strATCmdTemp);
			strATCmdGet+=strATCmdTemp;
			if (strATCmdGet.find("OK")==string::npos)
			{
				nhaveTrytime++;	
				continue;
			}
			else
			{
				return true;
			}			
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool CSMS::AT_SendMsg_text(const string & msgCenterNo,
									const string & desTelNo,
									const string & msg)
{
	// ȥ������ǰ��"+"
	string strSmsc=msgCenterNo;
	string strNumber=desTelNo;
    string strContent=msg;

	string strATCmdGet;
	string strATCmdSend;
//	strATCmdSend=(char *)cmd;

	if (SendATCmd("AT+CMGS=\""+strNumber+"\"\r")==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdGet);
		if (strATCmdGet!=strATCmdSend+"\r\n> " && strATCmdGet.find(">")==string::npos)
		{				
			return false;
		}		
	}
	else
	{
		return false;
	}

	//send
	int nhaveTrytime=0;

	strATCmdGet;
	string strATCmdTemp="";
	
	if (SendATCmd(msg+"\x01a")==true)
	{
		Sleep(waitForReturnCMD);
		GetATCmd(strATCmdTemp);
		strATCmdGet=strATCmdTemp;
		while (nhaveTrytime<WaitForSMSSendOKTime)
		{
			Sleep(waitForReturnCMD*3);//�������賤һ��
			GetATCmd(strATCmdTemp);
			strATCmdGet+=strATCmdTemp;
			if (strATCmdGet.find("OK")==string::npos)
			{
				nhaveTrytime++;	
				continue;
			}
			else
			{
				return true;
			}			
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool CSMS::OpenComm(const char * pzPortName)
{
	_handlePort = CreateFile(pzPortName,  // Specify port device: default "COM1"
		GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
		0,                                  // the devide isn't shared.
		NULL,                               // the object gets a default security.
		OPEN_EXISTING,                      // Specify which action to take on file. 
		0,                                  // default.
		NULL);                              // default.
	
	if (_handlePort==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	_bComOpened=true;
	_strComName=pzPortName;
	return true;
}

bool CSMS::CloseComm()
{
	bool bReturn;
	if (_handlePort==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	{
		bReturn=CloseHandle(_handlePort);
		_handlePort=INVALID_HANDLE_VALUE;
		_bComOpened=false;
		return bReturn;
	}
	
}

bool CSMS::SetComm(const DCB & dcb)
{
	if (_bComOpened)
	{
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
		_bComSetted=true;
		return true;
	}
	else
	{
		return false;
	}
	
}

bool CSMS::SetComm()
{
	DCB dcb;
	dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
	dcb.BaudRate = CBR_9600;
	
	return SetComm(dcb);
	
}