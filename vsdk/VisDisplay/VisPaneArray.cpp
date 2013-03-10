////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPANE
//
// @module VisPaneArray.cpp |
//
// This file implements the <c CVisPaneArrayWnd> class.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPaneArray\.h>
//
// <nl>
// @index | VISPANE
//
////////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisDisplayPch.h"
#include "VisDisplayProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////
// Class static vars

BEGIN_MESSAGE_MAP(CVisPaneArrayWnd, CVisPaneWndBase)
	//{{AFX_MSG_MAP(CVisPaneArrayWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////


CVisPaneArray::CVisPaneArray(int iCol, int iRow, CWnd *pwndParent,
		bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(iCol, iRow, evispaneDefault,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError("CVisPaneArray::CVisPaneArray(void)",
				__FILE__, __LINE__);
	}

	if (fCreateWnd)
		CreateWnd();
}

CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (fCreateWnd)
		CreateWnd();
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows,
		int dxPaneInside, int dyPaneInside,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetSizePaneInside(dxPaneInside, dyPaneInside);

	if (fCreateWnd)
		CreateWnd();
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows, SIZE sizePaneInside,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetSizePaneInside(sizePaneInside);

	if (fCreateWnd)
		CreateWnd();
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows,
		POINT pointPaneArray, SIZE sizePaneInside,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);


// UNDONE:  In initial testing, it appeared that the size of the status bar
// was somehow added to sizePaneInside when the panes were created.  We should
// look into this more to make sure that the correct size is used for the panes.

	SetSizePaneInside(sizePaneInside);
	SetPointOffsetInParent(pointPaneArray);

	if (fCreateWnd)
		CreateWnd();
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows, RECT rectPaneArray,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetRectOutside(rectPaneArray);

	if (fCreateWnd)
		CreateWnd();
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows,
		const CVisSequenceBase& refsequence,
		int dxPaneInside, int dyPaneInside,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetSizePaneInside(dxPaneInside, dyPaneInside);

	// Put the images in the panes and display the array of panes.
	Display(refsequence, evispanedispDefault, false, fCreateWnd);
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows, SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetSizePaneInside(sizePaneInside);

	// Put the images in the panes and display the array of panes.
	Display(refsequence, evispanedispDefault, false, fCreateWnd);
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows,
		const CVisSequenceBase& refsequence,
		POINT pointPaneArray, SIZE sizePaneInside,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetSizePaneInside(sizePaneInside);
	SetPointOffsetInParent(pointPaneArray);

	// Put the images in the panes and display the array of panes.
	Display(refsequence, evispanedispDefault, false, fCreateWnd);
}


