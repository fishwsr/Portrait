// VisImageWnd.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#pragma once


// WARNING:  This class is not finished yet and may be changed.


class VisCoreExport CVisImageWnd
{
public:
	// UNDONE:  Provide a default constructor and public create methods
	// that can accept an image reference and a title string.
	CVisImageWnd(const CVisImageBase& refimage);

	~CVisImageWnd(void);

	bool HasWindow(void) const;
	bool IsThreadRunning(void) const;

	HWND Hwnd(void) const;

	// This will not attempt to kill the thread, but it will return
	// before cmsTimeout milliseconds if the thread is killed by a user
	// action (closing the window).
	void WaitForThreadToQuit(DWORD cmsTimeout = INFINITE);

	// This will attempt to kill the thread, but it will return after
	// cmsTimeout milliseconds even if the thread has not been killed.
	void KillThread(DWORD cmsTimeout = INFINITE);


	// LATER:  Add a static table to keep track of these threads, then
	// add methods to wait for all threads to quit and to kill all threads.


	// LATER:  Add an auto-delete flag.

	// LATER:  Remember the time of window creation and add options to destroy
	// the window after a fixed amount of time or to keep only the last N
	// windows open?

	// LATER:  Window position option (or better automatic positioning)?

	// LATER:  Flag to specify whether window should be activated when it is
	// displayed?

protected:

	static ATOM RegisterClassIfNeeded(void);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT wMsg,
			WPARAM wParam, LPARAM lParam);

	static void StaticOnPaint(HWND hwnd);
	static void StaticOnDestroy(HWND hwnd);
	static unsigned __stdcall StaticThreadMain(void *pvCmdWnd);

	void OnPaint(void);
	void OnDestroy(void);

	unsigned __stdcall ThreadMain(void);


private:
	HANDLE m_hEventInitialized;
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	HWND m_hwnd;
	CVisImageBase *m_pimage;
	bool m_fKillThread;
	bool m_fThreadRunning;

	static char s_szAppName[MAX_PATH];

	bool Create(const char *szTitle = 0);


	// Private copy constructor and assignment operator to prevent the compiler
	// from providing default implementations.
	CVisImageWnd(const CVisImageWnd& refimagewnd);
	CVisImageWnd& operator=(const CVisImageWnd& refimagewnd);
};
