// VisPaneArray.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// LATER:  Split VisPaneArray.* into VisPaneArray.* and VisPaneArrayWnd.*?



inline CVisPaneArray::~CVisPaneArray(void)
{
	m_pPaneArrayWnd->Release();
}


inline int CVisPaneArray::NPanes(void) const
{
	return m_pPaneArrayWnd->NPanes();
}


inline int CVisPaneArray::NPaneCols(void) const
{
	return m_pPaneArrayWnd->NPaneCols();
}


inline int CVisPaneArray::NPaneRows(void) const
{
	return m_pPaneArrayWnd->NPaneRows();
}


inline bool CVisPaneArray::UseRowMajorPaneOrder(void) const
{
	return m_pPaneArrayWnd->UseRowMajorPaneOrder();
}


inline int CVisPaneArray::ColFromPaneIndex(int ipane) const
{
	return m_pPaneArrayWnd->ColFromPaneIndex(ipane);
}


inline int CVisPaneArray::RowFromPaneIndex(int ipane) const
{
	return m_pPaneArrayWnd->RowFromPaneIndex(ipane);
}


inline CVisPane CVisPaneArray::Pane(int i) const
{
	return m_pPaneArrayWnd->Pane(i);
}


inline CVisPane CVisPaneArray::operator[](int i) const
{
	return m_pPaneArrayWnd->operator[](i);
}


inline CVisPane CVisPaneArray::Pane(int iCol, int iRow) const
{
	return m_pPaneArrayWnd->Pane(iCol, iRow);
}


inline int CVisPaneArray::XOffsetInParent(void) const
{
	return m_pPaneArrayWnd->XOffsetInParent();
}


inline void CVisPaneArray::SetXOffsetInParent(int x, bool fRepaint)
{
	m_pPaneArrayWnd->SetXOffsetInParent(x, fRepaint);
}


inline int CVisPaneArray::YOffsetInParent(void) const
{
	return m_pPaneArrayWnd->YOffsetInParent();
}


inline void CVisPaneArray::SetYOffsetInParent(int y, bool fRepaint)
{
	m_pPaneArrayWnd->SetYOffsetInParent(y, fRepaint);
}


inline CPoint CVisPaneArray::PointOffsetInParent(void) const
{
	return m_pPaneArrayWnd->PointOffsetInParent();
}


inline void CVisPaneArray::SetPointOffsetInParent(POINT point, bool fRepaint)
{
	m_pPaneArrayWnd->SetPointOffsetInParent(point, fRepaint);
}


inline int CVisPaneArray::WidthPaneInside(void) const
{
	return m_pPaneArrayWnd->WidthPaneInside();
}


inline void CVisPaneArray::SetWidthPaneInside(int dx, bool fRepaint)
{
	m_pPaneArrayWnd->SetWidthPaneInside(dx, fRepaint);
}


inline int CVisPaneArray::WidthBetweenPanes(void) const
{
	return m_pPaneArrayWnd->WidthBetweenPanes();
}


inline void CVisPaneArray::SetWidthBetweenPanes(int dx, bool fRepaint)
{
	m_pPaneArrayWnd->SetWidthBetweenPanes(dx, fRepaint);
}


inline int CVisPaneArray::WidthPaneBorder(void) const
{
	return m_pPaneArrayWnd->WidthPaneBorder();
}


inline void CVisPaneArray::SetWidthPaneBorder(int dx, bool fRepaint)
{
	m_pPaneArrayWnd->SetWidthPaneBorder(dx, fRepaint);
}


inline int CVisPaneArray::HeightPaneInside(void) const
{
	return m_pPaneArrayWnd->HeightPaneInside();
}


inline void CVisPaneArray::SetHeightPaneInside(int dy, bool fRepaint)
{
	m_pPaneArrayWnd->SetHeightPaneInside(dy, fRepaint);
}


inline CSize CVisPaneArray::SizePaneInside(void) const
{
	return m_pPaneArrayWnd->SizePaneInside();
}


inline void CVisPaneArray::SetSizePaneInside(int dx, int dy, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizePaneInside(dx, dy, fRepaint);
}


inline void CVisPaneArray::SetSizePaneInside(SIZE size, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizePaneInside(size, fRepaint);
}


