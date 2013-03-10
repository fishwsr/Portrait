///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisHistogramEq.h -- locally equalize image histogram
//
// DESCRIPTION
//  CVisHistogramEqualizer is used to loclly equalize the histogram of
//  pixel distributions in an image.
//
// SEE ALSO
//  VisHistogramEq.h    more detailed documentation
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  02-Sep-98  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
//  Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "VisImageProcPch.h"
#include <VisImageProc.h>
#include "VisHistogramEq.h"

//
//  Compute the number of patches and allocate the histogram
//

void CVisHistogramEqualizerBase::InitializeHistograms(const CVisShape& s,
                                                      int n_gray_levels)
{
    if (m_patch_size < 4)
        m_patch_size = __max(s.Width(), s.Height());

    // Initialize the 2-D array of histograms
    m_x_patch = (s.Width()  + m_patch_size-1) / m_patch_size;
    m_y_patch = (s.Height() + m_patch_size-1) / m_patch_size;
    m_pad     = __max(2, (m_interp_order + 1) / 2);
    CVisShape s2(-m_pad, -m_pad, m_x_patch+m_pad, m_y_patch+m_pad,
                 n_gray_levels);
    m_histogram.Allocate(s2);
}

// TODO: This code should go in VisConvolve.inl, since it's shared
//  in several places (VisAlign.cpp, VisLineExtract.cpp)
// Note that this version destructively modifies the contents of img.

template <class TImage>
static void Blur_121(TImage &img, int blur_steps)
{
    for (int b = 0; b < (blur_steps & -2); b += 2)
        VisConvolveSeparable(img, img,
                             VisConvolutionKernel_14641,
                             VisConvolutionKernel_14641, 1);
    if (b < blur_steps)
        VisConvolveSeparable(img, img,
                             VisConvolutionKernel_121,
                             VisConvolutionKernel_121, 1);
}

void CVisHistogramEqualizerBase::SmoothHistograms()
{
    if (m_interp_order == 0 && m_blur_steps == 0)
        return;

    // Copy boundary patches to extrapolate tables
    CVisShape s2(0, 0, m_x_patch, m_y_patch);
    CVisIntImage h2 = m_histogram.SubImage(s2);
    const CVisShape& s = m_histogram.Shape();
#if 1   // Use Pad() routine
    h2.Pad(evispadReplicate, s);
#else
    int size = m_histogram.NBands() * sizeof(int);
    for (int y = 0; y < m_y_patch; y++) {
        for (int i = 0; i < m_pad; i++) {
            memcpy(&m_histogram.Pixel(0          -i, y, 0),
                   &m_histogram.Pixel(0            , y, 0), size);
            memcpy(&m_histogram.Pixel(m_x_patch-1+i, y, 0),
                   &m_histogram.Pixel(m_x_patch-1  , y, 0), size);
        }
    }
    for (int x = s.Left(); x < s.Right(); x++) {
        for (int i = 0; i < m_pad; i++) {
            memcpy(&m_histogram.Pixel(x, 0          -i, 0),
                   &m_histogram.Pixel(x, 0            , 0), size);
            memcpy(&m_histogram.Pixel(x, m_y_patch-1+i, 0),
                   &m_histogram.Pixel(x, m_y_patch-1  , 0), size);
        }
    }
#endif

    // Smooth the histograms using an iterated 1_2_1 blur
    Blur_121(h2, m_blur_steps);

    // One extra blur step for linear filtering (add neighboring cells)
    if (m_interp_order & 1) {
        CVisShape s1(s.Left(), s.Top(), s.Right()-1, s.Bottom()-1);
        CVisIntImage h1 = m_histogram.SubImage(s1);
        VisConvolveSeparable(h1, h1,
                             VisConvolutionKernel_11,
                             VisConvolutionKernel_11, 1);
    }
}

//
// Create a viewable histogram image.
//  Used for illustrative purposes (i.e., to generate figures)
//

void CVisHistogramEqualizerBase::CreateHistogramImage(int patch_x, int patch_y)
{
    static int height = 256;
    int n = m_histogram.NBands();
    m_hist_image.Allocate(n, height);
    m_hist_image.FillPixels(255);           // white
    int *hist = &m_histogram.Pixel(patch_x, patch_y, 0);
    for (int i = 0, max = 0; i < n; i++) {
        if (max < hist[i])
            max = hist[i];
    }
    for (i = 0; i < n; i++) {
        int y = (height-1) - hist[i] * (height-1) / max;    // bottom to top
        m_hist_image.Pixel(i, y) = 0;       // black
    }
}

