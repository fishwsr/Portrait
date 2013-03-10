///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlend.h -- image blending and compositing routines
//
// SPECIFICATION
//  void VisComposite(CVisRGBAByteImage &src, CVisRGBAByteImage &dst,
//                      EVisComposite = composite_over);
//
//  void VisAccumulate(CVisRGBAByteImage &src, CVisFloatImage &accumulator,
//                       CVisFloatImage &weight);
//  CVisRGBAByteImage VisAccumulateVisMatrixNormalize(CVisFloatImage &accumulator);
//
//  void VisPremultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst);
//  void VisUnmultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst);
//
// PARAMETERS
//  src             source image
//  dst             destination or result image
//  accumulator     accumulation buffer (A-buffer)
//  weight          weighting image or single weight for the whole image
//  
// DESCRIPTION
//  These routines are used for blending images, using either image
//  compositing operators (such as over), or using a weighted average
//  (A-buffer).  Eventually, we would also like to have a weighted
//  median or some other robust averaging operator.
//
//  Routines are also provided for premultiplying and unmultiplying
//  the alpha channel.
//
// SEE ALSO
//  VisBlend.cpp      resampling algorithm implementation
//
//  [Porter & Duff 84]	Thomas Porter and Tom Duff, 
//      Compositing Digital Images, Computer Graphics (SIGGRAPH’84),
//      18(3), 1984, pp. 253–259.
//  [Smith 95a]	Alvy Ray Smith, Image Compositing Fundamentals,
//       Technical Memo 4, Microsoft Corporation, July 1995.
//  [Smith 95b]	Alvy Ray Smith, A Sprite Theory of Image Computing,
//      Technical Memo 5, Microsoft Corporation, July 1995.
//  [Blinn ??] Alpha Compositing Fundamentals,
//      Computer Graphics and Applications
//  [Blinn 94a]	James F. Blinn, Compositing, Part 1: Theory 
//      (Jim Blinn’s Corner), IEEE Computer Graphics and Applications, 
//      14(5), September 1994, pp. 83-87.
//  [Blinn 94b]	James F. Blinn, Compositing, Part 2: Practice 
//      (Jim Blinn’s Corner), IEEE Computer Graphics and Applications,
//      14(6), November 1994, pp. 78-82.
//  [Blinn 96] Jim Blinn, Implementation Guide to Alpha Blending.
//      Internal memo, Microsoft Corporation, Jun 1996.
//  [Szeliski 96] Richard Szeliski, Fast Color Pixel Blends, 
//      Technical Note No. 2 (Vision Technology Group), May 1996
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  16-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//  03-Aug-99  Arno Schoedl (arno@schoedl.org)
//  Added single weight for the whole image and multiple band support.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_BLEND_H_
#define _VIS_BLEND_H_

enum EVisComposite {
    composite_over          = 0,    // Porter & Duff OVER (frgd + (1-frgd.A)bkgd)
    composite_accumulate    = 1,    // add up then return the average
    composite_feather       = 2,    // add up with center weighting
    composite_median        = 3,    // median (build pixel lists, then find median)
    composite_minimum       = 4,    // minimum of both images
    composite_maximum       = 5,    // maximum of both images
    composite_sum           = 6,    // per-band sum, ignoring alpha, clipped to 255
    composite_pyramid_feather = 7,  // pyramid blending with feather weight
    composite_average       = 8,    // average frgd/bkgd (transparency)
    composite_screen        = 9,    // alternate frgd/bkgd (transparency)
    composite_eol
    // more to come ...
};

void VisComposite(CVisRGBAByteImage &frgb, CVisRGBAByteImage &bkgd, CVisRGBAByteImage &dst,
                    EVisComposite op = composite_over);

void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAIntImage &accumulator,
                     CVisByteImage &weight, int decrement);
void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAFloatImage &accumulator,
                     CVisFloatImage &weight, int decrement);
void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAIntImage &accumulator,
                     int weight, int decrement);
void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAFloatImage &accumulator,
                     float weight, int decrement);

void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAIntPixel> &accumulator,
                     CVisByteImage &weight, int decrement);
void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     CVisFloatImage &weight, int decrement);
void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAIntPixel> &accumulator,
                     int weight, int decrement);
void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     float weight, int decrement);

void VisAccumulate(CVisGrayFloatImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     float weight, int decrement);

void VisAccumulateNormalize(CVisRGBAFloatImage &accumulator, CVisRGBAByteImage &dst,
                              int empty_value = 0);
void VisAccumulateNormalize(CVisRGBAIntImage &accumulator, CVisRGBAByteImage &dst,
                              int empty_value = 0);
void VisAccumulateNormalize(CVisImage<CVisGrayAFloatPixel> &accumulator, CVisGrayFloatImage &dst,
                            int empty_value = 0);

CVisByteImage VisCityBlockDistance(CVisRGBAByteImage &src, int max_dist = 255,
                                   int raised_cosine = 1);

CVisFloatImage VisFloatCityBlockDistance(CVisRGBAByteImage &src, int max_dist = 255,
                                   int raised_cosine = 1);

void VisPremultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst);
void VisUnmultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst);

#endif // _VIS_BLEND_H_
