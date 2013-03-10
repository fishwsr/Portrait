// VisStrip.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


inline int CVisStrip::Left(void) const
{
	return m_xLeft;
}


inline int CVisStrip::Right(void) const
{
	return m_xRight;
}


inline int CVisStrip::Width(void) const
{
	return m_xRight - m_xLeft;
}


inline int CVisStrip::Row(void) const
{
	return m_yRow;
}


inline void CVisStrip::SetLeft(int x)
{
	m_xLeft = x;
}


inline void CVisStrip::SetRight(int x)
{
	m_xRight = x;
}


inline void CVisStrip::SetWidth(int dx)
{
	m_xRight = m_xLeft + dx;
}


inline void CVisStrip::SetRow(int y)
{
	m_yRow = y;
}




inline CVisStripList::CVisStripList(
		const CVisShape& refshapeImage,
		const CVisStripListEnum& refstripenum)
  : m_shapeImage(refshapeImage),
	m_stripenum(refstripenum)
{
}


inline CVisStripList::CVisStripList(
		const CVisStripList& refstriplist)
  : m_shapeImage(refstriplist.m_shapeImage),
	m_stripenum(refstriplist.m_stripenum)
{
}


inline CVisStripList&
	CVisStripList::operator=(
		const CVisStripList& refstriplist)
{
	m_shapeImage = refstriplist.m_shapeImage;
	m_stripenum = refstriplist.m_stripenum;

	return *this;
}


inline const CVisShape& CVisStripList::ShapeImage(void) const
{
	return m_shapeImage;
}


inline CVisStripList::iterator CVisStripList::begin(void)
{
	return m_stripenum.begin();
}


inline CVisStripList::iterator CVisStripList::end(void)
{
	return m_stripenum.end();
}


inline const CVisStripListEnum&
	CVisStripList::StripEnum(void) const
{
	return m_stripenum;
}







inline const CVisStripList StripListMakePixRunList(
	const CVisPixRunList& refpixrunlist)
{
	// Strips are only used with single-band images.
	assert(refpixrunlist.ShapeImage().NBands() == 1);
	
	CVisStripListBuilder striplistbuilder;

	CVisPixRunListEnum pixrunenum(
			refpixrunlist.PixRunEnum());
	pixrunenum.Reset();

	assert(refpixrunlist.ShapeImage().top
			< refpixrunlist.ShapeImage().bottom);
	for (int y = refpixrunlist.ShapeImage().top;
			y != refpixrunlist.ShapeImage().bottom;
			++y)
	{
		// Rows must start with a (possibly zero-length) run of
		// background pixels.
		bool fForeground = false;

		// This will loop over all pixels in the row.
		int x = refpixrunlist.ShapeImage().left;
		while (x != refpixrunlist.ShapeImage().right)
		{
			assert(pixrunenum.ObjCur() >= 0);
			if (fForeground)
			{
				striplistbuilder.ObjNext().SetRow(y);
				striplistbuilder.ObjNext().SetLeft(x);
				x += pixrunenum.ObjCur();
				striplistbuilder.ObjNext().SetRight(x);
				striplistbuilder.AddObj();
			}
			else
			{
				x += pixrunenum.ObjCur();
			}

			assert(x <= refpixrunlist.ShapeImage().right);

			// The next iteration will use the next run in the list.
			fForeground = !fForeground;
			assert(pixrunenum.FMore());
			pixrunenum.Next();
		}

		// Rows must end with a (possibly zero-length) run of
		// background pixels.
		if (!fForeground)
		{
			// Add a zero-length run.
			assert(pixrunenum.ObjCur() == 0);
			assert(pixrunenum.FMore());
			pixrunenum.Next();
		}
	}
	
	// We should have proccessed each run in the list while constructing
	// the new image.
	assert(!pixrunenum.FMore());

	return CVisStripList(refpixrunlist.ShapeImage(),
			striplistbuilder);
}




template<class TPixel>
inline void SetImagePixelsFromStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelSetValue,
	const CVisStripList& refstriplist)
{
	// Strips are only used with single-band images.
	assert(refstriplist.ShapeImage().NBands() == 1);
	assert(refstriplist.ShapeImage() == refimage.Shape());
	
	CVisStripListEnum stripenum
			= refstriplist.StripEnum();
	for (stripenum.Reset();
			stripenum.FMore();
			stripenum.Next())
	{
		// This will loop over all pixels in the strip.
		CVisStrip& refstrip = stripenum.ObjCur();

		TPixel *pPixelCur
				= &(refimage.Pixel(refstrip.Left(), refstrip.Row()));

		// Note:  We don't use
		//		&(refimage.Pixel(refstrip.Right(), refstrip.Row()))
		// here because refstrip.Right() may not be < refimage.Right().
		const TPixel *pPixelLim = pPixelCur + refstrip.Width();

		assert(pPixelCur < pPixelLim);
		while (pPixelCur != pPixelLim)
		{
			*pPixelCur = refpixelSetValue;
			++ pPixelCur;
		}
	}
}



template<class TPixel>
inline void XorImagePixelsInStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelXorValue,
	const CVisStripList& refstriplist)
{
	// Strips are only used with single-band images.
	assert(refstriplist.ShapeImage().NBands() == 1);
	assert(refstriplist.ShapeImage() == refimage.Shape());
	
	CVisStripListEnum stripenum
			= refstriplist.StripEnum();
	for (stripenum.Reset();
			stripenum.FMore();
			stripenum.Next())
	{
		// This will loop over all pixels in the strip.
		CVisStrip& refstrip = stripenum.ObjCur();

		TPixel *pPixelCur
				= &(refimage.Pixel(refstrip.Left(), refstrip.Row()));

		// Note:  We don't use
		//		&(refimage.Pixel(refstrip.Right(), refstrip.Row()))
		// here because refstrip.Right() may not be < refimage.Right().
		const TPixel *pPixelLim = pPixelCur + refstrip.Width();

		assert(pPixelCur < pPixelLim);
		while (pPixelCur != pPixelLim)
		{
			*pPixelCur ^= refpixelXorValue;
			++ pPixelCur;
		}
	}
}


template<class TPixel>
inline void AddImagePixelsToStripList(
	CVisImage<TPixel>& refimage,
	const TPixel& refpixelAddValue,
	const CVisStripList& refstriplist)
{
	// Strips are only used with single-band images.
	assert(refstriplist.ShapeImage().NBands() == 1);
	assert(refstriplist.ShapeImage() == refimage.Shape());
	
	CVisStripListEnum stripenum
			= refstriplist.StripEnum();
	for (stripenum.Reset();
			stripenum.FMore();
			stripenum.Next())
	{
		// This will loop over all pixels in the strip.
		CVisStrip& refstrip = stripenum.ObjCur();

		TPixel *pPixelCur
				= &(refimage.Pixel(refstrip.Left(), refstrip.Row()));

		// Note:  We don't use
		//		&(refimage.Pixel(refstrip.Right(), refstrip.Row()))
		// here because refstrip.Right() may not be < refimage.Right().
		const TPixel *pPixelLim = pPixelCur + refstrip.Width();

		assert(pPixelCur < pPixelLim);
		while (pPixelCur != pPixelLim)
		{
			*pPixelCur += refpixelAddValue;
			++ pPixelCur;
		}
	}
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
