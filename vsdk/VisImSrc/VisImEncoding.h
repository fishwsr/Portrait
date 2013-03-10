////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMENCODING
//
// @module VisImEncoding.h |
//
// This file defines constants used to specify standard image encodings.
// (Cameras may support image encodings that are not arrays of pixels.)
// Helper functions are also provided to convert between <c EVisImEncoding>
// values and the Four-CC <t DWORD> values used to describe Windows images.
// (See http://www.flash.net/~dawilson/fourcc/index.htm for more info.)
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImEncoding\.inl>
// <nl>
//
// @head3 Constants |
// @index const enum | VISIMENCODING
//
// @head3 Functions |
// @index func | VISIMENCODING
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMENCODING_H__
#define __VIS_IMSRC_VISIMENCODING_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000




#ifndef VIS_DEFINED_EVIS_IM_ENCODING

	#define VIS_DEFINED_EVIS_IM_ENCODING

	// @enum EVisImEncoding
	// These constants used to specify standard image encodings.
	//
	// (Cameras may support image encodings that are not arrays of pixels.
	// See http://www.flash.net/~dawilson/fourcc/index.htm for more info.
	// Note that IYUV and I420 are the same format.  We only list IYUV.)
	enum EVisImEncoding
	{
		// @emem
		// 
		evisimencodingUnknown		= 0,

		evisimencodingFirst			= 0,

		// @emem
		// 
		evisimencodingBW1bit,

		// @emem
		// 
		evisimencodingGray4bit,

		// @emem
		// 
		evisimencodingColor4bit,

		// @emem
		// 
		evisimencodingGray8bit,

		// @emem
		// 
		evisimencodingColor8bit,

		// @emem
		// 
		evisimencodingGray16bit,

		// @emem
		// 
		evisimencodingRGB565,

		// @emem
		// 
		evisimencodingRGB555,

		// @emem
		// 
		evisimencodingRGB24,

		// @emem
		// 
		evisimencodingRGBA32,

		// @emem
		// 
		evisimencodingRGB48,

		// @emem
		// 
		evisimencodingRGBA64,

		// @emem
		// 
		evisimencodingYUV24,

		// @emem
		// 
		evisimencodingYUVA32,

		// @emem
		// 
		evisimencodingYUV48,

		// @emem
		// 
		evisimencodingYUVA64,

		// @emem
		// 
		evisimencodingUYVY,

		// @emem
		// 
		evisimencodingYUY2,

		// @emem
		// 
		evisimencodingYVYU,

		// @emem
		// 
		evisimencodingY41P,

		// @emem
		// 
		evisimencodingY211,

		// @emem
		// 
		evisimencodingY41T,

		// @emem
		// 
		evisimencodingY42T,

		// @emem
		// 
		evisimencodingCLJR,

		// @emem
		// 
		evisimencodingYVU9,

		// @emem
		// 
		evisimencodingIF09,

		// @emem
		// 
		evisimencodingYV12,

		// @emem
		// (Note that IYUV and I420 are the same format.  We only list IYUV.)
		evisimencodingIYUV,

		// @emem
		// 
		evisimencodingCLPL,

		// @emem
		// 
		evisimencodingY210,

		// @emem
		// 
		evisimencodingY410,

		// @emem
		// 
		evisimencodingYUVP,

		evisimencodingLim
	};

#endif // VIS_DEFINED_EVIS_IM_ENCODING



// @func int | VisCbitGetImEncoding |
//
// Find the number of bits per pixel to specify in a <t BITMAPINFOHEADER>
// for a given <c EVisImEncoding> value.  This could crash if
// <p evisimencoding> is not valid.
//
// @parm <c EVisImEncoding> | evisimencoding |
// A valid <c EVisImEncoding> value.
inline int VisCbitGetImEncoding(EVisImEncoding evisimencoding);


// @func DWORD | VisDwFourCCGetImEncoding |
//
// Given a valid <c EVisImEncoding> value, find a Four-CC value that
// describes specified image encoding.  This could crash if
// <p evisimencoding> is not valid.
//
// (See http://www.flash.net/~dawilson/fourcc/index.htm for more
// information about Four-CC values.)
//
// @parm <c EVisImEncoding> | evisimencoding |
// A valid <c EVisImEncoding> value.
inline DWORD VisDwFourCCGetImEncoding(EVisImEncoding evisimencoding);


// @func const char * | VisSzGetImEncoding |
//
// Given a valid <c EVisImEncoding> value, find a string that describes
// specified image encoding.  This could crash if <p evisimencoding>
// is not valid.
//
// @parm <c EVisImEncoding> | evisimencoding |
// A valid <c EVisImEncoding> value.
inline const char *VisSzGetImEncoding(EVisImEncoding evisimencoding);


// @func <c EVisImEncoding> | VisImEncodingGetDwFourCC |
//
// Given a Four-CC value describing an image encoding, find a <c EVisImEncoding>
// value.  This will return <e EVisImEncoding::evisimencodingUnknown> if
// the Four-CC value is not recognized.
//
// @parm DWORD | dwFourCC |
// A Four-CC value (usually taken from a <t BITMAPINFOHEADER>).
//
// (See http://www.flash.net/~dawilson/fourcc/index.htm for more
// information about Four-CC values.)
//
// @parm int | cbit |
// The number of bits per pixel specified for this Four-CC value
// in a <t BITMAPINFOHEADER>.  This is needed with BI_RGB because
// it can be used in a few different image encodings.  (This number
// is also used in an assert in _DEBUG code.)
inline EVisImEncoding VisImEncodingGetDwFourCC(DWORD dwFourCC, int cbit);


// @func <c EVisImEncoding> | VisImEncodingGetSz |
//
// Given a string describing an image encoding, find a <c EVisImEncoding>
// value.  This will return <e EVisImEncoding::evisimencodingUnknown> if
// the string is not recognized.
//
// @parm const char * | sz |
// The string.
inline EVisImEncoding VisImEncodingGetSz(const char *sz);


#endif // __VIS_IMSRC_VISIMENCODING_H__
