///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLineExtract.h -- extract lines from images
//
// DESCRIPTION
//  
//
// SEE ALSO
//  VisLineExtract.cpp          implementation
//  
//
//  Rick's research notes, 14-May-98
//
// BUGS
//
//
// DESIGN
//  
//
// HISTORY
//  14-May-98  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_HOUGH_LINE_EXTRACT_H_
#define _VIS_HOUGH_LINE_EXTRACT_H_

#include <vector>

//
//  A CVisEdgel is an edge fragment computed at one pixel
//

class CVisEdgel : public CVisSortable<CVisEdgel> {
public:
    float x, y;         // sub-pixel edge position
    float n_x, n_y;     // orientation, as normal vector
    float theta;        // orientation, as angle (radians)
    float length;       // length of edgel
    float strength;     // strength of edgel (local gradient magnitude)
    float line_length;  // length of line (estimated from ellipsoid)
    float sigma;        // estimated std. dev. of edgel noise
	float r;			// line equation is x n_y - y n_x = r or x sintheta - y costheta = r
	int cMarkers;		// Number of marker regions on this line
	bool fReferenceLine;// Used to find phase larger than one period
	
	CVisEdgel() { x=y=n_x=n_y=theta=strength=line_length=sigma=r=0;length=1;};
    bool ZeroCrossing(int v00, int v01, int v10, int v11);
    void NormalizeN();
    void ComputeTheta();
    CVisEdgel CorrectRadialDistortion(float x0, float y0, float W,
                                      float k1, float k2);

	// Get the endpoints of the edgel.
	void GetEndpoints(float *px0, float *py0, float *px1, float *py1) const;

	// Set x, y, n_x, and n_y from the given endpoints.  The n_x and n_y
	// values will be normalized.
	void SetEndpoints(float x0, float y0, float x1, float y1);

	bool ContainsPoint(float fltX, float fltY, float fltError = 1.0) const;
};

typedef std::vector<CVisEdgel> CVisEdgelList;

// Transform a segment on the plane of projection and return the
// edgel cooresponding to the projection of the transformed segment.
CVisEdgel operator*(const CVisTransform4x4& reftransform,
		const CVisEdgel& refedgel);

//
// CVisIndexList is used to keep a list of indices of the edgels
// which make up a line.  We need an explicit deallocator.
//

class CVisIndexList : public std::vector<int>
{
    // We need to add an explicit deallocator (same fn as ~vector()).
    //  This code copied from the <vector> header file.
public:
	void clearout()
		{_Destroy(_First, _Last);
		allocator.deallocate(_First, _End - _First);
		_First = 0, _Last = 0, _End = 0; }
};

typedef std::vector<CVisIndexList> CVisIndexCollection;

//
// The CVisEdgeExtract class extract a list of edgels from an image
//

class CVisEdgeExtract {
public:
    // Set the source image
    void SetImage(CVisByteImage& input);

    // Extract edges using difference of Gaussians
    void ExtractDoG(int n_level_diff = 2, int n_blur = 2,
                    int margin = 4, float min_strength = 16.0);
    void NormalizeAndComputeTheta();

    // Correct for radial distortion
    void CorrectRadialDistortion(float k1, float k2);

    // Return the computed edgel list
    CVisEdgelList& EdgelList()      { return m_edgel_list; }

    // Draw the edgel list as PostScript file
    void PrintEdgels(char* filename);
    void PrintEdgels(char* filename, CVisEdgelList& el);
    void PrintEdgels(char* filename, CVisIndexCollection& ec);
    void PrintHeader(FILE* stream);

protected:
    CVisByteImage m_input;  // input image
    CVisEdgelList m_edgel_list;     // resulting edgel list
};


//
// The CVisLineFit class is used to accumulate 2nd order moments of edgels
// and keep a record of its associated edgel indeces
//

class CVisLineFit : public CVisSortable<CVisLineFit> {
public:

    void Reset();
    void Increment(const CVisEdgel& e, int i);
    CVisEdgel Fit(const CVisEdgelList& el);
    double LengthSum()  { return m_l; }
    CVisLineFit& operator+=(const CVisLineFit& a);
    CVisIndexList& IndexList()  { return m_il; }

protected:
    CVisIndexList m_il; // list of constituent edgel indices
    double m_l;         // sum of lengths
};

