///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPIXEL
//
// @module VisPixFmt.h |
//
// This file defines the constant values used to describe standard
// pixel types used in images.  It also provides a templated-function
// to get a constant describing a given pixel type.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPixel\.inl> <l VisImage\.h>
//
// <nl>
// @index | VISPIXEL
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_PIXEL_H__
#define __VIS_CORE_PIXEL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "..\VisImSrc\VisImEncoding.h"



////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisPixFmt |
// Standard pixel types
//
////////////////////////////////////////////////////////////////////////////
enum EVisPixFmt
{
	// @emem
	// Pixel type has not been determined yet.
	evispixfmtNil				= - 1,

	// @emem
	// Pixel type is not known.
	evispixfmtUnknown			= 0x0000,

	//------------------------------------------------------------------
	// @group Numeric Types

	// @emem
	// Values for this type are <t char> or <t unsigned char> values.
	evispixfmtChar				= 0x0001,

	// @emem
	// Values for this type are <t short> or <t unsigned short> values.
	evispixfmtShort				= 0x0002,

	// @emem
	// Values for this type are <t int> or <t unsigned int> values.
	evispixfmtInt				= 0x0003,

	// @emem
	// Values for this type are <t long> or <t unsigned long> values.
	evispixfmtLong				= 0x0004,

	// @emem
	// Values for this type are <t float> values.
	evispixfmtFloat				= 0x0005,

	// @emem
	// Values for this type are <t double> values.
	evispixfmtDouble				= 0x0006,

	// @emem
	// Mask for the numeric type information.
	evispixfmtNumericTypeMask	= 0x003f,

	//------------------------------------------------------------------
	// @group Flag for Signed or Unsigned Integer Types

	// @emem
	// Indicates that this type is <t signed>.  Not used with
	// <e EVisPixFmt::evispixfmtFloat> or  <e EVisPixFmt::evispixfmtDouble>.
	evispixfmtSigned				= 0x0000,

	// @emem
	// Indicates that this type is <t unsigned>.  Not used with
	// <e EVisPixFmt::evispixfmtFloat> or  <e EVisPixFmt::evispixfmtDouble>.
	evispixfmtUnsigned			= 0x0040,

	// @emem
	// Mask for the <t signed> of <t unsigned> integer type information.
	evispixfmtUnsignedMask		= 0x0040,

	//------------------------------------------------------------------
	// @group Unsigned Integer Types

	// @emem
	// Values for this type are <t unsigned char> values.
	evispixfmtUChar				= evispixfmtChar | evispixfmtUnsigned,

	// @emem
	// Values for this type are <t unsigned short> values.
	evispixfmtUShort			= evispixfmtShort | evispixfmtUnsigned,

	// @emem
	// Values for this type are <t unsigned int> values.
	evispixfmtUInt				= evispixfmtInt | evispixfmtUnsigned,

	// @emem
	// Values for this type are <t unsigned long> values.
	evispixfmtULong				= evispixfmtLong | evispixfmtUnsigned,

	//------------------------------------------------------------------
	// @group Flag indicating whether numeric type information is
	// used with this pixel type.

	// @emem
	// Indicates that the numeric type information is not valid.
	evispixfmtNumInvalid		= 0x0000,

	// @emem
	// Indicates that the numeric type information is valid.
	evispixfmtNumValid			= 0x0080,

	// @emem
	// Mask for the flag indicating whether numeric type information
	// is valid.
	evispixfmtNumValidMask		= 0x0080,

	//------------------------------------------------------------------
	// @group Structure of Information in Pixel

	// @emem
	// The pixel consists of a single value of this type.
	evispixfmtGray				= 0x0100,

	// @emem
	// The pixel consists of a structure containing red, green, blue,
	// and alpha values of this type.
	// @xref <l VisImageTypes\.h>
	evispixfmtRGBA				= 0x0200,

	// @emem
	// The pixel consists of a structure containing red, green, and
	// blue values of this type.
	evispixfmtRGB				= 0x0300,

	// @emem
	// The pixel consists of a structure containing intensity (Y),
	// chroma (U and V), and alpha values of this type.
	evispixfmtYUVA				= 0x0400,

	// @emem
	// The pixel consists of a structure containing intensity (Y),
	// and chroma (U and V) values of this type.
	evispixfmtYUV				= 0x0500,

	// @emem
	// The pixel is a type that is used in the raw image that we
	// get from a digitizer.  The low 6 bits of this value give the
	// <t EVisPixFmt> value that describes this type.  Most users
	// of the SDK will not need to use the pixel types.
	evispixfmtImEncoding			= 0x2000,

	// @emem
	// The pixel consists of another structure which can be cast to
	// and constructed from a value of this type.
	evispixfmtOther				= 0x4000,

	// @emem
	// Mask for the pixel structure information.
	evispixfmtStructureMask		= 0x7f00,

	//------------------------------------------------------------------
	// @group Common Grayscale Pixel Types

	// @emem
	// Grayscale <c char> pixels
	evispixfmtGrayChar = evispixfmtGray | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtChar,

