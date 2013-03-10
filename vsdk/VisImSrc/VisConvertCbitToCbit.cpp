// VisConvertCbitToCbit.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"


#include "VisConvertCbitToCbit.inl"



VisImSrcExport void VISAPI VisConvert555To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineConvert555To32bit(pvSrc, pvDest, dx, dy, fReflect);
}


VisImSrcExport void VISAPI VisConvert565To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	VisInlineConvert565To32bit(pvSrc, pvDest, dx, dy, fReflect);
}


VisImSrcExport void VISAPI VisConvertCbitToCbit(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	VisInlineConvertCbitToCbit(pvSrc, cbitSrc, pvDest, cbitDest, dx, dy, fReflect);
}

