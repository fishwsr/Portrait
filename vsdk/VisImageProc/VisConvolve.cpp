///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisConvolve.cpp -- generic image convolution implementation
//  
// DESCRIPTION
//  These routines are used for convolving images with a specified
//  kernel.  The kernel itself is specified as a small image.  The
//  origin of the kernel (0,0) should also be the origin of the image,
//  e.g., for a (0.25, 0.5, 0.25) symmetric kernel, the image origin
//  (start_row) should be 1.
//
//  The separable image convolution provides a more efficient
//  implementation for separable kernels.  It can also be used for
//  pure horizontal or vertical convolutions by supplying a null
//  image (Image()) for the unused convolution kernel.  Separable
//  image filtering is made more efficient by computing both convolutions
//  together for each output row.
//
// SEE ALSO
//  VisConvolve.h    full description of convolution routines
//
// BUGS
//  When compiling in Release version (/O2), the CVisRGBAByteImage and CVisFloatImage
//  versions are not properly instantiated unless dummy code is added
//  to VisConvolveSeparable.
//
// DESIGN
//  If the source image does not have enough padding so that it can
//  be addressed directly inside the convolution loop, then a temporary
//  image (with undefined pad values) will be created.  This situation
//  should be avoided by first pre-padding the image in the desired way.
//
//  The condition as to whether the VisCore.has sufficient pixels is
//  for all (k,l) in kernel  (i.e., Top()..Bottom() - 1, Left()..Right() - 1)
//    for all (i,j) in src (i.e., Top()..Bottom() - 1, Left()..Right() - 1)
//      (i+k,j+l) is an addressable pixel
//          [src.MemoryRect().top..src.MemoryRect().top+src.MemoryHeight()-1]
//
//	The use of subsampling together with multi-band images may lead to
//	less efficient code than other situations.  This is because the inner
//	convolution loops do not explicitly loop over bands.  Instead, the
//	following three common situations are supported (see VisConvolveRow):
//		1. performing a vertical convolution, o (offset) = inter-row
//			offset, stride = 1 (since need dense results)
//		2. performing a horizontal convolution, single band, o = 1,
//			stride can be arbitrary (subsampling >= 1)
//		3. performing a horizontal convolution, multi-band, o = nbands,
//			stride = 1
//	If we are performing subsampling with a multi-band image, and we pass
//	o = nbands, and stride = subsampling, then only one band gets evaluated.
//	When this situation occurs, we can either perform a dense convolution and
//	subsample later, or call VisConvolveRow for each pixel.
//
//  The original separable convolution design interleaved horizontal and
//  vertical convolutions in order to minimize memory bandwidth (writing
//  then reading the intermediate results).  However, this made the code
//  much more hairy, so I have adopted a simpler two-pass strategy.
//  The performance penalty should be quite small for reasonable kernels.
//  If performance becomes an issue, then consider dividing up the source
//  and destination images into swaths (this could be added to the
//  top-level routine).
//
// HISTORY
//  25-Mar-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
//  20-May-96  Richard Szeliski (szeliski) at Microsoft
//  Removed templated coefficient type (always use float kernel)
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////



// UNDONE:  Do the RGBA byte aritmetic operators work right here, or do they
// need to be changed to return a different intermediate type?


#include "VisImageProcPch.h"
#include "VisImageProc.h"


//#define max(a,b)  (((a) > (b)) ? (a) : (b))
//#define min(a,b)  (((a) < (b)) ? (a) : (b))


#include "VisConvolve.h"

//
//  Round by adding 0.5 to all integral types
//   (this doesn't work correctly with signed ints, since we aren't
//    invoking floor()).
//

template <class TPixel>
static float RoundAmount(TPixel x) { return 0.5f; }

static float RoundAmount(float x) { return 0.0f; }
static float RoundAmount(double x) { return 0.0f; }

EVisPad g_evispadConvolveEnlarge = evispadReplicate;

//
// Convolve a row (o determines whether horizontal or vertical)
//

