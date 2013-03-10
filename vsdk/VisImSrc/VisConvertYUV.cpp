// VisConvertYUV.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"


#include "VisConvertYUV.inl"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


VisImSrcExport unsigned long VISAPI VisGammaCorrect(unsigned long ul)
{
	return VisInlineGammaCorrect(ul);
}


// LATER:  In the code in the INL file, we shift unsigned long values left and
// then check ulSign (== (1 << (31 - iShift))) to see if the result was negative.
// It would probably be better to shift long values, so that the shift extends the
// sign and we can check (1 << 31) to see if the result was negative.


template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToYUV(const TPixYUV *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, bool fSubsample)
{
	VisInlineYUV422ToYUV(ppixSrc, pvDest, cbitDest, dx, dy, fReflect, fSubsample);
}

VisImSrcExport void VISAPI VisYUVPlanarToYUV(const void *pvSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, int cbppSrc,
		bool fVBeforeU)
{
	VisInlineYUVPlanarToYUV(pvSrc, pvDest, cbitDest, dx, dy, fReflect, cbppSrc,
			fVBeforeU);
}



template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToRGBA32(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample, int evisconvyuv)
{
	VisInlineYUV422ToRGBA32(ppixSrc, pvDest, dx, dy, fReflect, fSubsample,
			evisconvyuv);
}

VisImSrcExport void VISAPI VisYUV444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv)
{
	VisInlineYUV444ByteToRGBA32(pvSrc, pvDest, dx, dy, fReflect,
			evisconvyuv, false);
}

VisImSrcExport void VISAPI VisYUVA444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv)
{
	VisInlineYUV444ByteToRGBA32(pvSrc, pvDest, dx, dy, fReflect,
			evisconvyuv, true);
}

VisImSrcExport void VISAPI VisYUV444UShortToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv)
{
	VisInlineYUV444UShortToRGBA32(pvSrc, pvDest, dx, dy, fReflect,
			evisconvyuv, false);
}

VisImSrcExport void VISAPI VisYUVA444UShortToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv)
{
	VisInlineYUV444UShortToRGBA32(pvSrc, pvDest, dx, dy, fReflect,
			evisconvyuv, true);
}


VisImSrcExport void VISAPI VisYUVPlanarToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv, int cbppSrc,
		bool fVBeforeU)
{
	VisInlineYUVPlanarToRGBA32(pvSrc, pvDest, dx, dy, fReflect,
			evisconvyuv, cbppSrc, fVBeforeU);
}





template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToGrayByte(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample)
{
	VisInlineYUV422ToGrayByte(ppixSrc, pvDest, dx, dy, fReflect, fSubsample);
}

VisImSrcExport void VISAPI VisYUV444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444ByteToGrayByte(pvSrc, pvDest, dx, dy, fReflect, false);
}

VisImSrcExport void VISAPI VisYUVA444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444ByteToGrayByte(pvSrc, pvDest, dx, dy, fReflect, true);
}

VisImSrcExport void VISAPI VisYUV444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444UShortToGrayByte(pvSrc, pvDest, dx, dy, fReflect, false);
}

VisImSrcExport void VISAPI VisYUVA444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444UShortToGrayByte(pvSrc, pvDest, dx, dy, fReflect, true);
}


VisImSrcExport void VISAPI VisYUVPlanarToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int cbppSrc,
		bool fVBeforeU)
{
	VisInlineYUVPlanarToGrayByte(pvSrc, pvDest, dx, dy, fReflect,
			cbppSrc, fVBeforeU);
}



template<class TPixYUV>
VisImSrcExport void VISAPI VisYUV422ToGrayULong(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample)
{
	VisInlineYUV422ToGrayULong(ppixSrc, pvDest, dx, dy, fReflect,
			fSubsample);
}

VisImSrcExport void VISAPI VisYUV444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444ByteToGrayULong(pvSrc, pvDest, dx, dy, fReflect,
			false);
}

VisImSrcExport void VISAPI VisYUVA444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444ByteToGrayULong(pvSrc, pvDest, dx, dy, fReflect,
			true);
}

VisImSrcExport void VISAPI VisYUV444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444UShortToGrayULong(pvSrc, pvDest, dx, dy, fReflect,
			false);
}

VisImSrcExport void VISAPI VisYUVA444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineYUV444UShortToGrayULong(pvSrc, pvDest, dx, dy, fReflect,
			true);
}


VisImSrcExport void VISAPI VisYUVPlanarToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int cbppSrc,
		bool fVBeforeU)
{
	VisInlineYUVPlanarToGrayULong(pvSrc, pvDest, dx, dy, fReflect,
			cbppSrc, fVBeforeU);
}



// LATER:  Add code to convert gray to 24-bit YUV?
VisImSrcExport void VISAPI VisGrayToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	VisInlineGrayToYUV(pvSrc, cbitSrc, pvDest, cbitDest, dx, dy, fReflect);
}



///////////////////////////////////////////////////////////////////////////
// Explict template instantiation.

template
VisImSrcExport void VISAPI VisYUV422ToYUV(const SVisYUYV *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToYUV(const SVisYVYU *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToYUV(const SVisUYVY *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, bool fSubsample);


template
VisImSrcExport void VISAPI VisYUV422ToRGBA32(const SVisYUYV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample, int evisconvyuv);

template
VisImSrcExport void VISAPI VisYUV422ToRGBA32(const SVisYVYU *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample, int evisconvyuv);

template
VisImSrcExport void VISAPI VisYUV422ToRGBA32(const SVisUYVY *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample, int evisconvyuv);


template
VisImSrcExport void VISAPI VisYUV422ToGrayByte(const SVisYUYV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToGrayByte(const SVisYVYU *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToGrayByte(const SVisUYVY *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);


template
VisImSrcExport void VISAPI VisYUV422ToGrayULong(const SVisYUYV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToGrayULong(const SVisYVYU *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);

template
VisImSrcExport void VISAPI VisYUV422ToGrayULong(const SVisUYVY *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample);

