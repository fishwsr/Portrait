// VisConvertRGB.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"


#include "VisConvertRGB.inl"



VisImSrcExport void VISAPI VisGrayToRGB(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	VisInlineGrayToRGB(pvSrc, cbitSrc, pvDest, cbitDest, dx, dy, fReflect);
}


VisImSrcExport void VISAPI VisRGBToGray(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	VisInlineRGBToGray(pvSrc, cbitSrc, pvDest, cbitDest, dx, dy, fReflect);
}


VisImSrcExport void VISAPI VisRGBToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	VisInlineRGBToYUV(pvSrc, cbitSrc, pvDest, cbitDest, dx, dy, fReflect);
}
