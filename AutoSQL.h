#ifndef AUTOSQL_H
#define AUTOSQL_H
#include <afxtempl.h>
#include <string>

using namespace std;

/*
select bh,sum(sno) 
from soeevent as b left join ccc on b.xx=ccc.x 
where pno>10 and bh>1 
group by bh,xxx,xxx 
having sum(sno)>1 and bh>1000 
order by bh,xxx,xxx
*/

struct struct_Condition
{
	CString m_sConditionName;
	CString m_sConditionSQL;
	bool    m_bCanUse;
};

class AutoSQL
{
public:
	AutoSQL();
	~AutoSQL();

	int SetOriginSQL(CString &sSQL);
	int AddCondition(CString sConditionname,CString sConditionSQL);
	int AddCondition(string & sConditionname,string & sConditionSQL);
	int AddOrderBy(CString sOrderbySQL);
	int AddOrderBy(const string & sOrderbySQL);
	int AddColumn(CString strColumn);
	int DelCondition(CString sConditionname);
	int RemoveCondition();
	CString GetDealedSQL();

private:
	//这些都是原始SQL分解出来的
	CStringArray m_sArrColumns;				//列名的数组
	CStringArray m_sArrTables;				//表名的数组
	CStringArray m_sArrWheres;				//where后面的条件
	CStringArray m_sArrGroupBy;				//group by 的内容
	CStringArray m_sArrHaving;				//having 的内容
	CStringArray m_sArrOrderBy;				//order by 的内容

	CArray<struct_Condition,struct_Condition &> m_CArrCondition;	//附加的限制条件
	

	bool m_bAutoGroupBy;					//加的列里面有sum()需要把列都group by


	CString m_sOriginSQL;					//初始的SQL
	CString m_sTempWord;					//分解的一段
	char    m_cTempChar;					//当前字符
	long	m_lSQLLength;					//原始语句的长度
	int    m_nTempWordBegin;					//分解的一段的起点
	int    m_nTempWordEnd;					//分解的一段的终点
	int	   m_iBracketNum;					//括号计数
	CString m_sBeginWord;				//头字符  select ,from ,等等关键字

private:
	void InitVar();
	void ParseSQL(const CString & sSQL);

	void Parse(const CString &sOriginSQL,int & nBegin,int & nEnd,LPCSTR lpSeparater,LPCSTR lpKeyWord,CStringArray &CArrReturn);
	void ParseSelect(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);
	void ParseFrom(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);
	void ParseWhere(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);
	void ParseGroupBy(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);
	void ParseHaving(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);
	void ParseOrderBy(const CString &sOriginSQL,int & nBegin,int & nEnd,CStringArray &CArrReturn);

	bool IsEqualNoCase(const CString & sSrc1,LPCSTR lpSrc2);	//不区分大小写是否相等
	void Trim(CString & sSrc);									//去掉头尾的空字符
	CString GetOneWord(const CString &sOrigin,int & nBegin,int & nEnd);
	int AddWords(CString &Return,const CStringArray &CArrSrc,LPCSTR lpHead,LPCSTR lpSeparator);
	int AddWords(CString &Return,const CStringArray &CArrSrc,const CArray<struct_Condition,struct_Condition &> &CArrAppend,LPCSTR lpHead,LPCSTR lpSeparator);
	bool IsWhiteSpace(char cSrc);
	bool IsKeyNoCase(const CString & sSrc1);
	void KeyWordBackWard(const CString &sOrigin,int & nBegin);

};


#endif