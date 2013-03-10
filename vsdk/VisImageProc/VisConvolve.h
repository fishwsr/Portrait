///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisConvolve.h -- generic image convolution routines
//
// SPECIFICATION
//  void VisConvolve(CVisImageOf &src, CVisImageOf &dst,
//                     CVisFloatImage &kernel, int subsample);
//  CVisImageOf VisConvolve(CVisImageOf &src, CVisFloatImage &kernel);
//  void VisConvolveSeparable(CVisImageOf &src, CVisImageOf &dst,
//                              CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
//                              int subsample);
//  CVisImageOf VisConvolveSeparable(CVisImageOf &src,
//                                     CVisFloatImage &h_kernel, 
//                                     CVisFloatImage &v_kernel);
//
// PARAMETERS
//  src             source (input) image
//  dst             destination (result) image
//                  should NOT share storage with src
//  kernel          convolution coefficients
//  h_kernel,       separable convolution coefficients
//    v_kernel        both kernels should be single row images
//  subsample       optional subsampling stride (for pyramids)
//                  negative numbers indicate upsampling (separable only)
//  
// DESCRIPTION
//  These routines are used for convolving images with a specified
//  kernel.  The kernel itself is specified as a small image.  The
//  origin of the kernel (0,0) should also be the origin of the image,
//  e.g., for a (0.25, 0.5, 0.25) symmetric kernel, the image origin
//  (img.SCol()) should be -1.
//
//  The separable image convolution provides a more efficient
//  implementation for separable kernels.  It can also be used for
//  pure horizontal or vertical convolutions by supplying a null
//  image (Image()) for the unused convolution kernel.
//
// SEE ALSO
//  VisImage.h        description of basic Image representation
//  VisConvolve.cpp   implementation details
//
// BUGS
//  
//
// DESIGN
//  The need to convert convolution coefficients to images is a little
//  onerous, but adds great flexibility to the code.  An example of
//  use might be
//      float Gaussian5[] = {0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f};
//      CVisFloatImage gkernel(1, 5, 0, -2, 1, 0, Gaussian5, 1);
//      CVisByteImage blurry = VisConvolveSeparable(sharp, gkernel, gkernel);
//  A few common kernels are defined in this header, and more can be added.
//
//  If the source image does not have enough padding so that it can
//  be addressed directly inside the convolution loop, then a temporary
//  image (with undefined pad values) will be created.  This situation
//  should be avoided by first pre-padding the image in the desired way.
//  NOTE:  This behavior is likely to change, as Matthew adds special
//  edge handling code ...
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

#ifndef _VIS_CONVOLVE_H_
#define _VIS_CONVOLVE_H_

extern EVisPad g_evispadConvolveEnlarge;
 
template <class CVisImageOf>
void VisConvolve(CVisImageOf &src, CVisImageOf &dst,
                   CVisFloatImage &kernel, int subsample);

template <class CVisImageOf>
CVisImageOf VisConvolve(CVisImageOf &src, CVisFloatImage &kernel);

template <class CVisImageOf>
void VisConvolveSeparable(CVisImageOf &src, CVisImageOf &dst,
                            CVisFloatImage &h_kernel, CVisFloatImage &v_kernel,
                            int subsample);

template <class CVisImageOf>
CVisImageOf VisConvolveSeparable(CVisImageOf &src,
                                   CVisFloatImage &h_kernel, 
                                   CVisFloatImage &v_kernel);

// Standard kernels you can use as arguments to the constructor

extern CVisFloatImage VisConvolutionKernel_11;
extern CVisFloatImage VisConvolutionKernel_121;
extern CVisFloatImage VisConvolutionKernel_14641;

#endif // _VIS_CONVOLVE_H_
