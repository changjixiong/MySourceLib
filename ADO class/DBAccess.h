// DBAccess.h: interface for the CDBAccess class.
//
//////////////////////////////////////////////////////////////////////
/*=============================================================================
��������Ĳ��������������� ����(anyou@sina.com,anyou@msn.com)��װ��CAdoConnection��
����������: 
��ԭ���ϣ���Ĭ�������ɵ�ʹ�ã��޸ģ��򴫲��˴��룬
����Ҫ������ĳĳ��Ŀ�л�������ҵ��;��������ע���ˣ�
�Ҳ���������bug���⣬���Ȩ���������ɵ���ʧ���κ�����
(http://www.vckbase.com/document/viewdoc/?id=668)

  ������������,��������Ϸ������䲿������   ������(Jason.Chang)
  
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
	
	// ���� -----------------------------------------------
public:
	// ���Ӷ��� ----------------------------------
	_ConnectionPtr& GetConnection() {return m_pConnection;};
	// �쳣��Ϣ ----------------------------------
	CString GetLastErrorText();
	// ����״̬ ----------------------------------
	BOOL IsOpen();
	
	// ���� -----------------------------------------------


public:
	// ���ݿ����� --------------------------------
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectUseUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	void Close();

	void SetTimeOut(long nTimeOut)
	{
		m_pConnection->CommandTimeout=nTimeOut;
	}	
	
	
	// ���� -----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
	
	// ���� -----------------------------------------------
public:
	// ִ�� SQL ��� ------------------------------
	_RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
	
	
};

#endif // !defined(AFX_DBACCESS_H__0EBA0FB3_C1AC_40B9_B85E_FA5E496AD65D__INCLUDED_)