	// @emem
	// Grayscale <c short> pixels
	evispixfmtGrayShort = evispixfmtGray | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtShort,

	// @emem
	// Grayscale <c int> pixels
	evispixfmtGrayInt = evispixfmtGray | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtInt,

	// @emem
	// Grayscale <c long> pixels
	evispixfmtGrayLong = evispixfmtGray | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtLong,

	// @emem
	// Grayscale <c BYTE> pixels
	evispixfmtGrayByte = evispixfmtGray | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtChar,

	// @emem
	// Grayscale <c unsigned short> pixels
	evispixfmtGrayUShort = evispixfmtGray | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtShort,

	// @emem
	// Grayscale <c unsigned int> pixels
	evispixfmtGrayUInt = evispixfmtGray | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtInt,

	// @emem
	// Grayscale <c unsigned long> pixels
	evispixfmtGrayULong = evispixfmtGray | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtLong,

	// @emem
	// Grayscale <c float> pixels
	evispixfmtGrayFloat = evispixfmtGray | evispixfmtNumValid
			| evispixfmtFloat,

	// @emem
	// Grayscale <c double> pixels
	evispixfmtGrayDouble = evispixfmtGray | evispixfmtNumValid
			| evispixfmtDouble,

	//------------------------------------------------------------------
	// @group Common Color (RGBA) Pixel Types

	// @emem
	// Color (RGBA) <c char> pixels
	evispixfmtRGBAChar = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtChar,

	// @emem
	// Color (RGBA) <c short> pixels
	evispixfmtRGBAShort = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtShort,

	// @emem
	// Color (RGBA) <c int> pixels
	evispixfmtRGBAInt = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtInt,

	// @emem
	// Color (RGBA) <c long> pixels
	evispixfmtRGBALong = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtLong,

	// @emem
	// Color (RGBA) <c BYTE> pixels
	evispixfmtRGBAByte = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtChar,

	// @emem
	// Color (RGBA) <c unsigned short> pixels
	evispixfmtRGBAUShort = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtShort,

	// @emem
	// Color (RGBA) <c unsigned int> pixels
	evispixfmtRGBAUInt = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtInt,

	// @emem
	// Color (RGBA) <c unsigned long> pixels
	evispixfmtRGBAULong = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtLong,

	// @emem
	// Color (RGBA) <c float> pixels
	evispixfmtRGBAFloat = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtFloat,

	// @emem
	// Color (RGBA) <c double> pixels
	evispixfmtRGBADouble = evispixfmtRGBA | evispixfmtNumValid
			| evispixfmtDouble,

	//------------------------------------------------------------------
	// @group Common Color (YUVA) Pixel Types

	// @emem
	// Color (YUVA) <c char> pixels
	evispixfmtYUVAChar = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtChar,

	// @emem
	// Color (YUVA) <c short> pixels
	evispixfmtYUVAShort = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtShort,

	// @emem
	// Color (YUVA) <c int> pixels
	evispixfmtYUVAInt = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtInt,

	// @emem
	// Color (YUVA) <c long> pixels
	evispixfmtYUVALong = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtSigned | evispixfmtLong,

	// @emem
	// Color (YUVA) <c BYTE> pixels
	evispixfmtYUVAByte = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtChar,

	// @emem
	// Color (YUVA) <c unsigned short> pixels
	evispixfmtYUVAUShort = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtShort,

	// @emem
	// Color (YUVA) <c unsigned int> pixels
	evispixfmtYUVAUInt = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtInt,

	// @emem
	// Color (YUVA) <c unsigned long> pixels
	evispixfmtYUVAULong = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtUnsigned | evispixfmtLong,

	// @emem
	// Color (YUVA) <c float> pixels
	evispixfmtYUVAFloat = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtFloat,

	// @emem
	// Color (YUVA) <c double> pixels
	evispixfmtYUVADouble = evispixfmtYUVA | evispixfmtNumValid
			| evispixfmtDouble,

	//------------------------------------------------------------------
	// @group Special 16bpp YUV 4:2:2 Structures

	// @emem
	// Color 16bpp <c SVisYUYV> structures
	evispixfmtYUYV = evispixfmtImEncoding | evispixfmtNumInvalid
			| evisimencodingYUY2,

	// @emem
	// Color 16bpp <c SVisYVYU> structures
	evispixfmtYVYU = evispixfmtImEncoding | evispixfmtNumInvalid
			| evisimencodingYVYU,

	// @emem
	// Color 16bpp <c SVisUYVY> structures
	evispixfmtUYVY = evispixfmtImEncoding | evispixfmtNumInvalid
			| evisimencodingUYVY
};


// Old functions used to get an EVisPixFmt value from a reference to a pixel.
template<class TPixel>
inline EVisPixFmt VisPixFmtGetTPixel(const TPixel pixel);


// New class used to get an EVisPixFmt value (without a reference to a pixel).
template<class TPixel>
class CVisPixFmtGetTPixel
{
public:
	static EVisPixFmt PixFmt(void);
};



#include "VisPixFmt.inl"


#endif // __VIS_CORE_PIXEL_H__