CVisPaneArray::CVisPaneArray(int cPaneCols, int cPaneRows,
		const CVisSequenceBase& refsequence,
		RECT rectPaneArray,
		const char *szTitle, int evispane,
		CWnd *pwndParent, bool fCreateWnd, bool fDownThenAcross)
  : m_pPaneArrayWnd(new CVisPaneArrayWnd(cPaneCols, cPaneRows, evispane,
		pwndParent, fDownThenAcross))
{
	if (m_pPaneArrayWnd == 0)
	{
		throw CVisMemoryError(
				"CVisPaneArray::CVisPaneArray(int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	if (szTitle != 0)
		SetTitle(szTitle);

	SetRectOutside(rectPaneArray);

	// Put the images in the panes and display the array of panes.
	Display(refsequence, evispanedispDefault, false, fCreateWnd);
}


CVisPaneArray::CVisPaneArray(const CVisPaneArray& refpanearray)
  : m_pPaneArrayWnd(0)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneArrayMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pPaneArrayWnd = refpanearray.m_pPaneArrayWnd;
	m_pPaneArrayWnd->AddRef();

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


CVisPaneArray& CVisPaneArray::operator=(const CVisPaneArray& refpanearray)
{
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisPaneArrayMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pPaneArrayWnd->Release();
	m_pPaneArrayWnd = refpanearray.m_pPaneArrayWnd;
	m_pPaneArrayWnd->AddRef();

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return *this;
}



////////////////////////////////////////////////////////////////////////////


CVisPaneArrayWnd::CVisPaneArrayWnd(int cPaneCols, int cPaneRows, int evispane,
		CWnd *pwndParent, bool fDownThenAcross)
  : CVisPaneWndBase(evispane, true, ((evispane & evispaneAutoDestroy) != 0),
			((evispane & evispaneArrayStatusBar) != 0)),
	m_cPaneCol(cPaneCols),
	m_cPaneRow(cPaneRows),
	m_dxBetweenPanes(6),
	m_dxPaneBorder(2),
	m_fDownThenAcross(fDownThenAcross),
	m_rgpane(new CVisPane[m_cPaneCol * m_cPaneRow]),
	m_fAllowMultiSelect(true)
{
	if (m_rgpane == 0)
	{
		throw CVisMemoryError("CVisPaneArrayWnd::CVisPaneArrayWnd(int iCol,"
				"int iRow, int evispane, CWnd *pwndParent)",
				__FILE__, __LINE__);
	}

	// Use a larger border for the pane array.
	SetWidthBorder(m_dxBetweenPanes);

	SetPWndParent(pwndParent);

	for (int ipane = 0; ipane < NPanes(); ++ipane)
	{
		m_rgpane[ipane].SetIsPartOfArray(true);
		m_rgpane[ipane].SetIndexInParent(ipane);
	}

	if (evispane != evispaneUnknown)
	{
		int evispaneChildren = EvispaneOptionsForPanes();
		for (int ipane = 0; ipane < NPanes(); ++ipane)
		{
			m_rgpane[ipane].SetEvispaneOptions(evispaneChildren, true);
			m_rgpane[ipane].SetWidthBorder(WidthPaneBorder());
			m_rgpane[ipane].SetPWndParent(this);
		}
	}
}


CVisPaneArrayWnd::~CVisPaneArrayWnd(void)
{
	// Destroy the window if it's still up.  (This will also be done
	// in the base class's dtor, but we do it here to make the cleanup
	// code below easier.)
	if (HasWnd())
		DestroyWnd();

	assert(m_rgpane != 0);
	delete[] m_rgpane;
}


void CVisPaneArrayWnd::SetAllowMultiSelect(bool fAllowMultiSelect,
		bool fUpdateNow)
{
	if ((m_fAllowMultiSelect) && (!fAllowMultiSelect))
	{
		bool fFoundSelection = false;
		for (int ipane = 0; ipane < NPanes(); ++ipane)
		{
			if (m_rgpane[ipane].IsSelected())
			{
				if (fFoundSelection)
					m_rgpane[ipane].SetIsSelected(false, fUpdateNow);

				fFoundSelection = true;
			}
		}
	}

	m_fAllowMultiSelect = fAllowMultiSelect;
}


BOOL CVisPaneArrayWnd::RealCreateWnd(const char *szCursor, bool fNewThread)
{
	BOOL fRet = CVisPaneWndBase::RealCreateWnd(szCursor, fNewThread);

	if ((fRet) && (!fNewThread))
	{
		// We're in the thread that created the window.  We also need to
		// create the pane windows.
		int evispaneChildren = EvispaneOptionsForPanes();
		for (int ipane = 0; ipane < NPanes(); ++ipane)
		{
//			m_rgpane[ipane].SetRectOutside(RectPaneOutside(ipane));
			m_rgpane[ipane].CreateWnd(evispaneChildren, this);
			m_rgpane[ipane].PWnd()->MoveWindow(&RectPaneOutside(ipane));
		}
	}

	return fRet;
}


RECT CVisPaneArrayWnd::RectPaneOutside(int ipane) const
{
	return RectPaneOutside(ColFromPaneIndex(ipane), RowFromPaneIndex(ipane));
}


RECT CVisPaneArrayWnd::RectPaneOutside(int iColPane, int iRowPane) const
{
	int dxPane = WidthPaneOutside();
	int dyPane = HeightPaneOutside();
	int dx = dxPane + WidthBetweenPanes();
	int dy = dyPane + WidthBetweenPanes();

	RECT rect;
	rect.left = WidthBorder() + iColPane * (dxPane + WidthBetweenPanes());
	rect.right = rect.left + dxPane;
	rect.top = WidthBorder() + iRowPane * (dyPane + WidthBetweenPanes());
	rect.bottom = rect.top + dyPane;

	return rect;
}



void CVisPaneArrayWnd::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	if ((!HasWnd()) && (fCreateWndIfNeeded))
		CreateWnd();

	if ((iimageLim < 0) || (iimageLim > refsequence.Length()))
		iimageLim = refsequence.Length();

	if (iimageFirst < 0)
		iimageFirst = 0;

	int ipaneLim = min(NPanes(), ipaneFirst + iimageLim - iimageFirst);

	int iimage = iimageFirst;
	for (int ipane = ipaneFirst; ipane < ipaneLim; ++ipane, ++iimage)
	{
		m_rgpane[ipane].Display(evisnormalize, dblMin, dblMax,
				refsequence[iimage], evispanedisp, fUpdateNow,
				fCreateWndIfNeeded);
	}
}



void CVisPaneArrayWnd::OnDraw(CDC* pDC)
{
	DrawPanesAndBorders(*pDC);
}


void CVisPaneArrayWnd::DrawPanesAndBorders(CDC& dc)
{
	if (UseStatusBar())
		StatusBar().RedrawWindow();
	DrawBetweenSpace(dc);
	DrawBorders(dc, true);

	SetHasRefreshed(true);
}


void CVisPaneArrayWnd::DrawBetweenSpace(CDC& dc)
{
	CRect rectInside = RectClientInside();

#ifdef VIS_FILL_PANE_ARRAY_BG

	// It might look better if we redraw the entire background when resizing
	// the array.
	dc.FillSolidRect(rectInside, ColorrefBackground());

#else // VIS_FILL_PANE_ARRAY_BG

	// It might be faster to only draw the background between the panes.
	if (WidthBetweenPanes() > 0)
	{
		CRect rectBar;

		// Draw vertical bars between panes.
		int dx = WidthPaneOutside() + WidthBetweenPanes();
		rectBar.left = rectInside.left - WidthBetweenPanes();
		rectBar.right = rectInside.left;
		rectBar.top = rectInside.top;
		rectBar.bottom = rectInside.bottom;

		for (int iCol = 0; iCol < m_cPaneCol; ++iCol)
		{
			rectBar.left += dx;
			rectBar.right += dx;

			if (iCol == m_cPaneCol - 1)
			{
				rectBar.right = rectInside.right;
				if (rectBar.left == rectBar.right)
					break;
			}

			dc.FillSolidRect(rectBar, ColorrefBackground());
		}

		// Draw horizontal bars between panes.
		int dy = HeightPaneOutside() + WidthBetweenPanes();
		rectBar.left = rectInside.left;
		rectBar.right = rectInside.right;
		rectBar.top = rectInside.top - WidthBetweenPanes();
		rectBar.bottom = rectInside.top;

		for (int iRow = 0; iRow < m_cPaneRow; ++iRow)
		{
			rectBar.top += dy;
			rectBar.bottom += dy;

			if (iRow == m_cPaneRow - 1)
			{
				rectBar.bottom = rectInside.bottom;
				if (rectBar.top == rectBar.bottom)
					break;
			}

			dc.FillSolidRect(rectBar, ColorrefBackground());
		}
	}

#endif // VIS_FILL_PANE_ARRAY_BG
}



// @mfunc Redraws the window, pane border, and if valid the image otherwise the 
//        background color is used.
void CVisPaneArrayWnd::OnPaint()
{
//	assert(IsValid());

	CRect rect;
	if ((GetUpdateRect(&rect))
			&& (rect.Width() > 0)
			&& (rect.Height() > 0))
	{
		PAINTSTRUCT paintstruct;
		CDC *pdc = BeginPaint(&paintstruct);
		DrawPanesAndBorders(*pdc);
		EndPaint(&paintstruct);
	}
}


void CVisPaneArrayWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetMessageText();
}


void CVisPaneArrayWnd::OnSize(UINT nType, int cx, int cy) 
{
	CVisPaneWndBase::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CVisPaneArrayWnd::OnMove(int x, int y) 
{
	CVisPaneWndBase::OnMove(x, y);
	
	RecalcLayout();
}


void CVisPaneArrayWnd::RecalcLayout(void)
{
	for (int ipane = 0; ipane < NPanes(); ++ipane)
	{
		if (m_rgpane[ipane].HasWnd())
			m_rgpane[ipane].PWnd()->MoveWindow(&RectPaneOutside(ipane));
	}
	
	if ((UseStatusBar()) && (StatusBar().m_hWnd != 0))
	{
		CRect rectStatusBar = RectClientStatusBar();
		StatusBar().MoveWindow(&rectStatusBar);
	}
}
