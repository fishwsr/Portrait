// VisDisplayImage.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#include "VisDisplayPch.h"
#include "VisDisplayProj.h"

#include <math.h>


VisDisplayExport CVisPane VISAPI VisDisplayImage(
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisImageBase& refimage,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	CVisPane pane(pointOffsetInParent, sizePaneInside, szTitle,
			evispane, pwndParent, true);

	pane.Display(evisnormalize, dblMin, dblMax, refimage, evispanedisp);

	return pane;
}


// Constants

// The max number of panes allows in an array
const int kviscPaneInArrayMax = 128;


// Helper functions

inline CSize VisSizeScreen(void)
{
	return CSize(GetSystemMetrics(SM_CXFULLSCREEN),
			GetSystemMetrics(SM_CYFULLSCREEN));
}

inline CSize VisSizeMaxFromSequence(const CVisSequenceBase& refsequence,
		int iFirst, int iLim)
{
	CSize size(0, 0);

	if (iFirst < 0)
		iFirst = 0;
	if ((iLim < 0) || (iLim > refsequence.Length()))
		iLim = refsequence.Length();
	if (iLim - iFirst > kviscPaneInArrayMax)
		iLim = iFirst + kviscPaneInArrayMax;

	for (int i = iFirst; i < iLim; ++i)
	{
		if (refsequence[i].Width() > size.cx)
			size.cx = refsequence[i].Width();
		if (refsequence[i].Height() > size.cy)
			size.cy = refsequence[i].Height();
	}

	return size;
}


inline int VisCRowForPanes(int cPane, SIZE sizePaneMax, SIZE sizeBounding)
{
	int cRow;
	if (cPane <= 1)
	{
		cRow = 1;
	}
	else
	{
		if (cPane > kviscPaneInArrayMax)
			cPane = kviscPaneInArrayMax;

		// Make sure we don't divide by zero in the calculations below.
		if (sizePaneMax.cx <= 0)
			sizePaneMax.cx = 1;
		if (sizePaneMax.cy <= 0)
			sizePaneMax.cy = 1;
		if (sizeBounding.cx <= 0)
			sizeBounding.cx = 1;
		if (sizeBounding.cy <= 0)
			sizeBounding.cy = 1;

		// Since dxArray ~ dxPane * cCol and dyArray ~ dyPane * cRow, and the
		// bounding dimensions limit the array dimensions, big arrays will want
		// (cRow / cCol) ~ (dxPane / dyPane) * (dyBounding / dxBounding).  Because
		// cCol * cRow >= cPane, big arrays will want
		// cRow >= sqrt(cPane * (dxPane / dyPane) * (dyBounding / dxBounding)).
		int cRowRightAspect = floor(sqrt(cPane
				* ((double) sizePaneMax.cx) / ((double) sizePaneMax.cy)
				* ((double) sizeBounding.cy) / ((double) sizeBounding.cx)) + 0.5);

		// These should be true, but we check them to make sure we didn't overlook anything.
		// LATER:  Verify that these are true and remove (or replace with asserts).
		if (cRowRightAspect < 1)
			cRowRightAspect = 1;
		if (cRowRightAspect > cPane)
			cRowRightAspect = cPane;

		// Find the lower and upper limits on cRow for arrays that will fit on
		// the screen.
		int cRowLow = floor(cPane * ((double) sizePaneMax.cx)
				/ ((double) sizeBounding.cx));
		if (cRowLow < 1)
			cRowLow = 1;
		int cRowHigh= ceil(((double) sizeBounding.cy) / ((double) sizePaneMax.cy));
		if (cRowHigh > cPane)
			cRowHigh = cPane;

		if (cRowLow > cRowHigh)
		{
			// We won't be able to make all panes fit.  Use cRowRightAspect.
			// LATER:  Should we try other numbers of rows?
			cRow = cRowRightAspect;
		}
		else
		{
			// We want to minimize the number of extra panes in the array.
			// LATER:  Make this more efficient.  (We don't need to check each cRow value.)
			int cPaneExtraMin = cPane;
			for (cRow = cRowLow; cRow <= cRowHigh; ++cRow)
			{
				if (cRow * ((cPane + cRow - 1) / cRow) - cPane < cPaneExtraMin)
					cPaneExtraMin = cRow * ((cPane + cRow - 1) / cRow) - cPane;
			}

			// In most cases, it's okay to have up to three extra panes in the pane array.
			// LATER:  Review this.
			if (cPaneExtraMin < 3)
				cPaneExtraMin = 3;

			// Now find the cRow values closest to cRowRightAspect that minimizes the number
			// of extra panes.
			// LATER:  Is this measure of "closeness" okay, or should we use another measure?
			int cRowIdeal = - cPane;
			int dcRowMin = cPane;
			for (cRow = cRowLow; cRow <= cRowHigh; ++cRow)
			{
				if (cRow * ((cPane + cRow - 1) / cRow) - cPane <= cPaneExtraMin)
				{
					int dcRow = cRow - cRowRightAspect;
					if (dcRow < 0)
						dcRow = - dcRow;

					if (dcRow < dcRowMin)
					{
						dcRowMin = dcRow;
						cRowIdeal = cRow;
					}
				}
			}

			cRow = cRowIdeal;
		}
	}

	assert(cRow > 0);
	assert(cRow <= cPane);

	return cRow;
}

