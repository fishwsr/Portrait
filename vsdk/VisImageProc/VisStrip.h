// VisStrip.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


enum { evisallocunitStrip = kcObjAllocUnitDefault };


class CVisStrip
{
public:
	int Left(void) const;
	int Right(void) const;
	int Width(void) const;
	int Row(void) const;

	void SetLeft(int x);
	void SetRight(int x);
	void SetWidth(int dx);
	void SetRow(int y);

private:
	int m_xLeft;
	int m_xRight;
	int m_yRow;
};


typedef CVisObjListEnum<CVisStrip, evisallocunitStrip> CVisStripListEnum;
typedef CVisObjListBuilder<CVisStrip, evisallocunitStrip> CVisStripListBuilder;



class CVisStripList
{
public:
	typedef CVisStripListEnum iterator;


	CVisStripList(const CVisShape& refshapeImage,
		const CVisStripListEnum& refstripenum);
	CVisStripList(const CVisStripList& refstriplist);
	CVisStripList& operator=(
		const CVisStripList& refstriplist);


	const CVisShape& ShapeImage(void) const;


	// LATER:  Add const methods that return const_iterators?
	iterator begin(void);
	iterator end(void);


	// This ia an lod method.  Use the begin() and end() methods
	// instead of this method.
	const CVisStripListEnum& StripEnum(void) const;


private:
	CVisShape m_shapeImage;
	CVisStripListEnum m_stripenum;
};



const CVisStripList StripListMakePixRunList(
	const CVisPixRunList& refpixrunlist);



template<class TPixel>
void SetImagePixelsFromStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelSetValue,
	const CVisStripList& refstriplist);


template<class TPixel>
inline void XorImagePixelsInStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelXorValue,
	const CVisStripList& refstriplist);


template<class TPixel>
inline void AddImagePixelsToStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelAddValue,
	const CVisStripList& refstriplist);



#include "VisStrip.inl"