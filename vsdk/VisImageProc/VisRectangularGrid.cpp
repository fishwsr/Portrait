// VisRectangularGrid.cpp
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


CVisRectangularGrid::CVisRectangularGrid(void)
  : m_cRow(0),
	m_cCol(0),
	m_fDotsInHolesInMarkers(true),
	m_fStoreThresholdedImage(false),
	m_fHaveGrid(false),
	m_cMarker(0),
	m_cLabeled(0),
	m_nPercentMinBlobSize(4),
	m_nThresholdSteps(2),
	m_iThresholdCur(- 1),
	m_iThresholdLow(0x60),
	m_iThresholdHigh(0xa0),
	m_fLimitThresholdOnAllSteps(false),
	m_fUpdateThresholdWhenGridFound(true)
{
}

bool CVisRectangularGrid::Extract(CVisByteImage& refimage)
{
	m_image = refimage;

	// Find the initial threshold to use to segment the image.
	int iThresholdT;
	if ((m_iThresholdCur >= 0) && (m_iThresholdCur < 255))
	{
		// Use a fixed threshold as the initial threshold value
		iThresholdT = m_iThresholdCur;
	}
	else
	{
		// Use a histogram of the image to find an initial threshold.
		CVisHistogram histogramT;
		VisHistogramFromImage(histogramT, m_image);
		histogramT.Lock();
		iThresholdT = histogramT.IThresholdOtsu();
		histogramT.Unlock();
	}

	// Limit the threshold, if needed.
	if ((m_iThresholdLow >= 0) && (m_iThresholdLow < 255)
			&& (m_iThresholdLow > iThresholdT))
	{
		iThresholdT = m_iThresholdLow;
	}
	if ((m_iThresholdHigh >= 0) && (m_iThresholdHigh < 255)
			&& (m_iThresholdHigh < iThresholdT))
	{
		iThresholdT = m_iThresholdHigh;
	}

	m_fHaveGrid = true;
	bool fFinalThresholdStep = false;
	BYTE bThreshold = (BYTE) iThresholdT;
	for (int iThresholdStep = 0; ((!fFinalThresholdStep) && (m_fHaveGrid));
			++iThresholdStep)
	{
		// Set a flag indicating whether this is the final thresholding step.
		// (The code below may change this flag if no additional steps will
		// be needed.)
		fFinalThresholdStep = (iThresholdStep == m_nThresholdSteps - 1);

		CVisFnOpPixelLEVal<BYTE> fnop(bThreshold);
		CVisPixRunList pixrunlist = PixRunListMakeImage(m_image, fnop);
		CVisBlobTreeInternal *pblobtree = new CVisBlobTreeInternal;
		
#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
CVisTimer2 timerT0;
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG
		// Make blob tree.  "false" means 4-connected foreground.
		// LATER:  This seems to be taking most of the time in the current
		// implementation.  We should look at improving the performance of
		// the connected-regions code, perhaps by using custom memory
		// management with some of the blob and strip structures.
		pblobtree->MakeTreeFromPixRunList(pixrunlist, false);
#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
// This seems to take a lot of time, usually between 5 and 15ms,
// with 12ms. typical, on my machine.  (erikhan)
char szBuf[256];
sprintf(szBuf, "Blob tree time = %d ms.\n", timerT0.CmsElapsed());
OutputDebugString(szBuf);
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG

		// Get potential grid points from the blob tree.  If it's not the final
		// thresholding step, we only need to find the marker blobs.
		GetPointsFromPBlobTree(pblobtree, !fFinalThresholdStep);

		if (m_cMarker < 2)
		{
			m_fHaveGrid = false;
		}
		else if (!fFinalThresholdStep)
		{
			// Use part of image around marker blobs to find the histogram
			// to use for the next iteration.
			RECT rectBounding = RectBoundingMarkers();
			if ((rectBounding.right - rectBounding.left == 1)
					|| (rectBounding.bottom - rectBounding.top == 1))
			{
				m_fHaveGrid = false;
			}
			else
			{
				// Use a histogram of the part of the image bordered by the
				// markers to find a better threshold estimate.
				CVisHistogram histogramT;
				VisHistogramFromImage(histogramT, m_image.SubImage(rectBounding));
				histogramT.Lock();
				int iThresholdNext = histogramT.IThresholdOtsu();
				histogramT.Unlock();

				// Limit the threshold, if needed.
				if (m_fLimitThresholdOnAllSteps)
				{
					if ((m_iThresholdLow >= 0) && (m_iThresholdLow < 255)
							&& (m_iThresholdLow > iThresholdNext))
					{
						iThresholdNext = m_iThresholdLow;
					}
					if ((m_iThresholdHigh >= 0) && (m_iThresholdHigh < 255)
							&& (m_iThresholdHigh < iThresholdNext))
					{
						iThresholdNext = m_iThresholdHigh;
					}
				}

				if (iThresholdNext != bThreshold)
				{
					// Set the threshold to use in the next iteration.
					bThreshold = (BYTE) histogramT.IThresholdOtsu();
				}
				else
				{
					// If the threshold hasn't changed, this should be the final step.
					fFinalThresholdStep = true;

					// We'll need to extract the points from the blob tree again, since
					// more information is required in the final step.
					GetPointsFromPBlobTree(pblobtree, !fFinalThresholdStep);
				}
			}
		}
		
		if (fFinalThresholdStep)
		{
			if ((m_cMarker != 4) || (m_vectorGridPoint.size() < 9))
			{
				m_fHaveGrid = false;
			}
			else
			{
#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
CVisTimer2 timerT1;
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG
				// Find close neighbors
				FindCloseNeighbors();
#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
// This time seems to be small, maybe 1ms.
sprintf(szBuf, "Find Neighbors time = %d ms.\n", timerT1.CmsElapsed());
OutputDebugString(szBuf);
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG

				// Limit marker points (which are corner points) to only two close neighbors
				for (int iMarker = 0; iMarker < 4; ++iMarker)
					m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].cCloseNeighbors = 2;

#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
CVisTimer2 timerT2;
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG
				m_fHaveGrid = (FOrderMarkers() && FFindLabels() && FMakeFinalGrid());				
#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
// This time seems to be small, maybe 1ms.
sprintf(szBuf, "Time for final steps = %d ms.\n", timerT2.CmsElapsed());
OutputDebugString(szBuf);
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG
			}
		}

		if ((!m_fHaveGrid) || (fFinalThresholdStep))
		{
			if (m_fStoreThresholdedImage)
			{
				BYTE bForeground = 255;
				CVisByteImage imageThresholded(m_image.Shape());
				SetFgBlobsInImage(*pblobtree, imageThresholded, bForeground);
				m_image = imageThresholded;
			}
		}

		// Cleanup
		delete pblobtree;
		pblobtree = 0;
	}

	if ((m_fUpdateThresholdWhenGridFound) && (m_fHaveGrid))
		m_iThresholdCur = bThreshold;

	return m_fHaveGrid;
}

