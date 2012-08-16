int KillProcess(DWORD Pid)
{   
	//打开进程得到进程句柄
    HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid) ;
	if(hProcess==NULL)
	{
		CStringTool::WriteLogEx("get handle failed");
		return 0;
		
	}
	//调用TerminateProcess结束进程
    if(TerminateProcess(hProcess,0))
	{
		CStringTool::WriteLogEx("kill OK");
		return 0;
	}
    else
	{
		CStringTool::WriteLogEx("kill failed");
		return -1;
	}
	
}



DWORD GetProcessIDByName(const char * szProcessName)
{
	char buff[1024]={0};
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	//获得系统内所有进程快照
	HANDLE hProcessSnap=CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{		
		return 0;
	}
	//枚举列表中的第一个进程
	BOOL bProcess=Process32First( hProcessSnap, &pe32);
	while(bProcess)
	{
		if (_stricmp(szProcessName,pe32.szExeFile)==0)
		{
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
		}
		
		bProcess= Process32Next(hProcessSnap, &pe32);		
	}
	CloseHandle(hProcessSnap);
    return 0;
}

void CKillProcessDlg::OnbtnOK()
{
	BOOL   fOK =   FALSE;
	HANDLE hToken =   NULL;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount =1;
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes =   SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
		fOK =   (GetLastError()==ERROR_SUCCESS);
		CloseHandle(hToken);
	} 
	
	UpdateData(TRUE);

	KillProcess(GetProcessIDByName(m_strName));
	UpdateData(FALSE);
}


void CPRIDES_ControlDlg::serviceControl(const string & strServiceName)
{
    UpdateData();  
    char cmdLine[MAX_PATH];  
	wsprintf ( 
				cmdLine,
				"%s query %s",
				(CStringTool::GetCurrentDirectory()+"\\sc.exe").c_str(),
				(_strServicePrefix+strServiceName).c_str()
			 );
    SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};  
    SECURITY_ATTRIBUTES *psa=NULL;  
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if ( GetVersionEx ( &osVersion ) )  
    {  
        if ( osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT )  
        {  
            psa=&sa;  
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }  
	//根据版本设置共享模式和安全属性  
    HANDLE hConsoleRedirect=CreateFile (  
		_strTempOutFile.c_str(),  
		GENERIC_WRITE,  
		dwShareMode,  
		psa,  
		OPEN_ALWAYS,  
		FILE_ATTRIBUTE_NORMAL,  
		NULL );  
    ASSERT ( hConsoleRedirect!=INVALID_HANDLE_VALUE );  
    STARTUPINFO s={sizeof ( s ) };  
    s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;  
	//使用标准柄和显示窗口  
    s.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    s.wShowWindow =SW_HIDE;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};  
    if ( CreateProcess ( NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi ) )  
    {  
        //等待进程返回  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  

		//关闭进程和主线程句柄  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  

    }  
	//关闭控制台定向输出文件句柄 
    CloseHandle ( hConsoleRedirect );  


     
    CFile myFile ( _strTempOutFile.c_str(),CFile::modeRead );  
    ASSERT ( myFile.m_hFile!=NULL );  
    char * pszNetStatus=new char[myFile.GetLength () +1];  
    ZeroMemory ( pszNetStatus,myFile.GetLength () +1 );  
    myFile.Read ( pszNetStatus,myFile.GetLength () );  
    myFile.Close ();  
    //打开文件,把它读到一个字符缓冲区  
    DeleteFile ( _strTempOutFile.c_str());  
    //删除临时文件  
    m_EditOutPut.SetWindowText ( pszNetStatus );  
    //把控制台程序输出信息写到编辑框中  
    delete pszNetStatus;  
	UpdateData(false); 
}


int CPRIDES_ControlDlg::GetServiceState(const string & strServiceName)
{
    char cmdLine[MAX_PATH];  
	wsprintf ( 
		cmdLine,
		"%s query %s",
		(CStringTool::GetCurrentDirectory()+"\\sc.exe").c_str(),
		(_strServicePrefix+strServiceName).c_str()
		);
    SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};  
    SECURITY_ATTRIBUTES *psa=NULL;  
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if ( GetVersionEx ( &osVersion ) )  
    {  
        if ( osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT )  
        {  
            psa=&sa;  
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }  
	//根据版本设置共享模式和安全属性  
    HANDLE hConsoleRedirect=CreateFile (  
		_strTempOutFile.c_str(),  
		GENERIC_WRITE,  
		dwShareMode,  
		psa,  
		CREATE_ALWAYS,  
		FILE_ATTRIBUTE_NORMAL,  
		NULL );  
    ASSERT ( hConsoleRedirect!=INVALID_HANDLE_VALUE );  
    STARTUPINFO s={sizeof ( s ) };  
    s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;  
	//使用标准柄和显示窗口  
    s.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    s.wShowWindow =SW_HIDE;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};  
    if ( CreateProcess ( NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi ) )  
    {  
        //等待进程返回  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  
		
		//关闭进程和主线程句柄  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  
		
    }  
	//关闭控制台定向输出文件句柄 
    CloseHandle ( hConsoleRedirect );  
	
	ifstream fin(_strTempOutFile.c_str());

	if (fin.is_open()==false)
	{
		return -1;
	}

	for (string str;getline(fin,str);)
	{
		CStringTool::MakeUpper(str);
		if (str.find("RUNNING")!=string::npos)
		{
			fin.clear();
			fin.close();
			return SERVICE_RUNNING;
		}
		else if(str.find("STOPPED")!=string::npos)
		{
			fin.clear();
			fin.close();
			return SERVICE_STOPPED;
		}
		else if(str.find("START_PENDING")!=string::npos)
		{
			fin.clear();
			fin.close();
			return SERVICE_START_PENDING;
		}
	}
	fin.clear();
	fin.close();
	return SERVICE_UNKNOW;	
}


