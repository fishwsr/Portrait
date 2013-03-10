///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisProc.h -- main Image Processing header file and assorted routines
//
// DESCRIPTION
//  Some assorted image processing routines which don't fit into
//  another category for now.
//
// SEE ALSO
//  
//
// BUGS
//  
//
// DESIGN
//
//
// HISTORY
//  01-Mar-96  Richard Szeliski (szeliski) at Microsoft
//  Created as ImageProc.h.
//
//  11-Nov-96  Richard Szeliski (szeliski) at Microsoft
//  Split off into VisProcMisc.h.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_PROC_MISC_H_
#define _VIS_PROC_MISC_H_


// func Check that all arguments are conformal and allocate a return image, if necessary.
//       A variable number of arguments is allowed. 
// tcarg class | TImage | Typed image.
// syntax template \<class TImage> inline TImage VisSetupResult(TImage *resultptr, TImage *src1)
// syntax template \<class TImage> inline TImage VisSetupResult(TImage *resultptr, TImage *src1, TImage *src2, ...)
// parm TImage * | resultptr | Return image.
// parm TImage * | src1 | First source image.
// parm TImage * | src2 | Second source image.
// parmvar Last argument must be 0, e.g., VisSetupResult(res, im1, im2, 0);
#ifdef LATER // May not be needed.  (Old implementation was bad.)
template <class TImage>
inline TImage VisSetupResult(int dxBorder, int dyBorder, TImage *resultptr, TImage *src1)
{
    if (resultptr == 0) {
        resultptr = new TImage;
		CVisShape shapeBorder(src1->Shape());
		shapeBorder.InflateRect(dxBorder, dyBorder);
        resultptr->Allocate(src1->Shape(), shapeBorder);
    }
    VisCheckConformal(resultptr, src1);
    return *resultptr;
}

template <class TImage> 
inline TImage VisSetupResult(int dxBorder, int dyBorder, TImage *resultptr, TImage *src1, TImage *src2, ...)
{
    if (resultptr == 0) {
        resultptr = new TImage;
		CVisShape shapeBorder(src1->Shape());
		shapeBorder.InflateRect(dxBorder, dyBorder);
        resultptr->Allocate(src1->Shape(), shapeBorder);
    }

    VisCheckConformal(resultptr, src1);
    VisCheckConformal(resultptr, src2);

    va_list vl;
    va_start(vl, src2);
    TImage *nextim = va_arg(vl, TImage *);

    while (nextim)
    {
        VisCheckConformal(resultptr, nextim);
        nextim = va_arg(vl, TImage *);
    }

    return *resultptr;
}
#endif // LATER


// func Reallocate if necessary to make result identical shape to img/subsampled.
// tcarg class | TImage | Typed image.
template <class TImage>
inline void VisMakeSimilar(
                           int dxBorder,
						   int dyBorder,
						   TImage &result,  // parm Result image.
                           TImage &img,     // parm Source image.
                           int subsample)       // parm Subsample factor for source image.
{
	CVisShape shapeBorder(img.Shape().Decimated(subsample));
	shapeBorder.InflateRect(dxBorder, dyBorder);

    if ((result.Shape() != img.Shape().Decimated(subsample))
			|| (result.MemoryShape() != shapeBorder))
	{
        result.Allocate(img.Shape().Decimated(subsample), shapeBorder);
	}
}



//
// Subsample(): keep only every nth pixel
//

template <class TImage>
inline TImage VisSubSample(TImage &src, int rate)
{
    TImage result = src;
    CVisShape s(src.Shape());
	s.Decimate(rate);
    result.Allocate(s);
    VisSubSample(src, result, rate);
    return result;
}

template <class TImage>
inline void VisSubSample(TImage &src, TImage &result, int rate)
{
    assert(result.Shape() == src.Shape().Decimated(rate));
    VisSubSample((CVisImageBase *) &src, (CVisImageBase *) &result, rate);
}

// Subsampling can be implemented without knowing the pixel type
void VisSubSample(CVisImageBase *src, CVisImageBase *dst, int rate);

//
// Supersample(): replicate each pixel n times
//

template <class TImage>
inline TImage VisSuperSample(TImage &src, int rate)
{
    TImage result = src;
    CVisShape s(src.Shape());
	s.ScaleUp(rate);
    result.Allocate(s);
    VisSuperSample(src, result, rate);
    return result;
}

template <class TImage>
inline void VisSuperSample(TImage &src, TImage &result, int rate)
{
    assert(result.Shape() == src.Shape().ScaledUp(rate));
    VisSuperSample((CVisImageBase *) &src, (CVisImageBase *) &result, rate);
}

// Supersampling can be implemented without knowing the pixel type
void VisSuperSample(CVisImageBase *src, CVisImageBase *dst, int rate);


#endif // _VIS_PROC_MISC_H_