void CVisRectangularGrid::GetPointsFromPBlobTree(CVisBlobTreeInternal *pblobtree,
		bool fOnlyMarkers)
{
	// Helper function to find possible grid points from a tree of blobs.

	// Find the size of the largest top-level blob.
	// LATER:  To make this more robust, try using the Kth largest
	// blob (say for K == 4).
	int nBlobSizeHigh = 0;
	CVisBlobInternal *pblob = pblobtree->PBlobFirst();
	for (; pblob != 0; pblob = pblob->NextSibling())
	{
		int nAreaBlob = pblob->Area();
		// We don't have to be too exact here, so we don't add the area of
		// holes in marker blobs.
//				if (pblob->FirstChild() != 0)
//					nAreaBlob += pblob->FirstChild()->Area();
		if ((nAreaBlob > nBlobSizeHigh)
				&& (pblob->RectBounding().left > 0)
				&& (pblob->RectBounding().top > 0)
				&& (pblob->RectBounding().right < m_image.Right())
				&& (pblob->RectBounding().bottom < m_image.Bottom()))
		{
			nBlobSizeHigh = nAreaBlob;
		}
	}


	// Find the top-level blobs that we want to use.
	m_cMarker = 0;
	m_vectorGridPoint.clear();
	int nBlobSizeLow = m_nPercentMinBlobSize * nBlobSizeHigh / 100;
	pblob = pblobtree->PBlobFirst();
	for (; pblob != 0; pblob = pblob->NextSibling())
	{
		// Skip small blobs
		if (pblob->Area() < nBlobSizeLow)
			continue;

		// Ship blobs on the border of the image
		if ((pblob->RectBounding().left == 0)
				|| (pblob->RectBounding().top == 0)
				|| (pblob->RectBounding().right == m_image.Right())
				|| (pblob->RectBounding().bottom == m_image.Bottom()))
			continue;

		CVisBlobInternal *pblobChild = pblob->FirstChild();
		bool fMarkerBlob = ((pblobChild != 0)
				&& (pblobChild->NextSibling() == 0)
				&& (((!m_fDotsInHolesInMarkers)
						&& (pblobChild->FirstChild() == 0))
					|| ((m_fDotsInHolesInMarkers)
						&& (pblobChild->FirstChild() != 0)
						&& (pblobChild->FirstChild()->NextSibling() == 0)
						&& (pblobChild->FirstChild()->FirstChild() == 0))));

		// If it's not the final step, we're only concerned about marker blobs.
		if ((fOnlyMarkers) && (!fMarkerBlob))
			continue;

		if ((pblobChild == 0) || (fMarkerBlob))
		{
			int nAreaBlob = pblob->Area();
			double dblSumX = pblob->SumX();
			double dblSumY = pblob->SumY();

			if (fMarkerBlob)
			{
				nAreaBlob += pblobChild->Area();
				dblSumX += pblobChild->SumX();
				dblSumY += pblobChild->SumY();

				if (m_fDotsInHolesInMarkers)
				{
					assert(pblobChild->FirstChild() != 0);
					nAreaBlob += pblobChild->FirstChild()->Area();
					dblSumX += pblobChild->FirstChild()->SumX();
					dblSumY += pblobChild->FirstChild()->SumY();
				}

				if (m_cMarker < (sizeof(m_mpiiMarkerToPoint) / sizeof(int)))
					m_mpiiMarkerToPoint[m_cMarker] = m_vectorGridPoint.size();
				++ m_cMarker;
			}

			m_vectorGridPoint.push_back(SVisGridPoint());
			m_vectorGridPoint.back().x = dblSumX / nAreaBlob;
			m_vectorGridPoint.back().y = dblSumY / nAreaBlob;
			m_vectorGridPoint.back().nArea = nAreaBlob;
			m_vectorGridPoint.back().fMarker = fMarkerBlob;
			m_vectorGridPoint.back().fBorder = false;
			m_vectorGridPoint.back().r = - 1;
			m_vectorGridPoint.back().c = - 1;
			m_vectorGridPoint.back().cCloseNeighbors = 0;
		}
	}
}

