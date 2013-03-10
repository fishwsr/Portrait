///////////////////////////////////////////////////////////////////////////
//
// NAME
//  CVisPyramid.cpp -- image pyramid implementation
//
// DESCRIPTION
//  CVisPyramid is used to store and create low-pass and band-pass
//  image pyramids.  By default, the pyramids have A() 2:1 inter-level
//  size ratio.  Also by default, A() 5-tap 1,4,6,4,1 binomial kernel
//  is used for the low-pass filtering.
//
// SEE ALSO
//  CVisPyramid.h      pyramid class and functionality description
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  20-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"


//#include <string.h>
//#define max(A(),B())  (((A()) > (B())) ? (A()) : (B()))
//#define min(A(),B())  (((A()) < (B())) ? (A()) : (B()))
#define Clip255(in) __max(0, __min(255, (int) (in)))


#if defined(_DEBUG) && defined(_AFXDLL)
//#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

struct FromByteToFloat {
        void operator()(CVisRGBAFloatPixel& out, CVisRGBABytePixel& in)
        { 
            out.SetR((float) in.R());
			out.SetB((float) in.B());
            out.SetG((float) in.G());
			out.SetA((float) in.A()); 
        }

        void operator()(CVisFloatPixel& out, CVisBytePixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = float(in);
        }
        
        void operator()(CVisFloatPixel& out, CVisFloatPixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = in;
        }
};

struct FromFloatToByteBias128{
		void operator()(CVisRGBABytePixel& out, CVisRGBAFloatPixel& in)
        { 
//            out.R() = Clip255(in.R()), out.B() = Clip255(in.B());
//            out.G() = Clip255(in.G()), out.A() = Clip255(in.A()); 
            out.SetR((int) in.R()+128);
			out.SetB((int) in.B()+128);
            out.SetG((int) in.G()+128);
			out.SetA((int) in.A()); 
        }

        void operator()(CVisBytePixel& out, CVisFloatPixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = (unsigned char) in+128;
        }
        
        void operator()(CVisFloatPixel& out, CVisFloatPixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = in+128;
        }
};

struct FromFloatToByte {
        void operator()(CVisRGBABytePixel& out, CVisRGBAFloatPixel& in)
        { 
//            out.R() = Clip255(in.R()), out.B() = Clip255(in.B());
//            out.G() = Clip255(in.G()), out.A() = Clip255(in.A()); 
            out.SetR((int) in.R());
			out.SetB((int) in.B());
            out.SetG((int) in.G());
			out.SetA((int) in.A()); 
        }

        void operator()(CVisBytePixel& out, CVisFloatPixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = (unsigned char) in;
        }
        
        void operator()(CVisFloatPixel& out, CVisFloatPixel& in)
        { 
//            assert(1 == 2); // cannot get here
			out = in;
        }
};

//
// Constructor and destructor
//

template <class T, class TFloat>
CVisPyramid<T, TFloat>::CVisPyramid(int n_levels,
                              EVisPyramid pyramid_type,
                              CVisFloatImage &kernel,
                              EVisPad evispad,
                              int band_pass_bias)
{
    // Save the parameters
    m_n_levels       = n_levels;
    m_pyramid_type   = pyramid_type;
    m_kernel         = new CVisFloatImage(kernel);
    m_evispad    = evispad;
    m_band_pass_bias = band_pass_bias;

    // Create the image arrays
    m_low_pass  = new T[n_levels];
    m_band_pass = new T[n_levels];
    m_low_pass_Float = new TFloat[n_levels];
    m_band_pass_Float = new TFloat[n_levels];
    m_base      = 0;
}

