#include "StdAfx.h"
#include "AutoSQL.h"
#include "StringTool.h"


AutoSQL::AutoSQL()
{
	InitVar();
}

AutoSQL::~AutoSQL()
{

}

void AutoSQL::Trim(CString & sSrc)
{
	sSrc.TrimLeft();
	sSrc.TrimRight();
}


bool AutoSQL::IsEqualNoCase(const CString & sSrc1,LPCSTR lpSrc2)
{
	return sSrc1.CompareNoCase(lpSrc2)==0 ? true:false;
}

bool AutoSQL::IsKeyNoCase(const CString & sSrc1)
{
	return 	(IsEqualNoCase(m_sTempWord,"from")||
		IsEqualNoCase(m_sTempWord,"where")||
		IsEqualNoCase(m_sTempWord,"group")||
		IsEqualNoCase(m_sTempWord,"having")||
		IsEqualNoCase(m_sTempWord,"order"));
	
}

bool AutoSQL::IsWhiteSpace(char cSrc)
{
	if (cSrc>32) return false;
	
    if ((cSrc>=8)&&(cSrc<=13)) return true;
	if (cSrc==32) return true;
	
    /* 8 BackSpae */		 		  
	/* 9 Tab */		 			  
	/* 10 NewLine */				  
	/* 11 Vertical Tab */		  
	/* 12 Form Feed */			  
	/* 13 Carriage Return */ 
	/* 32 Space */	 //Character is a WhiteSpace.
	return false;
	
}

void AutoSQL::Parse(const CString &sOriginSQL,
					int & nBegin,
					int & nEnd,
					LPCSTR lpSeparater,
					LPCSTR lpKeyWord,
					CStringArray &CArrReturn)
{
	bool bIsNewWord=TRUE,bIsNewSentence=TRUE,bSetEndWord=false;	
	
	m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);	
	
	if (!IsEqualNoCase(m_sBeginWord,lpKeyWord))
	{
		if (IsKeyNoCase(m_sTempWord))
		{
			m_sBeginWord=m_sTempWord;
			KeyWordBackWard(sOriginSQL,nBegin);
		}
		return;
	}
	else
	{
		m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);	
		if (IsEqualNoCase(lpKeyWord,"group")||IsEqualNoCase(lpKeyWord,"order"))
		{
			m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);	
		}
	}
		
	while(m_sTempWord.GetLength()>0)
	{
		if (IsKeyNoCase(m_sTempWord))
		{
			m_sBeginWord=m_sTempWord;
			//字符倒退一个
			KeyWordBackWard(sOriginSQL,nBegin);
			break;
		}

		/*
		if (IsEqualNoCase(m_sTempWord,"by"))
		{
			m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);	
		}
		*/

		if (bIsNewWord)
		{
			CArrReturn.Add(m_sTempWord);
		}
		else
		{
			CArrReturn[CArrReturn.GetSize()-1]+=" "+m_sTempWord;
		}		
		
		bIsNewWord=false;
		
		if (lpSeparater==",")
		{
			if (nBegin<sOriginSQL.GetLength()&&sOriginSQL[nBegin]==',')
			{
				bIsNewWord=true;
			}
			m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);
			continue;
		}

		m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);

		if (IsEqualNoCase(lpSeparater,"and")&&IsEqualNoCase(m_sTempWord,"and"))
		{
			bIsNewWord=true;
			m_sTempWord=GetOneWord(sOriginSQL,nBegin,nEnd);
		}
		
	
	}	
}
		 
CString AutoSQL::GetOneWord(const CString &sOrigin,int & nBegin,int & nEnd)
{
	CString sOneWord="";
	
	if (nBegin>=sOrigin.GetLength())
	{
		return sOneWord;
	}	
	/*
	select a,b, 9 as c ,   d  from e
	*/
	//先把空白字符和逗号跳过 这里主要是要定位到一个有效列的第一个字符
	while((nBegin<sOrigin.GetLength())&&(IsWhiteSpace(sOrigin[nBegin])||sOrigin[nBegin]==','))
	{
		nBegin++;
	}
	
	nEnd=nBegin;

	while(nEnd<sOrigin.GetLength())
	{
		//有括号的是子语句
		m_iBracketNum=0;
		if (sOrigin[nEnd]=='(')
		{
			m_iBracketNum++;
			while(m_iBracketNum>0)
			{
				nEnd++;
				if (sOrigin[nEnd]=='(')
				{
					m_iBracketNum++;
				}
				if (sOrigin[nEnd]==')')
				{
				m_iBracketNum--;
				}
				
			}		
		  //带括号的子句取完了
		}
		//空白字符或者逗号,结束
		if (IsWhiteSpace(sOrigin[nEnd])||sOrigin[nEnd]==',')
		{
			break;
		}
		nEnd++;
	}	

	sOneWord=sOrigin.Mid(nBegin,nEnd-nBegin);
	/*
	select a,b, 9 as c ,   d  from e
	*/
	//再次把空白字符过  这里主要是定位到逗号或者下一个字符，如果不是逗号，则下一个字符不是新列
	nBegin=nEnd;
	while(nBegin<sOrigin.GetLength()&&IsWhiteSpace(sOrigin[nBegin]))
	{
		nBegin++;
	}
	
	return sOneWord;

}

