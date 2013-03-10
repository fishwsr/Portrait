///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLineExtract.cpp -- extract lines from images
//
// DESCRIPTION
//  
//
// SEE ALSO
//  VisLineExtract.h        longer description of classes
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


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif // M_PI
#include <algorithm>
#include <time.h>

//
//  A CVisEdgel is an edge fragment computed at one pixel
//

//
// Extract an edgel as a (singular) zero crossing pair
//

bool CVisEdgel::ZeroCrossing(int v00, int v01, int v10, int v11)
{
    // Test how many sign changes there are
    int s00 = (v00 > 0), s01 = (v01 > 0);
    int s10 = (v10 > 0), s11 = (v11 > 0);
    int ex0 = (s00 != s01), ex1 = (s10 != s11);
    int ey0 = (s00 != s10), ey1 = (s01 != s11);
    int n_crossings = ex0 + ex1 + ey0 + ey1;
    if (n_crossings != 2)
        return false;       // ignore double edgel condition...

    // Compute the two edgel endpoints
    float pt[2][2];
    int k = 0;
    if (ex0)
        pt[k][0] = (v00 - 0.5) / (v00 - v01), pt[k][1] = 0, k++;
    if (ex1)
        pt[k][0] = (v10 - 0.5) / (v10 - v11), pt[k][1] = 1, k++;
    if (ey0)
        pt[k][1] = (v00 - 0.5) / (v00 - v10), pt[k][0] = 0, k++;
    if (ey1)
        pt[k][1] = (v01 - 0.5) / (v01 - v11), pt[k][0] = 1, k++;

    // Compute mid-point, and set orientation to difference
    x   = 0.5 * (pt[0][0] + pt[1][0]);
    y   = 0.5 * (pt[0][1] + pt[1][1]);
    n_x = pt[0][0] - pt[1][0];
    n_y = pt[0][1] - pt[1][1];
    // ... the length and theta are set in NormalizeN and ComputeTheta

    // Compute strength from the gradient magnitude
    if (ex0 && ex1)
        ey0 = ey1 = 1;      // use average if neither is crossed
    else if (ey0 && ey1)
        ex0 = ex1 = 1;      // use average if neither is crossed
    float grad_x = (ex0 * (v01 - v00) + ex1 * (v11 - v00)) / 
                        float(ex0 + ex1);
    float grad_y = (ey0 * (v10 - v00) + ey1 * (v11 - v10)) / 
                        float(ey0 + ey1);
    strength = grad_x * grad_x + grad_y * grad_y;

    // Reverse the orientation if not compatible with gradient
    if (grad_x * n_y - grad_y * n_x < 0.0)
        n_x = -n_x, n_y = -n_y;

    line_length = sigma = 0.0;  // only used for fitted lines
    return true;
}

//
// Compute length and orientation of an edgel
//

void CVisEdgel::NormalizeN()
{
    length = _hypot(n_x, n_y);
    double li = (length > 0.0) ? 1.0/length : 0.0;
    n_x *= li, n_y *= li;
}

void CVisEdgel::ComputeTheta()
{
    theta = (180 / M_PI) * atan2(n_y, n_x);
    if (theta < -180)
        theta += 360;
    if (theta >= 180)
        theta -= 360;   // [-180,180) needed in Hough binning...
}

//
//  Correct for radial lens distortion, including scaling and rotation
//

CVisEdgel CVisEdgel::CorrectRadialDistortion(float x0, float y0, float W, 
                                             float k1, float k2)
{
    CVisEdgel e = *this;
    float Wi = 2.0 / W;
    float xd = (x - x0)*Wi, xd2 = xd*xd;
    float yd = (y - y0)*Wi, yd2 = yd*yd;
    float r2 = xd2 + yd2;
    float s  = 1.0 + k1 * r2 + k2 * r2 * r2;
    e.x = x0 + s*(x - x0);
    e.y = y0 + s*(y - y0);

    // Compute the Jacobian of the transformation
    float du_dx = s + 2.0 * k1 *  xd2  + 4.0 * k2 *  xd2  * xd2;
    float du_dy =     2.0 * k1 * xd*yd + 4.0 * k2 * xd*yd * yd2;
    float dv_dx =     2.0 * k1 * xd*yd + 4.0 * k2 * xd*yd * xd2;
    float dv_dy = s + 2.0 * k1 * yd2 + 4.0 * k2 * yd2 * yd2;

    // Compute the new normal and line orientation
    e.n_x = length * (du_dx * n_x + du_dy * n_y);
    e.n_y = length * (dv_dx * n_x + dv_dy * n_y);
    e.NormalizeN();
    e.ComputeTheta();

    return e;
}

// Get the endpoints of the edgel.
void CVisEdgel::GetEndpoints(float *px0, float *py0, float *px1, float *py1) const
{
	assert((px0 != 0) && (py0 != 0) && (px1 != 0) && (py1 != 0));

	float fltT = length / 2.0;
	float fltNLength = _hypot(n_x, n_y);
	if (fltNLength > 0)
		fltT /= fltNLength;

	*px0 = x - fltT * n_x;
	*px1 = x + fltT * n_x;
	*py0 = y - fltT * n_y;
	*py1 = y + fltT * n_y;
}

// Set x, y, n_x, and n_y from the given endpoints.  The n_x and n_y
// values will be normalized.
void CVisEdgel::SetEndpoints(float x0, float y0, float x1, float y1)
{
	x = (x0 + x1) / 2;
	y = (y0 + y1) / 2;
	n_x = x1 - x0;
	n_y = y1 - y0;

	length = (float) _hypot(n_x, n_y);
	if (length > 0)
	{
		n_x /= length;
		n_y /= length;
	}
}

bool CVisEdgel::ContainsPoint(float fltX, float fltY, float fltError) const
{
	double dx = fltX - x;
	double dy = fltY - y;
	double dblT = n_y * dx - n_x * dy;
	if ((dblT >= - fltError) && (dblT <= fltError))
	{
		dblT = length / 2.0 + fltError;
		return (dx * dx + dy * dy <= dblT * dblT);
	}
	return false;
}

