// VisPane.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// LATER:  Split VisPane.* into VisPane.* and VisPaneWnd.*?



inline CVisPane::CVisPane(CVisPaneWnd *ppanewnd)
  : m_ppanewnd(ppanewnd)
{
	assert(ppanewnd != 0);

	m_ppanewnd->AddRef();
}


inline CVisPane::~CVisPane(void)
{
	m_ppanewnd->Release();
}


inline int CVisPane::XOffsetInParent(void) const
{
	return m_ppanewnd->XOffsetInParent();
}


inline void CVisPane::SetXOffsetInParent(int x, bool fRepaint)
{
	m_ppanewnd->SetXOffsetInParent(x, fRepaint);
}


inline int CVisPane::YOffsetInParent(void) const
{
	return m_ppanewnd->YOffsetInParent();
}


inline void CVisPane::SetYOffsetInParent(int y, bool fRepaint)
{
	m_ppanewnd->SetYOffsetInParent(y, fRepaint);
}


inline CVisPoint CVisPane::PointOffsetInParent(void) const
{
	return m_ppanewnd->PointOffsetInParent();
}


inline void CVisPane::SetPointOffsetInParent(POINT point, bool fRepaint)
{
	m_ppanewnd->SetPointOffsetInParent(point, fRepaint);
}


inline int CVisPane::WidthInside(void) const
{
	return m_ppanewnd->WidthInside();
}


inline void CVisPane::SetWidthInside(int dx, bool fRepaint)
{
	m_ppanewnd->SetWidthInside(dx, fRepaint);
}


inline int CVisPane::WidthOutside(void) const
{
	return m_ppanewnd->WidthOutside();
}


inline void CVisPane::SetWidthOutside(int dx, bool fRepaint)
{
	m_ppanewnd->SetWidthOutside(dx, fRepaint);
}


inline int CVisPane::WidthBorder(void) const
{
	return m_ppanewnd->WidthBorder();
}


inline void CVisPane::SetWidthBorder(int dx, bool fRepaint)
{
	m_ppanewnd->SetWidthBorder(dx, fRepaint);
}


inline int CVisPane::HeightInside(void) const
{
	return m_ppanewnd->HeightInside();
}


inline void CVisPane::SetHeightInside(int dy, bool fRepaint)
{
	m_ppanewnd->SetHeightInside(dy, fRepaint);
}


inline int CVisPane::HeightOutside(void) const
{
	return m_ppanewnd->HeightOutside();
}


inline void CVisPane::SetHeightOutside(int dy, bool fRepaint)
{
	m_ppanewnd->SetHeightOutside(dy, fRepaint);
}


inline CSize CVisPane::SizeInside(void) const
{
	return m_ppanewnd->SizeInside();
}


inline void CVisPane::SetSizeInside(int dx, int dy, bool fRepaint)
{
	m_ppanewnd->SetSizeInside(dx, dy, fRepaint);
}


inline void CVisPane::SetSizeInside(SIZE size, bool fRepaint)
{
	m_ppanewnd->SetSizeInside(size, fRepaint);
}


inline CSize CVisPane::SizeOutside(void) const
{
	return m_ppanewnd->SizeOutside();
}


inline void CVisPane::SetSizeOutside(int dx, int dy, bool fRepaint)
{
	m_ppanewnd->SetSizeOutside(dx, dy, fRepaint);
}


inline void CVisPane::SetSizeOutside(SIZE size, bool fRepaint)
{
	m_ppanewnd->SetSizeOutside(size, fRepaint);
}



inline CSize CVisPane::SizeInOutDifference(void) const
{
	return m_ppanewnd->SizeInOutDifference();
}


inline CVisPoint CVisPane::ImageOffset(void) const
{
	return m_ppanewnd->ImageOffset();
}


inline void CVisPane::SetImageOffset(int dx, int dy, bool fRefresh)
{
	m_ppanewnd->SetImageOffset(dx, dy, fRefresh);
}


inline void CVisPane::SetImageOffset(POINT point, bool fRefresh)
{
	m_ppanewnd->SetImageOffset(point, fRefresh);
}


inline CVisRect CVisPane::RectOutside(void) const
{
	return m_ppanewnd->RectOutside();
}


inline void CVisPane::SetRectOutside(const RECT& refrect, bool fRepaint)
{
	m_ppanewnd->SetRectOutside(refrect, fRepaint);
}