RECT CVisRectangularGrid::RectBoundingMarkers(void) const
{
	// Helper function to find the RECT bounding the marker points
	RECT rectBounding;

	// Assume that there's at least one marker
	assert(m_cMarker > 0);
	rectBounding.left = (int) floor(
			m_vectorGridPoint[m_mpiiMarkerToPoint[0]].x + 0.5);
	rectBounding.right = rectBounding.left + 1;
	rectBounding.top = (int) floor(
			m_vectorGridPoint[m_mpiiMarkerToPoint[0]].y + 0.5);
	rectBounding.bottom = rectBounding.top + 1;

	int iMarkerLim = min(m_cMarker, (sizeof(m_mpiiMarkerToPoint) / sizeof(int)));
	for (int iMarker = 1; iMarker < iMarkerLim; ++iMarker)
	{
		int xCur = (int) floor(
				m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].x + 0.5);
		int yCur = (int) floor(
				m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].y + 0.5);
		if (rectBounding.left > xCur)
			rectBounding.left = xCur;
		if (rectBounding.right < xCur + 1)
			rectBounding.right = xCur + 1;
		if (rectBounding.top > yCur)
			rectBounding.top = yCur;
		if (rectBounding.bottom < yCur + 1)
			rectBounding.bottom = yCur + 1;
	}

	return rectBounding;
}

