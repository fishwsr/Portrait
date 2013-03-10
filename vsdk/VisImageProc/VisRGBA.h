///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisRGBA.h -- special inline functions for RGBA arithmetic
//  
// SPECIFICATION
//  CVisRGBABytePixel VisPixelAverage(CVisRGBABytePixel p0, CVisRGBABytePixel p1);
//  CVisRGBABytePixel PixelSampleLinear(CVisRGBABytePixel p0, CVisRGBABytePixel p1, int scaled_a);
//  void VisConvolveRow11(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
//                     int stride, int upsample, int o);
//  void VisConvolveRow121(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
//                      int stride, int upsample, int o);
//  void VisConvolveRow161(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
//                      int stride, int upsample, int o);
//  void VisConvolveRow14641(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
//                        int stride, int upsample, int o);
//
// PARAMETERS
//  p0, p1          source CVisRGBABytePixel(s)
//  scaled_a        scaled blend coefficient (16 bit fraction)
//  csrc..o         see VisConvolve.cpp, which uses these

// DESCRIPTION
//  These routines implement fast basic operations on color
//  pixels, such as pixel averaging, linear interpolation,
//  convolution, and addition/subtraction.
//  
//  The basic strategy used is to use integer operations, and to
//  gang up two or four such operation per integer word.
//
//  VisPixelAverage returns the average of two pixels.
//  PixelSampleLinear returns p0 + (p1 - p0) * a.
//
//  The VisConvolveRow999 routines are internal to VisConvolve.cpp,
//  but appear here because they use the same parallel processing tricks.
//
// SEE ALSO
//  Fast Color Pixel Blends, Technical Note No. 2 (Vision Technology Group)
//   a study of algorithms for quickly blending color pixels
//
// BUGS
//  
//
// DESIGN
//  The current design is based on using 32-bit integer arithmetic
//  to do multiple (2 or 4) bands concurrently.
//  On a 64-bit architecture, double longs may be more efficient.
//  When the MMX architecture is available, the routines could
//  be re-written to optionally take advantage of this.
//
// HISTORY
//  23-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created from code in VisResample.cpp.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_RGBA_FAST_H_
#define _VIS_RGBA_FAST_H_

// This line should match the one in VisResample.cpp:
#ifndef scale_bits
#define scale_bits 16       // (u,v) fixed point scaling
#endif

//
//  These are used in VisResample.cpp and VisRGBA.cpp:
//

// LATER:  Since some overloads are not for RGBA types, should this be moved
// to another header file.

inline unsigned char VisPixelAverage(unsigned char p0, unsigned char p1)
{
    return (unsigned char) (p0 + p1 + 1)/2;
}

inline int VisPixelAverage(int p0, int p1)
{
    return (int) (p0 + p1 + 1)/2;
}

inline CVisRGBABytePixel VisPixelAverage(CVisRGBABytePixel p0, CVisRGBABytePixel p1)
{
    int i0 = *(int *) &p0, i1 = *(int *) &p1;
    int i2 = ((i0 >> 1) & 0x7f7f7f7f) +
             ((i1 >> 1) & 0x7f7f7f7f) +
             ((i0 | i1) & 0x01010101);
    return *(CVisRGBABytePixel *) &i2;
}

inline int VisPixelAverage4Bytes(int i0, int i1)    // used in VisBlend.cpp
{
    int i2 = ((i0 >> 1) & 0x7f7f7f7f) +
             ((i1 >> 1) & 0x7f7f7f7f) +
             ((i0 | i1) & 0x01010101);
    return i2;
}

inline float VisPixelAverage(float p0, float p1)
{
    return 0.5f*(p0 + p1);
}

inline CVisRGBA<float> VisPixelAverage(CVisRGBA<float> p0, CVisRGBA<float> p1)
{
    return CVisRGBA<float>(0.5f*(p0.R() + p1.R()), 0.5f*(p0.G() + p1.G()),
                           0.5f*(p0.B() + p1.B()), 0.5f*(p0.A() + p1.A()));
}

inline double VisPixelAverage(double p0, double p1)
{
    return 0.5*(p0 + p1);
}

// LATER:  Since some overloads are not for RGBA types, should this be moved
// to another header file.
// LATER:  Should we add a special case for CVisRGBAFloatPixel, since it's used in VisResample.cpp?

#ifndef g_scale_bits
#define g_scale_bits 16       // (u,v) fixed point scaling
#endif
#define g_scale (1 << g_scale_bits)

// Rick Szeliski, 8/23/99:
// I redefined the generic function to be called PixelSampleLinearScalar,
// since that forces users to select between the Scalar and RGBA codes on
// a per-type basis.  This will hopefully avoid surprise bugs in the future,
// at the expense of having to do explicit type instantiation...

template <class TPixel>
inline TPixel PixelSampleLinearScalar(TPixel p0, TPixel p1,
                                          int scaled_a)
{
    return (TPixel) (p0 + (scaled_a * (p1 - p0) + (TPixel) g_scale/2) / g_scale);
}

inline int PixelSampleLinear(int p0, int p1, int scaled_a)
{
    return PixelSampleLinearScalar(p0, p1, scaled_a);
}

