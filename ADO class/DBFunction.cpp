// DBFunction.cpp: implementation of the CDBFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFunction.h"
#include "StringTool.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBFunction::CDBFunction()
{
	m_pConnection = NULL;
	m_SearchDirection = adSearchForward;
	m_pRecordset.CreateInstance("ADODB.Recordset");
		if (m_pRecordset == NULL)
		{
			AfxMessageBox("RecordSet ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����COM����.");
			CStringTool::WriteLogEx("RecordSet create failed! please make sure the com environment is inited.");
		}
	m_pCommand.CreateInstance("ADODB.Command");

	if (m_pCommand == NULL)
	{
		CStringTool::WriteLogEx("Command create failed! please make sure the com environment is inited.");
	}

	ASSERT(m_pCommand != NULL);
	ASSERT(m_pRecordset != NULL);
}

CDBFunction::CDBFunction(CDBAccess * pCdbaccess)
{
	m_SearchDirection = adSearchForward;
	m_pConnection = pCdbaccess;
	ASSERT(m_pConnection != NULL);
	m_pRecordset.CreateInstance("ADODB.Recordset");
	#ifdef _DEBUG
		if (m_pRecordset == NULL)
		{
			AfxMessageBox("RecordSet create failed! please make sure the com environment is inited.");
		}
	#endif
	ASSERT(m_pRecordset != NULL);
}

CDBFunction::~CDBFunction()
{
	Release();
}

void CDBFunction::SetAdoConnection(CDBAccess *pConnection)
{
	m_pConnection = pConnection;
}

/*========================================================================
Name:	�ر����Ӳ��ͷŶ���.
-----------------------------------------------------
Remarks: �ر����Ӳ��ͷ�CAdoRecordSet����, ���������ϴ���������ȫ�����
CAdoRecordSet����.
==========================================================================*/
void CDBFunction::Release()
{
	if (IsOpen()) CloseRecordset();
	m_pRecordset.Release();
	m_pRecordset = NULL;
}

