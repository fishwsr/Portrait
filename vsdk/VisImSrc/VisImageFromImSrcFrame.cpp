// VisImageFromImSrcFrame.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"

#include "VisConvertRGB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// LATER:  Some of the conversion functions should be optimized.  We might
// want to add some restrictions (eg. 4-byte or 8-byte alignment) to improve
// the conversion functions.

// LATER:  Allow more destination pixel types.


const EVisImEncoding CVisImageFromImSrcFrame::s_rgimencodingSupportedGrayByte[] =
{
	evisimencodingGray8bit,
	evisimencodingGray16bit,

	evisimencodingYVU9,
	evisimencodingIF09,
	evisimencodingYV12,
	evisimencodingIYUV,

	evisimencodingUYVY,
	evisimencodingYUY2,
	evisimencodingYVYU,
	evisimencodingY42T,
	evisimencodingY211,

	evisimencodingYUV24,
	evisimencodingYUVA32,

	evisimencodingYUV48,
	evisimencodingYUVA64,

	evisimencodingRGB24,
	evisimencodingRGBA32,

	evisimencodingRGB48,
	evisimencodingRGBA64,

	evisimencodingRGB565,
	evisimencodingRGB555,

	evisimencodingGray4bit,
	evisimencodingBW1bit

#ifdef LATER  // LATER:  Add support for the following formats.
	evisimencodingColor4bit,
	evisimencodingColor8bit,
	evisimencodingY41P,
	evisimencodingY41T,
	evisimencodingCLJR,
	evisimencodingCLPL,
	evisimencodingY210,
	evisimencodingY410,
	evisimencodingYUVP,
#endif // LATER
};


const EVisImEncoding CVisImageFromImSrcFrame::s_rgimencodingSupportedGrayULong[] =
{
	evisimencodingGray16bit,

	evisimencodingRGB48,
	evisimencodingRGBA64,

	evisimencodingYUV48,
	evisimencodingYUVA64,

	evisimencodingRGB24,
	evisimencodingRGBA32,

	evisimencodingYUV24,
	evisimencodingYUVA32,

	evisimencodingGray8bit,

	evisimencodingYVU9,
	evisimencodingIF09,
	evisimencodingYV12,
	evisimencodingIYUV,

	evisimencodingUYVY,
	evisimencodingYUY2,
	evisimencodingYVYU,
	evisimencodingY42T,
	evisimencodingY211,

	evisimencodingRGB565,
	evisimencodingRGB555,

	evisimencodingGray4bit,
	evisimencodingBW1bit

#ifdef LATER  // LATER:  Add support for the following formats.
	evisimencodingColor4bit,
	evisimencodingColor8bit,
	evisimencodingY41P,
	evisimencodingY41T,
	evisimencodingCLJR,
	evisimencodingCLPL,
	evisimencodingY210,
	evisimencodingY410,
	evisimencodingYUVP,
#endif // LATER
};


const EVisImEncoding CVisImageFromImSrcFrame::s_rgimencodingSupportedRGBAByte[] =
{
	evisimencodingRGBA32,
	evisimencodingRGB24,
	evisimencodingRGBA64,
	evisimencodingRGB48,
	evisimencodingRGB565,
	evisimencodingRGB555,

	evisimencodingYUVA32,
	evisimencodingYUV24,
	evisimencodingYUVA64,
	evisimencodingYUV48,

	evisimencodingUYVY,
	evisimencodingYUY2,
	evisimencodingYVYU,
	evisimencodingY42T,
	evisimencodingY211,

	evisimencodingYV12,
	evisimencodingIYUV,
	evisimencodingYVU9,
	evisimencodingIF09,

	evisimencodingGray16bit,
	evisimencodingGray8bit,
	evisimencodingGray4bit,
	evisimencodingBW1bit

#ifdef LATER  // LATER:  Add support for the following formats.
	evisimencodingColor4bit,
	evisimencodingColor8bit,
	evisimencodingY41P,
	evisimencodingY41T,
	evisimencodingCLJR,
	evisimencodingCLPL,
	evisimencodingY210,
	evisimencodingY410,
	evisimencodingYUVP,
#endif // LATER
};


const EVisImEncoding CVisImageFromImSrcFrame::s_rgimencodingSupportedYUVAByte[] =
{
	evisimencodingYUVA32,
	evisimencodingYUV24,
	evisimencodingYUVA64,
	evisimencodingYUV48,

	evisimencodingUYVY,
	evisimencodingYUY2,
	evisimencodingYVYU,
	evisimencodingY42T,
	evisimencodingY211,

	evisimencodingYV12,
	evisimencodingIYUV,
	evisimencodingYVU9,
	evisimencodingIF09,

	evisimencodingRGBA32,
	evisimencodingRGB24,
	evisimencodingRGBA64,
	evisimencodingRGB48,
	evisimencodingRGB565,
	evisimencodingRGB555,

	evisimencodingGray16bit,
	evisimencodingGray8bit,
	evisimencodingGray4bit,
	evisimencodingBW1bit

#ifdef LATER  // LATER:  Add support for the following formats.
	evisimencodingColor4bit,
	evisimencodingColor8bit,
	evisimencodingY41P,
	evisimencodingY41T,
	evisimencodingCLJR,
	evisimencodingCLPL,
	evisimencodingY210,
	evisimencodingY410,
	evisimencodingYUVP,
#endif // LATER
};