template <class TPixel>
void VisConvolveRow(TPixel *src, TPixel *dst, int n,
                 int stride, int upsample, int o, float *w, int m)
{
    float bias = RoundAmount(src[0]);
    if (m == 1) {
        float w0 = w[0];
        if (w0 == 1.0f && upsample == 1 && stride == 1)
            memcpy(dst, src, n*sizeof(TPixel));
        else if (w0 == 1.0f) {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = src[0];
        } else {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*src[0] + bias);
        }
    } else if (m == 2) {
        float w0 = w[0], w1 = w[1];
        if (w0 == w1) {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*(src[0*o] + src[1*o]) + bias);
        } else {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*src[0*o] + w1*src[1*o] + bias);
        }
    } else if (m == 3) {
        float w0 = w[0], w1 = w[1], w2 = w[2];
        if (w0 == w2) {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*(src[0*o] + src[2*o]) +
                                    w1*src[1*o] + bias);
        } else {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*src[0*o] + w1*src[1*o] +
                                    w2*src[2*o] + bias);
        }
    } else if (m == 4) {
        float w0 = w[0], w1 = w[1], w2 = w[2], w3 = w[3];
        if (w0 == w3 && w1 == w2) {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*(src[0*o] + src[3*o]) + 
                                    w1*(src[1*o] + src[2*o]) + bias);
        } else {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*src[0*o] + w1*src[1*o] +
                                    w2*src[2*o] + w3*src[3*o] + bias);
        }
    } else if (m == 5) {
        float w0 = w[0], w1 = w[1], w2 = w[2], w3 = w[3], w4 = w[4];
        if (w0 == w4 && w1 == w3) {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*(src[0*o] + src[4*o]) +
                                    w1*(src[1*o] + src[3*o]) +
                                    w2*src[2*o] + bias);
        } else {
            for (int i = 0; i < n; i++, dst += upsample, src += stride)
                *dst = (TPixel) (w0*src[0*o] + w1*src[1*o] + w2*src[2*o] +
                                    w3*src[3*o] + w4*src[4*o] + bias);
        }
    // add more, all the way up to 9?
    } else {
        for (int i = 0; i < n; i++, dst += upsample, src += stride) {
            double sum = 0.0;
            TPixel *p = &src[0];
            for (int k = 0; k < m; k++, p += o)
                sum += w[k] * p[0];
            *dst = (TPixel) sum;
        }
    }
}

//
//  Special cases:  1 1,  1 2 1, and 1 4 6 4 1 kernels
//

void VisConvolveRow(CVisRGBABytePixel *src, CVisRGBABytePixel *dst, int n,
                 int stride, int upsample, int o, float *w, int m)
{
    // This routine should never get called
    //  The CVisRGBAByteImage should have been aliased to a CVisByteImage instead!
    assert(src == 0);
}

template <class TPixel>
void VisConvolveRow11(TPixel *src, TPixel *dst, int n, 
                   int stride, int upsample, int o)
{
    for (int i = 0; i < n; i++, dst += upsample, src += stride)
        *dst = (TPixel) ((src[0*o] + src[1*o] + 1)/2);
}

template <class TPixel>
void VisConvolveRow121(TPixel *src, TPixel *dst, int n, 
                   int stride, int upsample, int o)
{
    for (int i = 0; i < n; i++, dst += upsample, src += stride)
        *dst = (TPixel) ((src[0*o] + 2*src[1*o] + src[2*o] + 2)/4);
}

template <class TPixel>
void VisConvolveRow161(TPixel *src, TPixel *dst, int n, 
                   int stride, int upsample, int o)
{
    for (int i = 0; i < n; i++, dst += upsample, src += stride)
        *dst = (TPixel) ((src[0*o] + 6*src[1*o] + src[2*o] + 4)/8);
}

template <class TPixel>
void VisConvolveRow14641(TPixel *src, TPixel *dst, int n,
                      int stride, int upsample, int o)
{
    for (int i = 0; i < n; i++, dst += upsample, src += stride)
        *dst = (TPixel) ((6*src[2*o] + 4*(src[1*o] + src[3*o]) +
                             (src[0*o] + src[4*o] + 8))/16);
}

