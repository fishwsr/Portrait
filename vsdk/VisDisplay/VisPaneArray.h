////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPANEARRAY
//
// @module VisPaneArray.h |
//
//  This file declares classes used to display an array of CVisPane
// (actually CVisPaneWnd) objects.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPaneArray\.cpp>
//
// <nl>
// @index | VISPANEARRAY
//
////////////////////////////////////////////////////////////////////////////



#ifndef __VIS_DISPLAY_PANEARRAY_H__
#define __VIS_DISPLAY_PANEARRAY_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// LATER:  Split CVisPaneArrayWnd into CVisPaneArrayWndBase and
// CVisPaneArrayWnd.  The base class should have virtual methods
// for finding pane positions.  We should be able to add a new
// CVisPanePyramid class (derived from CVisPaneArrayWndBase) to
// display a pyramid of an image (with the pane windows layed-out
// correctly).  The CVisPanePyramid class ctors should take a
// scale (ratio of sizes between levels) and number of levels.


// Forward declaration
class CVisPaneArrayWnd;


////////////////////////////////////////////////////////////////////////////
//  
// @class ***
//
////////////////////////////////////////////////////////////////////////////
class VisDisplayExport CVisPaneArray
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Constructors and destructor

    // @cmember
	// Default constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisPaneArray(int cPaneCol, int cPaneRow, CWnd *pwndParent = 0,
			bool fCreateWnd = false, bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCol, int cPaneRow, int evispane,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows,
			int dxPaneInside, int dyPaneInside,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows, SIZE sizePaneInside,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows,
			POINT pointPaneArray, SIZE sizePaneInside,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows, RECT rectPaneArray,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows,
			const CVisSequenceBase& refsequence,
			int dxPaneInside, int dyPaneInside,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows, SIZE sizePaneInside,
			const CVisSequenceBase& refsequence,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows,
			const CVisSequenceBase& refsequence,
			POINT pointPaneArray, SIZE sizePaneInside,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Constructor.
	CVisPaneArray(int cPaneCols, int cPaneRows,
			const CVisSequenceBase& refsequence,
			RECT rectPaneArray,
			const char *szTitle = 0, int evispane = evispaneDefault,
			CWnd *pwndParent = 0, bool fCreateWnd = true,
			bool fDownThenAcross = false);

    // @cmember
	// Copy constructor.
	CVisPaneArray(const CVisPaneArray& refpane);

    // @cmember
	// Assignment operator.
	CVisPaneArray& operator=(const CVisPaneArray& refpane);

    // @cmember
	// Destructor.
	virtual ~CVisPaneArray(void);


