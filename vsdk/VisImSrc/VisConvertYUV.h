////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISCONVERTYUV
//
// @module VisConvertYUV.h |
//
// This file defines functions that convert arrays of YUV pixels
// to grayscale or RGB (or RGBA) values.  The YUV pixels may be encoded
// in an image format specified by an <c EVisImEncoding> value.
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
// @xref <l VisConvertYUV\.inl> <l VisConvertYUV\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISCONVERTYUV_H__
#define __VIS_IMSRC_VISCONVERTYUV_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <assert.h>


#ifndef VISAPI
#define VISAPI __stdcall
#endif // VISAPI


// @enum EVisConvYUV |
//
// These values are used to specify the equations to use when converting
// YUV values to RGB values.
//
// Most of the equations used to convert between YUV (and YCbCr) and
// (gamma-corrected) RGB come from the following source:
//
// <tab> Video Demystified, second edition, by Keith Jack, ISBN 1-878707-36-1.
//
#ifndef VIS_DEFINED_EVIS_CONV_YUV

	#define VIS_DEFINED_EVIS_CONV_YUV

	enum EVisConvYUV
	{
		// @emem
		// 
		// Basic YUV Conversion (0 \<= Y \<= 255, - 112 \<= U \<= 112, - 157 \<= V \<= 157)
		// <nl> <tab>  R = Y + 1.140 V
		// <nl> <tab>  G = Y - 0.394 U - 0.581 V
		// <nl> <tab>  B = Y + 2.032 U
		evisconvyuvBasic			= 0,

		evisconvyuvFirst			= 0,

		// @emem
		// 
		// Digital YUV Conversion (0 \<= Y \<= 255, - 128 \<= U \<= 127, - 128 \<= V \<= 127)
		// <nl> <tab>  R = Y + 1.402 V
		// <nl> <tab>  G = Y - 0.344 U - 0.714 V
		// <nl> <tab>  B = Y + 1.772 U
		evisconvyuvDigital,

		evisconvyuv422				= 1,  // Old name

		evisconvyuvDefault			= 1,  // Was 0 (evisconvyuvBasic) in the first SDK release

		// @emem
		// 
		// NTSC or PAL (N) YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 \<= Y \<= 130)
		// <nl> <tab>  R = 1.969 Y - 0.009 U + 2.244 V
		// <nl> <tab>  G = 1.969 Y - 0.775 U - 1.143 V
		// <nl> <tab>  B = 1.969 Y + 4.006 U - 0.009 V
		evisconvyuvNTSCNoSat,

		// @emem
		// 
		// NTSC or PAL (N) YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 \<= Y \<= 130)
		// <nl> <tab>  R = 1.691 Y - 0.008 U + 1.927 V + 16
		// <nl> <tab>  G = 1.691 Y - 0.666 U - 0.982 V + 16
		// <nl> <tab>  B = 1.691 Y + 3.440 U - 0.008 V + 16
		evisconvyuvNTSCSat,

		// @emem
		// 
		// PAL YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 \<= Y \<= 137)
		// <nl> <tab>  R = 1.862 Y - 0.005 U + 2.121 V
		// <nl> <tab>  G = 1.862 Y - 0.731 U - 1.083 V
		// <nl> <tab>  B = 1.862 Y + 3.788 U - 0.002 V
		evisconvyuvPALNoSat,

		// @emem
		// 
		// PAL YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 \<= Y \<= 137)
		// <nl> <tab>  R = 1.599 Y - 0.004 U + 1.822 V + 16
		// <nl> <tab>  G = 1.599 Y - 0.628 U - 0.930 V + 16
		// <nl> <tab>  B = 1.599 Y + 3.253 U - 0.002 V + 16
		evisconvyuvPALSat,

		// @emem
		// 
		// Grayscale YUV Conversion (using intensity information)
		// <nl> <tab>  R = Y
		// <nl> <tab>  G = Y
		// <nl> <tab>  B = Y
		evisconvyuvGrayscale,

		// @emem
		// 
		// Unscaled YUV (not in the source above)
		// It has been suggested that the following conversion may be useful with some
		// cameras.  (We can derive the equation from Y = 0.299 R + 0.587 G + 0.114 B,
		// <nl> <tab>  U = B - Y, and V = R - Y.)
		// <nl> <tab>  R = Y + V
		// <nl> <tab>  G = Y - 0.245 U - 0.509 V
		// <nl> <tab>  B = Y + U
		evisconvyuvUnscaled,

		// @emem
		// 
		// YCbCr Conversion (nominal range 16 to 235, no thresholding needed)
		// (Nominal input ranges:  16 \<= Y \<= 235, 16 \<= Cb \<= 235, 16 \<= Cr \<= 235)
		// <nl> <tab>  R = Y - 1.371 (Cr - 128)
		// <nl> <tab>  G = Y - 0.336 (Cb - 128) - 0.698 (Cr - 128)
		// <nl> <tab>  B = Y + 1.732 (Cb - 128)
		evisconvyuvCbCrNoSat,

		// @emem
		// 
		// YCbCr Conversion (nominal range 0 to 255, thresholding needed)
		// (Nominal input ranges:  16 \<= Y \<= 235, 16 \<= Cb \<= 235, 16 \<= Cr \<= 235)
		// <nl> <tab>  R = 1.164 (Y - 16) - 1.596 (Cr - 128)
		// <nl> <tab>  G = 1.164 (Y - 16) - 0.392 (Cb - 128) - 0.813 (Cr - 128)
		// <nl> <tab>  B = 1.164 (Y - 16) + 2.017 (Cb - 128)
		evisconvyuvCbCrSat,

		evisconvyuvLim,
	};

#endif // VIS_DEFINED_EVIS_CONV_YUV


#ifndef VIS_DEFINED_SVIS_YUV422_16BPP

	#define VIS_DEFINED_SVIS_YUV422_16BPP

	struct SVisYUYV { BYTE bY1, bU, bY2, bV; };
	struct SVisYVYU { BYTE bY1, bV, bY2, bU; };
	struct SVisUYVY { BYTE bU, bY1, bV, bY2; };

#endif // VIS_DEFINED_SVIS_YUV422_16BPP



///////////////////////////////////////////////////////////////////////////
// Functions defined in VisImSrcb.lib.


VisImSrcExport unsigned long VISAPI VisGammaCorrect(unsigned long ul);


template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToYUV(const TPixYUV *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect = false,
		bool fSubsample = false);

VisImSrcExport void VISAPI VisYUVPlanarToYUV(const void *pvSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);


template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToRGBA32(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false,
		int evisconvyuv = evisconvyuvDefault);

VisImSrcExport void VISAPI VisYUV444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault);

VisImSrcExport void VISAPI VisYUVA444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault);

VisImSrcExport void VISAPI VisYUV444UShortToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault);

VisImSrcExport void VISAPI VisYUVA444UShortToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault);

VisImSrcExport void VISAPI VisYUVPlanarToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault,
		int cbppSrc = 16, bool fVBeforeU = false);



template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToGrayByte(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false);

VisImSrcExport void VISAPI VisYUV444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVA444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUV444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVA444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVPlanarToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);



template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToGrayULong(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false);

VisImSrcExport void VISAPI VisYUV444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVA444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUV444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVA444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

VisImSrcExport void VISAPI VisYUVPlanarToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);


// Supported conversions:  {1, 2, 4, 8, 16}-bit gray to {32, 48, 64}-bit YUV.
VisImSrcExport void VISAPI VisGrayToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);



#endif // __VIS_IMSRC_VISCONVERTYUV_H__
