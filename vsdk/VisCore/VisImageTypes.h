///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGETYPES
//
// @module VisImageTypes.h |
//
// Type definitions for images using the standard numeric and RGBA pixel
// types.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImage\.h> <l VisImage\.inl> <l VisImage\.cpp>
// <l VisImageOf\.h> <l VisImageOf\.inl> <l VisRGBAPixels\.h>
// <l VisYUVAPixels\.h>
//
// <nl>
// @index | VISIMAGETYPES
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_IMAGETYPES_H__
#define __VIS_CORE_IMAGETYPES_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef signed char		CVisCharPixel;
typedef short			CVisShortPixel;
typedef int				CVisIntPixel;
typedef long			CVisLongPixel;
typedef unsigned char	CVisBytePixel;
typedef unsigned char	CVisUCharPixel;
typedef unsigned short	CVisUShortPixel;
typedef unsigned int	CVisUIntPixel;
typedef unsigned long	CVisULongPixel;
typedef float			CVisFloatPixel;
typedef double			CVisDoublePixel;


typedef CVisCharPixel		 CVisGrayCharPixel;
typedef CVisShortPixel		 CVisGrayShortPixel;
typedef CVisIntPixel		 CVisGrayIntPixel;
typedef CVisLongPixel		 CVisGrayLongPixel;
typedef CVisBytePixel		 CVisGrayBytePixel;
typedef CVisUCharPixel		 CVisGrayUCharPixel;
typedef CVisUShortPixel		 CVisGrayUShortPixel;
typedef CVisUIntPixel		 CVisGrayUIntPixel;
typedef CVisULongPixel		 CVisGrayULongPixel;
typedef CVisFloatPixel		 CVisGrayFloatPixel;
typedef CVisDoublePixel		 CVisGrayDoublePixel;


// @type CVisCharImage | <c CVisImage>\<signed char>
// @xref <l VisImageTypes\.h>
typedef CVisImage<signed char>   CVisCharImage;

// @type CVisShortImage | <c CVisImage>\<short>
// @xref <l VisImageTypes\.h>
typedef CVisImage<short>  CVisShortImage;

// @type CVisIntImage | <c CVisImage>\<int>
// @xref <l VisImageTypes\.h>
typedef CVisImage<int>    CVisIntImage;

// @type CVisLongImage | <c CVisImage>\<long>
// @xref <l VisImageTypes\.h>
typedef CVisImage<long>  CVisLongImage;


// @type CVisByteImage |
// <c CVisImage>\<unsigned char>
// @xref <l VisImageTypes\.h>
typedef CVisImage<unsigned char>	CVisByteImage;
typedef CVisImage<unsigned char>	CVisUCharImage;

// @type CVisUShortImage |
// <c CVisImage>\<unsigned short>
// @xref <l VisImageTypes\.h>
typedef CVisImage<unsigned short>	CVisUShortImage;

// @type CVisUIntImage |
// <c CVisImage>\<unsigned int>
// @xref <l VisImageTypes\.h>
typedef CVisImage<unsigned int>	CVisUIntImage;

// @type CVisULongImage |
// <c CVisImage>\<unsigned long>
// @xref <l VisImageTypes\.h>
typedef CVisImage<unsigned long>	CVisULongImage;


// @type CVisFloatImage | <c CVisImage>\<float>
// @xref <l VisImageTypes\.h>
typedef CVisImage<float>  CVisFloatImage;

// @type CVisDoubleImage | <c CVisImage>\<double>
// @xref <l VisImageTypes\.h>
typedef CVisImage<double> CVisDoubleImage;


typedef CVisCharImage		 CVisGrayCharImage;
typedef CVisShortImage		 CVisGrayShortImage;
typedef CVisIntImage		 CVisGrayIntImage;
typedef CVisLongImage		 CVisGrayLongImage;
typedef CVisByteImage		 CVisGrayByteImage;
typedef CVisUCharImage		 CVisGrayUCharImage;
typedef CVisUShortImage		 CVisGrayUShortImage;
typedef CVisUIntImage		 CVisGrayUIntImage;
typedef CVisULongImage		 CVisGrayULongImage;
typedef CVisFloatImage		 CVisGrayFloatImage;
typedef CVisDoubleImage		 CVisGrayDoubleImage;


// @type CVisRGBACharImage | <c CVisImage>\<<c CVisRGBACharPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBACharPixel>   CVisRGBACharImage;