int CPRIDES_ControlDlg::OperateService(const string & strServiceName,
									   const string & strCmd,
									   CString & strState)
{
	char cmdLine[MAX_PATH];  
	wsprintf ( 
		cmdLine,
		"%s %s %s",
		(CStringTool::GetCurrentDirectory()+"\\sc.exe").c_str(),
		strCmd.c_str(),
		(_strServicePrefix+strServiceName).c_str()
		);
    SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};  
    SECURITY_ATTRIBUTES *psa=NULL;  
    DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;  
    OSVERSIONINFO osVersion={0};  
    osVersion.dwOSVersionInfoSize =sizeof ( osVersion );  
    if ( GetVersionEx ( &osVersion ) )  
    {  
        if ( osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT )  
        {  
            psa=&sa;  
            dwShareMode|=FILE_SHARE_DELETE;  
        }  
    }  
	//根据版本设置共享模式和安全属性  
    HANDLE hConsoleRedirect=CreateFile (  
		_strTempOutFile.c_str(),  
		GENERIC_WRITE,  
		dwShareMode,  
		psa,  
		CREATE_ALWAYS,  
		FILE_ATTRIBUTE_NORMAL,  
		NULL );  
    ASSERT ( hConsoleRedirect!=INVALID_HANDLE_VALUE );  
    STARTUPINFO s={sizeof ( s ) };  
    s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;  
	//使用标准柄和显示窗口  
    s.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄  
    s.wShowWindow =SW_HIDE;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0};  
    if ( CreateProcess ( NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi ) )  
    {  
        //等待进程返回  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  
		
		//关闭进程和主线程句柄  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  
		
    }  
	//关闭控制台定向输出文件句柄 
    CloseHandle ( hConsoleRedirect );  
	
	ifstream fin(_strTempOutFile.c_str());
	
	if (fin.is_open()==false)
	{
		return -1;
	}
	
	for (string str;getline(fin,str);)
	{
		CStringTool::MakeUpper(str);
		if (str.find("RUNNING")!=string::npos)
		{
			fin.clear();
			fin.close();
			strState="SERVICE_RUNNING";
			return SERVICE_RUNNING;
		}
		else if(str.find("STOPPED")!=string::npos)
		{
			fin.clear();
			fin.close();
			strState="SERVICE_STOPPED";
			return SERVICE_STOPPED;
		}
		else if(str.find("START_PENDING")!=string::npos)
		{
			fin.clear();
			fin.close();
			strState="SERVICE_START_PENDING";
			return SERVICE_START_PENDING;
		}
	}
	fin.clear();
	fin.close();
	strState="SERVICE_UNKNOW";
	return SERVICE_UNKNOW;	
}


void CPRIDES_ControlDlg::UpPrivilege(bool bEnable)
{
	//up the priviliege,otherwise the handle of service process can not be getten
	BOOL   fOK =   FALSE;
	HANDLE hToken =   NULL;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken)){
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount =1;
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes =  bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
		fOK =   (GetLastError()==ERROR_SUCCESS);
		CloseHandle(hToken);
	} 
}



int CRegisterDlg::Register(string strCmd)
{
	
	STARTUPINFO s={sizeof ( s ) };  
	s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES; //使用标准柄和显示窗口  
    s.wShowWindow =SW_HIDE;//隐藏控制台窗口  
    PROCESS_INFORMATION pi={0}; 
	
	
	if ( CreateProcess ( NULL,(char *)strCmd.c_str(),NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi ) )  
    {  
        //等待进程返回  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  
		
		//关闭进程和主线程句柄  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  
		
    }  


	if ( CreateProcess ( NULL,(char *)strCmd.c_str(),NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi ) )  
    {  
        //等待进程返回  
        WaitForSingleObject ( pi.hProcess ,INFINITE );  
		
		//关闭进程和主线程句柄  
        CloseHandle ( pi.hProcess );  
        CloseHandle ( pi.hThread );  
		
    } 
	else
	{
		AfxMessageBox("register failed");
		return -1;
	}
	return 0;
}


string CRegisterDlg::MakeRegisterCMD()
{
	string strCmd;
	strCmd=CStringTool::GetCurrentDirectory()+string("\\Sc.exe delete PRIDES-")+(LPCSTR)m_strServiceName;

	strCmd=CStringTool::GetCurrentDirectory()+string("\\Sc.exe create PRIDES-")+(LPCSTR)m_strServiceName;
	strCmd+=" binpath= \""+CStringTool::GetCurrentDirectory()+string("\\")+(LPCSTR)m_strServiceName+string(".exe ")+CStringTool::GetCurrentDirectory()+"\"";
	
	if (m_bAutomatic)
	{
		strCmd+=" start= auto";
	}

	return strCmd;
}

HANDLE	CStationInfo::hMutexWriteLog=CreateMutex(NULL,FALSE,NULL);
