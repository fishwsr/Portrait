///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisRegionExtract.cpp -- extract lines from images
//
// DESCRIPTION
//  Extract connected component regions from an image.
//
// SEE ALSO
//  VisRegionExtract.h          longer description of classes
//
//  Rick's research notes, 16-May-98
//
// BUGS
//
//
// DESIGN
//  
//
// HISTORY
//  18-May-98  Richard Szeliski (szeliski) at Microsoft
//  Split off from VisLineExtract.h.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#include <algorithm>

#ifndef M_PI
const double M_PI = 3.1415926535;
#endif

//
// The CVisConnectedRegion is used for connected component computation
//

CVisConnectedRegion& CVisConnectedRegion::operator+=(CVisRegionRun& run)
{
    assert(id == run.id);
    n += run.n;
    x += run.n * run.x + run.n * (run.n - 1) / 2;
    y += run.n * run.y;
    return *this;
}

CVisConnectedRegion& CVisConnectedRegion::operator+=(CVisConnectedRegion& r)
{
    n += r.n;
    x += r.x;
    y += r.y;
    return *this;
}

void CVisConnectedRegion::Fit()
{
    if (n <= 0)
        return;
    x /= n;
    y /= n;
}

void CVisConnectedRegion::AddCloseNeighbor(int iNeighbor, bool fBorder,
		double dblDistSquared)
{
	if ((nCloseNeighbors < 4)
			|| (dblDistSquared < dblSquaredDistToNeighbor[3]))
	{
		for (int i = nCloseNeighbors; i > 0; --i)
		{
			if (dblDistSquared < dblSquaredDistToNeighbor[i - 1])
			{
				if (i < 4)
				{
					iCloseNeighbor[i] = iCloseNeighbor[i - 1];
					fBorderNeighbor[i] = fBorderNeighbor[i - 1];
					dblSquaredDistToNeighbor[i] = dblSquaredDistToNeighbor[i - 1];
				}
			}
			else
			{
				break;
			}
		}
		iCloseNeighbor[i] = iNeighbor;
		fBorderNeighbor[i] = fBorder;
		dblSquaredDistToNeighbor[i] = dblDistSquared;

		if (nCloseNeighbors < 4)
			++nCloseNeighbors;
	}
}

//
// The CVisRegionExtract class extract connected components from an image
//

void CVisRegionExtract::SetImage(CVisByteImage& input)
{
    // Set the source image
    m_input = input;
}

struct CVisTriStateThreshold {
    void operator()(unsigned char& p) {
        p = (p & 0x80) ? 100 : 255;
    }
};

void CVisRegionExtract::InitializeLabels(int n_level_diff, int n_blur)
{
    // Compute a difference of Gaussians image
    m_label = DifferenceOfGaussian(m_input, n_level_diff, n_blur);

    // Threshold the image
    VisMap1(CVisTriStateThreshold(), m_label);
}

// This probably belongs in VisConvolve ...
void VisErodeLabel(CVisByteImage& src, CVisByteImage& tmp)
{
    // What should we do about boundary pixels???
    //  this needs to be resolved at same time as general convolution...

    // Do a vertical erosion pass
    for (int y = src.Top(); y < src.Bottom(); y++) {
        unsigned char *s0 = src.PtrToFirstPixelInRow(y-(y>src.Top()));
        unsigned char *s1 = src.PtrToFirstPixelInRow(y);
        unsigned char *s2 = src.PtrToFirstPixelInRow(y+(y<src.Bottom()-1));
        unsigned char *dp = tmp.PtrToFirstPixelInRow(y);
        for (int x = 0, width = src.Width(); x < width; x++) {
            int val = s1[x];
            dp[x] = (s0[x] != val || s2[x] != val) ? 0 : val;
        }
    }

    // Do a horizontal erosion pass
    //  note that we could use a small 3-row buffer instead of tmp,
    //  but this should be left to the general separable convolution code...
    for (y = src.Top(); y < src.Bottom(); y++) {
        unsigned char *sp = tmp.PtrToFirstPixelInRow(y);
        unsigned char *dp = src.PtrToFirstPixelInRow(y);
        dp[0] = (sp[1] != sp[0]) ? 0 : sp[0];
        for (int x = 1, width = src.Width(); x < width-1; x++) {
            int val = sp[x];
            dp[x] = (sp[x-1] != val || sp[x+1] != val) ? 0 : val;
        }
        dp[x] = (sp[x] != sp[x-1]) ? 0 : sp[x];
    }
}

void CVisRegionExtract::ErodeLabels(int n_erode)
{
    // Erode label image to disconnect thin necks
    CVisByteImage tmp(m_label.Shape());
    for (int i = 0; i < n_erode; i++) {
        VisErodeLabel(m_label, tmp);
    }
#ifdef VIS_WRITE_DEBUG_FILES
    m_label.WriteFile("tmp_triS.pgm");
#endif // VIS_WRITE_DEBUG_FILES
}

void CVisRegionExtract::ExtractRuns(int y)
{
    // Extract runs above (below) threshold
    CVisRunList& rl = m_runs[y];
    rl.clear();
    int width = m_label.Width();
    unsigned char *p = m_label.PtrToFirstPixelInRow(y+m_label.Top());
    for (int x = 0; x < width; x++) {
        if (p[x] == 0)
            continue;

        // Extract the run
        CVisRegionRun run;
        run.x = x, run.y = y, run.val = p[x];
        for (x++; x < width && p[x] == run.val; x++);
        run.n = x - run.x;
        rl.push_back(run);
    }
}

void CVisRegionExtract::MergeRun(CVisRegionRun& run, CVisRunList& prev)
{
    // Find all runs on previous line overlapping this one
    CVisIntV connected;
    for (int i = 0, n = prev.size(); i < n; i++) {
        if (run.Overlap(prev[i]))
            connected.push_back(prev[i].id);
    }
    n = connected.size();

    // No overlapping runs, start a new region
    if (n == 0) {
        run.id = m_region_list.size();
        CVisConnectedRegion r;
        r.Reset(run.id);
        r.val = run.val;
        r += run;
        m_region_list.push_back(r);
        return;
    }

    // Sort by region id, merge with first region found
    std::sort(connected.begin(), connected.end());
    run.id = connected[0];
    CVisConnectedRegion& r = m_region_list[run.id];
    r += run;

    // Merge all other connected regions...
    for (i = 1; i < n; i++) {
        int id = connected[i];
        if (id != connected[i-1]) {   // new region
            CVisConnectedRegion& r2 = m_region_list[id];
            r += r2;
            r2.Reset(r2.id, run.id);    // back pointer to region

            // Renumber appropriate runs
            for (int j = 0, m = prev.size(); j < m; j++)
                if (prev[j].id == id)
                    prev[j].id = run.id;
        }
    }
}

void CVisRegionExtract::RenumberRegions()
{
    // Renumber the regions to be contiguous
    CVisRegionList new_list;
    for (int i = 0, n = m_region_list.size(); i < n; i++) {
        CVisConnectedRegion& r = m_region_list[i];
        if (r.equiv_id == -1) {
            // Valid region, push onto new list
            r.equiv_id = new_list.size();
            new_list.push_back(r);
        } else {
            // Change equiv ptr to new id
            r.equiv_id = m_region_list[r.equiv_id].equiv_id;
        }
    }

    // Renumber all runs to have new numbers
    for (int y = 0; y < m_runs.size(); y++) {
        CVisRunList& rl = m_runs[y];
        for (int r = 0; r < rl.size(); r++) {
            CVisRegionRun& run = rl[r];
            run.id = m_region_list[run.id].equiv_id;
        }
    }

    // Replace region list with new list
    m_region_list = new_list;
}

void CVisRegionExtract::ExtractRegions()
{
    // Extract regions above (below) a threshold
    int height = m_label.Height();
    m_runs.resize(height);
    CVisRunList empty;  // for first line

    // Scan through lines, looking for runs and merging them
    // into regions
    for (int y = 0; y < height; y++) {

        // Extract the new set of runs
        ExtractRuns(y);

        // Merge the new runs with the previous ones
        CVisRunList& next = m_runs[y];
        CVisRunList& prev = (y > 0) ? m_runs[y-1] : empty;
        for (int r = 0; r < next.size(); r++)
            MergeRun(next[r], prev);
    }

    // Compute centroid of each region
    for (int i = 0; i < m_region_list.size(); i++)
        m_region_list[i].Fit();

    // Renumber the regions to be contiguous
    RenumberRegions();
}


