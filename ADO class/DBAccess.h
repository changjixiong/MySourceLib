// DBAccess.h: interface for the CDBAccess class.
//
//////////////////////////////////////////////////////////////////////
/*=============================================================================
本类里面的部分内容是引用自 成真(anyou@sina.com,anyou@msn.com)封装的CAdoConnection类
其声明如下: 
在原则上，我默许您自由地使用，修改，或传播此代码，
但您要是用在某某项目中或用于商业用途，那请您注意了，
我不会对因代码bug问题，或版权问题对您造成的损失负任何责任
(http://www.vckbase.com/document/viewdoc/?id=668)

  鉴于以上声明,我在这里合法引用其部分内容   常纪雄(Jason.Chang)
  
==============================================================================*/

#if !defined(AFX_DBACCESS_H__0EBA0FB3_C1AC_40B9_B85E_FA5E496AD65D__INCLUDED_)
#define AFX_DBACCESS_H__0EBA0FB3_C1AC_40B9_B85E_FA5E496AD65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <io.h>
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
using namespace ADODB; 


class CDBAccess  
{
public:
	CDBAccess();
	virtual ~CDBAccess();
	
protected:
	void Release();
	
	// 属性 -----------------------------------------------
public:
	// 连接对象 ----------------------------------
	_ConnectionPtr& GetConnection() {return m_pConnection;};
	// 异常信息 ----------------------------------
	CString GetLastErrorText();
	// 连接状态 ----------------------------------
	BOOL IsOpen();
	
	// 操作 -----------------------------------------------


public:
	// 数据库连接 --------------------------------
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectUseUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	void Close();

	void SetTimeOut(long nTimeOut)
	{
		m_pConnection->CommandTimeout=nTimeOut;
	}	
	
	
	// 数据 -----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
	
	// 处理 -----------------------------------------------
public:
	// 执行 SQL 语句 ------------------------------
	_RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
	
	
};

#endif // !defined(AFX_DBACCESS_H__0EBA0FB3_C1AC_40B9_B85E_FA5E496AD65D__INCLUDED_)
