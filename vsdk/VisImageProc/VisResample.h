///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisResample.h -- image resampling (zoom, rotate) routines
//
// SPECIFICATION
//  void VisZoom(Image &src, Image &dst,
//          double x_scale, double x_offset, double y_scale, double y_offset,
//          TVisInterpolation interpolation = VIS_INTERP_LINEAR,
//          int zero_outside = 1);
//
//  void VisResample(Image &src, Image &dst, CVisTransform4x4 &geometry,
//          TVisInterpolation interpolation = VIS_INTERP_LINEAR,
//          int zero_outside = 1, int composite_under = 0,
//          int true_perspective = 0,
//          CVisByteImage *depth = 0, int parallax_flow = 0,
//          double u_p = 0.0, double v_p = 0.0, double w_p = 0.0);
//
//  Image VisResample(Image &src, CVisShape &shape,
//                   CVisTransform4x4 &geometry,
//                   TVisInterpolation interpolation = VIS_INTERP_LINEAR,
//
//  float VisResampleMaxUVError;
//
//  CVisShape VisResampleShape(CVisImage &src, CVisTransform4x4 &geometry);
//
//  CVisShape Intersect(CVisShape &sh1, CVisShape &sh2);
//
// PARAMETERS
//  src                 source image ("texture map")
//  dst                 destination image (resampled image)
//  x_scale..y_offset   zoom parameters
//  geometry            4x4 homogeneous coordinate transform matrix
//  interpolation       VIS_INTERP_NEAREST or VIS_INTERP_LINEAR (bilinear)
//  geometry            4x4 homogeneous coordinate transform matrix
//  zero_outside        set pixels outside of warped src to 0
//  composite_under     draw the warped pixels UNDER dst image
//  true_perspective    do a per-pixel perspective division (slow)
//  depth               optional per-pixel out-of-plane depth
//  parallax_flow       depth image contains uv_pair corrections, not depths
//  u_p, v_p, w_p       scaled epipole (parallax) vector
//
// GLOBALS
//  VisResampleMaxUVError     maximum (u,v) error in perspective resampling
//                              (controls recursive subdivision in line sampline)
//  
// DESCRIPTION
//  These routines are used for resampling images, for example to zoom
//  in on an image in a window, to rotate or shear an image.
//   VisResample resamples src into dst, or into a fresh
//    image specified by shape.
//   VisResampleShape  returns the shape of a slightly
//    enlarged bounding box for the transformed src image.
//   Intersect intersect two bounding box areas.
//
//  The VisResample routine is optimized to handle certain special cases,
//  such as pans and zooms, as efficiently as possible.  General
//  affine/projective warping is also fairly efficient, using an
//  incremental algorithm to march through the image.  Two-pass warping
//  (resampling each scanline at regular column intervals first) would
//  probably speed up the code by 30%, but it has not been implemented
//  (it also doesn't give the same results).  In all cases, and inverse
//  algorithm is used (each dst pixel is looked up in src).
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
//  only specific instances have been implemented (see VisResample.cpp).
//
// SEE ALSO
//  VisWarp.h         more general warping function
//  VisImage.h        definition of image class library
//  VisGeometry.h     definition of geometric transformations
//  VisResample.cpp   resampling algorithm implementation
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
//  01-Apr-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_RESAMPLE_H_
#define _VIS_RESAMPLE_H_

enum TVisInterpolation {
    VIS_INTERP_NEAREST,     // nearest neighbor interpolation (replicate)
    VIS_INTERP_LINEAR,      // bilinear interpolation
    VIS_INTERP_CUBIC        // bicubic interpolation
};

template <class TImage>
void VisZoom(TImage &src, TImage &dst,
             double x_scale, double x_offset,
             double y_scale, double y_offset,
             TVisInterpolation interpolation = VIS_INTERP_LINEAR,
             int zero_outside = 1);

template <class TImage>
TImage VisZoomed(TImage &src, double zoom,
                      TVisInterpolation interpolation);

template <class TImage>
void VisResample(TImage &src, TImage &dst,
                 CVisTransform4x4 geometry,
                 TVisInterpolation interpolation = VIS_INTERP_LINEAR,
                 int zero_outside = 1, int composite_under = 0,
                 int true_perspective = 0,
                 CVisByteImage *depth = 0, int parallax_flow = 0,
                 double u_p = 0.0, double v_p = 0.0, double w_p = 0.0);

template <class TImage>
void VisSphericalResample(TImage &src, TImage &dst, 
                          CVisTransform4x4 TVR, 
                          TVisInterpolation interpolation = VIS_INTERP_LINEAR, 
                          int spherical = 1); 

template <class TImage>
TImage VisResample(TImage &src, CVisShape shape,
                        CVisTransform4x4 geometry,
                        TVisInterpolation interpolation = VIS_INTERP_LINEAR);

extern float VisResampleMaxUVError;

CVisByteImage VisForwardWarpDepthMap(CVisByteImage& depth, CVisByteImage& warped,
                                     double u_p, double v_p, double w_p,
                                     int d_min, int d_max,
                                     int true_perspective = 0, int parallax_flow = 0,
                                     int gap_size = 1, int splat_size = 1,
                                     int enlarge_by = 8);

CVisShape VisResampleShape(CVisShape sh, const CVisTransform4x4 &geometry,
                              float max_stretch = 2.0);

void VisResampleCorners(CVisShape sh, const CVisTransform4x4 &m,
                        CVisVector4 uvw[4]);
void VisResampleOutline(CVisShape sh, const CVisTransform4x4 &m,
                        int line[4][2][2], double zoom = 1.0);
                                                  
// For testing purposes...
void VisResampleTestLoop(CVisRGBAByteImage &cimg, CVisRGBAByteImage &rimg,
                           void (*callback)(CVisRGBAByteImage &img));

#endif // _VIS_RESAMPLE_H_