int CVisRegionExtract::ExtractCircularRegions(double k1, double k2,
		int nThresholdPrev, int nPercentMinBlobSize, double *pdblXUp, double *pdblYUp)
{
	CVisHistogram histogramT;
	VisHistogramFromImage(histogramT, m_input);
	histogramT.Lock();
	BYTE bThreshold = histogramT.IThresholdOtsu();
	CVisFnOpPixelLEVal<BYTE> fnop(bThreshold);
	CVisPixRunList pixrunlist = PixRunListMakeImage(m_input, fnop);
	CVisBlobTreeInternal *pblobtree = new CVisBlobTreeInternal;
	
	// Make blob tree.  "false" means 4-connected foreground.
	pblobtree->MakeTreeFromPixRunList(pixrunlist, false);

	// Find the size of the largest top-level blob.
	// LATER:  To make this more robust, try using the Kth largest
	// blob (say for K == 4).
	// UNDONE:  Modify the code below to work with regions containing
	// more than one hole (with smaller holes encoding position information).
	int nBlobSizeHigh = 0;
	CVisBlobInternal *pblob = pblobtree->PBlobFirst();
	for (; pblob != 0; pblob = pblob->NextSibling())
	{
		int nAreaBlob = pblob->Area();
		// We don't have to be too exact here, so we don't add the area of
		// holes in marker blobs.
//		if (pblob->FirstChild() != 0)
//			nAreaBlob += pblob->FirstChild()->Area();
		if ((nAreaBlob > nBlobSizeHigh)
				&& (pblob->RectBounding().left > 0)
				&& (pblob->RectBounding().top > 0)
				&& (pblob->RectBounding().right < m_input.Right())
				&& (pblob->RectBounding().bottom < m_input.Bottom()))
		{
			nBlobSizeHigh = nAreaBlob;
		}
	}

	if (m_fComputeImageThresholded)
	{
		BYTE bForeground = 255;
		m_imageThresholded.Allocate(m_input.Shape());
		SetFgBlobsInImage(*pblobtree, m_imageThresholded, bForeground);
	}


#ifdef OLD_CODE
	// Find the top-level blobs that we want to use.
	// Also look for holes in blobs that could give orientation information.
	double dxUp = 0;
	double dyUp = 0;
	m_region_list.clear();
	int nBlobSizeLow = nPercentMinBlobSize * nBlobSizeHigh / 100;
	pblob = pblobtree->PBlobFirst();
	for (; pblob != 0; pblob = pblob->NextSibling())
	{
		int nAreaBlob = pblob->Area();
		if (pblob->FirstChild() != 0)
			nAreaBlob += pblob->FirstChild()->Area();
		if ((nAreaBlob > nBlobSizeLow)
				&& (pblob->RectBounding().left > 0)
				&& (pblob->RectBounding().top > 0)
				&& (pblob->RectBounding().right < m_input.Right())
				&& (pblob->RectBounding().bottom < m_input.Bottom()))
		{
			m_region_list.push_back(CVisConnectedRegion());
			CVisConnectedRegion& refregion = m_region_list.back();

			if (pblob->FirstChild() != 0)
			{
				int nWeight = min(pblob->Area(), pblob->FirstChild()->Area());
				dxUp += nWeight * (pblob->FirstChild()->DblXCentroid()
						- pblob->DblXCentroid());
				dyUp += nWeight * (pblob->FirstChild()->DblYCentroid()
						- pblob->DblYCentroid());

				refregion.x = ((double) (pblob->SumX()
							+ pblob->FirstChild()->SumX()))
						/ ((double) nAreaBlob);
				refregion.y = ((double) (pblob->SumY()
							+ pblob->FirstChild()->SumY()))
						/ ((double) nAreaBlob);
				refregion.fMarker = true;
			}
			else
			{
				refregion.x = pblob->DblXCentroid();
				refregion.y = pblob->DblYCentroid();
				refregion.fMarker = false;
			}


			refregion.val = 1;  // Is this okay?
			refregion.n = nAreaBlob;
			refregion.id = m_region_list.size();
			refregion.equiv_id = refregion.id;  // Is this okay?
			refregion.cedgel = 0;
//			refregion.fMarker = false;
			refregion.fHasChild = (pblob->FirstChild() != 0);
		}
	}
#endif // OLD_CODE

	// Setup variables used to correct radial distortion.
	bool fCorrectRadialDistortion = ((k1 != 0.0) || (k2 != 0.0));
    double dblHalfWidthInv  = 2.0 / m_input.Width();
    double x0 = 0.5 * (m_input.Left() + m_input.Right() - 1);
    double y0 = 0.5 * (m_input.Top() + m_input.Bottom() - 1);

	// Find the top-level blobs that we want to use.
	// Also look for holes in blobs that could give orientation information.
	double dxUp = 0;
	double dyUp = 0;
	m_region_list.clear();
	m_border_region_list.clear();
	int nBlobSizeLow = nPercentMinBlobSize * nBlobSizeHigh / 100;
	pblob = pblobtree->PBlobFirst();
	for (; pblob != 0; pblob = pblob->NextSibling())
	{
		int nAreaBlob = pblob->Area();
		double dblSumX = pblob->SumX();
		double dblSumY = pblob->SumY();

		CVisBlobInternal *pblobChildLargest = pblob->FirstChild();
		CVisBlobInternal *pblobChildSecondLargest = 0;
		CVisBlobInternal *pblobChild = pblob->FirstChild();
		for (; pblobChild != 0; pblobChild = pblobChild->NextSibling())
		{
			nAreaBlob += pblobChild->Area();
			dblSumX += pblobChild->SumX();
			dblSumY += pblobChild->SumY();
			if (pblobChild->Area() > pblobChildLargest->Area())
			{
				if ((pblobChildSecondLargest == 0)
						|| (pblobChildLargest->Area()
									> pblobChildSecondLargest->Area()))
				{
					pblobChildSecondLargest = pblobChildLargest;
				}
				pblobChildLargest = pblobChild;
			}
			else if ((pblobChild != pblobChildLargest)
					&& ((pblobChildSecondLargest == 0)
						|| (pblobChild->Area()
									> pblobChildSecondLargest->Area())))
			{
				pblobChildSecondLargest = pblobChild;
			}
		}

		CVisConnectedRegion *pregionT = 0;
		bool fBorderRegion = ((pblob->RectBounding().left == 0)
			|| (pblob->RectBounding().top == 0)
			|| (pblob->RectBounding().right == m_input.Right())
			|| (pblob->RectBounding().bottom == m_input.Bottom()));
		if (fBorderRegion)
		{
			m_border_region_list.push_back(CVisConnectedRegion());
			pregionT = &(m_border_region_list.back());
		}
		else if (nAreaBlob > nBlobSizeLow)
		{
			m_region_list.push_back(CVisConnectedRegion());
			pregionT = &(m_region_list.back());
		}

		if (pregionT != 0)
		{
			CVisConnectedRegion& refregion = *pregionT;

			refregion.x = dblSumX / ((double) nAreaBlob);
			refregion.y = dblSumY / ((double) nAreaBlob);
			refregion.n = nAreaBlob;
			refregion.nXPosition = 0;
			refregion.nYPosition = 0;

			// Remember the uncorrected position for use with the bounding-box
			// comparisons below.
			double dblXUncorrected = refregion.x;
			double dblYUncorrected = refregion.y;

			if (fCorrectRadialDistortion)
			{
				double xd = (refregion.x - x0) * dblHalfWidthInv, xd2 = xd * xd;
				double yd = (refregion.y - y0) * dblHalfWidthInv, yd2 = yd * yd;
				double r2 = xd2 + yd2;
				double s  = 1.0 + k1 * r2 + k2 * r2 * r2;
				refregion.x = x0 + s * (refregion.x - x0);
				refregion.y = y0 + s * (refregion.y - y0);
			}

			// Special case:  The hole in the center is circular and may appear to
			// be the largest hole.  It's not the largest hole.  We try to fix this
			// problem here by using the second-largest hole instead of the center
			// hole.
			if ((pblobChildSecondLargest != 0)
					&& (pblobChildLargest->RectBounding().left <= dblXUncorrected)
					&& (pblobChildLargest->RectBounding().right >= dblXUncorrected)
					&& (pblobChildLargest->RectBounding().top <= dblYUncorrected)
					&& (pblobChildLargest->RectBounding().bottom >= dblYUncorrected))
			{
				pblobChildLargest = pblobChildSecondLargest;
			}

			if (pblobChildLargest != 0)
			{
				refregion.xChild = pblobChildLargest->DblXCentroid();
				refregion.yChild = pblobChildLargest->DblYCentroid();

				if (fCorrectRadialDistortion)
				{
					double xd = (refregion.xChild - x0) * dblHalfWidthInv, xd2 = xd * xd;
					double yd = (refregion.yChild - y0) * dblHalfWidthInv, yd2 = yd * yd;
					double r2 = xd2 + yd2;
					double s  = 1.0 + k1 * r2 + k2 * r2 * r2;
					refregion.xChild = x0 + s * (refregion.xChild - x0);
					refregion.yChild = y0 + s * (refregion.yChild - y0);
				}

				double dblDxBlobUp = refregion.xChild - refregion.x;
				double dblDyBlobUp = refregion.yChild - refregion.y;
//				double dblXDownBlob = dblXUncorrected - dblDxBlobUp;
//				double dblYDownBlob = dblYUncorrected - dblDyBlobUp;
				double dblMagSquaredUp = dblDxBlobUp * dblDxBlobUp
						+ dblDyBlobUp * dblDyBlobUp;

				if (!fBorderRegion)
				{
					int nWeight = pblobChildLargest->Area();
					dxUp += nWeight * dblDxBlobUp;
					dyUp += nWeight * dblDyBlobUp;
				}

				// Look at holes to find position information.
				double dblXReference = refregion.x;
				double dblYReference = refregion.y;
				for (pblobChild = pblob->FirstChild(); pblobChild != 0;
						pblobChild = pblobChild->NextSibling())
				{
					if (pblobChild == pblobChildLargest)
						continue;

					double dblXChild = pblobChild->DblXCentroid();
					double dblYChild = pblobChild->DblYCentroid();
					if (fCorrectRadialDistortion)
					{
						double xd = (dblXChild - x0) * dblHalfWidthInv, xd2 = xd * xd;
						double yd = (dblYChild - y0) * dblHalfWidthInv, yd2 = yd * yd;
						double r2 = xd2 + yd2;
						double s  = 1.0 + k1 * r2 + k2 * r2 * r2;
						dblXChild = x0 + s * (dblXChild - x0);
						dblYChild = y0 + s * (dblYChild - y0);
					}
					double dblDxChild = dblXChild - dblXReference;
					double dblDyChild = dblYChild - dblYReference;
					double dblMagSquaredChild = dblDxChild * dblDxChild
							+ dblDyChild * dblDyChild;
					double dblChildDotUp = dblDxChild * dblDxBlobUp
							+ dblDyChild * dblDyBlobUp;

					// UNDONE:  Replace the bounding-box conditions with vector conditions.
					if ((pblobChild->RectBounding().left <= dblXUncorrected)
							&& (pblobChild->RectBounding().right >= dblXUncorrected)
							&& (pblobChild->RectBounding().top <= dblYUncorrected)
							&& (pblobChild->RectBounding().bottom >= dblYUncorrected))
					{
						// This child is a hole in the center of the blob.
						refregion.nXPosition |= 2;
					}
//					else if ((pblobChild->RectBounding().left <= dblXDownBlob)
//							&& (pblobChild->RectBounding().right >= dblXDownBlob)
//							&& (pblobChild->RectBounding().top <= dblYDownBlob)
//							&& (pblobChild->RectBounding().bottom >= dblYDownBlob))
					else if ((dblChildDotUp * dblChildDotUp
							/ (dblMagSquaredUp * dblMagSquaredChild)) > 0.8)
					{
						// This child is a hole in the bottom of the blob.
						refregion.nYPosition |= 2;
					}
					else
					{
						// Use dot and cross products to find the position of
						// this child relative to the position of the largest
						// child.
						bool fTopRow = (dblChildDotUp > 0.0);
						bool fRightSide = (dblDxChild * dblDyBlobUp
								< dblDyChild * dblDxBlobUp);
						if (fTopRow)
						{
							if (fRightSide)
								refregion.nXPosition |= 1;
							else
								refregion.nXPosition |= 4;
						}
						else
						{
							if (fRightSide)
								refregion.nYPosition |= 1;
							else
								refregion.nYPosition |= 4;
						}
					}
				}
			}
			else
			{
				refregion.xChild = refregion.x;
				refregion.yChild = refregion.y;
			}

			refregion.val = 1;  // Is this okay?
			refregion.id = m_region_list.size();
			refregion.equiv_id = refregion.id;  // Is this okay?
			refregion.cedgel = 0;
			refregion.nCloseNeighbors = 0;

			refregion.fMarker = (pblobChildLargest != 0);
			refregion.fHasChild = (pblobChildLargest != 0);
		}
	}

	delete pblobtree;

	if (pdblXUp != 0)
		*pdblXUp = dxUp;
	if (pdblYUp != 0)
		*pdblYUp = dyUp;

	return bThreshold;
}