inline CSize CVisPaneArray::SizePaneOutside(void) const
{
	return m_pPaneArrayWnd->SizePaneOutside();
}


inline void CVisPaneArray::SetSizePaneOutside(int dx, int dy, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizePaneOutside(dx, dy, fRepaint);
}


inline void CVisPaneArray::SetSizePaneOutside(SIZE size, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizePaneOutside(size, fRepaint);
}


inline int CVisPaneArray::WidthOutside(void) const
{
	return m_pPaneArrayWnd->WidthOutside();
}


inline void CVisPaneArray::SetWidthOutside(int dx, bool fRepaint)
{
	m_pPaneArrayWnd->SetWidthOutside(dx, fRepaint);
}


inline int CVisPaneArray::WidthBorder(void) const
{
	return m_pPaneArrayWnd->WidthBorder();
}


inline void CVisPaneArray::SetWidthBorder(int dx, bool fRepaint)
{
	m_pPaneArrayWnd->SetWidthBorder(dx, fRepaint);
}


inline int CVisPaneArray::HeightOutside(void) const
{
	return m_pPaneArrayWnd->HeightOutside();
}


inline void CVisPaneArray::SetHeightOutside(int dy, bool fRepaint)
{
	m_pPaneArrayWnd->SetHeightOutside(dy, fRepaint);
}


inline CSize CVisPaneArray::SizeOutside(void) const
{
	return m_pPaneArrayWnd->SizeOutside();
}


inline void CVisPaneArray::SetSizeOutside(int dx, int dy, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizeOutside(dx, dy, fRepaint);
}


inline void CVisPaneArray::SetSizeOutside(SIZE size, bool fRepaint)
{
	m_pPaneArrayWnd->SetSizeOutside(size, fRepaint);
}


inline CRect CVisPaneArray::RectOutside(void) const
{
	return m_pPaneArrayWnd->RectOutside();
}


inline void CVisPaneArray::SetRectOutside(const RECT& refrect, bool fRepaint)
{
	m_pPaneArrayWnd->SetRectOutside(refrect, fRepaint);
}


inline bool CVisPaneArray::AllowMultiSelect(void) const
{
	return m_pPaneArrayWnd->AllowMultiSelect();
}


inline void CVisPaneArray::ClearSelections(bool fUpdateNow)
{
	m_pPaneArrayWnd->ClearSelections(fUpdateNow);
}


inline void CVisPaneArray::SetAllowMultiSelect(bool fAllowMultiSelect, bool fUpdateNow)
{
	m_pPaneArrayWnd->SetAllowMultiSelect(fAllowMultiSelect, fUpdateNow);
}


inline DWORD CVisPaneArray::Style(void) const
{
	return m_pPaneArrayWnd->Style();
}


inline void CVisPaneArray::SetStyle(DWORD dw)
{
	m_pPaneArrayWnd->SetStyle(dw);
}


inline DWORD CVisPaneArray::ExStyle(void) const
{
	return m_pPaneArrayWnd->ExStyle();
}


inline void CVisPaneArray::SetExStyle(DWORD dw)
{
	m_pPaneArrayWnd->SetExStyle(dw);
}


inline const char *CVisPaneArray::Title(void)
{
	return m_pPaneArrayWnd->Title();
}


inline void CVisPaneArray::SetTitle(const char *szTitle)
{
	m_pPaneArrayWnd->SetTitle(szTitle);
}


inline bool CVisPaneArray::HasWnd(void) const
{
	return m_pPaneArrayWnd->HasWnd();
}


inline const CWnd *CVisPaneArray::PWnd(void) const
{
	return ((m_pPaneArrayWnd->HasWnd()) ? m_pPaneArrayWnd : 0);
}


inline CWnd *CVisPaneArray::PWnd(void)
{
	return ((m_pPaneArrayWnd->HasWnd()) ? m_pPaneArrayWnd : 0);
}


inline const CWnd *CVisPaneArray::PWndParent(void) const
{
	return m_pPaneArrayWnd->PWndParent();
}


inline CWnd *CVisPaneArray::PWndParent(void)
{
	return m_pPaneArrayWnd->PWndParent();
}


inline COLORREF CVisPaneArray::ColorrefBackground(void) const
{
	return m_pPaneArrayWnd->ColorrefBackground();
}


