// VisImageWnd.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"

#include <process.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CVisImageWndManager
{
public:
	// UNDONE:  There should be a static CVisImageWndManager object in some CPP file.
	// It's ctor should initialize a critical section.  CVisImageWnd ctors should call
	// the add method and CVisImageWnd dtors should call the remove method.  There
	// should be methods to get a table of (duplicate) thread handles for all CVisImageWnd
	// objects in use.  These tables can be used to wait for or kill all CVisImageWnd threads.
	// The CVisImageWndManager dtor should kill all CVisImageWnd threads and destroy
	// the critical section before exiting.
	CVisImageWndManager(void);
	~CVisImageWndManager(void);

	void AddImageWnd(CVisImageWnd *pimagewnd);
	void RemoveImageWnd(CVisImageWnd *pimagewnd);

private:
	// UNDONE:  Critical section and list entries
	// (Keep a list of CVisImageWnds in use, and provide methods
	// to gett duplicate handles for all threads in use.)
};



char CVisImageWnd::s_szAppName[MAX_PATH];


CVisImageWnd::CVisImageWnd(const CVisImageBase& refimage)
  : m_hEventInitialized(0),
	m_hThread(0),
	m_dwThreadID(0),
	m_hwnd(0),
	m_pimage(0),
	m_fKillThread(false),
	m_fThreadRunning(false)
{
	if ((RegisterClassIfNeeded() != 0)
			&& (refimage.IsValid()))
	{
		m_pimage = refimage.Clone();
		m_hEventInitialized = CreateEvent(0, true, false, 0);
		
		unsigned int uiThreadID;
		m_hThread = (HANDLE) _beginthreadex(0, 0, StaticThreadMain, this, 0,
				&uiThreadID);
		m_dwThreadID = uiThreadID;

		if (m_hThread != 0)
		{
			WaitForSingleObject(m_hEventInitialized, INFINITE);
			SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
		}

		CloseHandle(m_hEventInitialized);
		m_hEventInitialized = 0;
	}
}

CVisImageWnd::~CVisImageWnd(void)
{
	KillThread();
	if (m_hThread != 0)
	{
		CloseHandle(m_hThread);
		m_hThread = 0;
	}

	if (m_pimage != 0)
	{
		delete m_pimage;
		m_pimage = 0;
	}
}

bool CVisImageWnd::HasWindow(void) const
{
	return (m_hwnd != 0);
}

bool CVisImageWnd::IsThreadRunning(void) const
{
	return m_fThreadRunning;
}

HWND CVisImageWnd::Hwnd(void) const
{
	return m_hwnd;
}

void CVisImageWnd::WaitForThreadToQuit(DWORD cmsTimeout)
{
	// This will not attempt to kill the thread, but it will return
	// before cmsTimeout milliseconds if the thread is killed by a user
	// action (closing the window).
	WaitForSingleObject(m_hThread, cmsTimeout);
}

void CVisImageWnd::KillThread(DWORD cmsTimeout)
{
	// This will attempt to kill the thread, but it will return after
	// cmsTimeout milliseconds even if the thread has not been killed.
	if (m_hThread != 0)
	{
		// Kill it.
		::PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
		m_fKillThread = true;

		if (cmsTimeout != 0)
			WaitForThreadToQuit(cmsTimeout);
	}
}