//
// Dispatch to special case
//

template <class TPixel>
void VisConvolveRowX(TPixel *src, TPixel *dst, int n,
                  int stride, int upsample, int o, float *w, int m)
{
    if (m == 2 && w[0] == 0.5f && w[1] == 0.5f)
        VisConvolveRow11(src, dst, n, stride, upsample, o);
    else if (m == 3 && w[0] == 0.25f && w[1] == 0.5f && w[2] == 0.25f)
        VisConvolveRow121(src, dst, n, stride, upsample, o);
    else if (m == 3 && w[0] == 0.125f && w[1] == 0.75f && w[2] == 0.125f)
        VisConvolveRow161(src, dst, n, stride, upsample, o);
    else if (m == 5 && w[0] == 0.0625f && w[1] == 0.25f && 
             w[2] == 0.375f && w[3] == 0.25f&& w[4] == 0.0625f)
        VisConvolveRow14641(src, dst, n, stride, upsample, o);
    else
        VisConvolveRow(src, dst, n, stride, upsample, o, w, m);
}

void VisConvolveRowX(float *src, float *dst, int n,
                  int stride, int upsample, int o, float *w, int m)
{
    // Don't bother special-casing
    VisConvolveRow(src, dst, n, stride, upsample, o, w, m);
}

void VisConvolveRowX(double *src, double *dst, int n,
                  int stride, int upsample, int o, float *w, int m)
{
    // Don't bother special-casing
    VisConvolveRow(src, dst, n, stride, upsample, o, w, m);
}

static int standard_kernel(CVisFloatImage &kernel)
{
    int m = kernel.Width();
    float *w = kernel.PtrToFirstPixelInRow(kernel.Top());
    if (m == 2 && w[0] == 0.5f && w[1] == 0.5f)
        return 2;
    else if (m == 3 && w[0] == 0.25f && w[1] == 0.5f && w[2] == 0.25f)
        return 3;
    else if (m == 5 && w[0] == 0.0625f && w[1] == 0.25f && 
             w[2] == 0.375f && w[3] == 0.25f&& w[4] == 0.0625f)
        return 5;
    return 0;
}

//
//  These routines assume the VisCore.has already been correctly padded.
//

template <class TPixel>
static void SafeConvolveHorizontal(CVisImage<TPixel> &src,
                                  CVisImage<TPixel> &dst,
                                  CVisFloatImage &kernel, int subsample)
{
    int length = kernel.Width();
    // Need to cast ceil() to (double) since it is now (VC++4.2) defined for double and float.
    int suboff = subsample * ((int) ceil((double)src.Left() / subsample)) - src.Left();
    int offset = kernel.Left() + suboff;
    int nrows  = dst.Height();
    int ncols  = dst.Width();
    int nbands = src.NBands();
    float *w   = kernel.PtrToFirstPixelInRow(kernel.Top());

    // Iterate over the rows
    for (int r = 0; r < nrows; r++) {
        TPixel *srcp = src.PtrToFirstPixelInRow(r+src.Top())+offset*nbands;
        TPixel *dstp = dst.PtrToFirstPixelInRow(r+dst.Top());
        if (nbands == 1 || subsample == 1)
            VisConvolveRowX(srcp, dstp, ncols*nbands,
                         subsample, 1, nbands, w, length);
        else        // multiband subsampled is less efficient
            for (int b = 0; b < nbands; b++)
                VisConvolveRowX(&srcp[b], &dstp[b], ncols,
                             subsample*nbands, nbands, nbands, w, length);
    }
}

