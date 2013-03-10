// VisConsoleWnd.cpp
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"
#include <richedit.h>
#include <commctrl.h>

extern const char* gvis_szWarning;
extern const char* gvis_szError;		
extern const char* gvis_szTrace;		
extern const char* gvis_szProgressReport;

#define IDC_WARNING_SPIN			10001
#define IDC_ERROR_SPIN				10002
#define IDC_PROGRESS_SPIN			10003
#define IDC_TRACE_SPIN				10004

#define IDC_WARNING_EDIT			10005
#define IDC_ERROR_EDIT				10006
#define IDC_PROGRESS_EDIT			10007
#define IDC_TRACE_EDIT				10008


const char* gvis_szConsoleWindowClass = "Console Window";
const char* gvis_szConsoleWindowTitle = "Vision SDK Diagnostics Console";

int CVisConsoleWnd::s_nWidth = 0;


ATOM CVisConsoleWnd::ConsoleRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcexConsole;

	wcexConsole.cbSize = sizeof(WNDCLASSEX); 

	wcexConsole.style			= CS_HREDRAW | CS_VREDRAW;
	wcexConsole.lpfnWndProc	= (WNDPROC)ConsoleWndProc;
	wcexConsole.cbClsExtra		= 0;
	wcexConsole.cbWndExtra		= 0;
	wcexConsole.hInstance		= hInstance;
	wcexConsole.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcexConsole.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcexConsole.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcexConsole.lpszMenuName	= NULL;
	wcexConsole.lpszClassName	= gvis_szConsoleWindowClass;
	wcexConsole.hIconSm		= NULL;

	return RegisterClassEx(&wcexConsole);
}

HWND CVisConsoleWnd::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rct = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT+600, CW_USEDEFAULT+400 };
	
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX ;
	ConsoleRegisterClass(hInstance);
	m_hConsoleWnd = CreateWindow(gvis_szConsoleWindowClass, 
								gvis_szConsoleWindowTitle, 
								dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, 
								600, 400, NULL, NULL, hInstance, this);

	::ShowWindow(m_hConsoleWnd, nCmdShow);
	UpdateWindow(m_hConsoleWnd);

    return m_hConsoleWnd;
}