// Transform a segment on the plane of projection and return the
// edgel cooresponding to the projection of the transformed segment.
CVisEdgel operator*(const CVisTransform4x4& reftransform,
		const CVisEdgel& refedgel)
{
	float x0, y0, x1, y1;
	refedgel.GetEndpoints(&x0, &y0, &x1, &y1);
	CVisVector4 v0(x0, y0, 1.0, 1.0);
	CVisVector4 v1(x1, y1, 1.0, 1.0);

	// Apply the transform.
	CVisVector4 v0T = reftransform * v0;
	CVisVector4 v1T = reftransform * v1;

	// Project the results
	if ((v0T[2] != 0.0) && (v1T[2] != 0.0))
	{
		x0 = (float) (v0T[0] / v0T[2]);
		y0 = (float) (v0T[1] / v0T[2]);
		x1 = (float) (v1T[0] / v1T[2]);
		y1 = (float) (v1T[1] / v1T[2]);
	}
	else if (v0T[2] != 0.0)
	{
		x0 = (float) (v0T[0] / v0T[2]);
		y0 = (float) (v0T[1] / v0T[2]);
		x1 = x0;
		y1 = y0;
	}
	else if (v1T[2] != 0.0)
	{
		x1 = (float) (v1T[0] / v1T[2]);
		y1 = (float) (v1T[1] / v1T[2]);
		x0 = x1;
		y0 = y1;
	}
	else
	{
		// Not sure what to do in this case.
		x0 = y0 = x1 = y1 = 0.0;
	}

	CVisEdgel edgelRet;
	edgelRet.SetEndpoints(x0, y0, x1, y1);
	edgelRet.cMarkers = refedgel.cMarkers;
	edgelRet.fReferenceLine = refedgel.fReferenceLine;

	return edgelRet;
}


//
// The CVisEdgeExtract class extract a list of edgels from an image
//

void CVisEdgeExtract::SetImage(CVisByteImage& input)
{
    m_input = input;
}

// These three functions should probably go into VisPyramid or VisConvolve:

template <class ImagePixelType>
static ImagePixelType Blur_121(ImagePixelType &img, int blur_steps)
{
    if (blur_steps > 0) {
        ImagePixelType blurred(img.Shape());    // new image
        for (int b = 0; b < (blur_steps & -2); b += 2)
            VisConvolveSeparable((b == 0) ? img : blurred, blurred,
                                   VisConvolutionKernel_14641,
                                   VisConvolutionKernel_14641, 1);
        if (b < blur_steps)
            VisConvolveSeparable((b == 0) ? img : blurred, blurred,
                                   VisConvolutionKernel_121,
                                   VisConvolutionKernel_121, 1);
        return blurred;
    } else
        return img;  // make a copy, sharing storage
}

template <class TypedImage>
void PyramidBlur(TypedImage& src, int n_levels)
{
    if (n_levels > 0) {
        CVisFloatImage &kernel = VisConvolutionKernel_14641;
        TypedImage reduced(src.Shape().Decimated(2));
        VisConvolveSeparable(src, reduced, kernel, kernel, 2);
        PyramidBlur(reduced, n_levels-1);
        VisConvolveSeparable(reduced, src, kernel, kernel, -2);
    }
}

template <class PIXELTYPE>
void
VisSubtractClip(CVisImage<PIXELTYPE> &src1, CVisImage<PIXELTYPE> &src2,
                CVisImage<PIXELTYPE> &dst, PIXELTYPE offset)
{
    VisCheckConformal(&src1, &src2, &dst, 0);
    int nc = dst.Width() * dst.NBands();
    for (int i=0; i < dst.Height(); i++) {
        PIXELTYPE *p1 = src1.PtrToFirstPixelInRow(i+src1.Top());
        PIXELTYPE *p2 = src2.PtrToFirstPixelInRow(i+src2.Top());
        PIXELTYPE *p3 =  dst.PtrToFirstPixelInRow(i+ dst.Top());
        for (int j=0; j < nc; j++) {
            int diff = p1[j] - p2[j];
            p3[j] = ((diff < -128) ? -128 : (diff > 127) ? 127 : diff) + offset;
        }
    }
}

#if 0
template <class TypedImage>
TypedImage DifferenceOfGaussian(TypedImage& src, int n_levels, int n_blur)
{
    TypedImage original = Blur_121(src, n_blur);
    TypedImage blurred(original.Shape());
    blurred.CopyPixels(original);
    PyramidBlur(blurred, n_levels);
    VisSubtractClip(original, blurred, blurred);
    return blurred;
}
#else
CVisByteImage DifferenceOfGaussian(CVisByteImage& src, int n_levels, int n_blur, int offset)
{
    CVisByteImage original = Blur_121(src, n_blur);
    CVisByteImage blurred(original.Shape());
    original.CopyPixelsTo(blurred);
    PyramidBlur(blurred, n_levels);
    VisSubtractClip(original, blurred, blurred, (unsigned char) offset);
    return blurred;
}
#endif

void CVisEdgeExtract::ExtractDoG(int n_level_diff, int n_blur,
                                 int margin, float min_strength)
{
    // Compute the Difference of Gaussians image
    CVisByteImage DoG = 
        DifferenceOfGaussian(m_input, n_level_diff, n_blur, 0);
#ifdef VIS_WRITE_DEBUG_FILES
    DoG.WriteFile("tmp_DoG.pgm");   // should look like sharp edges
#endif // VIS_WRITE_DEBUG_FILES

    // Scan through all quads looking for zero crossings
    //  Note that if we wanted to link the edges, we would need a 
    //  separate image of pointers to edgels...
    m_edgel_list.clear();       // empty out the list
    int width = DoG.Width();
    int height = DoG.Height();
    for (int y = margin; y < height-1-margin; y++) {
        char *p0 = (char *) DoG.PtrToFirstPixelInRow(y+DoG.Top()+0);
        char *p1 = (char *) DoG.PtrToFirstPixelInRow(y+DoG.Top()+1);
        for (int x = margin; x < width-1-margin; x++) {

            // Find the zero crossing, and if successful, push edgel
            CVisEdgel e;
            if (e.ZeroCrossing(p0[x], p0[x+1], p1[x], p1[x+1]) &&
                e.strength >= min_strength) {
                e.x += x, e.y += y;
				e.NormalizeN();
				e.ComputeTheta();
                m_edgel_list.push_back(e);
            }
        }
    }
}