inline unsigned char PixelSampleLinear(unsigned char p0, unsigned char p1, int scaled_a)
{
    return PixelSampleLinearScalar(p0, p1, scaled_a);
}

inline CVisRGBABytePixel PixelSampleLinear(CVisRGBABytePixel p0, CVisRGBABytePixel p1, int scaled_a)
{
     // UNDONE:  Should we be using "unsigned int" vars here?
	int i0 = *(int *) &p0, i1 = *(int *) &p1;
    if (i0 == i1)
        return *(CVisRGBABytePixel *) &i0;
    int sa = (scaled_a >> (scale_bits - 8)) & 255;  // make it byte?
    int j0 =  i0       & 0x00ff00ff, j1 =  i1       & 0x00ff00ff;
    int k0 = (i0 >> 8) & 0x00ff00ff, k1 = (i1 >> 8) & 0x00ff00ff;
    int j2 =  j0       + ((sa * (j1 - j0) + 0x00800080) >> 8);
    int k2 = (k0 << 8) +   sa * (k1 - k0) + 0x00800080;
    int i2 = (k2 & 0xff00ff00) + (j2 & 0x00ff00ff);
    return *(CVisRGBABytePixel *) &i2;
}

inline float PixelSampleLinear(float p0, float p1, int scaled_a)
{
    const float iscale = 1.0f / g_scale;
    return p0 + (scaled_a * iscale) * (p1 - p0);
}

inline double PixelSampleLinear(double p0, double p1, int scaled_a)
{
    const double iscale = 1.0 / g_scale;
    return p0 + (scaled_a * iscale) * (p1 - p0);
}

// I wish we could define the following as the PixelResampleLinear function directly,
// but VC (or C++ ?) doesn't allow for multiple conflicting templated functions,
// i.e., it will not automatically find the most restrictive template...  (Rick 8/23/99)

template <class TPixel>
inline CVisRGBA<TPixel> PixelSampleLinearRGBA(CVisRGBA<TPixel> p0, CVisRGBA<TPixel> p1,
                                          int scaled_a)
{
    TPixel R = PixelSampleLinear(p0.R(), p1.R(), scaled_a);
    TPixel G = PixelSampleLinear(p0.G(), p1.G(), scaled_a);
    TPixel B = PixelSampleLinear(p0.B(), p1.B(), scaled_a);
    TPixel A = PixelSampleLinear(p0.A(), p1.A(), scaled_a);
    return CVisRGBA<TPixel>(R, G, B, A);
}

// We unfortunately now have to define all possible RGBA types of interest...
inline CVisRGBA<float> PixelSampleLinear(CVisRGBA<float> p0, CVisRGBA<float> p1, int scaled_a)
{
    return PixelSampleLinearRGBA(p0, p1, scaled_a);
}


// Fast RGBA over operation:  taken from Jim Blinn's technical note
//  Turbo version of over operator
//  used in VisBlend.cpp:

// Caution (t-rdror, 8/10/99):  This version of RGBA_over works with premultiplied colors
// and assumes that each premultiplied color lies in the range [0, A], where A is the
// value of alpha for the corresponding pixel.  This assumption is true if each
// premultiplied color corresponds to a non-premultiplied color between 0 and 255.  
// For example, compositing RGBA pixels (0,100,0,100) over (0,255,0,255) gives (0,255,0,255),
// but compositing (0,101,0,100) over (0,255,0,255) gives (1,0,0,255)

static inline int RGBA_over(int F, int B)
{
    long a = (F >> 24) & 0xFF;      // check that this is right!
    long t = 255 - a, tt, ttr;
	long B00rr00bb = B & 0x00FF00FF;
	tt = t*B00rr00bb + 0x00800080;
	ttr = (tt & 0xFF00FF00)>>8;
	long Drb = ((tt+ttr) & 0xFF00FF00)>>8;

	long B00aa00gg = (B & 0xFF00FF00)>>8;
	tt = t*B00aa00gg + 0x00800080;
	ttr = (tt & 0xFF00FF00)>>8; // UNDONE:  Should we be using "unsigned long" vars here?
	long Dag =  (tt+ttr) & 0xFF00FF00;

	long C = F + Drb + Dag;
    return C;
}


//
//  These are used in VisConvolve.cpp:
//    see that file for examples of usage
//

void VisConvolveRow11(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                   int stride, int upsample, int o);
void VisConvolveRow121(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                    int stride, int upsample, int o);
void VisConvolveRow161(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                    int stride, int upsample, int o);
void VisConvolveRow14641(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n,
                      int stride, int upsample, int o);

// Returns an gray-valued image corresponding to the alpha values of
// an RGBA image.  For debugging purposes.
// Limitation:  Assumes single-band byte image.
CVisGrayByteImage VisAlphaImageGray(const CVisRGBAByteImage& img);

// Same as above, but returns an RGBA image with gray colors
CVisRGBAByteImage VisAlphaImageRGBA(const CVisRGBAByteImage& img);

// Writes an image to a text file for debugging purposes
// Each row contains: column, row, R, G, B, A, in that order
void VisRGBAFloatImageFileTextWrite(const CVisRGBAFloatImage& img, char* filename);

#endif // _VIS_RGBA_FAST_H_