void AutoSQL::KeyWordBackWard(const CString &sOrigin,int & nBegin)
{
	//pos倒退到上一个字符
	//GetOneWord 将begin 定位到逗号或者新字符的开始
	//尾倒退
	//倒推的时候nBegin<=0的情况是不应该存在的
	ASSERT(nBegin>0);
	
	nBegin--;
	
	//定位到上一个字符的尾
	while(nBegin>=0&&(IsWhiteSpace(sOrigin[nBegin])||sOrigin[nBegin]==','))
	{
		nBegin--;
	}
	
	if (nBegin<=0)
	{
		return;
	}

	while(nBegin>=0&&isalpha(sOrigin[nBegin]))
	{
		nBegin--;
	}
	nBegin++;
}

void AutoSQL::ParseSelect(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{	
	Parse(sOriginSQL,nBegin,nEnd,",","select",CArrReturn);	
}

void AutoSQL::ParseFrom(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{
	Parse(sOriginSQL,nBegin,nEnd,",","from",CArrReturn);
}

void AutoSQL::ParseWhere(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{
	Parse(sOriginSQL,nBegin,nEnd,"and","where",CArrReturn);
}

void AutoSQL::ParseGroupBy(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{
	Parse(sOriginSQL,nBegin,nEnd,",","group",CArrReturn);
}

void AutoSQL::ParseHaving(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{
	Parse(sOriginSQL,nBegin,nEnd,"and","having",CArrReturn);
}

void AutoSQL::ParseOrderBy(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn)
{
	Parse(sOriginSQL,nBegin,nEnd,",","order",CArrReturn);
}

void AutoSQL::InitVar()
{
	m_sArrColumns.RemoveAll();				//列名的数组
	m_sArrTables.RemoveAll();				//表名的数组
	m_sArrWheres.RemoveAll();				//where后面的条件
	m_sArrGroupBy.RemoveAll();				//group by 的内容
	m_sArrHaving.RemoveAll();				//having 的内容
	m_sArrOrderBy.RemoveAll();				//order by 的内容

	m_CArrCondition.RemoveAll();

	m_bAutoGroupBy=false;					//加的列里面有sum()需要把列都group by
	
	m_sOriginSQL="";					//初始的SQL

	m_sTempWord="";

	m_lSQLLength=0;					//原始语句的长度
	m_nTempWordBegin=0;					//分解的一段的起点
	m_nTempWordEnd=0;					//分解的一段的终点
}


int AutoSQL::SetOriginSQL(CString &sSQL)
{
	//先去掉头尾的空格
	Trim(sSQL);
	m_lSQLLength=sSQL.GetLength();

	if (m_lSQLLength<=6)
	{
		//select 长度就已经是6了
		return -1;
	}

	InitVar();

	m_sOriginSQL=sSQL;
	//先分解原始的SQL
	ParseSQL(m_sOriginSQL);
	return 1;
}

void AutoSQL::ParseSQL(const CString & sSql)
{	
	//分解m_sOriginSQL;
	//先取select;
	m_sTempWord=sSql.Mid(0,6);
	if (!IsEqualNoCase(m_sTempWord,"select"))//不是select 开头的
	{
		return;
	}
	/* 样板
	select bh,sum(sno) 
	from soeevent as b left join ccc on b.xx=ccc.x 
	where pno>10 and bh>1 
	group by bh,xxx,xxx 
	having sum(sno)>1 and bh>1000 
	order by bh,xxx,xxx
	*/
	//定位到0
	m_nTempWordBegin=0;
	m_sBeginWord="select";
	m_nTempWordEnd=m_nTempWordBegin;
	//开始分解select后面的列,分解到m_sArrColumns
	ParseSelect(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrColumns);
	
	//开始分解from后面的表名
	ParseFrom(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrTables);
	
	//开始分解where
	ParseWhere(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrWheres);
	
	//开始分解group by
	ParseGroupBy(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrGroupBy);

	//开始分解 having 
	ParseHaving(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrHaving);

	//开始分解 order by
	ParseOrderBy(sSql,m_nTempWordBegin,m_nTempWordEnd,m_sArrOrderBy);
}

CString AutoSQL::GetDealedSQL()
{
	//组合
	CString sReturn="";
	
	AddWords(sReturn,m_sArrColumns,"select",",");
	AddWords(sReturn,m_sArrTables,"from",",");
	AddWords(sReturn,m_sArrWheres,m_CArrCondition,"where","and");
	AddWords(sReturn,m_sArrGroupBy,"group by",",");
	AddWords(sReturn,m_sArrHaving,"having","and");
	AddWords(sReturn,m_sArrOrderBy,"order by",",");	

	return sReturn;
}

int AutoSQL::AddWords(CString &Return,
					  const CStringArray &CArrSrc,
					  LPCSTR lpHead,
					  LPCSTR lpSeparator)
{
	int nPos,nSize;
	nSize=CArrSrc.GetSize();
	if (nSize<=0)
	{
		return 0;
	}

	Return+=lpHead;
	Return+=" "+CArrSrc[0];

	for (nPos=1;nPos<nSize;nPos++)
	{
		Return+=" ";
		Return+=lpSeparator;
		Return+=" "+CArrSrc[nPos];
	}
	Return+=" ";
	return 1;
}

int AutoSQL::AddWords(CString &Return,
					  const CStringArray &CArrSrc,
					  const CArray<struct_Condition,struct_Condition &> &CArrAppend,
					  LPCSTR lpHead,
					  LPCSTR lpSeparator)
{
	int nPos,nSize,nSizeAppend;
	bool bHaveDealFrist=false;

	nSize=CArrSrc.GetSize();
	nSizeAppend=CArrAppend.GetSize();

	if (nSize==0&&nSizeAppend==0)
	{
		return 0;
	}

	Return+=lpHead;
	Return+=" ";

	for (nPos=0;nPos<nSize;nPos++)
	{

		if (bHaveDealFrist)
		{
			Return+=lpSeparator;
			Return+=" "+CArrSrc[nPos];	
			Return+=" ";
		}
		else
		{
			Return+=CArrSrc[nPos]+" ";
			bHaveDealFrist=true;
		}		
	}

	for (nPos=0;nPos<nSizeAppend&&CArrAppend[nPos].m_bCanUse;nPos++)
	{
		
		if (bHaveDealFrist)
		{
			Return+=lpSeparator;
			Return+=" ("+CArrAppend[nPos].m_sConditionSQL;
			Return+=") ";			
		}
		else
		{
			Return+="(";
			Return+=CArrAppend[nPos].m_sConditionSQL+") ";
			bHaveDealFrist=true;
		}		
	}
	
	return 1;	
}

int AutoSQL::AddCondition(CString sConditionname,CString sConditionSQL)
{
	
	if (sConditionname.GetLength()<=0||sConditionSQL.GetLength()<=0)
	{
		return 0;
	}

	sConditionname.MakeLower();
	Trim(sConditionname);
	
	sConditionSQL.MakeLower();
	Trim(sConditionSQL);

	m_CArrCondition.SetSize(m_CArrCondition.GetSize()+1);

	m_CArrCondition[m_CArrCondition.GetSize()-1].m_bCanUse=true;
	m_CArrCondition[m_CArrCondition.GetSize()-1].m_sConditionName=sConditionname;
	m_CArrCondition[m_CArrCondition.GetSize()-1].m_sConditionSQL=sConditionSQL;

	return 1;
}

int AutoSQL::AddCondition(string & sConditionname,string & sConditionSQL)
{
	
	if (sConditionname.length()<=0||sConditionSQL.length()<=0)
	{
		return 0;
	}
	

	CStringTool::trim(sConditionname);
	CStringTool::MakeUpper(sConditionname);

	
	
	CStringTool::trim(sConditionSQL);
	CStringTool::MakeUpper(sConditionSQL);
	
	m_CArrCondition.SetSize(m_CArrCondition.GetSize()+1);
	
	m_CArrCondition[m_CArrCondition.GetSize()-1].m_bCanUse=true;
	m_CArrCondition[m_CArrCondition.GetSize()-1].m_sConditionName=sConditionname.c_str();
	m_CArrCondition[m_CArrCondition.GetSize()-1].m_sConditionSQL=sConditionSQL.c_str();
	
	return 1;
}

int AutoSQL::AddOrderBy(CString sOrderbySQL)
{
	m_sArrOrderBy.Add(sOrderbySQL);
	return 1;
}

int AutoSQL::AddOrderBy(const string & sOrderbySQL)
{
	m_sArrOrderBy.Add(sOrderbySQL.c_str());
	return 1;
}

int AutoSQL::AddColumn(CString strColumn)
{
	m_sArrColumns.Add(strColumn);
	return 1;
}

int AutoSQL::DelCondition(CString sConditionname)
{
	if (sConditionname.GetLength()<=0)
	{
		return 0;
	}
	
	sConditionname.MakeLower();
	Trim(sConditionname);
	
	int pos=0;
	for (pos=0;pos<m_CArrCondition.GetSize();pos++)
	{
		if (m_CArrCondition[pos].m_sConditionName==sConditionname)
		{
			m_CArrCondition.RemoveAt(pos);
			break;
		}
	}
	
	return 1;
}

int AutoSQL::RemoveCondition()
{
	m_CArrCondition.RemoveAll();

	return 1;
}

