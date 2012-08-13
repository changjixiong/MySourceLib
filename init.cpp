//COM env
BOOL CApp::InitInstance()
{
	CoInitialize(NULL);
}


int CApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	return CWinApp::ExitInstance();
}

