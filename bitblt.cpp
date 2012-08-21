HDC    DC=GetDC(hwnd);
		HDC	   MemDC=CreateCompatibleDC(DC);		
		HBITMAP bitGround;

		DeleteObject(bitGround);
		bitGround=(HBITMAP)LoadImage(NULL,"./Ground.BMP",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		
		HBITMAP bitOld=(HBITMAP)SelectObject(MemDC,bitGround);
		
		DIBSECTION ds;
		GetObject(bitGround,sizeof(ds),&ds);
		
		BitBlt(DC,
			0,0,
			ds.dsBmih.biWidth,ds.dsBmih.biHeight,MemDC,
			0,0,
			SRCCOPY);
		
		SelectObject(MemDC,bitOld);
		DeleteObject(bitOld);