inline DWORD CVisPane::Style(void) const
{
	return m_ppanewnd->Style();
}


inline void CVisPane::SetStyle(DWORD dw)
{
	m_ppanewnd->SetStyle(dw);
}


inline DWORD CVisPane::ExStyle(void) const
{
	return m_ppanewnd->ExStyle();
}


inline void CVisPane::SetExStyle(DWORD dw)
{
	m_ppanewnd->SetExStyle(dw);
}


inline const char *CVisPane::Title(void)
{
	return m_ppanewnd->Title();
}


inline void CVisPane::SetTitle(const char *szTitle)
{
	m_ppanewnd->SetTitle(szTitle);
}


inline bool CVisPane::HasWnd(void) const
{
	return m_ppanewnd->HasWnd();
}


inline const CWnd *CVisPane::PWnd(void) const
{
	return ((m_ppanewnd->HasWnd()) ? m_ppanewnd : 0);
}


inline CWnd *CVisPane::PWnd(void)
{
	return ((m_ppanewnd->HasWnd()) ? m_ppanewnd : 0);
}


inline const CWnd *CVisPane::PWndParent(void) const
{
	return m_ppanewnd->PWndParent();
}


inline CWnd *CVisPane::PWndParent(void)
{
	return m_ppanewnd->PWndParent();
}


inline bool CVisPane::HasValidImage(void) const
{
	return m_ppanewnd->HasValidImage();
}


inline bool CVisPane::CouldNotDisplayImage(void) const
{
	return m_ppanewnd->CouldNotDisplayImage();
}


inline const CVisImageBase& CVisPane::Image(void) const
{
	return m_ppanewnd->Image();
}


inline const CVisImageBase& CVisPane::ScreenImage(void) const
{
	return m_ppanewnd->ScreenImage();
}


inline bool CVisPane::IsSelected(void) const
{
	return m_ppanewnd->IsSelected();
}


inline void CVisPane::SetIsSelected(bool fIsSelected, bool fUpdateNow)
{
	m_ppanewnd->SetIsSelected(fIsSelected, fUpdateNow);
}


inline COLORREF CVisPane::ColorrefBackground(void) const
{
	return m_ppanewnd->ColorrefBackground();
}


inline void CVisPane::SetColorrefBackground(COLORREF colorref)
{
	m_ppanewnd->SetColorrefBackground(colorref);
}


inline bool CVisPane::HasRefreshed(void) const
{
	return m_ppanewnd->HasRefreshed();
}


inline int CVisPane::EvispaneOptions(void) const
{
	return m_ppanewnd->EvispaneOptions();
}


inline bool CVisPane::IsPartOfArray(void) const
{
	return m_ppanewnd->IsPartOfArray();
}


inline int CVisPane::IndexInParent(void) const
{
	return m_ppanewnd->IndexInParent();
}

inline void CVisPane::SetMessageText(const char *szMessage)
{
	m_ppanewnd->SetMessageText(szMessage);
}


inline BOOL CVisPane::CreateWnd(int evispane, CWnd *pwndParent,
		bool fChangeStyles)
{
	return m_ppanewnd->CreateWnd(evispane, pwndParent, fChangeStyles);
}


inline BOOL CVisPane::ShowWnd(int nCmdShow)
{
	return m_ppanewnd->ShowWnd(nCmdShow);
}


inline void CVisPane::DestroyWnd(void)
{
	m_ppanewnd->DestroyWnd();
}


inline void CVisPane::Refresh(bool fUpdateNow)
{
	m_ppanewnd->Refresh(fUpdateNow);
}


inline void CVisPane::Clear(bool fRemovePointsAndLines, bool fUpdateNow)
{
	m_ppanewnd->Clear(fRemovePointsAndLines, fUpdateNow);
}


inline void CVisPane::Clear(COLORREF colorref, bool fRemovePointsAndLines,
		bool fUpdateNow)
{
	m_ppanewnd->Clear(colorref, fRemovePointsAndLines, fUpdateNow);
}


