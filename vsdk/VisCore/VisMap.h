///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisMap.h -- image mapping functions, based on function objects (~STL)
//
// SPECIFICATION
//  template <class OPERATOR, class PIXELTYPE1, ..., class PIXELTYPEn>
//  void VisMapn(OPERATOR& fn,
//               CVisImage<PIXELTYPE1> img1, ... ,
//               CVisImage<PIXELTYPEn> imgn);
//
//  or
//
//  template <class OPERATOR, class PIXELTYPE1, ..., class PIXELTYPEn>
//  void VisMapn(const RECT& refrect, OPERATOR& fn,
//               CVisImage<PIXELTYPE1> img1, ... ,
//               CVisImage<PIXELTYPEn> imgn);
//
//
// PARAMETERS
//  refrect				optional RECT giving coordinates of pixels used
//  fn                  instance of OPERATOR class which defines operator()
//  img0..imgn          input/output images
//
//
// DESCRIPTION
//  These routines are used to iterate over a collection of images,
//  applying the same function to all pixels.  Because the function is
//  passed in as a function object, the optimized (Release) version of
//  the code should be very efficient.
//
//  The OPERATOR class should look something like:
//      struct SampleFn1 {
//          inline void operator()(byte& out, float& in)
//              { out = __max(0, __min(255, int(in))); }
//      };
//
//  This function could then be called using
//      VisMap2(SampleFn1(), byte_img, float_img);
//
//  Note that it is also possible to pass non-image (scalar) parameters
//  to the inner function by making them member variables
//      struct SampleFn2 {
//          const float scale;
//          inline SampleFn2(float s) : scale(s) {};
//          inline void operator()(byte& out, float& in)
//              { out = __max(0, __min(255, int(in * scale))); }
//      };
//      VisMap2(SampleFn2(0.5f), byte_img, float_img);
//
//  This same style can be used to create functions with side-effects,
//  e.g., functions which (almost) return a value:
//      struct SumFn1 {
//          double sum;
//          inline SumFn1() : sum(0.0) {};
//          inline void operator()(float in)
//              { sum += in; }
//      };
//      SumFn1 op1;
//      VisMap2(op1, float_img);
//      double result = op1.sum;
//  
//  A RECT specifying the coordinates whose pixels should be passed to
//  the function operator can be passed as an optional first argument to
//  the VisMap functions.  If a RECT is not specified, the intersection
//  of the (active) image RECTs is used.
//
//
// SEE ALSO
//  VisImage.h        definition of image class library
//
//
// BUGS
//  
//
// DESIGN
//  LATER:
//  This is just a first pass at the desired functionality.  In
//  particular, we don't currently handle the non-safe regions,
//  or have support for window (neighborhood) operations.
//
//  LATER:
//  The current design uses indices when enumerating the pixels in an
//  image row.  This is efficient for byte and 4-byte RGBA pixel values,
//  but it may be better to use pointers that are incremented as we go
//  through rows containing non-standard pixel values.
//
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef VIS_CORE_MAP_H
#define VIS_CORE_MAP_H


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



template <class TFnOp, class TPixel0>
inline void VisMap1(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0);

template <class TFnOp, class TPixel0>
inline void VisMap1(TFnOp& fnop, CVisImage<TPixel0>& refimage0);


// VisMap1P takes one image and one pixel value.  For each pixel in the
// image, it calls a function operator that takes two pixel values.
template <class TFnOp, class TPixel>
inline void VisMap1P(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0, const TPixel& refpixel);

template <class TFnOp, class TPixel>
inline void VisMap1P(TFnOp& fnop, CVisImage<TPixel>& refimage0,
		const TPixel& refpixel);


template <class TFnOp, class TPixel0, class TPixel1>
inline void VisMap2(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1);

template <class TFnOp, class TPixel0, class TPixel1>
inline void VisMap2(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1);


// VisMap1P1 takes an image, a pixel value, and a second image.  For each image
// (x, y, band) position, it calls a function operator that takes three pixel
// values.
template <class TFnOp, class TPixel>
inline void VisMap1P1(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		const TPixel& refpixel,
		CVisImage<TPixel>& refimage1);

template <class TFnOp, class TPixel>
inline void VisMap1P1(TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		const TPixel& refpixel,
		CVisImage<TPixel>& refimage1);


// VisMap2P takes two images and a pixel value.  For each image (x, y, band)
// position, it calls a function operator that takes three pixel values.
template <class TFnOp, class TPixel>
inline void VisMap2P(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		CVisImage<TPixel>& refimage1,
		const TPixel& refpixel);

template <class TFnOp, class TPixel>
inline void VisMap2P(TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		CVisImage<TPixel>& refimage1,
		const TPixel& refpixel);


template <class TFnOp, class TPixel0, class TPixel1, class TPixel2>
inline void VisMap3(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2);

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2>
inline void VisMap3(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2);


template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3>
inline void VisMap4(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3);

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3>
inline void VisMap4(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3);


template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4>
inline void VisMap5(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4);

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4>
inline void VisMap5(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4);


template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4, class TPixel5>
inline void VisMap5(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4,
		CVisImage<TPixel5>& refimage5);

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4, class TPixel5>
inline void VisMap5(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4,
		CVisImage<TPixel5>& refimage5);



#include "VisMap.inl"


#endif // VIS_CORE_MAP_H