ATOM CVisImageWnd::RegisterClassIfNeeded(void)
{
	static bool fRegisteredClass = false;
	static ATOM atomRet;

	if (!fRegisteredClass)
	{
		s_szAppName[0] = 0;
		GetModuleFileName(0, s_szAppName, sizeof(s_szAppName));

		WNDCLASSEX wcex; 
		wcex.style         = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc   = (WNDPROC) CVisImageWnd::WndProc;
		wcex.cbClsExtra    = 0;
		wcex.cbWndExtra    = 0;
		wcex.hInstance     = ::GetModuleHandle(0);
		// LATER:  wcex.hIcon         = LoadIcon (hInstance, szAppName);
		wcex.hIcon         = 0;
		wcex.hCursor       = LoadCursor(0, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
		wcex.lpszMenuName  = "WIN95";

		wcex.lpszClassName = s_szAppName; 

		// Added elements for Windows 95: 
		wcex.cbSize = sizeof(WNDCLASSEX); 
		// LATER:  wcex.hIconSm = LoadIcon(wcex.hInstance, "SMALL"); 
		wcex.hIconSm = 0; 

		atomRet = (ATOM) RegisterClassEx(&wcex);

		fRegisteredClass = true;

		if (atomRet != 0)
			printf("\n\nRegistered class.\n");
	}

	return atomRet;
}

LRESULT CALLBACK CVisImageWnd::WndProc(HWND hwnd, UINT wMsg,
		WPARAM wParam, LPARAM lParam) 
{
	switch (wMsg)
	{
	case WM_PAINT:
		CVisImageWnd::StaticOnPaint(hwnd);
		break;

	case WM_DESTROY:
		CVisImageWnd::StaticOnDestroy(hwnd);
		break;

	case WM_CREATE:
		// lParam should be a non-NULL pointer to a CreateStruct.  The first
		// member of the struct is a pointer to the CVisImageWnd class that
		// created this window.  We should save that pointer here.
		assert(lParam != 0);
		assert(*((long *) lParam) != 0);
		SetWindowLong(hwnd, GWL_USERDATA, *((long *) lParam));
		break;

	default:
		return (DefWindowProc(hwnd, wMsg, wParam, lParam));
	}

	return (0);
}

void CVisImageWnd::StaticOnPaint(HWND hwnd)
{
	assert(hwnd != 0);

	CVisImageWnd *pimagewnd = (CVisImageWnd *)
			(void *) GetWindowLong(hwnd, GWL_USERDATA);
	assert(pimagewnd != 0);
	assert(pimagewnd->m_hwnd == hwnd);

	pimagewnd->OnPaint();
}

void CVisImageWnd::StaticOnDestroy(HWND hwnd)
{
	assert(hwnd != 0);

	CVisImageWnd *pimagewnd = (CVisImageWnd *)
			(void *) GetWindowLong(hwnd, GWL_USERDATA);
	assert(pimagewnd != 0);
	assert(pimagewnd->m_hwnd == hwnd);

	pimagewnd->OnDestroy();
}

unsigned __stdcall CVisImageWnd::StaticThreadMain(void *pvCmdWnd)
{
	CVisImageWnd *pimagewnd = (CVisImageWnd *) pvCmdWnd;
	assert(pimagewnd != 0);
	return pimagewnd->ThreadMain();
}

void CVisImageWnd::OnPaint(void)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);

	// UNDONE:  If no image, fill with a solid color?
	if (m_pimage != 0)
		m_pimage->DisplayInHdc(hdc);

	EndPaint(m_hwnd, &ps);
}

void CVisImageWnd::OnDestroy(void)
{
	m_hwnd = 0;
	KillThread(0);
}

unsigned __stdcall CVisImageWnd::ThreadMain(void)
{
	m_fThreadRunning = true;

	assert(m_pimage != 0);
	Create();

	SetEvent(m_hEventInitialized);

	// Message loop
	MSG msg;
	while ((::GetMessage(&msg, 0, 0, 0) == 1) && (!m_fKillThread))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		// Sleep(0);
	}

#ifdef USE_CMD_WND_DEBUG_STRINGS
	OutputDebugString("CVisImageWnd thread exiting.\n");
#endif // USE_CMD_WND_DEBUG_STRINGS

	// Destroy the window before destroying the thread.
	if (m_hwnd != 0)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = 0;
	}

	m_fThreadRunning = false;

	return 0;
}

bool CVisImageWnd::Create(const char *szTitle)
{
	bool fRet = false;

	if ((m_pimage != 0) && (m_pimage->IsValid()))
	{
		if (szTitle == 0)
			szTitle = m_pimage->Name();
		if (szTitle == 0)
			szTitle = "Untitled Image";

		// Find the desired window dimensions
		RECT rectT = m_pimage->Rect();
		AdjustWindowRectEx(&rectT, WS_OVERLAPPEDWINDOW, false, 0);
		int dx = rectT.right - rectT.left;
		int dy = rectT.bottom - rectT.top;

		m_hwnd = CreateWindow(s_szAppName, szTitle, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, 0, dx, dy, 0, 0, ::GetModuleHandle(0),
				(void *) this);

		if (m_hwnd != 0)
		{ 
			// LATER:  Add an "activate window" flag?
			ShowWindow(m_hwnd, SW_SHOWNA);
			UpdateWindow(m_hwnd);
			fRet = true;
		}
	}

	return fRet;
}