bool CVisRectangularGrid::FOrderMarkers(void)
{
	assert(m_cMarker == 4);

	int iLeft = 0;
	for (int iMarker = 1; iMarker < 4; ++iMarker)
	{
		if ((m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].x
					< m_vectorGridPoint[m_mpiiMarkerToPoint[iLeft]].x)
				|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].x
						== m_vectorGridPoint[m_mpiiMarkerToPoint[iLeft]].x)
					&& (m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].y
						< m_vectorGridPoint[m_mpiiMarkerToPoint[iLeft]].y)))
		{
			iLeft = iMarker;
		}
	}

	// Move the left marker to the start of the list.
	if (iLeft != 0)
	{
		int iPointT = m_mpiiMarkerToPoint[iLeft];
		m_mpiiMarkerToPoint[iLeft] = m_mpiiMarkerToPoint[0];
		m_mpiiMarkerToPoint[0] = iPointT;
		iLeft = 0;
	}

	// Find primary and secondary orderings to use to sort the remaining markers.
	int rgiOrderPrimary[3];
	double rgdblOrderSecondary[3];
	for (iMarker = 1; iMarker < 4; ++iMarker)
	{
		double dblDxT = m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].x
				- m_vectorGridPoint[m_mpiiMarkerToPoint[0]].x;
		double dblDyT = m_vectorGridPoint[m_mpiiMarkerToPoint[iMarker]].y
				- m_vectorGridPoint[m_mpiiMarkerToPoint[0]].y;
		if (dblDxT > 0)
		{
			rgiOrderPrimary[iMarker - 1] = 0;
			rgdblOrderSecondary[iMarker - 1] = dblDyT / dblDxT;
		}
		else if (dblDxT == 0)
		{
			rgiOrderPrimary[iMarker - 1] = 1;
			rgdblOrderSecondary[iMarker - 1] = - dblDyT;
		}
		else if (dblDyT > 0)
		{
			rgiOrderPrimary[iMarker - 1] = 2;
			rgdblOrderSecondary[iMarker - 1] = - dblDxT / dblDyT;
		}
		else if (dblDyT == 0)
		{
			rgiOrderPrimary[iMarker - 1] = 3;
			rgdblOrderSecondary[iMarker - 1] = - dblDxT;
		}
		else
		{
			rgiOrderPrimary[iMarker - 1] = 4;
			rgdblOrderSecondary[iMarker - 1] = - dblDxT / dblDyT;
		}
	}

	// Sort the remaining markers.
	for (int iSort0 = 0; iSort0 < 2; ++iSort0)
	{
		for (int iSort1 = iSort0 + 1; iSort1 < 3; ++iSort1)
		{
			if ((rgiOrderPrimary[iSort0] > rgiOrderPrimary[iSort1])
					|| ((rgiOrderPrimary[iSort0] == rgiOrderPrimary[iSort1])
						&& (rgdblOrderSecondary[iSort0] > rgdblOrderSecondary[iSort1])))
			{
				int iT = m_mpiiMarkerToPoint[iSort0 + 1];
				m_mpiiMarkerToPoint[iSort0 + 1] = m_mpiiMarkerToPoint[iSort1 + 1];
				m_mpiiMarkerToPoint[iSort1 + 1] = iT;

				iT = rgiOrderPrimary[iSort0];
				rgiOrderPrimary[iSort0] = rgiOrderPrimary[iSort1];
				rgiOrderPrimary[iSort1] = iT;

				double dblT = rgdblOrderSecondary[iSort0];
				rgdblOrderSecondary[iSort0] = rgdblOrderSecondary[iSort1];
				rgdblOrderSecondary[iSort1] = dblT;
			}
		}
	}

	return true;  // UNDONE:  Fail in some cases (eg all markers on same line)?
}

void CVisRectangularGrid::FindCloseNeighbors(void)
{
	// Helper function to find close neighbors for each point.
	for (int i = 0; i < m_vectorGridPoint.size(); ++i)
	{
		SVisGridPoint& refgridpointCur = m_vectorGridPoint[i];
		for (int j = 0; j < m_vectorGridPoint.size(); ++j)
		{
			if (i == j)
				continue;

			SVisGridPoint& refgridpointOther = m_vectorGridPoint[j];

			double dblDxT = (refgridpointCur.x - refgridpointOther.x);
			double dblDyT = (refgridpointCur.y - refgridpointOther.y);
			double dblDistSquared = (dblDxT * dblDxT + dblDyT * dblDyT);

			if ((refgridpointCur.cCloseNeighbors < 4)
					|| (dblDistSquared
							< refgridpointCur.rgdblDistSquaredToCloseNeighbor[3]))
			{
				for (int iCN = refgridpointCur.cCloseNeighbors; iCN > 0; --iCN)
				{
					if (dblDistSquared
							< refgridpointCur.rgdblDistSquaredToCloseNeighbor[iCN - 1])
					{
						if (iCN < 4)
						{
							refgridpointCur.mpiiCloseNeighborToGridPoint[iCN]
									= refgridpointCur.mpiiCloseNeighborToGridPoint[iCN - 1];
							refgridpointCur.rgdblDistSquaredToCloseNeighbor[iCN]
									= refgridpointCur.rgdblDistSquaredToCloseNeighbor[iCN - 1];
						}
					}
					else
					{
						break;
					}
				}
				refgridpointCur.mpiiCloseNeighborToGridPoint[iCN] = j;
				refgridpointCur.rgdblDistSquaredToCloseNeighbor[iCN] = dblDistSquared;

				if (refgridpointCur.cCloseNeighbors < 4)
					++refgridpointCur.cCloseNeighbors;
			}
		}
	}
}

