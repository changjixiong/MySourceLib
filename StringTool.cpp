// StringTool.cpp: implementation of the CStringTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <functional>
#include "StringTool.h"
#include <ctype.h>
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileFind	CStringTool::m_cFind;
ifstream	CStringTool:: m_ifstream;

int	CStringTool::iSeparatorPos=0;
int CStringTool::i_No=0;

long	CStringTool::m_nRLEHead=0;
long	CStringTool::m_nRLETail=0;
long	CStringTool::m_nWordNum=0;

char	CStringTool::m_szProfileString[1024]={""};
char	CStringTool::m_szGetLine[1024]={""};
char	CStringTool::sErrorMessage[1024]={""};

CString	CStringTool::m_sWordNum="";
CString	CStringTool::m_sProfileString="";	
CString CStringTool::m_sFileLineContent="";
CString	CStringTool::m_sGetLine;
CString	CStringTool::m_sFilePathWithNoPostfix;
CString	CStringTool::m_sFileNameWithNoPostfix;
CString	CStringTool::sSelectSQL="";
CString	CStringTool::m_sFilePath;
CString	CStringTool::m_sFileName;
CString	CStringTool::m_sRLETemp;
CString	CStringTool::m_sRLEWord1;
CString	CStringTool::m_sRLEWord2;
CString	CStringTool::m_sRLEWord3;

string CStringTool::m_strFilePath;
string CStringTool::m_strFileName;
string CStringTool::m_strTriggerTime;

string	CStringTool::_strLogFolder="C:";
string	CStringTool::_strLogFilePrefix="log";
CString	CStringTool::m_LogPath=_strLogFolder.c_str()+CString("\\")+_strLogFilePrefix.c_str()+CString(".log");

long CStringTool::_nLogLength=1000000;
//CString	CStringTool::m_LogPath="C:\\log.txt";

CStringTool::CStringTool()
{

}

CStringTool::~CStringTool()
{

}

void CStringTool::Split(CStringArray& sArray,CString sString,char Separator)
{
	sArray.RemoveAll();
	sString.TrimLeft();
	sString.TrimRight();
	if (sString.GetLength()<=0)
	{
		return;
	}	
	if (sString.GetLength()>0)
	{
		iSeparatorPos=sString.Find(Separator);	
		
		while(iSeparatorPos>=0)
		{
			sArray.Add(sString.Left(iSeparatorPos));
			sString=sString.Mid(iSeparatorPos+1);
			iSeparatorPos=sString.Find(Separator);
		}
		
	}	
	sArray.Add(sString);
	iSeparatorPos=0;
}

void CStringTool::Split(vector<string> & vectorReturn,const string & strSource,char Separator)
{
	vectorReturn.clear();	
	
	string::size_type nPosBegin,nPosEnd;
	string strtemp;

	if (strSource.length()<=0)
	{
		return;
	}	
		
	nPosBegin=0;
	nPosEnd=strSource.find(Separator);

	while (string::npos!=nPosEnd)
	{
		vectorReturn.push_back(strSource.substr(nPosBegin,(nPosEnd - nPosBegin)));
		nPosBegin=nPosEnd+1;
		nPosEnd=strSource.find(Separator,nPosBegin);
	}
	vectorReturn.push_back(strSource.substr(nPosBegin));
}

void CStringTool::SplitProfileString(CStringArray& sArray,char Separator,LPCSTR lpSection,LPCSTR lpKey,LPCSTR lpDefault,LPCSTR lpFileName)
{
	GetPrivateProfileString(lpSection,lpKey,lpDefault,m_szProfileString,sizeof(m_szProfileString),lpFileName);
	m_sProfileString=m_szProfileString;
	sArray.RemoveAll();
	
	if (m_sProfileString.GetLength()>0)
	{
		iSeparatorPos=m_sProfileString.Find(Separator);	
		
		while(iSeparatorPos>=0)
		{
			sArray.Add(m_sProfileString.Left(iSeparatorPos));
			m_sProfileString=m_sProfileString.Mid(iSeparatorPos+1);
			iSeparatorPos=m_sProfileString.Find(Separator);
		}
		sArray.Add(m_sProfileString);
	}	
	iSeparatorPos=0;
}

void CStringTool::GetFilePathArray(CStringArray &sArrFilePath,const CString &sFindPath,const CString sPostfix)
{
	sArrFilePath.RemoveAll();
	if (sFindPath.GetLength()>0)
	{
		if(sPostfix.GetLength()<=0)
		{
			m_cFind.FindFile(sFindPath+CString("*.*"));
		}
		else
		{
			m_cFind.FindFile(sFindPath+CString("*.")+sPostfix);
		}
		
		while(m_cFind.FindNextFile())
		{
			sArrFilePath.Add(m_cFind.GetFilePath());
		}
		sArrFilePath.Add(m_cFind.GetFilePath());

		m_cFind.Close();
	}
}

void CStringTool::GetFileNameArray(CStringArray &sArrFileName,const CString &sFindPath,const CString sPostfix)
{
	sArrFileName.RemoveAll();
	if (sFindPath.GetLength()>0)
	{
		if(sPostfix.GetLength()<=0)
		{
			m_cFind.FindFile(sFindPath+CString("*.*"));
		}
		else
		{
			m_cFind.FindFile(sFindPath+CString("*.")+sPostfix);
		}
		
		while(m_cFind.FindNextFile())
		{
			sArrFileName.Add(m_cFind.GetFileName());
		}
		sArrFileName.Add(m_cFind.GetFileName());
		m_cFind.Close();
	}
}

