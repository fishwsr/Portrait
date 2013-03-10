////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPANE
//
// @module VisPane.cpp |
//
// This file implements the <c CVisPaneWnd> class.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPane\.h>
//
// <nl>
// @index | VISPANE
//
////////////////////////////////////////////////////////////////////////////



// LATER:  Add formatting code to tile panes across screen in the order
// that they are created?


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisDisplayPch.h"
#include "VisDisplayProj.h"

#include <memory.h>

#include "resource.h"


// Ignore warning that "identifier was truncated to '255' characters in
// the debug information".  (We get the warning from the STL code for
// std::set.)
#pragma warning(disable : 4786)


#include <set>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// Class static vars

int  CVisPaneWndBase::s_dyStatusBar = 18;

IMPLEMENT_DYNCREATE(CVisPaneThread, CWinThread)

BEGIN_MESSAGE_MAP(CVisRefCntWnd, CWnd)
	//{{AFX_MSG_MAP(CVisRefCntWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CVisPaneWndBase, CVisRefCntWnd)
	//{{AFX_MSG_MAP(CVisPaneWndBase)
	ON_WM_DESTROY()
	ON_WM_SIZING()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CVisPaneWnd, CVisPaneWndBase)
	//{{AFX_MSG_MAP(CVisPaneWnd)
	ON_WM_PAINT()
   	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CVisPaneThread, CWinThread)
	//{{AFX_MSG_MAP(CVisPaneThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVisPaneThread

