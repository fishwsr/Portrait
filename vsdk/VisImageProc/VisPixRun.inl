// VisPixRun.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG




inline CVisPixRunList::CVisPixRunList(void)
  : m_shapeImage(CVisRect(0, 0, 0, 0), 1)
{
}



inline CVisPixRunList::CVisPixRunList(
		const CVisShape& refshapeImage,
		const CVisPixRunListEnum& refpixrunenum)
  : m_shapeImage(refshapeImage),
	m_pixrunenum(refpixrunenum)
{
}


inline CVisPixRunList::CVisPixRunList(
		const CVisPixRunList& refpixrunlist)
  : m_shapeImage(refpixrunlist.m_shapeImage),
	m_pixrunenum(refpixrunlist.m_pixrunenum)
{
}


inline CVisPixRunList&
	CVisPixRunList::operator=(
		const CVisPixRunList& refpixrunlist)
{
	m_shapeImage = refpixrunlist.m_shapeImage;
	m_pixrunenum = refpixrunlist.m_pixrunenum;

	return *this;
}


inline const CVisShape& CVisPixRunList::ShapeImage(void) const
{
	return m_shapeImage;
}


inline const CVisPixRunListEnum&
	CVisPixRunList::PixRunEnum(void) const
{
	return m_pixrunenum;
}





template<class TPixel, class TFnOpEvalPixel>
inline const CVisPixRunList PixRunListMakeImage(
	const CVisImage<TPixel>& refimage,
	TFnOpEvalPixel& reffnopEvalPixel)
{
	CVisPixRunListBuilder pixrunlistbuilder;

	// This is the number of pixels to examine in each row.
	int dpPixelWidth = refimage.Width() * refimage.NBands();

	assert(refimage.Top() < refimage.Bottom());
	for (int y = refimage.Top(); y != refimage.Bottom(); ++y)
	{
		// Rows must start with a (possibly zero-length) run of
		// background pixels.
		BOOL fForegroundPrev = false;
		unsigned int cPixelRun = 0;

		// This will loop over all pixels in the row.
		const TPixel *pPixelCur = refimage.PtrToFirstPixelInRow(y);
		const TPixel *pPixelLim = pPixelCur + dpPixelWidth;
		for ( ; pPixelCur != pPixelLim; ++pPixelCur)
		{
			BOOL fForegroundCur = reffnopEvalPixel(*pPixelCur);
			if (fForegroundPrev == fForegroundCur)
			{
				// Continue the current run.
				++ cPixelRun;
			}
			else
			{
				// Record the previous run.
				pixrunlistbuilder.AddObj(cPixelRun);

				// Start a new run.
				cPixelRun = 1;
				fForegroundPrev = fForegroundCur;
			}
		}

		// Record the current run.
		pixrunlistbuilder.AddObj(cPixelRun);

		// Rows must end with a (possibly zero-length) run of
		// background pixels.
		if (fForegroundPrev)
		{
			// Add a zero-length run.
			pixrunlistbuilder.AddObj(0);
		}
	}

	return CVisPixRunList(refimage.Shape(), pixrunlistbuilder);
}



template<class TPixel>
inline const CVisImage<TPixel> ImageMakePixRunList(
	const CVisPixRunList& refpixrunlist,
	const TPixel& refpixelBackground,
	const TPixel& refpixelForeground)
{
	CVisImage<TPixel> image(refpixrunlist.ShapeImage());

	CVisPixRunListEnum pixrunenum(
			refpixrunlist.PixRunEnum());
	pixrunenum.Reset();

	// This is the number of pixels to examine in each row.
	int dpPixelWidth = image.Width() * image.NBands();

	assert(image.Top() < image.Bottom());
	for (int y = image.Top(); y != image.Bottom(); ++y)
	{
		// Rows must start with a (possibly zero-length) run of
		// background pixels.
		bool fForeground = false;

		// This will loop over all pixels in the row.
		TPixel *pPixelCur = image.PtrToFirstPixelInRow(y);
		const TPixel *pPixelLim = pPixelCur + dpPixelWidth;
		while (pPixelCur != pPixelLim)
		{
			assert(pixrunenum.ObjCur() >= 0);
			if (fForeground)
			{
				for (unsigned int cPixelRun = pixrunenum.ObjCur();
						cPixelRun != 0;
						++pPixelCur, --cPixelRun)
				{
					*pPixelCur = refpixelForeground;
				}
			}
			else
			{
				for (unsigned int cPixelRun = pixrunenum.ObjCur();
						cPixelRun != 0;
						++pPixelCur, --cPixelRun)
				{
					*pPixelCur = refpixelBackground;
				}
			}

			assert(pPixelCur <= pPixelLim);

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

	return image;
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