bool CVisRectangularGrid::FFindLabels(void)
{
	// Helper function to label rows and columns in the grid.
	bool fFail = false;
	m_cLabeled = 0;

	// Label one of the markers
	m_vectorGridPoint[m_mpiiMarkerToPoint[0]].r = 0;
	m_vectorGridPoint[m_mpiiMarkerToPoint[0]].c = 0;
	++m_cLabeled;

	//
	// Label border points
	//

	for (int iBorder = 0; iBorder < 2; ++iBorder)
	{
		// Label a border connected to the first marker
		int iPrev = m_mpiiMarkerToPoint[0];
		int iCur = m_vectorGridPoint[iPrev].mpiiCloseNeighborToGridPoint[iBorder];
		if (iBorder == 0)
		{
			m_vectorGridPoint[iCur].r = 0;
			m_vectorGridPoint[iCur].c = 1;
		}
		else
		{
			m_vectorGridPoint[iCur].r = 1;
			m_vectorGridPoint[iCur].c = 0;
		}
		m_vectorGridPoint[iCur].fBorder = true;
		m_vectorGridPoint[iCur].cCloseNeighbors = 3;
		++ m_cLabeled;

		while (!m_vectorGridPoint[iCur].fMarker)
		{
			int iNext = INextLabeledFromIPrevICurBorder(iPrev, iCur);
			if (iNext == - 1)
			{
				fFail = true;
				break;
			}

			iPrev = iCur;
			iCur = iNext;
		}

		if (fFail)
			break;

		// Label a border connected to this other marker
		int iMarker = iCur;
		if (iPrev == m_vectorGridPoint[iCur].mpiiCloseNeighborToGridPoint[0])
		{
			iCur = m_vectorGridPoint[iCur].mpiiCloseNeighborToGridPoint[1];
		}
		else if (iPrev == m_vectorGridPoint[iCur].mpiiCloseNeighborToGridPoint[1])
		{
			iCur = m_vectorGridPoint[iCur].mpiiCloseNeighborToGridPoint[0];
		}
		else
		{
			fFail = true;
			break;
		}
		iPrev = iMarker;
		if (iBorder == 0)
		{
			m_vectorGridPoint[iCur].r = 1;
			m_vectorGridPoint[iCur].c = m_vectorGridPoint[iMarker].c;
		}
		else
		{
			m_vectorGridPoint[iCur].r = m_vectorGridPoint[iMarker].r;
			m_vectorGridPoint[iCur].c = 1;
		}
		m_vectorGridPoint[iCur].fBorder = true;
		m_vectorGridPoint[iCur].cCloseNeighbors = 3;
		++m_cLabeled;

		while (!m_vectorGridPoint[iCur].fMarker)
		{
			int iNext = INextLabeledFromIPrevICurBorder(iPrev, iCur);
			if (iNext == - 1)
			{
				fFail = true;
				break;
			}

			iPrev = iCur;
			iCur = iNext;
		}

		if (fFail)
			break;
	}

	//
	// Label interior points
	//


	if (!fFail)
	{
		// Go along points on one of the borders
		int iBorderPrev = m_mpiiMarkerToPoint[0];
		int iBorderCur = m_vectorGridPoint[iBorderPrev].mpiiCloseNeighborToGridPoint[0];
		while (!m_vectorGridPoint[iBorderCur].fMarker)
		{
			// Find the interior point and next border point connected to this neighbor.
			int iCur = - 1;
			int iBorderNext = - 1;
			for (int iNeighbor = 0; iNeighbor < 3; ++iNeighbor)
			{
				int iPoint = m_vectorGridPoint[iBorderCur].mpiiCloseNeighborToGridPoint[iNeighbor];
				if (iPoint != iBorderPrev)
				{
					// LATER:  Optimize by stopping when (m_vectorGridPoint[iPoint].fMarker).
					if ((m_vectorGridPoint[iPoint].fBorder) || (m_vectorGridPoint[iPoint].fMarker))
						iBorderNext = iPoint;
					else
						iCur = iPoint;
				}
			}

			if ((iCur == - 1) || (iBorderNext == - 1))
			{
				fFail = true;
				break;
			}

			// Advance along the border
			int iPrev = iBorderCur;
			iBorderCur = iBorderNext;
			iBorderPrev = iPrev;

			// Label interior points
			while (!m_vectorGridPoint[iCur].fBorder)
			{
				// LATER:  We might be able to optimize this to find the neighbors
				// when needed and to label both the "next" and "other" points.  By
				// labeling the "other" points when we label the "next" points, we
				// should only need to process every other row to label all points
				// in the image.
				int iNext = INextLabeledFromIPrevICurInterior(iPrev, iCur);
				if (iNext == - 1)
				{
					fFail = true;
					break;
				}

				iPrev = iCur;
				iCur = iNext;
			}

			if (fFail)
				break;
		}
	}

	// Check that the number of rows and columns is right.
	int iRowLast = m_vectorGridPoint[m_mpiiMarkerToPoint[2]].r;
	int iColLast = m_vectorGridPoint[m_mpiiMarkerToPoint[2]].c;
	int cRow = iRowLast + 1;
	int cCol = iColLast + 1;
	if (((m_vectorGridPoint[m_mpiiMarkerToPoint[1]].r == 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[1]].c != iColLast))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[1]].c == 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[1]].r != iRowLast))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[3]].r == 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[3]].c != iColLast))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[3]].c == 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[3]].r != iRowLast))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[1]].c != 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[1]].r != 0))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[1]].r != 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[3]].r != 0))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[1]].c != 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[3]].c != 0))
			|| ((m_vectorGridPoint[m_mpiiMarkerToPoint[3]].c != 0)
				&& (m_vectorGridPoint[m_mpiiMarkerToPoint[3]].r != 0))
			|| (((m_cRow > 0) && (m_cCol > 0))
				&& (((m_cRow != cRow) && (m_cCol != cRow))
					|| ((m_cRow != cCol) && (m_cCol != cCol))
					|| (m_cLabeled != m_cRow * m_cCol))))
	{
		fFail = true;
	}
	else if ((m_cRow <= 0) && (m_cCol <= 0))
	{
		// Remember the number of rows and columns
		if (cRow <= cCol)
		{
			m_cRow = cRow;
			m_cCol = cCol;
		}
		else
		{
			m_cRow = cCol;
			m_cCol = cRow;
		}
	}

	return (!fFail);
}