EditModeEnum CDBFunction::GetEditMode()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return m_pRecordset->GetEditMode();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: UpdateBatch function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEditNone;
	} 
	return	adEditNone; 
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��ͷ
==========================================================================*/
BOOL CDBFunction::IsBOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoBOF;
	}
	catch(_com_error e)
	{
		TRACE(_T("Warning: IsBOF function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��β
==========================================================================*/
BOOL CDBFunction::IsEOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoEOF;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsEOF function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
Name:		ȡ�ü�¼�������״̬(�Ǵ�״̬���ǹر�״̬). ���첽��ʽִ
row�� Recordset ����, ��˵����ǰ�Ķ���״̬�����ӡ�ִrow���ǻ�ȡ״̬.
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
[����]				[˵��] 
----------------------------------
adStateClosed		ָʾ�����ǹرյ�. 
adStateOpen			ָʾ�����Ǵ򿪵�. 
adStateConnecting	ָʾ Recordset ������������. 
adStateExecuting	ָʾ Recordset ��������ִrow����. 
adStateFetching		ָʾ Recordset �����row���ڱ���ȡ. 
-----------------------------------------------------
Remarks:	 ������ʱʹ�� State ����ȷ��ָ������ĵ�ǰ״̬. ��������ֻ
����. Recordset ����� State ���Կ��������ֵ. ����: �������ִrow���,
�����Խ��� adStateOpen �� adStateExecuting �����ֵ.
==========================================================================*/
long CDBFunction::GetState()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

BOOL CDBFunction::IsOpen()
{
	try
	{
		return (m_pRecordset != NULL && (GetState() & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsOpen function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}


/*########################################################################
------------------------------------------------
��¼�����²���
------------------------------------------------
########################################################################*/

/*========================================================================
Remarks:	��ʼ����µļ�¼. 
==========================================================================*/
BOOL CDBFunction::AddNew()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			if (m_pRecordset->AddNew() == S_OK)
			{
				return TRUE;
			}
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: AddNew function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		sprintf(CStringTool::sErrorMessage,"Warning: AddNew function error. error msg: %s; file: %s; row: %d\n", e.ErrorMessage(), __FILE__, __LINE__);
		CStringTool::WriteLog();
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
Remarks:	�ڵ��� AddNew �ȷ�����, ���ô˷�����ɸ��»��޸�.
==========================================================================*/
BOOL CDBFunction::Update()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			if (m_pRecordset->Update() == S_OK)
			{
				return TRUE;
			}
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Update function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	
	CancelUpdate();
	return	FALSE;
}

/*========================================================================
Name:		ȡ��ִrow������첽 Execute �� Open �����ĵ���.
-----------------------------------------------------
Remarks:	ʹ�� Cancel ������ִֹrow�첽 Execute �� Open ��������(��ͨ
�� adAsyncConnect��adAsyncExecute �� adAsyncFetch �������õķ���).
�������ͼ��ֹ�ķ�����û��ʹ�� adAsyncExecute, �� Cancel ��������row
ʱ����.
==========================================================================*/
BOOL CDBFunction::Cancel()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Cancel();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Cancel error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		�رմ򿪵Ķ����κ���ض���.
-----------------------------------------------------
Remarks:	ʹ�� Close �����ɹر� Recordset �����Ա��ͷ����й�����ϵͳ
��Դ. �رն��󲢷ǽ������ڴ���ɾ��, ���Ը��������������ò����ڴ˺�
�ٴδ�. Ҫ��������ڴ�����ȫɾ��, �ɽ������������Ϊ NULL.
���������������ģʽ�½�row�༭, ����Close��������������,Ӧ����
���� Update �� CancelUpdat ����. ������������ڼ�ر� Recordset ��
��, �����ϴ� UpdateBatch ���������������޸Ľ�ȫ����ʧ.
���ʹ�� Clone ���������Ѵ򿪵� Recordset ����ĸ���, �ر�ԭʼ
Recordset���丱������Ӱ���κ���������.
==========================================================================*/
void CDBFunction::CloseRecordset()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			if (GetEditMode() == adEditNone) CancelUpdate();
			m_pRecordset->Close();
		}
	}
	catch (const _com_error& e)
	{
		TRACE(_T("Warning: close Record set error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
}

/*========================================================================
Name:		ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���.
-----------------------------------------------------
Remarks:	ʹ�� CancelUpdate ������ȡ���Ե�ǰ��¼�������κθ��Ļ����
����ӵļ�¼. �ڵ��� Update �������޷������Ե�ǰ��¼���¼�¼�����ĸ�
��, ���Ǹ����ǿ����� RollbackTrans �����ؾ�������һ����, �����ǿ���
�� CancelBatch ����ȡ���������µ�һ����.
����ڵ��� CancelUpdate ����ʱ����¼�¼, ����� AddNew ֮ǰ�ĵ�ǰ
��¼���ٴγ�Ϊ��ǰ��¼.
�����δ���ĵ�ǰ��¼������¼�¼, ���� CancelUpdate ��������������.
==========================================================================*/
BOOL CDBFunction::CancelUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			if (GetEditMode() == adEditNone || m_pRecordset->CancelUpdate() == S_OK)
			{
				return TRUE;
			}
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CancelUpdate function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}


/*========================================================================
Params:		
- strSQL:		SQL���, ����, �洢���̵��û�־� Recordset file��.
- CursorType:   ��ѡ. CursorTypeEnum ֵ, ȷ���� Recordset ʱӦ��
ʹ�õ��α�����. ��Ϊ���г���֮һ.
[����]				[˵��] 
-----------------------------------------------
adOpenForwardOnly	�򿪽���ǰ�����α�. 
adOpenKeyset		�򿪼��������α�. 
adOpenDynamic		�򿪶�̬�����α�. 
adOpenStatic		�򿪾�̬�����α�. 
-----------------------------------------------
- LockType:     ��ѡ, ȷ���� Recordset ʱӦ��ʹ�õ���������(����)
�� LockTypeEnum ֵ, ��Ϊ���г���֮һ.
[����]				[˵��] 
-----------------------------------------------
adLockReadOnly		ֻ�� - ���ܸı�����. 
adLockPessimistic	����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼. 
adLockOptimistic	����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼. 
adLockBatchOptimistic ����ʽ������ - ����������ģʽ(����������ģʽ
���). 
-----------------------------------------------
- lOption		��ѡ. ������ֵ, ����ָʾ strSQL ����������. ��Ϊ��
�г���֮һ.
[����]				[˵��] 
-------------------------------------------------
adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
adCmdTable			ָʾADO����SQL��ѯ������ strSQL �����ı��е�
����row. 
adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з�������row. 
adCmdStoredProc		ָʾstrSQLΪ�洢����. 
adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
adCmdFile			ָʾӦ����strSQL��������file�лָ�����(�����)
Recordset. 
adAsyncExecute		ָʾӦ�첽ִrowstrSQL. 
adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
������, Ӧ���첽��ȡ����ʣ���row. ��������row��δ
��ȡ, ��Ҫ���߳̽�������ֱ��row���¿���. 
adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
��row��δ��ȡ, ��ǰrow�Զ��Ƶ�fileĩβ. 
==========================================================================*/
BOOL CDBFunction::Open(LPCTSTR strSQL, long lOption, CursorTypeEnum CursorType, LockTypeEnum LockType)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(m_pRecordset != NULL);
	ASSERT(AfxIsValidString(strSQL));
	
	if(strcmp(strSQL, _T("")) != 0)
	{
		m_strSQL = strSQL;
	}
	if (m_pConnection == NULL || m_pRecordset == NULL)
	{
		return FALSE;
	}
	
	if (m_strSQL.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	try
	{
		if (IsOpen()) CloseRecordset();

		#ifdef Show_sql_open_string
				CStringTool::WriteLogEx((LPCTSTR)m_strSQL);
		#endif
		
		return SUCCEEDED(m_pRecordset->Open(_variant_t(LPCTSTR(m_strSQL)), 
			_variant_t((IDispatch*)m_pConnection->GetConnection(), true),
			CursorType, LockType, lOption));
			
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: open RecordSet error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		//TRACE(_T("%s\r\n"), GetLastError());
		return FALSE;
	}
}


BOOL CDBFunction::Open(const string & strSQL, long lOption, CursorTypeEnum CursorType, LockTypeEnum LockType)
{
	return Open(strSQL.c_str(),lOption,CursorType,LockType);
}

/*========================================================================
Name:		��ȡ��ǰ��¼���м�¼��Ŀ
==========================================================================*/
long CDBFunction::GetRecordCount()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		long count = m_pRecordset->GetRecordCount();
		
		// ���ado��֧�ִ����ԣ����ֹ������¼��Ŀ --------
		if (count < 0)
		{
			long pos = GetAbsolutePosition();
			MoveFirst();
			count = 0;
			while (!IsEOF()) 
			{
				count++;
				MoveNext();
			}
			SetAbsolutePosition(pos);
		}
		return count;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetRecordCount function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
}

/*========================================================================
Name:		��ȡ��ǰ��¼�����ֶ���Ŀ
==========================================================================*/
long CDBFunction::GetFieldsCount()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return GetFields()->Count;
	}
	catch(_com_error e)
	{
		TRACE(_T("Warning: GetFieldsCount function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
}

/*========================================================================
Remarks:	Recordset ������� Field ������ɵ� Fields ����. ÿ��Field
�����Ӧ Recordset ���е�һ��.
==========================================================================*/
FieldsPtr CDBFunction::GetFields()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetFields();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFields function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}


_RecordsetPtr& CDBFunction::GetRecordset()
{
	return m_pRecordset;
}

/*========================================================================
name:		ָ�� Recordset ����ǰ��¼�����λ��. 
----------------------------------------------------------
returns:	���û򷵻ش� 1 �� Recordset ���� (PageCount) ����ҳ���ĳ���
��ֵ�����߷������³���. 
[����]			[˵��]
---------------------------------
adPosUnknown	Recordset Ϊ�գ���ǰλ��δ֪�������ṩ�߲�֧�� AbsolutePage ����.  
adPosBOF		��ǰ��¼ָ��λ�� BOF(�� BOF ����Ϊ True).  
adPosEOF		��ǰ��¼ָ��λ�� EOF(�� EOF ����Ϊ True).  
----------------------------------------------------------
Remarks:		ʹ�� AbsolutePosition ���Կɸ������� Recordset �е����
λ���ƶ�����¼����ȷ����ǰ��¼�����λ��. �ṩ�߱���֧�ָ����Ե���Ӧ��
�ܲ���ʹ�ø�����. 
ͬ AbsolutePage ����һ����AbsolutePosition �� 1 ��ʼ�����ڵ�ǰ��¼
Ϊ Recordset �еĵ�һ����¼ʱ���� 1. �� RecordCount ���Կɻ�� Recordset 
������ܼ�¼��. 
���� AbsolutePosition ����ʱ����ʹ������ָ��λ�ڵ�ǰ�����еļ�¼��
ADO Ҳ��ʹ����ָ���ļ�¼��ʼ���¼�¼�����¼��ػ���. CacheSize ���Ծ���
�ü�¼��Ĵ�С. 
ע��   ���ܽ� AbsolutePosition ������Ϊ����ļ�¼���ʹ��. ɾ��ǰ��
�ļ�¼ʱ��������¼�ĵ�ǰλ�ý������ı�. ��� Recordset �������²�ѯ��
���´򿪣����޷���֤������¼����ͬ�� AbsolutePosition. ��ǩ��Ȼ�Ǳ��ֺ�
���ظ���λ�õ��Ƽ���ʽ���������������͵� Recordset ����Ķ�λʱ��Ψһ��
��ʽ. 
==========================================================================*/
BOOL CDBFunction::SetAbsolutePosition(int nPosition)
{
	ASSERT(m_pRecordset != NULL);
	
	try
	{
		m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);		
		return TRUE;
	}
	catch(_com_error &e)
	{
		TRACE(_T("Warning: SetAbsolutePosition function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

long CDBFunction::GetAbsolutePosition()
{
	ASSERT(m_pRecordset != NULL);
	
	try
	{
		return m_pRecordset->GetAbsolutePosition();
	}
	catch(_com_error &e)
	{
		TRACE(_T("Warning: GetAbsolutePosition function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

BOOL CDBFunction::SetCursorLocation(CursorLocationEnum CursorLocation)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCursorLocation(CursorLocation);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: PutCursorLocation function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

CursorLocationEnum CDBFunction::GetCursorLocation()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetCursorLocation();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetCursorLocation function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adUseNone;
	}
}

/*########################################################################
------------------------------------------------
��ȡ�ֶε�ֵ
------------------------------------------------
  ########################################################################*/

BOOL CDBFunction::GetCollect(long index, CString& strReturnValue)
{
	ASSERT(m_pRecordset != NULL);
	//if (index < 0 || index >= GetFieldsCount())
	//{
	//	return FALSE;
	//}

	try
	{
		if (!IsOpen())
		{
			MessageBox(NULL, _T("database may be disconnect ,\r\n please reconnect and retry."), _T("note"), MB_ICONINFORMATION);
			return FALSE;
		} 
		if (m_pRecordset->adoEOF)
		{
			return FALSE;
		}
		_variant_t value = m_pRecordset->GetCollect(_variant_t(index));
		CStringTool::vartostr(strReturnValue,value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: cell get faild. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	
	return FALSE;
}

BOOL CDBFunction::GetCollect(long index, string& strReturnValue)
{
	CString strTemp;
	if (GetCollect(index,strTemp))
	{
		strReturnValue=LPCSTR(strTemp);
		return TRUE;
	}

	return false;
}

BOOL CDBFunction::GetCollect(LPCSTR strFieldName, CString &strReturnValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (!IsOpen())
		{
			MessageBox(NULL, _T("database may be disconnect ,\r\n please reconnect and retry."), _T("note"), MB_ICONINFORMATION);
			return FALSE;
		} 
		if (m_pRecordset->adoEOF)
		{
			return FALSE;
		}
		_variant_t value = m_pRecordset->GetCollect(_variant_t(LPCTSTR(strFieldName)));
		CStringTool::vartostr(strReturnValue,value);
		return TRUE;	
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: cell get faild. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	
	return FALSE;
}
BOOL CDBFunction::GetCollect(LPCSTR strFieldName, string &strReturnValue)
{
	CString strTemp;
	strReturnValue="";
	if (GetCollect(strFieldName,strTemp))
	{
		strReturnValue=LPCSTR(strTemp);
		return TRUE;
	}
	return FALSE;
}

BOOL CDBFunction::GetCollect(LPCSTR strFieldName,  long &lReturnValue)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		_variant_t value = m_pRecordset->GetCollect(_variant_t(strFieldName));
		CStringTool::vartol(lReturnValue,value);
		return TRUE;
	}
	catch (_com_error e)
	{
		lReturnValue = 0;
		return FALSE;
	} 	
	return FALSE;
}

BOOL CDBFunction::GetChunk(FieldPtr pField, LPVOID lpData)
{
	ASSERT(pField != NULL);
	ASSERT(lpData != NULL);
	
	UCHAR chData;
	long index = 0;
	long Len=pField->ActualSize;
	while (index < Len)
	{ 
		try
		{
			_variant_t varChunk = pField->GetChunk(100);
			if (varChunk.vt != (VT_ARRAY | VT_UI1))
			{
				return FALSE;
			}
			
            for (long i = 0; i < 100; i++)
            {
                if (SUCCEEDED( SafeArrayGetElement(varChunk.parray, &i, &chData) ))
                {
					((UCHAR*)lpData)[index] = chData;
                    index++;
                }
                else
				{
                    break;
				}
            }
		}
		catch (_com_error e)
		{
			TRACE(_T("Warning: GetChunk function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CDBFunction::GetChunk(FieldPtr pField, FILE * const fp)
{
	ASSERT(pField != NULL);
	ASSERT(fp != NULL);
	
	UCHAR chData;
	long index = 0;
	long Len=pField->ActualSize;
	UCHAR * lpContent=new UCHAR[Len];
	
	while (index < Len)
	{ 
		try
		{
			_variant_t varChunk = pField->GetChunk(100);
			if (varChunk.vt != (VT_ARRAY | VT_UI1))
			{
				return FALSE;
			}
			
            for (long i = 0; i < 100; i++)
            {
                if (SUCCEEDED( SafeArrayGetElement(varChunk.parray, &i, &chData) ))
                {
					((UCHAR*)lpContent)[index] = chData;
                    index++;
                }
                else
				{
                    break;
				}
            }
		}
		catch (_com_error e)
		{
			TRACE(_T("Warning: GetChunk function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			delete [] lpContent;
			lpContent=NULL;
			return FALSE;
		}
	}
	fwrite(lpContent,1,Len,fp);
	delete [] lpContent;
	lpContent=NULL;
	return TRUE;
}

BOOL CDBFunction::GetChunk(long index, LPVOID lpData)
{
	if (adFldLong & GetFieldAttributes(index))
		return  GetChunk(GetField(index), lpData);
	else return FALSE;
}

BOOL CDBFunction::GetChunk(LPCSTR strFieldName, LPVOID lpData)
{
	if (adFldLong & GetFieldAttributes(strFieldName))
		return  GetChunk(GetField(strFieldName), lpData);
	else return FALSE;
}

BOOL CDBFunction::GetChunk(LPCSTR strFieldName, FILE * const fp)
{
	if (adFldLong & GetFieldAttributes(strFieldName))
		return  GetChunk(GetField(strFieldName), fp);
	else return FALSE;
}

/*########################################################################
			  ------------------------------------------------
								��¼����������
			  ------------------------------------------------
  ########################################################################*/

/*========================================================================
	Name:		����ǰ��¼λ���ƶ��� Recordse �еĵ�һ����¼.
==========================================================================*/
BOOL CDBFunction::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveFirst function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
	Name:		����ǰ��¼λ���ƶ��� Recordset �е����һ����¼.
	-----------------------------------------------------
	Remarks:	Recordset �������֧����ǩ��������ƶ�; ������ø÷�����
			��������.
==========================================================================*/
BOOL CDBFunction::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveLast function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
	Name:		����ǰ��¼λ������ƶ�һ����¼(���¼���Ķ���).
	-----------------------------------------------------
	Remarks:	Recordset �������֧����ǩ������α��ƶ�; ���򷽷����ý���
	������.����׼�¼�ǵ�ǰ��¼���ҵ��� MovePrevious ����, �� ADO ����ǰ��
	¼������ Recordset (BOFΪTrue) ���׼�¼֮ǰ. ��BOF����Ϊ True ʱ�����
	������������. ��� Recordse ����֧����ǩ������α��ƶ�, �� MovePrevious 
	��������������.
==========================================================================*/
BOOL CDBFunction::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MovePrevious function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;	
}

/*========================================================================
	Name:		����ǰ��¼��ǰ�ƶ�һ����¼(�� Recordset �ĵײ�).
	-----------------------------------------------------
	Remarks:	������һ����¼�ǵ�ǰ��¼���ҵ��� MoveNext ����, �� ADO ��
	��ǰ��¼���õ� Recordset (EOFΪ True)��β��¼֮��. �� EOF �����Ѿ�Ϊ 
	True ʱ��ͼ��ǰ�ƶ�����������.
==========================================================================*/
BOOL CDBFunction::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveNext function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;
}

/*========================================================================
	Name:		�ƶ� Recordset �����е�ǰ��¼��λ��.
    ----------------------------------------------------------
	Params:		
		- lRecords    �����ų����ͱ��ʽ, ָ����ǰ��¼λ���ƶ��ļ�¼��.
		- Start    ��ѡ, �ַ����������, ���ڼ�����ǩ. Ҳ��Ϊ���� 
		BookmarkEnum ֵ֮һ: 
		[����]				[˵��] 
		--------------------------------
		adBookmarkCurrent	Ĭ��. �ӵ�ǰ��¼��ʼ. 
		adBookmarkFirst		���׼�¼��ʼ. 
		adBookmarkLast		��β��¼��ʼ. 
==========================================================================*/
BOOL CDBFunction::Move(long lRecords, _variant_t Start)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Move(lRecords, _variant_t(Start)));
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Move function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}


/*========================================================================
name:		���� Recordset ������ָ����׼�ļ�¼. ��������׼�����¼��
λ���������ҵ��ļ�¼�ϣ�����λ�ý������ڼ�¼����ĩβ. 
----------------------------------------------------------
params:		[criteria]:   �ַ���������ָ�������������������Ƚϲ�������
ֵ�����. 
[searchDirection]:    ��ѡ�� SearchDirectionEnum ֵ��ָ����
��Ӧ�ӵ�ǰrow������һ����Чrow��ʼ. ��ֵ��Ϊ adSearchForward �� adSearchBackward. 
�������ڼ�¼���Ŀ�ʼ����ĩβ������ searchDirection ֵ����. 
[����]			[˵��]
---------------------------------
adSearchForward 	
adSearchBackward	
----------------------------------------------------------
Remarks:	criteria �е�"�Ƚϲ�����"������">"(����)��"<"(С��)��"="(��
��)��">="(���ڻ����)��"<="(С�ڻ����)��"<>"(������)��"like"(ģʽƥ��).  
criteria �е�ֵ�������ַ�������������������. �ַ���ֵ�Ե����ŷֽ�(��
"state = 'WA'"). ����ֵ��"#"(���ּǺ�)�ֽ�(��"start_date > #7/22/97#"). 
��"�Ƚϲ�����"Ϊ"like"�����ַ���"ֵ"���԰���"*"(ĳ�ַ��ɳ���һ�λ�
���)����"_"(ĳ�ַ�ֻ����һ��). (��"state like M_*"�� Maine �� Massachusetts 
ƥ��.). 
==========================================================================*/
BOOL CDBFunction::Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(AfxIsValidString(lpszFind));
	
	try
	{
		if (strcmp(lpszFind, _T("")) != 0)
		{
			m_strFind = lpszFind;
		}
		
		if (m_strFind.IsEmpty()) return FALSE;
		
		m_pRecordset->Find(_bstr_t(m_strFind), 0, SearchDirection, "");
		
		if ((IsEOF() || IsBOF()) )
		{
			return FALSE;
		}
		else
		{
			m_SearchDirection = SearchDirection;
			GetBookmark();
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Find function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
name:		������һ�����������ļ�¼.
==========================================================================*/
BOOL CDBFunction::FindNext()
{
	ASSERT(m_pRecordset != NULL);
	
	try
	{
		if (m_strFind.IsEmpty()) return FALSE;
		
		m_pRecordset->Find(_bstr_t(m_strFind), 1, m_SearchDirection, m_varBookmark);
		
		if ((IsEOF() || IsBOF()) )
		{
			return FALSE;
		}
		else
		{
			GetBookmark();
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: FindNext function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
returns:	��������ֵ֮һ. ��Ӧ�� OLE DB ���ͱ�ʶ�����±��˵��������
���и���.
[����]			[˵��] 
---------------------------------------------------------
adArray			����������һ������߼� OR ��ָʾ���������������͵�
��ȫ���� (DBTYPE_ARRAY). 
adBigInt			8 �ֽڴ����ŵ����� (DBTYPE_I8). 
adBinary			������ֵ (DBTYPE_BYTES). 
adBoolean			������ֵ (DBTYPE_BOOL). 
adByRef			����������һ������߼� OR ��ָʾ������������������
�ݵ�ָ�� (DBTYPE_BYREF). 
adBSTR			�Կս�β���ַ��� (Unicode) (DBTYPE_BSTR). 
adChar			�ַ���ֵ (DBTYPE_STR). 
adCurrency		����ֵ (DBTYPE_CY).�������ֵ�С����λ�ù̶���С��
���Ҳ�����λ����.��ֵ����Ϊ 8 �ֽڷ�ΧΪ10,000 �Ĵ���
������ֵ. 
adDate			����ֵ (DBTYPE_DATE).���ڰ�˫��������ֵ������, ��
��ȫ����ʾ�� 1899 �� 12 �� 30 ��ʼ��������.С��������
һ�쵱�е�Ƭ��ʱ��. 
adDBDate			����ֵ (yyyymmdd) (DBTYPE_DBDATE). 
adDBTime			ʱ��ֵ (hhmmss) (DBTYPE_DBTIME). 
adDBTimeStamp		ʱ��� (yyyymmddhhmmss �� 10 �ڷ�֮һ��С��)(DBTYPE_DBTIMESTAMP). 
adDecimal			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_DECIMAL). 
adDouble			˫���ȸ���ֵ (DBTYPE_R8). 
adEmpty			δָ��ֵ (DBTYPE_EMPTY). 
adError			32 - λ������� (DBTYPE_ERROR). 
adGUID			ȫ��Ψһ�ı�ʶ�� (GUID) (DBTYPE_GUID). 
adIDispatch		OLE ������ Idispatch �ӿڵ�ָ�� (DBTYPE_IDISPATCH). 
adInteger			4 �ֽڵĴ��������� (DBTYPE_I4). 
adIUnknown		OLE ������ IUnknown �ӿڵ�ָ�� (DBTYPE_IUNKNOWN).
adLongVarBinary	��������ֵ. 
adLongVarChar		���ַ���ֵ. 
adLongVarWChar	�Կս�β�ĳ��ַ���ֵ. 
adNumeric			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_NUMERIC). 
adSingle			�����ȸ���ֵ (DBTYPE_R4). 
adSmallInt		2 �ֽڴ��������� (DBTYPE_I2). 
adTinyInt			1 �ֽڴ��������� (DBTYPE_I1). 
adUnsignedBigInt	8 �ֽڲ����������� (DBTYPE_UI8). 
adUnsignedInt		4 �ֽڲ����������� (DBTYPE_UI4). 
adUnsignedSmallInt 2 �ֽڲ����������� (DBTYPE_UI2). 
adUnsignedTinyInt 1 �ֽڲ����������� (DBTYPE_UI1). 
adUserDefined		�û�����ı��� (DBTYPE_UDT). 
adVarBinary		������ֵ. 
adVarChar			�ַ���ֵ. 
adVariant			�Զ������� (DBTYPE_VARIANT). 
adVector			����������һ������߼� OR ��, ָʾ������ DBVECTOR 
�ṹ(�� OLE DB ����).�ýṹ����Ԫ�صļ������������� 
(DBTYPE_VECTOR) ���ݵ�ָ��. 
adVarWChar		�Կս�β�� Unicode �ַ���. 
adWChar			�Կս�β�� Unicode �ַ��� (DBTYPE_WSTR). 
----------------------------------------------------------
Remarks:	����ָ���ֶε���������.
==========================================================================*/
DataTypeEnum CDBFunction::GetFieldType(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}	
}

DataTypeEnum CDBFunction::GetFieldType(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}	
}

bool CDBFunction::Execute(LPCTSTR strSQL, bool bAutoCloseConnect,long lOptions)
{
	ASSERT(m_pConnection != NULL);
	if (m_pConnection->IsOpen())
	{
		if (m_pConnection->Execute(strSQL,lOptions))
		{
			if (bAutoCloseConnect)
			{
				m_pConnection->Close();
			}			
			return true;
		}
		else
		{
			m_pConnection->Close();
			#ifdef _DEBUG
						CStringTool::WriteLogEx(strSQL);
			#endif
			return false;
		}
		
	}
	else
	{	
		return false;
	}		
}

void CDBFunction::CloseConnect()
{
	CloseRecordset();
	m_pConnection->Close();
}

BOOL CDBFunction::ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	return m_pConnection->ConnectSQLServer(dbsrc,dbname,user,pass,lOptions);
}
BOOL CDBFunction::ConnectAccess(CString dbpath, CString pass, long lOptions)
{
	return m_pConnection->ConnectAccess(dbpath, pass, lOptions);
}
BOOL CDBFunction::ConnectUseUDLFile(LPCTSTR strFileName, long lOptions)
{
	return m_pConnection->ConnectUseUDLFile(strFileName,lOptions);
}



/*========================================================================
name:		����Ψһ��ʶ Recordset �����е�ǰ��¼����ǩ��
==========================================================================*/
_variant_t CDBFunction::GetBookmark()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (IsOpen())
		{
			m_varBookmark = m_pRecordset->GetBookmark();
			return m_varBookmark;
		}		
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetBookmark function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return _variant_t((long)adBookmarkFirst);
}

/*########################################################################
------------------------------------------------
������׷�ӵ������ı������������� Field ����. 
------------------------------------------------
########################################################################*/
BOOL CDBFunction::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
	SAFEARRAY FAR *pSafeArray = NULL;
	SAFEARRAYBOUND rgsabound[1];
	LPBYTE pbData = NULL;	
	
	try
	{
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
		
		for (long i = 0; i < (long)nBytes; i++)
		{
			UCHAR &chData	= ((UCHAR*)lpData)[i];
			HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
			if (FAILED(hr))	return FALSE;
		}
		_variant_t varChunk;		
		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = pSafeArray;
		
		return (pField->AppendChunk(varChunk) == S_OK);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: AppendChunk function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CDBFunction::AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpData != NULL);
	if (adFldLong & GetFieldAttributes(strFieldName))
	{
		return AppendChunk(GetField(strFieldName), lpData, nBytes);
	}
	else return FALSE;
}

BOOL CDBFunction::AppendChunk(FieldPtr pField, LPVOID lpData, long nBytes)
{
	SAFEARRAY FAR *pSafeArray = NULL;
	SAFEARRAYBOUND rgsabound[1];
	LPBYTE pbData = NULL;	
	
	try
	{
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
		
		for (long i = 0; i < (long)nBytes; i++)
		{
			UCHAR &chData	= ((UCHAR*)lpData)[i];
			HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
			if (FAILED(hr))	return FALSE;
		}
		_variant_t varChunk;		
		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = pSafeArray;
		
		return (pField->AppendChunk(varChunk) == S_OK);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: AppendChunk function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CDBFunction::AppendChunk(LPCSTR strFieldName, LPVOID lpData, long nBytes)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpData != NULL);
	if (adFldLong & GetFieldAttributes(strFieldName))
	{
		return AppendChunk(GetField(strFieldName), lpData, nBytes);
	}
	else return FALSE;
}

/*========================================================================
Name:	ȡ��ָ���е��ֶζ����ָ��.	
==========================================================================*/
FieldPtr CDBFunction::GetField(long lIndex)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lIndex));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

FieldPtr CDBFunction::GetField(LPCTSTR lpszFieldName)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lpszFieldName));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

/*========================================================================
name:		ȡ�� Field ����һ����������.
----------------------------------------------------------
returns:	���� Field ����, Attributes ����Ϊֻ��, ��ֵ����Ϊ��������
һ������ FieldAttributeEnum ֵ�ĺ�.
[����]				[˵��] 
-------------------------------------------
adFldMayDefer			ָʾ�ֶα��ӳ�, ������ӵ��������¼������Դ����
�ֶ�ֵ, ������ʽ������Щ�ֶ�ʱ�Ž�row����. 
adFldUpdatable		ָʾ����д����ֶ�. 
adFldUnknownUpdatable ָʾ�ṩ���޷�ȷ���Ƿ����д����ֶ�. 
adFldFixed			ָʾ���ֶΰ�����������. 
adFldIsNullable		ָʾ���ֶν��� Null ֵ. 
adFldMayBeNull		ָʾ���ԴӸ��ֶζ�ȡ Null ֵ. 
adFldLong				ָʾ���ֶ�Ϊ���������ֶ�. ��ָʾ����ʹ�� AppendChunk 
�� GetChunk ����. 
adFldRowID			ָʾ�ֶΰ����־õ�row��ʶ��, �ñ�ʶ���޷���д��
���ҳ��˶�row��row��ʶ(���¼�š�Ψһ��ʶ����)�ⲻ
�����������ֵ. 
adFldRowVersion		ָʾ���ֶΰ����������ٸ��µ�ĳ��ʱ������ڱ��. 
adFldCacheDeferred	ָʾ�ṩ�߻������ֶ�ֵ, ����������Ի���Ķ�ȡ. 
==========================================================================*/
long CDBFunction::GetFieldAttributes(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetAttributes();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldAttributes function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

long CDBFunction::GetFieldAttributes(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetAttributes();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldAttributes function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}


/*########################################################################
------------------------------------------------
�����ֶε�ֵ
------------------------------------------------
########################################################################*/
BOOL CDBFunction::PutCollect(long index, const _variant_t &value)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(index < GetFieldsCount());
	try
	{
		if (m_pRecordset != NULL) 
		{
			m_pRecordset->PutCollect(_variant_t(index), value);
			return	TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: PutCollect function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

BOOL CDBFunction::PutCollect(long index, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adBigInt)
		TRACE(_T("Warning: data type mismatch; file: %s; row: %d\n"), __FILE__, __LINE__);
#endif
	
	return PutCollect(index, _variant_t(value));
}

BOOL CDBFunction::PutCollect(long index, const CString &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (! (GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar
		|| GetFieldType(index) == adVarWChar
		|| GetFieldType(index) == adWChar
		|| GetFieldType(index) == adLongVarWChar))
		TRACE(_T("Warning: data type mismatch; file: %s; row: %d\n"), __FILE__, __LINE__);
#endif
	
	_variant_t vt;
	//BSTR bstrValue;
	vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
	vt.bstrVal=value.AllocSysString();
// 	bstrValue=value.AllocSysString();
// 	vt.bstrVal = bstrValue;
// 	SysFreeString(bstrValue);//AllocSysString֮��Ҫ�ͷ��ڴ�
	return PutCollect(index, vt);
}

BOOL CDBFunction::PutCollect(long index, const string &value)
{
	CString strTemp;
	strTemp=value.c_str();
	return PutCollect(index, strTemp);
}

BOOL CDBFunction::PutCollect(LPCSTR strFieldName, const _variant_t &vValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			m_pRecordset->put_Collect(_variant_t(strFieldName), vValue);
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
		TRACE(_T("Warning: PutCollect function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
	return	FALSE;
}

BOOL CDBFunction::PutCollect(LPCTSTR strFieldName, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBigInt)
		TRACE(_T("Warning: data type mismatch; file: %s; row: %d\n"), __FILE__, __LINE__);
#endif
	
	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CDBFunction::PutCollect(LPCTSTR strFieldName, const CString &vValue)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (! (GetFieldType(strFieldName) == adVarChar
		|| GetFieldType(strFieldName) == adChar
		|| GetFieldType(strFieldName) == adLongVarChar
		|| GetFieldType(strFieldName) == adVarWChar
		|| GetFieldType(strFieldName) == adWChar
		|| GetFieldType(strFieldName) == adLongVarWChar))
		TRACE(_T("Warning: data type mismatch; file: %s; row: %d\n"), __FILE__, __LINE__);
#endif
	
	_variant_t vt;
	//BSTR bstrValue;
	vt.vt = vValue.IsEmpty() ? VT_NULL : VT_BSTR;

	//------------
	vt.bstrVal =vValue.AllocSysString();
// 	bstrValue=vValue.AllocSysString();
// 	vt.bstrVal = bstrValue;
// 	SysFreeString(bstrValue);

	//------------
	return PutCollect(strFieldName, vt);
}

BOOL CDBFunction::PutCollect(LPCTSTR strFieldName, const string &vValue)
{
	CString strTemp;
	strTemp=vValue.c_str();

	return PutCollect(strFieldName,strTemp);
}

BOOL CDBFunction::PutCollect(LPCTSTR strFieldName, LPCTSTR vValue)
{
	CString strTemp;
	strTemp=vValue;
	
	return PutCollect(strFieldName,strTemp);
}

/*========================================================================
	Name:	ȡ���ֶε�ֵ��ʵ�ʳ���.
    ----------------------------------------------------------
	returns:	���س�����ֵ.ĳЩ�ṩ���������ø������Ա�Ϊ BLOB ����Ԥ��
	�ռ�, �ڴ������Ĭ��ֵΪ 0.
    ----------------------------------------------------------
	Remarks:	ʹ�� ActualSize ���Կɷ��� Field ����ֵ��ʵ�ʳ���.��������
	�ֶ�,ActualSize ����Ϊֻ��.��� ADO �޷�ȷ�� Field ����ֵ��ʵ
	�ʳ���, ActualSize ���Խ����� adUnknown.
				�����·�����ʾ, ActualSize ��  DefinedSize ����������ͬ: 
				adVarChar ������������󳤶�Ϊ 50 ���ַ��� Field ���󽫷���Ϊ 
				50 �� DefinedSize ����ֵ, ���Ƿ��ص� ActualSize ����ֵ�ǵ�ǰ��
				¼���ֶ��д洢�����ݵĳ���.
==========================================================================*/
long CDBFunction::GetFieldActualSize(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->ActualSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldActualSize function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}	
}

long CDBFunction::GetFieldActualSize(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->ActualSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldActualSize function error. error msg: %s; file: %s; row: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}	
}