inline void CVisPaneArray::SetColorrefBackground(COLORREF colorref)
{
	m_pPaneArrayWnd->SetColorrefBackground(colorref);
}


inline bool CVisPaneArray::HasRefreshed(void) const
{
	return m_pPaneArrayWnd->HasRefreshed();
}


inline int CVisPaneArray::EvispaneOptions(void) const
{
	return m_pPaneArrayWnd->EvispaneOptions();
}


inline BOOL CVisPaneArray::CreateWnd(int evispane, CWnd *pwndParent,
		bool fChangeStyles)
{
	return m_pPaneArrayWnd->CreateWnd(evispane, pwndParent, fChangeStyles);
}


inline BOOL CVisPaneArray::ShowWnd(int nCmdShow)
{
	return m_pPaneArrayWnd->ShowWnd(nCmdShow);
}


inline void CVisPaneArray::DestroyWnd(void)
{
	m_pPaneArrayWnd->DestroyWnd();
}


inline void CVisPaneArray::Refresh(bool fUpdateNow)
{
	m_pPaneArrayWnd->Refresh(fUpdateNow);
}


inline void CVisPaneArray::Clear(bool fUpdateNow)
{
	m_pPaneArrayWnd->Clear(fUpdateNow);
}


inline void CVisPaneArray::Clear(COLORREF colorref, bool fUpdateNow)
{
	m_pPaneArrayWnd->Clear(colorref, fUpdateNow);
}


inline void CVisPaneArray::Clear(int ipaneFirst, int ipaneLim, bool fUpdateNow)
{
	m_pPaneArrayWnd->Clear(ipaneFirst, ipaneLim, fUpdateNow);
}


inline void CVisPaneArray::Clear(int ipaneFirst, int ipaneLim, COLORREF colorref,
		bool fUpdateNow)
{
	m_pPaneArrayWnd->Clear(ipaneFirst, ipaneLim, colorref, fUpdateNow);
}


