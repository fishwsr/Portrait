///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPIXEL
//
// @module VisPixFmt.inl |
//
// This file defines the templated-function used to get a constant
// describing a given pixel type.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPixFmt\.h> <l VisImage\.h>
//
// <nl>
// @index | VISPIXEL
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_PIXEL_INL__
#define __VIS_CORE_PIXEL_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



///////////////////////////////////////////////////////////////////////////
// Old functions used to get an EVisPixFmt value from a reference to a pixel.


// Default:  Unknown pixel type.
template<class TPixel>
inline EVisPixFmt VisPixFmtGetTPixel(const TPixel pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtUnknown;
}


// Grayscale values
inline EVisPixFmt VisPixFmtGetTPixel(const signed char pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayChar;
}

inline EVisPixFmt VisPixFmtGetTPixel(const signed short pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayShort;
}

inline EVisPixFmt VisPixFmtGetTPixel(const signed int pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayInt;
}

inline EVisPixFmt VisPixFmtGetTPixel(const signed long pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayLong;
}

inline EVisPixFmt VisPixFmtGetTPixel(const unsigned char pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayByte;
}

inline EVisPixFmt VisPixFmtGetTPixel(const unsigned short pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayUShort;
}

inline EVisPixFmt VisPixFmtGetTPixel(const unsigned int pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayUInt;
}

inline EVisPixFmt VisPixFmtGetTPixel(const unsigned long pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayULong;
}

inline EVisPixFmt VisPixFmtGetTPixel(const float pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayFloat;
}

inline EVisPixFmt VisPixFmtGetTPixel(const double pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtGrayDouble;
}


// RGBA color values
inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<signed char> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAChar;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<signed short> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAShort;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<signed int> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAInt;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<signed long> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBALong;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<unsigned char> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAByte;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<unsigned short> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAUShort;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<unsigned int> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAUInt;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<unsigned long> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAULong;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<float> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBAFloat;
}

inline EVisPixFmt VisPixFmtGetTPixel(const CVisRGBA<double> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtRGBADouble;
}


// YUVA color values
inline EVisPixFmt VisPixFmtGetTPixel(
		const CVisYUVA<unsigned char, signed char> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYUVAByte;
}

inline EVisPixFmt VisPixFmtGetTPixel(
		const CVisYUVA<unsigned short, signed short> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYUVAUShort;
}

inline EVisPixFmt VisPixFmtGetTPixel(
		const CVisYUVA<unsigned int, signed int> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYUVAUInt;
}

inline EVisPixFmt VisPixFmtGetTPixel(
		const CVisYUVA<unsigned long, signed long> pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYUVAULong;
}


inline EVisPixFmt VisPixFmtGetTPixel(const SVisYUYV pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYUYV;
}

inline EVisPixFmt VisPixFmtGetTPixel(const SVisYVYU pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtYVYU;
}

inline EVisPixFmt VisPixFmtGetTPixel(const SVisUYVY pixel)
{
	VisUnreferenced(pixel);

	return evispixfmtUYVY;
}



///////////////////////////////////////////////////////////////////////////
// New class used to get an EVisPixFmt value (without a reference to a pixel).


// Default:  Unknown pixel type.
template<class TPixel>
inline EVisPixFmt CVisPixFmtGetTPixel<TPixel>::PixFmt(void)
{
	return evispixfmtUnknown;
}


// Grayscale values
inline EVisPixFmt CVisPixFmtGetTPixel<signed char>::PixFmt(void)
{
	return evispixfmtGrayChar;
}

inline EVisPixFmt CVisPixFmtGetTPixel<signed short>::PixFmt(void)
{
	return evispixfmtGrayShort;
}

inline EVisPixFmt CVisPixFmtGetTPixel<signed int>::PixFmt(void)
{
	return evispixfmtGrayInt;
}

inline EVisPixFmt CVisPixFmtGetTPixel<signed long>::PixFmt(void)
{
	return evispixfmtGrayLong;
}

inline EVisPixFmt CVisPixFmtGetTPixel<unsigned char>::PixFmt(void)
{
	return evispixfmtGrayByte;
}

inline EVisPixFmt CVisPixFmtGetTPixel<unsigned short>::PixFmt(void)
{
	return evispixfmtGrayUShort;
}

inline EVisPixFmt CVisPixFmtGetTPixel<unsigned int>::PixFmt(void)
{
	return evispixfmtGrayUInt;
}

inline EVisPixFmt CVisPixFmtGetTPixel<unsigned long>::PixFmt(void)
{
	return evispixfmtGrayULong;
}

inline EVisPixFmt CVisPixFmtGetTPixel<float>::PixFmt(void)
{
	return evispixfmtGrayFloat;
}

inline EVisPixFmt CVisPixFmtGetTPixel<double>::PixFmt(void)
{
	return evispixfmtGrayDouble;
}


// RGBA color values
inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<signed char> >::PixFmt(void)
{
	return evispixfmtRGBAChar;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<signed short> >::PixFmt(void)
{
	return evispixfmtRGBAShort;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<signed int> >::PixFmt(void)
{
	return evispixfmtRGBAInt;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<signed long> >::PixFmt(void)
{
	return evispixfmtRGBALong;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<unsigned char> >::PixFmt(void)
{
	return evispixfmtRGBAByte;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<unsigned short> >::PixFmt(void)
{
	return evispixfmtRGBAUShort;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<unsigned int> >::PixFmt(void)
{
	return evispixfmtRGBAUInt;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<unsigned long> >::PixFmt(void)
{
	return evispixfmtRGBAULong;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<float> >::PixFmt(void)
{
	return evispixfmtRGBAFloat;
}

inline EVisPixFmt CVisPixFmtGetTPixel<CVisRGBA<double> >::PixFmt(void)
{
	return evispixfmtRGBADouble;
}


// YUVA color values
inline EVisPixFmt
	CVisPixFmtGetTPixel<CVisYUVA<unsigned char, signed char> >::PixFmt(void)
{
	return evispixfmtYUVAByte;
}

inline EVisPixFmt
	CVisPixFmtGetTPixel<CVisYUVA<unsigned short, signed short> >::PixFmt(void)
{
	return evispixfmtYUVAUShort;
}

inline EVisPixFmt
	CVisPixFmtGetTPixel<CVisYUVA<unsigned int, signed int> >::PixFmt(void)
{
	return evispixfmtYUVAUInt;
}

inline EVisPixFmt
	CVisPixFmtGetTPixel<CVisYUVA<unsigned long, signed long> >::PixFmt(void)
{
	return evispixfmtYUVAULong;
}


inline EVisPixFmt CVisPixFmtGetTPixel<SVisYUYV>::PixFmt(void)
{
	return evispixfmtYUYV;
}

inline EVisPixFmt CVisPixFmtGetTPixel<SVisYVYU>::PixFmt(void)
{
	return evispixfmtYVYU;
}

inline EVisPixFmt CVisPixFmtGetTPixel<SVisUYVY>::PixFmt(void)
{
	return evispixfmtUYVY;
}



// LATER:  RGB, YUV, and other EVisImEncoding values?


#endif // __VIS_CORE_PIXEL_INL__