template <class TPixel>
static void SafeConvolveVertical(CVisImage<TPixel> &src,
                                 CVisImage<TPixel> &dst,
                                 CVisFloatImage &kernel, int subsample)
{
    int length = kernel.Width();
    int suboff = subsample * ((int) ceil((double)src.Top() / subsample)) - src.Top();
    int offset = kernel.Left() + suboff;
    int nrows  = dst.Height();
    int ncols  = dst.Width();
    int nbands = src.NBands();
    float *w   = kernel.PtrToFirstPixelInRow(kernel.Top());

    // Iterate over the rows
	int dppixelRowSrc = src.CbRow() / src.CbPixel();
	int dppixelOffsetSrc = dppixelRowSrc * offset;
	for (int r = 0; r < nrows; r++) {
        int rs = r * subsample;
        //TPixel *src0 = src.PtrToFirstPixelInRow(rs+src.Top()+offset+0);
        //TPixel *src1 = src.PtrToFirstPixelInRow(rs+src.Top()+offset+1);
        TPixel *src0 = src.PtrToFirstPixelInRow(rs+src.Top()) + dppixelOffsetSrc;
        TPixel *src1 = src0 + dppixelRowSrc;
        TPixel *dstp = dst.PtrToFirstPixelInRow(r +dst.Top());
        VisConvolveRowX(src0, dstp, ncols*nbands, 1, 1, dppixelRowSrc,
                     w, length);
    }
}

template <class TPixel>
static void SafeConvolveSeparable(CVisImage<TPixel> &src,
                                  CVisImage<TPixel> &dst,
                                  CVisFloatImage &h_kernel,
                                  CVisFloatImage &v_kernel,
                                  int subsample)
{
    int kernel_rows = v_kernel.Width();
    int kernel_cols = h_kernel.Width();
    int roffset = v_kernel.Left();
    int nrows  = src.Height();
    int ncols  = dst.Width();
    int nbands = src.NBands();
    CVisShape s1x1(0, 0, 1, 1);
    if (v_kernel.Shape() == s1x1 && v_kernel.Pixel(0, 0) == 1.0)
        kernel_rows = 0;
    if (h_kernel.Shape() == s1x1 && h_kernel.Pixel(0, 0) == 1.0)
        kernel_cols = 0;

    // Special cases
    if (! kernel_cols && ! kernel_rows)
        dst = src;      // pure copy, should not occur in practice
    else if (kernel_cols && ! kernel_rows)
        SafeConvolveHorizontal(src, dst, h_kernel, subsample);
    else if (kernel_rows && ! kernel_cols)
        SafeConvolveVertical(src, dst, h_kernel, subsample);

    else {
        // Create a temporary image for intermediate result
        int tmp_rows = nrows + kernel_rows - 1;
        CVisImage<TPixel> tmp1(CVisShape(dst.Left(), src.Top()+roffset, 
                                    dst.Left()+ncols, src.Top()+roffset+tmp_rows, nbands));

        // Enlarge the source so that vertical convolution works later
        CVisImage<TPixel> src2 = 
            src.SubImage(src.Left(), src.Top()+roffset, src.Right(),
					src.Top()+roffset+tmp_rows);

        // Do the horizontal convolution
        SafeConvolveHorizontal(src2, tmp1, h_kernel, subsample);

        // Crop the temporary image
        CVisImage<TPixel> tmp2 =
            tmp1.SubImage(dst.Left(), src.Top(), dst.Left()+ncols, src.Top()+nrows);

        // Do the vertical convolution
        SafeConvolveVertical(tmp2, dst, v_kernel, subsample);
    }
}

//
// Special code for 2x upsampling (Expand in CVisPyramid.h)
//

template <class TImage>
static TImage EvenOddTaps(TImage &kernel, int odd)
{
    int ncols = kernel.Width(), scol = kernel.Left();
    TImage shifted = kernel.SubImage(scol  + ((scol & 1) ^ odd), 0,
                                          scol+ncols, 1);
    shifted.ResetOrigin(shifted.Left()+odd, 0); // may not be right !?
    TImage taps = VisSubSample(shifted, 2);
    for (int i = 0; i < taps.Width(); i++)
        taps.Pixel(i+taps.Left(), 0) *= 2;
    return taps;
}

