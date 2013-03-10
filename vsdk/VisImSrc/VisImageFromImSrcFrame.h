////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGEFROMIMSRCFRAME
//
// @module VisImageFromImSrcFrame.h |
//
// This file defines a class that is used internally to get <c CVisImageBase>
// objects from an <c SVisImSrcFrame> structure.  Most users of the
// library will not need to use this class.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImageFromImSrcFrame\.inl> <l VisImageFromImSrcFrame\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMAGEFROMIMSRCFRAME_H__
#define __VIS_IMSRC_VISIMAGEFROMIMSRCFRAME_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CVisImageFromImSrcFrame
{
public:
	VisImSrcExport CVisImageFromImSrcFrame(EVisPixFmt evispixfmt);

	VisImSrcExport CVisImageBase *PImageGetImSrcFrame(SVisImSrcFrame& refimsrcframe,
			bool fCopyVolatile = true, bool fCopyDMABuffers = false,
			bool fSubsampleYUV422 = false);

	inline int CImEncodingSupported(void) const;

	inline EVisImEncoding ImEncodingSupportedI(int i) const;

	inline bool FIsSupportedImEncoding(EVisImEncoding evisimencoding) const;

protected:
	static CVisByteImage *PImageOfByteFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers);
	static CVisULongImage *PImageOfULongFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers);
	static CVisRGBAByteImage *PImageOfRGBAByteFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422);
	static CVisYUVAByteImage *PImageOfYUVAByteFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422);
	static CVisYUVAUShortImage *PImageOfYUVAUShortFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422);
	static CVisImage<SVisYUYV> *PImageOfYUYVFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers);
	static CVisImage<SVisYVYU> *PImageOfYVYUFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers);
	static CVisImage<SVisUYVY> *PImageOfUYVYFromImSrcFrame(
			SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers);

	static const EVisImEncoding s_rgimencodingSupportedGrayByte[];
	static const EVisImEncoding s_rgimencodingSupportedGrayULong[];
	static const EVisImEncoding s_rgimencodingSupportedRGBAByte[];
	static const EVisImEncoding s_rgimencodingSupportedYUVAByte[];
	static const EVisImEncoding s_rgimencodingSupportedYUVAUShort[];

private:
	const EVisPixFmt m_evispixfmt;
	EVisImEncoding m_imencodingPreferred;
	const EVisImEncoding *m_rgimencodingSupported;
	int m_cimencodingSupported;
};



#include "VisImageFromImSrcFrame.inl"


#endif // __VIS_IMSRC_VISIMAGEFROMIMSRCFRAME_H__
