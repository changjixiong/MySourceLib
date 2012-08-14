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
