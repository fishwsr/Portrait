///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisRegionExtract.h -- extract lines from images
//
// DESCRIPTION
//  Extract connected component regions from an image.
//
// SEE ALSO
//  VisRegionExtract.cpp          implementation
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

#ifndef _VIS_REGION_EXTRACT_H_
#define _VIS_REGION_EXTRACT_H_

#include <vector>
//#include "VisSfM/VisSDStream.h"
#include "VisLineExtract.h"

//
// The CVisConnectedRegion is used for connected component computation
//

class CVisRegionRun
{
public:
    int id;         // region id
    int val;        // gray-level value (> 0)
    int n;          // number of pixels
    int x;          // column start
    int y;          // row number

    bool Overlap(CVisRegionRun &a) {
        return (val == a.val) && (a.x < x+n) && (x < a.x+a.n);
    }
    bool operator<(const CVisRegionRun &a) const {
        return id < a.id;
    }
    bool operator==(const CVisRegionRun &a) const {
        return id == a.id;
    }
};
class CVisRunList :
    public std::vector<CVisRegionRun>,
    public CVisSortable<CVisRunList> {};
typedef std::vector<CVisRunList> CVisRunDoubleList;

class CVisConnectedRegion : public CVisSortable<CVisConnectedRegion>
{
public:
    double x, y;    // centroid (also used for moment calculations)
    int id;         // region id
    int val;        // gray-level value (> 0)
    int n;          // number of pixels
    int equiv_id;   // lower-numbered region connected to
	int cedgel;		// number of edgels containing this region
	float fltHalfSqrtArea;	// sqrt(n) / 2, chached when finding phase
	bool fMarker;	// Is this a marker region?
	bool fHasChild;	// Does this region contain a child region?
    double xChild, yChild;	// centroid of largest hold in blob
	int nXPosition, nYPosition;	// Position encoded with holes in marker blobs

	// Nearest neighbor information.  (Four or fewer closest neighbors.)
	// UNDONE:  Probably better use a single array of structs here.
	int nCloseNeighbors;
	int iCloseNeighbor[4];
	bool fBorderNeighbor[4];
	double dblSquaredDistToNeighbor[4];

	void AddCloseNeighbor(int iNeighbor, bool fBorder, double dblDistSquared);



    void Reset(int id0, int eq = -1) {
        id = id0, x = y = n = 0, equiv_id = eq;
    }
    CVisConnectedRegion& operator+=(CVisRegionRun& run);
    CVisConnectedRegion& operator+=(CVisConnectedRegion& r);
    void Fit();     // compute centroid
};
typedef std::vector<CVisConnectedRegion> CVisRegionList;

//
// The CVisRegionExtract class extract connected components from an image
//

class CVisRegionExtract {
public:
    // Set the source image
    void SetImage(CVisByteImage& input);

    // Compute initial tri-state labels
    void InitializeLabels(int n_level_diff = 2, int n_blur = 2);

    // Erode label image to disconnect thin necks
    void ErodeLabels(int n_erode = 2);

    // Extract regions above (below) a threshold
    void ExtractRegions();
	
	// Extract circular regions and return the threshold to use with
	// the next image (based on a histogram of this image).  This will correct
	// for radial distortion when the regions are extracted.
	int ExtractCircularRegions(double k1, double k2,
			int nThresholdPrev, int nPercentMinBlobSize,
			double *pdblXUp, double *pdblYUp);

    // Correct for radial distortion
    void CorrectRadialDistortion(double k1, double k2);

    // Return the computed region list
    CVisRegionList& RegionList()    { return m_region_list; }

    // Return the list of border regions
    CVisRegionList& BorderRegionList()    { return m_border_region_list; }

	// Option to compute a thresholded image (for debugging)
	bool FComputeImageThresholded(void) const { return m_fComputeImageThresholded; }
	void SetFComputeImageThresholded(bool f) { m_fComputeImageThresholded = f; }
	CVisByteImage& ImageThresholded(void) { return m_imageThresholded; }