void CStringTool::GetFilePathAndNameArrayWithPostfix(CStringArray &sArrFilePath,
													CStringArray &sArrFileName,
													const CString &sFindPath,
													const CString sPostfix,
													const long lNameMinLength)
{
	sArrFilePath.RemoveAll();
	sArrFileName.RemoveAll();
	if (sFindPath.GetLength()>0)
	{
		if(sPostfix.GetLength()<=0)
		{
			m_cFind.FindFile(sFindPath+CString("\\*.*"));
		}
		else
		{
			m_cFind.FindFile(sFindPath+CString("\\*.")+sPostfix);
		}
		
		while(m_cFind.FindNextFile())
		{

			m_sFilePath=m_cFind.GetFilePath();
			m_sFileName=m_cFind.GetFileName();

			//如果名字是还有10月，11月，12月的，则换成A,B,C
			//类似081201001.dat，9位长度
			if (lNameMinLength>0&&m_sFileName.Find(".")<lNameMinLength)//如果指定了文件长度，aaa.dat 文件长度为3
				continue;

			if (m_sFileName.Find(".") ==9)
			{
				if (m_sFileName.Mid(2,2)==CString("10"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("A")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}else if (m_sFileName.Mid(2,2)==CString("11"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("B")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}else if (m_sFileName.Mid(2,2)==CString("12"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("C")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}
			}

			sArrFilePath.Add(m_sFilePath);
			sArrFileName.Add(m_sFileName);
			
		}
		m_sFilePath=m_cFind.GetFilePath();
		m_sFileName=m_cFind.GetFileName();

		//如果名字是还有10月，11月，12月的，则换成A,B,C
		//类似081201001.dat，9位长度
		
				

		if (lNameMinLength<=0||m_sFileName.Find(".")>=lNameMinLength)//如果指定了文件长度，aaa.dat 文件长度为3
		{
			if (m_sFileName.Find(".") ==9)
			{
				if (m_sFileName.Mid(2,2)==CString("10"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("A")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}else if (m_sFileName.Mid(2,2)==CString("11"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("B")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}else if (m_sFileName.Mid(2,2)==CString("12"))
				{
					m_sFileName=m_sFileName.Mid(0,2)+CString("C")+m_sFileName.Mid(4,m_sFileName.GetLength() - 4);
				}
			}
			
			sArrFilePath.Add(m_sFilePath);
			sArrFileName.Add(m_sFileName);
		}
		
		m_cFind.Close();
	}
}

int CStringTool::GetFilePathAndNameArrayWithNoPostfix(vector<string> &sArrFilePath,
													  vector<string> &sArrFileName,
													  const string &sFindPath,
													  const string sPostfix,
													  const long lNameMinLength,
													  bool bNeedConvertName)
{
	sArrFilePath.clear();
	sArrFileName.clear();
	if (sFindPath.length()>0)
	{
		if(sPostfix.length()<=0)
		{
			if (m_cFind.FindFile((sFindPath+("\\*.*")).c_str())==0)
			{
				return -1;
			}
			
		}
		else
		{
			if (m_cFind.FindFile((sFindPath+("\\*.")+sPostfix).c_str())==0)
			{
				return -1;
			}			
		}
		while(m_cFind.FindNextFile())
		{
			
			m_sFilePathWithNoPostfix=m_cFind.GetFilePath();
			m_sFileNameWithNoPostfix=m_cFind.GetFileTitle();
			
			m_sFilePathWithNoPostfix=m_sFilePathWithNoPostfix.Mid(0,m_sFilePathWithNoPostfix.Find('.'));
			
			//如果名字是还有10月，11月，12月的，则换成A,B,C
			//类似081201001，9位长度
			if (lNameMinLength>0&&m_sFileNameWithNoPostfix.GetLength()<lNameMinLength)//如果指定了文件长度，如aaa.dat 文件长度为3
				continue;
			
			if (bNeedConvertName&&m_sFileNameWithNoPostfix.GetLength()==9)
			{
				if (m_sFileNameWithNoPostfix.Mid(2,2)=="10")
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("A")+m_sFileNameWithNoPostfix.Mid(4,5);
				}else if (m_sFileNameWithNoPostfix.Mid(2,2)==("11"))
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("B")+m_sFileNameWithNoPostfix.Mid(4,5);
				}else if (m_sFileNameWithNoPostfix.Mid(2,2)==("12"))
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("C")+m_sFileNameWithNoPostfix.Mid(4,5);
				}
			}
			
			//if (m_sFileNameWithNoPostfix.GetLength()==X)其他情况如果要处理，则扩充之
			
			sArrFilePath.push_back((LPCSTR)m_sFilePathWithNoPostfix);
			sArrFileName.push_back((LPCSTR)m_sFileNameWithNoPostfix);
			
		}
		m_sFilePathWithNoPostfix=m_cFind.GetFilePath();
		m_sFileNameWithNoPostfix=m_cFind.GetFileTitle();
		
		m_sFilePathWithNoPostfix=m_sFilePathWithNoPostfix.Mid(0,m_sFilePathWithNoPostfix.Find('.'));
		
		//如果名字是还有10月，11月，12月的，则换成A,B,C
		//类似081201001，9位长度
		if (lNameMinLength<=0||m_sFileNameWithNoPostfix.GetLength()>=lNameMinLength)//如果指定了文件长度，如aaa.dat 文件长度为3
		{
			if (bNeedConvertName&&m_sFileNameWithNoPostfix.GetLength()==9)
			{
				if (m_sFileNameWithNoPostfix.Mid(2,2)==("10"))
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("A")+m_sFileNameWithNoPostfix.Mid(4,5);
				}else if (m_sFileNameWithNoPostfix.Mid(2,2)==("11"))
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("B")+m_sFileNameWithNoPostfix.Mid(4,5);
				}else if (m_sFileNameWithNoPostfix.Mid(2,2)==("12"))
				{
					m_sFileNameWithNoPostfix=m_sFileNameWithNoPostfix.Mid(0,2)+("C")+m_sFileNameWithNoPostfix.Mid(4,5);
				}
			}
			
			//if (m_sFileNameWithNoPostfix.GetLength()==X)其他情况如果要处理，则扩充之
			
			sArrFilePath.push_back((LPCSTR)m_sFilePathWithNoPostfix);
			sArrFileName.push_back((LPCSTR)m_sFileNameWithNoPostfix);
		}
		
		m_cFind.Close();
	}
	
	return 0;
}

int	CStringTool::GetFileInfoList(
									 vector<CFileInfo>	&vFileInfo,	
									 const	string &strFindPath,
									 const	string &strLastFileName,
									 const	string &strLastFileDate,									 
									 const	CFileConvertInfo &fileConvertInfo,
									 const	string &strDateFormat,
									 const	string &strPrefix,
									 long lNameMinLength
								)
{
	//bool bHaveNext=false;

	vFileInfo.clear();
	CFileInfo fileInfo;
	_finddata_t file;
	long h_handle;

	h_handle=_findfirst((fileConvertInfo.m_strTimeInTheFile.length()>0) ? (strFindPath+"\\*."+fileConvertInfo.m_strTimeInTheFile).c_str():(strFindPath+"\\*.dat").c_str(),
						&file);

	if (h_handle==-1L)
	{
		#ifdef _DEBUG
		WriteLog("get error no");
		sprintf(sErrorMessage,"%s _findfirst failed error no:%ul",strFindPath.c_str(),GetLastError());
		WriteLog();			
		#endif
		return -1;
 	}

	do 
	{

		#ifdef Show_judge_file
		sprintf(sErrorMessage,"judge file %s , attrib=%x",file.name,file.attrib);
		WriteLog();
		#endif

		if ((file.attrib & _A_HIDDEN)||(file.attrib & _A_SYSTEM) || (file.attrib & _A_SUBDIR))
		{
			//if the file is hidden or is a system file or is a subdir,then continue;
			continue;
		}

		if (strcmp(file.name,".")&&strcmp(file.name,".."))
		{
			/*
			string m_strFileName;//文件名
			string m_strFileNameNoPosfix;//文件名不带后缀	
			string m_strFilePrePath;//上一级路径，不带文件名	
			string m_strTriggerTime;
			string m_strTime_write;
			*/
			fileInfo.m_strFilePrePath=strFindPath;
			fileInfo.m_strFileName=file.name;
			fileInfo.m_strFileNameNoPosfix=fileInfo.m_strFileName.substr(0,fileInfo.m_strFileName.find("."));
			
			//类似081201001，9位长度
			if (lNameMinLength>0&&fileInfo.m_strFileNameNoPosfix.length()<lNameMinLength)//如果指定了文件长度，如aaa.dat 文件长度为3
				continue;
			//如果名字是还有10月，11月，12月的，则换成A,B,C

			fileInfo.m_strConvertFileName=fileInfo.m_strFileNameNoPosfix;
			if (fileConvertInfo.m_bNeedConvertFileName && fileInfo.m_strFileNameNoPosfix.length()==9)
			{
				if (fileInfo.m_strFileNameNoPosfix.substr(2,2)=="10")
				{
					fileInfo.m_strConvertFileName=fileInfo.m_strFileNameNoPosfix.substr(0,2)+("A")
													+fileInfo.m_strFileNameNoPosfix.substr(4,5);
				}else if (fileInfo.m_strFileNameNoPosfix.substr(2,2)==("11"))
				{
					fileInfo.m_strConvertFileName=fileInfo.m_strFileNameNoPosfix.substr(0,2)+("B")
													+fileInfo.m_strFileNameNoPosfix.substr(4,5);
				}else if (fileInfo.m_strFileNameNoPosfix.substr(2,2)==("12"))
				{
					fileInfo.m_strConvertFileName=fileInfo.m_strFileNameNoPosfix.substr(0,2)+("C")
													+fileInfo.m_strFileNameNoPosfix.substr(4,5);
				}
			}

			fileInfo.m_strTime_write=stringTime(file.time_write);	

			//如果前缀不符
			if (strPrefix.length()>0 && strPrefix!=fileInfo.m_strFileName.substr(0,strPrefix.length()))
			{
				continue;
			}
			
			switch (fileConvertInfo.m_nJudgeType)
			{
			case JudgeFilelastWriteTime_Name: //先判时间，再判名字

				if (fileInfo.m_strTime_write<strLastFileDate)//小于则无视
				{
					
					continue;						
				}
				else if (fileInfo.m_strTime_write==strLastFileDate)//等于
				{
					//等于则判名字
					if (fileInfo.m_strFileNameNoPosfix<=strLastFileName)//名字不大于，无视
					{
						continue;
					}
				}
				
				//大于直接处理				

				break;
			case JudgeFileName:
				if (fileInfo.m_strConvertFileName<=strLastFileName)
				{
					continue;
				}
				break;
			case JudgeFilelastWriteTimeAndName:
				break;
			default :
				continue;
				break;
			}

			if (fileConvertInfo.m_bNeedGetTriggerTime)
			{
				m_strTriggerTime="";
				if (GetTiggerTime(fileInfo.m_strFilePrePath+"\\"+fileInfo.m_strFileNameNoPosfix,
					fileConvertInfo.m_strTimeInTheFile,
					fileConvertInfo.m_nTimeInLastCharNum,
					fileConvertInfo.m_nTimeInTheLastLine,
					strDateFormat,
					fileConvertInfo.m_strDestFormat,
					m_strTriggerTime))
				{
					WriteLog((strFindPath+"\\"+fileInfo.m_strFileNameNoPosfix+" Get Tigger Time failed").c_str());
					continue;
				}
				
				fileInfo.m_strTriggerTime=m_strTriggerTime;
			}
			
			if (vFileInfo.size()>MaxNumOfComtradeFileVector)
			{				
				break;
			}

			vFileInfo.push_back(fileInfo);							
		}
		
	} while (!_findnext(h_handle,&file));

	_findclose(h_handle);
	
	switch (fileConvertInfo.m_nJudgeType)
	{
		case JudgeFilelastWriteTime_Name:
			if (vFileInfo.size()>1)
			{
				sort(vFileInfo.begin(),vFileInfo.end(),CompareFileByLastWriteTime_name);
			}			
			break;
		case JudgeFileName:
			if (vFileInfo.size()>1)
			{
				sort(vFileInfo.begin(),vFileInfo.end(),CompareFileByName);
			}
			break;
		case JudgeFilelastWriteTimeAndName:
			break;
		default :
			break;
	}		
	return 0;
}

