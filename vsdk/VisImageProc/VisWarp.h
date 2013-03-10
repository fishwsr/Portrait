///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisResample.h -- image resampling (zoom, warp) routines
//
// SPECIFICATION
//  void VisInverseWarp(Image &src, Image &dst, CVisFloatImage &uv,
//          TVisInterpolation interpolation = VIS_INTERP_LINEAR,
//          int uv_spacing = 1, int zero_outside = 1);
//  void VisForwardWarp(Image &src, Image &dst, CVisFloatImage &uv,
//          TVisInterpolation interpolation = VIS_INTERP_LINEAR,
//          int uv_spacing = 1, int zero_outside = 1);
//  
//
// PARAMETERS
//  src             source image ("texture map")
//  dst             destination image (resampled image)
//  uv              floating point "texture coordinate" image
//  interpolation   VIS_INTERP_NEAREST or VIS_INTERP_LINEAR (bilinear)
//  uv_spacing      spacing of (u,v) samples (can be multiples of 1 pixel)
//  zero_outside    set pixels outside of warped src to 0
//  
// DESCRIPTION
//  These routines are used for general image warping,
//  e.g., for morphing or depth-corrected resampling.
//
//  There are currently two variants implemented or anticipated:
//   1. inverse warp, where each pixel in dst has an associated (u,v)
//      coordinate in src
//   2. forward warp, where each pixel in src has an associated (u,v)
//      coordinate in dst
//
//  The first version is reasonably fast, just looking up the appropriate
//  src pixel for each dst pixel.  Optionally, a uv_spacing can be
//  specified so that only lower-resolution (u,v) coordinates are supplied.
//  Note that (u,v) refers to exact pixel coordinates in src, and NOT
//  to a displacement (flow).  Flow vectors should be converted to
//  (u,v) (i.e., pixel addresses (x,y)) externally.
//
//  The second version is slower, since a rendering algorithm must
//  be invoked.  The effect of this routine is to render each triangle
//  in the (u,v) grid (squares are broken arbitrarily into two triangles)
//  into the dst array using the src pixels as a texture map.  This is
//  much less efficient than inverse rendering, although the loss
//  in efficiency is decreased for larger values of uv_spacing.
//
//  Note that neither the inverse or forward warping algorithms support
//  projective texture coordinates (u,v,w).  If this is important, then
//  the correct (u,v) coordinates should be computed externally.  The
//  routine VisConvertZtoUV is provided to do this computation
//  (see VisGeometry.h).
//
//  For all resampling operations, there is a choice of two pixel
//  interpolation functions:
//      VIS_INTERP_NEAREST  nearest neighbor
//      VIS_INTERP_LINEAR   bilinear sub-pixel interpolation
//  Mip-mapping (using an image pyramid for minification) is not currently
//  supported (we may add it later to the pyramid class ...?)
//
//  The default behavior is to zero out pixels which fall outside of
//  the warped src.  This behavior can be suppressed, in which case it
//  is the caller's responsibility to first initialize dst.
//
//  While this header file provides a generic (templated) interface,
//  only specific instances have been implemented (see VisWarp.cpp).
//
// SEE ALSO
//  VisImage.h        definition of image class library
//  VisWarp.cpp       implementation of warping function
//  VisCylindrical.h  cylindrical warping functions
//
//  Vision Library      Technical Note No. 1 (Vision Technology Group)
//  Digital Image Warping, George Wolberg, IEEE Computer Society Press, 1990
//
// BUGS
//   Under Visual C++ 4.1 (or earlier), a templated function with optional
//  arguments causes the compiler to crash:
//      C:\public\VisProjectVectors\Image Test\tmpbug.cpp(29) : fatal error C1001: INTERNAL COMPILER ERROR
//      (compiler file 'msc1.cpp', line 1090)
//        Please choose the Technical Support command on the Visual C++
//  Bug filed with PSS (635-7007) on 02-Apr-96
//  A temporary work around is to re-declare all prefixes of functions
//  with optional arguments ... ugh!
//
// DESIGN
//  The treatment of boundary pixels is problematic.  Sometimes, when
//  a sub-pixel position is close to a valid pixel position, the right
//  thing to do is to return a partially valid pixel (semi-transparent).
//  This only really makes sense if the VisCore.has an associated alpha
//  channel.  To get this kind of behavior, the src image should have
//  an extra row of zero-valued alpha pixels around the edge.  It is
//  the responsibility of the caller to pad (or SubImage) the image
//  appropriately.
//
// HISTORY
//  10-May-96  Richard Szeliski (szeliski) at Microsoft
//  Split off from VisResample.h
//
//  10-Dec-96  Richard Szeliski (szeliski) at Microsoft
//  Moved cylindrical warping code to VisCylindrical.h
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_WARP_H_
#define _VIS_WARP_H_

#include "VisResample.h"  // needed for TVisInterpolation enum type

struct SVisUVCoord {
    float u, v;         // x & y coordinates in src ("texture") image
};

#ifdef TICKLES_VC_BUG
template <class TPixel>
void VisInverseWarp(CVisImage<TPixel> &src, 
                    CVisImage<TPixel> &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation = VIS_INTERP_LINEAR,
                    int uv_spacing = 1, int zero_outside = 1,
                    CVisImage<TPixel> *grad_img_ptr = 0)
#else
#if 1   // only this version works, right now
void VisInverseWarp(CVisRGBAByteImage &src, CVisRGBAByteImage &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisRGBAByteImage *grad_img_ptr);
void VisInverseWarp(CVisByteImage &src, CVisByteImage &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisByteImage *grad_img_ptr);
void VisInverseWarp(CVisFloatImage &src, CVisFloatImage &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisFloatImage *grad_img_ptr);
#else
template <class TPixel>
void VisInverseWarp(CVisImage<TPixel> &src, 
                    CVisImage<TPixel> &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisImage<TPixel> *grad_img_ptr);
#endif
#endif

#if 0   // not implemented yet
template <class TImage>
void VisForwardWarp(TImage &src, TImage &dst, CVisFloatImage &uv,
                      TVisInterpolation interpolation = VIS_INTERP_LINEAR,
                      int uv_spacing = 1, int zero_outside = 1);
#endif

#endif // _VIS_WARP_H_
