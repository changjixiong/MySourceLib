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
			AfxMessageBox("RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
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
Name:	关闭连接并释放对象.
-----------------------------------------------------
Remarks: 关闭连接并释放CAdoRecordSet对象, 这样基本上从内容中完全清除了
CAdoRecordSet对象.
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
Name:		指针是否在在记录集头
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
Name:		指针是否在在记录集尾
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
Name:		取得记录集对象的状态(是打开状态还是关闭状态). 对异步方式执
row的 Recordset 对象, 则说明当前的对象状态是连接、执row还是获取状态.
-----------------------------------------------------
returns:	返回下列常量之一的长整型值.
[常量]				[说明] 
----------------------------------
adStateClosed		指示对象是关闭的. 
adStateOpen			指示对象是打开的. 
adStateConnecting	指示 Recordset 对象正在连接. 
adStateExecuting	指示 Recordset 对象正在执row命令. 
adStateFetching		指示 Recordset 对象的row正在被读取. 
-----------------------------------------------------
Remarks:	 可以随时使用 State 属性确定指定对象的当前状态. 该属性是只
读的. Recordset 对象的 State 属性可以是组合值. 例如: 如果正在执row语句,
该属性将是 adStateOpen 和 adStateExecuting 的组合值.
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
记录集更新操作
------------------------------------------------
########################################################################*/

/*========================================================================
Remarks:	开始添加新的纪录. 
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
Remarks:	在调用 AddNew 等方法后, 调用此方法完成更新或修改.
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
Name:		取消执row挂起的异步 Execute 或 Open 方法的调用.
-----------------------------------------------------
Remarks:	使用 Cancel 方法终止执row异步 Execute 或 Open 方法调用(即通
过 adAsyncConnect、adAsyncExecute 或 adAsyncFetch 参数调用的方法).
如果在试图终止的方法中没有使用 adAsyncExecute, 则 Cancel 将返回运row
时错误.
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
Name:		关闭打开的对象及任何相关对象.
-----------------------------------------------------
Remarks:	使用 Close 方法可关闭 Recordset 对象以便释放所有关联的系统
资源. 关闭对象并非将它从内存中删除, 可以更改它的属性设置并且在此后
再次打开. 要将对象从内存中完全删除, 可将对象变量设置为 NULL.
如果正在立即更新模式下进row编辑, 调用Close方法将产生错误,应首先
调用 Update 或 CancelUpdat 方法. 如果在批更新期间关闭 Recordset 对
象, 则自上次 UpdateBatch 调用以来所做的修改将全部丢失.
如果使用 Clone 方法创建已打开的 Recordset 对象的副本, 关闭原始
Recordset或其副本将不影响任何其他副本.
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
Name:		取消在调用 Update 方法前对当前记录或新记录所作的任何更改.
-----------------------------------------------------
Remarks:	使用 CancelUpdate 方法可取消对当前记录所作的任何更改或放弃
新添加的记录. 在调用 Update 方法后将无法撤消对当前记录或新记录所做的更
改, 除非更改是可以用 RollbackTrans 方法回卷的事务的一部分, 或者是可以
用 CancelBatch 方法取消的批更新的一部分.
如果在调用 CancelUpdate 方法时添加新记录, 则调用 AddNew 之前的当前
记录将再次成为当前记录.
如果尚未更改当前记录或添加新记录, 调用 CancelUpdate 方法将产生错误.
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
- strSQL:		SQL语句, 表名, 存储过程调用或持久 Recordset file名.
- CursorType:   可选. CursorTypeEnum 值, 确定打开 Recordset 时应该
使用的游标类型. 可为下列常量之一.
[常量]				[说明] 
-----------------------------------------------
adOpenForwardOnly	打开仅向前类型游标. 
adOpenKeyset		打开键集类型游标. 
adOpenDynamic		打开动态类型游标. 
adOpenStatic		打开静态类型游标. 
-----------------------------------------------
- LockType:     可选, 确定打开 Recordset 时应该使用的锁定类型(并发)
的 LockTypeEnum 值, 可为下列常量之一.
[常量]				[说明] 
-----------------------------------------------
adLockReadOnly		只读 - 不能改变数据. 
adLockPessimistic	保守式锁定 - 通常通过在编辑时立即锁定数据源的记录. 
adLockOptimistic	开放式锁定 - 只在调用 Update 方法时才锁定记录. 
adLockBatchOptimistic 开放式批更新 - 用于批更新模式(与立即更新模式
相对). 
-----------------------------------------------
- lOption		可选. 长整型值, 用于指示 strSQL 参数的类型. 可为下
列常量之一.
[常量]				[说明] 
-------------------------------------------------
adCmdText			指示strSQL为命令文本, 即普通的SQL语句. 
adCmdTable			指示ADO生成SQL查询返回以 strSQL 命名的表中的
所有row. 
adCmdTableDirect	指示所作的更改在strSQL中命名的表中返回所有row. 
adCmdStoredProc		指示strSQL为存储过程. 
adCmdUnknown		指示strSQL参数中的命令类型为未知. 
adCmdFile			指示应从在strSQL中命名的file中恢复保留(保存的)
Recordset. 
adAsyncExecute		指示应异步执rowstrSQL. 
adAsyncFetch		指示在提取 Initial Fetch Size 属性中指定的初始
数量后, 应该异步提取所有剩余的row. 如果所需的row尚未
提取, 主要的线程将被堵塞直到row重新可用. 
adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
的row尚未提取, 当前row自动移到file末尾. 
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
Name:		获取当前记录集中记录数目
==========================================================================*/
long CDBFunction::GetRecordCount()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		long count = m_pRecordset->GetRecordCount();
		
		// 如果ado不支持此属性，则手工计算记录数目 --------
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
Name:		获取当前记录集中字段数目
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
Remarks:	Recordset 对象包括 Field 对象组成的 Fields 集合. 每个Field
对象对应 Recordset 集中的一列.
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
name:		指定 Recordset 对象当前记录的序号位置. 
----------------------------------------------------------
returns:	设置或返回从 1 到 Recordset 对象 (PageCount) 所含页数的长整
型值，或者返回以下常量. 
[常量]			[说明]
---------------------------------
adPosUnknown	Recordset 为空，当前位置未知，或者提供者不支持 AbsolutePage 属性.  
adPosBOF		当前记录指针位于 BOF(即 BOF 属性为 True).  
adPosEOF		当前记录指针位于 EOF(即 EOF 属性为 True).  
----------------------------------------------------------
Remarks:		使用 AbsolutePosition 属性可根据其在 Recordset 中的序号
位置移动到记录，或确定当前记录的序号位置. 提供者必须支持该属性的相应功
能才能使用该属性. 
同 AbsolutePage 属性一样，AbsolutePosition 从 1 开始，并在当前记录
为 Recordset 中的第一个记录时等于 1. 从 RecordCount 属性可获得 Recordset 
对象的总记录数. 
设置 AbsolutePosition 属性时，即使该属性指向位于当前缓存中的记录，
ADO 也将使用以指定的记录开始的新记录组重新加载缓存. CacheSize 属性决定
该记录组的大小. 
注意   不能将 AbsolutePosition 属性作为替代的记录编号使用. 删除前面
的记录时，给定记录的当前位置将发生改变. 如果 Recordset 对象被重新查询或
重新打开，则无法保证给定记录有相同的 AbsolutePosition. 书签仍然是保持和
返回给定位置的推荐方式，并且在所有类型的 Recordset 对象的定位时是唯一的
方式. 
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
读取字段的值
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
								记录集导航操作
			  ------------------------------------------------
  ########################################################################*/

/*========================================================================
	Name:		将当前记录位置移动到 Recordse 中的第一个记录.
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
	Name:		将当前记录位置移动到 Recordset 中的最后一个记录.
	-----------------------------------------------------
	Remarks:	Recordset 对象必须支持书签或向后光标移动; 否则调用该方法将
			产生错误.
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
	Name:		将当前记录位置向后移动一个记录(向记录集的顶部).
	-----------------------------------------------------
	Remarks:	Recordset 对象必须支持书签或向后游标移动; 否则方法调用将产
	生错误.如果首记录是当前记录并且调用 MovePrevious 方法, 则 ADO 将当前记
	录设置在 Recordset (BOF为True) 的首记录之前. 而BOF属性为 True 时向后移
	动将产生错误. 如果 Recordse 对象不支持书签或向后游标移动, 则 MovePrevious 
	方法将产生错误.
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
	Name:		将当前记录向前移动一个记录(向 Recordset 的底部).
	-----------------------------------------------------
	Remarks:	如果最后一个记录是当前记录并且调用 MoveNext 方法, 则 ADO 将
	当前记录设置到 Recordset (EOF为 True)的尾记录之后. 当 EOF 属性已经为 
	True 时试图向前移动将产生错误.
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
	Name:		移动 Recordset 对象中当前记录的位置.
    ----------------------------------------------------------
	Params:		
		- lRecords    带符号长整型表达式, 指定当前记录位置移动的记录数.
		- Start    可选, 字符串或变体型, 用于计算书签. 也可为下列 
		BookmarkEnum 值之一: 
		[常量]				[说明] 
		--------------------------------
		adBookmarkCurrent	默认. 从当前记录开始. 
		adBookmarkFirst		从首记录开始. 
		adBookmarkLast		从尾记录开始. 
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
name:		搜索 Recordset 中满足指定标准的记录. 如果满足标准，则记录集
位置设置在找到的记录上，否则位置将设置在记录集的末尾. 
----------------------------------------------------------
params:		[criteria]:   字符串，包含指定用于搜索的列名、比较操作符和
值的语句. 
[searchDirection]:    可选的 SearchDirectionEnum 值，指定搜
索应从当前row还是下一个有效row开始. 其值可为 adSearchForward 或 adSearchBackward. 
搜索是在记录集的开始还是末尾结束由 searchDirection 值决定. 
[常量]			[说明]
---------------------------------
adSearchForward 	
adSearchBackward	
----------------------------------------------------------
Remarks:	criteria 中的"比较操作符"可以是">"(大于)、"<"(小于)、"="(等
于)、">="(大于或等于)、"<="(小于或等于)、"<>"(不等于)或"like"(模式匹配).  
criteria 中的值可以是字符串、浮点数或者日期. 字符串值以单引号分界(如
"state = 'WA'"). 日期值以"#"(数字记号)分界(如"start_date > #7/22/97#"). 
如"比较操作符"为"like"，则字符串"值"可以包含"*"(某字符可出现一次或
多次)或者"_"(某字符只出现一次). (如"state like M_*"与 Maine 和 Massachusetts 
匹配.). 
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
name:		查找下一条满足条件的记录.
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
returns:	返回下列值之一. 相应的 OLE DB 类型标识符在下表的说明栏的括
号中给出.
[常量]			[说明] 
---------------------------------------------------------
adArray			与其他类型一起加入逻辑 OR 以指示该数据是那种类型的
安全数组 (DBTYPE_ARRAY). 
adBigInt			8 字节带符号的整数 (DBTYPE_I8). 
adBinary			二进制值 (DBTYPE_BYTES). 
adBoolean			布尔型值 (DBTYPE_BOOL). 
adByRef			与其他类型一起加入逻辑 OR 以指示该数据是其他类型数
据的指针 (DBTYPE_BYREF). 
adBSTR			以空结尾的字符串 (Unicode) (DBTYPE_BSTR). 
adChar			字符串值 (DBTYPE_STR). 
adCurrency		货币值 (DBTYPE_CY).货币数字的小数点位置固定、小数
点右侧有四位数字.该值保存为 8 字节范围为10,000 的带符
号整型值. 
adDate			日期值 (DBTYPE_DATE).日期按双精度型数值来保存, 数
字全部表示从 1899 年 12 月 30 开始的日期数.小数部分是
一天当中的片段时间. 
adDBDate			日期值 (yyyymmdd) (DBTYPE_DBDATE). 
adDBTime			时间值 (hhmmss) (DBTYPE_DBTIME). 
adDBTimeStamp		时间戳 (yyyymmddhhmmss 加 10 亿分之一的小数)(DBTYPE_DBTIMESTAMP). 
adDecimal			具有固定精度和范围的精确数字值 (DBTYPE_DECIMAL). 
adDouble			双精度浮点值 (DBTYPE_R8). 
adEmpty			未指定值 (DBTYPE_EMPTY). 
adError			32 - 位错误代码 (DBTYPE_ERROR). 
adGUID			全局唯一的标识符 (GUID) (DBTYPE_GUID). 
adIDispatch		OLE 对象上 Idispatch 接口的指针 (DBTYPE_IDISPATCH). 
adInteger			4 字节的带符号整型 (DBTYPE_I4). 
adIUnknown		OLE 对象上 IUnknown 接口的指针 (DBTYPE_IUNKNOWN).
adLongVarBinary	长二进制值. 
adLongVarChar		长字符串值. 
adLongVarWChar	以空结尾的长字符串值. 
adNumeric			具有固定精度和范围的精确数字值 (DBTYPE_NUMERIC). 
adSingle			单精度浮点值 (DBTYPE_R4). 
adSmallInt		2 字节带符号整型 (DBTYPE_I2). 
adTinyInt			1 字节带符号整型 (DBTYPE_I1). 
adUnsignedBigInt	8 字节不带符号整型 (DBTYPE_UI8). 
adUnsignedInt		4 字节不带符号整型 (DBTYPE_UI4). 
adUnsignedSmallInt 2 字节不带符号整型 (DBTYPE_UI2). 
adUnsignedTinyInt 1 字节不带符号整型 (DBTYPE_UI1). 
adUserDefined		用户定义的变量 (DBTYPE_UDT). 
adVarBinary		二进制值. 
adVarChar			字符串值. 
adVariant			自动变体型 (DBTYPE_VARIANT). 
adVector			与其他类型一起加入逻辑 OR 中, 指示数据是 DBVECTOR 
结构(由 OLE DB 定义).该结构含有元素的计数和其他类型 
(DBTYPE_VECTOR) 数据的指针. 
adVarWChar		以空结尾的 Unicode 字符串. 
adWChar			以空结尾的 Unicode 字符串 (DBTYPE_WSTR). 
----------------------------------------------------------
Remarks:	返回指定字段的数据类型.
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
name:		返回唯一标识 Recordset 对象中当前记录的书签。
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
将数据追加到大型文本、二进制数据 Field 对象. 
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
Name:	取得指定列的字段对象的指针.	
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
name:		取得 Field 对象一项或多项属性.
----------------------------------------------------------
returns:	对于 Field 对象, Attributes 属性为只读, 其值可能为以下任意
一个或多个 FieldAttributeEnum 值的和.
[常量]				[说明] 
-------------------------------------------
adFldMayDefer			指示字段被延迟, 即不从拥有整个记录的数据源检索
字段值, 仅在显式访问这些字段时才进row检索. 
adFldUpdatable		指示可以写入该字段. 
adFldUnknownUpdatable 指示提供者无法确定是否可以写入该字段. 
adFldFixed			指示该字段包含定长数据. 
adFldIsNullable		指示该字段接受 Null 值. 
adFldMayBeNull		指示可以从该字段读取 Null 值. 
adFldLong				指示该字段为长二进制字段. 并指示可以使用 AppendChunk 
和 GetChunk 方法. 
adFldRowID			指示字段包含持久的row标识符, 该标识符无法被写入
并且除了对row进row标识(如记录号、唯一标识符等)外不
存在有意义的值. 
adFldRowVersion		指示该字段包含用来跟踪更新的某种时间或日期标记. 
adFldCacheDeferred	指示提供者缓存了字段值, 并已完成随后对缓存的读取. 
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
设置字段的值
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
// 	SysFreeString(bstrValue);//AllocSysString之后要释放内存
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
	Name:	取得字段的值的实际长度.
    ----------------------------------------------------------
	returns:	返回长整型值.某些提供者允许设置该属性以便为 BLOB 数据预留
	空间, 在此情况下默认值为 0.
    ----------------------------------------------------------
	Remarks:	使用 ActualSize 属性可返回 Field 对象值的实际长度.对于所有
	字段,ActualSize 属性为只读.如果 ADO 无法确定 Field 对象值的实
	际长度, ActualSize 属性将返回 adUnknown.
				如以下范例所示, ActualSize 和  DefinedSize 属性有所不同: 
				adVarChar 声明类型且最大长度为 50 个字符的 Field 对象将返回为 
				50 的 DefinedSize 属性值, 但是返回的 ActualSize 属性值是当前记
				录的字段中存储的数据的长度.
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