inline void CVisPaneArray::Display(const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(refsequence,
			evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::Display(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(evisnormalize, refsequence,
			evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(evisnormalize, dblMin, dblMax, refsequence,
			evispanedisp, fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::Display(const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(refsequence,
			iimageFirst, iimageLim, ipaneFirst, evispanedisp,
			fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::Display(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(evisnormalize, refsequence,
			iimageFirst, iimageLim, ipaneFirst, evispanedisp,
			fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::Display(EVisNormalize evisnormalize,
		double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	m_pPaneArrayWnd->Display(evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim, ipaneFirst, evispanedisp,
			fUpdateNow, fCreateWndIfNeeded);
}


inline void CVisPaneArray::RemoveAllPointsAndLines(bool fRefresh)
{
	m_pPaneArrayWnd->RemoveAllPointsAndLines(fRefresh);
}


inline void CVisPaneArray::RemoveAllPointsAndLines(int ipaneFirst, int ipaneLim,
		bool fRefresh)
{
	m_pPaneArrayWnd->RemoveAllPointsAndLines(ipaneFirst, ipaneLim, fRefresh);
}




inline int CVisPaneArrayWnd::NPanes(void) const
{
	return m_cPaneCol * m_cPaneRow;
}


inline int CVisPaneArrayWnd::NPaneCols(void) const
{
	return m_cPaneCol;
}


inline int CVisPaneArrayWnd::NPaneRows(void) const
{
	return m_cPaneRow;
}


inline bool CVisPaneArrayWnd::UseRowMajorPaneOrder(void) const
{
	return m_fDownThenAcross;
}


inline int CVisPaneArrayWnd::ColFromPaneIndex(int ipane) const
{
	if (UseRowMajorPaneOrder())
		return (ipane / NPaneRows());

	return (ipane % NPaneCols());
}


inline int CVisPaneArrayWnd::RowFromPaneIndex(int ipane) const
{
	if (UseRowMajorPaneOrder())
		return (ipane % NPaneRows());

	return (ipane / NPaneCols());
}


inline CVisPane CVisPaneArrayWnd::Pane(int i) const
{
	assert(HasWnd());
	assert((i >= 0) && (i < m_cPaneCol * m_cPaneRow));

	return m_rgpane[i];
}


inline CVisPane CVisPaneArrayWnd::operator[](int i) const
{
	return Pane(i);
}


inline CVisPane CVisPaneArrayWnd::Pane(int iCol, int iRow) const
{
	assert(HasWnd());
	assert((iCol >= 0) && (iCol < m_cPaneCol));
	assert((iRow >= 0) && (iRow < m_cPaneRow));

	return m_rgpane[iRow * m_cPaneCol + iCol];
}


inline int CVisPaneArrayWnd::WidthPaneInside(void) const
{
	return SizePaneInside().cx;
}


inline void CVisPaneArrayWnd::SetWidthPaneInside(int dx, bool fRepaint)
{
	SetWidthPaneOutside(dx + m_rgpane[0].SizeInOutDifference().cx, fRepaint);
}


inline int CVisPaneArrayWnd::WidthPaneOutside(void) const
{
	return (WidthInside() - (NPaneCols() - 1) * WidthBetweenPanes())
			/ NPaneCols();
}


inline void CVisPaneArrayWnd::SetWidthPaneOutside(int dx, bool fRepaint)
{
	SetSizePaneOutside(dx, HeightPaneOutside(), fRepaint);
}


inline int CVisPaneArrayWnd::WidthBetweenPanes(void) const
{
	return m_dxBetweenPanes;
}


inline void CVisPaneArrayWnd::SetWidthBetweenPanes(int dx, bool fRepaint)
{
	CSize sizePaneOutsideOld = SizePaneOutside();

	m_dxBetweenPanes = dx;

	CSize sizePaneOutsideNew = SizePaneOutside();

	if (sizePaneOutsideOld != sizePaneOutsideNew)
		RecalcLayout();
}


inline int CVisPaneArrayWnd::WidthPaneBorder(void) const
{
	return m_dxPaneBorder;
}


inline void CVisPaneArrayWnd::SetWidthPaneBorder(int dx, bool fRepaint)
{
	m_dxPaneBorder = dx;
	for (int ipane = 0; ipane < NPanes(); ++ipane)
		m_rgpane[ipane].SetWidthBorder(m_dxPaneBorder);

	if (fRepaint)
		Refresh();
}


inline int CVisPaneArrayWnd::HeightPaneInside(void) const
{
	return SizePaneInside().cy;
}


inline void CVisPaneArrayWnd::SetHeightPaneInside(int dy, bool fRepaint)
{
	SetHeightPaneOutside(dy + m_rgpane[0].SizeInOutDifference().cy, fRepaint);
}


inline int CVisPaneArrayWnd::HeightPaneOutside(void) const
{
	return (HeightInside() - (NPaneRows() - 1) * WidthBetweenPanes())
			/ NPaneRows();
}


inline void CVisPaneArrayWnd::SetHeightPaneOutside(int dy, bool fRepaint)
{
	SetSizePaneOutside(WidthPaneOutside(), dy, fRepaint);
}


inline CSize CVisPaneArrayWnd::SizePaneInside(void) const
{
	return SizePaneOutside() - m_rgpane[0].SizeInOutDifference();
}


inline void CVisPaneArrayWnd::SetSizePaneInside(int dx, int dy, bool fRepaint)
{
	SetSizePaneInside(CSize(dx, dy), fRepaint);
}


inline void CVisPaneArrayWnd::SetSizePaneInside(SIZE size, bool fRepaint)
{
	SetSizePaneOutside(m_rgpane[0].SizeInOutDifference() + size, fRepaint);
}


inline CSize CVisPaneArrayWnd::SizePaneOutside(void) const
{
	return CSize(WidthPaneOutside(), HeightPaneOutside());
}


inline void CVisPaneArrayWnd::SetSizePaneOutside(int dx, int dy, bool fRepaint)
{
	SetSizeInside(dx + (dx + WidthBetweenPanes()) * (NPaneCols() - 1),
			dy + (dy + WidthBetweenPanes()) * (NPaneRows() - 1), fRepaint);
}


inline void CVisPaneArrayWnd::SetSizePaneOutside(SIZE size, bool fRepaint)
{
	SetSizePaneOutside(size.cx, size.cy, fRepaint);
}


inline bool CVisPaneArrayWnd::AllowMultiSelect(void) const
{
	return m_fAllowMultiSelect;
}


inline int CVisPaneArrayWnd::EvispaneOptionsForPanes(void) const
{
	return (evispaneStyleChangesSizeInside |
			(EvispaneOptions() & ~ (evispaneAutoDestroy
					| evispaneOwnThread
					| evispaneResizeToFit
					| evispaneSizeForArray
					| evispaneArrayTitle
					| evispaneArrayBorder
					| evispaneArrayStatusBar
					| evispaneCanResize
					| evispaneCanMaximize
					| evispaneTopMost)));
}


inline void CVisPaneArrayWnd::ClearSelections(bool fUpdateNow)
{
	ClearSelections(0, - 1, fUpdateNow);
}


inline void CVisPaneArrayWnd::ClearSelections(int ipaneFirst, int ipaneLim,
		bool fUpdateNow)
{
	if (ipaneFirst < 0)
		ipaneFirst = 0;

	if ((ipaneLim < 0) || (ipaneLim > NPanes()))
		ipaneLim = NPanes();

	for (int ipane = ipaneFirst; ipane < ipaneLim; ++ipane)
	{
		if (m_rgpane[ipane].IsSelected())
			m_rgpane[ipane].SetIsSelected(false, fUpdateNow);
	}
}


inline void CVisPaneArrayWnd::Clear(bool fUpdateNow)
{
	Clear(0, - 1, fUpdateNow);
}


inline void CVisPaneArrayWnd::Clear(COLORREF colorref, bool fUpdateNow)
{
	Clear(0, - 1, colorref, fUpdateNow);
}


inline void CVisPaneArrayWnd::Clear(int ipaneFirst, int ipaneLim, bool fUpdateNow)
{
	if (ipaneFirst < 0)
		ipaneFirst = 0;

	if ((ipaneLim < 0) || (ipaneLim > NPanes()))
		ipaneLim = NPanes();

	for (int ipane = ipaneFirst; ipane < ipaneLim; ++ipane)
		m_rgpane[ipane].Clear(fUpdateNow);
}


inline void CVisPaneArrayWnd::Clear(int ipaneFirst, int ipaneLim, COLORREF colorref,
		bool fUpdateNow)
{
	if (ipaneFirst < 0)
		ipaneFirst = 0;

	if ((ipaneLim < 0) || (ipaneLim > NPanes()))
		ipaneLim = NPanes();

	for (int ipane = ipaneFirst; ipane < ipaneLim; ++ipane)
		m_rgpane[ipane].Clear(colorref, fUpdateNow);
}


inline void CVisPaneArrayWnd::Display(const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(evisnormalizeDefault, 1.0, - 1.0, refsequence, 0, - 1, 0,
			evispanedisp, fUpdateNow, fUpdateNow);
}


inline void CVisPaneArrayWnd::Display(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(evisnormalize, 1.0, - 1.0, refsequence, 0, - 1, 0,
			evispanedisp, fUpdateNow, fUpdateNow);
}


inline void CVisPaneArrayWnd::Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(evisnormalize, dblMin, dblMax, refsequence, 0, - 1, 0,
			evispanedisp, fUpdateNow, fUpdateNow);
}


inline void CVisPaneArrayWnd::Display(const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(evisnormalizeDefault, 1.0, - 1.0, refsequence,
			iimageFirst, iimageLim, ipaneFirst,
			evispanedisp, fUpdateNow, fUpdateNow);
}


inline void CVisPaneArrayWnd::Display(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim, int ipaneFirst,
		EVisPaneDisp evispanedisp,
		bool fUpdateNow, bool fCreateWndIfNeeded)
{
	Display(evisnormalize, 1.0, - 1.0, refsequence,
			iimageFirst, iimageLim, ipaneFirst, evispanedisp,
			fUpdateNow, fUpdateNow);
}


inline void CVisPaneArrayWnd::RemoveAllPointsAndLines(bool fRefresh)
{
	RemoveAllPointsAndLines(0, - 1, fRefresh);
}


inline void CVisPaneArrayWnd::RemoveAllPointsAndLines(int ipaneFirst, int ipaneLim,
		bool fRefresh)
{
	if (ipaneFirst < 0)
		ipaneFirst = 0;

	if ((ipaneLim < 0) || (ipaneLim > NPanes()))
		ipaneLim = NPanes();

	for (int ipane = ipaneFirst; ipane < ipaneLim; ++ipane)
		m_rgpane[ipane].RemoveAllPointsAndLines(fRefresh);
}
