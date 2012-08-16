#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
bool IsNagative(int nType,unsigned int  *pa);
const char * GetSignificand(char * const pDest,int nType,unsigned int  *pa);
const char * GetExponent(char * const pDest,int nType,unsigned int  *pa);

void PrintBit(int nType,unsigned int  *pa);
void PrintFloatBit(unsigned int  *pa);
void PrintDoubleBit(unsigned int  *pa);

float BCharToFloat(const char * pchar);
double BCharToDouble(const char * pchar);
int BCharToInt(const char * pchar);
void PrintFloatAndDouble(int nType,unsigned int  *pa);
float MakeFloat(int nType,bool bIsNagative, char * pExponent, const char * pSignificand);
double MakeDouble(int nType,bool bIsNagative, char * pExponent, const char * pSignificand);

#define FLOAT_TYPE 1
#define DOUBLE_TYPE 2

void PrintFloatAndDouble(int nType,unsigned int  *pa)
{
	char szExponent[12],szSignificand[53];
	memset(szExponent,0,12);
	memset(szSignificand,0,53);
	int nExponent;

	PrintBit(nType,pa);	
		
	GetExponent(szExponent,nType,pa);
	GetSignificand(szSignificand,nType,pa);

	nExponent=BCharToInt(szExponent);

	cout<<": "<<szExponent;

	cout<<" e:"<<nExponent<<", E:"<<nExponent - (nType==FLOAT_TYPE ? 127: 1023)<<" ,";

	if (nType==FLOAT_TYPE)
	{
		cout<<*((float*)pa)<<endl;
		cout<<"              "<<szSignificand<<"   ";
		cout<<MakeFloat(nType,IsNagative(nType,pa),szExponent,szSignificand)<<endl<<endl;	
	}
	else
	{
		cout<<*((double*)pa)<<endl;
		cout<<"              "<<szSignificand<<"   ";
		cout<<MakeDouble(nType,IsNagative(nType,pa),szExponent,szSignificand)<<endl<<endl;	
	}
	
}

float MakeFloat(int nType,bool bIsNagative, char * pExponent, const char * pSignificand)
{
	long nPower=(BCharToInt(pExponent) - (nType==FLOAT_TYPE ? 127: 1023));	
	int nSign= bIsNagative ? -1:1;

	if (BCharToInt(pExponent)==0 && BCharToFloat(pSignificand)==0)
	{
		return 0;
	}

	if (nPower>0)
	{
		return nSign*(1+BCharToFloat(pSignificand))*(1<<nPower);
		
	}
	else
	{
		return (nSign*(1+BCharToFloat(pSignificand)))/(1<<(-nPower));
	}
}

double MakeDouble(int nType,bool bIsNagative, char * pExponent, const char * pSignificand)
{
	long nPower=(BCharToInt(pExponent) - (nType==FLOAT_TYPE ? 127: 1023));	
	int nSign= bIsNagative ? -1:1;
	

	if (BCharToInt(pExponent)==0 && BCharToDouble(pSignificand)==0)
	{
		return 0;
	}

	if (nPower>0)
	{
		return nSign*(1+BCharToDouble(pSignificand))*(1<<nPower);		
	}
	else
	{
		return (nSign*(1+BCharToDouble(pSignificand)))/(1<<(-nPower));
	}
}

int BCharToInt(const char * pchar)
{
	int nResult=0;
	int nLen=strlen(pchar);

	for (int i=0;i<nLen;i++)
	{
		nResult+=(pchar[i] - '0')*(1<<(nLen - 1 - i));
	}

	return nResult;
}

float BCharToFloat(const char * pchar)
{
	float nResult=0;
	int nLen=strlen(pchar);

	for (int i=0;i<nLen;i++)
	{
		nResult+=((float)(pchar[i] - '0'))/(1<<(i+1));
	}

	return nResult;
}

double BCharToDouble(const char * pchar)
{
	double nResult=0;
	int nLen=strlen(pchar);
	
	if(nLen>=32)
	{
		nLen=31;
	}

	for (int i=0;i<nLen;i++)
	{
		nResult+=((double)(pchar[i] - '0'))/(1<<(i+1));
	}
	
	return nResult;
}

void PrintBit(int nType,unsigned int  *pa)
{
	if (nType==FLOAT_TYPE)
	{
		PrintFloatBit(pa);
	}
	else
	{
		PrintDoubleBit(pa);
	}
}
void PrintFloatBit(unsigned int  *pa)
{
	int i;

	for (i=31;i>=0;i--)
	{
		cout<<(*pa>>i & 1)<<( i==31 || i==23 ? "-":"");		
	}

	cout<<endl;
}

void PrintDoubleBit(unsigned int  *pa)
{	
	int i;

	for (i=31;i>=0;i--)
	{
		cout<<(*(pa+1)>>i & 1)<<( i==31 || i==20 ? "-":"");		
	}	

	for (i=31;i>=0;i--)
	{
		cout<<(*pa>>i & 1);		
	}
	
	cout<<endl;
}

const char * GetExponent(char * const pDest,int nType,unsigned int  *pa)
{
	int nBegin= nType==FLOAT_TYPE ? 30:62;
	int nEnd = nType==FLOAT_TYPE ? 22: 51;

	int i;
	if (nType==FLOAT_TYPE)
	{

		for (i=nBegin;i>nEnd;i--)
		{
			pDest[nBegin - i]=(*pa>>i & 1)+'0';
		}
	}
	else
	{
		for (i=nBegin;i>nEnd;i--)
		{
			pDest[nBegin - i]=(*(pa+1)>>(i - 32) & 1)+'0';
		}
	}

	return pDest;
	
}

const char * GetSignificand(char * const pDest,int nType,unsigned int  *pa)
{
	int nBegin= nType==FLOAT_TYPE ? 22:51;
	int nLen= nType==FLOAT_TYPE ? 22:51;
	int i;
	if (nType==FLOAT_TYPE)
	{
		for (i=nBegin;i>=0;i--)
		{
			pDest[nBegin - i]=(*pa>>i & 1)+'0';
		}
		
	}
	else
	{	
		for (i=nBegin;i>=32;i--)
		{
			pDest[nBegin - i]=(*(pa+1)>>(i - 32) & 1)+'0';
		}

		for (i=31;i>=0;i--)
		{
			pDest[nBegin - i]=(*pa>>i & 1)+'0';
		}

	}

	return pDest;
}

bool IsNagative(int nType,unsigned int  *pa)
{
	unsigned int *pTemp= nType==FLOAT_TYPE ? pa: pa+1;

	return (*pTemp>>31 & 1) ==1;
}


int main()
{
	float a;

// 	a=0;	
//   	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=-11.2345612345678;	
//   	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=-1.001;
// 	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=19.2;
// 	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=1.2;
// 	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=0.000001;
// 	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);
// 
// 	a=0.0000001;
// 	PrintFloatAndDouble(FLOAT_TYPE,(unsigned int  *)&a);

	double b=19.2;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);	
	
	b=1.2;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);

	b=1.00002;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);

	b=0.00102;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);

	b=-1.912345;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);

	b=-1.9;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);
	
	b=0;
	PrintFloatAndDouble(DOUBLE_TYPE,(unsigned int  *)&b);

	return 0;
}