void CVisEdgeExtract::NormalizeAndComputeTheta()
{
    for (int i = 0, n = m_edgel_list.size(); i < n; i++) {
        m_edgel_list[i].NormalizeN();
        m_edgel_list[i].ComputeTheta();
    }
}

void CVisEdgeExtract::CorrectRadialDistortion(float k1, float k2)
{
    float width  = m_input.Width();
    float height = m_input.Height();
    CVisRect shape  = m_input.Shape();
    float x0 = 0.5*(shape.left+shape.right-1);
    float y0 = 0.5*(shape.top+shape.bottom-1);
    for (int i = 0, n = m_edgel_list.size(); i < n; i++)
        m_edgel_list[i] =
        m_edgel_list[i].CorrectRadialDistortion(x0, y0, width, k1, k2);
}

//
// Draw the edgel list as PostScript file
//

void CVisEdgeExtract::PrintEdgels(char* filename)
{
    PrintEdgels(filename, m_edgel_list);
}

void CVisEdgeExtract::PrintEdgels(char* filename, CVisEdgelList& el)
{
    FILE* stream = fopen(filename, "w");
    if (stream == 0) {
        char *errmsg = "Could not open the file for writing";
        throw CVisFileIOError(filename, errmsg, eviserrorOpen,
                            "CVisEdgeExtract::PrintEdgels", __FILE__);
    }
    PrintHeader(stream);

    // Draw each edgel
    for (int i = 0, n = el.size(); i < n; i++) {
        CVisEdgel& e = el[i];
        fprintf(stream, "%.2f %.2f %.2f %.2f %.2f edgel\n",
                e.x, e.y, e.n_x, e.n_y, e.length);
    }

    // Print epilogue and close
    fprintf(stream, "\nshowpage\n");
    fclose(stream);
}

void CVisEdgeExtract::PrintEdgels(char* filename, CVisIndexCollection& ec)
{
    CVisEdgelList el;
    for (int i = 0, n = ec.size(); i < n; i++) {
        CVisIndexList& il = ec[i];
        for (int j = 0, m = il.size(); j < m; j++) {
            int k = il[j];
            el.push_back(m_edgel_list[k]);
        }
    }
    PrintEdgels(filename, el);
}

//
//  Some of this code is borrowed from PSFileIO.cpp:
//    modify the routines in that file so that we can share code...
//

static char *EPS_header = "\
%%!PS-Adobe-3.0 EPSF-3.0\n\
%%%%BoundingBox: %d %d %d %d\n\
%%%%Creator: MSVisSDK\n\
%%%%CreationDate: %s\n\
%%%%LanguageLevel: 2\n\
%%%%EndComments\n\
\n\
0.25 setlinewidth\n\
%d %d translate\n\
0 %d mul %d translate 1 -1 scale\n\
/edgel {\n\
0.5 mul /length exch def\n\
length mul /dy exch def\n\
length mul /dx exch def\n\
/y exch def /x exch def\n\
x dx sub y dy sub moveto\n\
x dx add y dy add lineto stroke\n\
} def\n\
\n";

static void PS_date(char *buf)
{
    // See on-line help for time() for alternative possibilities
    char buf1[9], buf2[9];
    /* Display operating system-style date and time. */
    _strdate(buf1);
    _strtime(buf2);
    sprintf(buf, "%s %s", buf1, buf2);
}
 
void CVisEdgeExtract::PrintHeader(FILE* stream)
{
    char datestr[128];
    PS_date(datestr);
    
    // Print encapsulated PostScript header
    int pwidth  = m_input.Width();
    int pheight = m_input.Height();
    int hoffset = ((85*72/10) - pwidth )/2;     // 8.5" wide page
    int voffset = ((11*72   ) - pheight)/2;     // 11"  high page
    fprintf(stream, EPS_header, hoffset, voffset,
            hoffset + pwidth, voffset + pheight, datestr,
            hoffset, voffset, pwidth, pheight);

}

//
// The CVisLineFit class is used to accumulate 2nd order moments of edgels
//

void CVisLineFit::Reset()
{
    m_il.clearout();
    m_l = 0.0;
}

CVisLineFit& CVisLineFit::operator+=(const CVisLineFit& a)
{
    m_l += a.m_l;
    m_il.insert(m_il.end(), a.m_il.begin(), a.m_il.end());
    return *this;
}

void CVisLineFit::Increment(const CVisEdgel& e, int i)
{
    m_il.push_back(i);
    m_l += e.length;
}

CVisEdgel CVisLineFit::Fit(const CVisEdgelList& el)
{
    CVisEdgel l;

    // Check for empty list
    if (m_il.empty() || m_l <= 0.0) {
        memset(&l, 0, sizeof(CVisEdgel));  // clear out everything...
        return l;
    }
    double il = 1.0 / m_l;

    // Accumulate the moments
    double lx = 0.0, ly = 0.0, lxx = 0.0, lxy = 0.0, lyy = 0.0;
    double ls = 0.0, lt = 0.0;
    for (int i = 0, n = m_il.size(); i < n; i++) {
        int j = m_il[i];
        const CVisEdgel& e = el[j];
        double l = e.length, x = e.x, y = e.y, s = e.strength, t = e.theta;
        lx += l*x, ly += l*y;
        lxx += l*x*x, lxy += l*x*y, lyy += l*y*y;
        ls += l*s, lt += l*t;
    }

    // Compute centroid, and subtract from second order statistics
    l.x = lx * il;
    l.y = ly * il;
    double xx = lxx * il - l.x * l.x;
    double xy = lxy * il - l.x * l.y;
    double yy = lyy * il- l.y * l.y;

    // Compute the average strength, theta, and sum of edgel lengths
    l.strength = ls * il;
    l.theta = lt * il;

    // Compute ellipsoid eigenvalues and eigenvectors
    double b  = xx + yy;
    double discr = (xx - yy)*(xx - yy) + 4.0 * xy * xy;
    discr = (discr > 0.0) ? sqrt(discr) : 0.0;  // for roundoff error...
    double s0 = 0.5*(b + discr);
    double s1 = 0.5*(b - discr);
    l.sigma = (s1 > 0.0) ? sqrt(s1) : 1.0e-20;
    if (fabs(xx - s0) > fabs(yy - s0))
        l.n_x = -xy, l.n_y = xx - s0;
    else
        l.n_x = s0 - yy, l.n_y =  xy;

    // Normalize to obtain direction, change sign if different from avg.
    l.NormalizeN();
    float ct = cos(M_PI/180*l.theta), st = sin(M_PI/180*l.theta);
    if (ct * l.n_x + st * l.n_y < 0.0)
        l.n_x = -l.n_x, l.n_y = -l.n_y;
    l.ComputeTheta();

    // Compute final summed edgel length and line length
    l.length = m_l;
    l.line_length = sqrt(6.0 * (s0 - s1));

    return l;
}