int CVisRectangularGrid::INextLabeledFromIPrevICurBorder(int iPrev, int iCur)
{
	// Helper function used when labeling border rows
	assert((0 <= iPrev) && (iPrev < m_vectorGridPoint.size()));
	assert((m_vectorGridPoint[iPrev].fMarker) || (m_vectorGridPoint[iPrev].fBorder));
	assert(m_vectorGridPoint[iPrev].r >= 0);
	assert(m_vectorGridPoint[iPrev].c >= 0);
	assert((0 <= iCur) && (iCur < m_vectorGridPoint.size()));
	assert(m_vectorGridPoint[iCur].fBorder);
	assert(m_vectorGridPoint[iCur].r >= 0);
	assert(m_vectorGridPoint[iCur].c >= 0);

	bool fFail = false;

	SVisGridPoint& refgridpointCur = m_vectorGridPoint[iCur];

	int iNext = refgridpointCur.mpiiCloseNeighborToGridPoint[0];
	if (iNext == iPrev)
		iNext = refgridpointCur.mpiiCloseNeighborToGridPoint[2];
		
	int iOther = refgridpointCur.mpiiCloseNeighborToGridPoint[1];
	if (iOther == iPrev)
		iOther = refgridpointCur.mpiiCloseNeighborToGridPoint[2];

	double dblDxPrev = m_vectorGridPoint[iPrev].x - refgridpointCur.x;
	double dblDyPrev = m_vectorGridPoint[iPrev].y - refgridpointCur.y;
	double dblDxNext = m_vectorGridPoint[iNext].x - refgridpointCur.x;
	double dblDyNext = m_vectorGridPoint[iNext].y - refgridpointCur.y;
	double dblDxOther = m_vectorGridPoint[iOther].x - refgridpointCur.x;
	double dblDyOther = m_vectorGridPoint[iOther].y - refgridpointCur.y;

	// Compare dot products to see if we chose iNext correctly.
	if ((dblDxPrev * dblDxOther + dblDyPrev * dblDyOther)
			< (dblDxPrev * dblDxNext + dblDyPrev * dblDyNext))
	{
		// Not correct.  Switch next and other.
		int iT = iNext;
		iNext = iOther;
		iOther = iT;
	}

	// Label next border point
	int dr = refgridpointCur.r - m_vectorGridPoint[iPrev].r;
	int dc = refgridpointCur.c - m_vectorGridPoint[iPrev].c;
	if (!m_vectorGridPoint[iNext].fMarker)
	{
		m_vectorGridPoint[iNext].fBorder = true;
		m_vectorGridPoint[iNext].cCloseNeighbors = 3;
	}
	if ((m_vectorGridPoint[iNext].r < 0) && (m_vectorGridPoint[iNext].c < 0))
	{
		m_vectorGridPoint[iNext].r = refgridpointCur.r + dr;
		m_vectorGridPoint[iNext].c = refgridpointCur.c + dc;
		++m_cLabeled;
	}
	
	if ((m_vectorGridPoint[iNext].r != refgridpointCur.r + dr)
			|| (m_vectorGridPoint[iNext].c != refgridpointCur.c + dc))
	{
		fFail = true;
	}

	// Label the other (interior) point
	int rOther = refgridpointCur.r;
	int cOther = refgridpointCur.c;
	if ((refgridpointCur.r == 0) || (refgridpointCur.c == 0))
	{
		if (dr == 0)
			++ rOther;
		else
			++ cOther;
	}
	else
	{
		if (dr == 0)
			-- rOther;
		else
			-- cOther;
	}
	if ((m_vectorGridPoint[iOther].r < 0) && (m_vectorGridPoint[iOther].c < 0))
	{
		m_vectorGridPoint[iOther].r = rOther;
		m_vectorGridPoint[iOther].c = cOther;
		++m_cLabeled;
	}

	if ((m_vectorGridPoint[iOther].r != rOther)
			|| (m_vectorGridPoint[iOther].c != cOther))
	{
		fFail = true;
	}

	if (fFail)
		iNext = - 1;

	return iNext;
}

