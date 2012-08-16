// MSMQ.h: interface for the CMSMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSMQ_H__AE9093B7_53CA_414D_8AE5_7C94D58F7C4B__INCLUDED_)
#define AFX_MSMQ_H__AE9093B7_53CA_414D_8AE5_7C94D58F7C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "mqoa.dll"
using namespace MSMQ;
#include "Include.h"
#include "StringTool.h"
#include "StringTool.h"
#include "msg.h"
#include "MSGSQLDATA.h"
#include "msgsqldatartu.h"
#include "msgcomtradedata.h"
#include "MSGCOMTRADEDATA_DRRMS.H"
#include "MSGAlarm.h"


template <typename T>
HRESULT ChangeStruct2Variant (CComVariant &var, T *pData)
{	
	HRESULT hr = S_OK;	
	// 使用SafeArrayCreateVector API创建一个单维SAFEARRAY，分配一个sizeof(_DATA)大小的连续内存块	
	// VT--UI1代表非负整形的变量类型，1个字节	
	// 常数'0'定义数组的下界	
	LPSAFEARRAY lpsa = SafeArrayCreateVector(VT_UI1, 0, sizeof(T));	
	
	LPBYTE pbData = NULL;	
	
	if (lpsa)		
	{		
		//在你访问SAFEARRAY数据之前，你必须调用SafeArrayAccessData。该函数锁定数据并且返回一个指针。在这里，锁定数组意味着增加该数组的内部计数器（cLocks）
		hr = SafeArrayAccessData(lpsa, (void **)&pbData);		
	}	
	else
		hr = HRESULT_FROM_WIN32(GetLastError());
	
	if (SUCCEEDED(hr))		
	{		
		// 使用safe array：		
		// 将传入的_DATA指针指向的内存复制到pbData	
		//int i;
		//i=sizeof(*pData);
		CopyMemory(pbData, pData, sizeof(*pData));		
		// 设置var的类型为数组		
		var.vt = VT_ARRAY|VT_UI1;		
		// 将var和我们的单维SAFEARRAY拉上关系：		
		var.parray = lpsa;		
	}	
	
	if (pbData)		
	{
		//相应用来释放数据的函数是SafeArrayUnaccessData()，该功能释放该参数的计数。		
		SafeArrayUnaccessData(var.parray);		
	}
	
	if (FAILED(hr))		
	{
		// 销毁SAFEARRAY		
		SafeArrayDestroy(lpsa);		
	}	
	return hr;	
}

template <typename T>
HRESULT ChangeVariant2Struct (CComVariant &var, T *DP)
{
	
	SAFEARRAY* psa;
	
	BYTE HUGEP *lpb;
	
	psa = var.parray;
	
	SafeArrayAccessData(psa, (void HUGEP **)&lpb);
	
	CopyMemory((LPVOID)DP, (LPVOID)lpb, sizeof(T));
	
	SafeArrayUnaccessData(psa);	
	
	return S_OK;
	
}

class CMSMQ  
{
public:

	CMSMQ();
	virtual ~CMSMQ();

public:
	void CreateQueue(CString _sServerName,CString _sLabel);
	void CreateQueue(CString _sIPAddress,CString _sQueueName,CString _sLabel,CString _sQueueType="private");
	
	bool SendMessage(CString _sServerName,CString _sMessage,bool _bRecoverable=true);
	bool SendMessage(const CString & _sIPAddress,
						const CString & _sQueueName,
						CString _sMessage,
						CString _sQueueType="private",
						bool _bRecoverable=true);	
		
	CString ReadMessage(CString _sServerName);
	CString ReadMessage(const CString & _sIPAddress,const CString & _sQueueName);	
	void ReleaseAll();

public:
	IMSMQQueuePtr		qDest;		// Represents Destination Queue
	IMSMQQueueInfoPtr	qInfo;		//Represents an open instance of the destination queue
	IMSMQMessagePtr     qMsg;		//Represents the message
	
	LPTSTR lpszSystemInfo;			// pointer to system information string 
	DWORD cchBuff ;					// size of computer 
	TCHAR tchBuffer[1024];			// buffer for string
	CString m_ServerName;
	HWND m_hParent;                 //the hwnd of the window  which use this class's object

public:
	template<typename T>
	bool RecevieMessage_temp(T * msg,
							const CString & strIPAddress,
							const CString & strQueueName,
							CString strQueueType,
							long nTimeOut=3000)
	{
		CString sServerName;
		_variant_t vtReceiveTimeout;
		vtReceiveTimeout = nTimeOut;
		_bstr_t label,body;
		BSTR bstrServerName,bstrfname;
		CString fname;
		
		if (strIPAddress.CompareNoCase("localhost")==0||strIPAddress=="127.0.0.1")
		{
			sServerName=".";
		}
		else
		{
			sServerName=strIPAddress;
		}
		
		if (strQueueType=="private")
		{
			sServerName+="\\Private$\\"+strQueueName;	
		}
		else
		{
			return FALSE;
		}
		
		try
		{
			bstrServerName=sServerName.AllocSysString();
			
			if (sServerName.Left(1)=='.')
			{
				fname="DIRECT=OS:";
			}
			else
			{
				fname="DIRECT=TCP:";
			}
			
			fname+=sServerName;	
			bstrfname=fname.AllocSysString();
			
			qInfo->PathName=bstrServerName;
			SysFreeString(bstrServerName);		
			qInfo->FormatName = bstrfname;
			SysFreeString(bstrfname);
			qDest = qInfo->Open(MQ_RECEIVE_ACCESS,MQ_DENY_NONE);
			
			qMsg = qDest->Receive(&vtMissing,&vtMissing,&vtMissing,&vtReceiveTimeout);
			ChangeVariant2Struct(CComVariant(qMsg->Body), msg);
			qDest->Close();
			
			return true;		
		}
		catch(_com_error e)
		{
			sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
			return false;		
		}
	}