//
// The CVisHoughLineExtract class converts an CVisEdgelList into
//  a CVisLineList using a generalized Hough transform plus fitting.
//
CVisHoughLineExtract::CVisHoughLineExtract() :
      k1(0.0), k2(0.0)
{
}

CVisHoughLineExtract::~CVisHoughLineExtract()
{
    ResetAll();
}

CVisLineFit& CVisHoughLineExtract::Bin(int ir, int it)
{
    // Toroidally wrap the theta value, if necessary
    if (it < -m_n_t)
        it += 2*m_n_t;
    if (it >= m_n_t)
        it -= 2*m_n_t;
    // Clip r value, if necessary (only needed if radial corrected)
    if (ir < -m_n_r)
        ir += 2*m_n_r;
    if (ir >= m_n_r)
        ir -= 2*m_n_r;
    return *(CVisLineFit *) &m_Hough.Pixel(ir, it, 0);
}

void CVisHoughLineExtract::ResetAll()
{
    // Clear out the memory inside each bin
    for (int y = m_Hough.Top(); y < m_Hough.Bottom(); y++) {
        for (int x = m_Hough.Left(); x < m_Hough.Right(); x++) {
            Bin(x, y).Reset();
        }
    }
}
    
void CVisHoughLineExtract::Initialize(RECT& shape,
                                 int pixel_spacing, int angle_spacing)
{
    // Compute the extent and number of bins, and re-allocate Hough table
    m_x0 = 0.5*(shape.left+shape.right-1);
    m_y0 = 0.5*(shape.top+shape.bottom-1);
    m_width = shape.right - shape.left;
    m_r_s = 1.0 / pixel_spacing;
    m_n_r = int(0.5 * m_r_s * _hypot(shape.right - shape.left,shape.bottom - shape.top)) + 2;
    m_n_t = int(180 / angle_spacing);
    m_t_s = m_n_t / 180.0;
    int nbands = (sizeof(CVisLineFit) + sizeof(double)-1) / sizeof(double);
    CVisShape s(-m_n_r, -m_n_t, m_n_r, m_n_t, nbands);
    ResetAll();     // clear out any old lists
    m_Hough.Allocate(s);
    m_Hough.ClearPixels();
        // WARNING:  this relies on all 0 being a cleared state for vector<>
}

void CVisHoughLineExtract::Accumulate(CVisEdgelList& edgel_list)
{
    // Save the edgel list for later fitting
    m_edgel_list = edgel_list;

    // For every edgel, compute the bin and increment it
    ResetAll();
    for (int i = 0, n = edgel_list.size(); i < n; i++) {
        CVisEdgel& e = edgel_list[i];
        double r = (e.x - m_x0) * e.n_y - (e.y - m_y0) * e.n_x;
        double t = e.theta;
        int ir = int(floor(r * m_r_s + 0.5));
        int it = int(floor(t * m_t_s));
        Bin(ir, it).Increment(e, i);
    }

#ifdef VIS_WRITE_DEBUG_FILES
    static count_scale = 10.0;
    CVisByteImage counts = GetCounts(count_scale);
    counts.WriteFile("tmp_counts.pgm");
#endif // VIS_WRITE_DEBUG_FILES
}

CVisByteImage CVisHoughLineExtract::GetCounts(float scale)
{
    // This routine is really only here for debugging / visualization
    CVisByteImage counts(CVisRect(m_Hough.Shape()));
    for (int y = m_Hough.Top(); y < m_Hough.Bottom(); y++) {
        for (int x = m_Hough.Left(); x < m_Hough.Right(); x++) {
            int count = int(Bin(x, y).LengthSum() * scale + 0.5);
            counts.Pixel( x, y) = (count < 255) ? count : 255;
        }
    }
    return counts;
}

// This class is used to sort the bins by summed length counts

class CVisLineCntLoc {
public:
    double l;
    int ir, it;
    inline bool operator< (const CVisLineCntLoc& a) const {
        return l > a.l;   // want a decreasing order sort
    }
    inline bool operator== (const CVisLineCntLoc& a) const {
        return l == a.l;
    }
};
typedef std::vector<CVisLineCntLoc> CVisLineCntList;