void CVisRegionExtract::CorrectRadialDistortion(double k1, double k2)
{
    // Correct for radial distortion

#ifdef USE_EDGELS_TO_CORRECT_RADIAL_DISTORTION

    double width  = m_input.Width();
    double height = m_input.Height();
    CVisRect shape  = m_input.Shape();
    float x0 = 0.5*(shape.left+shape.right-1);
    float y0 = 0.5*(shape.top+shape.bottom-1);
    for (int i = 0, n = m_region_list.size(); i < n; i++) {
        CVisConnectedRegion& r = m_region_list[i];
        CVisEdgel e;
        e.x = r.x, e.y = r.y;
        e.n_x = 1.0, e.n_y = 0.0;
        e = e.CorrectRadialDistortion(x0, y0, width, k1, k2);
        r.x = e.x, r.y = e.y;
    }
    for (i = 0, n = m_border_region_list.size(); i < n; i++) {
        CVisConnectedRegion& r = m_border_region_list[i];
        CVisEdgel e;
        e.x = r.x, e.y = r.y;
        e.n_x = 1.0, e.n_y = 0.0;
        e = e.CorrectRadialDistortion(x0, y0, width, k1, k2);
        r.x = e.x, r.y = e.y;
    }

#else // USE_EDGELS_TO_CORRECT_RADIAL_DISTORTION

	// This code uses the same algorithm as the code above, but
	// it doesn't do the extra normalization and angle calculations
	// that are needed with edgels representing lines.
    double dblHalfWidthInv  = 2.0 / m_input.Width();
    double x0 = 0.5 * (m_input.Left() + m_input.Right() - 1);
    double y0 = 0.5 * (m_input.Top() + m_input.Bottom() - 1);
    for (int i = 0, n = m_region_list.size(); i < n; i++)
	{
        CVisConnectedRegion& r = m_region_list[i];
		double xd = (r.x - x0) * dblHalfWidthInv, xd2 = xd * xd;
		double yd = (r.y - y0) * dblHalfWidthInv, yd2 = yd * yd;
		double r2 = xd2 + yd2;
		double s  = 1.0 + k1 * r2 + k2 * r2 * r2;
		r.x = x0 + s * (r.x - x0);
		r.y = y0 + s * (r.y - y0);
    }
    for (i = 0, n = m_border_region_list.size(); i < n; i++)
	{
        CVisConnectedRegion& r = m_border_region_list[i];
		double xd = (r.x - x0) * dblHalfWidthInv, xd2 = xd * xd;
		double yd = (r.y - y0) * dblHalfWidthInv, yd2 = yd * yd;
		double r2 = xd2 + yd2;
		double s  = 1.0 + k1 * r2 + k2 * r2 * r2;
		r.x = x0 + s * (r.x - x0);
		r.y = y0 + s * (r.y - y0);
    }

#endif // USE_EDGELS_TO_CORRECT_RADIAL_DISTORTION
}

void CVisRegionExtract::ComputeLabels(int scale, int bias)
{
    // Fill in an image with (scaled) region id's
    m_label.ClearPixels();
    for (int y = 0; y < m_runs.size(); y++) {
        CVisRunList& rl = m_runs[y];
        for (int r = 0; r < rl.size(); r++) {
            CVisRegionRun& run = rl[r];
            unsigned char *p = m_label.PtrToFirstPixelInRow(y+m_label.Top());
            int val = (scale * run.id) % (256 - bias) + bias;
            memset(&p[run.x], val, run.n);
        }
    }
}