CVisByteImage& CVisHistogramEqualizerBase::HistogramImage()
{
    return m_hist_image;
}

//
// For each histogram, create a lookup table using a blend
// of the cumulative distribution function and a ramp
//

void CVisHistogramEqualizerBase::CreateLookUpTable(int lut[], int n)
{
    // Compute the cumulative distribution function
    for (int i = 1; i < n; i++)
        lut[i] += lut[i-1];

    if (lut[n-1] <= 0)
        return;
    double scale1 = 255.0 / lut[n-1], scale2 = 256.0 / n;
    double a1 = m_equalize * scale1, a2 = (1.0 - m_equalize) * scale2;

    // Blend the cumulative distr. function with a plain ramp
    for (i = 0; i < n; i++)
        lut[i] = int(a1 * lut[i] + a2 * i);
}

void CVisHistogramEqualizerBase::CreateLookUpTables()
{
    // For each histogram, create a lookup table
    for (int y = m_histogram.Top(); y < m_histogram.Bottom(); y++) {
        for (int x = m_histogram.Left(); x < m_histogram.Right(); x++) {
            CreateLookUpTable(&m_histogram.Pixel(x, y, 0), m_histogram.NBands());
        }
    }
}

//
//  Accumulate the histogram entries (treat R,G,B as separate bands)
//

template <class TPixel>
static void VisAccumulatePatch(CVisImage<TPixel>& img,
                               int histogram[], bool ignore_4th)
{
    for (int y = img.Top(); y < img.Bottom(); y++) {
        TPixel *p = &img.FirstPixelInRow(y);
        int ncols = img.Width() * img.NBands();
        for (int x = 0; x < ncols; x++)
            if (! ignore_4th || (x & 3) != 3)
                histogram[p[x]] += 1;
    }
}

static void VisAccumulatePatch(CVisRGBAByteImage& img, int histogram[], bool ignore_4th)
{
    CVisByteImage gimg;
	gimg.Alias(img);
    VisAccumulatePatch(gimg, histogram, true);
}

static void VisAccumulatePatch(CVisRGBAUShortImage& img, int histogram[], bool ignore_4th)
{
    CVisUShortImage gimg;
	gimg.Alias(img);
    VisAccumulatePatch(gimg, histogram, true);
}

template <class TPixel>
void CVisHistogramEqualizer<TPixel>::AccumulateHistograms(const CVisImage<TPixel>& inimg)
{
    CVisShape s = inimg.Shape();

    for (int y = 0; y < m_y_patch; y++) {
        for (int x = 0; x < m_x_patch; x++) {
            CVisShape p1(CVisPoint(s.Left()+x*m_patch_size, s.Top()+y*m_patch_size),
                         CVisSize(m_patch_size, m_patch_size), inimg.NBands());
            p1 &= s;    // clip to image size
            VisAccumulatePatch(inimg.SubImage(p1), &m_histogram.Pixel(x,y,0), false);
        }
    }
}

//
//  Apply the normalized cumulative histogram table to the pixel values
//

static inline float LERP(float v0, float v1, float u)
{
    return v0 + (v1 - v0)*u;
}

static inline float QERP(float v0, float v1, float v2, float u)
{
    return 0.5f*(v0 + v1) + (v1 - v0)*u + 0.5f*(v0 - 2.0f*v1 + v2)*(u*u);
}