inline int VisCRowForPanes(int cPane, SIZE sizePaneMax)
{
	return VisCRowForPanes(cPane, sizePaneMax, VisSizeScreen());
}



VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(CVisPoint(0, 0),
			VisSizeMaxFromSequence(refsequence, iimageFirst, iimageLim),
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	int cRow;
	int cCol;

	if (refsequence.Length() == 0)
	{
		cRow = 1;
		cCol = 1;
	}
	else
	{
		cRow = VisCRowForPanes(refsequence.Length(), sizePaneInside);
		cCol = (refsequence.Length() + cRow - 1) / cRow;
	}

	return VisDisplayImages(cCol, cRow,
			pointOffsetInParent, sizePaneInside,
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


VisDisplayExport CVisPaneArray VisDisplayImages(const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	int cRow;
	int cCol;

	if (refsequence.Length() == 0)
	{
		cRow = 1;
		cCol = 1;
	}
	else
	{
		CSize sizePaneMax = VisSizeMaxFromSequence(refsequence,
				iimageFirst, iimageLim);
		cRow = VisCRowForPanes(refsequence.Length(), sizePaneMax,
				((const CRect&) refrectInParent).Size());
		cCol = (refsequence.Length() + cRow - 1) / cRow;
	}

	return VisDisplayImages(cCol, cRow, refrectInParent,
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}



VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		int nPaneCols, int nPaneRows,
		EVisNormalize evisnormalize,
		double dblMin, double dblMax, const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	return VisDisplayImages(nPaneCols, nPaneRows, CVisPoint(0, 0),
			VisSizeMaxFromSequence(refsequence, iimageFirst, iimageLim),
			evisnormalize, dblMin, dblMax, refsequence,
			iimageFirst, iimageLim,
			evispanedisp, szTitle, evispane, pwndParent);
}


VisDisplayExport CVisPaneArray VISAPI VisDisplayImages(
		int nPaneCols, int nPaneRows,
		POINT pointOffsetInParent, SIZE sizePaneInside,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	CVisPaneArray panearray(nPaneCols, nPaneRows, pointOffsetInParent,
			sizePaneInside, szTitle, evispane, pwndParent, false);

	CSize sizeScreen(VisSizeScreen());
	if (panearray.WidthOutside() > sizeScreen.cx)
		panearray.SetWidthOutside(sizeScreen.cx);
	if (panearray.HeightOutside() > sizeScreen.cy)
		panearray.SetHeightOutside(sizeScreen.cy);

	if (refsequence.Length() == 0)
	{
		panearray.CreateWnd();
	}
	else
	{
		panearray.Display(evisnormalize, dblMin, dblMax,
			refsequence, iimageFirst, iimageLim, 0,
			evispanedisp);
	}

	return panearray;
}


VisDisplayExport CVisPaneArray VisDisplayImages(int nPaneCols, int nPaneRows,
		const RECT& refrectInParent,
		EVisNormalize evisnormalize, double dblMin, double dblMax,
		const CVisSequenceBase& refsequence,
		int iimageFirst, int iimageLim,
		EVisPaneDisp evispanedisp,
		const char *szTitle, int evispane,
		CWnd *pwndParent)
{
	CVisPaneArray panearray(nPaneCols, nPaneRows, refrectInParent,
			szTitle, evispane, pwndParent, false);

	CSize sizeScreen(VisSizeScreen());
	if (panearray.WidthOutside() > sizeScreen.cx)
		panearray.SetWidthOutside(sizeScreen.cx);
	if (panearray.HeightOutside() > sizeScreen.cy)
		panearray.SetHeightOutside(sizeScreen.cy);

	if (refsequence.Length() == 0)
	{
		panearray.CreateWnd();
	}
	else
	{
		panearray.Display(evisnormalize, dblMin, dblMax,
			refsequence, iimageFirst, iimageLim, 0,
			evispanedisp);
	}

	return panearray;
}