CVisEdgelList CVisRegionExtract::ExtractGrid(double gap, int match_other)
{
    // Compute a "grid" from region centers
    CVisEdgelList el;
#ifdef VIS_WRITE_DEBUG_FILES
    ComputeLabels(13, 128);
    LabelImage().WriteFile("tmp_labels.pgm");
#endif // VIS_WRITE_DEBUG_FILES

	double dblGapPlusOne = 1.0 + gap;
	double dblGapPlusOne2 = dblGapPlusOne * dblGapPlusOne;
	double dblGapPlusOne2Halved = dblGapPlusOne2 * 0.5;

#ifdef OLD_CODE
    // For each blob, find its matching neighbors (within size limit)
    for (int i = 0, n = m_region_list.size(); i < n; i++) {
        CVisConnectedRegion& r = m_region_list[i];
        for (int j = i+1; j < n; j++) {
            CVisConnectedRegion& s = m_region_list[j];
            if (match_other && s.val == r.val)
                continue;
			// If one blob is more than twice as large as the other, use the
			// minimum of the two blob areas to determine the minimum distance
			// for lines between the wto blobs.
			double max_dist2;
			if (((s.n << 1) < r.n) || ((r.n << 1) < s.n))
				max_dist2 = dblGapPlusOne2 * min(r.n, s.n);
			else
				max_dist2 = dblGapPlusOne2Halved * (r.n + s.n);
            double dx = s.x - r.x, dy = s.y - r.y;
            double dist2 = dx * dx + dy * dy;
            if (dist2 <= max_dist2) {
                CVisEdgel e;
                e.x   = 0.5 * (r.x + s.x);
                e.y   = 0.5 * (r.y + s.y);
                e.n_x = s.x - r.x;
                e.n_y = s.y - r.y;
                e.strength = 1;
                e.line_length = e.sigma = 0.0;
                e.NormalizeN();
                e.ComputeTheta();

				// If one of the regions is around twice (use 5/3 to be
				// safe) the size of its neighbors, it is a marker region.
				// Tell the region that it is a marker region here.
				// (We don't need to tell the edgel about the marker region.)
//				r.fMarker = ((r.fMarker) && ((s.n << 1) < r.n));
//				s.fMarker = ((s.fMarker) && ((r.n << 1) < s.n));
				r.fMarker = ((r.fMarker) && ((s.n * 5) < (r.n * 3)));
				s.fMarker = ((s.fMarker) && ((r.n * 5) < (s.n * 3)));
				++ r.cedgel;
				++ s.cedgel;

                el.push_back(e);
            }
        }
    }
#endif // OLD_CODE

    // For each blob, find its matching neighbors (within size limit)
    for (int i = 0, n = m_region_list.size(); i < n; i++)
	{
        CVisConnectedRegion& r = m_region_list[i];
        for (int j = i+1; j < n; j++)
		{
            CVisConnectedRegion& s = m_region_list[j];
            if (match_other && s.val == r.val)
                continue;
			// If one blob is more than twice as large as the other, use the
			// minimum of the two blob areas to determine the minimum distance
			// for lines between the wto blobs.
			double max_dist2;
			if (((s.n << 1) < r.n) || ((r.n << 1) < s.n))
				max_dist2 = dblGapPlusOne2 * min(r.n, s.n);
			else
				max_dist2 = dblGapPlusOne2Halved * (r.n + s.n);
            double dx = s.x - r.x, dy = s.y - r.y;
            double dist2 = dx * dx + dy * dy;
            if (dist2 <= max_dist2)
			{
                r.AddCloseNeighbor(j, false, dist2);
                s.AddCloseNeighbor(i, false, dist2);
            }
        }

		// Also consider neighboring border regions
		for (j = 0; j < m_border_region_list.size(); j++)
		{
            CVisConnectedRegion& s = m_border_region_list[j];
			// Don't use border blob area when finding close lines.
			double max_dist2 = dblGapPlusOne2 * r.n;
            double dx = s.x - r.x, dy = s.y - r.y;
            double dist2 = dx * dx + dy * dy;
            if (dist2 <= max_dist2)
			{
                r.AddCloseNeighbor(j, true, dist2);
            }
		}
    }

    // For each blob, find its matching neighbors (within size limit)
    for (i = 0, n = m_region_list.size(); i < n; i++)
	{
        CVisConnectedRegion& r = m_region_list[i];
        for (int j = 0; j < r.nCloseNeighbors; j++)
		{
			if (r.fBorderNeighbor[j])
				continue;
            CVisConnectedRegion& s = m_region_list[r.iCloseNeighbor[j]];

			// We should only add the edgel if r is one of s's
			// closest neighbors.
			assert(s.nCloseNeighbors > 0);
			if (s.dblSquaredDistToNeighbor[s.nCloseNeighbors - 1]
					< r.dblSquaredDistToNeighbor[j])
			{
				continue;
			}

            CVisEdgel e;
            e.x   = 0.5 * (r.x + s.x);
            e.y   = 0.5 * (r.y + s.y);
            e.n_x = s.x - r.x;
            e.n_y = s.y - r.y;
            e.strength = 1;
            e.line_length = e.sigma = 0.0;
            e.NormalizeN();
            e.ComputeTheta();

			// If one of the regions is around twice (use 5/3 to be
			// safe) the size of its neighbors, it is a marker region.
			// Tell the region that it is a marker region here.
			// (We don't need to tell the edgel about the marker region.)
//				r.fMarker = ((r.fMarker) && ((s.n << 1) < r.n));
//				s.fMarker = ((s.fMarker) && ((r.n << 1) < s.n));
			r.fMarker = ((r.fMarker) && ((s.n * 5) < (r.n * 3)));
			s.fMarker = ((s.fMarker) && ((r.n * 5) < (s.n * 3)));
			++ r.cedgel;
			++ s.cedgel;

            el.push_back(e);
        }
    }

    return el;
}


//
//  The CVisGridExtract is used to fit a grid to a checkerboard image
//

CVisGridExtract::CVisGridExtract()
{
    // User-modifiable parameters for radial lens distortion correction
    k1 = 0.0;                   // quadratic correction component (scaled by f)
    k2 = 0.0;                   // quartic   correction component (scaled by f)
    n_steps = 0;                // radial lens distortion correction steps
    step_size = 0.5;            // radial lens distortion correction factor

    // User-modifiable parameters for edge extraction
    n_level_diff = 5;           // number of levels in difference of Gaussian
    n_blur = 2;                 // number of blur steps at finest level
    margin = 4;                 // number of ignored pixels around edge
    min_strength = 16.0;        // minimum strength of edgel

    // User-modifiable parameters for region extraction
    n_erode = 2;                // number of erosion steps before blob extraction
    gap = 0.8;                  // maximum extra distance between blob centers

    // User-modifiable parameters for line extraction
    pixel_spacing = 8;          // spacing between lines in different bins
    angle_spacing = 15;         // amount of orientation in each bin
    n_nghbrs = 1;               // half-size of accumulation neighborhood (1 => 3x3)
    collapse_opposites = true;  // ignore polarity of the line segments
    min_length = 20.0;          // minimum length of line

    // User-modifiable parameters for vanishing points
    f = 100.0f;                 // focal length (in pixels)
    n_bins = 64;                // number of bins on side (in van. pt. space)
    min_dot = 0.8f;             // cos of max angle between paired lines
    min_dist = 8.0f;            // lines must be at least this far apart
    vp_nghbrs = 1;              // half-size of accumulation neighborhood (1 => 3x3)
    min_count = 6;              // minimum number of intersections
	
	// Settings used when extracting circular regions
	m_fFindCircles = true; // false;
	m_nThresholdNext = - 1;
	m_nThresholdPrev = - 1;
	m_nPercentMinBlobSize = 4;

	// No pose estimate yet.
	m_fHavePose = false;
	memset(&m_pose[0][0], 0, sizeof(CVisTransform4x4));

	m_dblXUp = 0.0;
	m_dblYUp = 0.0;

	m_dxGridSpacing = 0.0;
	m_dyGridSpacing = 0.0;
	m_fltPhaseX = 0.0;
	m_fltPhaseY = 0.0;
	m_cPeriodPhaseRange = 1;

	m_fInterceptsFromOrigCenters = true;
	m_fInterceptsFromTCenters = false;
	m_fUsePositionsForInterceptOffsets = false;
//	m_fAverageVanishingPoints = true;
	m_fAverageVanishingPoints = false;
	m_fComputeImageThresholded = false;
}