bool CStringTool::CompareFileName(const string & str1,
									const string &str2,
									const string & strPrefix)
{
	//str1大返回真
	if (strPrefix.length()>0)//有前缀
	{
		if (str1.substr(0,strPrefix.length())==strPrefix)
		{
			return (str1>str2);
		}
		else
		{
			return false;//前缀不匹配返回假
			
		}
	}
	else
	{
		return (str1>str2);
	}

}

bool CStringTool::CompareFileByLastWriteTime_name(const CFileInfo &fileInfo1,const CFileInfo &fileInfo2)
{
	if (fileInfo1.m_strTime_write==fileInfo2.m_strTime_write)
	{
		return fileInfo1.m_strFileNameNoPosfix<fileInfo2.m_strFileNameNoPosfix;
	}
	return fileInfo1.m_strTime_write<fileInfo2.m_strTime_write;
}

bool CStringTool::CompareFileByName(const CFileInfo &fileInfo1,const CFileInfo &fileInfo2)
{
	return fileInfo1.m_strConvertFileName<fileInfo2.m_strConvertFileName;
}

bool CStringTool::CompareFinddataByName(const _finddata_t &fileInfo1,const _finddata_t &fileInfo2)
{
	return fileInfo1.name<fileInfo2.name;
}

void CStringTool::GetFileContentAddSeparator(CString &sContentReturn,
											 const CString &sFindPath,
											 const CString &sSeparator,
											 bool bRLEEncode)
{
	sContentReturn="";
	
	m_ifstream.open(sFindPath);

	if (m_ifstream.is_open())
	{
		while(m_ifstream.getline(m_sGetLine.GetBuffer(LineContentLength),LineContentLength))		
		{
			//m_sGetLine=m_szGetLine;
			m_sGetLine.ReleaseBuffer();
			m_sGetLine.TrimLeft();
			m_sGetLine.TrimRight();
			if (bRLEEncode)
			{
				RLEEncode(m_sGetLine);/*这个还没写完*/
				m_sGetLine+="\n";
			}		

			if (m_sGetLine.GetLength()<=0)
				continue;

			sContentReturn+=m_sGetLine;
			sContentReturn+=sSeparator;
		}
		m_ifstream.close();
		m_ifstream.clear();
	}	

}

void CStringTool::ArrToString(const CStringArray &sArrSource,CString &sReturn,const LPCSTR lpSeparator)
{
	sReturn="";
	for (i_No=0;i_No<sArrSource.GetSize() - 1;i_No++)
	{
		sReturn+=sArrSource[i_No];
		sReturn+=lpSeparator;
	}

	sReturn+=sArrSource[i_No];
}

void CStringTool::ArrToString(const CStringArray &sArrSource,CString &sReturn,char cSeparator)
{
	sReturn="";
	for (i_No=0;i_No<sArrSource.GetSize() - 1;i_No++)
	{
		sReturn+=sArrSource[i_No];
		sReturn+=cSeparator;
	}
	
	sReturn+=sArrSource[i_No];
}

void CStringTool::ConvertToDefaultDateTimeFormat(CString &sDataTime)
{
	//更好的函数请参看ConvertDateTime

	//这里还要处理一下，因为有两种情况 
	//11/26/05,15:19:02.267000，10/07/2009,08:10:15.951288
	//24位                       26位
	if (sDataTime.GetLength()==24)
	{
		sDataTime=CString("20")+sDataTime.Mid(6,2)+CString("-")+sDataTime.Mid(0,2)+CString("-")+sDataTime.Mid(3,2)+" "+sDataTime.Mid(9,12);
	}
	
	if (sDataTime.GetLength()==26)
	{
		sDataTime=CString("20")+
					sDataTime.Mid(sDataTime.GetLength() - 18,2)+
					CString("-")+sDataTime.Mid(3,2)+CString("-")+sDataTime.Mid(0,2)+
					" "+sDataTime.Mid(sDataTime.GetLength() - 15,12);
	}
	
}

