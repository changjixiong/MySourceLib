(GetSystemMetrics(SM_CXSCREEN) - 640)/2,
(GetSystemMetrics(SM_CYSCREEN) - 480)/2, 


	RECT window_rect;
	GetWindowRect(hwnd,&window_rect);
	AdjustWindowRectEx(&window_rect,
						GetWindowStyle(hwnd),
						GetMenu(hwnd)!=NULL,
						GetWindowExStyle(hwnd));

	SetWindowPos(hwnd,
				HWND_TOP,
				(GetSystemMetrics(SM_CXSCREEN) - (window_rect.right - window_rect.left))/2,
				(GetSystemMetrics(SM_CYSCREEN) - (window_rect.bottom - window_rect.top))/2,
				window_rect.right - window_rect.left,
				window_rect.bottom - window_rect.top,
				SWP_SHOWWINDOW);