void CVisGridExtract::SetImage(CVisByteImage& image)
{
    m_input = image;
    m_edge_extract.SetImage(image);
    m_region_extract.SetImage(image);
    m_region_extract.SetFComputeImageThresholded(m_fComputeImageThresholded);
}

// Helper function.  Return the phase of x when spacing is dx.
float CVisGridExtract::PhaseFromXAndDx(float x, float dx)
{
	float fltPhase;
	
	if (dx != 0.0)
	{
		// fltPhase = (float) ((x - ((int) (x / dx + 0.5)) * dx) / dx);
		fltPhase = (float) (x / dx - floor(x / dx + 0.5));
	}
	else
	{
		fltPhase = 0.0;
	}

	assert((- 0.5 <= fltPhase) && (fltPhase < 0.5));

	return fltPhase;
}

// Helper function.  Sort a vector and return the median.
float CVisGridExtract::MedianAfterSorting(std::vector<float>& refvector)
{
	float fltMedian;

	std::sort(refvector.begin(), refvector.end());
	if (refvector.size() == 0)
	{
		fltMedian = 0;
	}
	else if ((refvector.size() & 1) != 0)
	{
		fltMedian = refvector[refvector.size() / 2];
	}
	else
	{
		fltMedian = (refvector[refvector.size() / 2]
				+ refvector[refvector.size() / 2 - 1]) / 2.0;
	}

	return fltMedian;
}

// Helper function.  Find an average phase vector
float CVisGridExtract::AveragePhase(std::vector<float>& refvector,
		float fltMin, float fltMax)
{
	assert(refvector.size() > 0);

	// Find the vector sum of the phases (mapped to the unit circle).
	float fltRange = fltMax - fltMin;
	float fltHalfRange = (float) (fltRange / 2.0);
	double dblConversion = M_PI / fltHalfRange;
	double dblXSum = 0.0;
	double dblYSum = 0.0;
	double dblAngleT;
	for (int i = 0; i < refvector.size(); ++i)
	{
		dblAngleT = (refvector[i] - fltMin) * dblConversion;
		dblXSum += cos(dblAngleT);
		dblYSum += sin(dblAngleT);
	}

	if ((dblXSum == 0.0) && (dblYSum == 0.0))
	{
		// Rare case.  No good average.
		return 0.0;
	}

	dblAngleT = atan2(dblXSum, dblYSum);
	float fltPhaseAvg = (float) (dblAngleT / dblConversion + fltMin);
	if (fltPhaseAvg < fltMin)
		fltPhaseAvg += fltRange;

// UNDONE: Just return the average phase.  (less noisy?)
//return fltPhaseAvg;

	// For vectors close to the average phase vector, make a list of
	// their differences from the average phase vector.
	float fltCloseLow = - 0.2 / fltRange;
	float fltCloseHigh = 0.2 / fltRange;
	float fltDiffSum = 0.0;
	std::vector<float> vectorfltRelPhase;
	for (i = 0; i < refvector.size(); ++i)
	{
		float fltDiffT = (float) (refvector[i] - fltPhaseAvg);
		if (fltDiffT > fltHalfRange)
			fltDiffT -= fltRange;
		else if (fltDiffT < - fltHalfRange)
			fltDiffT += fltRange;
		if ((fltDiffT >= fltCloseLow) && (fltDiffT <= fltCloseHigh))
		{
			vectorfltRelPhase.push_back(fltDiffT);
			fltDiffSum += fltDiffT;
		}
	}

	// If we found vectors close to the average phase vector, use their
	// median as the average phase that we return.
	if (!vectorfltRelPhase.empty())
	{
//		fltPhaseAvg += MedianAfterSorting(vectorfltRelPhase);
		fltPhaseAvg += fltDiffSum / vectorfltRelPhase.size();
		if (fltPhaseAvg >= fltMax)
			fltPhaseAvg -= fltRange;
		else if (fltPhaseAvg < fltMin)
			fltPhaseAvg += fltRange;
	}

	return fltPhaseAvg;
}

CVisGridExtract::SIntercept::SIntercept(float fltPosition)
  : position(fltPosition),
	cPeriodOffset(0),
	cMarkers(0),
	fReference(false)
{
}

int CVisGridExtract::CPeriodMarkedCalcInterceptOffsets(
		std::vector<CVisGridExtract::SIntercept>& refvector, float fltPeriod)
{
	// Given sorted intercepts and grid spacing, find intercept offsets
	// from marked intercepts and return the number of periods between
	// marked intercepts.  If m_fUsePositionsForInterceptOffsets is true,
	// offsets will be calculated by dividing the intercept positions by
	// the grid spacing.  Otherwise, offsets will be calculated by the
	// number of intercepts to the previous or next marked intercept.
	int i;
	int cPeriodT;
	int cPeriodMarked = 0;
	int iFirstReference = - 1;
	int iPrevReference = - 1;
	for (i = 0; i < refvector.size(); ++i)
	{
		// assume that the vector is sorted.
		assert((i == 0) || (refvector[i].position >= refvector[i - 1].position));

		if (refvector[i].fReference)
		{
			iPrevReference = i;
			if (iFirstReference < 0)
				iFirstReference = i;
		}
		else if (iPrevReference >= 0)
		{
			if (m_fUsePositionsForInterceptOffsets)
			{
				cPeriodT = floor((refvector[i].position
							- refvector[iPrevReference].position)
						/ fltPeriod + 0.5);
			}
			else
			{
				cPeriodT = i - iPrevReference;
			}
			refvector[i].cPeriodOffset = - cPeriodT;

			if ((refvector[i].cMarkers > 0) && (cPeriodT > 0)
					&& ((cPeriodMarked == 0) || (cPeriodT < cPeriodMarked)))
			{
				cPeriodMarked = cPeriodT;
			}
		}
	}
	for (i = 0; i < iFirstReference; ++i)
	{
		if (m_fUsePositionsForInterceptOffsets)
		{
			cPeriodT = floor((refvector[iFirstReference].position
						- refvector[i].position)
					/ fltPeriod + 0.5);
		}
		else
		{
			cPeriodT = iFirstReference - i;
		}
		refvector[i].cPeriodOffset = cPeriodT;

		if ((refvector[i].cMarkers > 0) && (cPeriodT > 0)
				&& ((cPeriodMarked == 0) || (cPeriodT < cPeriodMarked)))
		{
			cPeriodMarked = cPeriodT;
		}
	}

	return cPeriodMarked;
}

float CVisGridExtract::PhaseFromIntercepts(
		std::vector<CVisGridExtract::SIntercept>& refvector, float fltPeriod,
		int cPeriodMarked)
{
	// Given sorted intercepts, grid spacing, and number of intercepts
	// between marked intercepts, find the current phase (between
	// - cPeriodMarked / 2.0 and cPeriodMarked / 2.0) by looking at the
	// intercept(s) closest to 0.
	float fltPhase;

	// Special cases.
	if (refvector.size() < 2)
	{
		if (refvector.size() == 0)
			fltPhase = 0.0f;
		else
			fltPhase = - refvector[0].position / fltPeriod;
	}
	else
	{
		int i;
		int iFirstNonNegative = - 1;
		for (i = 0; i < refvector.size(); ++i)
		{
			// assume that the vector is sorted.
			assert((i == 0) || (refvector[i].position >= refvector[i - 1].position));

			if (refvector[i].position >= 0.0f)
			{
				iFirstNonNegative = i;
				break;
			}
		}

		if (iFirstNonNegative > 0)
		{
			float fltSpacing = refvector[iFirstNonNegative].position
					- refvector[iFirstNonNegative - 1].position;
			fltPhase = - refvector[iFirstNonNegative - 1].position / fltSpacing
					- refvector[iFirstNonNegative - 1].cPeriodOffset;
		}
		else if (iFirstNonNegative == 0)
		{
			// All are >= 0.  Use first intercept to find phase.
			fltPhase = - refvector[0].position / fltPeriod
					- refvector[0].cPeriodOffset;
		}
		else
		{
			// All are < 0.  Use last to find phase.
			fltPhase = - refvector.back().position / fltPeriod
					- refvector.back().cPeriodOffset;
		}
	}

	// Normalize fltPhase;
	int cPeriodT = (int) floor(fltPhase / cPeriodMarked + 0.5);
	fltPhase -= cPeriodT  * cPeriodMarked;
	assert(- cPeriodMarked / 2.0 <= fltPhase);
	assert(fltPhase < cPeriodMarked / 2.0);

	return fltPhase;
}

