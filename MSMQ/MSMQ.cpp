// MSMQ.cpp: implementation of the CMSMQ class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "MSMQ.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSMQ::CMSMQ()
{
	qDest = NULL;
	try
	{
		qInfo.CreateInstance("MSMQ.MSMQQueueInfo");
		qMsg.CreateInstance("MSMQ.MSMQMessage");	
		
	}
	catch (_com_error)
	{
		//sprintf(CStringTool::sErrorMessage,"Create MSMQQueue failed");
		//sprintf(CStringTool::sErrorMessage,"CMSMQ contruct error :%s, ,file:%s,row %d",e.ErrorMessage(),__FILE__, __LINE__);
		CStringTool::WriteLogEx("msmq error");
		//ReleaseAll();
	}
}

CMSMQ::~CMSMQ()
{
	
}

void CMSMQ::CreateQueue(CString _sServerName,CString _sLabel)
{
	HRESULT ht;
   	try
	{
	
		CString fname;
		fname="DIRECT=OS:";
		fname+=_sServerName;
		qInfo->PathName = _sServerName.AllocSysString();
		qInfo->FormatName = fname.AllocSysString();
		qInfo->Label= _sLabel.AllocSysString();		 // Setting the Queue Label
		ht = qInfo->Create();
		qInfo->Journal=1;								 // Enabling the Journal Option
		qInfo->Update();
		qInfo->Refresh();
	}
	catch (_com_error e)
	{
		sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
	}
}

void CMSMQ::CreateQueue(CString _sIPAddress,CString _sQueueName,CString _sLabel,CString _sQueueType)
{
	CString sServerName;
	
	_sIPAddress.MakeLower();

	if (_sIPAddress=="localhost"||_sIPAddress=="127.0.0.1")
	{
		_sIPAddress=".";
	}
		
	_sQueueType.MakeLower();
	if (_sQueueType=="private")
	{
		sServerName=_sIPAddress+"\\Private$\\"+_sQueueName;
		CreateQueue(sServerName,_sLabel);
	}
	else if (_sQueueType=="public")
	{
	}		
}

CString CMSMQ::ReadMessage(CString _sServerName)
{
	
	_variant_t vtReceiveTimeout;
	vtReceiveTimeout = (long)3000;
	_bstr_t label,body;
	BSTR bstrServerName,bstrfname;
	CString fname;
	
	try
	{
		bstrServerName=_sServerName.AllocSysString();
		
		if (_sServerName.Left(1)=='.')
		{
			fname="DIRECT=OS:";
		}
		else
		{
			fname="DIRECT=TCP:";
		}
		
		fname+=_sServerName;	
		bstrfname=fname.AllocSysString();
		
		qInfo->PathName=bstrServerName;
		SysFreeString(bstrServerName);		
		qInfo->FormatName = bstrfname;
		SysFreeString(bstrfname);
		qDest = qInfo->Open(MQ_RECEIVE_ACCESS,MQ_DENY_NONE);
		
		qMsg = qDest->Receive(&vtMissing,&vtMissing,&vtMissing,&vtReceiveTimeout);
		label=qMsg->GetLabel();
		body=qMsg->GetBody().bstrVal;
		//m_msgLabel=(LPSTR)label;
		qDest->Close();		
		return (LPSTR)body;		

    }
	catch(_com_error e)
	{
		sprintf(CStringTool::sErrorMessage,"%s",e.ErrorMessage());
		return (LPSTR)NULL;
		
	}
}

CString CMSMQ::ReadMessage(const CString & _sIPAddress,const CString & _sQueueName)
{
	CString sServerName;

	if (_sIPAddress.CompareNoCase("localhost")==0||_sIPAddress=="127.0.0.1")
	{
		sServerName=".";
	}
	else
	{
		sServerName=_sIPAddress;
	}
		
	sServerName+="\\Private$\\"+_sQueueName;	

	return ReadMessage(sServerName);
	
}

void CMSMQ::ReleaseAll()
{
	 qInfo->Release();
	 if (qMsg)
	 {
		 qMsg->Release();
	 }

}

bool CMSMQ::SendMessage(CString _sServerName,CString _sMessage,bool _bRecoverable)
{
	CString fname;
	VARIANT vMessage;
	BSTR strServerName,bstrfname;	
	
	try
	{
		//FormatName:DIRECT=TCP:192.168.0.19\private$\q1
		if (_sServerName.Left(1)=='.')
		{
			fname="DIRECT=OS:";
		}
		else
		{
			fname="DIRECT=TCP:";
		}
		
		fname+=_sServerName;
		strServerName=_sServerName.AllocSysString();
		qInfo->PathName=strServerName;
		SysFreeString(strServerName);
		
		bstrfname=fname.AllocSysString();
		qInfo->FormatName =bstrfname;    
		SysFreeString(bstrfname);
		// Open the queue.
		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
		
		VariantInit(&vMessage);
		vMessage.vt=VT_BSTR;
		vMessage.bstrVal=_sMessage.AllocSysString();
		
		if (_bRecoverable)//关机后不消失
		{
			qMsg->Delivery=MQMSG_DELIVERY_RECOVERABLE;  
		}
		
		qMsg->put_Body(vMessage);		
		VariantClear(&vMessage);
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

bool CMSMQ::SendMessage(const CString & _sIPAddress,const CString & _sQueueName,CString _sMessage,CString _sQueueType,bool _bRecoverable)
{
	
	CString sServerName;
	
	if (_sIPAddress.CompareNoCase("localhost")==0||_sIPAddress=="127.0.0.1")
	{
		sServerName=".";
	}
	else
	{
		sServerName=_sIPAddress;
	}
	
	_sQueueType.MakeLower();
	if (_sQueueType=="private")
	{
		sServerName+="\\Private$\\"+_sQueueName;
		
	}
	else if (_sQueueType=="public")
	{
		
	}	
	
	return SendMessage(sServerName,_sMessage,_bRecoverable);
}