template <class TPixel>
static void VisApplyLUT(CVisImage<TPixel>& inimg,
                        CVisImage<TPixel>& outimg,
                        CVisIntImage& histogram, int xp, int yp,
                        int interp_order, int patch_size, bool ignore_4th)
{
    for (int y = inimg.Top(); y < inimg.Bottom(); y++) {
        const TPixel *p = &inimg.FirstPixelInRow(y);
        TPixel *q = &outimg.FirstPixelInRow(y);
        int ncols = inimg.Width() * inimg.NBands();
        if (interp_order == 0) {
            int *h00 = &histogram.Pixel(xp, yp, 0);
            for (int x = 0; x < ncols; x++)
                q[x] = (ignore_4th && (x & 3) == 3) ? 255 : h00[p[x]];
        } else {
            int *hmm = &histogram.Pixel(xp-1, yp-1, 0);
            int *h0m = &histogram.Pixel(xp  , yp-1, 0);
            int *hpm = &histogram.Pixel(xp+1, yp-1, 0);
            int *hm0 = &histogram.Pixel(xp-1, yp  , 0);
            int *h00 = &histogram.Pixel(xp  , yp  , 0);
            int *hp0 = &histogram.Pixel(xp+1, yp  , 0);
            int *hmp = &histogram.Pixel(xp-1, yp+1, 0);
            int *h0p = &histogram.Pixel(xp  , yp+1, 0);
            int *hpp = &histogram.Pixel(xp+1, yp+1, 0);
            int nbands = inimg.NBands();
            float n1 = 1.0/patch_size, yf = (y-inimg.Top())*n1, xf = 0.0;
            for (int x = 0, b = 0; x < ncols; x++, b++) {
                int v = p[x];
                if (ignore_4th && (x & 3) == 3)
                    q[x] = 255;
                else if (interp_order == 1)
                    q[x] = int(LERP(LERP(hmm[v], h0m[v], xf),
                                    LERP(hm0[v], h00[v], xf), yf) + 0.5);
                else
                    q[x] = int(QERP(QERP(hmm[v], h0m[v], hpm[v], xf),
                                    QERP(hm0[v], h00[v], hp0[v], xf),
                                    QERP(hmp[v], h0p[v], hpp[v], xf), yf) + 0.5);
                if (b == nbands-1)
                    xf += n1, b = -1;
            }
        }
    }
}

static void VisApplyLUT(CVisRGBAByteImage& inimg, CVisRGBAByteImage& outimg,
                        CVisIntImage& histogram, int xp, int yp,
                        int interp_order, int patch_size, bool ignore_4th)
{
    CVisByteImage ginimg, goutimg;
	ginimg.Alias(inimg);
	goutimg.Alias(outimg);
    VisApplyLUT(ginimg, goutimg, histogram, xp, yp,
                interp_order, patch_size, true);
}

static void VisApplyLUT(CVisRGBAUShortImage& inimg, CVisRGBAUShortImage& outimg,
                        CVisIntImage& histogram, int xp, int yp,
                        int interp_order, int patch_size, bool ignore_4th)
{
    CVisUShortImage ginimg, goutimg;
	ginimg.Alias(inimg);
	goutimg.Alias(outimg);
    VisApplyLUT(ginimg, goutimg, histogram, xp, yp,
                interp_order, patch_size, true);
}

template <class TPixel>
void CVisHistogramEqualizer<TPixel>::ComputeEqualizedImage(const CVisImage<TPixel>& inimg,
                                                           CVisImage<TPixel>& outimg)
{
    if (m_interp_order < 0 || m_interp_order > 2)
        m_interp_order = 0;

    // Iterate over the patches
    CVisShape s = inimg.Shape();
    for (int y = 0; y < m_y_patch; y++) {
        for (int x = 0; x < m_x_patch; x++) {
            CVisShape p1(CVisPoint(s.Left()+x*m_patch_size, s.Top()+y*m_patch_size),
                         CVisSize(m_patch_size, m_patch_size), inimg.NBands());
            p1 &= s;    // clip to image size
            VisApplyLUT(inimg.SubImage(p1), outimg.SubImage(p1),
                        m_histogram, x, y, m_interp_order, m_patch_size, false);
        }
    }
}

//
//  Apply all the processing steps: initialize, accumulate, smooth, look-up
//

template <class TPixel>
void CVisHistogramEqualizer<TPixel>::Equalize(const CVisImage<TPixel>& inimg,
        CVisImage<TPixel>& outimg, int n_gray_levels,
        int create_histogram_image)
{
    // Initialize, accumulate, and then compute output image
    InitializeHistograms(inimg.Shape(), n_gray_levels);
    AccumulateHistograms(inimg);
    SmoothHistograms();
    if (create_histogram_image == 1)    // histogram
        CreateHistogramImage();
    CreateLookUpTables();
    if (create_histogram_image == 2)    // cumulative distribution
        CreateHistogramImage();
    if (outimg.Shape() != inimg.Shape())
        outimg.Allocate(inimg.Shape());
    ComputeEqualizedImage(inimg, outimg);
}

