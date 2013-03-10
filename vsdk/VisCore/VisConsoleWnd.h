// VisConsoleWnd.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
#ifndef _VIS_CONSOLE_WND_
#define _VIS_CONSOLE_WND_



class CVisConsoleWnd
{
public:

	ATOM ConsoleRegisterClass(HINSTANCE hInstance);
	HWND InitInstance(HINSTANCE hInstance, int nCmdShow);
	void CreateControls(HWND hConsoleWnd);


	void AddMessage(LPCSTR lpstr);

	void ShowWindow(int nCmdShow);

	HWND ListControl(void);
	static LRESULT ConsoleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetListControl(HWND hList);

private:
	HWND m_hConsoleWnd;
	HWND m_hListBox;

	static int s_nWidth;
};

#endif // _VIS_CONSOLE_WND_