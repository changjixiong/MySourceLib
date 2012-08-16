// DBFunction.h: interface for the CDBFunction class.
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
	
	// ���� ------------------------------------------------	
public:
	// ��ǰ�༭״̬ ----------------------------
	EditModeEnum GetEditMode();
	// ��ǰ״̬ --------------------------------
	BOOL IsEOF();
	BOOL IsBOF();
	BOOL IsOpen();
	long GetState();

	// ���Ӷ��� ----------------------------
	CDBAccess * GetConnection() {return m_pConnection;}
	void SetAdoConnection(CDBAccess * pConnection);

	// ���λ�� --------------------------------
	CursorLocationEnum GetCursorLocation();
	BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);
	
	// ������� --------------------------------
	CursorTypeEnum GetCursorType();
	BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);

	// ��ǩ --------------------------------
	_variant_t GetBookmark();

	// ��ǰ��¼λ�� ------------------------
	long GetAbsolutePosition();
	BOOL SetAbsolutePosition(int nPosition);

	// ��¼�����ֶ��� ----------------------
	long GetRecordCount();
	long GetFieldsCount();

	// ��¼������ --------------------------
	_RecordsetPtr& GetRecordset();
	
public:
	// �ֶμ� -------------------------------
	FieldsPtr GetFields();

	// �ֶ����� -----------------------------
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);
	// �ֶζ��� -----------------------------
	FieldPtr  GetField(long lIndex);
	FieldPtr  GetField(LPCTSTR lpszFieldName);

	// �ֶ��������� -------------------------
	DataTypeEnum GetFieldType(long lIndex);
	DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

	// �ֶ�ʵ�ʳ��� -------------------------
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);
	
	// ��¼���� --------------------------------------------
public:	
	BOOL AddNew();
	BOOL Update();
	BOOL CancelUpdate();
	
	// ��¼���������� --------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));

	// ����ָ���ļ�¼ ----------------------
	BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	BOOL FindNext();
	
// ��ѯ ------------------------------------------------
public:
	BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Open(const string & strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Cancel();
	//void Close();
	void CloseRecordset();
	void CloseConnect();
	bool Execute(LPCTSTR strSQL,bool bAutoCloseConnect=true, long lOptions = adCmdText);
	
	//����
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectUseUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);

	
	// �ֶδ�ȡ --------------------------------------------
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
	//��Ա���� --------------------------------------------
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