void CStringTool::RLEEncode(CString &strSRC)
{
	strSRC.TrimLeft();
	strSRC.TrimRight();
	m_sRLETemp="";
	m_nRLEHead=0;
	m_nRLETail=0;
// 	1,0,1,-2,2,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,2,1,1       原始数据	
// 	1,0,1,-2,2,3(1),6(0),1,6(0),2,1,1                      压缩后的数据
	
	
	GetThreeWords(strSRC,m_sRLEWord1,m_sRLEWord2,m_sRLEWord3,m_nRLEHead,m_nRLETail);

	while (1)
	{
		if (m_sRLEWord1==m_sRLEWord2&&m_sRLEWord1==m_sRLEWord3)
		{
			m_nWordNum++;			
			GetOneWord(strSRC,m_sRLEWord3,m_nRLEHead,m_nRLETail);
			if (m_sRLEWord3.GetLength()<=0)
			{
				m_nWordNum+=2;
				m_sWordNum.Format("%d",m_nWordNum);
				m_sRLETemp+=m_sWordNum+"("+m_sRLEWord1+")";
				m_nWordNum=0;
				break;
			}
		}
		else
		{
			if (m_nWordNum>0)
			{
				m_nWordNum+=2;
				m_sWordNum.Format("%d",m_nWordNum);
				m_sRLETemp+=m_sWordNum+"("+m_sRLEWord1+"),";
				m_nWordNum=0;
				m_sRLEWord1=m_sRLEWord3;
				GetTwoWords(strSRC,m_sRLEWord2,m_sRLEWord3,m_nRLEHead,m_nRLETail);
				if (m_sRLEWord2.GetLength()<=0)
				{
					m_sRLETemp+=m_sRLEWord1;
					break;
				}
				else
				{
					if (m_sRLEWord3.GetLength()<=0)
					{
						m_sRLETemp+=m_sRLEWord1+",";
						m_sRLETemp+=m_sRLEWord2;
						break;
					}
				}
			}
			else
			{
				m_sRLETemp+=m_sRLEWord1+",";
				m_sRLEWord1=m_sRLEWord2;
				m_sRLEWord2=m_sRLEWord3;
				GetOneWord(strSRC,m_sRLEWord3,m_nRLEHead,m_nRLETail);
				if (m_sRLEWord3.GetLength()<=0)
				{
					m_sRLETemp+=m_sRLEWord1+",";
					m_sRLETemp+=m_sRLEWord2;
					break;
				}
			}
			
		}
	}
	strSRC=m_sRLETemp;
}

void CStringTool::RLEDecode(CString &strSRC)
{
	m_sRLETemp="";
	strSRC.TrimLeft();
	strSRC.TrimRight();
	strSRC=m_sRLETemp;
}

void CStringTool::GetThreeWords(CString &strSRC,CString &strWord1,CString &strWord2,CString &strWord3,long &nHead,long&nTail)
{
	strWord1="";
	strWord2="";
	strWord3="";
	if (strSRC.GetLength()<=0)
	{
		return;
	}

	GetOneWord(strSRC,strWord1,nHead,nTail);

	GetOneWord(strSRC,strWord2,nHead,nTail);

	GetOneWord(strSRC,strWord3,nHead,nTail);	

}

void CStringTool::GetTwoWords(CString &strSRC,CString &strWord1,CString &strWord2,long &nHead,long&nTail)
{
	strWord1="";
	strWord2="";	
	if (strSRC.GetLength()<=0)
	{
		return;
	}
	
	GetOneWord(strSRC,strWord1,nHead,nTail);
	
	GetOneWord(strSRC,strWord2,nHead,nTail);	
}

void CStringTool::GetOneWord(CString &strSRC,CString &strWord,long &nHead,long &nTail)
{
	strWord="";
	if (nHead>=strSRC.GetLength())
	{
		return;
	}

	nTail=strSRC.Find(",",nHead);
	
	if (nTail<0)
	{
		strWord=strSRC.Mid(nHead,strSRC.GetLength() - nHead);
		nHead+=strWord.GetLength()+1;
		return;
	}
	strWord=strSRC.Mid(nHead,nTail- nHead);
	nHead+=strWord.GetLength()+1;
	return;
}