template <class TPixel>
static void UpSampleBy2Horizontal(CVisImage<TPixel> &src,
                                  CVisImage<TPixel> &dst,
                                  CVisFloatImage &kernel)
{
    // Even and odd coefficients of the filter
    int nrows  = dst.Height();
    int ncols  = dst.Width();
    int nbands = dst.NBands();
    int oddcol = dst.Left() & 1;    // starting column is odd, shift kernels by 1
    CVisFloatImage k0 = EvenOddTaps(kernel, 0^oddcol);
    CVisFloatImage k1 = EvenOddTaps(kernel, 1^oddcol);
    float *w0 = k0.PtrToFirstPixelInRow(k0.Top());
    float *w1 = k1.PtrToFirstPixelInRow(k1.Top());
    int offset0 = k0.Left()-oddcol;
    int offset1 = k1.Left();

    // Iterate over the rows
    for (int r = 0; r < nrows; r++) {
        TPixel *src0 = src.PtrToFirstPixelInRow(r+src.Top())+offset0*nbands;
        TPixel *src1 = src.PtrToFirstPixelInRow(r+src.Top())+offset1*nbands;
        TPixel *dstp = dst.PtrToFirstPixelInRow(r+dst.Top());
        for (int b = 0; b < nbands; b++) {
            VisConvolveRowX(&src0[b], &dstp[b       ], (ncols+1)/2,
                         nbands, 2*nbands, nbands, w0, k0.Width());
            VisConvolveRowX(&src1[b], &dstp[b+nbands], (ncols+0)/2,
                         nbands, 2*nbands, nbands, w1, k1.Width());
        }
    }
}

template <class TPixel>
static void UpSampleBy2Vertical(CVisImage<TPixel> &src,
                                CVisImage<TPixel> &dst,
                                CVisFloatImage &kernel)
{
    // Even and odd coefficients of the filter
    int nrows  = dst.Height();
    int ncols  = dst.Width();
    int nbands = dst.NBands();
    int oddrow = dst.Top() & 1;    // starting row is odd, shift kernels by 1
    CVisFloatImage k0 = EvenOddTaps(kernel, 0^oddrow);
    CVisFloatImage k1 = EvenOddTaps(kernel, 1^oddrow);
    float *w0 = k0.PtrToFirstPixelInRow(k0.Top());
    float *w1 = k1.PtrToFirstPixelInRow(k1.Top());
    int offset0 = k0.Left()-oddrow;
    int offset1 = k1.Left();

    // Iterate over the rows
	int dppixelRowSrc = src.CbRow() / src.CbPixel();
	int dppixelOffset0Src = dppixelRowSrc * offset0;
	int dppixelOffset1Src = dppixelRowSrc * offset1;
    for (int r = 0; r < nrows; r++) {
        int rs = r / 2;
        int dppixelOffset = ((r & 1) ? dppixelOffset1Src : dppixelOffset0Src);
        int length = (r & 1) ? k1.Width() : k0.Width();
        float   *w = (r & 1) ? w1 : w0;
        TPixel *src0 = src.PtrToFirstPixelInRow(rs+src.Top()) + dppixelOffset;
        TPixel *src1 = src0 + dppixelRowSrc;
        TPixel *dstp = dst.PtrToFirstPixelInRow(r +dst.Top());
        VisConvolveRowX(src0, dstp, ncols*nbands, 1, 1, dppixelRowSrc,
                     w, length);
    }
}

template <class TPixel>
static void UpSampleBy2Separable(CVisImage<TPixel> &src,
                                 CVisImage<TPixel> &dst,
                                 CVisFloatImage &h_kernel,
                                 CVisFloatImage &v_kernel)
{
    int kernel_rows = v_kernel.Width();
    int kernel_cols = h_kernel.Width();
    int roffset = v_kernel.Left();
    int nrows  = src.Height();
    int ncols  = dst.Width();
    int nbands = src.NBands();

    // Only works for true separable filter (no 1-D convolutions)
    assert(kernel_rows && kernel_cols);

    // Create a temporary image for intermediate result
    int tmp_rows = nrows + kernel_rows/2;   // is this right?
    int half_offset = (roffset + (roffset & 1)) / 2;
    CVisImage<TPixel> tmp1(CVisShape(dst.Left(), src.Top()+half_offset, 
                                dst.Left()+ncols, src.Top()+half_offset+tmp_rows, nbands));

    // Enlarge the source so that vertical convolution works later
    CVisImage<TPixel> src2 = 
        src.SubImage(src.Left(), src.Top()+half_offset, src.Right(),
				src.Top()+half_offset+tmp_rows);

    // Do the horizontal convolution
    UpSampleBy2Horizontal(src2, tmp1, h_kernel);

    // Crop the temporary image
    CVisImage<TPixel> tmp2 =
        tmp1.SubImage(dst.Left(), src.Top(), dst.Left()+ncols, src.Top()+nrows);

    // Do the vertical convolution
    UpSampleBy2Vertical(tmp2, dst, v_kernel);
}


