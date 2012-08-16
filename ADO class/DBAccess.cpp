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
	//创建 Connection 对象---------------------------	
	//m_pConnection.CreateInstance("ADODB.Connection");
	//#ifdef _DEBUG
		//if (m_pConnection == NULL)
		if (m_pConnection.CreateInstance("ADODB.Connection"))
		{
			//AfxMessageBox("Connection 对象创建失败! 请确认是否初始化了COM环境\r\n");
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
Name:	关闭连接并释放对象.
-----------------------------------------------------
Remarks: 关闭连接并释放connection对象.
==========================================================================*/
void CDBAccess::Release()
{
	if (IsOpen()) Close();
	m_pConnection.Release();
}

/*========================================================================
Name:	取得最后发生的error msg.
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
	Name:	检测连接对象是否为打开状态.
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
Name:	关闭与数据源的连接.
-----------------------------------------------------
Remarks: 使用 Close 方法可关闭 Connection 对象以便释放所有关联的系统资源. 
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
Name:	连接 SQL SERVER 数据库. 
-----------------------------------------------------
Params:		[dbsrc]:	SQL SERVER 服务器名.
[dbname]:	默认的数据库名.
[user]:		用户名.
[pass]:		密码.
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
Name:	连接 ACCESS 数据库. 
-----------------------------------------------------
Params:		[dbpath]:	MDB 数据库file路径名.
[pass]:		访问密码.
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
Name:	通过 udl file连接数据库. 
-----------------------------------------------------
Params:		[strFileName]:	UDL 数据库连接file路径名.
==========================================================================*/
BOOL CDBAccess::ConnectUseUDLFile(LPCTSTR strFileName, long lOptions)
{
	CString strConnect = _T("File Name=");
	strConnect += strFileName;	
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:		连接到数据源.
-----------------------------------------------------
Params:		[lpszConnect]: 连接字符串, 包含连接信息.
[lOptions]:	可选. 决定该方法是以同步还是异步的方式连接数据
源. 可以是如下某个常量:
[常量]					[说明] 
----------------------------------
adConnectUnspecified	(默认)同步方式打开连接. 
adAsyncConnect			异步方式打开连接. Ado用 ConnectComplete 事
件来通知已经完成连接. 
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
		// 连接数据库 ---------------------------------------------
		
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
Name:		执row指定的查询、SQL 语句、存储过程等.
----------------------------------------------------------
Remarks:	请参考 CAdoRecordSet 类的Open方法. 返回的 Recordset 对象始
终为只读、仅向前的游标.
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