//
//  This routine used only to generate sample 2-D histograms for paper...
//

struct Vis2DHistAcc {
    Vis2DHistAcc(CVisIntImage& h2d, int ignore_same) : 
        m_h2d(h2d), m_max(0), m_ignore_same(ignore_same) {}
    void operator()(unsigned char p0, unsigned char p1)
    {
        if (m_ignore_same && p0 == p1)
            return;
        int &h = m_h2d.Pixel(p0, 255 - p1); // flip top to bottom
        h += 1;
        if (h > m_max && (p0 != 255 || p1 != 255))
            m_max = h;
    }
    CVisIntImage& m_h2d;
    int m_max, m_ignore_same;
};

struct Vis2DHistScale {
    Vis2DHistScale(int max) : m_max(max) {}
    void operator()(int hI, unsigned char& hB) {
        hB = __max(0, 255 - 255 * hI / m_max);
    }
    int m_max;
};

CVisByteImage VisHistogram2D(CVisRGBAByteImage& img1,
                             CVisRGBAByteImage& img2, 
                             bool ignore_same, double gain)
{
    CVisByteImage bimg1, bimg2;
    bimg1.Alias(img1), bimg2.Alias(img2);   // convert to 4-band
    CVisIntImage histI(256, 256);
    histI.ClearPixels();
    Vis2DHistAcc h2da(histI, ignore_same);
    VisMap2(h2da, bimg1, bimg2);            // accumulate and find max
    histI.Pixel(255, 255) -= img1.Width() * img1.Height();  // remove A band
    CVisByteImage histB(256, 256);
    Vis2DHistScale h2ds(int(h2da.m_max / gain));
    VisMap2(h2ds, histI, histB);
    return histB;
}

//
//  To form a Multi-image histogram equalization ("auto bracketing"),
//  we first convert a collection of images (sequence) into a summed
//  brightness image (sum each color channel independently).
//

inline void operator+=(CVisRGBAUShortPixel& acc, CVisRGBABytePixel& in)
{
    acc.SetRGBA(acc.R()+in.R(), acc.G()+in.G(), acc.B()+in.B(), 255);
}

template <class TPixel1, class TPixel2>
struct CVisIncrementPixel
{
    inline void operator()(TPixel1& acc, TPixel2& in)
        { acc += in; }
};

template <class TPixel1, class TPixel2>
static void VisSumImages(const CVisSequence<TPixel1>& seq,
                         CVisImage<TPixel2>& sum)
{
    sum.Allocate(seq[0].Shape());
    sum.ClearPixels();
    for (int i = 0; i < seq.Length(); i++)
	{
        VisMap2(CVisIncrementPixel<TPixel2,TPixel1>(), sum,
				const_cast<CVisSequence<TPixel1>&>(seq)[i]);
	}
}

static inline void VisAssign(CVisRGBABytePixel& dst, CVisRGBAUShortPixel& src)
{
    dst.SetRGBA(src.R(), src.G(), src.B(), 255);
}

template <class TPixel1, class TPixel2>
static inline void VisAssign(TPixel1& dst, TPixel2& src)
{
    dst = src;
}

template <class TPixel1, class TPixel2>
struct CVisCopyPixel
{
    inline void operator()(TPixel1& src, TPixel2& dst)
        { VisAssign(dst, src); }
};

template <class TPixel, class TPixelSum>
void CVisMultiHistogramEqualizer<TPixel, TPixelSum>::MultiEqualize(
    const CVisSequence<TPixel>& inseq, CVisImage<TPixel>& outimg,
    int n_gray_levels)
{
    // Form the summed image
    CVisImage<TPixelSum> sum;
    VisSumImages(inseq, sum);

    // Perform the equalization on the summed brightness
    Equalize(sum, sum, n_gray_levels * inseq.Length());

    // Convert back into a regular image
    if (sum.Shape() != outimg.Shape())
        outimg.Allocate(sum.Shape());
    VisMap2(CVisCopyPixel<TPixelSum,TPixel>(), sum, outimg);
}


//  Force template instantiation

template CVisHistogramEqualizer<CVisRGBABytePixel>;
template CVisMultiHistogramEqualizer<CVisRGBABytePixel, CVisRGBAUShortPixel>;