void CVisHoughLineExtract::Extract(int nghbrs, bool collapse_opposites,
                              float min_length)
{
    // Create the list of lengths/locations, and sort it
    CVisLineCntList lcl;
    for (int it = m_Hough.Top(); it < m_Hough.Bottom(); it++) {
        for (int ir = m_Hough.Left(); ir < m_Hough.Right(); ir++) {
            CVisLineCntLoc lc;
            lc.l = Bin(ir, it).LengthSum(), lc.ir = ir, lc.it = it;
            if (lc.l > 0.0)
                lcl.push_back(lc);
        }
    }
    std::sort(lcl.begin(), lcl.end());

    // Walk through the list of sorted bins, clearing as we go
    m_line_list.clear();
    m_index_lists.clear();
	double dblLengthSum = 0.0;
    for (int i = 0; i < lcl.size(); i++) {

        // Test if non-empty
        CVisLineCntLoc& lc = lcl[i];
        CVisLineFit &ctr = Bin(lc.ir, lc.it);
        if (ctr.LengthSum() == 0.0)
            continue;

        // Optionally add in neighboring bins
        for (int it = lc.it-nghbrs; it <= lc.it+nghbrs; it++) {
            for (int ir = lc.ir-nghbrs; ir <= lc.ir+nghbrs; ir++) {
                if (it == lc.it && ir == lc.ir)
                    continue;
                CVisLineFit &nghbr = Bin(ir, it);
                ctr += nghbr;
                nghbr.Reset();  // clear out count
            }
        }

        // Optionally add in opposite polarity (flip sign of r)
        if (collapse_opposites)
        for (it = lc.it+m_n_t-nghbrs; it <= lc.it+m_n_t+nghbrs; it++) {
            for (int ir = -lc.ir-nghbrs; ir <= -lc.ir+nghbrs; ir++) {
                CVisLineFit &nghbr = Bin(ir, it);
                ctr += nghbr;
                nghbr.Reset();  // clear out count
            }
        }

        // Do the fit, and clear out the bin
        CVisEdgel& l = ctr.Fit(m_edgel_list);
        if (l.length > min_length) {
			dblLengthSum += l.length;
            m_line_list.push_back(l);
            m_line_list.back().cMarkers = 0;
            m_line_list.back().fReferenceLine = false;
            m_index_lists.push_back(ctr.IndexList());
        }
        ctr.Reset();
    }

	// Remove short lines.
	if (dblLengthSum > 0.0)
	{
		double dblLengthHalfAvg = 0.5 * (dblLengthSum / m_line_list.size());
		while (m_line_list.back().length < dblLengthHalfAvg)
		{
			m_line_list.pop_back();
			m_index_lists.back().clearout();  // UNDONE:  Is this needed?
			m_index_lists.pop_back();
		}
	}
}


//
//  Correct radial lens distortion by minimizing edgel to line
//  perpendicular distances (plumbline method).
//  See Rick's research notes, 21-Oct-98.
//
//  Basic equation to be solved is
//      k1 r^2 x.n + k2 r^4 x.n + a_i + b_i x.n_p = e_j
//

void CVisHoughLineExtract::EstimateRadialDistortion(CVisEdgelList& edgel_list,
                                                    float step_size)
{
    int n = m_index_lists.size();
    CVisDMatrix A(2*n+2, 2*n+2);
    CVisDVector b(2*n+2), k1k2abs(2*n+2);
    A = 0.0, b = 0.0;
    float Wi = 2.0 / m_width;

    // Iterate over the lines
    for (int i = 0; i< n; i++) {
		CVisEdgel& l = LineList()[i]; 
        CVisIndexList& il = m_index_lists[i];
        double n_x = l.n_x, n_y = l.n_y;             // line equation normal
        double di = (l.x - m_x0) * n_y - (l.y - m_y0) * n_x; // dist. to ctr.

        // Iterate over the constituent edgels
        for (int j = 0, m = il.size(); j < m; j++) {
            int k = il[j];
            CVisEdgel& e = edgel_list[k];
            double x  = e.x - m_x0, y = e.y - m_y0; // coord rel. to ctr.
            double xd = x*Wi, xd2 = xd*xd;
            double yd = y*Wi, yd2 = yd*yd;
            double r2 = xd2 + yd2;                  // squared radius
            double dj = x * n_y - y * n_x;          // dist along normal
            double eij= dj - di;                    // perp. dist to line
            double c0 = r2 * dj;                    // first coeff
            double c1 = r2 * c0;                    // second coeff
            double pj = x * n_x + y * n_y;          // dist along line

            // Form the normal equations
            double c[4] = {c0, c1, 1.0, pj};
            int indx[4] = {0, 1, 2*i+2, 2*i+3};
            for (int i0 = 0; i0 < 4; i0++) {
                int j0 = indx[i0];
                double v0 = c[i0];
                for (int i1 = 0; i1 < 4; i1++) {
                    int j1 = indx[i1];
                    double v1 = c[i1];
                    A[j0][j1] += v0 * v1;
                }
                b[j0] += v0 * eij;
            }
        }
    }

    // Solve the normal equations for k1 and k2
	VisDMatrixSolveSPD(A, b, k1k2abs); 
    k1 += step_size * (k1k2abs[0] - k1);
    k2 += step_size * (k1k2abs[1] - k2);
}

#ifdef OLD

void CVisHoughLineExtract::LineAssignment(CVisEdgelList& edgel_list)
{
	int n_lines = LineList().size(); 
	m_point_list = new CVisPointList[n_lines]; 
	int threshold = 3;  // only 8 neighbors
	m_used_edgels = 0;

	for (int j = 0; j < n_lines; j++) // for each line
	{
		CVisEdgel& e0 = LineList()[j]; 
		CVisPointList& c0 = m_point_list[j]; 
		double r0 = (e0.x - m_x0) * e0.n_y - (e0.y - m_y0) * e0.n_x; 
		double t0 = e0.theta; 
		int ir0 = int(floor(r0 * m_r_s + 0.5));
		int it0 = int(floor(t0 * m_t_s));

		// step through all edgels and find those close enough
		for (int i = 0, n = edgel_list.size(); i < n; i++) {
			CVisEdgel& e = edgel_list[i];
			double r = (e.x - m_x0) * e.n_y - (e.y - m_y0) * e.n_x;
			double t = e.theta;
			int ir = int(floor(r * m_r_s + 0.5));
			int it = int(floor(t * m_t_s));
			if ((ir0 - ir) * (ir0 - ir) + (it0 - it) * (it0 - it) < threshold)
				c0.push_back(i); 
		}
		m_used_edgels += c0.size(); 
	}
}