void CVisGridExtract::FindMarkersOnLines(void)
{
	// Identify lines on marker regions.
	CVisEdgelList& edgelistLines = LineList();
	CVisRegionList& refregionlist = m_region_extract.RegionList();
	for (int iRegion = 0; iRegion < refregionlist.size(); ++iRegion)
	{
		CVisConnectedRegion& refregion = refregionlist[iRegion];
		if ((refregion.cedgel > 0) && (refregion.fMarker))
		{
			refregion.fltHalfSqrtArea = (float) sqrt((double) refregion.n) / 2.0;
			for (int iLine = 0; iLine < edgelistLines.size(); iLine++)
			{
				CVisEdgel& refedgelLine = edgelistLines[iLine];
				if (refedgelLine.ContainsPoint(refregion.x, refregion.y,
						refregion.fltHalfSqrtArea))
				{
					++ refedgelLine.cMarkers;
				}
			}
		}
	}
}

void CVisGridExtract::FindIntercepts(
		std::vector<CVisGridExtract::SIntercept>& refvectorX,
		std::vector<CVisGridExtract::SIntercept>& refvectorY)
{
	float xC = 0.5 * (m_input.Left() + m_input.Right() - 1);
	float yC = 0.5 * (m_input.Top() + m_input.Bottom() - 1);

	CVisTransform4x4 transformTranslate;
	transformTranslate[0][3] = - xC;
	transformTranslate[1][3] = - yC;

	CVisTransform4x4 transformT = GridPose().Transposed();
	transformT[0][2] *= f;
	transformT[1][2] *= f;
	transformT[2][0] /= f;
	transformT[2][1] /= f;
	transformT *= transformTranslate;

//	const double dblCloseDot = sqrt(3.0) / 2.0;
	const double dblCloseDot = 0.91;

	// Find reference lines.
	// Use 1 as the high value to require at least one marker on marked lines.
	int cMarkersXHigh = 1;
	int cMarkersYHigh = 1;
	bool fHaveReferenceLines = false;
	CVisEdgelList& edgelistLines = LineList();
	if (!m_fUseEdgels)
	{
		int iLineHorizReference = - 1;
		int iLineVertReference = - 1;

		for (int iLine = 0; iLine < edgelistLines.size(); iLine++)
		{
			if ((edgelistLines[iLine].cMarkers >= cMarkersXHigh)
					|| (edgelistLines[iLine].cMarkers >= cMarkersYHigh))
			{
				CVisEdgel edgelT = transformT * edgelistLines[iLine];
				
				if ((edgelT.n_x > dblCloseDot) || (edgelT.n_x < - dblCloseDot))
				{
					// Could this be the horizontal reference line?
					if (edgelT.cMarkers > cMarkersYHigh)
					{
						cMarkersYHigh = edgelT.cMarkers;
						iLineHorizReference = iLine;
					}
					else if ((edgelT.cMarkers == cMarkersYHigh)
							&& ((iLineHorizReference < 0)
								|| ((edgelistLines[iLineHorizReference].x - xC)
										* (edgelistLines[iLineHorizReference].x - xC)
										+ (edgelistLines[iLineHorizReference].y - yC)
										* (edgelistLines[iLineHorizReference].y - yC)
									> (edgelistLines[iLine].x - xC)
										* (edgelistLines[iLine].x - xC)
										+ (edgelistLines[iLine].y - yC)
										* (edgelistLines[iLine].y - yC))))
					{
						iLineHorizReference = iLine;
					}
				}
				else if ((edgelT.n_y > dblCloseDot) || (edgelT.n_y < - dblCloseDot))
				{
					// Could this be the vertical reference line?
					if (edgelT.cMarkers > cMarkersXHigh)
					{
						cMarkersXHigh = edgelT.cMarkers;
						iLineVertReference = iLine;
					}
					else if ((edgelT.cMarkers == cMarkersXHigh)
							&& ((iLineVertReference < 0)
								|| ((edgelistLines[iLineVertReference].x - xC)
										* (edgelistLines[iLineVertReference].x - xC)
										+ (edgelistLines[iLineVertReference].y - yC)
										* (edgelistLines[iLineVertReference].y - yC)
									> (edgelistLines[iLine].x - xC)
										* (edgelistLines[iLine].x - xC)
										+ (edgelistLines[iLine].y - yC)
										* (edgelistLines[iLine].y - yC))))
					{
						iLineVertReference = iLine;
					}
				}
			}
		}
		if (iLineHorizReference >= 0)
			edgelistLines[iLineHorizReference].fReferenceLine = true;
		if (iLineVertReference >= 0)
			edgelistLines[iLineVertReference].fReferenceLine = true;

		fHaveReferenceLines = (iLineHorizReference >= 0)
				 && (iLineVertReference >= 0);
	}

	for (int iLine = 0; iLine < edgelistLines.size(); iLine++)
	{
		float fltIntercept = 0.0;
		CVisEdgel& refedgelLine = edgelistLines[iLine];
		CVisEdgel edgelT = transformT * refedgelLine;
		
		if ((edgelT.n_x > dblCloseDot) || (edgelT.n_x < - dblCloseDot))
		{
			// Look for y-intercept.
			if (m_fInterceptsFromOrigCenters)
			{
				CVisVector4 vectorT(refedgelLine.x, refedgelLine.y, 1, 1);
				vectorT = transformT * vectorT;
				if (vectorT[2] != 0)
				{
					fltIntercept = (float) (vectorT[1] / vectorT[2]);
				}
				else
				{
					// Unexpected case.  Just use center of transformed line.
					fltIntercept = edgelT.y;
				}
			}
			else if (m_fInterceptsFromTCenters)
			{
				fltIntercept = edgelT.y;
			}
			else
			{
				fltIntercept = (float)
					(edgelT.y - edgelT.x * edgelT.n_y / edgelT.n_x);
			}

			refvectorY.push_back(SIntercept(fltIntercept));
			refvectorY.back().cMarkers = refedgelLine.cMarkers;
			if (fHaveReferenceLines)
			{
				refvectorY.back().fReference
						= refedgelLine.fReferenceLine;
			}
			else
			{
				refvectorY.back().fReference
						= (refedgelLine.cMarkers == cMarkersYHigh);
			}
		}
		else if ((edgelT.n_y > dblCloseDot) || (edgelT.n_y < - dblCloseDot))
		{
			// Look for x-intercept.
			if (m_fInterceptsFromOrigCenters)
			{
				CVisVector4 vectorT(refedgelLine.x, refedgelLine.y, 1, 1);
				vectorT = transformT * vectorT;
				if (vectorT[2] != 0)
				{
					fltIntercept = (float) (vectorT[0] / vectorT[2]);
				}
				else
				{
					// Unexpected case.  Just use center of transformed line.
					fltIntercept = edgelT.x;
				}
			}
			else if (m_fInterceptsFromTCenters)
			{
				fltIntercept = edgelT.x;
			}
			else
			{
				fltIntercept = (float)
					(edgelT.x - edgelT.y * edgelT.n_x / edgelT.n_y);
			}

			refvectorX.push_back(SIntercept(fltIntercept));
			refvectorX.back().cMarkers = refedgelLine.cMarkers;
			if (fHaveReferenceLines)
			{
				refvectorX.back().fReference
						= refedgelLine.fReferenceLine;
			}
			else
			{
				refvectorX.back().fReference
						= (refedgelLine.cMarkers == cMarkersXHigh);
			}
		}
	}

	std::sort(refvectorX.begin(), refvectorX.end());
	std::sort(refvectorY.begin(), refvectorY.end());
}

