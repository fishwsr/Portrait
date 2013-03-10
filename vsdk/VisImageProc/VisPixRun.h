// VisPixRun.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef unsigned int CVisPixRun;


enum { evisallocunitPixRun = kcObjAllocUnitDefault };


typedef CVisObjListEnum<CVisPixRun, evisallocunitPixRun> CVisPixRunListEnum;
typedef CVisObjListBuilder<CVisPixRun, evisallocunitPixRun> CVisPixRunListBuilder;


class CVisPixRunList
{
public:
	CVisPixRunList(void);
	CVisPixRunList(const CVisShape& refshapeImage,
		const CVisPixRunListEnum& refpixrunenum);
	CVisPixRunList(const CVisPixRunList& refpixrunlist);
	CVisPixRunList& operator=(
		const CVisPixRunList& refpixrunlist);
	const CVisShape& ShapeImage(void) const;
	const CVisPixRunListEnum& PixRunEnum(void) const;

private:
	CVisShape m_shapeImage;
	CVisPixRunListEnum m_pixrunenum;
};


template<class TPixel, class TFnOpEvalPixel>
const CVisPixRunList PixRunListMakeImage(
	const CVisImage<TPixel>& refimage,
	TFnOpEvalPixel& reffnopEvalPixel);


template<class TPixel>
const CVisImage<TPixel> ImageMakePixRunList(
	const CVisPixRunList& refpixrunlist,
	const TPixel& refpixelBackground,
	const TPixel& refpixelForeground);



#include "VisPixRun.inl"