//
// The CVisHoughLineExtract class converts an CVisEdgelList into
//  a CVisLineList using a generalized Hough transform plus fitting.
//

class CVisHoughLineExtract {
public:
    // Constructor and destructor
    CVisHoughLineExtract();
    ~CVisHoughLineExtract();

    // Initialize, accumulate, and extract lines
    void Initialize(RECT& shape,
                    int pixel_spacing, int angle_spacing);
    void Accumulate(CVisEdgelList& edgel_list);
    CVisByteImage GetCounts(float scale);
    void Extract(int nghbrs = 1, bool collapse_opposites = false,
                 float min_length = 20.0);
    CVisEdgelList& LineList()   { return m_line_list; }
    CVisIndexCollection& IndexCollection()   { return m_index_lists; }

    // Re-estimate the radial distortion
	void EstimateRadialDistortion(CVisEdgelList& edgel_list,
                                  float step_size);
#ifdef OLD
    // Iteratively correct the radial distortion
	void CorrectDistortion(CVisEdgelList& edgel_list,
                           int n_steps, float step_size);  
	void LineAssignment(CVisEdgelList& edgel_list); 
	void SetupOneSystem(CVisEdgelList& edgel_list, int i, int startRow); 
	void SetupSystems(CVisEdgelList& edgel_list); 
#endif

    // Setable parameters
    double k1, k2;              // radial distortion parameters

protected:
    CVisLineFit& Bin(int it, int ir);   // access one Hough bin
    void ResetAll();                    // Reset() all Bin()s

    CVisDoubleImage m_Hough;    // accumulator array for local line fits
    int m_n_r, m_n_t;           // half size of r and t bins
    float m_x0, m_y0;           // center of image
    float m_width;              // width of image (for radial distortion)
    float m_r_s, m_t_s ;        // r and theta scale factors
    CVisEdgelList m_edgel_list; // list of input edgels
    CVisEdgelList m_line_list;  // list (vector) of fitted lines
    CVisIndexCollection m_index_lists;  // lists of fitted edgel indices
};

// This belongs in some other file, like VisPyramid.h or VisConvolve.h:...
CVisByteImage DifferenceOfGaussian(CVisByteImage& src, int n_levels, int n_blur,
		int offset = 0);


//
// The CVisVPFit class is used to accumulate outer products of line equations
//

class CVisVPFit : public CVisSortable<CVisLineFit> {
public:

    void Reset();
    CVisVPFit& operator+=(CVisVPFit& a);
    CVisVPFit& operator+=(CVisVector4& l);
    CVisVector4 Fit(float f);
    int Count()  { return int(m_count); }

protected:
    double m_A[3][3];   // outer product matrix
    double m_count;     // count of line pairs added
};

//
// The CVisVanishingPoint class is used to compute vanishing points
// from a set of lines.
//

typedef std::vector<CVisVector4> CVisVector4V;
#if 0   // I'm not sure why these aren't needed ...???
inline bool operator<(const CVisVector4& a, const CVisVector4& b) { return true; }
inline bool operator==(const CVisVector4& a, const CVisVector4& b) { return true; }
#endif

class CVisVanishingPoint {
public:
    void Initialize(RECT& shape, float f = 100.0, int n_bins = 64,
                    float min_dot = 0.8, float min_dist = 4.0);
    void Accumulate(CVisEdgelList& line_list);
    void Increment(CVisEdgel& l0, CVisEdgel& l1);
    void Extract(int nghbrs = 1, int min_count = 6);
    CVisVector4V& PointList()           { return m_vp_list; }

    void EstimateF(float& f, float& confidence);
    bool EstimateR(CVisTransform4x4& refpose, float fltXUp, float fltYUp,
			bool fUseWeightedAverage);
    // TODO: add routines for estimating other pose parameters...

protected:
    CVisVPFit& Bin(int vx, int vy);
    CVisByteImage GetCounts(float scale);

    float m_x0, m_y0;           // center of image
    int m_f;                    // estimated focal length
    int m_n_bins;               // number of bins
    float m_min_dot;            // minimum dot product between lines
    float m_min_dist;           // minimum distance between lines
    CVisDoubleImage m_Hough;    // accumulator array for local line fits
    CVisVector4V m_vp_list;     // list (vector) of vanishing points
};

#endif // _VIS_HOUGH_LINE_EXTRACT_H_