void CVisHoughLineExtract::SetupOneSystem(CVisEdgelList& edgel_list, int i, int startRow)
{
	// fill in partial derivatives in Ai(n x 2) Bi(n x 2) ei(n x 1)
	int n_points = m_point_list[i].size(); 
	
	// rd^2 = (xd - xc)^2 + (yd - yc)^2
	// s	= 1 + k1 rd^2 + k2 rd^4
	// xu = xd s
	// xu sin(theta) + yu cos(theta) = r    D.C. Brown
	// xu sin(theta') - yu cos(theta') = r	Rick's theta' = atan2(ny, nx); 
	// f(k1, k2, theta, r) = ( xd sin(theta) + yd cos(theta) ) s - r = 0; 
	// A1 = df/dk1 = ( xd sin(theta) + yd cos(theta) ) rd^2; 
	// A2 = df/dk2 = ( xd sin(theta) + yd cos(theta) ) rd^4; 
	// B1 = df/theta = s (xd cos(theta) - yd sin(theta));
	// B2 = -1; 

	double xc		= m_x0, yc = m_y0; 
	CVisEdgel& line = LineList()[i]; 
	double theta	= line.theta * M_PI / 180.0;			// using theta'
	double r		= line.r; 
	double sintheta = sin(theta), costheta = cos(theta); 
	double w2 = 1.0 / (80.0*80.0);  // half of width of image; for now

	for (int j = 0; j < n_points; j++)
	{
		int p = (m_point_list[i])[j]; 
		CVisEdgel& edgel = edgel_list[p]; 
		double xd = edgel.x, yd = edgel.y; 
		double rd2 = ((xd - xc) * (xd - xc) + (yd - yc) * (yd - yc)) * w2; 
		double rd4 = rd2 * rd2; 
		double s = 1.0 + k1 * rd2 + k2 * rd2 * rd2; 
		double temp1 = xd * sintheta - yd * costheta; 
		double temp2 = xd * costheta + yd * sintheta; 
		// copy into A and e properly
		int jj = j + startRow; 
		A[jj][0] = temp1 * rd2;
		A[jj][1] = temp1 * rd4; 
		A[jj][2*(i+1)] = s * temp2;
		A[jj][2*(i+1)+1] = -1; 
		e[jj] = r - s * temp1; 
	}
}

void CVisHoughLineExtract::SetupSystems(CVisEdgelList& edgel_list)
{
	int n_lines = LineList().size(); 

	int allpoints = 0; 
	for (int i = 0; i < n_lines; i++) // for each line
	{
		int n_points = m_point_list[i].size(); 
		SetupOneSystem(edgel_list, i, allpoints); 
		allpoints += n_points; 
	}

	ATA = A.Transposed() * A; 
	ATe = A.Transposed() * e; 

	VisDMatrixSolveSPD(ATA, ATe, dkdr); 
}

void CVisHoughLineExtract::CorrectDistortion(CVisEdgelList& edgel_list,
                                             int n_steps, float step_size)
{
    LineAssignment(edgel_list);
	// prepare
	int n_lines = LineList().size(); 
	int m = 2*n_lines + 2; 
	A.Resize(m_used_edgels, m); 
	A = 0.0; 
	e.Resize(m_used_edgels); 
	ATA.Resize(m, m); 
	ATe.Resize(m); 
	dkdr.Resize(m); 

	// initial r
	for (int i = 0; i < n_lines; i++)
	{
		CVisEdgel& line = LineList()[i]; 
		line.r = line.x * line.n_y - line.y * line.n_x; // or x sin(theta') - y cos(theta')
	}

	for (int steps = 0; steps < n_steps; steps++)
	{
		// solver
		SetupSystems(edgel_list); 

		// update parameters
		k1 += step_size * dkdr[0]; 
		k2 += step_size * dkdr[1]; 
		for (int i = 0; i < n_lines; i++)
		{
			CVisEdgel& line = LineList()[i]; 
			line.theta += step_size * dkdr[2*(i+1)] * 180 / M_PI; 
			line.r += step_size * dkdr[2*(i+1)+1]; 
		}
	}

	// cleanup 
	if (m_point_list)
		delete [] m_point_list; 
}
#endif

//
// The CVisVPFit class is used to accumulate outer products of line equations
//

void CVisVPFit::Reset()
{
    memset(this, 0, sizeof(CVisVPFit));
}

CVisVPFit& CVisVPFit::operator+=(CVisVPFit& a)
{
    m_count += a.m_count;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m_A[i][j] += a.m_A[i][j];
    return *this;
}

CVisVPFit& CVisVPFit::operator+=(CVisVector4& l)
{
    m_count += 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m_A[i][j] += l[i] * l[j];
    return *this;
}

CVisVector4 CVisVPFit::Fit(float f)
{
    CVisVector4 v(0, 0, 0, 0);
    if (m_count == 0.0)
        return v;

    // Compute smallest eigenvector of A
    CVisDMatrix Am(3, 3, &m_A[0][0]);
    CVisDVector vv(3, &v[0]);
    VisMinEigenValue(Am, vv);
    if (v[2] < 0.0)
        v *= -1;
    v[2] /= f;
    v[3] = m_count;
    return v;
}

//
// The CVisVanishingPoint class is used to compute vanishing points
// from a set of lines.
//

void CVisVanishingPoint::Initialize(RECT& shape,
                    float f, int n_bins,
                    float min_dot, float min_dist)
{
    // Compute the extent and number of bins, and re-allocate Hough table
    m_x0 = 0.5*(shape.left+shape.right-1);
    m_y0 = 0.5*(shape.top+shape.bottom-1);
    m_f  = f;
    m_n_bins   = n_bins;
    m_min_dot  = min_dot;
    m_min_dist = min_dist;
    int nbands = sizeof(CVisVPFit) / sizeof(double);
    m_Hough.Allocate(CVisShape(0, 0, n_bins, n_bins, nbands));
}

void CVisVanishingPoint::Accumulate(CVisEdgelList& line_list)
{
    // For every pair of edgels, compute the bin and increment it
    m_Hough.ClearPixels();
    for (int i = 0, n = line_list.size(); i < n; i++) {
        CVisEdgel& l0 = line_list[i];
        for (int j = i+1; j < n; j++) {
            CVisEdgel& l1 = line_list[j];
            Increment(l0, l1);
        }
    }

#ifdef VIS_WRITE_DEBUG_FILES
    static count_scale = 10.0;
    CVisByteImage counts = GetCounts(count_scale);
    counts.WriteFile("tmp_vp_counts.pgm");
#endif // VIS_WRITE_DEBUG_FILES
}

CVisVPFit& CVisVanishingPoint::Bin(int vx, int vy)
{
    // Toroidally wrap the (x,y) values, but flipping signs
    while (vx < 0)
        vx += m_n_bins, vy = m_n_bins-1 - vy;
    while (vx >= m_n_bins)
        vx -= m_n_bins, vy = m_n_bins-1 - vy;
    while (vy < 0)
        vy += m_n_bins, vx = m_n_bins-1 - vx;
    while (vy >= m_n_bins)
        vy -= m_n_bins, vx = m_n_bins-1 - vx;
    return *(CVisVPFit *) &m_Hough.Pixel(vx, vy, 0);
}