//
//  Regular non-separable convolution:
//    this code has NOT been corrected to account for subsampling
//    when the start row/col are not multiples of subsample
//

template <class TPixel>
static void SafeConvolve(CVisImage<TPixel> &src,
                         CVisImage<TPixel> &dst,
                         CVisFloatImage kernel, int subsample)
{
    int rsize   = kernel.Height();       // kernel sizes
    int csize   = kernel.Width();
    int roffset = kernel.Top();        // kernel offsets
    int coffset = kernel.Left();
    int nrows   = dst.Height();          // image size
    int ncols   = dst.Width();
    int nbands  = src.NBands();
    int skip    = (subsample-1) * nbands;
	int dppixelRowSrc = src.CbRow() / src.CbPixel();
	int dppixelRowDst = dst.CbRow() / dst.CbPixel();

    int soffset = src.CbRow() / src.CbPixel();       // inter-line spacing
    int koffset = kernel.CbRow() / kernel.CbPixel();       // inter-line spacing
    float *w0   = &kernel.Pixel(kernel.Left(), kernel.Top());
    float bias  = RoundAmount(src.Pixel(src.Left(), src.Top(), 0));

    // Iterate over the rows
    for (int r = 0; r < nrows; r++) {
//        int src_row = src.Top() + r*subsample + roffset;
//        int dst_row = dst.Top() + r;
//        TPixel *p0 = &src.Pixel(src.Left() + coffset, src_row, 0);
//        TPixel *q  = &dst.Pixel(dst.Left(), dst_row,           0);

        int dppixelSrc = (r*subsample + roffset) * dppixelRowSrc
				+ coffset * nbands;
        int dppixelDst = r * dppixelRowDst;
        TPixel *p0 = &src.Pixel(src.StartPoint(), 0) + dppixelSrc;
        TPixel *q  = &dst.Pixel(dst.StartPoint(), 0) + dppixelDst;

        // Iterate over the columns and bands
        for (int c = 0; c < ncols; c++) {
            for (int b = 0; b < nbands; b++, q++, p0++) {
                double sum = 0.0;
                TPixel *p1 = p0;
                float     *w1 = w0;

                // Iterate over the kernel
                for (int y = 0; y < rsize; y++) {
                    TPixel *p = p1;
                    for (int x = 0; x < csize; x++, p += nbands)
                        sum += w1[x] * p[0];
                    p1 += soffset;
                    w1 += koffset;
                }
                q[0] = (TPixel) (sum + bias);
            }
            p0 += skip;
        }
    }
}


//
//  Enlarge the image if it isn't large enough to do the convolution
//  This is a placeholder for the code that Matthew will eventually write
//  to take care of this case.
//
   