const EVisImEncoding CVisImageFromImSrcFrame::s_rgimencodingSupportedYUVAUShort[] =
{
	evisimencodingYUVA64,
	evisimencodingYUV48,
	evisimencodingYUVA32,
	evisimencodingYUV24,

	evisimencodingUYVY,
	evisimencodingYUY2,
	evisimencodingYVYU,
	evisimencodingY42T,
	evisimencodingY211,

	evisimencodingYV12,
	evisimencodingIYUV,
	evisimencodingYVU9,
	evisimencodingIF09,

	evisimencodingRGBA32,
	evisimencodingRGB24,
	evisimencodingRGBA64,
	evisimencodingRGB48,
	evisimencodingRGB565,
	evisimencodingRGB555,

	evisimencodingGray16bit,
	evisimencodingGray8bit,
	evisimencodingGray4bit,
	evisimencodingBW1bit

#ifdef LATER  // LATER:  Add support for the following formats.
	evisimencodingColor4bit,
	evisimencodingColor8bit,
	evisimencodingY41P,
	evisimencodingY41T,
	evisimencodingCLJR,
	evisimencodingCLPL,
	evisimencodingY210,
	evisimencodingY410,
	evisimencodingYUVP,
#endif // LATER
};


VisImSrcExport CVisImageFromImSrcFrame::CVisImageFromImSrcFrame(EVisPixFmt evispixfmt)
  : m_evispixfmt(evispixfmt),
	m_imencodingPreferred(evisimencodingUnknown),
	m_rgimencodingSupported(0),
	m_cimencodingSupported(0)
{
	switch(m_evispixfmt)
	{
	case evispixfmtGrayByte:
		m_rgimencodingSupported = s_rgimencodingSupportedGrayByte;
		m_cimencodingSupported = sizeof(s_rgimencodingSupportedGrayByte)
				/ sizeof(EVisImEncoding);
		m_imencodingPreferred = m_rgimencodingSupported[0];
		break;

	case evispixfmtGrayUInt:
	case evispixfmtGrayULong:
		m_rgimencodingSupported = s_rgimencodingSupportedGrayULong;
		m_cimencodingSupported = sizeof(s_rgimencodingSupportedGrayULong)
				/ sizeof(EVisImEncoding);
		m_imencodingPreferred = m_rgimencodingSupported[0];
		break;

	case evispixfmtRGBAByte:
		m_rgimencodingSupported = s_rgimencodingSupportedRGBAByte;
		m_cimencodingSupported = sizeof(s_rgimencodingSupportedRGBAByte)
				/ sizeof(EVisImEncoding);
		m_imencodingPreferred = m_rgimencodingSupported[0];
		break;

	case evispixfmtYUVAByte:
		m_rgimencodingSupported = s_rgimencodingSupportedYUVAByte;
		m_cimencodingSupported = sizeof(s_rgimencodingSupportedYUVAByte)
				/ sizeof(EVisImEncoding);
		m_imencodingPreferred = m_rgimencodingSupported[0];
		break;

	case evispixfmtYUVAUShort:
		m_rgimencodingSupported = s_rgimencodingSupportedYUVAUShort;
		m_cimencodingSupported = sizeof(s_rgimencodingSupportedYUVAUShort)
				/ sizeof(EVisImEncoding);
		m_imencodingPreferred = m_rgimencodingSupported[0];
		break;

	case evispixfmtYUYV:
		m_cimencodingSupported = 1;
		m_imencodingPreferred = evisimencodingYUY2;
		break;

	case evispixfmtYVYU:
		m_cimencodingSupported = 1;
		m_imencodingPreferred = evisimencodingYVYU;
		break;

	case evispixfmtUYVY:
		m_cimencodingSupported = 1;
		m_imencodingPreferred = evisimencodingUYVY;
		break;
	}
}