void CVisVanishingPoint::Increment(CVisEdgel& l0, CVisEdgel& l1)
{
    // Test if lines are parallel enough
    double dot = l0.n_x * l1.n_x + l0.n_y * l1.n_y;
    if (fabs(dot) < m_min_dot)
        return;

    // Test if lines are far apart enough
    double d0 = (l0.x - l1.x) * l0.n_y - (l0.y - l1.y) * l0.n_x;
    double d1 = (l0.x - l1.x) * l1.n_y - (l0.y - l1.y) * l1.n_x;
    if (fabs(d0) < m_min_dist || fabs(d1) < m_min_dist)
        return;

    // Form the two line equations and their cross product (vanishing pt)
    CVisVector4 e0(m_f  * l0.n_y,  -m_f * l0.n_x,
                   l0.y * l0.n_x - l0.x * l0.n_y);
    CVisVector4 e1(m_f  * l1.n_y,  -m_f * l1.n_x,
                   l1.y * l1.n_x - l1.x * l1.n_y);
	CVisVector4 v = e0.Crossed(e1);
    if (v[2] < 0.0)
        v *= -1;

    // Compute the bin as the (r,theta) mapping to square
    double r = _hypot(v[0], v[1]);
    double t = atan2(r, v[2]) / (M_PI / 2);  // in [0,1]
    double s = (r > 0.0) ? t / __max(fabs(v[0]), fabs(v[1])) : 0.0;
    v *= s;
    int vx = int(m_n_bins * 0.5 * (v[0] + 1.0));
    int vy = int(m_n_bins * 0.5 * (v[1] + 1.0));
    Bin(vx, vy) += e0;
    Bin(vx, vy) += e1;
}

CVisByteImage CVisVanishingPoint::GetCounts(float scale)
{
    // This routine is really only here for debugging / visualization
    CVisByteImage counts(CVisRect(m_Hough.Shape()));
    for (int y = m_Hough.Top(); y < m_Hough.Bottom(); y++) {
        for (int x = m_Hough.Left(); x < m_Hough.Right(); x++) {
            int count = int(Bin(x, y).Count() * scale + 0.5);
            counts.Pixel(x, y) = (count < 255) ? count : 255;
        }
    }
    return counts;
}

void CVisVanishingPoint::Extract(int nghbrs, int min_count)
{
    // Create the list of lengths/locations, and sort it
    CVisLineCntList lcl;
    for (int it = m_Hough.Top(); it < m_Hough.Bottom(); it++) {
        for (int ir = m_Hough.Left(); ir < m_Hough.Right(); ir++) {
            CVisLineCntLoc lc;
            lc.l = Bin(ir, it).Count(), lc.ir = ir, lc.it = it;
            if (lc.l > 0.0)
                lcl.push_back(lc);
        }
    }
    std::sort(lcl.begin(), lcl.end());

    // Walk through the list of sorted bins, clearing as we go
    for (int i = 0; i < lcl.size(); i++) {

        // Test if non-empty
        CVisLineCntLoc& lc = lcl[i];
        CVisVPFit &ctr = Bin(lc.ir, lc.it);
        if (ctr.Count() == 0)
            continue;

        // Optionally add in neighboring bins
        for (int it = lc.it-nghbrs; it <= lc.it+nghbrs; it++) {
            for (int ir = lc.ir-nghbrs; ir <= lc.ir+nghbrs; ir++) {
                if (it == lc.it && ir == lc.ir)
                    continue;
                CVisVPFit &nghbr = Bin(ir, it);
                ctr += nghbr;
                nghbr.Reset();  // clear out count
            }
        }

        // Do the fit, and clear out the bin
        CVisVector4 vp = ctr.Fit(m_f);
        if (vp[3] > min_count)
            m_vp_list.push_back(vp);
        ctr.Reset();
    }
}

void CVisVanishingPoint::EstimateF(float& f, float& confidence)
{
    // For the appropriate value of f, the two vanishing point,
    // with the last component scaled by f, should be orthogonal.
    // This routine should ONLY be called if the two vanishing points
    //  are not too far from the image center (within a few widths).
    if (m_vp_list.size() < 2) {
        f = confidence = 0.0;   // failure
        return;
    }
    CVisVector4& v0 = m_vp_list[0];
    CVisVector4& v1 = m_vp_list[1];
    double num = v0[0]*v1[0] + v0[1]*v1[1];
    double den = v0[2]*v1[2];
    if (num * den >= 0.0)
        f = confidence = 0.0;   // failure
    else    // bug in formula, fixed by Rick, 15-Oct-99
        m_f = f = sqrt(-num / den), confidence = sqrt(fabs(den));
}

