// VisRectangularGrid.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


struct SVisGridPoint
{
	double x, y;
	int nArea;

	// LATER:  We might want to remove r, c, fBorder, and fMarker from points
	// in the final grid.  (It's useful to keep them in intermediat points
	// so that we can provide feedback to the user when we can't locate the
	// final grid.)
	int r, c;
	bool fMarker;
	bool fBorder;

	// LATER:  We don't need to store information about neighbors in this
	// structure.  We only need information about neighbors for points that
	// we think may be in the grid (points connected to markers by a chain
	// of close neighbors).  We only use information about neighbors at most
	// once for any point.  The code would be more efficient if we just
	// calcualted the neighbors for a point when they were needed.  (But there
	// wouldn't be much of the preformance gain in the current implementation,
	// since most of the time now is used to find the connected components in
	// the thresholded image.)
	int cCloseNeighbors;  // May be 0 in final grid, since this is implicit if we find a grid.
	int mpiiCloseNeighborToGridPoint[4];
	double rgdblDistSquaredToCloseNeighbor[4];  // May be 0.0 in final grid
};


class CVisRectangularGrid
{
public:
	// Constructor
	CVisRectangularGrid(void);


	// Operations
	bool Extract(CVisByteImage& refimage);

	
	// Attributes
	// UNDONE:  Make attributes private and provide access methods
	int m_cRow;
	int m_cCol;

	std::vector<SVisGridPoint> m_vectorGridPoint;// I and RC variations

	bool HaveGrid(void) const
		{ return m_fHaveGrid; }

	const CVisByteImage& Image(void) const
		{ return m_image; }


	// Options
	// UNDONE:  Move inline functions to an INL or CPP file
	int PercentMinBlobSize(void) const
		{ return m_nPercentMinBlobSize; }
	void SetPercentMinBlobSize(int nPercent)
		{ m_nPercentMinBlobSize = nPercent; }

	int NThresholdSteps(void) const
		{ return m_nThresholdSteps; }
	void SetNThresholdSteps(int nSteps)
		{ m_nThresholdSteps = nSteps; }

	int IThreshold(void) const
		{ return m_iThresholdCur; }
	void ClearIThreshold(void)
		{ m_iThresholdCur = - 1; }
	void SetIThreshold(int iThreshold)
		{ m_iThresholdCur = iThreshold; }

	int IThresholdLow(void) const
		{ return m_iThresholdLow; }
	int IThresholdHigh(void) const
		{ return m_iThresholdHigh; }
	void ClearThresholdRange(void)
		{ m_iThresholdLow = - 1; m_iThresholdHigh = - 1; }
	void SetThresholdRange(int iLow, int iHigh)
		{
			assert((iLow >= 0) && (iLow < 255));
			assert((iHigh >= 0) && (iHigh < 255));
			assert(iLow <= iHigh);
			m_iThresholdLow = iLow;
			m_iThresholdHigh = iHigh;
		}


	bool DotsInHolesInMarkers(void) const
		{ return m_fDotsInHolesInMarkers; }
	void SetDotsInHolesInMarkers(bool f)
		{ m_fDotsInHolesInMarkers = f; }

	bool StoreThresholdedImage(void) const
		{ return m_fStoreThresholdedImage; }
	void SetStoreThresholdedImage(bool f)
		{ m_fStoreThresholdedImage = f; }

	bool UpdateThresholdWhenGridFound(void) const
		{ return m_fUpdateThresholdWhenGridFound; }
	void SetUpdateThresholdWhenGridFound(bool f)
		{ m_fUpdateThresholdWhenGridFound = f; }

	bool LimitThresholdOnAllSteps(void) const
		{ return m_fLimitThresholdOnAllSteps; }
	void SetLimitThresholdOnAllSteps(bool f)
		{ m_fLimitThresholdOnAllSteps = f; }


private:
	bool m_fHaveGrid;

	CVisByteImage m_image;

	int m_cMarker;
	int m_cLabeled;
	int m_mpiiMarkerToPoint[10];

	int m_nPercentMinBlobSize;
	int m_nThresholdSteps;
	int m_iThresholdCur;
	int m_iThresholdLow;
	int m_iThresholdHigh;

	bool m_fDotsInHolesInMarkers;
	bool m_fStoreThresholdedImage;
	bool m_fUpdateThresholdWhenGridFound;
	bool m_fLimitThresholdOnAllSteps;

	// Helper function to find possible grid points from a tree of blobs.
	void GetPointsFromPBlobTree(CVisBlobTreeInternal *pblobtree, bool fOnlyMarkers = false);

	// Helper function to find the RECT bounding the marker points
	RECT RectBoundingMarkers(void) const;

	// Helper functions to order the marker points so that they
	// go CW around their centroid.
	bool FOrderMarkers(void);

	// Helper function to find close neighbors for each point.
	void FindCloseNeighbors(void);

	// Helper function to label rows and columns in the grid.
	bool FFindLabels(void);

	// Helper function used when labeling border rows
	int INextLabeledFromIPrevICurBorder(int iPrev, int iCur);

	// Helper function used when labeling interior rows
	int INextLabeledFromIPrevICurInterior(int iPrev, int iCur);

	// Helper function to make the final grid after the initial label have been found.
	bool FMakeFinalGrid(void);
};
