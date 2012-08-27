void CGraphicTestDlg::TransparentBlt2(int nX0 , int nY0)
{

	HBITMAP bitMan=(HBITMAP)LoadImage(AfxGetInstanceHandle(),"./c00000.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	HBITMAP bitManDealed=CreateCompatibleBitmap(pClientDC->m_hDC,22,42);
	HBITMAP bitMask=CreateBitmap(22,42,1,1,NULL);

	HDC	hdcMan=CreateCompatibleDC(pClientDC->m_hDC);
	HDC hdcManDealed=CreateCompatibleDC(pClientDC->m_hDC);
	HDC hdcMask=CreateCompatibleDC(pClientDC->m_hDC);
	
	HBITMAP bitOldMan=(HBITMAP)SelectObject(hdcMan,bitMan);
	HBITMAP bitOldManDealed=(HBITMAP)SelectObject(hdcManDealed,bitManDealed);
	HBITMAP bitOldMask=(HBITMAP)SelectObject(hdcMask,bitMask);

	BitBlt(hdcManDealed,0,0,22,42,hdcMan,0,0,SRCCOPY);

	SetBkColor(hdcManDealed,RGB(255,0,0));	
	BitBlt(hdcMask,0,0,22,42,hdcManDealed,0,0,SRCCOPY);

	
	SetBkColor(hdcManDealed,RGB(0,0,0));
	SetTextColor(hdcManDealed,RGB(255,255,255));
	BitBlt(hdcManDealed,0,0,22,42,hdcMask,0,0,SRCAND);
	
	BitBlt(pClientDC->m_hDC,nX0,nY0,22,42,hdcMask,0,0,SRCAND);
	BitBlt(pClientDC->m_hDC,nX0,nY0,22,42,hdcManDealed,0,0,SRCPAINT);
	
	SelectObject(hdcMan,bitOldMan);
	DeleteObject(bitMan);
	DeleteDC(hdcMan);

	SelectObject(hdcManDealed,bitOldManDealed);
	DeleteObject(bitManDealed);
	DeleteDC(hdcManDealed);
	
	SelectObject(hdcMask,bitOldMask);
	DeleteObject(bitMask);
	DeleteDC(hdcMask);
	
}

