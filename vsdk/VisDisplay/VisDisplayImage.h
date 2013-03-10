// VisDisplayImage.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


inline CVisPane VisDisplayImage(const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


inline CVisPane VisDisplayImage(EVisNormalize evisnormalize,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


inline CVisPane VisDisplayImage(EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(int dxPaneInside, int dyPaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

VisDisplayExport CVisPane VISAPI VisDisplayImage(POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPane VisDisplayImage(const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);




inline CVisPaneArray VisDisplayImages(const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


inline CVisPaneArray VisDisplayImages(EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(POINT pointOffsetInParent,
		SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

VisDisplayExport CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);




inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		EVisNormalize evisnormalize,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		EVisNormalize evisnormalize, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);


VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		int nPaneCols, int nPaneRows, EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

inline CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);

VisDisplayExport CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst = 0, int iimageLim = - 1,
		EVisPaneDisp evispanedisp = evispanedispDefault,
		const char *szTitle = 0, int evispane = evispaneDefault,
		CWnd *pwndParent = 0);



#include "VisDisplayImage.inl"