void CVisGridExtract::FindGridSpacingAndPhase(void)
{
	// Given the line list, pose and region list if not using edgels,
	// find the grid spacing and phase.  If not using edgels, this
	// method will look for lines containing marked regions and use
	// such lines to get more accurate phase information.

	if (!m_fUseEdgels)
		FindMarkersOnLines();

	std::vector<CVisGridExtract::SIntercept> vectorinterceptX;
	std::vector<CVisGridExtract::SIntercept> vectorinterceptY;
	FindIntercepts(vectorinterceptX, vectorinterceptY);

	// Find grid spacing.
	std::vector<float> vectorfltT;
	for (int i = 1; i < vectorinterceptX.size(); ++i)
	{
		vectorfltT.push_back((float) vectorinterceptX[i].position
				- vectorinterceptX[i - 1].position);
	}
	if (!vectorfltT.empty())
		m_dxGridSpacing = MedianAfterSorting(vectorfltT);

	vectorfltT.clear();
	for (i = 1; i < vectorinterceptY.size(); ++i)
	{
		vectorfltT.push_back((float) vectorinterceptY[i].position
				- vectorinterceptY[i - 1].position);
	}
	if (!vectorfltT.empty())
		m_dyGridSpacing = MedianAfterSorting(vectorfltT);


	// Find the number of periods between marked lines.
	m_cPeriodPhaseRange = 1;
	if (!m_fUseEdgels)
	{
		int cPeriodMarkedX = CPeriodMarkedCalcInterceptOffsets(
				vectorinterceptX, m_dxGridSpacing);
		int cPeriodMarkedY = CPeriodMarkedCalcInterceptOffsets(
				vectorinterceptY, m_dyGridSpacing);

		if (cPeriodMarkedX == 0)
			if (cPeriodMarkedY == 0)
				m_cPeriodPhaseRange = 1;
			else
				m_cPeriodPhaseRange = cPeriodMarkedY;
		else
			if (cPeriodMarkedY == 0)
				m_cPeriodPhaseRange = cPeriodMarkedX;
			else
				m_cPeriodPhaseRange = (cPeriodMarkedX + cPeriodMarkedY) / 2;
	}

	// Find the phase.
	// UNDONE:  What should we do if the grid spacing is zero?
	if (m_dxGridSpacing != 0)
	{
		m_fltPhaseX = PhaseFromIntercepts(vectorinterceptX, m_dxGridSpacing,
				m_cPeriodPhaseRange);
	}
	if (m_dyGridSpacing != 0)
	{
		m_fltPhaseY = PhaseFromIntercepts(vectorinterceptY, m_dyGridSpacing,
				m_cPeriodPhaseRange);
	}


//New code to find the phase.
//UNDONE:  Clean up this function when we get the new code working.

	// Assume marker blobs are spaced four units apart.
// UNDONE:  Add an option to choose spacings of 3 or 4.
//	m_cPeriodPhaseRange = 4;
	m_cPeriodPhaseRange = 3;

	// Look for a reference marked.
	if ((m_dxGridSpacing == 0) && (m_dyGridSpacing != 0))
		m_dxGridSpacing = m_dyGridSpacing;
	else if ((m_dxGridSpacing != 0) && (m_dyGridSpacing == 0))
		m_dyGridSpacing = m_dxGridSpacing;

	float xC = 0.5 * (m_input.Left() + m_input.Right() - 1);
	float yC = 0.5 * (m_input.Top() + m_input.Bottom() - 1);

	CVisTransform4x4 transformTranslate;
	transformTranslate[0][3] = - xC;
	transformTranslate[1][3] = - yC;

	CVisTransform4x4 transformT = GridPose().Transposed();
	transformT[0][2] *= f;
	transformT[1][2] *= f;
	transformT[2][0] /= f;
	transformT[2][1] /= f;
	transformT *= transformTranslate;

	int iRefMarker = - 1;
	int cLinesThroughRefMarker = 0;
	int cNeighborsRefMarker = 0;
	CVisRegionList& refregionlist = m_region_extract.RegionList();
	for (int iTry = 0; ((iRefMarker < 0) && (iTry < 2)); ++iTry)
	{
		double dblRefMarkerDistSquaredToCenter = 0.0;
		CVisEdgelList& edgelistLines = LineList();
		for (int iRegion = 0; iRegion < refregionlist.size(); ++iRegion)
		{
			CVisConnectedRegion& refregion = refregionlist[iRegion];
			if ((refregion.cedgel > 0)
					&& ((refregion.fMarker)
						|| ((iTry == 1) && (refregion.fHasChild))))
			{
				int cLinesThroughMarker = 0;
				int cNeighborsMarker = 0;
				double dblMarkerDistToCenter = 0.0;
				refregion.fltHalfSqrtArea = (float) sqrt((double) refregion.n) / 2.0;
				for (int iLine = 0; iLine < edgelistLines.size(); iLine++)
				{
					CVisEdgel& refedgelLine = edgelistLines[iLine];
					if (refedgelLine.ContainsPoint(refregion.x, refregion.y,
							refregion.fltHalfSqrtArea))
					{
						if (++cLinesThroughMarker == 2)
							break;
					}
				}

				bool fFoundNewRefMarker = false;
				if (cLinesThroughMarker < cLinesThroughRefMarker)
				{
					continue;
				}
				else if (cLinesThroughMarker > cLinesThroughRefMarker)
				{
					fFoundNewRefMarker = true;
				}
				else if (refregion.nCloseNeighbors < cNeighborsRefMarker)
				{
					continue;
				}
				else if (refregion.nCloseNeighbors > cNeighborsRefMarker)
				{
					fFoundNewRefMarker = true;
				}

				double dblMarkerDistSquaredToCenter
						= (refregion.x - xC) * (refregion.x - xC)
							+ (refregion.y - yC) * (refregion.y - yC);

				if ((fFoundNewRefMarker)
						|| (dblMarkerDistSquaredToCenter
								< dblRefMarkerDistSquaredToCenter))
				{
					iRefMarker = iRegion;
					cLinesThroughRefMarker = cLinesThroughMarker;
					cNeighborsRefMarker = refregion.nCloseNeighbors;
					dblRefMarkerDistSquaredToCenter = dblMarkerDistSquaredToCenter;
				}
			}
		}
	}

	if ((iRefMarker > 0) && (m_dxGridSpacing != 0) && (m_dyGridSpacing != 0))
	{
		// Rotate marked and center and compute phase
		CVisConnectedRegion& refregionRefMarker = refregionlist[iRefMarker];
		CVisVector4 vectorRefMarker(refregionRefMarker.x, refregionRefMarker.y, 1, 1);
		vectorRefMarker = transformT * vectorRefMarker;
		if (vectorRefMarker[2] != 0)  // UNDONE:  Assert that this is true?
			vectorRefMarker /= vectorRefMarker[2];

		// Flip the x coordinate to get the x direction right.
		vectorRefMarker[0] *= - 1.0;

//#ifdef SAVE
#ifdef _DEBUG  // UNDONE:  Remove this
		char szBuf[256];
		sprintf(szBuf, "Marker blob at (%d, %d) has position (%d, %d).\n",
				(int) refregionRefMarker.x, (int) refregionRefMarker.y,
				refregionRefMarker.nXPosition, refregionRefMarker.nYPosition);
		OutputDebugString(szBuf);
#endif // _DEBUG
//#endif // SAVE

		m_fltPhaseX = vectorRefMarker[0] / m_dxGridSpacing;
		m_fltPhaseY = vectorRefMarker[1] / m_dyGridSpacing;

#ifdef SAVE // This will compute phase based on center of current image
		CVisVector4 vectorC(xC, yC, 1, 1);
		vectorC = transformT * vectorC;
		if (vectorC[2] != 0)  // UNDONE:  Assert that this is true?
			vectorC /= vectorC[2];

		// Flip the x coordinate to get the x direction right.
		vectorC[0] *= - 1.0;

		double dx = vectorC[0] - vectorRefMarker[0];
		double dy = vectorC[1] - vectorRefMarker[1];

		m_fltPhaseX = dx / m_dxGridSpacing;
		m_fltPhaseY = dy / m_dyGridSpacing;
#endif // SAVE

		// Add position information encoded in marker blob
		assert((m_cPeriodPhaseRange == 3.0) || (m_cPeriodPhaseRange == 4.0));  // From above
		m_fltPhaseX += m_cPeriodPhaseRange * refregionRefMarker.nXPosition;
		m_fltPhaseY += m_cPeriodPhaseRange * refregionRefMarker.nYPosition;

		// Marker blobs are spaced four units apart and there are 8 horizontal and
		// 8 vertical marker blob positions.
		m_cPeriodPhaseRange *= 8;

		// Normalize phase.
		double dblPeriod = m_cPeriodPhaseRange;
		double dblHalfPeriod = dblPeriod / 2;
		m_fltPhaseX -= dblPeriod * floor((m_fltPhaseX + dblHalfPeriod) / dblPeriod);
		m_fltPhaseY -= dblPeriod * floor((m_fltPhaseY + dblHalfPeriod) / dblPeriod);
	}

}