VisImSrcExport CVisImageBase *CVisImageFromImSrcFrame::PImageGetImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422)
{
	CVisImageBase *pimage = 0;

	switch(m_evispixfmt)
	{
	case evispixfmtGrayByte:
		pimage = PImageOfByteFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers);
		break;

	case evispixfmtGrayUInt:
	case evispixfmtGrayULong:
		assert(sizeof(int) == sizeof(long));
		pimage = PImageOfULongFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers);
		break;

	case evispixfmtRGBAByte:
		pimage = PImageOfRGBAByteFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers, fSubsampleYUV422);
		break;

	case evispixfmtYUVAByte:
		pimage = PImageOfYUVAByteFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers, fSubsampleYUV422);
		break;

	case evispixfmtYUVAUShort:
		pimage = PImageOfYUVAUShortFromImSrcFrame(refimsrcframe,
				fCopyVolatile, fCopyDMABuffers, fSubsampleYUV422);
		break;

	case evispixfmtYUYV:
		pimage = PImageOfYUYVFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers);
		break;

	case evispixfmtYVYU:
		pimage = PImageOfYVYUFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers);
		break;

	case evispixfmtUYVY:
		pimage = PImageOfUYVYFromImSrcFrame(refimsrcframe, fCopyVolatile,
				fCopyDMABuffers);
		break;
	}

	return pimage;
}