CVisPaneThread::CVisPaneThread()
{
	m_heventInitialized = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CVisPaneThread::~CVisPaneThread()
{
}

void CVisPaneThread::Init(CVisPaneWndBase *ppanewndbase,
		const char *szCursor)
{
	m_ppanewndbase = ppanewndbase;
	m_szCursor = szCursor;

	ResumeThread();

	WaitForSingleObject(m_heventInitialized, INFINITE);
}

BOOL CVisPaneThread::InitInstance()
{
	if (m_ppanewndbase->RealCreateWnd(m_szCursor, false))
		m_pMainWnd = m_ppanewndbase;
	
	// Signal that initialization is complete.
	SetEvent(m_heventInitialized);
	TRACE0("\n Thread Init");
	return (m_pMainWnd != 0);
}

int CVisPaneThread::ExitInstance()
{
	CloseHandle(m_heventInitialized);
	TRACE0("\n Thread Exit");
	return CWinThread::ExitInstance();
}


////////////////////////////////////////////////////////////////////////////
// CVisPane

CVisPane::CVisPane(CWnd *pwndParent)
  : m_ppanewnd(new CVisPaneWnd(evispaneDefault, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError("CVisPane::CVisPane(void)",
				__FILE__, __LINE__);
	}
}

CVisPane::CVisPane(int evispane, CWnd *pwndParent)
  : m_ppanewnd(new CVisPaneWnd(evispane, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError(
				"CVisPane::CVisPane(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}
}


CVisPane::CVisPane(int dx, int dy, const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd)
  : m_ppanewnd(new CVisPaneWnd(evispane, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError("CVisPane::CVisPane(int dx, int dy,...)",
				__FILE__, __LINE__);
	}

	// Initialize other CVisPaneWnd members.
	SetSizeInside(dx, dy, false);
	SetTitle(szTitle);

	if (fCreateWnd)
		CreateWnd();
}


CVisPane::CVisPane(SIZE size, const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd)
  : m_ppanewnd(new CVisPaneWnd(evispane, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError("CVisPane::CVisPane(SIZE size,...)",
				__FILE__, __LINE__);
	}

	// Initialize other CVisPaneWnd members.
	SetSizeInside(size, false);
	SetTitle(szTitle);

	if (fCreateWnd)
		CreateWnd();
}


CVisPane::CVisPane(POINT point, SIZE size, const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd)
  : m_ppanewnd(new CVisPaneWnd(evispane, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError("CVisPane::CVisPane(POINT point, SIZE size,...)",
				__FILE__, __LINE__);
	}

	// Initialize other CVisPaneWnd members.
	SetImageOffset(point, false);
	SetSizeInside(size, false);
	SetTitle(szTitle);

	if (fCreateWnd)
		CreateWnd();
}


CVisPane::CVisPane(const RECT& refrect, const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd)
  : m_ppanewnd(new CVisPaneWnd(evispane, pwndParent))
{
	if (m_ppanewnd == 0)
	{
		throw CVisMemoryError("CVisPane::CVisPane(const RECT& refrect,...)",
				__FILE__, __LINE__);
	}

	// Initialize other CVisPaneWnd members.
	SetRectOutside(refrect, false);
	SetTitle(szTitle);

	if (fCreateWnd)
		CreateWnd();
}


CVisPane::CVisPane(const CVisPane& refpane)
  : m_ppanewnd(0)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_ppanewnd = refpane.m_ppanewnd;
	m_ppanewnd->AddRef();

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


CVisPane& CVisPane::operator=(const CVisPane& refpane)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_ppanewnd->Release();
	m_ppanewnd = refpane.m_ppanewnd;
	m_ppanewnd->AddRef();

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return *this;
}

void CVisPane::SetMirrorImage(bool fMirror)
{
	m_ppanewnd->SetMirrorImage(fMirror);
}

bool CVisPane::GetMirrorImage() const
{
	return m_ppanewnd->GetMirrorImage();
}


////////////////////////////////////////////////////////////////////////////
// Helper class
class CVisRefCntWndSet
{
public:
	CVisRefCntWndSet(void);

	~CVisRefCntWndSet(void);

protected:
	// Only CVisRefCntWnd can access the set contained in this class.
	friend class CVisRefCntWnd;

	// Note:  We could use CVisPaneWnd here.  We tried using void to get shorter
	// names (in an attempt to get rid of warning 4786).
	std::set<void *> m_setppanewnd;
};


CVisRefCntWndSet::CVisRefCntWndSet(void)
  : m_setppanewnd()
{
}

CVisRefCntWndSet::~CVisRefCntWndSet(void)
{
	// Destroy any windows that are up when the program ends.
	while (!m_setppanewnd.empty())
	{
		// UNDONE:  Add a thread ID check and only attempt to delete windows
		// that were created in this thread.  (Or look at the fOwnThread property of
		// the pane?)
		if (((CVisRefCntWnd *) (*m_setppanewnd.begin()))->HasWnd())
		{
			((CVisRefCntWnd *) (*m_setppanewnd.begin()))->DestroyWindow();
		}
		else
		{
			// When the main application exits, threads and windows
			// can be destroyed without notifying our code.
#ifdef _DEBUG
			static bool fWarnedUser = false;
			if (!fWarnedUser)
			{
				fWarnedUser = true;
				OutputDebugString("VisSDK Warning ("
						"CVisRefCntWndSet::~CVisRefCntWndSet, " __FILE__
						"):  Some CVisRefCntWnd objects were not freed.\n");
			}
#endif // _DEBUG


#ifdef VIS_PANE_RELEASE_ALL_WNDS

			// We could call the Release method of the CVisRefCntWnd object
			// to avoid memory leaks, but that may cause other problems,
			// since memory used by the thread may have already been freed.
			((CVisRefCntWnd *) (*m_setppanewnd.begin()))->Release();

#else // VIS_PANE_RELEASE_ALL_WNDS

			// If we don't release the CVisRefCntWnd object, we need to
			// explicitly remove it from the set to avoid an infinite loop.
			m_setppanewnd.erase(m_setppanewnd.begin());

#endif // VIS_PANE_RELEASE_ALL_WNDS
		}
	}
}


static CVisRefCntWndSet s_panewndset;


////////////////////////////////////////////////////////////////////////////
// CVisRefCntWnd

CVisRefCntWnd::CVisRefCntWnd(bool fAutoDestroy)
  : m_cRef(1),
	m_strTitle(""),
	m_fAutoDestroy(fAutoDestroy),
	m_fHasRefreshed(false)
{
	// Add a pointer to this object to the set of active CVisPaneWnd objects.
	s_panewndset.m_setppanewnd.insert(this);
}


CVisRefCntWnd::~CVisRefCntWnd(void)
{
	// Remove the pointer to this object from the set of active CVisPaneWnd
	// objects.
	s_panewndset.m_setppanewnd.erase(this);

	// Destroy the window if it's still up.
	if (HasWnd())
		DestroyWindow();
}


BOOL CVisRefCntWnd::PreCreateWindow( CREATESTRUCT& cs )
{
	assert(m_hWnd == 0);

	// If the AutoDelete option is not specified, increment the reference count
	// on this object when the window is created.
	if (!m_fAutoDestroy)
		AddRef();
	// Call the base class.
	int i=WS_SYSMENU;
	cs.style &= (~i);
	return CWnd::PreCreateWindow(cs);
}


void CVisRefCntWnd::PostNcDestroy(void)
{
	assert(m_hWnd == 0);

	// Call the base class.
	CWnd::PostNcDestroy();

	// If the AutoDestroy option is not specified, decrement the reference
	// count on this object when the window is destroyed.
	if (!m_fAutoDestroy)
		Release();
}


const char *CVisRefCntWnd::Title(void)
{
	if (HasWnd())
	{
		CString stringT;
		GetWindowText(stringT);
		if (stringT.IsEmpty())
			m_strTitle = "";
		else
			m_strTitle = stringT;
	}

	return m_strTitle.c_str();
}


void CVisRefCntWnd::SetTitle(const char *szTitle)
{
	if (szTitle == 0)
		m_strTitle = "";
	else
		m_strTitle = szTitle;

	if (HasWnd())
		SetWindowText(m_strTitle.c_str());
}


void CVisRefCntWnd::Refresh(bool fUpdateNow)
{
	SetHasRefreshed(false);

	if (HasWnd())
	{
		if (fUpdateNow)
			RedrawWindow();
		else
			Invalidate();
	}
}


////////////////////////////////////////////////////////////////////////////
// CVisPaneWndBase

CVisPaneWndBase::CVisPaneWndBase(int evispane, bool fIsPaneArrayWnd,
			bool fAutoDestroy, bool fUseStatusBar)
  : CVisRefCntWnd(fAutoDestroy),
	m_rectInParent(0, 0, 2 * edxBorderDefault, 2 * edxBorderDefault),
	m_dxBorder(edxBorderDefault),
	m_evispane(evispane),
	m_dwStyle(0),
	m_dwExStyle(0),
	m_colorrefBackground(RGB(192, 192, 192)),  // LATER:  Should we use (192, 192, 192)? // yes - mturk
	m_pWndParent(0),
	m_ipaneInParent(- 1),
	m_ppanethread(0),
	m_statusbar(),
	m_fUseStatusBar(fUseStatusBar),
	m_fIsPaneArrayWnd(fIsPaneArrayWnd),
	m_fIsPartOfArray(false)
{
	if (UseStatusBar())
		m_rectInParent.bottom += DyStatusBar();

	// Find the window styles from the EVisPaneArray options.
	FindStyles(evispane);
}


CVisPaneWndBase::~CVisPaneWndBase(void)
{
	// If the application has ended and this window has its own
	// thread, the status bar may think that it still has a valid
	// window.  That can cause an unwanted assert when the status
	// bar is destroyed.  This code prevents the assert in the
	// status bar's destructor.
	assert(!HasWnd());
	if (m_statusbar.m_hWnd != 0)
		m_statusbar.Detach();

	// If the application has ended and this window has its own
	// thread, we need to explicitly destroy the thread object in
	// the destructor to avoid memory leaks.
	if (m_ppanethread != 0)
	{
#ifdef _DEBUG
		static bool fWarnedUser = false;
		if (!fWarnedUser)
		{
			fWarnedUser = true;
			OutputDebugString("VisSDK Warning ("
					"CVisPaneWndBase::~CVisPaneWndBase, " __FILE__
					"):  Some CVisPaneThread objects were not freed.\n");
		}

		// We could delete the thread to avoid memory leaks, but that may
		// cause other problems, since memory used by the thread may have
		// already been freed.
		TRACE0("\n detele thread");
		//::TerminateThread(m_ppanethread->m_hThread,0);
		 //delete m_ppanethread;
#endif // _DEBUG
	}
}


// @mfunc ***
int CVisPaneWndBase::OnDestroy()
{
	// Save the current settings in member vars.
	FindRectInParent();
	FindDwStyle();
	FindDwExStyle();
	FindTitle();

	return 0;
}


void CVisPaneWndBase::SetXOffsetInParent(int x, bool fRepaint)
{
	FindRectInParent();

	if (m_rectInParent.left != x)
	{
		m_rectInParent.right = x + m_rectInParent.Width();
		m_rectInParent.left = x;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetYOffsetInParent(int y, bool fRepaint)
{
	FindRectInParent();

	if (m_rectInParent.top != y)
	{
		m_rectInParent.bottom = y + m_rectInParent.Height();
		m_rectInParent.top = y;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetPointOffsetInParent(POINT point, bool fRepaint)
{
	FindRectInParent();

	if (m_rectInParent.TopLeft() != point)
	{
		m_rectInParent.right = point.x + m_rectInParent.Width();
		m_rectInParent.bottom = point.y + m_rectInParent.Height();
		m_rectInParent.left = point.x;
		m_rectInParent.top = point.y;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetWidthInside(int dx, bool fRepaint)
{
	int dxAdjust = dx - WidthInside();

	if (dxAdjust != 0)
	{
#ifdef _DEBUG
		CheckRectInParent();
#endif // _DEBUG

		m_rectInParent.right += dxAdjust;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetWidthOutside(int dx, bool fRepaint)
{
	FindRectInParent();

	if (dx != m_rectInParent.Width())
	{
		m_rectInParent.right = m_rectInParent.left + dx;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetWidthBorder(int dx, bool fRepaint)
{
	if (dx != m_dxBorder)
	{
		m_dxBorder = dx;

		if (!StyleChangesSizeInside())
		{
			FindRectInParent();

			m_rectInParent.right += (2 * (dx - m_dxBorder));
			m_rectInParent.bottom += (2 * (dx - m_dxBorder));
		}

		SetHasRefreshed(false);

		if (HasWnd())
		{
			if (!StyleChangesSizeInside())
				MoveWindow(m_rectInParent, fRepaint);
			else
				Refresh();
		}
	}
}


void CVisPaneWndBase::SetHeightInside(int dy, bool fRepaint)
{
	int dyAdjust = dy - HeightInside();

	if (dyAdjust != 0)
	{
#ifdef _DEBUG
		CheckRectInParent();
#endif // _DEBUG

		m_rectInParent.bottom += dyAdjust;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetHeightOutside(int dy, bool fRepaint)
{
	FindRectInParent();

	if (dy != m_rectInParent.Height())
	{
		m_rectInParent.bottom = m_rectInParent.top + dy;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}

CSize CVisPaneWndBase::SizeInOutDifference(void) const
{
	CVisRect rectDifference(0, 0, 2 * m_dxBorder, 2 * m_dxBorder);
	if (UseStatusBar())
		rectDifference.bottom += DyStatusBar();

	if (!AdjustWindowRectEx(&rectDifference, m_dwStyle, false, m_dwExStyle))
	{
#ifdef _DEBUG
		OutputDebugString("VisSDK Warning:  AdjustWindowRectEx failed in "
				__FILE__ "!\n");
#endif // _DEBUG
	}

	if ((m_dwStyle & WS_HSCROLL) != 0)
		rectDifference.bottom += GetSystemMetrics(SM_CYHSCROLL);

	if ((m_dwStyle & WS_VSCROLL) != 0)
		rectDifference.right += GetSystemMetrics(SM_CXVSCROLL);

	return rectDifference.Size();
}


void CVisPaneWndBase::SetSizeOutside(int dx, int dy, bool fRepaint)
{
	FindRectInParent();

	if ((dx != m_rectInParent.Width())
			|| (dy != m_rectInParent.Height()))
	{
		m_rectInParent.right = m_rectInParent.left + dx;
		m_rectInParent.bottom = m_rectInParent.top + dy;

		SetHasRefreshed(false);

		if (HasWnd())
		{
			MoveWindow(m_rectInParent, fRepaint);
			// Put the status bar at the bottom of the window.
			if (UseStatusBar())
			{
				CVisRect rectStatusBar = RectClientStatusBar();
				StatusBar().MoveWindow(&rectStatusBar);
			}
		}
	}

}


void CVisPaneWndBase::SetRectOutside(const RECT& refrect, bool fRepaint)
{
	FindRectInParent();

	if (m_rectInParent != refrect)
	{
		m_rectInParent = refrect;

		SetHasRefreshed(false);

		if (HasWnd())
			MoveWindow(m_rectInParent, fRepaint);
	}
}


void CVisPaneWndBase::SetStyle(DWORD dw)
{
	FindDwStyle();

	if (m_dwStyle != dw)
	{
		CSize sizeInside;
		if (!StyleChangesSizeInside())
			sizeInside = SizeInside();

		if (HasWnd())
		{
			DWORD dwSame = (m_dwStyle & dw);
			DWORD dwAdd = (dw ^ dwSame);
			DWORD dwRemove = (m_dwStyle ^ dwSame);

			SetHasRefreshed(false);

			ModifyStyle(dwRemove, dwAdd);
		}

		m_dwStyle = dw;
		
		if (!StyleChangesSizeInside())
			SetSizeInside(sizeInside);
	}
}


void CVisPaneWndBase::SetExStyle(DWORD dw)
{
	FindDwExStyle();

	if (m_dwExStyle != dw)
	{
		CSize sizeInside;
		if (!StyleChangesSizeInside())
			sizeInside = SizeInside();

		if (HasWnd())
		{
			DWORD dwSame = (m_dwExStyle & dw);
			DWORD dwAdd = (dw ^ dwSame);
			DWORD dwRemove = (m_dwExStyle ^ dwSame);

			SetHasRefreshed(false);

			ModifyStyleEx(dwRemove, dwAdd);
		}

		m_dwExStyle = dw;
		
		if (!StyleChangesSizeInside())
			SetSizeInside(sizeInside);
	}
}


void CVisPaneWndBase::FindRectInParent(RECT& refrect) const
{
	refrect = m_rectInParent;
	
	if (HasWnd())
	{
		GetWindowRect(&refrect);

		if (m_pWndParent != 0)
		{
			// Need to convert the screen coordinates to client coordinates
			// in the parent window.
			m_pWndParent->ScreenToClient(&refrect);
		}
	}
}


#ifdef _DEBUG
void CVisPaneWndBase::CheckRectInParent(void)
{
	CVisRect rectInParent = m_rectInParent;
	FindRectInParent();
	assert(rectInParent == m_rectInParent);
}
#endif // _DEBUG


void CVisPaneWndBase::SetUseStatusBar(bool fUseStatusBar)
{
	// For now, don't support a change to this setting while the window is
	// up.  LATER:  Allow the user to change this when the window is up?
	assert(!HasWnd());

	m_fUseStatusBar = fUseStatusBar;

	if (!StyleChangesSizeInside())
	{
		if ((m_fUseStatusBar) && (!fUseStatusBar))
		{
			SetHeightOutside(HeightOutside() - DyStatusBar());
		}
		else if ((!m_fUseStatusBar) && (fUseStatusBar))
		{
			SetHeightOutside(HeightOutside() + DyStatusBar());
		}
	}
}


BOOL CVisPaneWndBase::CreateWnd(int evispane, CWnd *pwndParent,
		bool fChangeStyles)
{
	// Warn the user with an assert.
	assert(!HasWnd());

	// But write the code to protect the user in the Releasde build.
	if (!HasWnd())
	{
		if (pwndParent != 0)
			SetPWndParent(pwndParent);

		SetEvispaneOptions(evispane, fChangeStyles);

		const char *szCursor;
		// LATER:  Don't use crosshair if we won't display pixel values.
		if (IsPaneWnd())
			szCursor = IDC_CROSS;
		else
			szCursor = IDC_ARROW;

		return RealCreateWnd(szCursor,
				((EvispaneOptions() & evispaneOwnThread) != 0));
	}

	return true;
}


void CVisPaneWndBase::PostNcDestroy(void)
{
	// If we have our own thread, it will be destroyed when our window
	// is destroyed.
	m_ppanethread = 0;

	// Call the base class.
	CVisRefCntWnd::PostNcDestroy();
}


void CVisPaneWndBase::Refresh(bool fUpdateNow)
{
	CVisRefCntWnd::Refresh(fUpdateNow);

	if ((HasWnd()) && (UseStatusBar()))
	{
		if (fUpdateNow)
			m_statusbar.RedrawWindow();
		else
			m_statusbar.Invalidate();
	}
}


CVisRect CVisPaneWndBase::RectClient(void) const
{
	assert(HasWnd());
	CVisRect rectClient;
	GetClientRect(&rectClient);
	return rectClient;
}


CVisRect CVisPaneWndBase::RectClientStatusBar(const CVisRect& refrectClient) const
{
	assert(StatusBar());
	CVisRect rect(refrectClient);
	rect.top = rect.bottom - DyStatusBar();

	return rect;
}


CVisRect CVisPaneWndBase::RectClientInside(const CVisRect& refrectClient) const
{
	CVisRect rect(RectClientBorder(refrectClient));
	rect.left += WidthBorder();
	rect.top += WidthBorder();
	rect.right -= WidthBorder();
	rect.bottom -= WidthBorder();

	return rect;
}


CVisRect CVisPaneWndBase::RectClientBorder(const CVisRect& refrectClient) const
{
	CVisRect rect(refrectClient);

	if (UseStatusBar())
		rect.bottom -= DyStatusBar();

	return rect;
}


BOOL CVisPaneWndBase::RealCreateWnd(const char *szCursor, bool fNewThread)
{
	const UINT k_nClassStyle = (CS_OWNDC | CS_HREDRAW | CS_VREDRAW
				| CS_DBLCLKS | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW);

	if (fNewThread)
	{
		m_ppanethread = (CVisPaneThread *) AfxBeginThread(
				RUNTIME_CLASS(CVisPaneThread), THREAD_PRIORITY_NORMAL, 
				0, CREATE_SUSPENDED, NULL);

		// Initialize thread and create the window.
		m_ppanethread->Init(this, szCursor);
	}
	else
	{
		HCURSOR hCursor = ::LoadCursor(0, szCursor);
		assert(hCursor != 0);

		// LATER:  The following line seems right, but it caused link
		// problems when another module with its own DLLMain was added
		// to the link.  What's the best way to do this.
		// AFX_MANAGE_STATE(AfxGetStaticModuleState());
#ifdef VIS_DISPLAY_LIB		

		HICON hIcon = LoadDisplayIcon();

//#elif defined(VIS_DISPLAY_BUILD)		
#else // VIS_DISPLAY_DLL
		HICON hIcon = ::LoadIcon(VisHResourceGet(),
				MAKEINTRESOURCE(IDI_IDW_ICON));
#endif // VIS_DISPLAY_DLL
		assert(hIcon != 0);

		// Use the brush from the parent if there is a parent.
		HBRUSH hBrushBackground = 0;
		if ((PWndParent() != 0) && (PWndParent()->m_hWnd != 0))
		{
			hBrushBackground = (HBRUSH) GetClassLong(PWndParent()->m_hWnd,
					GCL_HBRBACKGROUND);
		}

		LPCTSTR szWndClass = AfxRegisterWndClass(k_nClassStyle, hCursor,
				hBrushBackground, hIcon);

		SetHasRefreshed(false);

		BOOL fRet = CreateEx(ExStyle(), szWndClass, Title(), Style(),
				RectOutside(), PWndParent(), 0, 0);

		assert(!fRet || HasWnd());

		if ((HasWnd()) && (UseStatusBar()))
		{
			if (StatusBar().Create(this, WS_CHILD | WS_VISIBLE
					/*| WS_DLGFRAME*/ | WS_DISABLED)) // mturk - took out WS_DLGFRAME
			{
				UINT n = 0;
				StatusBar().SetIndicators(&n, 1);
				StatusBar().SetPaneStyle( 0, SBPS_STRETCH);
				// LATER:  Use the appropriate default pane text for this object?
				StatusBar().SetWindowText("");

				SIZE size = StatusBar().CalcFixedLayout(true, true); 
				if (size.cy != DyStatusBar()) // mturk - this undoes the initial setting of s_dyStatusBar (if different from 18) - why?
				{
					if (!StyleChangesSizeInside())
					{
						SetHeightOutside(HeightOutside() + size.cy - DyStatusBar(),
								false);
					}

					s_dyStatusBar = size.cy;
				}

				// Put the status bar at the bottom of the window.
				CVisRect rectStatusBar = RectClientStatusBar();
				StatusBar().MoveWindow(&rectStatusBar);
			}
		}
	}

	return HasWnd();
}

#ifdef VIS_DISPLAY_LIB
HICON CVisPaneWndBase::LoadDisplayIcon() const
{
	static HICON hIcon = NULL;
	if(hIcon == NULL)
	{
		BYTE byteMask[128];
		
		ZeroMemory(byteMask, 128);

		BITMAP bmMask;

		bmMask.bmPlanes = 1;
		bmMask.bmType = 0;
		bmMask.bmHeight = 32;
		bmMask.bmWidth = 32;
		bmMask.bmWidthBytes = 4;
		bmMask.bmBitsPixel = 1;
		bmMask.bmBits = byteMask;
		


		const BYTE abHeader[] = {0x28, 0x00, 0x00, 0x00, 
								0x20, 0x00, 0x00, 0x00, 
								0x20, 0x00, 0x00, 0x00, 
								0x01, 0x00, 0x04, 0x00, 
								0x00, 0x00, 0x00, 0x00, 
								0x00, 0x02, 0x00, 0x00, 
								0xC4, 0x0E, 0x00, 0x00, 
								0xC4, 0x0E, 0x00, 0x00, 
								0x10, 0x00, 0x00, 0x00, 
								0x10, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x80, 0x00, 
								0x00, 0x80, 0x00, 0x00, 
								0x00, 0x80, 0x80, 0x00, 
								0x80, 0x00, 0x00, 0x00, 
								0x80, 0x00, 0x80, 0x00, 
								0x80, 0x80, 0x00, 0x00, 
								0x80, 0x80, 0x80, 0x00, 
								0xC0, 0xC0, 0xC0, 0x00, 
								0x00, 0x00, 0xFF, 0x00, 
								0x00, 0xFF, 0x00, 0x00, 
								0x00, 0xFF, 0xFF, 0x00, 
								0xFF, 0x00, 0x00, 0x00, 
								0xFF, 0x00, 0xFF, 0x00, 
								0xFF, 0xFF, 0x00, 0x00, 
								0xFF, 0xFF, 0xFF, 0x00 };

		const BYTE abPixels[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
								0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 
								0x0F, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
								0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
								0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x88, 0x88, 0x87, 0x87, 0x87, 0x87, 
								0x87, 0x87, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x88, 0x87, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x07, 0x77, 0x78, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x07, 0x77, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x70, 
								0x0F, 0x87, 0x80, 0x00, 0x07, 0x77, 0x77, 0x77, 
								0x77, 0x77, 0x77, 0x70, 0x00, 0x00, 0x00, 0x70, 
								0x0F, 0x78, 0x77, 0x77, 0x7F, 0x8F, 0x0F, 0x0F, 
								0x8F, 0x80, 0x8F, 0x77, 0x77, 0x70, 0x00, 0x70, 
								0x0F, 0x87, 0x77, 0xF8, 0xF7, 0x77, 0x00, 0x00, 
								0x00, 0x08, 0x08, 0xF8, 0xF8, 0x77, 0x70, 0x70, 
								0x0F, 0x77, 0x8F, 0x07, 0x00, 0x00, 0x87, 0x08, 
								0x70, 0x00, 0x80, 0x0F, 0x8F, 0x8F, 0x77, 0x70, 
								0x0F, 0x88, 0xF8, 0x00, 0x0F, 0x80, 0x00, 0x00, 
								0x00, 0x00, 0x78, 0x00, 0x08, 0xF8, 0xF8, 0x70, 
								0x0F, 0x8F, 0x80, 0x77, 0xF0, 0x00, 0x00, 0x07, 
								0x00, 0x00, 0x00, 0x8F, 0x00, 0x08, 0x8F, 0x70, 
								0x0F, 0x88, 0x07, 0x0F, 0x70, 0x00, 0x04, 0x4F, 
								0xF8, 0x40, 0x00, 0x07, 0x78, 0x80, 0x08, 0x70, 
								0x0F, 0x87, 0x77, 0x70, 0x00, 0x00, 0x77, 0x44, 
								0x44, 0x77, 0x07, 0x70, 0x77, 0x08, 0x80, 0x70, 
								0x0F, 0x07, 0x7F, 0x00, 0x70, 0x47, 0x66, 0x77, 
								0x47, 0x66, 0x70, 0x78, 0x77, 0x78, 0x08, 0x70, 
								0x0F, 0x80, 0x00, 0x08, 0xF4, 0x78, 0x68, 0x00, 
								0x04, 0x68, 0x67, 0xFF, 0xF8, 0x77, 0x87, 0x70, 
								0x0F, 0x77, 0x70, 0xFF, 0xF4, 0x67, 0x86, 0x87, 
								0x76, 0x86, 0x67, 0xFF, 0x8F, 0x87, 0x70, 0x70, 
								0x0F, 0x80, 0x00, 0x0F, 0xFF, 0x46, 0x68, 0x68, 
								0x68, 0x66, 0x48, 0xF8, 0xF8, 0x70, 0x7F, 0x70, 
								0x0F, 0x88, 0x88, 0xF0, 0x07, 0x76, 0x66, 0x66, 
								0x66, 0x67, 0x88, 0x70, 0x00, 0x78, 0x7F, 0x70, 
								0x0F, 0x78, 0x8F, 0x8F, 0x80, 0x07, 0x70, 0x70, 
								0x70, 0x70, 0x00, 0x7F, 0x8F, 0x77, 0xF7, 0x70, 
								0x0F, 0x87, 0x88, 0xF8, 0x88, 0xF0, 0x78, 0x08, 
								0x08, 0xF8, 0xF8, 0xF8, 0x77, 0x8F, 0x77, 0x70, 
								0x0F, 0x88, 0x87, 0x87, 0x8F, 0x8F, 0x8F, 0x8F, 
								0x8F, 0x87, 0x87, 0x77, 0xF8, 0xF7, 0x78, 0x70, 
								0x0F, 0x8F, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 
								0x78, 0x78, 0x7F, 0x8F, 0x8F, 0x78, 0x88, 0x70, 
								0x0F, 0x88, 0xF8, 0x88, 0x87, 0x87, 0x87, 0x87, 
								0x87, 0x88, 0x88, 0xF8, 0xF8, 0x88, 0x88, 0x70, 
								0x0F, 0x8F, 0x88, 0x8F, 0x88, 0x88, 0x88, 0x88, 
								0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x8F, 0x88, 0x88, 0xF8, 0x88, 0x88, 
								0x8F, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x8F, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 
								0x88, 0x8F, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x0F, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
								0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 
								0x08, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
								0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		HDC hdc = ::CreateCompatibleDC(0);
		HBITMAP hbmMask = CreateBitmapIndirect(&bmMask);
		// UNDONE:  Icon isn't right.  We need to fix something.

		HBITMAP hbmColor = CreateDIBitmap(hdc, (const BITMAPINFOHEADER *)abHeader,
				CBM_INIT, abPixels,
				(const BITMAPINFO *) abHeader, DIB_RGB_COLORS); 

		::DeleteDC(hdc);

		ICONINFO iconinfo;
		iconinfo.hbmColor = hbmColor;
		iconinfo.hbmMask = hbmMask;
		iconinfo.xHotspot = 16;
		iconinfo.yHotspot = 16;
		iconinfo.fIcon = TRUE;

		hIcon = CreateIconIndirect(&iconinfo);

	}
	return hIcon;
}
#endif // VIS_DISPLAY_LIB

void CVisPaneWndBase::DrawBorders(CDC& dc, bool fSolidBgColor, bool fSelected)
		const
{
    if (WidthBorder() > 0)
	{
		CBrush brush1;
		CBrush brush2;
		if ((fSelected) && (WidthBorder() <= 2))
		{
			// solid red border
			brush1.CreateSolidBrush(RGB(255,0,0));
			brush2.CreateSolidBrush(RGB(255,0,0));
		}
		else if (fSolidBgColor)
		{
			brush1.CreateSolidBrush(ColorrefBackground());
			brush2.CreateSolidBrush(ColorrefBackground());
		}
		else
		{
			brush1.CreateSolidBrush(RGB(162,162,162));
			brush2.CreateSolidBrush(RGB(222,222,222));
		}

		CVisRect rectBorder = RectClientBorder();
		CVisRect rectInside = RectClientInside();

		// Now draw the borders
		CVisPoint rgpoint[6];
		rgpoint[0].x = rectBorder.BottomRight().x;
		rgpoint[0].y = rectBorder.TopLeft().y;
		rgpoint[1].x = rectBorder.TopLeft().x;
		rgpoint[1].y = rectBorder.TopLeft().y;
		rgpoint[2].x = rectBorder.TopLeft().x;
		rgpoint[2].y = rectBorder.BottomRight().y;
		rgpoint[3].x = rectInside.TopLeft().x;
		rgpoint[3].y = rectInside.BottomRight().y;
		rgpoint[4].x = rectInside.TopLeft().x;
		rgpoint[4].y = rectInside.TopLeft().y;
		rgpoint[5].x = rectInside.BottomRight().x;
		rgpoint[5].y = rectInside.TopLeft().y;

		CRgn rgn;
		rgn.CreatePolygonRgn(rgpoint, 6, ALTERNATE);
		dc.FillRgn(&rgn, &brush1);
		rgn.DeleteObject();

		BOOL fDrawSelected = (fSelected) && (WidthBorder() > 2);
		CPen *penOld;
		if (fDrawSelected)
		{
			CPen pen(PS_SOLID, 1, RGB(255,0,0));
			penOld = dc.SelectObject(&pen);

			dc.MoveTo(rgpoint[3]+CVisPoint(-1,1));
			dc.LineTo(rgpoint[4]+CVisPoint(-1,-1));
			dc.LineTo(rgpoint[5]+CVisPoint(1,-1));
			dc.MoveTo(rgpoint[3]+CVisPoint(-2,2));
			dc.LineTo(rgpoint[4]+CVisPoint(-2,-2));
			dc.LineTo(rgpoint[5]+CVisPoint(2,-2));
			// don't deselect the pen yet, it's used again below
		}

		rgpoint[0].x = rectBorder.TopLeft().x;
		rgpoint[0].y = rectBorder.BottomRight().y;
		rgpoint[1].x = rectBorder.BottomRight().x;
		rgpoint[1].y = rectBorder.BottomRight().y;
		rgpoint[2].x = rectBorder.BottomRight().x;
		rgpoint[2].y = rectBorder.TopLeft().y ;
		rgpoint[3].x = rectInside.BottomRight().x;
		rgpoint[3].y = rectInside.TopLeft().y;
		rgpoint[4].x = rectInside.BottomRight().x;
		rgpoint[4].y = rectInside.BottomRight().y;
		rgpoint[5].x = rectInside.TopLeft().x;
		rgpoint[5].y = rectInside.BottomRight().y;

//		CRgn rgn;
		rgn.CreatePolygonRgn(rgpoint, 6, ALTERNATE);
		dc.FillRgn(&rgn, &brush2);
		rgn.DeleteObject();

		if (fDrawSelected)
		{
			dc.MoveTo(rgpoint[3]+CVisPoint(0,-1));
			dc.LineTo(rgpoint[4]+CVisPoint(0,0));
			dc.LineTo(rgpoint[5]+CVisPoint(-1,0));
			dc.MoveTo(rgpoint[3]+CVisPoint(1,-2));
			dc.LineTo(rgpoint[4]+CVisPoint(1,1));
			dc.LineTo(rgpoint[5]+CVisPoint(-2,1));

			// now deselect the pen
			dc.SelectObject(penOld)->DeleteObject();
		}
	}
}


void CVisPaneWndBase::FillBackgroundBetweenRects(CDC& dc, const RECT& rectInner,
				const RECT& rectOuter) const
{
	assert(rectInner.left >= rectOuter.left);
	if (rectInner.left > rectOuter.left)
	{
		CVisRect rect(rectOuter);
		rect.right = rectInner.left;
		dc.FillSolidRect(rect, ColorrefBackground());
	}

	assert(rectInner.top >= rectOuter.top);
	if (rectInner.top > rectOuter.top)
	{
		CVisRect rect(rectOuter);
		rect.bottom = rectInner.top;
		dc.FillSolidRect(rect, ColorrefBackground());
	}

	assert(rectInner.right <= rectOuter.right);
	if (rectInner.right < rectOuter.right)
	{
		CVisRect rect(rectOuter);
		rect.left = rectInner.right;
		dc.FillSolidRect(rect, ColorrefBackground());
	}

	assert(rectInner.bottom <= rectOuter.bottom);
	if (rectInner.bottom < rectOuter.bottom)
	{
		CVisRect rect(rectOuter);
		rect.top = rectInner.bottom;
		dc.FillSolidRect(rect, ColorrefBackground());
	}
}


void CVisPaneWndBase::DrawTextInDcRect(CDC& dc, const RECT& rectText,
		const char *szText, bool fFillBackground) const
{
	if ((rectText.right > rectText.left)
			&& (rectText.bottom > rectText.top))
	{
		if (fFillBackground)
			dc.FillSolidRect(&rectText, ColorrefBackground());

		if (szText != 0)
		{
			COLORREF colorrefBackgroundOld = dc.GetBkColor();
			dc.SetBkColor(ColorrefBackground());

			// Look at the green component of the background color to
			// choose the text color.
			// LATER:  Find a better way to choose the text color?
			COLORREF colorrefTextColorOld = dc.GetTextColor();
			if (ColorrefBackground() & 0x8000)
				dc.SetTextColor(0);
			else
				dc.SetTextColor(0xffffff);

			CVisRect rectT(rectText);
			dc.DrawText(szText, -1, &rectT, DT_CENTER | DT_WORDBREAK);

			dc.SetTextColor(colorrefTextColorOld);
			dc.SetBkColor(colorrefBackgroundOld);
		}
	}
}


void CVisPaneWndBase::SetMessageText(const char *szMessage)
{
	if (UseStatusBar())
	{
		if (szMessage != 0)
		{
			StatusBar().SetWindowText(szMessage);
		}
		else
		{
			// LATER:  Should default message text be pane title string?
			StatusBar().SetWindowText(Title());
		}
	}
	else if (IsPartOfArray())
	{
		assert(PWndParent() != 0);
		
		// LATER:  Should pane pass its default text to parent when szMessage == 0?
		((CVisPaneWndBase *) PWndParent())->SetMessageText(szMessage);
	}
}

void CVisPaneWndBase::SetEvispaneOptions(int evispane, bool fChangeStyles)
{
	if (evispane != evispaneUnknown)
	{
		m_evispane = evispane;

		// If desired, find the window styles from the EVisPane options.
		if (fChangeStyles)
			FindStyles(evispane);

		SetUseStatusBar((evispane & evispaneArrayStatusBar) != 0);

		// If the AutoDelete option is not specified, increment the reference count
		// on this object when the window is created.
		SetAutoDestroy(((m_evispane & evispaneAutoDestroy) != 0));
	}
	
	// UNDONE:  We need to make sure that styles are updated when PwndParent is set.
	// For now, we update them here to be safe.
	FindStyles(m_evispane);
}


void CVisPaneWndBase::FindStyles(int evispane)
{
	if (evispane != evispaneUnknown)
	{
		int dwStyle = 0;
		int dwExStyle = 0;

		if ((!IsPartOfArray()) && ((evispane & evispaneCanResize) != 0))
		{
			dwStyle |= WS_THICKFRAME;
		}
		else if (((IsPaneWnd())
						&& ((evispane & evispanePaneBorder) != 0))
					|| ((!IsPartOfArray())
						&& ((evispane & evispaneArrayBorder) != 0)))
		{
			dwStyle |= WS_BORDER;
		}

		if ((evispane & evispaneVisible) != 0)
		{
			dwStyle |= WS_VISIBLE;
		}

		if (PWndParent() == 0)
		{
			dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_OVERLAPPED;

			if ((evispane & evispaneTabStop) != 0)
			{
				dwStyle |= WS_TABSTOP;
			}

			if (((IsPaneWnd())
						&& (((evispane & evispaneAutoTitle) != 0)
							|| ((evispane & evispanePaneTitle) != 0)))
					|| ((!IsPartOfArray())
						&& ((evispane & evispaneArrayTitle) != 0)))
			{
				dwStyle |= WS_CAPTION;
			}
			else
			{
				dwStyle |= WS_POPUP;
			}

			if ((evispane & evispaneCanMaximize) != 0)
			{
				dwStyle |= WS_MAXIMIZEBOX;
			}

			if ((evispane & evispaneTopMost) != 0)
			{
				dwExStyle |= WS_EX_TOPMOST;
			}
		}
		else
		{
			dwStyle |= WS_CHILD;

			if ((IsPartOfArray()) || ((evispane & evispaneTabStop) != 0))
			{
				dwStyle |= WS_TABSTOP;
			}

			if (((IsPaneWnd())
						&& (((evispane & evispaneAutoTitle) != 0)
							|| ((evispane & evispanePaneTitle) != 0)))
					|| ((!IsPartOfArray())
						&& ((evispane & evispaneArrayTitle) != 0)))
			{
				dwStyle |= WS_CAPTION;
			}
		}

		// Some window style options are not supported yet.
		if ((evispane & evispaneScrollImage) != 0)
		{
			// Not implemented yet.
			// LATER:  Try to support this option.
			assert(0);
		}
		
		SetStyle(dwStyle);
		SetExStyle(dwExStyle);
	}
}


void CVisPaneWndBase::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CVisRefCntWnd::OnSizing(fwSide, pRect);
	
	if (UseStatusBar())
	{
		CVisRect rectWindow;
		GetWindowRect(&rectWindow);
		if (rectWindow != *pRect)
		{
			CVisRect rectClient;
			GetClientRect(&rectClient);
			if (rectWindow.left != pRect->left)
				rectClient.left += (pRect->left - rectWindow.left);
			if (rectWindow.top != pRect->top)
				rectClient.top += (pRect->top - rectWindow.top);
			if (rectWindow.right != pRect->right)
				rectClient.right += (pRect->right - rectWindow.right);
			if (rectWindow.bottom != pRect->bottom)
				rectClient.bottom += (pRect->bottom - rectWindow.bottom);

			SIZE size = StatusBar().CalcDynamicLayout(100, LM_STRETCH | LM_HORZ);
			size = StatusBar().CalcFixedLayout(true, true);

			CVisRect rectStatusBar = rectClient;
			rectStatusBar.top = rectStatusBar.bottom - size.cy;
			StatusBar().MoveWindow(&rectStatusBar);
		}
	}
}


void CVisPaneWndBase::OnNcLButtonDown(UINT nHitTest, CVisPoint point) 
{
	// Don't accept the NCLButtonDown message if this pane is part
	// of an array of panes.  This prevents the user from moving the
	// panes in an array of panes.
	if (((EvispaneOptions() & evispaneNoMove) == 0)
			&& (!IsPartOfArray()))
	{
		CVisRefCntWnd::OnNcLButtonDown(nHitTest, point);
	}
}


void CVisPaneWndBase::OnNcLButtonDblClk(UINT nHitTest, CVisPoint point) 
{
	// Don't accept the NCLButtonDblClk message if this pane is part
	// of an array of panes.  This prevents the user from maximizing the
	// panes in an array of panes.
	if (((EvispaneOptions() & evispaneCanMaximize) != 0)
			&& (!IsPartOfArray()))
	{
		CVisRefCntWnd::OnNcLButtonDblClk(nHitTest, point);
	}
}


////////////////////////////////////////////////////////////////////////////
// CVisPaneWnd

CVisPaneWnd::CVisPaneWnd(int evispane, CWnd *pwndParent)
  : CVisPaneWndBase(evispane, false,
			((evispane & evispaneAutoDestroy) != 0),
			((evispane & evispaneArrayStatusBar) != 0)),
	m_pimageOriginal(0),
	m_pimageScreen(0),
	m_pointImageOffset(0, 0),
	m_evisnormalize(evisnormAdjustInts),  // Should this be evisnormDefault?
	m_dblMin(1.0),
	m_dblMax(- 1.0),
	m_evismouseModifierKeysCur(evismouseNoModifiers),
	m_fIsSelected(false),
	m_fUseScrnBuf(true),
	m_dequeDraw(),
	m_fCouldNotDisplayImage(false),
	m_mirrorImage(false)
{
	SetPWndParent(pwndParent);

	// Initialize the lists of mouse event handlers.
	for (int iMouseEvent = 0; iMouseEvent < ecMouseEvent; ++iMouseEvent)
		m_rgpMouseEvent[iMouseEvent] = 0;

	// Call SetEvispaneOptions here to make sure standard mouse event
	// handlers get added.
	SetEvispaneOptions(evispane, false);
}


CVisPaneWnd::~CVisPaneWnd(void)
{
	// Destroy the window if it's still up.  (This will also be done
	// in the base class's dtor, but we do it here to make the cleanup
	// code below easier.)
	if (HasWnd())
		DestroyWnd();

	// Remove all mouse event handlers.
	for (int iMouseEvent = 0; iMouseEvent < ecMouseEvent; ++iMouseEvent)
	{
		SVisMouseEvent *pmouseevent = m_rgpMouseEvent[iMouseEvent];

		while (pmouseevent != 0)
		{
			SVisMouseEvent *pmouseeventPrev = pmouseevent;
			pmouseevent = pmouseevent->m_pmouseeventNext;

			delete pmouseeventPrev;
		}

		m_rgpMouseEvent[iMouseEvent] = 0;
	}

	DeleteOriginalImage();
	DeleteScreenImage();
}


void CVisPaneWnd::SetImageOffset(int dx, int dy, bool fRefresh)
{
	if ((dx != m_pointImageOffset.x) || (dy != m_pointImageOffset.y))
	{
		m_pointImageOffset.x = dx;
		m_pointImageOffset.y = dy;

		SetHasRefreshed(false);
		if (fRefresh)
			Refresh();
	}
}


void CVisPaneWnd::SetIsSelected(bool fIsSelected, bool fUpdateNow)
{
	if ((!m_fIsSelected)
			&& (fIsSelected)
			&& (IsPartOfArray())
			&& (!((CVisPaneArrayWnd *) PWndParent())->AllowMultiSelect()))
	{
		((CVisPaneArrayWnd *) PWndParent())->ClearSelections(fUpdateNow);
	}

	bool fChangedSelection = (((m_fIsSelected) && (!fIsSelected))
			|| ((!m_fIsSelected) && (fIsSelected)));
	m_fIsSelected = fIsSelected;

	// If we change the selection and there's a border, we need to refresh
	// the window.
	if ((fChangedSelection) && (WidthBorder() > 0))
	{
		Refresh(fUpdateNow);
	}
}


void CVisPaneWnd::Clear(bool fRemovePointsAndLines, bool fUpdateNow)
{
	DeleteOriginalImage();
	DeleteScreenImage();
	m_fCouldNotDisplayImage = false;

	if (fRemovePointsAndLines)
		m_dequeDraw.clear();

	SetHasRefreshed(false);
	Refresh(fUpdateNow);
}


void CVisPaneWnd::Display(const CVisImageBase& refimage, POINT pointImageOffset,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	DeleteOriginalImage();
	DeleteScreenImage();
	m_fCouldNotDisplayImage = false;

	if (((EvispaneOptions() & evispaneStretchToFit) != 0)
			|| ((EvispaneOptions() & evispaneStretchPreserveAspect) != 0))
	{
		// Don't allow an image offset with these options.
		SetImageOffset(CVisPoint(0, 0), false);
	}
	else
	{
		SetImageOffset(pointImageOffset, false);
	}

	if (refimage.IsValid())
	{
		if ((WidthInside() == 0) || ((EvispaneOptions() & evispaneResizeToFit) != 0))
		{
			SetWidthInside(refimage.Width(), false);
		}

		if ((HeightInside() == 0) || ((EvispaneOptions() & evispaneResizeToFit) != 0))
		{
			SetHeightInside(refimage.Height(), false);
		}

		if ((!HasWnd()) && (fCreateWndIfNeeded))
		{
			CreateWnd();
		}
	}

	bool fAlreadyRefreshed = false;

	CVisImageBase *pimage = 0;
	switch (evispanedisp)
	{
	case evispanedispCopyOrigUseScrnBuf:
	case evispanedispCopyOrigNoScrnBuf:
		pimage = refimage.Clone(&(refimage.Shape()));
		pimage->Copy(refimage);
		SetOriginalImage(pimage);
		m_fUseScrnBuf = (evispanedisp == evispanedispCopyOrigUseScrnBuf);
		break;

	case evispanedispAliasOrigUseScrnBuf:
	case evispanedispAliasOrigNoScrnBuf:
		SetOriginalImage(refimage.Clone());
		m_fUseScrnBuf = (evispanedisp == evispanedispCopyOrigUseScrnBuf);
		break;

	case evispanedispNoOrigUseScrnBuf:
	case evispanedispNoOrigNoScrnBuf:
		SetOriginalImage(refimage.Clone());
		m_fUseScrnBuf = (evispanedisp == evispanedispNoOrigUseScrnBuf);
		SetHasRefreshed(false);
		Refresh(true);
		fAlreadyRefreshed = true;
		DeleteOriginalImage();
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	if (((EvispaneOptions() & evispaneAutoTitle) != 0)
			&& (refimage.Name(false) != 0))
	{
		SetTitle(refimage.Name(false));
	}

	if (!fAlreadyRefreshed)
	{
		SetHasRefreshed(false);
		Refresh(fUpdateNow);
	}
}


void CVisPaneWnd::AddPoint(int x, int y, int nPenStyle, int dxThickness,
		COLORREF colorref, bool fRefresh)
{
	SVisDraw draw;
	draw.m_evisdraw = evisdrawPoint;
	draw.m_wPenStyle = nPenStyle;
	draw.m_wPenWidth = dxThickness;
	draw.m_colorref = colorref;
	draw.m_point1.x = x;
	draw.m_point1.y = y;
	draw.m_point2.x = 0;
	draw.m_point2.y = 0;

	m_dequeDraw.push_back(draw);

	// Make sure that the screen image will be redrawn with to
	// reflect this change.
	// LATER:  There are cases when we don't want to delete the screen image, e.g. when
    //  displaying graphics only (no image)
	DeleteScreenImage();

	SetHasRefreshed(false);
	if (fRefresh)
		Refresh();
}


void CVisPaneWnd::AddLine(int x1, int y1, int x2, int y2, int nPenStyle,
		int dxThickness, COLORREF colorref, bool fRefresh)
{
	SVisDraw draw;
	draw.m_evisdraw = evisdrawLine;
	draw.m_wPenStyle = nPenStyle;
	draw.m_wPenWidth = dxThickness;
	draw.m_colorref = colorref;
	draw.m_point1.x = x1;
	draw.m_point1.y = y1;
	draw.m_point2.x = x2;
	draw.m_point2.y = y2;

	m_dequeDraw.push_back(draw);

	// Make sure that the screen image will be redrawn with to
	// reflect this change.
	// LATER:  There are cases when we don't want to delete the screen image, e.g. when
    //  displaying graphics only (no image)
	DeleteScreenImage();

	SetHasRefreshed(false);
	if (fRefresh)
		Refresh();
}

// Added so we can draw directly on the pane without using the drawing queue
// We need to rethink the line and point (and ?) drawing...
void CVisPaneWnd::DrawLine(int x1, int y1, int x2, int y2, int nPenStyle,
		int dxThickness, COLORREF colorref)
{
	CClientDC dc(this);
    CPen pen(PS_SOLID, dxThickness, colorref);
	CVisRect rect;
	GetClientRect(&rect);
    CVisPoint point(x1 + rect.TopLeft().x, y1 + rect.TopLeft().y);
    CPen *oldpen = dc.SelectObject(&pen);
    dc.DPtoLP(&point);
    dc.MoveTo(point);
    dc.LineTo(point+CVisPoint(x2-x1, y2-y1));
    dc.SelectObject(oldpen);
}


void CVisPaneWnd::RemovePoint(int x, int y, bool fRefresh)
{
	for (int i = m_dequeDraw.size() - 1; i >= 0; --i)
	{
		const SVisDraw& refdraw = m_dequeDraw[i];

		if ((refdraw.m_evisdraw == evisdrawPoint)
				&& (refdraw.m_point1.x == x)
				&& (refdraw.m_point1.y == y))
		{
			m_dequeDraw.erase(m_dequeDraw.begin() + i);
			break;
		}
	}

	// Make sure that the screen image will be redrawn with to
	// reflect this change.
	// LATER:  There are cases when we don't want to delete the screen image, e.g. when
    //  displaying graphics only (no image)
	DeleteScreenImage();

	SetHasRefreshed(false);
	if (fRefresh)
		Refresh();
}


void CVisPaneWnd::RemoveLine(int x1, int y1, int x2, int y2, bool fRefresh)
{
	for (int i = m_dequeDraw.size() - 1; i >= 0; --i)
	{
		const SVisDraw& refdraw = m_dequeDraw[i];

		if ((refdraw.m_evisdraw == evisdrawLine)
				&& (refdraw.m_point1.x == x1)
				&& (refdraw.m_point1.y == y1)
				&& (refdraw.m_point2.x == x2)
				&& (refdraw.m_point2.y == y2))
		{
			m_dequeDraw.erase(m_dequeDraw.begin() + i);
			break;
		}
	}

	// Make sure that the screen image will be redrawn with to
	// reflect this change.
	// LATER:  There are cases when we don't want to delete the screen image, e.g. when
    //  displaying graphics only (no image)
	DeleteScreenImage();

	SetHasRefreshed(false);
	if (fRefresh)
		Refresh();
}


void CVisPaneWnd::RemoveAllPointsAndLines(bool fRefresh)
{
	if (!m_dequeDraw.empty())
	{
		m_dequeDraw.clear();

		// Make sure that the screen image will be redrawn with to
		// reflect this change.
        // LATER:  There are cases when we don't want to delete the screen image, e.g. when
        //  displaying graphics only (no image)
		DeleteScreenImage();
	}

	SetHasRefreshed(false);
	if (fRefresh)
		Refresh();
}


void CVisPaneWnd::AddMouseEventCallback(int evismouse,
		VisPfnMouseEventCallback pfnCallback, const char *sz,
		void *pvUser)
{
	int iMouse = IGetMouse(evismouse);

	SVisMouseEvent *pmouseevent = new SVisMouseEvent;
	if (sz != 0)
		pmouseevent->m_string = sz;
	pmouseevent->m_pvUser = pvUser;
	pmouseevent->m_pfnCallback = pfnCallback;
	pmouseevent->m_pmouseeventNext = m_rgpMouseEvent[iMouse];

	m_rgpMouseEvent[iMouse] = pmouseevent;
}


void CVisPaneWnd::RemoveMouseEventCallback(int evismouse,
		VisPfnMouseEventCallback pfnCallback, void *pvUser)
{
	int iMouseFirst;
	int iMouseLim;
	
	if (evismouse == 0)
	{
		iMouseFirst = 0;
		iMouseLim = ecMouseEvent;
	}
	else
	{
		iMouseFirst = IGetMouse(evismouse);
		iMouseLim = iMouseFirst + 1;
	}

	for (int iMouse = iMouseFirst; iMouse < iMouseLim; ++iMouse)
	{
		bool fRemovedMouseEvent = false;
		SVisMouseEvent *pmouseeventPrev = 0;
		SVisMouseEvent *pmouseevent = m_rgpMouseEvent[iMouse];

		while (pmouseevent != 0)
		{
			if ((pmouseevent->m_pfnCallback == pfnCallback)
					&& (pmouseevent->m_pvUser == pvUser))
			{
				// Remove the mouse event callback.
				if (pmouseeventPrev == 0)
				{
					m_rgpMouseEvent[iMouse] = pmouseevent->m_pmouseeventNext;
				}
				else
				{
					pmouseeventPrev->m_pmouseeventNext
							= pmouseevent->m_pmouseeventNext;
				}
				delete pmouseevent;

				fRemovedMouseEvent = true;
				break;
			}

			pmouseeventPrev = pmouseevent;
			pmouseevent = pmouseevent->m_pmouseeventNext;
		}

		if (fRemovedMouseEvent)
			break;
	}
}

void CVisPaneWnd::SetMirrorImage(bool fMirror)
{
	m_mirrorImage = fMirror;
}

bool CVisPaneWnd::GetMirrorImage() const
{
	return m_mirrorImage;
}


void CVisPaneWnd::SetEvispaneOptions(int evispane, bool fChangeStyles)
{
	CVisPaneWndBase::SetEvispaneOptions(evispane, fChangeStyles);

	if (EvispaneOptions() != evispaneUnknown)
	{
		// Use a private pvUser value to distinguish between handlers added
		// by this class and handlers added by the user.
		void *pvInternalID = (void *) (((BYTE *) this) + 3);

		if ((EvispaneOptions() & evispaneSelectHandler) != 0)
		{
			// Call RemoveMouseEventCallback here to make sure that we
			// don't accidentally add two copies of the same event handler.
			RemoveMouseEventCallback(evismouseLButtonDown,
					CVisPaneWnd::SelectPaneCallback, pvInternalID);

			AddMouseEventCallback(evismouseLButtonDown,
					CVisPaneWnd::SelectPaneCallback, "Select Pane",
					pvInternalID);
		}
		else
		{
			RemoveMouseEventCallback(evismouseLButtonDown,
					CVisPaneWnd::SelectPaneCallback, pvInternalID);
		}

		if ((EvispaneOptions() & evispaneSelectHandler) != 0)
		{
			// Call RemoveMouseEventCallback here to make sure that we
			// don't accidentally add two copies of the same event handler.
			RemoveMouseEventCallback(evismouseMouseMove | evismouseNoButton,
					CVisPaneWnd::ShowPixelValuesCallback, pvInternalID);

			AddMouseEventCallback(evismouseMouseMove | evismouseNoButton,
					CVisPaneWnd::ShowPixelValuesCallback, "Show Values",
					pvInternalID);
		}
		else
		{
			RemoveMouseEventCallback(evismouseMouseMove | evismouseNoButton,
					CVisPaneWnd::ShowPixelValuesCallback, pvInternalID);
		}
	}
}


void CVisPaneWnd::SetOriginalImage(CVisImageBase *pimage)
{
	DeleteOriginalImage();

	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneImageMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pimageOriginal = pimage;

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


void CVisPaneWnd::SetScreenImage(CVisImageBase *pimage)
{
	DeleteScreenImage();

	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneImageMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pimageScreen = pimage;

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


void CVisPaneWnd::DeleteOriginalImage(void)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneImageMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	if (m_pimageOriginal != 0)
	{
		CVisImageBase *pimage = m_pimageOriginal;
		m_pimageOriginal = 0;
		delete pimage;
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


void CVisPaneWnd::DeleteScreenImage(void)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneImageMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	if (m_pimageScreen != 0)
	{
		CVisImageBase *pimage = m_pimageScreen;
		m_pimageScreen = 0;
		delete pimage;
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


void CVisPaneWnd::FindImageRects(CVisRect& rectSrc, CVisRect& rectDest)
{
	if (((m_pimageOriginal == 0) && (m_pimageScreen == 0))
			|| (!m_pimageOriginal->IsValid()))
	{
		rectSrc = CVisRect(0, 0, 0, 0);
		rectDest = rectSrc;
	}
	else
	{
		rectDest = RectClientInside();
		if (m_pimageOriginal != 0)
			rectSrc = m_pimageOriginal->Rect();
		else
			rectSrc = m_pimageScreen->Rect();

		if ((EvispaneOptions() & evispaneStretchPreserveAspect) != 0)
		{
			if ((rectDest.Width() > 0) && (rectDest.Height() > 0)
					&& (rectSrc.Width() > 0) && (rectSrc.Height() > 0))
			{
				if (rectDest.Width() * rectSrc.Height()
						> rectSrc.Width() * rectDest.Height())
				{
					int dx = rectSrc.Width() * rectDest.Height()
							/ rectSrc.Height();
					rectDest.left += ((rectDest.Width() - dx) / 2);
					rectDest.right = rectDest.left + dx;
				}
				else
				{
					int dy = rectDest.Width() * rectSrc.Height()
							/ rectSrc.Width();
					rectDest.top += ((rectDest.Height() - dy) / 2);
					rectDest.bottom = rectDest.top + dy;
				}
			}
		}
		else if ((EvispaneOptions() & evispaneStretchToFit) == 0)
		{
			if (ImageOffset().x >= 0)
				rectDest.left += ImageOffset().x;
			else
				rectSrc.left -= ImageOffset().x;
			if (ImageOffset().y >= 0)
				rectDest.top += ImageOffset().y;
			else
				rectSrc.top -= ImageOffset().y;

			if (rectSrc.Width() >= rectDest.Width())
				rectSrc.right = rectSrc.left + rectDest.Width();
			else
				rectDest.right = rectDest.left + rectSrc.Width();
			if (rectSrc.Height() >= rectDest.Height())
				rectSrc.bottom = rectSrc.top + rectDest.Height();
			else
				rectDest.bottom = rectDest.top + rectSrc.Height();
		}
	}
}



// @mfunc Redraws the window, pane border, and if valid the image otherwise the 
//        background color is used.
void CVisPaneWnd::OnPaint()
{
//	assert(IsValid());

	CVisRect rect;
	if ((GetUpdateRect(&rect))
			&& (rect.Width() > 0)
			&& (rect.Height() > 0))
	{
		PAINTSTRUCT paintstruct;
		CDC *pdc = BeginPaint(&paintstruct);
		OnDraw(pdc);
		EndPaint(&paintstruct);
	}
}


void CVisPaneWnd::OnDraw(CDC* pDC)
{
	DrawImageAndPointsAndLines(*pDC);

	// LATER:  Is this needed?
	//if (UseStatusBar())
	//	StatusBar().RedrawWindow();

	DrawBorders(*pDC, false, IsSelected());

	SetHasRefreshed(true);
}

// @mfunc Draws image and border in window client area.
void CVisPaneWnd::DrawImageAndPointsAndLines(
                                   CDC& dc) // @parm display context in which to draw
{
	bool fDisplayedImage = false;
	
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneImageMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	CVisRect rectClientImage = RectClientInside();
	CVisRect rectImageSrc;
	CVisRect rectImageDest;
	FindImageRects(rectImageSrc, rectImageDest);

	if ((rectImageSrc.Width() > 0)
			&& (rectImageSrc.Height() > 0))
	{
		assert((m_pimageOriginal != 0) || (m_pimageScreen != 0));

		if (m_pimageScreen != 0)
		{
			assert(m_fUseScrnBuf);
			assert(m_pimageOriginal->Rect() == m_pimageScreen->Rect());
			fDisplayedImage = m_pimageScreen->DisplayInHdc(
					dc.GetSafeHdc(), rectImageSrc, rectImageDest, SRCCOPY, 0, GetMirrorImage());
		}
		else if ((FCanDirectlyDisplayImage(*m_pimageOriginal))
				&& ((!m_fUseScrnBuf) || (m_dequeDraw.empty())))
		{
			fDisplayedImage = m_pimageOriginal->DisplayInHdc(
					dc.GetSafeHdc(), rectImageSrc, rectImageDest, SRCCOPY, 0, GetMirrorImage());

			assert((!m_fUseScrnBuf) || (m_dequeDraw.empty()));
			if (!m_dequeDraw.empty())
				DrawPointsAndLines(dc, rectImageSrc.TopLeft() - rectImageDest.TopLeft());
		}
		else if (!m_fCouldNotDisplayImage)
		{
			DeleteScreenImage();

			SetScreenImage(new CVisRGBAByteImage(m_pimageOriginal->Rect()));

			// LATER:  Better OOM error handling.
			if (m_pimageScreen != 0)
			{
				bool fCopiedPixels = false;
				if (m_pimageOriginal->NBands() == 1)
				{
					fCopiedPixels = m_pimageOriginal->CopyPixelsTo(
							*m_pimageScreen, m_evisnormalize,
							m_dblMin, m_dblMax);
				}
				else
				{
					fCopiedPixels = m_pimageOriginal->CopyPixelsTo(0, 0,
							*m_pimageScreen, m_evisnormalize,
							m_dblMin, m_dblMax);
				}

				if (fCopiedPixels)
				{
					HDC hdc = m_pimageScreen->Hdc();
					
					if (hdc != 0)
					{
						DrawPointsAndLines(*CDC::FromHandle(hdc),
								rectImageSrc.TopLeft());
					}

					fDisplayedImage = m_pimageScreen->DisplayInHdc(
							dc.GetSafeHdc(), rectImageSrc, rectImageDest, SRCCOPY, 0, GetMirrorImage());

					if (hdc != 0)
					{
						m_pimageScreen->DestroyHdc();
					}
					else
					{
						DrawPointsAndLines(dc, rectImageSrc.TopLeft());
					}
				}

				if ((!m_fUseScrnBuf) || (!fCopiedPixels))
				{
					// Delete the temporary image.
					DeleteScreenImage();
				}
			}
		}

		if (fDisplayedImage)
		{
			if ((EvispaneOptions() & evispaneStretchToFit) == 0)
				FillBackgroundBetweenRects(dc, rectImageDest, rectClientImage);
		}
		else
		{
			m_fCouldNotDisplayImage = true;

			if ((EvispaneOptions() & evispaneBadImageErrorInWindow) != 0)
			{
				DrawTextInDcRect(dc, rectClientImage,
						"Error:  Image could not be displayed.");
			}
			else
			{
				dc.FillSolidRect(rectClientImage, ColorrefBackground());
			}
		}
	}
	else if ((m_pimageOriginal != 0)
			&& (!m_pimageOriginal->IsValid())
			&& ((EvispaneOptions() & evispaneBadImageErrorInWindow) != 0))
	{
		DrawTextInDcRect(dc, rectClientImage, "Error:  Invalid image.");
	}
	else if ((m_pimageOriginal == 0) && (m_pimageScreen == 0))
	{
		if (!m_dequeDraw.empty())
		{
			dc.FillSolidRect(rectClientImage, ColorrefBackground());
			DrawPointsAndLines(dc);
		}
		else if ((EvispaneOptions() & evispaneNoImageErrorInWindow) != 0)
		{
			DrawTextInDcRect(dc, rectClientImage, "Error:  No image.");
		}
		else
		{
			dc.FillSolidRect(rectClientImage, ColorrefBackground());
		}
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


void CVisPaneWnd::DrawPointsAndLines(CDC& dc, CVisPoint pointOffset)
{
	if (!m_dequeDraw.empty())
	{
		std::deque<SVisDraw>::iterator iteratorCur = m_dequeDraw.begin();
		std::deque<SVisDraw>::iterator iteratorLim = m_dequeDraw.end();
		for (; iteratorCur < iteratorLim; ++iteratorCur)
		{
			// LATER:  Do we need to call DPtoLP to convert the points and lines?
			CVisPoint pointT;
			if (iteratorCur->m_evisdraw == evisdrawPoint)
			{
				CPen pen(iteratorCur->m_wPenStyle, iteratorCur->m_wPenWidth,
						iteratorCur->m_colorref);
				CPen *oldpen = dc.SelectObject(&pen);

				pointT = iteratorCur->m_point1;
				pointT.x -= pointOffset.x;
				pointT.y -= pointOffset.y;
				dc.MoveTo(pointT);

				if (iteratorCur->m_wPenWidth <= 1)
					++ pointT.y;
				dc.LineTo(pointT);

				dc.SelectObject(oldpen);
			}
			else
			{
				assert(iteratorCur->m_evisdraw == evisdrawLine);

				CPen pen(iteratorCur->m_wPenStyle, iteratorCur->m_wPenWidth,
						iteratorCur->m_colorref);
				CPen *oldpen = dc.SelectObject(&pen);

				pointT = iteratorCur->m_point1;
				pointT.x -= pointOffset.x;
				pointT.y -= pointOffset.y;
				dc.MoveTo(pointT);

				pointT = iteratorCur->m_point2;
				pointT.x -= pointOffset.x;
				pointT.y -= pointOffset.y;
				dc.LineTo(pointT);

				dc.SelectObject(oldpen);
			}
		}
	}
}


bool CVisPaneWnd::FCanDirectlyDisplayImage(const CVisImageBase& refimage)
{
	// Note:  CVisYUVABytePixel images are only displayed as grayscale.
	return (((refimage.NBands() == 1)
				&& ((refimage.PixelTypeInfo() == typeid(BYTE))
					|| (refimage.PixelTypeInfo() == typeid(CVisRGBABytePixel))
					|| (refimage.PixelTypeInfo() == typeid(CVisYUVABytePixel))
					|| (refimage.PixelTypeInfo() == typeid(short))
					|| (refimage.PixelTypeInfo() == typeid(unsigned short))
					|| (refimage.PixelTypeInfo() == typeid(unsigned long))))
			|| (((refimage.NBands() == 3) || (refimage.NBands() == 4))
				&& (refimage.PixelTypeInfo() == typeid(BYTE))));
}



////////////////////////////////////////////////////////////////////////////
// Mouse handlers


int CVisPaneWnd::IGetMouse(int evismouse)
{
	int iRet;

	switch (evismouse & evismouseMaskAction)
	{
	case evismouseButtonDown:
		iRet = 0;
		break;

	case evismouseButtonUp:
		iRet = 10;
		break;

	case evismouseDoubleClick:
		iRet = 20;
		break;

	case evismouseMouseMove:
		iRet = 30;
		break;

#ifdef _DEBUG
	default:
		// We shouldn't get here!
		assert(0);
		break;
#endif // _DEBUG
	}

	switch (evismouse & evismouseMaskButton)
	{
	case evismouseLeftButton:
		// iRet += 0;
		break;

	case evismouseRightButton:
		iRet += 5;
		break;

	case 0:
		// Any button for move, left button in other cases.
		if ((evismouse & evismouseMaskAction) == evismouseMouseMove)
			iRet += 10;
		break;

	case evismouseNoButton:
		assert((evismouse & evismouseMaskAction) == evismouseMouseMove);
		iRet += 15;
		break;

	case evismouseBothButtons:
		assert((evismouse & evismouseMaskAction) == evismouseMouseMove);
		iRet += 20;
		break;

#ifdef _DEBUG
	default:
		// We shouldn't get here!
		assert(0);
		break;
#endif // _DEBUG
	}

	switch (evismouse & evismouseMaskModifier)
	{
	case 0:
		// iRet += 0;
		break;

	case evismouseNoModifiers:
		iRet += 1;
		break;

	case evismouseShift:
		iRet += 2;
		break;

	case evismouseControl:
		iRet += 3;
		break;

	case evismouseCtrlShift:
		iRet += 4;
		break;

#ifdef _DEBUG
	default:
		// We shouldn't get here!
		assert(0);
		break;
#endif // _DEBUG
	}

	return iRet;
}


EVisMouse CVisPaneWnd::MouseModifierKeysGetNFlags(UINT nFlags,
		bool fCheckButton)
{
	EVisMouse evismouse;

    UINT nFlagsMasked = (nFlags & (MK_SHIFT | MK_CONTROL));
	if (nFlagsMasked == 0)
	{
		evismouse = evismouseNoModifiers;
	}
	else if (nFlagsMasked == (MK_SHIFT | MK_CONTROL))
	{
		evismouse = evismouseCtrlShift;
	}
	else if (nFlagsMasked == MK_SHIFT)
	{
		evismouse = evismouseShift;
	}
	else
	{
		assert(nFlagsMasked == MK_CONTROL);

		evismouse = evismouseControl;
	}

	if (fCheckButton)
	{
		nFlagsMasked = (nFlags & (MK_LBUTTON | MK_RBUTTON));
		if (nFlagsMasked == 0)
		{
			evismouse = (EVisMouse) (evismouse | evismouseNoButton);
		}
		else if (nFlagsMasked == (MK_LBUTTON | MK_RBUTTON))
		{
			evismouse = (EVisMouse) (evismouse | evismouseBothButtons);
		}
		else if (nFlagsMasked == MK_RBUTTON)
		{
			evismouse = (EVisMouse) (evismouse | evismouseRightButton);
		}
		else
		{
			evismouse = (EVisMouse) (evismouse | evismouseLeftButton);
		}
	}

	return evismouse;
}


void CVisPaneWnd::OnMouseEvent(int evismouse, UINT nFlags, CVisPoint point) 
{
	int dxBorder = WidthBorder();

	// Only accept if inside the image area
	bool fOutside = false;
	CPoint p(dxBorder, dxBorder);
	if (point.x < p.x || point.y < p.y)
		fOutside = true;
	CPoint p2 = p + SizeInside();
	if (point.x >= p2.x || point.y >= p2.y)
		fOutside = true;

// UNDONE:  This is a temporary workaround.
#if !VIS_USER_BUILD_mturk

	// Only accept if inside the image area
	if (fOutside)
		return;

#endif // !VIS_USER_BUILD_mturk

	// Account for pane border
	point = point - ImageOffset() - CVisPoint(dxBorder, dxBorder);
	if (HasValidImage())
		point += Image().StartPoint();

	// First do the handlers for the specified modifier key combination.
	assert((evismouse & evismouseMaskModifier) != 0);
	int iMouse = IGetMouse(evismouse);
	SVisMouseEvent *pmouseevent = m_rgpMouseEvent[iMouse];
	while (pmouseevent != 0)
	{
		// Find the next event before calling the current event to allow the
		// current event to remove itself from the list.
		SVisMouseEvent *pmouseeventNext = pmouseevent->m_pmouseeventNext;

// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk

		pmouseevent->m_pfnCallback(nFlags, point, this,
				pmouseevent->m_pvUser, fOutside);

#else // VIS_USER_BUILD_mturk

		pmouseevent->m_pfnCallback(nFlags, point, this,
				pmouseevent->m_pvUser);

#endif // VIS_USER_BUILD_mturk

		pmouseevent = pmouseeventNext;
	}

	// Then do the handlers for any modifier key combination.
	int evismouseAnyKeys = (evismouse & ~evismouseMaskModifier);
	iMouse = IGetMouse(evismouseAnyKeys);
	pmouseevent = m_rgpMouseEvent[iMouse];
	while (pmouseevent != 0)
	{
		// Find the next event before calling the current event to allow the
		// current event to remove itself from the list.
		SVisMouseEvent *pmouseeventNext = pmouseevent->m_pmouseeventNext;

// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk

		pmouseevent->m_pfnCallback(nFlags, point, this,
				pmouseevent->m_pvUser, fOutside);

#else // VIS_USER_BUILD_mturk

		pmouseevent->m_pfnCallback(nFlags, point, this,
				pmouseevent->m_pvUser);

#endif // VIS_USER_BUILD_mturk

		pmouseevent = pmouseeventNext;
	}
}


void CVisPaneWnd::OnLButtonUp(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseButtonUp | evismouseLeftButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnLButtonDown(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseButtonDown | evismouseLeftButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnLButtonDblClk(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseDoubleClick | evismouseLeftButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnRButtonUp(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseButtonUp | evismouseRightButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnRButtonDown(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseButtonDown | evismouseRightButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnRButtonDblClk(UINT nFlags, CVisPoint point) 
{
	OnMouseEvent(evismouseDoubleClick | evismouseRightButton
			| MouseModifierKeysGetNFlags(nFlags, false), nFlags, point);
}


void CVisPaneWnd::OnMouseMove(UINT nFlags, CVisPoint point) 
{
	// First call handlers that specify a button state.
	OnMouseEvent(MouseModifierKeysGetNFlags(nFlags, true)
			| evismouseMouseMove, nFlags, point);

	// Then call handlers that work for any button state.
	OnMouseEvent(MouseModifierKeysGetNFlags(nFlags, false)
			| evismouseMouseMove, nFlags, point);
}


void CVisPaneWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// LATER:  We don't seem to be getting these messages in the ExArrayOfPanes
	// test program.  Maybe they're only going to the parent window?  (Are these
	// handlers really needed?)
	if ((nChar == VK_SHIFT) || (nChar == VK_CONTROL))
	{
		int evismouseModifierKeysPrev = m_evismouseModifierKeysCur;

		if (m_evismouseModifierKeysCur == evismouseNoModifiers)
			m_evismouseModifierKeysCur = 0;

		assert((evismouseShift | evismouseControl) == evismouseCtrlShift);
		
		if (nChar == VK_SHIFT)
			m_evismouseModifierKeysCur |= evismouseShift;
		else
			m_evismouseModifierKeysCur |= evismouseControl;

		if (evismouseModifierKeysPrev != m_evismouseModifierKeysCur)
			ShowEventHandlerMessage();
	}

    CVisPaneWndBase::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CVisPaneWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// LATER:  We don't seem to be getting these messages in the ExArrayOfPanes
	// test program.  Maybe they're only going to the parent window?  (Are these
	// handlers really needed?)
	if ((nChar == VK_SHIFT) || (nChar == VK_CONTROL))
	{
		int evismouseModifierKeysPrev = m_evismouseModifierKeysCur;

		assert((evismouseShift | evismouseControl) == evismouseCtrlShift);
		
		if (nChar == VK_SHIFT)
			m_evismouseModifierKeysCur &= ~evismouseShift;
		else
			m_evismouseModifierKeysCur &= ~evismouseControl;

		if (m_evismouseModifierKeysCur == 0)
			m_evismouseModifierKeysCur = evismouseNoModifiers;

		if (evismouseModifierKeysPrev != m_evismouseModifierKeysCur)
			ShowEventHandlerMessage();
	}

	CVisPaneWndBase::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CVisPaneWnd::FindButtonMessage(EVisMouse evismouseButton,
			CString& refstringMessage) const
{
	int iMouse = IGetMouse(evismouseButton | m_evismouseModifierKeysCur);
	if (m_rgpMouseEvent[iMouse] == 0)
		iMouse = IGetMouse(evismouseButton);

	if (m_rgpMouseEvent[iMouse] != 0)
		refstringMessage = m_rgpMouseEvent[iMouse]->m_string;
}


void CVisPaneWnd::ShowEventHandlerMessage(void)
{
	CString stringLButtonDown;
	FindButtonMessage(evismouseLButtonDown, stringLButtonDown);

	CString stringRButtonDown;
	FindButtonMessage(evismouseRButtonDown, stringRButtonDown);

	CString stringMessage;
	if (!stringLButtonDown.IsEmpty())
	{
		if (!stringRButtonDown.IsEmpty())
		{
			stringMessage = "L: " + stringLButtonDown
					+ ",  R: " + stringRButtonDown;
		}
		else
		{
			stringMessage = "L: " + stringLButtonDown;
		}
	}
	else if (!stringRButtonDown.IsEmpty())
	{
		stringMessage = "R: " + stringRButtonDown;
	}

	if (!stringMessage.IsEmpty())
		SetMessageText(stringMessage);
}


// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk

void VISAPI CVisPaneWnd::SelectPaneCallback(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser, bool fOutside)
{

#else // VIS_USER_BUILD_mturk

void VISAPI CVisPaneWnd::SelectPaneCallback(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser)
{
	bool fOutside = false;

#endif // VIS_USER_BUILD_mturk

	CVisPaneArrayWnd *ppanearraywnd
			= (CVisPaneArrayWnd *) ppanewnd->PWndParent();

	if ((ppanewnd->IsPartOfArray())
			&& (ppanearraywnd->AllowMultiSelect()))
	{
		// Want special code for multi-selection.
		UINT nFlagsMasked = (nFlags & (MK_SHIFT | MK_CONTROL));

		if (nFlagsMasked == 0)
		{
			// When no keys are held down, we should clear any other
			// selected panes and toggle the selection in the current
			// pane.
			ppanearraywnd->ClearSelections();
			ppanewnd->SetIsSelected(!ppanewnd->IsSelected());
		}
		else if (nFlagsMasked == MK_SHIFT)
		{
			// When the shift key is held down, we should change the
			// selection of panes between this pane and the first
			// pane selected.
			// LATER:  The code for the shift case isn't exactly right
			// because we don't know which pane was selected first.
			if (ppanewnd->IsSelected())
			{
				int ipane = ppanewnd->IndexInParent() - 1;
				while ((ipane >= 0)
						&& (ppanearraywnd->Pane(ipane).IsSelected()))
				{
					-- ipane;
				}
				++ ipane;
				while (++ipane < ppanewnd->IndexInParent())
				{
					ppanearraywnd->Pane(ipane).SetIsSelected(false);
				}

				ppanewnd->SetIsSelected(false);

				int ipaneLim = ppanearraywnd->NPanes();
				ipane = ppanewnd->IndexInParent() + 1;
				while ((ipane < ipaneLim)
						&& (ppanearraywnd->Pane(ipane).IsSelected()))
				{
					++ ipane;
				}
				-- ipane;
				while (--ipane > ppanewnd->IndexInParent())
				{
					ppanearraywnd->Pane(ipane).SetIsSelected(false);
				}
			}
			else
			{
				int ipane = ppanewnd->IndexInParent() - 1;
				while ((ipane >= 0)
						&& (!ppanearraywnd->Pane(ipane).IsSelected()))
				{
					-- ipane;
				}
				if (ipane >= 0)
				{
					++ ipane;
					while (ipane < ppanewnd->IndexInParent())
					{
						ppanearraywnd->Pane(ipane).SetIsSelected(true);
						++ ipane;
					}
				}

				ppanewnd->SetIsSelected(true);

				int ipaneLim = ppanearraywnd->NPanes();
				ipane = ppanewnd->IndexInParent() + 1;
				while ((ipane < ipaneLim)
						&& (!ppanearraywnd->Pane(ipane).IsSelected()))
				{
					++ ipane;
				}
				if (ipane < ipaneLim)
				{
					-- ipane;
					while (ipane > ppanewnd->IndexInParent())
					{
						ppanearraywnd->Pane(ipane).SetIsSelected(true);
						-- ipane;
					}
				}
			}
		}
		else
		{
			// When the control key is held down, we should toggle the
			// selection for this pane without changing the selection
			// in other panes.
			ppanewnd->SetIsSelected(!ppanewnd->IsSelected());
		}
	}
	else
	{
		// Just call the SetIsSelected method.
		ppanewnd->SetIsSelected(!ppanewnd->IsSelected());
	}
}


// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk

void VISAPI CVisPaneWnd::ShowPixelValuesCallback(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser, bool fOutside)
{

#else // VIS_USER_BUILD_mturk

void VISAPI CVisPaneWnd::ShowPixelValuesCallback(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser)
{
	bool fOutside = false;

#endif // VIS_USER_BUILD_mturk

	int evispaneOptions = ppanewnd->EvispaneOptions();
	const CVisImageBase *pimageOriginal = &(ppanewnd->Image());
	const CVisImageBase *pimageScreen = &(ppanewnd->ScreenImage());
	
	// Don't show pixel values for stretched images.
	// LATER:  Add code to support this?
	if (((evispaneOptions & evispaneStretchToFit) != 0)
		|| ((evispaneOptions & evispaneStretchPreserveAspect) != 0))
	{
		return;
	}
	
	CString stringStatusMessage;
	if ((evispaneOptions & evispaneNameInStatus) != 0)
	{
		stringStatusMessage = ppanewnd->Title();
		if (!stringStatusMessage.IsEmpty())
			stringStatusMessage += ":  ";
	}
	
	if (fOutside)
	{
		stringStatusMessage += "";
	}
	else
	{
		if ((pimageOriginal == 0) && (pimageScreen == 0))
		{
			// LATER:  Should this clear the status text?
			
			if ((evispaneOptions & evispaneNoImageErrorStatus) != 0)
				stringStatusMessage += "No image.";
			//		else
			//			stringStatusMessage += "";
		}
		else if ((pimageOriginal == 0) && (pimageScreen != 0))
		{
			// LATER:  Should this clear the status text?
			
			if ((evispaneOptions & evispaneNoImageErrorStatus) != 0)
				stringStatusMessage += "No original image.";
			//		else
			//			stringStatusMessage += "";
		}
		else if ((pimageOriginal != 0) && (!pimageOriginal->IsValid()))
		{
			if ((evispaneOptions & evispaneBadImageErrorInWindow) != 0)
				stringStatusMessage += "Invalid image.";
			//		else
			//			stringStatusMessage += "";
		}
		else if (ppanewnd->CouldNotDisplayImage())
		{
			if ((evispaneOptions & evispaneBadImageErrorInWindow) != 0)
				stringStatusMessage += "Image could not be displayed.";
			//		else
			//			stringStatusMessage += "";
		}
		else
		{
			CVisPoint pointImage = point;
			if (pimageOriginal->ContainsPoint(pointImage))
			{
				char szT[256];
				
				if (pimageOriginal != 0)
					pimageOriginal->GetPixelValueString(pointImage, szT, sizeof(szT));
				else
					pimageScreen->GetPixelValueString(pointImage, szT, sizeof(szT));
				
				CString stringT;
				stringT.Format("(%3d, %3d) -> [%s]",
					pointImage.x, pointImage.y, szT);
				
				stringStatusMessage += stringT;
			}
			else
			{
				stringStatusMessage += "Outside of image";
			}
		}
	}
	ppanewnd->SetMessageText(stringStatusMessage);
}