void CVisGridExtract::ExtractAll(bool use_edgels, bool estimate_f)
{
    CVisEdgelList& edgel_list = m_edge_extract.EdgelList();
    CVisRect bbox = m_input.Shape();

	m_fUseEdgels = (use_edgels != 0);
	if (use_edgels)
	{
		// UNDONE:  Should we allow these options with edgels?
		m_fInterceptsFromOrigCenters = false;
		m_fInterceptsFromTCenters = false;
	}

#ifdef _DEBUG
CVisTimer2 timerT;
int cmsStep1 = 0, cmsStep2 = 0, cmsStep3 = 0, cmsStep4 = 0;
#endif // _DEBUG

   // Iterate over the line fit / distortion correction loop
	for (int iter = 0; iter <= n_steps; iter++)
	{
		CVisEdgelList undistorted_edgels;

		// Extract per-pixel edgels
		if (use_edgels)
		{
			m_edge_extract.ExtractDoG(n_level_diff, n_blur, margin, min_strength);

#ifdef _DEBUG
cmsStep1 = timerT.CmsElapsed();
#endif // _DEBUG

			m_edge_extract.NormalizeAndComputeTheta();

#ifdef _DEBUG
cmsStep2 = timerT.CmsElapsed();
#endif // _DEBUG

			if (iter < n_steps)
				undistorted_edgels = m_edge_extract.EdgelList();
			if (k1 != 0.0 || k2 != 0.0)
				m_edge_extract.CorrectRadialDistortion(k1, k2);

#ifdef _DEBUG
cmsStep3 = timerT.CmsElapsed();
#endif // _DEBUG

		}

    // Extract the blob centers
		else
		{
			if (m_fFindCircles)
			{
				// UNDONE:  Find max or avg blob size here and use if to
				// set parmeters like min_count below.
				m_nThresholdNext = m_region_extract.ExtractCircularRegions(
						k1, k2, m_nThresholdPrev, m_nPercentMinBlobSize,
//						m_nThresholdPrev, m_nPercentMinBlobSize,
						&m_dblXUp, &m_dblYUp);

				// Set the minimum number of line intersections needed
				// to find vanishing points based on the number of regions
				// that we found.
				if (m_region_extract.RegionList().size() < 13)
				{
					min_count = 2;
				}
				else if (m_region_extract.RegionList().size() < 25)
				{
					min_count = 3;
				}
				else
				{
					min_count = (int)
							(0.5 * sqrt((double) m_region_extract.RegionList().size()));
				}
			}
			else
			{
				m_region_extract.InitializeLabels(n_level_diff, n_blur);

#ifdef _DEBUG
cmsStep1 = timerT.CmsElapsed();
#endif // _DEBUG

				m_region_extract.ErodeLabels(n_erode);

#ifdef _DEBUG
cmsStep2 = timerT.CmsElapsed();
#endif // _DEBUG

				m_region_extract.ExtractRegions();
			}

#ifdef _DEBUG
cmsStep3 = timerT.CmsElapsed();
#endif // _DEBUG

			if (iter < n_steps)
				undistorted_edgels = m_region_extract.ExtractGrid(gap);
			if ((!m_fFindCircles) && (k1 != 0.0 || k2 != 0.0))
//			if (k1 != 0.0 || k2 != 0.0)
				m_region_extract.CorrectRadialDistortion(k1, k2);

			// Note:  For circular regions, we want (1 + gap) * sqrt(PI) to
			// be between 3 and 3 * sqrt(2) if the centers of the circles
			// are three radii apart, so we want the default gap size to be
			// around 1.0.  Since the default gap size for a square grid is
			// 0.8, we multiply by 1.2 to get the gap size for circular
			// regions.  (We want it to be a little less than one to reduce
			// false edges.)
			// (Changed to 1.1 to avoid diagonals when using markers.  Maybe
			// it's better to only use the smaller blob's area instead?)
			// LATER:  We may want to vary the gap size with the number of
			// regions that we found.
			if (m_fFindCircles)
				edgel_list = m_region_extract.ExtractGrid(gap * 1.1, !m_fFindCircles);
			else
				edgel_list = m_region_extract.ExtractGrid(gap, !m_fFindCircles);

#ifdef _DEBUG
cmsStep4 = timerT.CmsElapsed();
#endif // _DEBUG

		}

		// Extract the lines
		m_line_extract.k1 = k1;
		m_line_extract.k2 = k2;
		m_line_extract.Initialize(bbox, pixel_spacing, angle_spacing);
		m_line_extract.Accumulate(edgel_list);
		// UNDONE:  min_length should depend on the max blob size or prev grid spacing.
		m_line_extract.Extract(n_nghbrs, collapse_opposites, min_length);

#ifdef VIS_WRITE_DEBUG_FILES
PrintEdgels("tmp_edgels.ps");
PrintLines("tmp_lines.ps");
PrintFittedEdgels("tmp_linels.ps");
#endif // VIS_WRITE_DEBUG_FILES

		// Re-estimate (k1,k2) using the plumbline method
		if (iter < n_steps)
		{
			m_line_extract.EstimateRadialDistortion(undistorted_edgels, step_size);
			k1 = m_line_extract.k1;
			k2 = m_line_extract.k2;
		}
	}

    // Extract the vanishing points
    m_vp.Initialize(bbox, f, n_bins, min_dot, min_dist);
    m_vp.Accumulate(m_line_extract.LineList());
    m_vp.Extract(vp_nghbrs, min_count);
	
	// UNDONE:  Not sure if this is a good idea.
	if ((m_vp.PointList().size() < 2) && (m_fFindCircles))
	{
		// Try reducing the number of lines needed to define a vanishing point.
		m_vp.Initialize(bbox, f, n_bins, min_dot, min_dist);
		m_vp.Accumulate(m_line_extract.LineList());
		m_vp.Extract(vp_nghbrs, 1);
	}

    // Estimate the grid rotation
    if (estimate_f)
	{
        float f_confidence;
        m_vp.EstimateF(f, f_confidence);
    }

	if (m_vp.PointList().size() < 2)
	{
		m_fHavePose = false;
	}
	else
	{
		// Try to find an "up" vector to use when finding the rotation matrix.
		double dblXUp = m_dblXUp;
		double dblYUp = m_dblYUp;
		if ((dblXUp == 0) && (dblXUp == 0))
		{
			// If no "up" vector from regions, try using the previous y vector.
			dblXUp = - m_pose[1][0];
			dblYUp = - m_pose[1][1];
		}
		if ((dblXUp == 0) && (dblXUp == 0))
		{
			// If no previous y vector, try using vanishing points.
			dblXUp = m_vp.PointList()[0][0];
			dblYUp = m_vp.PointList()[0][1];
			if (fabs(dblXUp) > fabs(dblYUp))
			{
				// Find orthogonal direction.
				dblXUp = - m_vp.PointList()[0][1];
				dblYUp = m_vp.PointList()[0][0];
			}
			if (dblYUp < 0)
			{
				// Switch signs
				dblXUp = - dblXUp;
				dblYUp = - dblYUp;
			}
		}

		m_fHavePose = m_vp.EstimateR(m_pose, (float) dblXUp, (float) dblYUp,
				m_fAverageVanishingPoints);
	}

    // Estimate the grid scale and translation
	FindGridSpacingAndPhase();

#ifdef _DEBUG
#ifdef VIS_PRINT_DEBUG_TIMING_STRINGS
char szBuf[256];
sprintf(szBuf, "CVisGridExtract::ExtractAll time = %d", timerT.CmsElapsed());
OutputDebugString(szBuf);
if (use_edgels)
	sprintf(szBuf, "  ExtractDoG = %d, Normalize = %d, Distortion = %d\n",
			cmsStep1, cmsStep2 - cmsStep1, cmsStep3 - cmsStep2);
else if (m_fFindCircles)
	sprintf(szBuf, "  Regions = %d, Grid = %d\n",
			cmsStep3, cmsStep4 - cmsStep3);
else
	sprintf(szBuf, "  Initialize = %d, Erode = %d, Regions = %d, Grid = %d\n",
			cmsStep1, cmsStep2 - cmsStep1, cmsStep3 - cmsStep2, cmsStep4 - cmsStep3);
OutputDebugString(szBuf);
#endif // VIS_PRINT_DEBUG_TIMING_STRINGS
#endif // _DEBUG
}
