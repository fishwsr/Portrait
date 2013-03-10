////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISCONVERTCBITTOCBIT
//
// @module VisConvertCbitToCbit.h |
//
// This file defines functions that copy an array of pixels having one
// structure (Gray, YUV-YUVA, or RGB-RGBA) to an array of pixels having
// a similar structure.  While the structure of the pixels in the two
// arrays much be similar, the pixel sizes may be different, and the
// image being coppied can be vertically reflected as it is copied.
// (When converting pixels of different sizes, alpha channels are not
// considered to be important.)
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
// @xref <l VisConvertCbitToCbit\.inl> <l VisConvertCbitToCbit\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_CONVERTCBITTOCBIT_H__
#define __VIS_IMSRC_CONVERTCBITTOCBIT_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


VisImSrcExport void VISAPI VisConvert555To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisConvert565To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

// Supported grayscale conversions:  {1, 2, 4, 8, 16}-bit to {8, 16, 32}-bit
// Supported 3 or 4 band conversions: {24, 32, 48, 64}-bit to {24, 32, 48, 64}-bit
VisImSrcExport void VISAPI VisConvertCbitToCbit(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


#endif // __VIS_IMSRC_CONVERTCBITTOCBIT_H__