inline void CVisPane::Display(const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(refimage, evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}

inline void CVisPane::Display(const CVisImageBase& refimage,
		POINT pointImageOffset, EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(refimage, pointImageOffset, evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPane::Display(EVisNormalize evisnormalize,
		const CVisImageBase& refimage, EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(evisnormalize, refimage, evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPane::Display(EVisNormalize evisnormalize,
		const CVisImageBase& refimage, POINT pointImageOffset,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(evisnormalize, refimage, pointImageOffset,
			evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}

inline void CVisPane::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(evisnormalize, dblMin, dblMax, refimage,
			evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}

inline void CVisPane::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		POINT pointImageOffset, EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_ppanewnd->Display(evisnormalize, dblMin, dblMax, refimage,
			pointImageOffset, evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPane::AddPoint(int x, int y, int nPenStyle, int dxThickness,
		COLORREF colorref, bool fRefresh)
{
	m_ppanewnd->AddPoint(x, y, nPenStyle, dxThickness, colorref, fRefresh);
}


inline void CVisPane::AddLine(int x1, int y1, int x2, int y2, int nPenStyle,
		int dxThickness, COLORREF colorref, bool fRefresh)
{
	m_ppanewnd->AddLine(x1, y1, x2, y2, nPenStyle, dxThickness, colorref, fRefresh);
}

// Added so we can draw directly on the pane without using the drawing queue
// We need to rethink the line and point (and ?) drawing...
inline void CVisPane::DrawLine(int x1, int y1, int x2, int y2, int nPenStyle,
		int dxThickness, COLORREF colorref)
{
	CWnd *wnd = PWnd();
	CClientDC dc(wnd);
    CPen pen(PS_SOLID, dxThickness, colorref);
	CVisRect rect;
	wnd->GetClientRect(&rect);
    CVisPoint point(x1 + rect.TopLeft().x, y1 + rect.TopLeft().y);
    CPen *oldpen = dc.SelectObject(&pen);
    dc.DPtoLP(&point);
    dc.MoveTo(point);
    dc.LineTo(point+CVisPoint(x2-x1, y2-y1));
    dc.SelectObject(oldpen);
}


inline void CVisPane::RemovePoint(int x, int y, bool fRefresh)
{
	m_ppanewnd->RemovePoint(x, y, fRefresh);
}


inline void CVisPane::RemoveLine(int x1, int y1, int x2, int y2, bool fRefresh)
{
	m_ppanewnd->RemoveLine(x1, y1, x2, y2, fRefresh);
}


inline void CVisPane::RemoveAllPointsAndLines(bool fRefresh)
{
	m_ppanewnd->RemoveAllPointsAndLines(fRefresh);
}


inline void CVisPane::AddMouseEventCallback(int evismouse,
		VisPfnMouseEventCallback pfnCallback, const char *sz,
		void *pvUser)
{
	m_ppanewnd->AddMouseEventCallback(evismouse, pfnCallback, sz, pvUser);
}


inline void CVisPane::RemoveMouseEventCallback(int evismouse,
		VisPfnMouseEventCallback pfnCallback, void *pvUser)
{
	m_ppanewnd->RemoveMouseEventCallback(evismouse, pfnCallback, pvUser);
}


inline void CVisPane::RemoveMouseEventCallback(
		VisPfnMouseEventCallback pfnCallback, void *pvUser)
{
	m_ppanewnd->RemoveMouseEventCallback(pfnCallback, pvUser);
}


inline void CVisPane::SetPWndParent(CWnd *pWndParent)
{
	m_ppanewnd->SetPWndParent(pWndParent);
}


inline void CVisPane::SetIsPartOfArray(bool fIsPartOfArray)
{
	m_ppanewnd->SetIsPartOfArray(fIsPartOfArray);
}


inline void CVisPane::SetIndexInParent(int m_ipaneInParent)
{
	m_ppanewnd->SetIndexInParent(m_ipaneInParent);
}


inline void CVisPane::SetEvispaneOptions(int evispane, bool fChangeStyles)
{
	m_ppanewnd->SetEvispaneOptions(evispane, fChangeStyles);
}



inline unsigned long CVisRefCntWnd::AddRef(void)
{
	return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);
}


inline unsigned long CVisRefCntWnd::Release(void)
{
	unsigned long cRef
			= (unsigned long) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}


inline bool CVisRefCntWnd::AutoDestroy(void) const
{
	return m_fAutoDestroy;
}


inline void CVisRefCntWnd::SetAutoDestroy(bool fAutoDestroy)
{
	m_fAutoDestroy = fAutoDestroy;
}


inline bool CVisRefCntWnd::HasRefreshed(void) const
{
	return m_fHasRefreshed;
}


inline void CVisRefCntWnd::SetHasRefreshed(bool fHasRefreshed)
{
	m_fHasRefreshed = fHasRefreshed;
}


inline bool CVisRefCntWnd::HasWnd(void) const
{
	return ((m_hWnd == 0) ? false : true);
}


inline BOOL CVisRefCntWnd::ShowWnd(int nCmdShow)
{
	assert(HasWnd());

	return ShowWindow(nCmdShow);
}


inline void CVisRefCntWnd::DestroyWnd(void)
{
	if (HasWnd())
		DestroyWindow();
}



inline int CVisPaneWndBase::XOffsetInParent(void) const
{
	return RectOutside().left;
}


inline int CVisPaneWndBase::YOffsetInParent(void) const
{
	return RectOutside().top;
}


inline CVisPoint CVisPaneWndBase::PointOffsetInParent(void) const
{
	return RectOutside().TopLeft();
}


inline int CVisPaneWndBase::WidthInside(void) const
{
	return SizeInside().cx;
}


inline int CVisPaneWndBase::WidthOutside(void) const
{
	return RectOutside().Width();
}


inline int CVisPaneWndBase::WidthBorder(void) const
{
	return m_dxBorder;
}


inline int CVisPaneWndBase::HeightInside(void) const
{
	return SizeInside().cy;
}


inline int CVisPaneWndBase::HeightOutside(void) const
{
	return RectOutside().Height();
}


inline CSize CVisPaneWndBase::SizeInside(void) const
{
	return SizeOutside() - SizeInOutDifference();
}


inline void CVisPaneWndBase::SetSizeInside(int dx, int dy, bool fRepaint)
{
	CSize sizeDifference = SizeInOutDifference();
	SetSizeOutside(dx + sizeDifference.cx, dy + sizeDifference.cy, fRepaint);
}


inline void CVisPaneWndBase::SetSizeInside(SIZE size, bool fRepaint)
{
	SetSizeInside(size.cx, size.cy, fRepaint);
}


inline CSize CVisPaneWndBase::SizeOutside(void) const
{
	return RectOutside().Size();
}


inline void CVisPaneWndBase::SetSizeOutside(SIZE size, bool fRepaint)
{
	SetSizeOutside(size.cx, size.cy, fRepaint);
}


inline CVisRect CVisPaneWndBase::RectOutside(void) const
{
	CVisRect rect;
	FindRectInParent(rect);

	return rect;
}


inline DWORD CVisPaneWndBase::Style(void) const
{
	if (HasWnd())
		return GetStyle();

	return m_dwStyle;
}


inline DWORD CVisPaneWndBase::ExStyle(void) const
{
	if (HasWnd())
		return GetExStyle();

	return m_dwExStyle;
}


inline COLORREF CVisPaneWndBase::ColorrefBackground(void) const
{
	return m_colorrefBackground;
}


inline void CVisPaneWndBase::SetColorrefBackground(COLORREF colorref)
{
	m_colorrefBackground = colorref;
	// LATER:  Should this change the brush used with this window?
}


inline void CVisPaneWndBase::FindRectInParent(void)
{
	if (HasWnd())
		FindRectInParent(m_rectInParent);
}


inline void CVisPaneWndBase::FindDwStyle(void)
{
	if (HasWnd())
		m_dwStyle = GetStyle();
}


inline void CVisPaneWndBase::FindDwExStyle(void)
{
	if (HasWnd())
		m_dwExStyle = GetExStyle();
}


inline void CVisPaneWndBase::FindTitle(void)
{
	if (HasWnd())
		Title();
}


inline const CWnd *CVisPaneWndBase::PWndParent(void) const
{
	return m_pWndParent;
}


inline CWnd *CVisPaneWndBase::PWndParent(void)
{
	return m_pWndParent;
}


inline void CVisPaneWndBase::SetPWndParent(CWnd *pWndParent)
{
	m_pWndParent = pWndParent;
}


inline bool CVisPaneWndBase::IsPartOfArray(void) const
{
	return m_fIsPartOfArray;
}


inline bool CVisPaneWndBase::IsPaneWnd(void) const
{
	return !m_fIsPaneArrayWnd;
}


inline bool CVisPaneWndBase::IsPaneArrayWnd(void) const
{
	return m_fIsPaneArrayWnd;
}


inline int CVisPaneWndBase::IndexInParent(void) const
{
	return m_ipaneInParent;
}


inline void CVisPaneWndBase::SetIsPartOfArray(bool fIsPartOfArray)
{
	m_fIsPartOfArray = fIsPartOfArray;
}


inline void CVisPaneWndBase::SetIndexInParent(int ipaneInParent)
{
	m_ipaneInParent = ipaneInParent;
}


inline bool CVisPaneWndBase::UseStatusBar(void) const
{
	return m_fUseStatusBar;
}


inline int CVisPaneWndBase::DyStatusBar(void)
{
	return s_dyStatusBar;
}

/*
inline const CStatusBar& CVisPaneWndBase::StatusBar(void) const
{
	return m_statusbar;
}


inline CStatusBar& CVisPaneWndBase::StatusBar(void)
{
	return m_statusbar;
}
*/

inline bool CVisPaneWndBase::HasPaneThread(void) const
{
	return (m_ppanethread != 0);
}


inline const CVisPaneThread& CVisPaneWndBase::PaneThread(void) const
{
	return *m_ppanethread;
}


inline CVisPaneThread& CVisPaneWndBase::PaneThread(void)
{
	return *m_ppanethread;
}


inline int CVisPaneWndBase::EvispaneOptions(void) const
{
	return m_evispane;
}


inline bool CVisPaneWndBase::StyleChangesSizeInside(void) const
{
	return ((m_fIsPartOfArray)
			|| ((m_evispane & evispaneStyleChangesSizeInside) != 0));
}


inline CVisRect CVisPaneWndBase::RectClientStatusBar(void) const
{
	return RectClientStatusBar(RectClient());
}


inline CVisRect CVisPaneWndBase::RectClientInside(void) const
{
	return RectClientInside(RectClient());
}


inline CVisPoint CVisPaneWndBase::PointTopLeftClientInside(void) const
{
	return CVisPoint(WidthBorder(), WidthBorder());
}


inline CVisRect CVisPaneWndBase::RectClientBorder(void) const
{
	return RectClientBorder(RectClient());
}



inline CVisPoint CVisPaneWnd::ImageOffset(void) const
{
	return m_pointImageOffset;
}


inline void CVisPaneWnd::SetImageOffset(POINT point, bool fRefresh)
{
	SetImageOffset(point.x, point.y, fRefresh);
}


inline bool CVisPaneWnd::HasValidImage(void) const
{
	return (((m_pimageOriginal != 0) && (m_pimageOriginal->IsValid()))
			? true : false);
}


inline bool CVisPaneWnd::CouldNotDisplayImage(void) const
{
	return m_fCouldNotDisplayImage;
}


inline const CVisImageBase& CVisPaneWnd::Image(void) const
{
	return *m_pimageOriginal;
}


inline const CVisImageBase& CVisPaneWnd::ScreenImage(void) const
{
	return *m_pimageScreen;
}


inline bool CVisPaneWnd::IsSelected(void) const
{
	return m_fIsSelected;
}


inline void CVisPaneWnd::Clear(COLORREF colorref, bool fRemovePointsAndLines,
		bool fUpdateNow)
{
	SetColorrefBackground(colorref);
	Clear(fRemovePointsAndLines, fUpdateNow);
}
	

inline void CVisPaneWnd::Display(const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(refimage, CVisPoint(0, 0), evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPaneWnd::Display(EVisNormalize evisnormalize,
		const CVisImageBase& refimage, EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_evisnormalize = evisnormalize;
	Display(refimage, CVisPoint(0, 0), evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPaneWnd::Display(EVisNormalize evisnormalize,
		const CVisImageBase& refimage, POINT pointImageOffset,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_evisnormalize = evisnormalize;
	Display(refimage, pointImageOffset, evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPaneWnd::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp, bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_evisnormalize = evisnormalize;
	m_dblMin = dblMin;
	m_dblMax = dblMax;
	Display(refimage, CVisPoint(0, 0), evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPaneWnd::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		POINT pointImageOffset, EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_evisnormalize = evisnormalize;
	m_dblMin = dblMin;
	m_dblMax = dblMax;
	Display(refimage, pointImageOffset, evispanedisp, fUpdateNow,
			fCreateWndIfNeeded);
}

inline void CVisPaneWnd::RemoveMouseEventCallback(
		VisPfnMouseEventCallback pfnCallback, void *pvUser)
{
	RemoveMouseEventCallback(0, pfnCallback, pvUser);
}