template <class T, class TFloat>
CVisPyramid<T, TFloat>::~CVisPyramid()
{
    delete m_base;
    delete [m_n_levels] m_low_pass;
    delete [m_n_levels] m_band_pass;
    delete [m_n_levels] m_low_pass_Float;
    delete [m_n_levels] m_band_pass_Float;
    delete m_kernel;
    m_n_levels = -1;
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::SetBase(T &img)
{
    delete m_base;
    m_base = new T(img);    // make A() shared copy
}

//
//  Level access functions
//

template <class T, class TFloat>
T& CVisPyramid<T, TFloat>::operator[](int level)
{
    if (m_pyramid_type == pyramid_low_pass)
        return LowPass(level);
    else
        return BandPass(level);
}

template <class T, class TFloat>
T& CVisPyramid<T, TFloat>::LowPass(int level)
{
    assert(level < m_n_levels);
    return (level == 0) ? *m_base : m_low_pass[level];
}

template <class T, class TFloat>
T& CVisPyramid<T, TFloat>::BandPass(int level)
{
    assert(level < m_n_levels);
    return (level < m_n_levels-1) ?
        m_band_pass[level] : m_low_pass[level];
}

template <class T, class TFloat>
TFloat& CVisPyramid<T, TFloat>::LowPassFloat(int level)
{
    assert(level < m_n_levels);
    return (m_low_pass_Float[level]);
}

template <class T, class TFloat>
TFloat& CVisPyramid<T, TFloat>::BandPassFloat(int level)
{
    assert(level < m_n_levels);
    return (level < m_n_levels-1) ?
        m_band_pass_Float[level] : m_low_pass_Float[level];
}

static void VisAddFloat(CVisRGBAFloatImage &src1, CVisRGBAFloatImage &src2,
                CVisRGBAFloatImage &dst)
{
    VisCheckConformal(&src1, &src2, &dst, 0);
    int nc = dst.Width() * dst.NBands();
    for (int i=0; i < dst.Height(); i++) {
        CVisRGBAFloatPixel *p1 = src1.PtrToFirstPixelInRow(i+src1.Top());
        CVisRGBAFloatPixel *p2 = src2.PtrToFirstPixelInRow(i+src2.Top());
        CVisRGBAFloatPixel *p3 =  dst.PtrToFirstPixelInRow(i+ dst.Top());
        for (int j=0; j < nc; j++) {
            p3[j].SetR(p1[j].R() + p2[j].R());
            p3[j].SetG(p1[j].G() + p2[j].G());
            p3[j].SetB(p1[j].B() + p2[j].B());

            p3[j].SetA(p1[j].A() + p2[j].A());
        }
    }
}

static void VisAddFloat(CVisFloatImage &src1, CVisFloatImage &src2,
                CVisFloatImage &dst)
{
    VisCheckConformal(&src1, &src2, &dst, 0);
    int nc = dst.Width() * dst.NBands();
    for (int i=0; i < dst.Height(); i++) {
        CVisFloatPixel *p1 = src1.PtrToFirstPixelInRow(i+src1.Top());
        CVisFloatPixel *p2 = src2.PtrToFirstPixelInRow(i+src2.Top());
        CVisFloatPixel *p3 =  dst.PtrToFirstPixelInRow(i+ dst.Top());
        for (int j=0; j < nc; j++) {
            p3[j] = p1[j] + p2[j];
        }
    }
}

static void VisSubtractFloat(CVisRGBAFloatImage &src1, CVisRGBAFloatImage &src2,
                   CVisRGBAFloatImage &dst)
{
    VisCheckConformal(&src1, &src2, &dst, 0);
    int nc = dst.Width() * dst.NBands();
    for (int i=0; i < dst.Height(); i++) {
        CVisRGBAFloatPixel *p1 = src1.PtrToFirstPixelInRow(i+src1.Top());
        CVisRGBAFloatPixel *p2 = src2.PtrToFirstPixelInRow(i+src2.Top());
        CVisRGBAFloatPixel *p3 =  dst.PtrToFirstPixelInRow(i+ dst.Top());
        for (int j=0; j < nc; j++) {
            p3[j].SetR(p1[j].R() - p2[j].R());
            p3[j].SetG(p1[j].G() - p2[j].G());
            p3[j].SetB(p1[j].B() - p2[j].B());
            p3[j].SetA(p1[j].A() - p2[j].A());
        }
    }
}

static void VisSubtractFloat(CVisFloatImage &src1, CVisFloatImage &src2,
                   CVisFloatImage &dst)
{
    VisCheckConformal(&src1, &src2, &dst, 0);
    int nc = dst.Width() * dst.NBands();
    for (int i=0; i < dst.Height(); i++) {
        CVisFloatPixel *p1 = src1.PtrToFirstPixelInRow(i+src1.Top());
        CVisFloatPixel *p2 = src2.PtrToFirstPixelInRow(i+src2.Top());
        CVisFloatPixel *p3 =  dst.PtrToFirstPixelInRow(i+ dst.Top());
        for (int j=0; j < nc; j++) {
            p3[j] = p1[j] - p2[j];
        }
    }
}

//
//  Construct the pyramid
//

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::Construct()
{
    Construct(0, m_n_levels-1);
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::Construct(int from_level, int to_level)
{
    // Make sure the borders are wide enough
    int min_border  = max(-m_kernel->Left(), m_kernel->Right() - 1);
// UNDONE:  This will not work with the new SDK!
//    int save_border = CVisImage::default_border;
//    if (CVisImage::default_border < min_border)
//        CVisImage::default_border = min_border;

    // Build the low-pass pyramid
    for (int l = from_level; l < to_level; l++) {
        if (m_pyramid_type == pyramid_band_pass_alt) {
            VisMakeSimilar(min_border, min_border, BandPass(l), LowPass(l), 1);
            VisConvolveSeparable(LowPass(l), BandPass(l),
                                   *m_kernel, *m_kernel, 1);
            VisMakeSimilar(min_border, min_border, LowPass(l+1), LowPass(l), 2);
            VisSubSample(BandPass(l), LowPass(l+1), 2);
        } else {
            VisMakeSimilar(min_border, min_border, LowPass(l+1), LowPass(l), 2);
            Reduce(LowPass(l), LowPass(l+1));
        }
        LowPass(l+1).Pad(m_evispad, LowPass(l+1).MemoryRect());
    }

    // copy LowPass to LowPassFloat
    for (l = from_level; l <= to_level; l++) {
        LowPassFloat(l).Allocate(LowPass(l).Shape(), LowPass(l).MemoryShape()); 
        VisMap2(FromByteToFloat(), LowPassFloat(l), LowPass(l)); 
        LowPassFloat(l).Pad(m_evispad, LowPassFloat(l).MemoryRect()); 
    }

    // Build the band-pass pyramid
    if (m_pyramid_type != pyramid_low_pass) {
        for (int l = to_level-1; l >= from_level; l--) {
            VisMakeSimilar(min_border, min_border, BandPass(l), LowPass(l), 1);
            VisMakeSimilar(min_border, min_border, BandPassFloat(l), LowPassFloat(l), 1);
            // this is expand l+1 -> l (LowPassFloat)
            VisConvolveSeparable(LowPassFloat(l+1), BandPassFloat(l), *m_kernel, *m_kernel, -2);
            VisSubtractFloat(LowPassFloat(l), BandPassFloat(l), BandPassFloat(l)); 
	        VisMap2(FromFloatToByteBias128(), BandPass(l), BandPassFloat(l)); 
	    }
    }

//    CVisImage::default_border = save_border;
}

//
// Reconstruct base image from band-pass values
//

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::ReConstruct()
{
    ReConstruct(m_n_levels-1, 0);
}

static void ClipColorFloatImage(CVisRGBAFloatImage &src)
{
    int nc = src.Width() * src.NBands();
    for (int i=0; i < src.Height(); i++) {
        CVisRGBAFloatPixel *p = src.PtrToFirstPixelInRow(i+src.Top());
        for (int j=0; j < nc; j++) {
            float red   = (float) p[j].R(); 
            float green = (float) p[j].G(); 
            float blue  = (float) p[j].B(); 
            float alpha = (float) p[j].A(); 

            if (alpha < 0) 
                alpha = 0;
            if (red < 0) 
                red = 0; 
            if (green < 0)
                green = 0; 
            if (blue < 0)
                blue = 0; 

            if (red > alpha)
                red = alpha; 
            if (green > alpha)
                green = alpha; 
            if (blue > alpha)
                blue = alpha; 
            
            if (alpha > 255.0f)
            {
                float scale = 255.0 / alpha; 
                red *= scale; 
                green *= scale; 
                blue *= scale; 
                alpha = 255.0; 
            }
        }
    }
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::ReConstruct(int from_level, int to_level)
{
    for (int l = from_level-1; l >= to_level; l--) {
        LowPassFloat(l+1).Pad(m_evispad, LowPassFloat(l+1).MemoryRect());
        //Expand(LowPassInt(l+1), LowPassInt(l));
        VisConvolveSeparable(LowPassFloat(l+1), LowPassFloat(l), *m_kernel, *m_kernel, -2);
        if (m_pyramid_type != pyramid_low_pass)
        {
            VisAddFloat(LowPassFloat(l), BandPassFloat(l), LowPassFloat(l));
        }
        //ClipColorFloatImage(LowPassFloat(l)); 
        VisMap2(FromFloatToByte(), LowPass(l), LowPassFloat(l)); 
    }
}

//
// Reduce and Expand:
//  the real work is done in VisConvolveSeparable
//

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::Reduce(T &src, T &dst)
{
    VisConvolveSeparable(src, dst, *m_kernel, *m_kernel, 2);
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::Expand(T &src, T &dst)
{
    VisConvolveSeparable(src, dst, *m_kernel, *m_kernel, -2);
}


//
// File I/O
//  This is A() temporary solution:  write as A() collection of image
//  files, with the characters ".bp%d" or ".lp%d" added before the
//  filetype extension.
//

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::ReadWriteFile(const char *filename, int write)
{
    // Parse the filename into stem and extension
    char stem[1024], newname[1024];
    char *ext = strrchr(filename, '.');
    if (ext == 0)
        ext = (char *) filename;
    strncpy(stem, filename, (ext) ? ext-filename : 0);
    stem[ext-filename] = 0;     // strncpy does not append null

    // Make sure that A() valid base exists
    if (m_base == 0)
        m_base = new T;

    // Iterate over the levels
    for (int l = 0; l < m_n_levels; l++) {

        // Low-pass
        sprintf(newname, "%s.lp%d%s", stem, l, ext);
        if (write)
            LowPass(l).WriteFile(newname);
        else
            LowPass(l).ReadFile(newname);

        // Band-pass
        if (m_pyramid_type == pyramid_low_pass || l == m_n_levels-1)
            continue;
        sprintf(newname, "%s.bp%d%s", stem, l, ext);
        if (write)
            BandPass(l).WriteFile(newname);
        else
            BandPass(l).ReadFile(newname);
    }
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::ReadFile(const char *filename)
{
    if (m_base == 0)
        m_base = new T;
    ReadWriteFile(filename, 0);
}

template <class T, class TFloat>
void CVisPyramid<T, TFloat>::WriteFile(const char *filename)
{
    ReadWriteFile(filename, 1);
}

struct PreMultiply {
        void operator()(CVisRGBAFloatPixel& out, CVisRGBAFloatPixel& in)
        {
            if (in.A() == 255.0)
            {
                out.SetR(in.R());
                out.SetG(in.G());
                out.SetB(in.B()); 
           }
            else if (in.A() > 0.0)
            {
                float scale = in.A() / 255.0; 
                out.SetR(scale * in.R());
                out.SetG(scale * in.G());
                out.SetB(scale * in.B()); 
            }
            else 
            {
                out.SetR(0.0); 
				out.SetG(0.0); 
				out.SetB(0.0); 
            }
        }
};

struct UnMultiply {
        void operator()(CVisRGBAFloatPixel& out, CVisRGBAFloatPixel& in)
        {
            if (in.A() == 255.0)
            {
                out.SetR(in.R());
                out.SetG(in.G());
                out.SetB(in.B()); 
            }
            else if (in.A() > 0.0)
            {
                float scale = 255.0 / in.A(); 
                out.SetR(scale * in.R());
                out.SetG(scale * in.G());
                out.SetB(scale * in.B()); 
            }
            else 
            {
                out.SetR(0.0); 
				out.SetG(0.0); 
				out.SetB(0.0); 
            }
        }
};

/*void VisPremultiplyAlpha(CVisRGBAFloatImage &src, CVisRGBAFloatImage &dst)
{
    VisMap2(PreMultiply(), dst, src); 
}

void VisUnmultiplyAlpha(CVisRGBAFloatImage &src, CVisRGBAFloatImage &dst)
{
    VisMap2(UnMultiply(), dst, src); 
}
*/
//
// Explicit template instantiation
//

template CVisPyramid<CVisByteImage, CVisFloatImage>;
template CVisPyramid<CVisRGBAByteImage, CVisRGBAFloatImage>;
//template CVisPyramid<CVisRGBAIntImage>;
template CVisPyramid<CVisFloatImage, CVisFloatImage>;
