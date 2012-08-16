void SetText(CEdit * pEdit,const char * szText)
{
	long len;
	len=pEdit->GetWindowTextLength();	
	if (len>=10000)
	{
		CString strWinText;
		pEdit->GetWindowText(strWinText);
		strWinText=strWinText.Mid(10000 - 1000);
		pEdit->SetWindowText(strWinText);
	}
	pEdit->SetSel(len,len);
	pEdit->ReplaceSel(szText);
	pEdit->LineScroll(pEdit->GetLineCount()-5);		
}

