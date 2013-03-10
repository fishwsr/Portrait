////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISCONVERTRGB
//
// @module VisConvertRGB.h |
//
// This file defines a function that converts an array of grayscale pixel
// values to RGB values and functions that convert arrays of RGB pixels
// to grayscale or YUV (or YUVA) values.
//
// Inline versions of these functions are defined for use in code
// that does not use the VisImSrc DLL.  Functions are also exported
// from the VisImSrc DLL for code that does use the VisImSrc DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisConvertRGB\.inl> <l VisConvertRGB\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISCONVERTRGB_H__
#define __VIS_IMSRC_VISCONVERTRGB_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <assert.h>


#ifndef VISAPI
#define VISAPI __stdcall
#endif // VISAPI


// Constants used in RGB to YUV conversion:
// Note:
//  0x20202 == 0xffffffff / 0xffff * 2
//  0x2020202 == 0xffffffff / 0xff * 2

#define kvisdblRgbToYuvYR   .299
#define kvisdblRgbToYuvYG   .587
#define kvisdblRgbToYuvYB   .114

// These are the old values.  They require clipping when used with some RGB values.
// I think that this conversion is the inverse of the evisconvyuvBasic conversion.
//#define VIS_IMSRC_CONVERT_RGB_CLIP_V
//#define kvisdblRgbToYuvU    .492
//#define kvisdblRgbToYuvV    .877

// These are the new values.  This is the inverse of the evisconvyuv422 conversion.
#define kvisdblRgbToYuvU    .564
#define kvisdblRgbToYuvV    .712

enum EVisRGBToYUV
{
	// UNDONE:  Need to write these 4 constants in terms of the doubles above.
	evisrgbtoyuvYR5		= 0x02781af4,  // kvisdblRgbToYuvYR == .299
	evisrgbtoyuvYG5		= 0x04D8F4B3,  // kvisdblRgbToYuvYG == .587
	evisrgbtoyuvYG6		= 0x0262a10b,  // kvisdblRgbToYuvYG == .587
	evisrgbtoyuvYB5		= 0x00f100db,  // kvisdblRgbToYuvYB == .114

	evisrgbtoyuvYR8		= (((long) (0x2020202 * kvisdblRgbToYuvYR + 1)) >> 1),
	evisrgbtoyuvYG8		= (((long) (0x2020202 * kvisdblRgbToYuvYG + 1)) >> 1),
	evisrgbtoyuvYB8		= (((long) (0x2020202 * kvisdblRgbToYuvYB + 1)) >> 1),
	evisrgbtoyuvU8		= (((long) (0x2020202 * kvisdblRgbToYuvU + 1)) >> 1),
	evisrgbtoyuvV6		= (((long) (0x2020202 * kvisdblRgbToYuvV + 1)) >> 3),

	evisrgbtoyuvYR16	= (((long) (0x20202 * kvisdblRgbToYuvYR + 1)) >> 1),
	evisrgbtoyuvYG16	= (((long) (0x20202 * kvisdblRgbToYuvYG + 1)) >> 1),
	evisrgbtoyuvYB16	= (((long) (0x20202 * kvisdblRgbToYuvYB + 1)) >> 1),
	evisrgbtoyuvU16		= (((long) (0x20202 * kvisdblRgbToYuvU + 1)) >> 1),
	evisrgbtoyuvV14		= (((long) (0x20202 * kvisdblRgbToYuvV + 1)) >> 3)
};



///////////////////////////////////////////////////////////////////////////
// Functions defined in VisImSrcb.lib.


// Supported conversions:  {1, 2, 4, 8, 16}-bit gray to 32-bit RGBA.
VisImSrcExport void VISAPI VisGrayToRGB(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


// Supported conversions:  {15, 16, 24, 32, 48, 64}-bit RGB to
// {8, 16, 32}-bit gray.
VisImSrcExport void VISAPI VisRGBToGray(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


// Supported conversions:  {15, 16, 24, 32, 48, 64}-bit RGB to
// {24, 32, 48, 64}-bit YUV.
VisImSrcExport void VISAPI VisRGBToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);



#endif // __VIS_IMSRC_VISCONVERTRGB_H__