int CVisRectangularGrid::INextLabeledFromIPrevICurInterior(int iPrev, int iCur)
{
	// Helper function used when labeling interior rows
	assert((0 <= iPrev) && (iPrev < m_vectorGridPoint.size()));
	assert(!m_vectorGridPoint[iPrev].fMarker);
	assert(m_vectorGridPoint[iPrev].r >= 0);
	assert(m_vectorGridPoint[iPrev].c >= 0);
	assert((0 <= iCur) && (iCur < m_vectorGridPoint.size()));
	assert(!m_vectorGridPoint[iCur].fBorder);
	assert(m_vectorGridPoint[iCur].r >= 0);
	assert(m_vectorGridPoint[iCur].c >= 0);

	SVisGridPoint& refgridpointCur = m_vectorGridPoint[iCur];

	int iNext = refgridpointCur.mpiiCloseNeighborToGridPoint[0];
	int iNeighbor = 1;
	if (iNext == iPrev)
	{
		iNext = refgridpointCur.mpiiCloseNeighborToGridPoint[1];
		iNeighbor = 2;
	}

	double dblDxPrev = m_vectorGridPoint[iPrev].x - refgridpointCur.x;
	double dblDyPrev = m_vectorGridPoint[iPrev].y - refgridpointCur.y;
	double dblDxNext = m_vectorGridPoint[iNext].x - refgridpointCur.x;
	double dblDyNext = m_vectorGridPoint[iNext].y - refgridpointCur.y;

	for (; iNeighbor < 4; ++iNeighbor)
	{
		double dblDxNeighbor = m_vectorGridPoint[
						refgridpointCur.mpiiCloseNeighborToGridPoint[iNeighbor]].x
				- refgridpointCur.x;
		double dblDyNeighbor = m_vectorGridPoint[
						refgridpointCur.mpiiCloseNeighborToGridPoint[iNeighbor]].y
				- refgridpointCur.y;

		// Compare dot products to find the right choce for iNext.
		if ((dblDxPrev * dblDxNeighbor + dblDyPrev * dblDyNeighbor)
				< (dblDxPrev * dblDxNext + dblDyPrev * dblDyNext))
		{
			iNext = refgridpointCur.mpiiCloseNeighborToGridPoint[iNeighbor];
		}
	}


	// Label next point
	int dr = refgridpointCur.r - m_vectorGridPoint[iPrev].r;
	int dc = refgridpointCur.c - m_vectorGridPoint[iPrev].c;
	if ((m_vectorGridPoint[iNext].r < 0) && (m_vectorGridPoint[iNext].c < 0))
	{
		m_vectorGridPoint[iNext].r = refgridpointCur.r + dr;
		m_vectorGridPoint[iNext].c = refgridpointCur.c + dc;
		++m_cLabeled;
	}

	// Fail if the labels are not right.
	if ((m_vectorGridPoint[iNext].r != refgridpointCur.r + dr)
			|| (m_vectorGridPoint[iNext].c != refgridpointCur.c + dc))
	{
		iNext = - 1;
	}

	return iNext;
}