	template<typename T>
	bool SendMessage_temp(const CString & strIPAddress,
							const CString & strQueueName,
							T & Message,
							CString strQueueType,
							bool bRecoverable)
	{
		CString sServerName;
		CString fname;
		CComVariant var;
		BSTR strServerName,bstrfname;	
		
		if (strIPAddress.CompareNoCase("localhost")==0||strIPAddress=="127.0.0.1")
		{
			sServerName=".";
		}
		else
		{
			sServerName=strIPAddress;
		}
		
		strQueueType.MakeLower();
		if (strQueueType=="private")
		{
			sServerName+="\\Private$\\"+strQueueName;
			
		}
		else if (strQueueType=="public")
		{
			return false;
		}	
		
		try
		{
			//FormatName:DIRECT=TCP:192.168.0.19\private$\q1
			if (sServerName.Left(1)=='.')
			{
				fname="DIRECT=OS:";
			}
			else
			{
				fname="DIRECT=TCP:";
			}
			
			fname+=sServerName;
			strServerName=sServerName.AllocSysString();
			qInfo->PathName=strServerName;
			SysFreeString(strServerName);
			
			bstrfname=fname.AllocSysString();
			qInfo->FormatName =bstrfname;    
			SysFreeString(bstrfname);		
			// Open the queue.
			qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			
			ChangeStruct2Variant(var, &Message);
			
			if (bRecoverable)//关机后不消失
			{
				qMsg->Delivery=MQMSG_DELIVERY_RECOVERABLE;  
			}
			qMsg->put_Body(var);			
			qMsg->Send(qDest);			
			qDest->Close();
			
			return true;		
		}
		catch(_com_error e)
		{		
			sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
			return false;
		}
	}

	bool MoveMsg(const CString & _sIPAddressSrc,
						const CString & _sQueueNameSrc,
						const CString & _sIPAddressDest,
						const CString & _sQueueNameDest,
						long nTimeOut=3000)
	{
		CString sServerName;

		_variant_t vtReceiveTimeout;
		vtReceiveTimeout = nTimeOut;
		_bstr_t label,body;
		BSTR bstrServerName,bstrfname;
		CString fname;
		
		CComVariant vcontent;
		
		if (_sIPAddressSrc.CompareNoCase("localhost")==0||_sIPAddressSrc=="127.0.0.1")
		{
			sServerName=".";
		}
		else
		{
			sServerName=_sIPAddressSrc;
		}
		

		sServerName+="\\Private$\\"+_sQueueNameSrc;	


		
		
		try
		{
			bstrServerName=sServerName.AllocSysString();
			
			if (sServerName.Left(1)=='.')
			{
				fname="DIRECT=OS:";
			}
			else
			{
				fname="DIRECT=TCP:";
			}
			
			fname+=sServerName;	
			bstrfname=fname.AllocSysString();
			
			qInfo->PathName=bstrServerName;
			SysFreeString(bstrServerName);		
			qInfo->FormatName = bstrfname;
			SysFreeString(bstrfname);
			qDest = qInfo->Open(MQ_RECEIVE_ACCESS,MQ_DENY_NONE);
			
			qMsg = qDest->Receive(&vtMissing,&vtMissing,&vtMissing,&vtReceiveTimeout);
			if (qMsg==NULL)
			{
				return false;
			}
			//ChangeVariant2Struct(CComVariant(qMsg->Body), msg);
			vcontent=CComVariant(qMsg->Body);
			qDest->Close();
	
		}
		catch(_com_error e)
		{
			sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
			CStringTool::WriteLog();
			return false;		
		}



		if (_sIPAddressDest.CompareNoCase("localhost")==0||_sIPAddressDest=="127.0.0.1")
		{
			sServerName=".";
		}
		else
		{
			sServerName=_sIPAddressDest;
		}
		
		sServerName+="\\Private$\\"+_sQueueNameDest;
		
		try
		{
			//FormatName:DIRECT=TCP:192.168.0.19\private$\q1
			if (sServerName.Left(1)=='.')
			{
				fname="DIRECT=OS:";
			}
			else
			{
				fname="DIRECT=TCP:";
			}
			
			fname+=sServerName;
			bstrServerName=sServerName.AllocSysString();
			qInfo->PathName=bstrServerName;
			SysFreeString(bstrServerName);
			
			bstrfname=fname.AllocSysString();
			qInfo->FormatName =bstrfname;    
			SysFreeString(bstrfname);		
			// Open the queue.
			qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			
			qMsg->Delivery=MQMSG_DELIVERY_RECOVERABLE;  

			qMsg->put_Body(vcontent);			
			qMsg->Send(qDest);			
			qDest->Close();
	
		}
		catch(_com_error e)
		{		
			sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
			return false;
		}

		return true;
	
	}

};

#endif // !defined(AFX_MSMQ_H__AE9093B7_53CA_414D_8AE5_7C94D58F7C4B__INCLUDED_)