CString CStringTool::vartostr(const _variant_t &var)
{
	CString strValue;
	
	switch (var.vt)
	{
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	case VT_I2://短整型
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://无符号短整型
		strValue.Format("%d", var.uiVal);
		break;
	case VT_INT://整型
		strValue.Format("%d", var.intVal);
		break;
	case VT_I4: //整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_I8: //长整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://无符号整型
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI4: //无符号整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_UI8: //无符号长整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	case VT_R4://浮点型
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://双精度型
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //小数
		strValue.Format("%.8f", (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
		}
		break;
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	case VT_BOOL://布尔型

		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //日期型
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_NULL://NULL值
		strValue = "";
		break;
	case VT_EMPTY://空
		strValue = "";
		break;
	case VT_UNKNOWN://未知类型
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}

void CStringTool::vartostr(CString &strValue,const _variant_t &var)
{
	strValue="";
	switch (var.vt)
	{
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	case VT_I2://短整型
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://无符号短整型
		strValue.Format("%d", var.uiVal);
		break;
	case VT_INT://整型
		strValue.Format("%d", var.intVal);
		break;
	case VT_I4: //整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_I8: //长整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://无符号整型
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI4: //无符号整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_UI8: //无符号长整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	case VT_R4://浮点型
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://双精度型
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //小数
		strValue.Format("%.8f", (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
		}
		break;
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	case VT_BOOL://布尔型

		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //日期型
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_NULL://NULL值
		strValue = "";
		break;
	case VT_EMPTY://空
		strValue = "";
		break;
	case VT_UNKNOWN://未知类型
	default:
		strValue = "UN_KNOW";
		break;
	}
	
}


void CStringTool::vartol(long &lValue,const _variant_t& var)
{
	lValue = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		lValue = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		lValue = var.bVal;
		break;
	case VT_UI2:
	case VT_I2:
		lValue = var.iVal;
		break;
	case VT_I4:
	case VT_UI4:
		lValue = var.lVal;
		break;
	case VT_INT:
		lValue = var.intVal;
		break;
	case VT_R4:
		lValue = (long)(var.fltVal + 0.5);
		break;
	case VT_R8:
		lValue = (long)(var.dblVal + 0.5);
		break;
	case VT_DECIMAL:
		lValue = (long)var;
		break;
	case VT_CY:
		lValue = (long)var;
		break;
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		lValue = atol((LPCTSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		lValue = 0;
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}

}

void CStringTool::ShowErrorMessage(HWND ParenthWnd)
{
	MessageBox(ParenthWnd,sErrorMessage,"错误",MB_OK);
}

void CStringTool::ShowErrorMessage()
{
	AfxMessageBox(sErrorMessage);	
}

int	CStringTool::WriteLogEx(const string & strContent)
{
	return WriteLog(strContent,_nLogLength);
}

int CStringTool::WriteLog()
{
	//成功返回0	
// 	FILE* log=NULL;
// 	log = fopen((LPCTSTR)m_LogPath, "a+");
// 	
// 	if (log==NULL)
// 	{
// 		return -1;
// 	}
	
//	CTime Currenttime=CTime::GetCurrentTime();	
//	CString strCurrenttime=Currenttime.Format("%Y-%m-%d %H:%M:%S");	
// 	fprintf(log,"%s    ",(LPCTSTR)strCurrenttime);
// 	fprintf(log,"%s\n\n",sErrorMessage);
// 	fclose(log);
// 	memset(sErrorMessage,0,1024);
// 	return 0;
	return WriteLog(sErrorMessage,_nLogLength);
}

int CStringTool::WriteLog(const string & strContent,long nMaxSize)
{
	FILE* log=NULL;
	log = fopen((LPCTSTR)m_LogPath, "a+");

	if (log==NULL)
	{
		return -1;
	}

	fseek(log,0,SEEK_END);
	long len=ftell(log);
	fseek(log,0,SEEK_SET);	
	fclose(log);

	if (len>0 && len+strContent.length()>nMaxSize)
	{		
		SetLogFolder_FilePrefix(_strLogFolder,_strLogFilePrefix);
// 		log = fopen((LPCTSTR)m_LogPath, "a+");
// 		if (log==NULL)
// 		{
// 			return -1;
// 		}
	}
	
	WriteLog(strContent);
	
	return 0;
}

int CStringTool::WriteLog(const string &sContent)
{
	//成功返回0
	FILE* log=NULL;
	log = fopen((LPCTSTR)m_LogPath, "a+");
	
	if (log==NULL)
	{
		return -1;
	}
	
	CTime Currenttime=CTime::GetCurrentTime();	
	
// 	fprintf(log,"%04d-%02d-%02d %02d:%02d:%02d    ",
// 		Currenttime.GetYear(),
// 		Currenttime.GetMonth(),
// 		Currenttime.GetDay(),
// 		Currenttime.GetHour(),
// 		Currenttime.GetMinute(),
// 		Currenttime.GetSecond());
	
	fprintf(log,"%s    %s\n\n",CurrentTime().c_str(),sContent.c_str());
	fclose(log);
	return 0;
}


void CStringTool::trim(CString & strReturn)
{
	strReturn.TrimRight();
	strReturn.TrimLeft();
}

// void CStringTool::SetLogPath(CString & strpath)
// {
// 	m_LogPath=strpath;
// }



void CStringTool::SetLogFolder_FilePrefix(const string & strLogFolder,const string & strFilePrefix)
{
	if (strLogFolder.length()>0&&strFilePrefix.length()>0)
	{
		_strLogFolder=strLogFolder;
		_strLogFilePrefix=strFilePrefix;
		m_LogPath=strLogFolder.c_str();
		if (strLogFolder[strLogFolder.length() - 1]!='\\')
		{
			m_LogPath+="\\";
		}
		
		m_LogPath+=strFilePrefix.c_str();

		time_t t_now;
		time(&t_now);
		CTime csourcetime(t_now);
		
		char sztime[24];
		sprintf(sztime,"%d%02d%02d%02d%02d%02d",
			csourcetime.GetYear(),
			csourcetime.GetMonth(),
			csourcetime.GetDay(),
			csourcetime.GetHour(),
			csourcetime.GetMinute(),
			csourcetime.GetSecond());

		m_LogPath+=sztime;
		m_LogPath+=".log";

	}

}


// void CStringTool::SetLogPath(const char * lpPath)
// {
// 	m_LogPath=lpPath;
// }

// int	CStringTool::JudgeDir(LPCSTR lpPath,bool bCreate)
// {
// 
// 	//成功返回0
// 	if (_access(lpPath,0)!=0)//不存在
// 	{
// 		if (bCreate)
// 		{
// 			return _mkdir(lpPath);			
// 		}
// 		else
// 		{
// 			return -1;
// 		}
// 		
// 	}
// 	else
// 	{
// 		return 0;//存在
// 	}
// }

int	CStringTool::JudgeDir(const string & lpPath,bool bCreate)
{
	int pos=0;
	pos=lpPath.find("\\");
	if (-1==pos)
	{
		return -1;
	}
	
	while (pos>0)
	{
		if (_access(lpPath.substr(0,pos).c_str(),0)!=0)
		{
			if (bCreate)
			{
				if (_mkdir(lpPath.substr(0,pos).c_str()))
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		
		pos=lpPath.find("\\",pos+1);
	}

	if (_access(lpPath.c_str(),0)!=0)
	{
		if (bCreate)
		{
			return _mkdir(lpPath.c_str());
		}
		else
		{
			return -1;
		}
	}
		
	return 0;
}

int CStringTool::GetPathList(const string & strDir,
							 const string & strFolder,
							 const char * lpFileFilter,
							 unsigned int nAttrib,
							 vector<_finddata_t> & vectorReturn)
{
	string strPath;
	_finddata_t file;
	long h_handle;
	
	vectorReturn.clear();
	
	strPath=strDir;
	if (strFolder.length()>0)
	{
		strPath+="\\";
		strPath+=strFolder;
	}
	
	strPath+="\\";
	strPath+=lpFileFilter;
	
	#ifdef find_file_folder_in
		WriteLog("find file&folder in "+strPath);
	#endif
	
	h_handle=_findfirst(strPath.c_str(),&file);
	
	if (h_handle==-1L)
	{
	#ifdef _DEBUG
			WriteLog("get error no");
			sprintf(sErrorMessage,"%s _findfirst failed error no:%ul",strPath.c_str(),GetLastError());
			WriteLog();			
	#endif
		return -1;
	}	
	
	#ifdef Show_search_in_path
		sprintf(sErrorMessage,"search in path: %s",strPath.c_str());
		WriteLog();
	#endif
	
	do 
	{
		
	#ifdef Show_judge_file
			sprintf(sErrorMessage,"judge file %s , attrib=%x",file.name,file.attrib);
			WriteLog();
	#endif

		if ((file.attrib & _A_HIDDEN)||(file.attrib & _A_SYSTEM))
		{
			//if the file is hidden or is a system file ,then continue;
			continue;
		}
		if (!(file.attrib & nAttrib))
		{
			continue;
		}
		
		if (strcmp(file.name,".")&&strcmp(file.name,".."))
		{
			
		#ifdef Show_fileName_push
					WriteLog((string(file.name)+" push into vector").c_str());
		#endif
			
			vectorReturn.push_back(file);			
		}
		
	} while (!_findnext(h_handle,&file));
	
	_findclose(h_handle);
	
	return 0;	
}

int CStringTool::GetPathListNum(const string & strDir,
							 const string & strFolder,
							 const char * lpFileFilter,
							 unsigned int nAttrib)
{
	string strPath;
	string strFileName;
	_finddata_t file;
	long h_handle;
	UINT nNum=0;
	
	strPath=strDir;
	if (strFolder.length()>0)
	{
		strPath+="\\";
		strPath+=strFolder;
	}
	
	strPath+="\\";
	strPath+=lpFileFilter;
	
#ifdef _DEBUG
	WriteLog("find file&folder in "+strPath);
#endif
	
	h_handle=_findfirst(strPath.c_str(),&file);
	
	if (h_handle==-1L)
	{
#ifdef _DEBUG
		WriteLog("get error no");
		sprintf(sErrorMessage,"%s _findfirst failed error no:%ul",strPath.c_str(),GetLastError());
		WriteLog();			
#endif
		return -1;
	}
	
	
#ifdef Show_search_in_path
	sprintf(sErrorMessage,"search in path: %s",strPath.c_str());
	WriteLog();
#endif
	
	do 
	{
		
// #ifdef _DEBUG
// 		sprintf(CStringTool::sErrorMessage,"judge file %s , attrib=%x",file.name,file.attrib);
// 		CStringTool::WriteLog();
// #endif
		
		if ((file.attrib & _A_HIDDEN)||(file.attrib & _A_SYSTEM))
		{
			//if the file is hidden or is a system file ,then continue;
			continue;
		}
		
		if (strcmp(file.name,".")&&strcmp(file.name,".."))
		{
			
			
// #ifdef _DEBUG
// 			CStringTool::WriteLog((strFileName+" push into vector").c_str());
// #endif
			
			nNum++;
			
		}
		
	} while (!_findnext(h_handle,&file));
	
	_findclose(h_handle);
	
	return nNum;	
}

// int CStringTool::GetFolderList(const string & strDir,
// 								const string & strFolder,							 
// 								vector<string> & vectorReturn)
// {
// 
// 	#ifdef Show_GetFolderList_dir_folder
// 		sprintf(CStringTool::sErrorMessage,"GetFolderList dir: %s ,folder: %s ",strDir.c_str(),strFolder.c_str());
// 		CStringTool::WriteLog();
// 	#endif
// 
// 	return	GetPathList(strDir,strFolder,"*",vectorReturn,_A_SUBDIR);
// 
// }

int CStringTool::GetFolderList(const string & strDir,
							   const string & strFolder,
							   unsigned int nAttrib,
							   vector<_finddata_t> & vectorReturn)
{
	
#ifdef Show_GetFolderList_dir_folder
	sprintf(sErrorMessage,"GetFolderList dir: %s ,folder: %s ",strDir.c_str(),strFolder.c_str());
	WriteLog();
#endif
	
	return	GetPathList(strDir,strFolder,"*",nAttrib,vectorReturn);
	
}

int CStringTool::GetFileList(const string & strDir,
							 const string & strFolder,
							 const char * lpFileFilter,
							 unsigned int nAttrib,
							 vector<_finddata_t> & vectorReturn)
{
	return	GetPathList(strDir,strFolder,lpFileFilter,nAttrib,vectorReturn);
}



int CStringTool::GetFileListNum(const string & strDir,
							 const string & strFolder,
							 const char * lpFileFilter)
{
	return	GetPathListNum(strDir,strFolder,lpFileFilter,0X00);
}

int CStringTool::DeleteFileNameFromList(vector<_finddata_t> & vecSource,vector<string> & vecDeleteList)
{
	if (vecSource.size()==0||vecDeleteList.size()==0)
	{
		return 0;
	}
	for (int i=0;i<vecDeleteList.size();++i)
	{
		DeleteFileNameFromList(vecSource,vecDeleteList[i]);
	}
	return 0;
}

int CStringTool::DeleteFileNameFromList(vector<_finddata_t> & vecSource,string & strDeleteFileName)
{
	if (strDeleteFileName.size()<=0)
	{
		return 0;
	}

	vector<_finddata_t>::iterator iterfind;

	for (iterfind=vecSource.begin();iterfind<vecSource.end();iterfind++)
	{
		if (strDeleteFileName==(*iterfind).name)
		{
			vecSource.erase(iterfind);
		}
		return 0;
	}

	return 0;
}

int	CStringTool::GetINIValue(const char * lpFileName,
							const char * lpSection,
							const char * lpKey,
							string & strReturn)
{
	char szValue[512];	
	GetPrivateProfileString(lpSection,lpKey,"",szValue,512,lpFileName);
	
	if (strlen(szValue)==0)
	{
		return -1;
	}
	strReturn=szValue;
	memset(szValue,0,512);
	return 0;
}

int	CStringTool::GetINIValue(string & strReturn,
							const char * lpFileName,
							const char * lpSection,
							const char * lpKey,
								bool bNeedSysPath)
{
	char szValue[4096];	
	memset(szValue,0,4096);
	strReturn="";
	if (bNeedSysPath)
	{
		char szSysPath[512];
		memset(szSysPath,0,512);
		::GetCurrentDirectory(512,szSysPath);
		GetPrivateProfileString(lpSection,lpKey,"",szValue,4096,(szSysPath+string("\\")+lpFileName).c_str());
	}
	else
	{
		GetPrivateProfileString(lpSection,lpKey,"",szValue,4096,lpFileName);
	}
	
	if (strlen(szValue)==0)
	{
		return -1;
	}
	strReturn=szValue;
	memset(szValue,0,4096);
	return 0;
}

int	CStringTool::GetINIValue(CString & cstrReturn,
							 const char * lpFileName,
							 const char * lpSection,
							 const char * lpKey,
							 bool bNeedSysPath)
{
	string strValue;
	cstrReturn="";
	GetINIValue(strValue,lpFileName,lpSection,lpKey,bNeedSysPath);
	
	if (strValue.length()==0)
	{
		return -1;
	}
	cstrReturn=strValue.c_str();

	return 0;
}


int	CStringTool::GetINIValue(const char * lpFileName,
							 const char * lpSection,
							 const char * lpKey,
							 char Separator,
							vector<string> & vecReturn)
{
	string strValue;
	GetINIValue(lpFileName,lpSection,lpKey,strValue);
	
	Split(vecReturn,strValue,Separator);

	return 0;
}

int	CStringTool::GetINIValue(const char * lpFileName,
							const char * lpSection,
							const char * lpKey,
							long & nReturn)
{
	char szValue[100];	
	GetPrivateProfileString(lpSection,lpKey,"",szValue,100,lpFileName);
	
	if (strlen(szValue)==0)
	{
		return -1;
	}
	nReturn=atol(szValue);	
	memset(szValue,0,100);
	return 0;
}

int	CStringTool::GetINIValue(const char * lpFileName,
							 const char * lpSection,
							 const char * lpKey,
							 int & nReturn)
{
	char szValue[100];	
	GetPrivateProfileString(lpSection,lpKey,"",szValue,100,lpFileName);
	
	if (strlen(szValue)==0)
	{
		return -1;
	}
	nReturn=atoi(szValue);	
	memset(szValue,0,100);
	return 0;
}

int CStringTool::ConvertDateTime(const string & strSourceFormat,
									const string & strSourceDateTime,									
									const string & strDestFormat,
									string &strReturn)
{
	if (strSourceDateTime.length()==0)
	{
		strReturn="";
		return -1;
	}
	strReturn=strDestFormat;	

	if (Replace(strSourceFormat,strSourceDateTime,strDestFormat,'Y',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'M',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'D',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'h',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'m',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'s',strReturn)
		||Replace(strSourceFormat,strSourceDateTime,strDestFormat,'f',strReturn))
	{
		strReturn="";
		return -1;
	}
	else
	{
		return 0;
	}	
}

int CStringTool::Replace(const string & strSourceFormat,
						 const string & strSourceDateTime,									
						 const string & strDestFormat,
						 char key,
						 string &strReturn)
{
	string::size_type nposBeginSource=0,nposEndSource=0;
	string::size_type nposBeginDest=0,nposEndDest=0;

	string strTemp;
	int nTempLenSource=0;
	int nTempLenDest=0;

	//1 处理年
	
	nposBeginSource=strSourceFormat.find(key);	
	nposEndSource=strSourceFormat.find_last_of(key);	
	if (nposBeginSource==string::npos||nposEndSource==string::npos)
	{
		return -1;
	}
	
	nTempLenSource= nposEndSource - nposBeginSource + 1;

	nposBeginDest=strDestFormat.find(key);	
	nposEndDest=strDestFormat.find_last_of(key);	
	if (nposBeginDest==string::npos||nposEndDest==string::npos)
	{
		return -1;
	}
	
	nTempLenDest = nposEndDest - nposBeginDest + 1;

	strTemp=strSourceDateTime.substr(nposBeginSource, nTempLenSource);

	switch (key)
	{
	case 'Y'://year
		switch (strTemp.length())
		{
		case 2:
			strTemp=(nTempLenDest==4 ? "20"+strTemp : strTemp);
			break;
		case 4:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 'M':
		switch (strTemp.length())
		{
		case 1:
			strTemp=(nTempLenDest==2 ? "0"+strTemp : strTemp);
			break;
		case 2:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 'D':
		switch (strTemp.length())
		{
		case 1:
			strTemp=(nTempLenDest==2 ? "0"+strTemp : strTemp);
			break;
		case 2:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 'h':
		switch (strTemp.length())
		{
		case 1:
			strTemp="0"+strTemp;
			break;
		case 2:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 'm':
		switch (strTemp.length())
		{
		case 1:
			strTemp="0"+strTemp;
			break;
		case 2:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 's':
		switch (strTemp.length())
		{
		case 1:
			strTemp="0"+strTemp;
			break;
		case 2:
			break;
		default:
			return -1;
			break;
		}
		break;
	case 'f':
		switch (strTemp.length())
		{
		case 1:
			strTemp="00"+strTemp;
			break;
		case 2:
			strTemp="0"+strTemp;
			break;
		case 3:
			break;
		case 6:
			break;
		default:
			return -1;
			break;
		}
		break;	
	default:
		return -1;
		break;
	}

	if (key=='f' && strTemp.length()>nTempLenDest )
	{
		strTemp=strTemp.substr(0,nTempLenDest);
	}
	
	strReturn.replace(nposBeginDest,
						nposEndDest - nposBeginDest + 1, 
						strTemp);
	
	
	return 0;

}

bool CStringTool::IsWhiteSpace(char cSrc)
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

string& CStringTool::lTrim(string	&ss)   
{   
	string::iterator	p=find_if(ss.begin(),ss.end(),not1(ptr_fun(isspace)));   
	ss.erase(ss.begin(),p);   	

	return  ss;   
}   

string& CStringTool::rTrim(string	&ss)   
{   
	string::reverse_iterator	p=find_if(ss.rbegin(),ss.rend(),not1(ptr_fun(isspace)));   
	ss.erase(p.base(),ss.end());   
	return	ss;   
}   

string& CStringTool::trim(string	&st)   
{   
	lTrim(rTrim(st));   
	return   st;   
}  

string CStringTool::GetLineBackwards(const string & strSource,long nLineNum)
{
	
	if (nLineNum<=0)
	{
		return "";
	}
	
	long	nPosEnd=0;
	long	nPosBegin=0;
	long	nCarriageReturnPos=0;	
	vector<int> vecCarriageReturnPos;//回车的下标
	
	nPosEnd=strSource.length()-1;		
	
	while (IsWhiteSpace(strSource[nPosEnd]))//跳过尾空白字符
	{
		nPosEnd--;
	}
	
	nCarriageReturnPos=strSource.rfind("\r\n",nPosEnd);	
	
	while ((vecCarriageReturnPos.size()<nLineNum) && nCarriageReturnPos>0)
	{	
		
		vecCarriageReturnPos.push_back(nCarriageReturnPos);
		nCarriageReturnPos=strSource.rfind("\r\n",nCarriageReturnPos - 1);
	}
	
	if (nLineNum>vecCarriageReturnPos.size()+1)
	{
		return "";
	}
	
	if (nLineNum==vecCarriageReturnPos.size()+1)
	{
		nPosBegin=0;
	}
	else
	{
		if (vecCarriageReturnPos.size()==0)
		{
			return "";
		}
		nPosBegin=vecCarriageReturnPos[vecCarriageReturnPos.size() - 1]+2;
	}

	
	if (nLineNum>1)//
	{
		if (nLineNum==vecCarriageReturnPos.size()+1)
		{
			if (vecCarriageReturnPos.size()==0)
			{
				return "";
			}
			nPosEnd=vecCarriageReturnPos[vecCarriageReturnPos.size() - 1] - 1;
		}
		else
		{
			if (vecCarriageReturnPos.size()<2)
			{
				return "";
			}
			nPosEnd=vecCarriageReturnPos[vecCarriageReturnPos.size() - 2] - 1;
		}		
	}
	vecCarriageReturnPos.clear();
	return strSource.substr(nPosBegin,nPosEnd - nPosBegin + 1);
}

string&  CStringTool::MakePath(string & strReturn,
						  const string & strDir , 
						  const string & strFolder , 
						  const string & strFileName,
						  const string & strPosfix)
{
			strReturn=strDir
			+(	strFolder.length()>0 ? "\\"+strFolder : "")			
			+(	strFileName.length()>0 ? 
					(strPosfix.length()>0 ? "\\"+strFileName+"."+strPosfix : "\\"+strFileName ) 
					: "");
			

	 return strReturn;
	
}

string  CStringTool::MakePath( const string & strDir , 
							   const string & strFolder , 
							   const string & strFileName,
							   const string & strPosfix)
{
	string strReturn;
	strReturn=strDir
		+(	strFolder.length()>0 ? "\\"+strFolder : "")			
		+(	strFileName.length()>0 ? 
		(strPosfix.length()>0 ? "\\"+strFileName+"."+strPosfix : "\\"+strFileName ) 
		: "");
	
	
	return strReturn;
	
}

string& CStringTool::MakeUpper(string & strSrc)
{
	for (int i=0;i<strSrc.length();strSrc[i]=toupper(strSrc[i]),++i);
	
	return strSrc;
}

int	CStringTool::GetTiggerTime(const string & sPath,
						  const string & sFileType,
						  const long & nDateInTheLastCharNum,
						  const long & nDateInTheLastLine,
						  const string & strDateTimeSourceForMat,
						  const string & strDateTimeDestForMat,
						  string & sReturn)
{
	
	long nFileLen=0;
	string strSource;
	
	char * pstrTime=new char[nDateInTheLastCharNum];
	FILE *fp=NULL;
	
	memset(pstrTime,0,nDateInTheLastCharNum);
	fp=fopen((sPath+"."+sFileType).c_str(),"rb");
	
	if (fp==NULL)
	{
		return -1;
	}
	
	fseek(fp,0,SEEK_END);
	nFileLen=ftell(fp);
	
	fseek(fp,((nFileLen - nDateInTheLastCharNum +1)>0 ? nFileLen - nDateInTheLastCharNum+1 : 0),SEEK_SET);
	fread(pstrTime,1,((nFileLen - nDateInTheLastCharNum +1)>0 ? nDateInTheLastCharNum - 1 : nFileLen),fp);	
	fclose(fp);
		
	strSource=CStringTool::GetLineBackwards(pstrTime,nDateInTheLastLine);
	
	DateStringFix(strSource);
	CStringTool::trim(strSource);

	CStringTool::ConvertDateTime(strDateTimeSourceForMat,
								strSource,
								strDateTimeDestForMat,
								sReturn);	

	if (sReturn.length()==0)
	{
		return -1;
	}
	
	delete [] pstrTime;
	pstrTime=NULL;
	return 0;
}

string CStringTool::AddTailToFileName(const string & strSource,const char * szTail)
{
	string strReturn;
	UINT npos=0;

	if (strlen(szTail)<=0)
	{
		return strSource;
	}

	npos=strSource.find(".");

	if (npos>0)
	{
		strReturn=strSource.substr(0,npos)+szTail+strSource.substr(npos,strSource.length() - npos);
	}
	else
	{
		strReturn=strSource+szTail;
	}

	return strReturn;

}

string CStringTool::CutSubString(const string & strSource,const char * szSubString)
{
	string strReturn;
	UINT npos=0;
	
	if (strlen(szSubString)<=0)
	{
		return strSource;
	}
	
	npos=strSource.find(szSubString);
	
	if (npos>0)
	{
		strReturn=strSource.substr(0,npos)+strSource.substr(npos+strlen(szSubString),strSource.length() - npos - strlen(szSubString));
	}
	
	return strReturn;	
}

int CStringTool::CheckFiles(const string & strPath,vector<string> & vecFileType,char Separator)
{
	int npos=0;
	if (Separator)
	{
		npos=strPath.find(Separator);
	}	

	for (int i=0;i<vecFileType.size();i++)
	{
		if (npos>0)
		{
			if (CheckFile(strPath.substr(0,npos)+"."+vecFileType[i]))
			{
				return -1;
			}			
		}
		else
		{
			if (CheckFile(strPath+"."+vecFileType[i]))
			{
				return -1;
			}
		}
	}

	return 0;
}

int CStringTool::CheckFile(const string & strPath,int nMode)
{
	if (_access(strPath.c_str(),nMode)==-1)
	{
		#ifdef _DEBUG
				sprintf(CStringTool::sErrorMessage,"access file %s faild",strPath.c_str());
				WriteLog();
		#endif

		return -1;		
	}
	return 0;
}


string CStringTool::GetOneWord(const string &sOrigin,int nBegin)
{
	string sOneWord="";
	int nposEnd=0;
	
	if (nBegin>=sOrigin.length())
	{
		return sOneWord;
	}	

	//先把空白字符和逗号跳过 这里主要是要定位到一个有效列的第一个字符
	while(nBegin<sOrigin.length() && IsWhiteSpace(sOrigin[nBegin]))
	{
		nBegin++;
	}
	
	nposEnd=nBegin;

	while(nposEnd<sOrigin.length())
	{
		//空白字符或者逗号,结束
		if (IsWhiteSpace(sOrigin[nposEnd]))
		{
			break;
		}
		nposEnd++;
	}	

	sOneWord=sOrigin.substr(nBegin,nposEnd-nBegin);
	
	return sOneWord;
}

string&   CStringTool::replace_all_distinct(string& str , const string& strOld , const string& strNew)   
{   
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=strNew.length())   {   
        if(   (pos=str.find(strOld,pos))!=string::npos   )   
            str.replace(pos,strOld.length(),strNew);   
        else   break;   
    }   
    return   str;   
}


string CStringTool::CurrentTime(long nLen)
{
	SYSTEMTIME UniversalTime;
	GetSystemTime(&UniversalTime);

	SYSTEMTIME ti;
	memset(&ti, 0, sizeof(SYSTEMTIME));
	
	TIME_ZONE_INFORMATION zinfo;
	GetTimeZoneInformation(&zinfo);
	
	SystemTimeToTzSpecificLocalTime(&zinfo,&UniversalTime, &ti);//
	char sztime[24];
	sprintf(sztime,"%d-%02d-%02d %02d:%02d:%02d.%03d",ti.wYear,ti.wMonth,ti.wDay,ti.wHour,ti.wMinute,ti.wSecond,ti.wMilliseconds);
	string strCurrentTime=sztime;
	if (nLen>0 && nLen<strCurrentTime.length())
	{
		strCurrentTime=strCurrentTime.substr(0,nLen);
	}
	return strCurrentTime;
}

bool CStringTool::GMTConvertToLocalTime(string & strTime)
{
	SYSTEMTIME UniversalTime;
	memset(&UniversalTime, 0, sizeof(SYSTEMTIME));
	//2009-09-09 21:01:01.999
	try
	{
		UniversalTime.wYear=atoi(strTime.substr(0,4).c_str());
		UniversalTime.wMonth=atoi(strTime.substr(5,2).c_str());
		UniversalTime.wDay=atoi(strTime.substr(8,2).c_str());
		UniversalTime.wHour=atoi(strTime.substr(11,2).c_str());
		UniversalTime.wMinute=atoi(strTime.substr(14,2).c_str());
		UniversalTime.wSecond=atoi(strTime.substr(17,2).c_str());
		if (strTime.length()==23)
		{
			UniversalTime.wMilliseconds=atoi(strTime.substr(20,3).c_str());
		}		
	}
	catch (CMemoryException* e)
	{
		return false;
	}		
	
	SYSTEMTIME ti;
	memset(&ti, 0, sizeof(SYSTEMTIME));
	
	TIME_ZONE_INFORMATION zinfo;
	GetTimeZoneInformation(&zinfo);
	
	SystemTimeToTzSpecificLocalTime(&zinfo,&UniversalTime, &ti);//
	char sztime[24];
	sprintf(sztime,"%d-%02d-%02d %02d:%02d:%02d.%03d",ti.wYear,ti.wMonth,ti.wDay,ti.wHour,ti.wMinute,ti.wSecond,ti.wMilliseconds);
	strTime=sztime;

	return true;
}


string CStringTool::stringTime(time_t sourcetime)
{
	CTime csourcetime(sourcetime);
	
	char sztime[24];
	sprintf(sztime,"%d-%02d-%02d %02d:%02d:%02d",
			csourcetime.GetYear(),
			csourcetime.GetMonth(),
			csourcetime.GetDay(),
			csourcetime.GetHour(),
			csourcetime.GetMinute(),
			csourcetime.GetSecond());

	return sztime;
}


int CStringTool::DateStringFix(string & strReturn)//日期修正
{
	DateStringFix(strReturn,"-");
	DateStringFix(strReturn,"/");
	return 0;
}

int CStringTool::DateStringFix(string & strReturn,const string & strSeparate)
{
	string::size_type pos1,pos2;

	pos1=strReturn.find(strSeparate);
	if (pos1!=string::npos)
	{
		if (1==pos1)
		{
			strReturn="0"+strReturn;
			pos1++;			
		}
		
		pos2=strReturn.find(strSeparate,pos1+1);
		
		if (2==pos2 - pos1)
		{
			strReturn=strReturn.substr(0,pos1+1)+"0"+strReturn.substr(pos1+1);
		}

	}
	return 0;
}

bool CStringTool::IsDate(const string & strDate)
{
	//2009*09*09

	if (strDate.length()!=10)
	{
		return false;
	}
 
	if (isdigit(strDate[0])
		&&isdigit(strDate[1])
		&&isdigit(strDate[2])
		&&isdigit(strDate[3])
		&&isdigit(strDate[5])
		&&isdigit(strDate[6])
		&&isdigit(strDate[8])
		&&isdigit(strDate[9])
		)
	{
		return true;
	}

	return false;
}

bool CStringTool::IsTime(const string & strTime)
{
	//21:01:01.999  or 21:01:01
	
	if (strTime.length()!=12 && strTime.length()!=9)
	{
		return false;
	}
	
	if (isdigit(strTime[0])
		&&isdigit(strTime[1])
		&&isdigit(strTime[3])
		&&isdigit(strTime[4])
		&&isdigit(strTime[6])
		&&isdigit(strTime[7]))
	{
		if (strTime.length()==12 && isdigit(strTime[9]) && isdigit(strTime[10]) && isdigit(strTime[11]))
		{
			return true;
		}
		else
		{
			return true;
		}
		
	}
	
	return false;
}

void CStringTool::SortByNameASC(vector<_finddata_t> vecFile)
{
	if (vecFile.size()<=0)
	{
		return;
	}
	sort(vecFile.begin(),vecFile.end(),CompareFinddataByName);
}

int CStringTool::CopyFile(const string & strDest,const string & strSrc)
{
	try
	{
		
		ifstream input(strSrc.c_str(),ios::binary);
		ofstream output(strDest.c_str(),ios::binary|ios::trunc);
		
		output<<input.rdbuf();
		
		input.close();
		output.close();
	}
	catch (CMemoryException* e)
	{
		return -1;	
	}
	catch (CFileException* e)
	{
		return -1;
	}
	catch (CException* e)
	{
		return -1;
	}

	return 0;
}

string CStringTool::GetCurrentDirectory()
{
	char szSysPath[512];
	memset(szSysPath,0,512);
	::GetCurrentDirectory(512,szSysPath);
	
	string strCurrentDirecrtory;
	strCurrentDirecrtory=szSysPath;

	return strCurrentDirecrtory;
}

bool CStringTool::TimeFromString(SYSTEMTIME &sysTm,string strTime)
{
	memset(&sysTm, 0, sizeof(SYSTEMTIME));
	//2009-09-09 21:01:01.999
	try
	{
		sysTm.wYear=atoi(strTime.substr(0,4).c_str());
		sysTm.wMonth=atoi(strTime.substr(5,2).c_str());
		sysTm.wDay=atoi(strTime.substr(8,2).c_str());
		sysTm.wHour=atoi(strTime.substr(11,2).c_str());
		sysTm.wMinute=atoi(strTime.substr(14,2).c_str());
		sysTm.wSecond=atoi(strTime.substr(17,2).c_str());
		if (strTime.length()==23)
		{
			sysTm.wMilliseconds=atoi(strTime.substr(20,3).c_str());
		}		
	}
	catch (CMemoryException* e)
	{
		return false;
	}	

	return true;
	
}