CVisByteImage *CVisImageFromImSrcFrame::PImageOfByteFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers)
{
	CVisByteImage *pimage = new CVisByteImage;

	// If there's no image, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rect = refimsrcframe.m_rect;

		// Do we have an image of the type that we're looking for?
		// (Note that planar YUV images can be used as grayscale images.)
		if ((!fReflect)
				&& ((refimsrcframe.m_evisimencoding == evisimencodingGray8bit)
					|| (refimsrcframe.m_evisimencoding == evisimencodingYVU9)
					|| (refimsrcframe.m_evisimencoding == evisimencodingIF09)
					|| (refimsrcframe.m_evisimencoding == evisimencodingYV12)
					|| (refimsrcframe.m_evisimencoding == evisimencodingIYUV)))
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(refimsrcframe.m_rect);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			pimage->Allocate(rect);

			// We need to (try to) convert the image to the type that we're looking for.
			switch (refimsrcframe.m_evisimencoding)
			{
			case evisimencodingBW1bit:
				VisConvertCbitToCbit(refimsrcframe.m_pvData, 1,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingGray4bit:
				VisConvertCbitToCbit(refimsrcframe.m_pvData, 4,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingGray8bit:
				VisConvertCbitToCbit(refimsrcframe.m_pvData, 8,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingGray16bit:
				VisConvertCbitToCbit(refimsrcframe.m_pvData, 16,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB555:
				VisRGBToGray(refimsrcframe.m_pvData, 15,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB565:
				VisRGBToGray(refimsrcframe.m_pvData, 16,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB24:
				VisRGBToGray(refimsrcframe.m_pvData, 24,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGBA32:
				VisRGBToGray(refimsrcframe.m_pvData, 32,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB48:
				VisRGBToGray(refimsrcframe.m_pvData, 48,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGBA64:
				VisRGBToGray(refimsrcframe.m_pvData, 64,
						pimage->PbPixel(0, 0), 8,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUV24:
				VisYUV444ByteToGrayByte((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUVA32:
				VisYUVA444ByteToGrayByte((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUV48:
				VisYUV444UShortToGrayByte((unsigned short *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUVA64:
				VisYUVA444UShortToGrayByte((unsigned short *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingUYVY:
			case evisimencodingY42T:  // Not completely right, but this should work okay
				VisYUV422ToGrayByte((SVisUYVY *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUY2:
			case evisimencodingY211:  // Not completely right, but this should work okay
				VisYUV422ToGrayByte((SVisYUYV *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYVYU:
				VisYUV422ToGrayByte((SVisYVYU *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYVU9:
			case evisimencodingIF09:
				VisYUVPlanarToGrayByte((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 9, true);
				break;

			case evisimencodingYV12:
				VisYUVPlanarToGrayByte((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, true);
				break;

			case evisimencodingIYUV:
				VisYUVPlanarToGrayByte((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, false);
				break;

#ifdef _DEBUG
			case evisimencodingColor8bit:
			case evisimencodingColor4bit:
			case evisimencodingY41P:
			case evisimencodingY41T:
			case evisimencodingCLJR:
			case evisimencodingCLPL:
			case evisimencodingY210:  // Don't really know what this format is!
			case evisimencodingY410:  // Don't really know what this format is!
			case evisimencodingYUVP:  // Don't really know what this format is!
			default:
				// LATER:  Add support for all evisimencoding values.
{
	static bool fAsserted = false;
	assert(fAsserted);
	if (!fAsserted)
	{
		char szBuf[255];
		int cpix = (refimsrcframe.m_rect.right - refimsrcframe.m_rect.left)
				* (refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top);
		int cbit = ((cpix == 0) ? 0 : 8 * refimsrcframe.m_cbData / cpix);
		sprintf(szBuf, "Unrecognized %d-bit format %#010x (== '%c%c%c%c').\n",
				cbit,
				refimsrcframe.m_evisimencoding,
				((char *) (&refimsrcframe.m_evisimencoding))[0],
				((char *) (&refimsrcframe.m_evisimencoding))[1],
				((char *) (&refimsrcframe.m_evisimencoding))[2],
				((char *) (&refimsrcframe.m_evisimencoding))[3]);
		OutputDebugString(szBuf);
	}
	fAsserted = true;
}
				break;
#endif // _DEBUG
			}
		}
	}

	// Set the time stamp in the image before returning.
	if ((refimsrcframe.m_filetime.dwLowDateTime == 0)
			&& (refimsrcframe.m_filetime.dwHighDateTime == 0))
	{
		// Set FILETIME in the image to the current time.
		pimage->SetFiletime();
	}
	else
	{
		pimage->SetFiletime(refimsrcframe.m_filetime);
	}

	return pimage;
}


CVisULongImage *CVisImageFromImSrcFrame::PImageOfULongFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers)
{
	VisUnreferenced(fCopyVolatile);
	VisUnreferenced(fCopyDMABuffers);

	CVisULongImage *pimage = new CVisULongImage;

	// If there's no image, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rect = refimsrcframe.m_rect;

		pimage->Allocate(rect);

		// Since there's no evisimencoding value for 32-bit gray images, we
		// need to (try to) convert the image to the type that we're looking for.
		switch (refimsrcframe.m_evisimencoding)
		{
		case evisimencodingBW1bit:
			VisConvertCbitToCbit(refimsrcframe.m_pvData, 1,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingGray4bit:
			VisConvertCbitToCbit(refimsrcframe.m_pvData, 4,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingGray8bit:
			VisConvertCbitToCbit(refimsrcframe.m_pvData, 8,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingGray16bit:
			VisConvertCbitToCbit(refimsrcframe.m_pvData, 16,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGB555:
			VisRGBToGray(refimsrcframe.m_pvData, 15,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGB565:
			VisRGBToGray(refimsrcframe.m_pvData, 16,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGB24:
			VisRGBToGray(refimsrcframe.m_pvData, 24,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGBA32:
			VisRGBToGray(refimsrcframe.m_pvData, 32,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGB48:
			VisRGBToGray(refimsrcframe.m_pvData, 48,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingRGBA64:
			VisRGBToGray(refimsrcframe.m_pvData, 64,
					pimage->PbPixel(0, 0), 32,
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYUV24:
			VisYUV444ByteToGrayULong((BYTE *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYUVA32:
			VisYUVA444ByteToGrayULong((BYTE *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYUV48:
			VisYUV444UShortToGrayULong((unsigned short *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYUVA64:
			VisYUVA444UShortToGrayULong((unsigned short *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingUYVY:
		case evisimencodingY42T:  // Not completely right, but this should work okay
			VisYUV422ToGrayULong((SVisUYVY *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYUY2:
		case evisimencodingY211:  // Not completely right, but this should work okay
			VisYUV422ToGrayULong((SVisYUYV *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYVYU:
			VisYUV422ToGrayULong((SVisYVYU *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect);
			break;

		case evisimencodingYVU9:
		case evisimencodingIF09:
			VisYUVPlanarToGrayULong((BYTE *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect, 9, true);
			break;

		case evisimencodingYV12:
			VisYUVPlanarToGrayULong((BYTE *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect, 12, true);
			break;

		case evisimencodingIYUV:
			VisYUVPlanarToGrayULong((BYTE *) refimsrcframe.m_pvData,
					pimage->PbPixel(0, 0),
					refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
					refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
					fReflect, 12, false);
			break;

#ifdef _DEBUG
		case evisimencodingColor8bit:
		case evisimencodingColor4bit:
		case evisimencodingY41P:
		case evisimencodingY41T:
		case evisimencodingCLJR:
		case evisimencodingCLPL:
		case evisimencodingY210:  // Don't really know what this format is!
		case evisimencodingY410:  // Don't really know what this format is!
		case evisimencodingYUVP:  // Don't really know what this format is!
		default:
			// LATER:  Add support for all evisimencoding values.
{
	static bool fAsserted = false;
	assert(fAsserted);
	if (!fAsserted)
	{
		char szBuf[255];
		int cpix = (refimsrcframe.m_rect.right - refimsrcframe.m_rect.left)
				* (refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top);
		int cbit = ((cpix == 0) ? 0 : 8 * refimsrcframe.m_cbData / cpix);
		sprintf(szBuf, "Unrecognized %d-bit format %#010x (== '%c%c%c%c').\n",
				cbit,
				refimsrcframe.m_evisimencoding,
				((char *) (&refimsrcframe.m_evisimencoding))[0],
				((char *) (&refimsrcframe.m_evisimencoding))[1],
				((char *) (&refimsrcframe.m_evisimencoding))[2],
				((char *) (&refimsrcframe.m_evisimencoding))[3]);
		OutputDebugString(szBuf);
	}
	fAsserted = true;
}
			break;
#endif // _DEBUG
		}
	}

	// Set the time stamp in the image before returning.
	if ((refimsrcframe.m_filetime.dwLowDateTime == 0)
			&& (refimsrcframe.m_filetime.dwHighDateTime == 0))
	{
		// Set FILETIME in the image to the current time.
		pimage->SetFiletime();
	}
	else
	{
		pimage->SetFiletime(refimsrcframe.m_filetime);
	}

	return pimage;
}


CVisRGBAByteImage *CVisImageFromImSrcFrame::PImageOfRGBAByteFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422)
{
	CVisRGBAByteImage *pimage = new CVisRGBAByteImage;

	// LATER:  Do we want an option to subsample images even if they are not
	// YUV422 images?

	// If there's no image, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rect = refimsrcframe.m_rect;

		// Do we have an image of the type that we're looking for?
		if ((!fReflect)
				&& (refimsrcframe.m_evisimencoding == evisimencodingRGBA32))
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(refimsrcframe.m_rect);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			// We need to (try to) convert the image to the type that we're looking for.

			// Find storage for the image pixels.
			if ((fSubsampleYUV422)
					&& ((refimsrcframe.m_evisimencoding == evisimencodingUYVY)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY42T)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYUY2)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY211)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYVYU)))
			{
				rect.left /= 2;
				rect.top /= 2;
				rect.right /= 2;
				rect.bottom /= 2;
			}
			pimage->Allocate(rect);

			// LATER:  Use the colormap information?

			switch (refimsrcframe.m_evisimencoding)
			{
			case evisimencodingBW1bit:
			case evisimencodingGray4bit:
			case evisimencodingGray8bit:
			case evisimencodingGray16bit:
				VisGrayToRGB(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), 32,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB555:
				VisConvert555To32bit(refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB565:
				VisConvert565To32bit(refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB24:
			case evisimencodingRGBA32:
			case evisimencodingRGB48:
			case evisimencodingRGBA64:
				VisConvertCbitToCbit(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), 32,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUV24:
				VisYUV444ByteToRGBA32((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYUVA32:
				VisYUVA444ByteToRGBA32((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYUV48:
				VisYUV444UShortToRGBA32((unsigned short *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYUVA64:
				VisYUVA444UShortToRGBA32((unsigned short *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingUYVY:
			case evisimencodingY42T:  // Not completely right, but this should work okay
				VisYUV422ToRGBA32((SVisUYVY *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYUY2:
			case evisimencodingY211:  // Not completely right, but this should work okay
				VisYUV422ToRGBA32((SVisYUYV *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYVYU:
				VisYUV422ToRGBA32((SVisYVYU *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422, refimsrcframe.m_evisconvyuv);
				break;

			case evisimencodingYVU9:
			case evisimencodingIF09:
				VisYUVPlanarToRGBA32((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv, 9, true);
				break;

			case evisimencodingYV12:
				VisYUVPlanarToRGBA32((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv, 12, true);
				break;

			case evisimencodingIYUV:
				VisYUVPlanarToRGBA32((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0),
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, refimsrcframe.m_evisconvyuv, 12, false);
				break;

#ifdef _DEBUG
			case evisimencodingColor8bit:
			case evisimencodingColor4bit:
			case evisimencodingY41P:
			case evisimencodingY41T:
			case evisimencodingCLJR:
			case evisimencodingCLPL:  // Need to split VisYUVPlanarToRGBA32 into two fns for this format.
			case evisimencodingY210:  // Don't really know what this format is!
			case evisimencodingY410:  // Don't really know what this format is!
			case evisimencodingYUVP:  // Don't really know what this format is!
			default:
				// LATER:  Add support for all evisimencoding values.
{
	static bool fAsserted = false;
	assert(fAsserted);
	if (!fAsserted)
	{
		char szBuf[255];
		int cpix = (refimsrcframe.m_rect.right - refimsrcframe.m_rect.left)
				* (refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top);
		int cbit = ((cpix == 0) ? 0 : 8 * refimsrcframe.m_cbData / cpix);
		sprintf(szBuf, "Unrecognized %d-bit format %#010x (== '%c%c%c%c').\n",
				cbit,
				refimsrcframe.m_evisimencoding,
				((char *) (&refimsrcframe.m_evisimencoding))[0],
				((char *) (&refimsrcframe.m_evisimencoding))[1],
				((char *) (&refimsrcframe.m_evisimencoding))[2],
				((char *) (&refimsrcframe.m_evisimencoding))[3]);
		OutputDebugString(szBuf);
	}
	fAsserted = true;
}
				break;
#endif // _DEBUG
			}
		}
	}

	// Set the time stamp in the image before returning.
	if ((refimsrcframe.m_filetime.dwLowDateTime == 0)
			&& (refimsrcframe.m_filetime.dwHighDateTime == 0))
	{
		// Set FILETIME in the image to the current time.
		pimage->SetFiletime();
	}
	else
	{
		pimage->SetFiletime(refimsrcframe.m_filetime);
	}

	return pimage;
}


CVisYUVAByteImage *CVisImageFromImSrcFrame::PImageOfYUVAByteFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422)
{
	CVisYUVAByteImage *pimage = new CVisYUVAByteImage;
	const int cbitDest = 32;

	// LATER:  Do we want an option to subsample images even if they are not
	// YUV422 images?

	// If there's no image, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rect = refimsrcframe.m_rect;

		// Do we have an image of the type that we're looking for?
		if ((!fReflect)
				&& (refimsrcframe.m_evisimencoding == evisimencodingYUVA32))
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(refimsrcframe.m_rect);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			// We need to (try to) convert the image to the type that we're looking for.

			// Find storage for the image pixels.
			if ((fSubsampleYUV422)
					&& ((refimsrcframe.m_evisimencoding == evisimencodingUYVY)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY42T)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYUY2)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY211)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYVYU)))
			{
				rect.left /= 2;
				rect.top /= 2;
				rect.right /= 2;
				rect.bottom /= 2;
			}
			pimage->Allocate(rect);

			// LATER:  Use the colormap information?

			switch (refimsrcframe.m_evisimencoding)
			{
			case evisimencodingBW1bit:
			case evisimencodingGray4bit:
			case evisimencodingGray8bit:
			case evisimencodingGray16bit:
				VisGrayToYUV(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB555:
				VisRGBToYUV(refimsrcframe.m_pvData, 15,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB565:
			case evisimencodingRGB24:
			case evisimencodingRGBA32:
			case evisimencodingRGB48:
			case evisimencodingRGBA64:
				VisRGBToYUV(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUV24:
			case evisimencodingYUVA32:
			case evisimencodingYUV48:
			case evisimencodingYUVA64:
				VisConvertCbitToCbit(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingUYVY:
			case evisimencodingY42T:  // Not completely right, but this should work okay
				VisYUV422ToYUV((SVisUYVY *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYUY2:
			case evisimencodingY211:  // Not completely right, but this should work okay
				VisYUV422ToYUV((SVisYUYV *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYVYU:
				VisYUV422ToYUV((SVisYVYU *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYVU9:
			case evisimencodingIF09:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 9, true);
				break;

			case evisimencodingYV12:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, true);
				break;

			case evisimencodingIYUV:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, false);
				break;

#ifdef _DEBUG
			case evisimencodingColor8bit:
			case evisimencodingColor4bit:
			case evisimencodingY41P:
			case evisimencodingY41T:
			case evisimencodingCLJR:
			case evisimencodingCLPL:  // Need to split VisYUVPlanarToRGBA32 into two fns for this format.
			case evisimencodingY210:  // Don't really know what this format is!
			case evisimencodingY410:  // Don't really know what this format is!
			case evisimencodingYUVP:  // Don't really know what this format is!
			default:
				// LATER:  Add support for all evisimencoding values.
{
	static bool fAsserted = false;
	assert(fAsserted);
	if (!fAsserted)
	{
		char szBuf[255];
		int cpix = (refimsrcframe.m_rect.right - refimsrcframe.m_rect.left)
				* (refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top);
		int cbit = ((cpix == 0) ? 0 : 8 * refimsrcframe.m_cbData / cpix);
		sprintf(szBuf, "Unrecognized %d-bit format %#010x (== '%c%c%c%c').\n",
				cbit,
				refimsrcframe.m_evisimencoding,
				((char *) (&refimsrcframe.m_evisimencoding))[0],
				((char *) (&refimsrcframe.m_evisimencoding))[1],
				((char *) (&refimsrcframe.m_evisimencoding))[2],
				((char *) (&refimsrcframe.m_evisimencoding))[3]);
		OutputDebugString(szBuf);
	}
	fAsserted = true;
}
				break;
#endif // _DEBUG
			}
		}
	}

	// Set the time stamp in the image before returning.
	if ((refimsrcframe.m_filetime.dwLowDateTime == 0)
			&& (refimsrcframe.m_filetime.dwHighDateTime == 0))
	{
		// Set FILETIME in the image to the current time.
		pimage->SetFiletime();
	}
	else
	{
		pimage->SetFiletime(refimsrcframe.m_filetime);
	}

	return pimage;
}




CVisYUVAUShortImage *CVisImageFromImSrcFrame::PImageOfYUVAUShortFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers, bool fSubsampleYUV422)
{
	// LATER:  Use template<TYUVImage, evisimencodingYUVImage> to combine this
	// function with the previous function?

	CVisYUVAUShortImage *pimage = new CVisYUVAUShortImage;
	const int cbitDest = 64;

	// LATER:  Do we want an option to subsample images even if they are not
	// YUV422 images?

	// If there's no image, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rect = refimsrcframe.m_rect;

		// Do we have an image of the type that we're looking for?
		if ((!fReflect)
				&& (refimsrcframe.m_evisimencoding == evisimencodingYUVA64))
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(refimsrcframe.m_rect);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			// We need to (try to) convert the image to the type that we're looking for.

			// Find storage for the image pixels.
			if ((fSubsampleYUV422)
					&& ((refimsrcframe.m_evisimencoding == evisimencodingUYVY)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY42T)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYUY2)
						|| (refimsrcframe.m_evisimencoding == evisimencodingY211)
						|| (refimsrcframe.m_evisimencoding == evisimencodingYVYU)))
			{
				rect.left /= 2;
				rect.top /= 2;
				rect.right /= 2;
				rect.bottom /= 2;
			}
			pimage->Allocate(rect);

			// LATER:  Use the colormap information?

			switch (refimsrcframe.m_evisimencoding)
			{
			case evisimencodingBW1bit:
			case evisimencodingGray4bit:
			case evisimencodingGray8bit:
			case evisimencodingGray16bit:
				VisGrayToYUV(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB555:
				VisRGBToYUV(refimsrcframe.m_pvData, 15,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingRGB565:
			case evisimencodingRGB24:
			case evisimencodingRGBA32:
			case evisimencodingRGB48:
			case evisimencodingRGBA64:
				VisRGBToYUV(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingYUV24:
			case evisimencodingYUVA32:
			case evisimencodingYUV48:
			case evisimencodingYUVA64:
				VisConvertCbitToCbit(refimsrcframe.m_pvData,
						VisCbitGetImEncoding(refimsrcframe.m_evisimencoding),
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect);
				break;

			case evisimencodingUYVY:
			case evisimencodingY42T:  // Not completely right, but this should work okay
				VisYUV422ToYUV((SVisUYVY *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYUY2:
			case evisimencodingY211:  // Not completely right, but this should work okay
				VisYUV422ToYUV((SVisYUYV *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYVYU:
				VisYUV422ToYUV((SVisYVYU *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, fSubsampleYUV422);
				break;

			case evisimencodingYVU9:
			case evisimencodingIF09:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 9, true);
				break;

			case evisimencodingYV12:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, true);
				break;

			case evisimencodingIYUV:
				VisYUVPlanarToYUV((BYTE *) refimsrcframe.m_pvData,
						pimage->PbPixel(0, 0), cbitDest,
						refimsrcframe.m_rect.right - refimsrcframe.m_rect.left,
						refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top,
						fReflect, 12, false);
				break;

#ifdef _DEBUG
			case evisimencodingColor8bit:
			case evisimencodingColor4bit:
			case evisimencodingY41P:
			case evisimencodingY41T:
			case evisimencodingCLJR:
			case evisimencodingCLPL:  // Need to split VisYUVPlanarToRGBA32 into two fns for this format.
			case evisimencodingY210:  // Don't really know what this format is!
			case evisimencodingY410:  // Don't really know what this format is!
			case evisimencodingYUVP:  // Don't really know what this format is!
			default:
				// LATER:  Add support for all evisimencoding values.
{
	static bool fAsserted = false;
	assert(fAsserted);
	if (!fAsserted)
	{
		char szBuf[255];
		int cpix = (refimsrcframe.m_rect.right - refimsrcframe.m_rect.left)
				* (refimsrcframe.m_rect.bottom - refimsrcframe.m_rect.top);
		int cbit = ((cpix == 0) ? 0 : 8 * refimsrcframe.m_cbData / cpix);
		sprintf(szBuf, "Unrecognized %d-bit format %#010x (== '%c%c%c%c').\n",
				cbit,
				refimsrcframe.m_evisimencoding,
				((char *) (&refimsrcframe.m_evisimencoding))[0],
				((char *) (&refimsrcframe.m_evisimencoding))[1],
				((char *) (&refimsrcframe.m_evisimencoding))[2],
				((char *) (&refimsrcframe.m_evisimencoding))[3]);
		OutputDebugString(szBuf);
	}
	fAsserted = true;
}
				break;
#endif // _DEBUG
			}
		}
	}

	// Set the time stamp in the image before returning.
	if ((refimsrcframe.m_filetime.dwLowDateTime == 0)
			&& (refimsrcframe.m_filetime.dwHighDateTime == 0))
	{
		// Set FILETIME in the image to the current time.
		pimage->SetFiletime();
	}
	else
	{
		pimage->SetFiletime(refimsrcframe.m_filetime);
	}

	return pimage;
}


CVisImage<SVisYUYV> *CVisImageFromImSrcFrame::PImageOfYUYVFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers)
{
	CVisImage<SVisYUYV> *pimage = new CVisImage<SVisYUYV>;

	// If there's no image, or if it's not in the right format, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0)
			&& (refimsrcframe.m_evisimencoding != evisimencodingYUY2))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rectImage;
		rectImage.left = refimsrcframe.m_rect.left / 2;
		rectImage.right = refimsrcframe.m_rect.right / 2;
		rectImage.top = refimsrcframe.m_rect.top;
		rectImage.bottom = refimsrcframe.m_rect.bottom;

		// Do we have an image of the type that we're looking for?
		if (!fReflect)
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(rectImage);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			pimage->Allocate(rectImage);

			VisConvertCbitToCbit(refimsrcframe.m_pvData, 32,
					pimage->PbPixel(0, 0), 32,
					rectImage.right - rectImage.left,
					rectImage.bottom - rectImage.top,
					fReflect);
		}
	}

	return pimage;
}


CVisImage<SVisYVYU> *CVisImageFromImSrcFrame::PImageOfYVYUFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers)
{
	CVisImage<SVisYVYU> *pimage = new CVisImage<SVisYVYU>;

	// If there's no image, or if it's not in the right format, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0)
			&& (refimsrcframe.m_evisimencoding != evisimencodingYVYU))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rectImage;
		rectImage.left = refimsrcframe.m_rect.left / 2;
		rectImage.right = refimsrcframe.m_rect.right / 2;
		rectImage.top = refimsrcframe.m_rect.top;
		rectImage.bottom = refimsrcframe.m_rect.bottom;

		// Do we have an image of the type that we're looking for?
		if (!fReflect)
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(rectImage);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			pimage->Allocate(rectImage);

			VisConvertCbitToCbit(refimsrcframe.m_pvData, 32,
					pimage->PbPixel(0, 0), 32,
					rectImage.right - rectImage.left,
					rectImage.bottom - rectImage.top,
					fReflect);
		}
	}

	return pimage;
}


CVisImage<SVisUYVY> *CVisImageFromImSrcFrame::PImageOfUYVYFromImSrcFrame(
		SVisImSrcFrame& refimsrcframe, bool fCopyVolatile,
			bool fCopyDMABuffers)
{
	CVisImage<SVisUYVY> *pimage = new CVisImage<SVisUYVY>;

	// If there's no image, or if it's not in the right format, fail.
	if ((pimage != 0)
			&& (refimsrcframe.m_pvData != 0)
			&& (refimsrcframe.m_cbData != 0)
			&& (refimsrcframe.m_evisimencoding != evisimencodingUYVY))
	{
		bool fReflect;
		if ((refimsrcframe.m_dwFlags & evisfiminfVertReflect) != 0)
			fReflect = true;
		else
			fReflect = false;

		RECT rectImage;
		rectImage.left = refimsrcframe.m_rect.left / 2;
		rectImage.right = refimsrcframe.m_rect.right / 2;
		rectImage.top = refimsrcframe.m_rect.top;
		rectImage.bottom = refimsrcframe.m_rect.bottom;

		// Do we have an image of the type that we're looking for?
		if (!fReflect)
		{
			// Make a CVisMemblock object to hold the pixel data.  We may
			// use this CVisMemBlock in the pimage->  If not, the memory in
			// the SVisImSrcFrame strucutre may be deleted in the
			// CVisMemBlock destructor.
			EVisMemBlock evismemblock = evismemblockAllocUserUnknown;

			if (((refimsrcframe.m_dwFlags & evisfiminfVolatileData) != 0)
						|| (refimsrcframe.m_pfnDeleteData == 0))
			{
				evismemblock = evismemblockAllocUserVolatileBuf;
			}
			else if ((refimsrcframe.m_dwFlags & evisfiminfDMABuffer) != 0)
			{
				evismemblock = evismemblockAllocUserDMABuf;
			}

			CVisMemBlock memblock(refimsrcframe.m_pvData, refimsrcframe.m_cbData,
					refimsrcframe.m_pfnDeleteData, refimsrcframe.m_pvDeleteData,
					evismemblock);

			// Indicate that we're taking control of the buffer by setting the
			// DeleteData members of the frame to 0.
			refimsrcframe.m_pfnDeleteData = 0;
			refimsrcframe.m_pvDeleteData = 0;

			bool fUseMemBlock = true;

			if ((evismemblock == evismemblockAllocUserVolatileBuf)
					&& (fCopyVolatile))
			{
				fUseMemBlock = false;
			}
			else if ((evismemblock == evismemblockAllocUserDMABuf)
					&& (fCopyDMABuffers))
			{
				fUseMemBlock = false;
			}

			if (fUseMemBlock)
			{
				pimage->SetMemBlock(memblock, refimsrcframe.m_rect);
			}
			else
			{
				pimage->Allocate(rectImage);

				CopyMemory(pimage->PbPixel(0, 0), refimsrcframe.m_pvData,
						pimage->NPixels() * pimage->CbPixel());
			}
		}
		else
		{
			pimage->Allocate(rectImage);

			VisConvertCbitToCbit(refimsrcframe.m_pvData, 32,
					pimage->PbPixel(0, 0), 32,
					rectImage.right - rectImage.left,
					rectImage.bottom - rectImage.top,
					fReflect);
		}
	}

	return pimage;
}