// @type CVisRGBAShortImage | <c CVisImage>\<<c CVisRGBAShortPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAShortPixel>  CVisRGBAShortImage;

// @type CVisRGBAIntImage | <c CVisImage>\<<c CVisRGBAIntPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAIntPixel>    CVisRGBAIntImage;

// @type CVisRGBALongImage | <c CVisImage>\<<c CVisRGBALongPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBALongPixel>  CVisRGBALongImage;


// @type CVisRGBAByteImage | <c CVisImage>\<<c CVisRGBABytePixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBABytePixel>   CVisRGBAByteImage;

// @type CVisRGBAUShortImage | <c CVisImage>\<<c CVisRGBAUShortPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAUShortPixel>  CVisRGBAUShortImage;

// @type CVisRGBAUIntImage | <c CVisImage>\<<c CVisRGBAUIntPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAUIntPixel>    CVisRGBAUIntImage;

// @type CVisRGBAULongImage | <c CVisImage>\<<c CVisRGBAULongPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAULongPixel>  CVisRGBAULongImage;


// @type CVisRGBAFloatImage | <c CVisImage>\<<c CVisRGBAFloatPixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBAFloatPixel>  CVisRGBAFloatImage;

// @type CVisRGBADoubleImage | <c CVisImage>\<<c CVisRGBADoublePixel>>
// @xref <l VisImageTypes\.h> <l VisRGBAPixels\.h>
typedef CVisImage<CVisRGBADoublePixel> CVisRGBADoubleImage;




// @type CVisYUVACharImage | <c CVisImage>\<<c CVisYUVACharPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVACharPixel>		CVisYUVACharImage;

// @type CVisYUVAShortImage | <c CVisImage>\<<c CVisYUVAShortPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAShortPixel>		CVisYUVAShortImage;

// @type CVisYUVAIntImage | <c CVisImage>\<<c CVisYUVAIntPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAIntPixel>		CVisYUVAIntImage;

// @type CVisYUVALongImage | <c CVisImage>\<<c CVisYUVALongPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVALongPixel>		CVisYUVALongImage;


// @type CVisYUVAByteImage | <c CVisImage>\<<c CVisYUVABytePixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVABytePixel>		CVisYUVAByteImage;

// @type CVisYUVAUShortImage | <c CVisImage>\<<c CVisYUVAUShortPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAUShortPixel>	CVisYUVAUShortImage;

// @type CVisYUVAUIntImage | <c CVisImage>\<<c CVisYUVAUIntPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAUIntPixel>		CVisYUVAUIntImage;

// @type CVisYUVAULongImage | <c CVisImage>\<<c CVisYUVAULongPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAULongPixel>		CVisYUVAULongImage;


// @type CVisYUVAFloatImage | <c CVisImage>\<<c CVisYUVAFloatPixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVAFloatPixel>		CVisYUVAFloatImage;

// @type CVisYUVADoubleImage | <c CVisImage>\<<c CVisYUVADoublePixel>>
// @xref <l VisImageTypes\.h> <l VisYUVAPixels\.h>
typedef CVisImage<CVisYUVADoublePixel>	CVisYUVADoubleImage;



// #define VIS_OLD_COLOR_NAMES

#ifdef VIS_OLD_COLOR_NAMES

#define CVisColorCharImage			CVisRGBACharImage
#define	CVisColorShortImage			CVisRGBAShortImage
#define	CVisColorIntImage			CVisRGBAIntImage
#define	CVisColorLongImage			CVisRGBALongImage
#define	CVisColorByteImage			CVisRGBAByteImage
#define	CVisColorUShortImage		CVisRGBAUShortImage
#define	CVisColorUIntImage			CVisRGBAUIntImage
#define	CVisColorULongImage			CVisRGBAULongImage
#define	CVisColorFloatImage			CVisRGBAFloatImage
#define	CVisColorDoubleImage		CVisRGBADoubleImage

#endif // VIS_OLD_COLOR_NAMES


#ifdef VIS_OLD_MULTIBAND_NAMES

#define CVisByteMultibandImage			CVisByteMultibandImage
#define	CVisIntMultibandImage			CVisIntMultibandImage
#define	CVisShortMultibandImage			CVisShortMultibandImage
#define	CVisFloatMultibandImage			CVisFloatMultibandImage
#define	CVisDoubleMultibandImage		CVisDoubleMultibandImage

#endif // VIS_OLD_MULTIBAND_NAMES


#endif // __VIS_CORE_IMAGETYPES_H__
