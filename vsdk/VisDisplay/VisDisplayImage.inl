// VisDisplayImage.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved



inline CVisPane VisDisplayImage(const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), refimage.Size(),
			evisnormalizeDefault, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), CSize(dxPaneInside, dyPaneInside),
			evisnormalizeDefault, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(pointOffsetInParent, sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(((CRect&) refrectInParent).TopLeft(),
			((CRect&) refrectInParent).Size(),
			evisnormalizeDefault, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(EVisNormalize evisnormalize,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), refimage.Size(),
			evisnormalize, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), CSize(dxPaneInside, dyPaneInside),
			evisnormalize, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), sizePaneInside,
			evisnormalize, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(pointOffsetInParent, sizePaneInside,
			evisnormalize, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(((CRect&) refrectInParent).TopLeft(),
			((CRect&) refrectInParent).Size(),
			evisnormalize, 1.0, 0.0, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), refimage.Size(),
			evisnormalize, dblMin, dblMax, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), CSize(dxPaneInside, dyPaneInside),
			evisnormalize, dblMin, dblMax, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(CVisPoint(0, 0), sizePaneInside,
			evisnormalize, dblMin, dblMax, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImage(((CRect&) refrectInParent).TopLeft(),
			((CRect&) refrectInParent).Size(),
			evisnormalize, dblMin, dblMax, refimage,
			evispanedisp, szTitle, evispane, pwndParent);
}





inline CVisPaneArray VisDisplayImages(const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(CVisPoint(0, 0), sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(pointOffsetInParent, sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(refrectInParent,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(CVisPoint(0, 0), sizePaneInside,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(pointOffsetInParent, sizePaneInside,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(refrectInParent,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(CVisPoint(0, 0), sizePaneInside,
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}





inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			CVisPoint(0, 0), sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			pointOffsetInParent, sizePaneInside,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows, refrectInParent,
			evisnormalizeDefault, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			CVisPoint(0, 0), sizePaneInside,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			pointOffsetInParent, sizePaneInside,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows, refrectInParent,
			evisnormalize, 1.0, 0.0, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows,
			CVisPoint(0, 0), sizePaneInside,
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}