bool CVisVanishingPoint::EstimateR(CVisTransform4x4& refpose,
		float fltXUp, float fltYUp, bool fUseWeightedAverage)
{
    // TODO: This routine should be re-written to use
    // an SVD, as in the VisComputeRotationFromF() routine.

	const double dblCloseDot = 0.8;  // Was sqrt(3.0) / 2.0, but we need a wider range.
	const double dblFarDot = sqrt(1 - dblCloseDot * dblCloseDot);

	CVisVector4 vectorUp(fltXUp, fltYUp, 0, 0);
	CVisVector4 vectorX(0, 0, 0, 0);
	CVisVector4 vectorY(0, 0, 0, 0);
	CVisVector4 vectorT(0, 0, 0, 0);
	CVisVector4 vectorZ(0, 0, 0, 0);

	if ((fltXUp == 0.0) && (fltYUp == 0.0))
	{
		vectorUp[0] = 0.0;
		vectorUp[1] = - 1.0;
	}
	vectorUp.Normalize();
	CVisVector4 vectorRight(fltYUp, - fltXUp, 0, 0);

	bool fFail = false;
	if (m_vp_list.size() < 2)
	{
		fFail = true;
	}
	else if (fUseWeightedAverage)
	{
		for (int iVP = 0; iVP < m_vp_list.size(); ++iVP)
		{
			assert(m_vp_list[iVP][3] > 0.0);

			double dblTmp = m_vp_list[iVP] * vectorUp;
			if (dblTmp > dblCloseDot)
			{
				vectorY[0] -= m_vp_list[iVP][0] * m_vp_list[iVP][3];
				vectorY[1] -= m_vp_list[iVP][1] * m_vp_list[iVP][3];
				vectorY[2] -= m_vp_list[iVP][2] * m_vp_list[iVP][3];
				vectorY[3] += m_vp_list[iVP][3];
			}
			else if (dblTmp < - dblCloseDot)
			{
				vectorY[0] += m_vp_list[iVP][0] * m_vp_list[iVP][3];
				vectorY[1] += m_vp_list[iVP][1] * m_vp_list[iVP][3];
				vectorY[2] += m_vp_list[iVP][2] * m_vp_list[iVP][3];
				vectorY[3] += m_vp_list[iVP][3];
			}
			else if ((dblTmp > - dblFarDot) && (dblTmp < dblFarDot))
			{
				if (m_vp_list[iVP] * vectorX >= 0)
				{
					vectorX[0] += m_vp_list[iVP][0] * m_vp_list[iVP][3];
					vectorX[1] += m_vp_list[iVP][1] * m_vp_list[iVP][3];
					vectorX[2] += m_vp_list[iVP][2] * m_vp_list[iVP][3];
				}
				else
				{
					vectorX[0] -= m_vp_list[iVP][0] * m_vp_list[iVP][3];
					vectorX[1] -= m_vp_list[iVP][1] * m_vp_list[iVP][3];
					vectorX[2] -= m_vp_list[iVP][2] * m_vp_list[iVP][3];
				}
				vectorX[3] += m_vp_list[iVP][3];
			}
			else
			{
				if (m_vp_list[iVP] * vectorT >= 0.0)
				{
					vectorT[0] += m_vp_list[iVP][0] * m_vp_list[iVP][3];
					vectorT[1] += m_vp_list[iVP][1] * m_vp_list[iVP][3];
					vectorT[2] += m_vp_list[iVP][2] * m_vp_list[iVP][3];
				}
				else
				{
					vectorT[0] -= m_vp_list[iVP][0] * m_vp_list[iVP][3];
					vectorT[1] -= m_vp_list[iVP][1] * m_vp_list[iVP][3];
					vectorT[2] -= m_vp_list[iVP][2] * m_vp_list[iVP][3];
				}
				vectorT[3] += m_vp_list[iVP][3];
			}
		}

		if (vectorX[3] == 0.0)
		{
			// This can happen.
			if (vectorT[0] * vectorUp[1] >= vectorT[1] * vectorUp[0])
				vectorX = vectorT;
			else
				vectorX = - vectorT;
		}
		else if (vectorY[3] == 0.0)
		{
			// This can happen.
			if (vectorT * vectorUp >= 0)
				vectorY = - vectorT;
			else
				vectorY = vectorT;
		}

		if ((vectorX[3] != 0.0) && (vectorY[3] != 0.0))
		{
			// Divide by weights to get weighted averages.
			vectorX /= vectorX[3];
			vectorY /= vectorY[3];
		}
		else
		{
			fFail = true;
		}
	}
	else
	{
		vectorX = m_vp_list[0];
		for (int i = 1; ((i < m_vp_list.size())
				&& (fabs(vectorX[0] * m_vp_list[i][0] + vectorX[1] * m_vp_list[i][1])
						>= dblCloseDot)); ++i)
		{
			// Loop
		}

		if (i < m_vp_list.size())
			vectorY = m_vp_list[i];
		else
			fFail = true;
	}

	if (!fFail)
	{
		assert(vectorX.MagnitudeSquared() > 0);
		assert(vectorY.MagnitudeSquared() > 0);

		// Remember the confidence that we have in X and Y.
		double dblWeightX = vectorX[3];
		double dblWeightY = vectorY[3];

		// Scale by the focal length
		vectorX[2] *= m_f;
		vectorY[2] *= m_f;

		// Normalize.
		vectorX.Normalize();
		vectorY.Normalize();

		// Check orientation.  Adjust if needed.
		if (fabs(vectorX * vectorUp) > fabs(vectorY * vectorUp))
		{
			// Switch x and y.
			vectorT = vectorX;
			vectorX = vectorY;
			vectorY = vectorT;
			double dblT = dblWeightX;
			dblWeightX = dblWeightY;
			dblWeightY = dblT;
		}
		if (vectorX * vectorRight < 0)
			vectorX *= - 1.0;
		if (vectorY * vectorUp > 0)
			vectorY *= - 1.0;

		// Find z.
		vectorZ = vectorX.Crossed(vectorY);
		vectorZ.Normalize();

		// Make X and Y orthogonal
		CVisVector4 vectorXPrime = vectorY.Crossed(vectorZ);
		CVisVector4 vectorYPrime = vectorZ.Crossed(vectorX);
		vectorT = vectorX * dblWeightX + vectorXPrime * dblWeightY;
		vectorY = vectorYPrime * dblWeightX + vectorY * dblWeightY;
		vectorX = vectorT;

		// Normalize after making X and Y orthogonal
		vectorX.Normalize();
		vectorY.Normalize();

#ifdef VIS_OLD_CODE
		// Do we have more confidence in X or Y?
		bool fTrustX = (dblWeightX >= dblWeightY);

		// UNDONE:  Should we use weights here to modify both vectors?
		if (fTrustX)
		{
			vectorY = vectorZ.Crossed(vectorX);
		}
		else
		{
			vectorX = vectorY.Crossed(vectorZ);
		}
#endif // VIS_OLD_CODE

		// Copy result into refpose
		memcpy(refpose[0], &vectorX[0], 3 * sizeof(double));
		memcpy(refpose[1], &vectorY[0], 3 * sizeof(double));
		memcpy(refpose[2], &vectorZ[0], 3 * sizeof(double));

		refpose[0][3] = 0.0;
		refpose[1][3] = 0.0;
		refpose[2][3] = 0.0;

		refpose[3][0] = 0.0;
		refpose[3][1] = 0.0;
		refpose[3][2] = 0.0;
		refpose[3][3] = 1.0;
	}

    return (!fFail);
}