template <class TImage>
TImage VisConvolveEnlarge(TImage &src, CVisFloatImage &kernel,
                     int warning_msg = 0)
{
    // Check that the source VisCore.has big enough borders
    if (src.Top() + kernel.Top() < src.MemoryRect().top ||
        src.Left() + kernel.Left() < src.MemoryRect().left ||
        src.Bottom() + kernel.Bottom() - 1 > src.MemoryRect().top+src.MemoryHeight() ||
        src.Right() + kernel.Right() - 1 > src.MemoryRect().left+src.MemoryWidth()) {

        // Replace this with a better warning (user-controllable warn level)
        if (warning_msg) {
            fprintf(stderr, "VisConvolve: kernel + src > src padding;");
            fprintf(stderr, "allocating a larger padded image (with undefined borders)\n");
            fprintf(stderr, "In the future, please enlarge and pad the image yourself.\n");
        }

        // An alternative strategy would be allocate a just big enough image
        int new_border = max(max(-kernel.Top(), -kernel.Left()),
                             max( kernel.Bottom() - 1,  kernel.Right() - 1));
		CVisShape shapeBigger(src.Shape());
		CVisShape shapeBiggerBorder(src.Shape());
		shapeBiggerBorder.InflateRect(new_border, new_border);
        TImage bigger;
		bigger.Allocate(shapeBigger, shapeBiggerBorder);
        src.CopyPixelsTo(bigger);
        bigger.Pad(g_evispadConvolveEnlarge, shapeBiggerBorder);
        return bigger;
    } else
        return src;
}

template <class TImage>
void VisConvolve(TImage &src, TImage &dst,
                   CVisFloatImage &kernel, int subsample)
{
    // Enlarge the src image, if necessary
    TImage src1 = VisConvolveEnlarge(src, kernel);

    // Check for consistent size
    assert(dst.Shape() == src1.Shape().Decimated(subsample));
    assert(subsample > 0);

    SafeConvolve(src1, dst, kernel, subsample);
}

template <class TImage>
TImage VisConvolve(TImage &src, CVisFloatImage &kernel)
{
    TImage result(src.Shape());
    VisConvolve(src, result, kernel, 1);
    return result;
}

void VisConvolve(CVisRGBAByteImage &src, CVisRGBAByteImage &dst,
                   CVisFloatImage &kernel, int subsample)
{
    CVisByteImage srcg;
	srcg.Alias(src);
    CVisByteImage dstg;
	dstg.Alias(dst);
    VisConvolve(srcg, dstg, kernel, subsample);
}
/*
void VisConvolve(CVisRGBAIntImage &src, CVisRGBAIntImage &dst,
                   CVisFloatImage &kernel, int subsample)
{
    CVisIntImage srcg;
	srcg.Alias(src);
    CVisIntImage dstg;
	dstg.Alias(dst);
    VisConvolve(srcg, dstg, kernel, subsample);
}
*/
void VisConvolve(CVisRGBAFloatImage &src, CVisRGBAFloatImage &dst,
                   CVisFloatImage &kernel, int subsample)
{
    CVisFloatImage srcg;
	srcg.Alias(src);
    CVisFloatImage dstg;
	dstg.Alias(dst);
    VisConvolve(srcg, dstg, kernel, subsample);
}

// #pragma auto_inline( off )