    // Fill in an image with (scaled) region id's
    void ComputeLabels(int scale = 1, int bias = 1);
    CVisByteImage& LabelImage()     { return m_label; }

    // Compute a "grid" from region centers
    CVisEdgelList ExtractGrid(double gap = 0.8, int match_other = 1);

protected:
    void ExtractRuns(int y);
    void MergeRun(CVisRegionRun& run, CVisRunList& prev);
    void RenumberRegions();

    CVisByteImage m_input;          // input image
    CVisByteImage m_label;          // labeled pixel image
    CVisRunDoubleList m_runs;       // list of runs in each row
    CVisRegionList m_region_list;   // computed region list
    CVisRegionList m_border_region_list;   // temporary border region list

	CVisByteImage m_imageThresholded;	// Thresholded image for debugging
	bool m_fComputeImageThresholded;
};


//
//  The CVisGridExtract is used to fit a grid to a checkerboard image
//

class CVisGridExtract {
public:
    // Initialize the parameters to defaults
    CVisGridExtract();

    // Set the source image
    void SetImage(CVisByteImage& input);

    // Compute edgels/blobs, lines, and vanishing point
    void ExtractAll(bool use_edgels, bool estimate_f = false);

    // Return the computed edgel list
    CVisEdgelList& EdgelList()
        { return m_edge_extract.EdgelList(); }

    // Return the computed line list
    CVisEdgelList& LineList()
        { return m_line_extract.LineList(); }
    CVisIndexCollection& IndexCollection()
        { return m_line_extract.IndexCollection(); }

    // Return the computed vanishing points
    CVisVector4V& VanishingPointList()
        { return m_vp.PointList(); }

    // Return the computed grid pose matrix
    CVisTransform4x4& GridPose()
        { return m_pose; }

    bool HavePose(void) const
        { return m_fHavePose; }

	// Set the pose to a value returned from an earlier grid extraction.
	void SetGridPose(const CVisTransform4x4& refpose)
	{
		m_pose = refpose;
		m_fHavePose = true;
	}

    // Print an edgel list
    void PrintEdgels(char* filename)
        { m_edge_extract.PrintEdgels(filename); }
    void PrintLines(char* filename)
        { m_edge_extract.PrintEdgels(filename, LineList()); }
    void PrintFittedEdgels(char* filename)
        { m_edge_extract.PrintEdgels(filename, IndexCollection()); }

	// Return line extractor 
    CVisHoughLineExtract& LineExtract() { return m_line_extract; }

	// Get grid spacing and phase information
    float GridSpacingX(void) const { return m_dxGridSpacing; }
    float GridSpacingY(void) const { return m_dyGridSpacing; }
    float PhaseX(void) const { return m_fltPhaseX; }
    float PhaseY(void) const { return m_fltPhaseY; }
    float PhaseMin(void) const { return (float) - 0.5 * m_cPeriodPhaseRange; }
    float PhaseMax(void) const { return (float) 0.5 * m_cPeriodPhaseRange; }
    float PhaseRange(void) const { return (float) m_cPeriodPhaseRange; }

	void SetPhase(float fltPhaseX, float fltPhaseY)
		{ m_fltPhaseX = fltPhaseX; m_fltPhaseY = fltPhaseY; }

	// "Up" vector, as computed from hole in circular regions.
	// (Not normalized.)
	double XUp(void) const { return m_dblXUp; }
	double YUp(void) const { return m_dblYUp; }


public:
    // User-modifiable parameters for radial lens distortion correction
    double k1;                  // quadratic correction component (scaled by f)
    double k2;                  // quartic   correction component (scaled by f)
    int n_steps;                // radial lens distortion correction steps
    float step_size;            // radial lens distortion correction factor

    // User-modifiable parameters for edge extraction
    int n_level_diff;           // number of levels in difference of Gaussian
    int n_blur;                 // number of blur steps at finest level
    int margin;                 // number of ignored pixels around edge
    double min_strength;        // minimum strength of edgel

    // User-modifiable parameters for region extraction
    int n_erode;                // number of erosion steps before blob extraction
    double gap;                 // maximum extra distance between blob centers