// Attributes
public:
    //------------------------------------------------------------------
	// @group Attributes

	int NPanes(void) const;
	int NPaneCols(void) const;
	int NPaneRows(void) const;

	bool UseRowMajorPaneOrder(void) const;
	int ColFromPaneIndex(int ipane) const;
	int RowFromPaneIndex(int ipane) const;

	// Don't allow access to panes when no window is displayed to avoid
	// problems synchronizing the pane sizes.
	// Don't return a reference because that would allow the user to
	// replace the pane used be this object.
	CVisPane Pane(int i) const;
	CVisPane Pane(int iCol, int iRow) const;
	CVisPane operator[](int i) const;

	int XOffsetInParent(void) const;
	void SetXOffsetInParent(int x, bool fRepaint = true);

	int YOffsetInParent(void) const;
	void SetYOffsetInParent(int y, bool fRepaint = true);

	CPoint PointOffsetInParent(void) const;
	void SetPointOffsetInParent(POINT point, bool fRepaint = true);

	int WidthPaneInside(void) const;
	void SetWidthPaneInside(int dx, bool fRepaint = true);

	int WidthOutside(void) const;
	void SetWidthOutside(int dx, bool fRepaint = true);

	int WidthBorder(void) const;
	void SetWidthBorder(int dx, bool fRepaint = true);

	int WidthPaneBorder(void) const;
	void SetWidthPaneBorder(int dx, bool fRepaint = true);

	int WidthBetweenPanes(void) const;
	void SetWidthBetweenPanes(int dx, bool fRepaint = true);

	int HeightPaneInside(void) const;
	void SetHeightPaneInside(int dy, bool fRepaint = true);

	int HeightOutside(void) const;
	void SetHeightOutside(int dy, bool fRepaint = true);

	CSize SizePaneInside(void) const;
	void SetSizePaneInside(int dx, int dy, bool fRepaint = true);
	void SetSizePaneInside(SIZE size, bool fRepaint = true);

	CSize SizePaneOutside(void) const;
	void SetSizePaneOutside(int dx, int dy, bool fRepaint = true);
	void SetSizePaneOutside(SIZE size, bool fRepaint = true);

	CSize SizeOutside(void) const;
	void SetSizeOutside(int dx, int dy, bool fRepaint = true);
	void SetSizeOutside(SIZE size, bool fRepaint = true);

	CRect RectOutside(void) const;
	void SetRectOutside(const RECT& refrect, bool fRepaint = true);

	bool AllowMultiSelect(void) const;
	void SetAllowMultiSelect(bool fAllowMultiSelect,
			bool fUpdateNow = false);

	void ClearSelections(bool fUpdateNow = false);

	void ClearSelections(int ipaneFirst, int ipaneLim = - 1,
			bool fUpdateNow = false);

	DWORD Style(void) const;
	void SetStyle(DWORD dw);

	DWORD ExStyle(void) const;
	void SetExStyle(DWORD dw);

	const char *Title(void);
	void SetTitle(const char *szTitle);

	bool HasWnd(void) const;
	const CWnd *PWnd(void) const;
	CWnd *PWnd(void);

	const CWnd *PWndParent(void) const;
	CWnd *PWndParent(void);

	COLORREF ColorrefBackground(void) const;
	void SetColorrefBackground(COLORREF colorref);

	bool HasRefreshed(void) const;


	int EvispaneOptions(void) const;


// Operations
public:
	//------------------------------------------------------------------
	// @group Window creation and destruction

	BOOL CreateWnd(int evispane = evispaneUnknown, CWnd *pwndParent = 0,
			bool fChangeStyles = true);

	BOOL ShowWnd(int nCmdShow = SW_SHOWNA);

	void DestroyWnd(void);


    //------------------------------------------------------------------
	// @group Updating the window

    // @cmember ***
	// If fUpdateNow is false, a WM_PAINT message will be posted to the
	// window's message queue and this method will return before the
	// window has been redrawn.  If fUpdateNow is true, this method will
	// not return until the window has been redrawn.
    void Refresh(bool fUpdateNow = false);


    //------------------------------------------------------------------
	// @group Displaying the array of panes

    // @cmember ***
    void Clear(bool fUpdateNow = false);

    // @cmember ***
    void Clear(COLORREF colorref, bool fUpdateNow = false);

    // @cmember ***
    void Clear(int ipaneFirst, int ipaneLim, bool fUpdateNow = false);

    // @cmember ***
    void Clear(int ipaneFirst, int ipaneLim, COLORREF colorref,
			bool fUpdateNow = false);


	void Display(const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize,
			const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize,
			const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);


    void RemoveAllPointsAndLines(bool fRefresh = true);

    void RemoveAllPointsAndLines(int ipaneFirst, int ipaneLim = - 1,
			bool fRefresh = true);


protected:
	CVisPaneArrayWnd *m_pPaneArrayWnd;
};
    



////////////////////////////////////////////////////////////////////////////
//  
// @class ***
// @base public | CVisPaneWndBase
//
////////////////////////////////////////////////////////////////////////////
// LATER:  Only export methods that need to be exported.
class VisDisplayExport CVisPaneArrayWnd : public CVisPaneWndBase
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Constructors and destructor

    // @cmember
	// Constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisPaneArrayWnd(int cPaneCols, int cPaneRows,
			int evispane = evispaneDefault, CWnd *pwndParent = 0,
			bool fDownThenAcross = false);

    // @cmember
	// Destructor.
	virtual ~CVisPaneArrayWnd(void);


