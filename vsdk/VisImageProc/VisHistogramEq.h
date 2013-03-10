///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisHistogramEq.h -- locally equalize image histogram
//
// DESCRIPTION
//  CVisHistogramEqualizer is used to loclly equalize the histogram of
//  pixel distributions in an image.
//
//  The image is broken up into patches, and the gray-level distribution
//  is computed in each patch (each R,G,B band contributes as an independent
//  gray level).  Patch histograms are then blurred to achieve some
//  spatial smoothness in the distribution.
//
//  A cumulative histogram is then formed in each patch, and then
//  optionally blended with a linear ramp to form the final compensation
//  function (a look-up table per patch).
//
//  The final original to final gray-level mapping is performed using these
//  LUTs, with optional bilinear or biquadratic interpolation of the LUT
//  values.
//
//  CVisMultiHistogramEqualizer takes a collection of images, taken at
//  different exposures ("bracketed"), and computes a mapping from the 
//  tuples of pixel values to a final equalized color.  In the current
//  implementation, the N input images are first summed to provide
//  "summed colors", on the assumption that these summed values are
//  monotonically related to the actual incident radience (this should
//  be true if each image's transfer function is itself monotonic).
//  These summed colors are then input to the VisHistogramEq class.
//
// CLASS PARAMTERS (PUBLIC MEMBER VARIABLES)
//  equalize        1.0 = full histogram equalization, 0 = no equalization
//  patch_size      size (in pixels) of local analysis patch (<= 0: single patch)
//  blur_iter       # of blur steps in smoothing analysis results
//  interp_order    interpolation of LUTs: 0=constant, 1=bilinear, 2=bi-quadratic
//
// SEE ALSO
//  VisHistogramEq.cpp      implementation code
//
//  Rick Szeliski's research notes, 2-Sep-98.
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

#ifndef __VIS_HISTOGRAM_EQUALIZE_H__
#define __VIS_HISTOGRAM_EQUALIZE_H__

struct CVisHistogramEqualizerParameters {
    double m_equalize;  // 1.0 = full histogram equalization, 0 = no equalization
    int m_patch_size;   // size (in pixels) of local analysis patch (<= 0: single patch)
    int m_blur_steps;   // # of blur steps in smoothing analysis results
    int m_interp_order; // interpolation of LUTs: 0=constant, 1=bilinear, 2=bi-quadratic

    CVisHistogramEqualizerParameters() :   // reasonable default values
        m_equalize(1.0), m_patch_size(64), 
        m_blur_steps(2), m_interp_order(2) {}
    void SetParam(const CVisHistogramEqualizerParameters& p)
        { *this = p; }
};

class CVisHistogramEqualizerBase : public CVisHistogramEqualizerParameters
{
public:
    CVisByteImage& HistogramImage();
protected:
    void InitializeHistograms(const CVisShape& s, int n_gray_levels);
    void SmoothHistograms();
    void CreateHistogramImage(int patch_x = 0, int patch_y = 0);
    void CreateLookUpTable(int lut[], int n);
    void CreateLookUpTables();

    int m_x_patch, m_y_patch;   // number of patches (x and y dimensions)
    int m_pad;                  // width of border patches
    CVisIntImage m_histogram;   // histograms for each patch
    CVisByteImage m_hist_image; // histogram image (for single patch)
};

template <class TPixel>
class CVisHistogramEqualizer : public CVisHistogramEqualizerBase
{
public:
    void Equalize(const CVisImage<TPixel>& inimg,
                  CVisImage<TPixel>& outimg,
                  int n_gray_levels = 256,
                  int create_histogram_image = 0);
protected:
    void AccumulateHistograms(const CVisImage<TPixel>& inimg);
    void ComputeEqualizedImage(const CVisImage<TPixel>& inimg,
                               CVisImage<TPixel>& outimg);
};

// This routine used to generate 2D histogram for illustrative purposes...
CVisByteImage VisHistogram2D(CVisRGBAByteImage& img1,
                             CVisRGBAByteImage& img2,
                             bool ignore_same = false, double gain = 1.0);

template <class TPixel, class TPixelSum>
class CVisMultiHistogramEqualizer : public CVisHistogramEqualizer<TPixelSum>
{
public:
    void MultiEqualize(const CVisSequence<TPixel>& inseq,
                       CVisImage<TPixel>& outimg,
                       int n_gray_levels = 256);
};

typedef CVisHistogramEqualizer<CVisRGBABytePixel>   CVisColorEqualizer;
typedef CVisHistogramEqualizer<unsigned char>       CVisMonochromeEqualizer;

typedef CVisMultiHistogramEqualizer<CVisRGBABytePixel, CVisRGBAUShortPixel>
    CVisColorMultiEqualizer;
typedef CVisMultiHistogramEqualizer<unsigned char, unsigned short>
    CVisMonochromeMultiEqualizer;

#endif // __VIS_HISTOGRAM_EQUALIZE_H__