LRESULT CVisConsoleWnd::ConsoleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_CREATE:
		{
			CREATESTRUCT* lpcs = (CREATESTRUCT *)lParam;
			CVisConsoleWnd* pConsoleWnd = (CVisConsoleWnd *)(lpcs->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, (LONG)pConsoleWnd);
			pConsoleWnd->CreateControls(hWnd);
			HFONT hFont = CreateFont(8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");
			SendMessage( pConsoleWnd->ListControl(), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));
			break;
		}
		case WM_SIZE:
		{
			WORD nWidth = LOWORD(lParam);  // width of client area 
			WORD nHeight = HIWORD(lParam); // height of client area 
			CVisConsoleWnd* pConsoleWnd = (CVisConsoleWnd *) GetWindowLong(hWnd, GWL_USERDATA);
			SetWindowPos(pConsoleWnd->ListControl(), 
							NULL, 10, 40, nWidth - 20, nHeight - 50, SWP_NOMOVE | SWP_NOZORDER );
			break;
		}
		case WM_COMMAND:
		{
			int fTranslated = false;
			switch(LOWORD(wParam))
			{
				case IDC_WARNING_EDIT:
				{
					UINT uValue = GetDlgItemInt(hWnd, IDC_WARNING_EDIT, &fTranslated, FALSE);
					if(fTranslated && (uValue <= 16))
					{
						PGlobals()->SetWarningConsoleLevel(uValue);
					}
					break;
				}
				case IDC_ERROR_EDIT:
				{
					UINT uValue = GetDlgItemInt(hWnd, IDC_ERROR_EDIT, &fTranslated, FALSE);
					if(fTranslated && (uValue <= 16))
					{
						PGlobals()->SetErrorConsoleLevel(uValue);
					}
					break;
				}
				case IDC_PROGRESS_EDIT:
				{
					UINT uValue = GetDlgItemInt(hWnd, IDC_PROGRESS_EDIT, &fTranslated, FALSE);
					if(fTranslated && (uValue <= 16))
					{
						PGlobals()->SetProgressConsoleLevel(uValue);
					}
					break;
				}
				case IDC_TRACE_EDIT:
				{
					UINT uValue = GetDlgItemInt(hWnd, IDC_TRACE_EDIT, &fTranslated, FALSE);
					if(fTranslated && (uValue <= 16))
					{
						PGlobals()->SetTraceConsoleLevel(uValue);
					}
					break;
				}
			}
			
			break;
		}
		case WM_NOTIFY:
		{
			switch(((NMHDR FAR*)lParam)->code)
			{
				case UDN_DELTAPOS:
				{
					LPNMUPDOWN lpnmud = (LPNMUPDOWN) lParam;
					switch(lpnmud->hdr.idFrom)
					{
						case IDC_WARNING_SPIN:
						{
							PGlobals()->SetWarningConsoleLevel(lpnmud->iPos + lpnmud->iDelta);
							break;
						}
						case IDC_ERROR_SPIN:
						{
							PGlobals()->SetErrorConsoleLevel(lpnmud->iPos + lpnmud->iDelta);
							break;
						}
						case IDC_PROGRESS_SPIN:
						{
							PGlobals()->SetProgressConsoleLevel(lpnmud->iPos + lpnmud->iDelta);
							break;
						}
						case IDC_TRACE_SPIN:
						{
							PGlobals()->SetTraceConsoleLevel(lpnmud->iPos + lpnmud->iDelta);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case WM_SYSCOMMAND:
		{
			if(wParam == SC_CLOSE)
				return 0;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
   }
   return DefWindowProc(hWnd, message, wParam, lParam);
}


void CVisConsoleWnd::CreateControls(HWND hConsoleWnd)
{
	RECT rc;
	HINSTANCE hInstance = ::GetModuleHandle(0);

	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osversion);

	rc.left = 10; rc.right = rc.left + 60; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hWarningStatic = CreateWindow("STATIC", gvis_szWarning, 
								WS_CHILD | WS_VISIBLE, rc.left, rc.top, 
								60, 20, hConsoleWnd, NULL, hInstance, NULL);

	DWORD dwEditStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;
	if(osversion.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		dwEditStyle |= ES_READONLY;
	}	

	rc.left = 80; rc.right = rc.left + 30; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hWarningEdit = CreateWindow("EDIT", NULL, dwEditStyle,
								rc.left, rc.top, 30, 20, hConsoleWnd, (HMENU)IDC_WARNING_EDIT, hInstance, NULL);

	rc.left = 150; rc.right = rc.left + 40; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hErrorStatic = CreateWindow("STATIC", gvis_szError, 
								WS_CHILD | WS_VISIBLE, rc.left, rc.top, 
								40, 20, hConsoleWnd, NULL, hInstance, NULL);

	rc.left = 200; rc.right = rc.left + 30; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hErrorEdit = CreateWindow("EDIT", NULL, dwEditStyle,
								rc.left, rc.top, 30, 20, hConsoleWnd, (HMENU)IDC_ERROR_EDIT, hInstance, NULL);

	rc.left = 270; rc.right = rc.left + 60; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hProgressStatic = CreateWindow("STATIC", gvis_szProgressReport, 
								WS_CHILD | WS_VISIBLE, rc.left, rc.top, 
								60, 20, hConsoleWnd, NULL, hInstance, NULL);
	rc.left = 340; rc.right = rc.left + 30; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hProgressEdit = CreateWindow("EDIT", NULL, dwEditStyle,
								rc.left, rc.top, 30, 20, hConsoleWnd, (HMENU)IDC_PROGRESS_EDIT, hInstance, NULL);


	rc.left = 410; rc.right = rc.left + 40; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hTraceStatic = CreateWindow("STATIC", gvis_szTrace, 
								WS_CHILD | WS_VISIBLE, rc.left, rc.top, 
								40, 20, hConsoleWnd, NULL, hInstance, NULL);

	rc.left = 460; rc.right = rc.left + 30; rc.top = 10; rc.bottom = rc.top + 20;
	HWND hTraceEdit = CreateWindow("EDIT", NULL, dwEditStyle,
								rc.left, rc.top, 30, 20, hConsoleWnd, (HMENU)IDC_TRACE_EDIT, hInstance, NULL);

	if(osversion.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		HWND hWarningSpin = 
					CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT, 
										110, 10, 20, 20, hConsoleWnd,
										IDC_WARNING_SPIN, hInstance, hWarningEdit, 
										16, 0, PGlobals()->WarningConsoleLevel());
		HWND hErrorSpin = 
					CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT, 
										230, 10, 20, 20, hConsoleWnd,
										IDC_ERROR_SPIN, hInstance, hErrorEdit, 
										16, 0, PGlobals()->ErrorConsoleLevel());
		HWND hProgressSpin = 
					CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT, 
										370, 10, 20, 20, hConsoleWnd,
										IDC_PROGRESS_SPIN, hInstance, hProgressEdit, 
										16, 0, PGlobals()->ProgressConsoleLevel());
		HWND hTraceSpin = 
					CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT, 
										490, 10, 20, 20, hConsoleWnd,
										IDC_TRACE_SPIN, hInstance, hTraceEdit, 
										16, 0, PGlobals()->TraceConsoleLevel());
	}
	else
	{

		SendMessage(hWarningEdit, EM_LIMITTEXT, (WPARAM)2, 0);
		SendMessage(hErrorEdit, EM_LIMITTEXT, (WPARAM)2, 0);
		SendMessage(hProgressEdit, EM_LIMITTEXT, (WPARAM)2, 0);
		SendMessage(hTraceEdit, EM_LIMITTEXT, (WPARAM)2, 0);

		SetDlgItemInt(hConsoleWnd, IDC_WARNING_EDIT, PGlobals()->WarningConsoleLevel(), FALSE);
		SetDlgItemInt(hConsoleWnd, IDC_ERROR_EDIT, PGlobals()->ErrorConsoleLevel(), FALSE);
		SetDlgItemInt(hConsoleWnd, IDC_PROGRESS_EDIT, PGlobals()->ProgressConsoleLevel(), FALSE);
		SetDlgItemInt(hConsoleWnd, IDC_TRACE_EDIT, PGlobals()->TraceConsoleLevel(), FALSE);
	}

	GetClientRect( hConsoleWnd, &rc );
	rc.left += 10;	rc.right -= 10;	rc.top += 40;	rc.bottom -= 10;
	m_hListBox = CreateWindow( "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
								rc.left, rc.top, rc.right - rc.left, 
								rc.bottom - rc.top, hConsoleWnd, NULL, hInstance, NULL);

}

void CVisConsoleWnd::ShowWindow(int nCmdShow)
{
	::ShowWindow(m_hConsoleWnd, nCmdShow);
}

void CVisConsoleWnd::AddMessage( LPCSTR lpstr )
{
	::SendMessage( m_hListBox, WM_SETREDRAW, (WPARAM) FALSE, 0 );

	int iIndex = ::SendMessageA( m_hListBox, LB_ADDSTRING, 0, (LPARAM) (LPCSTR) lpstr );
	::SendMessage( m_hListBox, LB_SETCURSEL, iIndex, 0 );

	int nLength = strlen(lpstr);
	SIZE sizeString;
	GetTextExtentPoint32(GetDC(m_hListBox), lpstr, nLength, &sizeString);
	if(s_nWidth < nLength)
	{
		s_nWidth = nLength;
		::SendMessage( m_hListBox, LB_SETHORIZONTALEXTENT, sizeString.cx, 0 );
	}
	::SendMessage( m_hListBox, WM_SETREDRAW, (WPARAM) TRUE, 0 );
}

HWND CVisConsoleWnd::ListControl(void)
{
	return m_hListBox;
}

void CVisConsoleWnd::SetListControl(HWND hList)
{
	m_hListBox = hList;
}