// Attributes
public:
    //------------------------------------------------------------------
	// @group Attributes

	int NPanes(void) const;
	int NPaneCols(void) const;
	int NPaneRows(void) const;

	bool UseRowMajorPaneOrder(void) const;
	int ColFromPaneIndex(int ipane) const;
	int RowFromPaneIndex(int ipane) const;

	// Don't allow access to panes when no window is displayed to avoid
	// problems synchronizing the pane sizes.
	// Don't return a reference because that would allow the user to
	// replace the pane used be this object.
	CVisPane Pane(int i) const;
	CVisPane Pane(int iCol, int iRow) const;
	CVisPane operator[](int i) const;

	int WidthPaneInside(void) const;
	void SetWidthPaneInside(int dx, bool fRepaint = true);

	// These are helper methods that aren't exposed in the CVisPaneArray
	// wrapper class.
	int WidthPaneOutside(void) const;
	void SetWidthPaneOutside(int dx, bool fRepaint = true);

	int WidthBetweenPanes(void) const;
	void SetWidthBetweenPanes(int dx, bool fRepaint = true);

	int WidthPaneBorder(void) const;
	void SetWidthPaneBorder(int dx, bool fRepaint = true);

	int HeightPaneInside(void) const;
	void SetHeightPaneInside(int dy, bool fRepaint = true);

	// This is a helper method that isn't exposed in the CVisPaneArray
	// wrapper class.
	int HeightPaneOutside(void) const;
	void SetHeightPaneOutside(int dy, bool fRepaint = true);

	CSize SizePaneInside(void) const;
	void SetSizePaneInside(int dx, int dy, bool fRepaint = true);
	void SetSizePaneInside(SIZE size, bool fRepaint = true);

	CSize SizePaneOutside(void) const;
	void SetSizePaneOutside(int dx, int dy, bool fRepaint = true);
	void SetSizePaneOutside(SIZE size, bool fRepaint = true);

	bool AllowMultiSelect(void) const;
	void SetAllowMultiSelect(bool AllowMultiSelect,
			bool fUpdateNow = false);

	void ClearSelections(bool fUpdateNow = false);

	void ClearSelections(int ipaneFirst, int ipaneLim = - 1,
			bool fUpdateNow = false);


// Operations
public:
    //------------------------------------------------------------------
	// @group Displaying the array of panes

    // @cmember ***
    void Clear(bool fUpdateNow = false);

    // @cmember ***
    void Clear(COLORREF colorref, bool fUpdateNow = false);

    // @cmember ***
    void Clear(int ipaneFirst, int ipaneLim, bool fUpdateNow = false);

    // @cmember ***
    void Clear(int ipaneFirst, int ipaneLim, COLORREF colorref,
			bool fUpdateNow = false);


	void Display(const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize,
			const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisSequenceBase& refsequence,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize,
			const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisSequenceBase& refsequence,
			int iimageFirst, int iimageLim = - 1, int ipaneFirst = 0,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);


    void RemoveAllPointsAndLines(bool fRefresh = true);

    void RemoveAllPointsAndLines(int ipaneFirst, int ipaneLim = - 1,
			bool fRefresh = true);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisPaneWnd)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//}}AFX_VIRTUAL


protected:
	virtual BOOL RealCreateWnd(const char *szCursor,
			bool fNewThread = false);

	RECT RectPaneOutside(int ipane) const;
	RECT RectPaneOutside(int iColPane, int iRowPane) const;

	void RecalcLayout(void);

private:
	int m_cPaneCol;
	int m_cPaneRow;
	int m_dxBetweenPanes;
	int m_dxPaneBorder;
	CVisPane *m_rgpane;
	bool m_fDownThenAcross;
	bool m_fAllowMultiSelect;


    void ShowMouseText(UINT nFlags);

	void DrawBetweenSpace(CDC& dc);
	void DrawPanesAndBorders(CDC& dc);

	int EvispaneOptionsForPanes(void) const;

    // Generated message map functions
protected:
	//{{AFX_MSG(CVisPaneArrayWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#include "VisPaneArray.inl"


#endif //#ifndef __VIS_DISPLAY_PANEARRAY_H__