    // User-modifiable parameters for line extraction
    int pixel_spacing;          // spacing between lines in different bins
    int angle_spacing;          // amount of orientation in each bin
    int n_nghbrs;               // half-size of accumulation neighborhood (1 => 3x3)
    bool collapse_opposites;    // ignore polarity of the line segments
    double min_length;          // minimum length of line

    // User-modifiable parameters for vanishing points
    float f;                    // focal length (in pixels)
    int n_bins;                 // number of bins on side (in van. pt. space)
    float min_dot;              // cos of max angle between paired lines
    float min_dist;             // lines must be at least this far apart
    int vp_nghbrs;              // half-size of accumulation neighborhood (1 => 3x3)
    int min_count;              // minimum number of intersections
	
	
	// Settings used when extracting circular regions
	bool m_fFindCircles;
	int m_nThresholdNext;
	int m_nThresholdPrev;
	int m_nPercentMinBlobSize;
	bool m_fInterceptsFromOrigCenters;
	bool m_fInterceptsFromTCenters;
	bool m_fUsePositionsForInterceptOffsets;
	bool m_fAverageVanishingPoints;

	bool m_fComputeImageThresholded;
	CVisByteImage& ImageThresholded(void)
			{ return m_region_extract.ImageThresholded(); }

	// Struct used internally.  Must be public for use with STL.
	struct SIntercept
	{
		// Members
		float position;
		int cPeriodOffset;
		int cMarkers;
		bool fReference;

		// Default ctor
		SIntercept(float fltPosition = 0.0f);

		bool operator<(const SIntercept& refintercept) const
			{ return position < refintercept.position; }
	};

protected:
    CVisByteImage m_input;              // input image
    CVisEdgeExtract m_edge_extract;     // edge-line based extractor
    CVisRegionExtract m_region_extract; // region-blor based extractor
    CVisHoughLineExtract m_line_extract;     // line extractor
    CVisVanishingPoint m_vp;            // vanishing point extractor
    CVisTransform4x4 m_pose;            // grid pose
	bool m_fHavePose;					// do we have a pose estimate?
	bool m_fUseEdgels;					// Are we using edgels to extract lines?

	float m_dxGridSpacing;
	float m_dyGridSpacing;
	float m_fltPhaseX;
	float m_fltPhaseY;
	int m_cPeriodPhaseRange;

	double m_dblXUp;
	double m_dblYUp;

	// Given the line list, pose and region list if not using edgels,
	// find the grid spacing and phase.  If not using edgels, this
	// method will look for lines containing marked regions and use
	// such lines to get more accurate phase information.
	void FindGridSpacingAndPhase(void);

	// Helper function.  Return the phase of x when spacing is dx.
	static float PhaseFromXAndDx(float x, float dx);

	// Helper function.  Sort a vector and return the median.
	float MedianAfterSorting(std::vector<float>& refvector);

	// Helper function.  Find an average phase vector
	float AveragePhase(std::vector<float>& refvector, float fltMin,
			float fltMax);

	// Given sorted intercepts and grid spacing, find intercept offsets
	// from marked intercepts and return the number of periods between
	// marked intercepts.  If m_fUsePositionsForInterceptOffsets is true,
	// offsets will be calculated by dividing the intercept positions by
	// the grid spacing.  Otherwise, offsets will be calculated by the
	// number of intercepts to the previous or next marked intercept.
	int CPeriodMarkedCalcInterceptOffsets(std::vector<SIntercept>& refvector,
			float fltPeriod);


	// Given sorted intercepts, grid spacing, and number of intercepts
	// between marked intercepts, find the current phase (between
	// - cPeriodMarked / 2.0 and cPeriodMarked / 2.0) by looking at the
	// intercept(s) closest to 0.
	float PhaseFromIntercepts(std::vector<SIntercept>& refvector,
			float fltPeriod, int cPeriodMarked);

	void FindMarkersOnLines(void);

	void FindIntercepts(std::vector<SIntercept>& refvectorX,
			std::vector<SIntercept>& refvectorY);
};

#endif // _VIS_REGION_EXTRACT_H_