template <class TImage>
void VisConvolveSeparable(TImage &src, TImage &dst,
                            CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
                            int subsample)
{
    // Special case for CVisRGBAByteImage: use GrayMultiband unless std kernel
    if (src.PixFmt() == evispixfmtRGBAByte &&
        !(standard_kernel(h_kernel) && standard_kernel(v_kernel))) {
        CVisByteImage srcg;
		srcg.Alias(src);
        CVisByteImage dstg;
		dstg.Alias(dst);
        VisConvolveSeparable(srcg, dstg, h_kernel, v_kernel, subsample);
        return;
    }
#ifdef NDEBUG
    //
    //  For some reason, the compiler with /O2 refuses to generate the code
    //  for void VisConvolveSeparable(Colorimage &, ...) & CVisFloatImage...
    //
    if (subsample == -999999999999) {
        CVisRGBAByteImage cimg;
        VisConvolveSeparable(cimg, cimg, h_kernel, v_kernel, subsample);
        CVisFloatImage fimg;
        VisConvolveSeparable(fimg, fimg, h_kernel, v_kernel, subsample);
        CVisIntImage iimg;
        VisConvolveSeparable(iimg, iimg, h_kernel, v_kernel, subsample);
    }
#endif

    // Enlarge the src image, if necessary
    CVisFloatImage kernel(CVisRect(h_kernel.Left(), v_kernel.Left(),
                          h_kernel.Right(), v_kernel.Right()), 1);
    TImage src1 = VisConvolveEnlarge(src, kernel);

    // Regular or subsampling
    assert(subsample == -2 || subsample > 0);
    if (subsample > 0) {
		assert(dst.Shape() == src1.Shape().Decimated(subsample));
        SafeConvolveSeparable(src1, dst, h_kernel, v_kernel, subsample);
    } else if (subsample == -2) {
		assert(dst.Shape().Decimated(2) == src1.Shape());
        UpSampleBy2Separable(src1, dst, h_kernel, v_kernel);
    }
#if 0
    static int gen = 0;
    char filename[1024];
    sprintf(filename, "tmp_enlarge_%02d.bmp", gen);
    src1.WriteFile(filename);
    sprintf(filename, "tmp_convolved_%02d.bmp", gen);
    dst.WriteFile(filename);
    gen++;
#endif
}
/*
void VisConvolveSeparable(CVisRGBAIntImage &src, CVisRGBAIntImage &dst,
                          CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
                          int subsample)
{
    // Special case for CVisRGBAIntImage: use GrayMultiband unless std kernel
    CVisIntImage srcg;
	srcg.Alias(src);
    CVisIntImage dstg;
	dstg.Alias(dst);
    VisConvolveSeparable(srcg, dstg, h_kernel, v_kernel, subsample);
}
*/
void VisConvolveSeparable(CVisRGBAFloatImage &src, CVisRGBAFloatImage &dst,
                          CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
                          int subsample)
{
    // Special case for CVisRGBAFloatImage: use GrayMultiband unless std kernel
    CVisFloatImage srcg;
	srcg.Alias(src);
    CVisFloatImage dstg;
	dstg.Alias(dst);
    VisConvolveSeparable(srcg, dstg, h_kernel, v_kernel, subsample);
}

template <class TImage>
TImage VisConvolveSeparable(TImage &src,
                                   CVisFloatImage &h_kernel, 
                                   CVisFloatImage &v_kernel)
{
    TImage result(src.Shape());
    VisConvolveSeparable(src, result, h_kernel, v_kernel, 1);
    return result;
}


//
//  Default kernels
//

static float k_11[2] = {0.5f, 0.5f};
static float k_121[3] = {0.25f, 0.5f, 0.25f};
static float k_14641[5] = {0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f};


CVisFloatImage VisConvolutionKernel_11   (CVisRect( 0, 0, 1, 1), 1,
										  evisimoptDefault, (BYTE *) &k_11);
CVisFloatImage VisConvolutionKernel_121  (CVisRect(-1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_121);
CVisFloatImage VisConvolutionKernel_14641(CVisRect(-2, 0, 3, 1), 1,
										  evisimoptDefault, (BYTE *) &k_14641);

//
// Explicit template instantiation:
//  note that this fails in the Release verions (/O2): see workaround above...
//

template <class TImage>
class VisConvolveInstantiate {
public:
    VisConvolveInstantiate();
};

template <class TImage>
VisConvolveInstantiate<TImage>::VisConvolveInstantiate()
{
    CVisFloatImage h_k, v_k;
    TImage img;
    VisConvolve(img, h_k);
    VisConvolveSeparable(img, h_k, v_k);
    VisConvolveSeparable(img, img, h_k, v_k, 1);
}

template VisConvolveInstantiate<CVisByteImage>;
template VisConvolveInstantiate<CVisIntImage>;
#if 0       // not currently used, since generate compiler error...
template <class CVisIntImage>
void VisConvolveSeparable(CVisIntImage &src, CVisIntImage &dst,
                            CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
                            int subsample);
#endif
template VisConvolveInstantiate<CVisRGBAByteImage>;
template VisConvolveInstantiate<CVisFloatImage>;
//template VisConvolveInstantiate<CVisRGBAIntImage>;
template VisConvolveInstantiate<CVisRGBAFloatImage>;
