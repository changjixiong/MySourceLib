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