bool CVisRectangularGrid::FMakeFinalGrid(void)
{
	// Helper function to make the final grid after the initial label have been found.
	bool fFail = false;
	bool fFlipRC = (m_vectorGridPoint[m_mpiiMarkerToPoint[2]].r + 1 != m_cRow);
	bool fFlipOrientation = (m_vectorGridPoint[m_mpiiMarkerToPoint[1]].r != 0);

	std::vector<double> vectordblX;
	std::vector<double> vectordblY;
	std::vector<int> vectornArea;

	int rLast = m_vectorGridPoint[m_mpiiMarkerToPoint[2]].r;
	int cLast = m_vectorGridPoint[m_mpiiMarkerToPoint[2]].c;
	int iLim = m_cRow * m_cCol;

	vectordblX.resize(iLim);
	vectordblY.resize(iLim);
	vectornArea.resize(iLim);

	for (int iPointCur = 0; iPointCur < m_vectorGridPoint.size(); ++iPointCur)
	{
		SVisGridPoint& refgridpointCur = m_vectorGridPoint[iPointCur];

		if (refgridpointCur.r < 0)
			continue;
		
		assert(refgridpointCur.c >= 0);

		int iNew;
		if (fFlipRC)
		{
			if (fFlipOrientation)
			{
				iNew = refgridpointCur.c * m_cCol + refgridpointCur.r;
			}
			else
			{
				iNew = (cLast - refgridpointCur.c) * m_cCol + refgridpointCur.r;
			}
		}
		else
		{
			if (fFlipOrientation)
			{
				iNew = (rLast - refgridpointCur.r) * m_cCol + refgridpointCur.c;
			}
			else
			{
				iNew = refgridpointCur.r * m_cCol + refgridpointCur.c;
			}
		}

		if (iNew >= iLim)
		{
			fFail = true;
			break;
		}


		vectordblX[iNew] = refgridpointCur.x;
		vectordblY[iNew] = refgridpointCur.y;
		vectornArea[iNew] = refgridpointCur.nArea;
	}

	m_vectorGridPoint.clear();
	m_vectorGridPoint.resize(iLim);
	int c = 0;
	int r = 0;
	rLast = m_cRow - 1;
	cLast = m_cCol - 1;
	for (int i = 0; i < iLim; ++i)
	{
		SVisGridPoint& refgridpointCur = m_vectorGridPoint[i];
		refgridpointCur.x = vectordblX[i];
		refgridpointCur.y = vectordblY[i];
		refgridpointCur.nArea = vectornArea[i];
		refgridpointCur.r = r;
		refgridpointCur.c = c;

		bool fBorderRow = ((r == 0) || (r == rLast));
		bool fBorderCol = ((c == 0) || (c == cLast));
		refgridpointCur.fMarker = (fBorderRow && fBorderCol);
		refgridpointCur.fBorder = (fBorderRow || fBorderCol);

		refgridpointCur.cCloseNeighbors = 0;  // LATER:  An option to add this info?

		// Increment the (r, c) position
		if (++c == m_cCol)
		{
			c = 0;
			++ r;
		}
	}

	return !fFail;
}

