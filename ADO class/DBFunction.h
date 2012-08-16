// DBFunction.h: interface for the CDBFunction class.
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
#if !defined(AFX_DBFUNCTION_H__3756CEB2_4009_4B26_9680_BFAAEEA3071C__INCLUDED_)
#define AFX_DBFUNCTION_H__3756CEB2_4009_4B26_9680_BFAAEEA3071C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "DBAccess.h"

using namespace std;

class CDBFunction  
{
public:
	CDBFunction();
	CDBFunction(CDBAccess * pCdbaccess);
	virtual ~CDBFunction();
	
protected:
	void Release();
	
	// 属性 ------------------------------------------------	
public:
	// 当前编辑状态 ----------------------------
	EditModeEnum GetEditMode();
	// 当前状态 --------------------------------
	BOOL IsEOF();
	BOOL IsBOF();
	BOOL IsOpen();
	long GetState();

	// 连接对象 ----------------------------
	CDBAccess * GetConnection() {return m_pConnection;}
	void SetAdoConnection(CDBAccess * pConnection);

	// 光标位置 --------------------------------
	CursorLocationEnum GetCursorLocation();
	BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);
	
	// 光标类型 --------------------------------
	CursorTypeEnum GetCursorType();
	BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);

	// 书签 --------------------------------
	_variant_t GetBookmark();

	// 当前记录位置 ------------------------
	long GetAbsolutePosition();
	BOOL SetAbsolutePosition(int nPosition);

	// 记录数及字段数 ----------------------
	long GetRecordCount();
	long GetFieldsCount();

	// 记录集对象 --------------------------
	_RecordsetPtr& GetRecordset();
	
public:
	// 字段集 -------------------------------
	FieldsPtr GetFields();

	// 字段属性 -----------------------------
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);
	// 字段对象 -----------------------------
	FieldPtr  GetField(long lIndex);
	FieldPtr  GetField(LPCTSTR lpszFieldName);

	// 字段数据类型 -------------------------
	DataTypeEnum GetFieldType(long lIndex);
	DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

	// 字段实际长度 -------------------------
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);
	
	// 记录更改 --------------------------------------------
public:	
	BOOL AddNew();
	BOOL Update();
	BOOL CancelUpdate();
	
	// 记录集导航操作 --------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));

	// 查找指定的记录 ----------------------
	BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	BOOL FindNext();
	
// 查询 ------------------------------------------------
public:
	BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Open(const string & strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Cancel();
	//void Close();
	void CloseRecordset();
	void CloseConnect();
	bool Execute(LPCTSTR strSQL,bool bAutoCloseConnect=true, long lOptions = adCmdText);
	
	//连接
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectUseUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);

	
	// 字段存取 --------------------------------------------
public:
	BOOL GetCollect(long index, CString &strReturnValue);
	BOOL GetCollect(LPCSTR strFieldName, CString &strReturnValue);
	BOOL GetCollect(long index, string &strReturnValue);
	BOOL GetCollect(LPCSTR strFieldName, string &strReturnValue);
	BOOL GetCollect(LPCSTR strFieldName,  long &lReturnValue);

	BOOL PutCollect(long index, const _variant_t &value);
	BOOL PutCollect(long index, const long   &value);	
	BOOL PutCollect(long index, const CString &value);
	BOOL PutCollect(long index, const string &value);
	BOOL PutCollect(LPCTSTR strFieldName, const _variant_t &vValue);
	BOOL PutCollect(LPCTSTR strFieldName, const long   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const CString &vValue);
	BOOL PutCollect(LPCTSTR strFieldName, const string &vValue);
	BOOL PutCollect(LPCTSTR strFieldName, LPCTSTR vValue);
	

	BOOL GetChunk(FieldPtr pField, FILE * const fp);
	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL GetChunk(long index, LPVOID lpData);
	BOOL GetChunk(LPCSTR strFieldName, LPVOID lpData);
	BOOL GetChunk(LPCSTR strFieldName, FILE * const fp);

	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, long nBytes);
	BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, long nBytes);
	

	
protected:
	//成员变量 --------------------------------------------
	CDBAccess     * m_pConnection;

	CString				m_strSQL;
	CString				m_sConnectString;
	CString				m_strFind;
	
	_RecordsetPtr		m_pRecordset;
	_CommandPtr			m_pCommand;
	SearchDirectionEnum m_SearchDirection;
	_variant_t			m_varBookmark;
	

};

#endif // !defined(AFX_DBFUNCTION_H__3756CEB2_4009_4B26_9680_BFAAEEA3071C__INCLUDED_)

