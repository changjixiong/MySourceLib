HANDLE hThread;
DWORD ThreadID;

hThread=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFunc,
		m_pFCarryer,
		0,
		&ThreadID);

void ThreadFunc(LPVOID lpParam)
{
	CoInitialize(NULL);
	CMFuncFCarryer *pFCarryer=(CMFuncFCarryer *)lpParam;
	while(m_bRun)
	{
		pFCarryer->CarryFileForConvert();
		Sleep(500);
	}
	CoUninitialize();
}



void CSATMQDlg::OnButtonstop() 
{
	// TODO: Add your control notification handler code here
	//KillTimer(1);
	g_bRun=false;
	DWORD nExitCode;
	while(1)
	{
		if (handleAlarmSend!=INVALID_HANDLE_VALUE)
		{
			GetExitCodeThread(handleAlarmSend,&nExitCode);
			if (nExitCode==STILL_ACTIVE)
			{
				continue;
			}
		}
		
		if (handleAlarmGenerate!=INVALID_HANDLE_VALUE)
		{
			GetExitCodeThread(handleAlarmGenerate,&nExitCode);		
			if (nExitCode==STILL_ACTIVE)
			{
				continue;
			}
		}		

		break;
	}
}

