// DBAccess.cpp: implementation of the CDBAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBAccess.h"
#include "StringTool.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBAccess::CDBAccess()
{
	//���� Connection ����---------------------------	
	//m_pConnection.CreateInstance("ADODB.Connection");
	//#ifdef _DEBUG
		//if (m_pConnection == NULL)
		if (m_pConnection.CreateInstance("ADODB.Connection"))
		{
			//AfxMessageBox("Connection ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����COM����\r\n");
			CStringTool::WriteLogEx("Connection create failed! please make sure that the COM environment inited");
		}
	//#endif
	ASSERT(m_pConnection != NULL);
}

CDBAccess::~CDBAccess()
{
	if (m_pConnection != NULL)
	{
		Release();
	}
}


/*========================================================================
Name:	�ر����Ӳ��ͷŶ���.
-----------------------------------------------------
Remarks: �ر����Ӳ��ͷ�connection����.
==========================================================================*/
void CDBAccess::Release()
{
	if (IsOpen()) Close();
	m_pConnection.Release();
}

/*========================================================================
Name:	ȡ���������error msg.
==========================================================================*/
CString CDBAccess::GetLastErrorText()
{
	ASSERT(m_pConnection != NULL);
	CString strErrors = "";
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors = m_pConnection->Errors;
			CString strError;
			for (long n = 0; n < pErrors->Count; n++)
			{
				ErrorPtr pError = pErrors->GetItem(n);
				strError.Format(_T("Description: %s\r\nState: %s, Native: %d, Source: %s\r\n"),
					(LPCTSTR)pError->Description,
					(LPCTSTR)pError->SQLState,
					pError->NativeError,
					(LPCTSTR)pError->Source);
				strErrors += strError;
			}
		}
		return strErrors;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetLastError function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return strErrors;
	} 
	return strErrors;
}

/*========================================================================
	Name:	������Ӷ����Ƿ�Ϊ��״̬.
==========================================================================*/
BOOL CDBAccess::IsOpen()
{
	try
	{
		return (m_pConnection != NULL && (m_pConnection->State & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsOpen function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:	�ر�������Դ������.
-----------------------------------------------------
Remarks: ʹ�� Close �����ɹر� Connection �����Ա��ͷ����й�����ϵͳ��Դ. 
==========================================================================*/
void CDBAccess::Close()
{
	try
	{
		if (m_pConnection != NULL && IsOpen()) 
		{
			m_pConnection->Close();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: close database error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
}

/*========================================================================
Name:	���� SQL SERVER ���ݿ�. 
-----------------------------------------------------
Params:		[dbsrc]:	SQL SERVER ��������.
[dbname]:	Ĭ�ϵ����ݿ���.
[user]:		�û���.
[pass]:		����.
==========================================================================*/
BOOL CDBAccess::ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=SQLOLEDB.1; Data Source=") + dbsrc + 
		_T("; Initial Catalog=") + dbname  +
		_T("; User ID=") + user + 
		_T("; PWD=") + pass;
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	���� ACCESS ���ݿ�. 
-----------------------------------------------------
Params:		[dbpath]:	MDB ���ݿ�file·����.
[pass]:		��������.
===========================================================================*/
BOOL CDBAccess::ConnectAccess(CString dbpath, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source=") + dbpath;
	if (pass != _T("")) 
	{
		strConnect += _T("Jet OLEDB:Database Password=") + pass + _T(";");
	}
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	ͨ�� udl file�������ݿ�. 
-----------------------------------------------------
Params:		[strFileName]:	UDL ���ݿ�����file·����.
==========================================================================*/
BOOL CDBAccess::ConnectUseUDLFile(LPCTSTR strFileName, long lOptions)
{
	CString strConnect = _T("File Name=");
	strConnect += strFileName;	
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:		���ӵ�����Դ.
-----------------------------------------------------
Params:		[lpszConnect]: �����ַ���, ����������Ϣ.
[lOptions]:	��ѡ. �����÷�������ͬ�������첽�ķ�ʽ��������
Դ. ����������ĳ������:
[����]					[˵��] 
----------------------------------
adConnectUnspecified	(Ĭ��)ͬ����ʽ������. 
adAsyncConnect			�첽��ʽ������. Ado�� ConnectComplete ��
����֪ͨ�Ѿ��������. 
==========================================================================*/
BOOL CDBAccess::Open(LPCTSTR lpszConnect, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszConnect));
	
	if (strcmp(lpszConnect, _T("")) != 0)
	{
		m_strConnect = lpszConnect;
	}
	
	if (m_strConnect.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if (IsOpen()) Close();	
	try
	{
		// �������ݿ� ---------------------------------------------
		
		return (m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnect)), "", "", lOptions) == S_OK);
		
	}
	catch (_com_error e)
	{
		sprintf(CStringTool::sErrorMessage,"connect error:%s, connect string :%s,file:%s,row %d",e.ErrorMessage(),lpszConnect,__FILE__, __LINE__);		
		CStringTool::WriteLogEx(LPCTSTR(CStringTool::sErrorMessage));
		CStringTool::WriteLogEx(LPCTSTR(m_strConnect));
		TRACE(_T("Warning: link to database error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		//TRACE(_T("%s\r\n"), GetLastErrorText());
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T("Warning: unknow error occur during link to database"));
		CStringTool::WriteLogEx("Warning: unknow error occur during link to database");
	}
	return FALSE;
}

/*========================================================================
Name:		ִrowָ���Ĳ�ѯ��SQL ��䡢�洢���̵�.
----------------------------------------------------------
Remarks:	��ο� CAdoRecordSet ���Open����. ���ص� Recordset ����ʼ
��Ϊֻ��������ǰ���α�.
==========================================================================*/
_RecordsetPtr CDBAccess::Execute(LPCTSTR lpszSQL, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszSQL));
	
	try
	{
		return m_pConnection->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	catch (_com_error e)
	{
		sprintf(CStringTool::sErrorMessage,"Execute error:%s, Execute string :%s,file:s%,row %d",e.ErrorMessage(),lpszSQL,__FILE__, __LINE__);
		CStringTool::WriteLogEx(CStringTool::sErrorMessage);
		TRACE(_T("Warning: Execute function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

