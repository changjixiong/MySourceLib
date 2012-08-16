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
	//��Щ����ԭʼSQL�ֽ������
	CStringArray m_sArrColumns;				//����������
	CStringArray m_sArrTables;				//����������
	CStringArray m_sArrWheres;				//where���������
	CStringArray m_sArrGroupBy;				//group by ������
	CStringArray m_sArrHaving;				//having ������
	CStringArray m_sArrOrderBy;				//order by ������

	CArray<struct_Condition,struct_Condition &> m_CArrCondition;	//���ӵ���������
	

	bool m_bAutoGroupBy;					//�ӵ���������sum()��Ҫ���ж�group by


	CString m_sOriginSQL;					//��ʼ��SQL
	CString m_sTempWord;					//�ֽ��һ��
	char    m_cTempChar;					//��ǰ�ַ�
	long	m_lSQLLength;					//ԭʼ���ĳ���
	int    m_nTempWordBegin;					//�ֽ��һ�ε����
	int    m_nTempWordEnd;					//�ֽ��һ�ε��յ�
	int	   m_iBracketNum;					//���ż���
	CString m_sBeginWord;				//ͷ�ַ�  select ,from ,�ȵȹؼ���

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

	bool IsEqualNoCase(const CString & sSrc1,LPCSTR lpSrc2);	//�����ִ�Сд�Ƿ����
	void Trim(CString & sSrc);									//ȥ��ͷβ�Ŀ��ַ�
	CString GetOneWord(const CString &sOrigin,int & nBegin,int & nEnd);
	int AddWords(CString &Return,const CStringArray &CArrSrc,LPCSTR lpHead,LPCSTR lpSeparator);
	int AddWords(CString &Return,const CStringArray &CArrSrc,const CArray<struct_Condition,struct_Condition &> &CArrAppend,LPCSTR lpHead,LPCSTR lpSeparator);
	bool IsWhiteSpace(char cSrc);
	bool IsKeyNoCase(const CString & sSrc1);
	void KeyWordBackWard(const CString &sOrigin,int & nBegin);

};


#endif