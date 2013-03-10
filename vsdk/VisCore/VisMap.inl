// VisMap.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#ifndef VIS_CORE_MAP_INL
#define VIS_CORE_MAP_INL

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#ifdef VIS_OLD_CODE


template <class OPERATOR, class PIXELTYPE0>
void VisMap1(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c]);
        }
    }
}

template <class OPERATOR, class PIXELTYPE0, class PIXELTYPE1>
void VisMap2(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0,
             CVisImage<PIXELTYPE1> img1)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    VisCheckConformal(&img0, &img1);
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        PIXELTYPE1 *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c], p1[c]);
        }
    }
}

template <class OPERATOR, class PIXELTYPE0, class PIXELTYPE1, class PIXELTYPE2>
void VisMap3(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0,
             CVisImage<PIXELTYPE1> img1,
             CVisImage<PIXELTYPE2> img2)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    VisCheckConformal(&img0, &img1);
    VisCheckConformal(&img0, &img2);
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        PIXELTYPE1 *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        PIXELTYPE2 *p2 = img2.PtrToFirstPixelInRow(r + img2.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c], p1[c], p2[c]);
        }
    }
}

template <class OPERATOR, class PIXELTYPE0, class PIXELTYPE1, class PIXELTYPE2,
          class PIXELTYPE3>
void VisMap4(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0,
             CVisImage<PIXELTYPE1> img1,
             CVisImage<PIXELTYPE2> img2,
             CVisImage<PIXELTYPE3> img3)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    VisCheckConformal(&img0, &img1);
    VisCheckConformal(&img0, &img2);
    VisCheckConformal(&img0, &img3);
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        PIXELTYPE1 *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        PIXELTYPE2 *p2 = img2.PtrToFirstPixelInRow(r + img2.Top());
        PIXELTYPE3 *p3 = img3.PtrToFirstPixelInRow(r + img3.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c], p1[c], p2[c], p3[c]);
        }
    }
}

template <class OPERATOR, class PIXELTYPE0, class PIXELTYPE1, class PIXELTYPE2,
          class PIXELTYPE3, class PIXELTYPE4>
void VisMap5(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0,
             CVisImage<PIXELTYPE1> img1,
             CVisImage<PIXELTYPE2> img2,
             CVisImage<PIXELTYPE3> img3,
             CVisImage<PIXELTYPE4> img4)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    VisCheckConformal(&img0, &img1);
    VisCheckConformal(&img0, &img2);
    VisCheckConformal(&img0, &img3);
    VisCheckConformal(&img0, &img4);
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        PIXELTYPE1 *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        PIXELTYPE2 *p2 = img2.PtrToFirstPixelInRow(r + img2.Top());
        PIXELTYPE3 *p3 = img3.PtrToFirstPixelInRow(r + img3.Top());
        PIXELTYPE4 *p4 = img4.PtrToFirstPixelInRow(r + img4.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c], p1[c], p2[c], p3[c], p4[c]);
        }
    }
}

template <class OPERATOR, class PIXELTYPE0, class PIXELTYPE1, class PIXELTYPE2,
          class PIXELTYPE3, class PIXELTYPE4, class PIXELTYPE5>
void VisMap6(OPERATOR& fn,
             CVisImage<PIXELTYPE0> img0,
             CVisImage<PIXELTYPE1> img1,
             CVisImage<PIXELTYPE2> img2,
             CVisImage<PIXELTYPE3> img3,
             CVisImage<PIXELTYPE4> img4,
             CVisImage<PIXELTYPE5> img5)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width() * img0.NBands();
    VisCheckConformal(&img0, &img1);
    VisCheckConformal(&img0, &img2);
    VisCheckConformal(&img0, &img3);
    VisCheckConformal(&img0, &img4);
    VisCheckConformal(&img0, &img5);
    for (r = 0; r < nrows; r++) {
        PIXELTYPE0 *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        PIXELTYPE1 *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        PIXELTYPE2 *p2 = img2.PtrToFirstPixelInRow(r + img2.Top());
        PIXELTYPE3 *p3 = img3.PtrToFirstPixelInRow(r + img3.Top());
        PIXELTYPE4 *p4 = img4.PtrToFirstPixelInRow(r + img4.Top());
        PIXELTYPE5 *p5 = img5.PtrToFirstPixelInRow(r + img5.Top());
        for (c = 0; c < ncols; c++) {
            fn(p0[c], p1[c], p2[c], p3[c], p4[c], p5[c]);
        }
    }
}



#else // VIS_OLD_CODE



template <class TFnOp, class TPixel0>
inline void VisMap1(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0)
{
	assert((refrect.top < refrect.bottom) && (refrect.left < refrect.right));

	int y = refrect.top;
	int yLim = refrect.bottom;

	// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
	// (The loop condition only involves a comparison of i to 0.)
	int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

	assert(refimage0.Shape().Includes(refrect));
	TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
	int cbRow0 = refimage0.CbRow();

	for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0), ++y)
	{
		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		for (int i = iFirst; i < 0; ++i)
		{
			fnop(p0[i]);
		}
	}
}

template <class TFnOp, class TPixel0>
inline void VisMap1(TFnOp& fnop, CVisImage<TPixel0>& refimage0)
{
	VisMap1(refimage0.Rect(), fnop, refimage0);
}

template <class TFnOp, class TPixel>
inline void VisMap1P(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0, const TPixel& refpixel)
{
	assert((refrect.top < refrect.bottom) && (refrect.left < refrect.right));

	int y = refrect.top;
	int yLim = refrect.bottom;

	// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
	// (The loop condition only involves a comparison of i to 0.)
	int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

	assert(refimage0.Shape().Includes(refrect));
	TPixel *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
	int cbRow0 = refimage0.CbRow();

	for (; y < yLim; p0 = (TPixel *) (((BYTE *) p0) + cbRow0), ++y)
	{
		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		for (int i = iFirst; i < 0; ++i)
		{
			fnop(p0[i], refpixel);
		}
	}
}

template <class TFnOp, class TPixel>
inline void VisMap1P(TFnOp& fnop, CVisImage<TPixel>& refimage0,
		const TPixel& refpixel)
{
	VisMap1P(refimage0.Rect(), fnop, refimage0, refpixel);
}

template <class TFnOp, class TPixel0, class TPixel1>
inline void VisMap2(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1)
{
	assert(refimage0.NBands() == refimage1.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel1 *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel1 *) (((BYTE *) p1) + cbRow1), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel0, class TPixel1>
inline void VisMap2(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1)
{
	VisMap2((refimage0.Rect() & refimage1.Rect()), fnop, refimage0,
			refimage1);
}

template <class TFnOp, class TPixel>
inline void VisMap1P1(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		const TPixel& refpixel,
		CVisImage<TPixel>& refimage1)
{
	assert(refimage0.NBands() == refimage1.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		for (; y < yLim; p0 = (TPixel *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel *) (((BYTE *) p1) + cbRow1), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], refpixel, p1[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel>
inline void VisMap1P1(TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		const TPixel& refpixel,
		CVisImage<TPixel>& refimage1)
{
	VisMap1P1((refimage0.Rect() & refimage1.Rect()), fnop, refimage0,
			refpixel, refimage1);
}

template <class TFnOp, class TPixel>
inline void VisMap2P(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	assert(refimage0.NBands() == refimage1.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		for (; y < yLim; p0 = (TPixel *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel *) (((BYTE *) p1) + cbRow1), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i], refpixel);
			}
		}
	}
}

template <class TFnOp, class TPixel>
inline void VisMap2P(TFnOp& fnop,
		CVisImage<TPixel>& refimage0,
		CVisImage<TPixel>& refimage1,
		const TPixel& refpixel)
{
	VisMap2P((refimage0.Rect() & refimage1.Rect()), fnop, refimage0,
			refimage1, refpixel);
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2>
inline void VisMap3(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2)
{
	assert(refimage0.NBands() == refimage1.NBands());
	assert(refimage0.NBands() == refimage2.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel1 *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		assert(refimage2.Shape().Includes(refrect));
		TPixel2 *p2 = refimage2.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow2 = refimage2.CbRow();

		for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel1 *) (((BYTE *) p1) + cbRow1),
				p2 = (TPixel2 *) (((BYTE *) p2) + cbRow2), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i], p2[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2>
inline void VisMap3(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2)
{
	CVisRect rect = (refimage0.Rect() & refimage1.Rect());
	VisMap3((rect & refimage2.Rect()), fnop, refimage0,
			refimage1, refimage2);
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3>
inline void VisMap4(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3)
{
	assert(refimage0.NBands() == refimage1.NBands());
	assert(refimage0.NBands() == refimage2.NBands());
	assert(refimage0.NBands() == refimage3.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel1 *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		assert(refimage2.Shape().Includes(refrect));
		TPixel2 *p2 = refimage2.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow2 = refimage2.CbRow();

		assert(refimage3.Shape().Includes(refrect));
		TPixel3 *p3 = refimage3.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow3 = refimage3.CbRow();

		for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel1 *) (((BYTE *) p1) + cbRow1),
				p2 = (TPixel2 *) (((BYTE *) p2) + cbRow2),
				p3 = (TPixel3 *) (((BYTE *) p3) + cbRow3), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i], p2[i], p3[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3>
inline void VisMap4(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3)
{
	CVisRect rect = (refimage0.Rect() & refimage1.Rect());
	rect &= refimage2.Rect();
	rect &= refimage3.Rect();
	VisMap4(rect, fnop, refimage0, refimage1, refimage2, refimage3);
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4>
inline void VisMap5(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4)
{
	assert(refimage0.NBands() == refimage1.NBands());
	assert(refimage0.NBands() == refimage2.NBands());
	assert(refimage0.NBands() == refimage3.NBands());
	assert(refimage0.NBands() == refimage4.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel1 *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		assert(refimage2.Shape().Includes(refrect));
		TPixel2 *p2 = refimage2.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow2 = refimage2.CbRow();

		assert(refimage3.Shape().Includes(refrect));
		TPixel3 *p3 = refimage3.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow3 = refimage3.CbRow();

		assert(refimage4.Shape().Includes(refrect));
		TPixel4 *p4 = refimage4.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow4 = refimage4.CbRow();

		for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel1 *) (((BYTE *) p1) + cbRow1),
				p2 = (TPixel2 *) (((BYTE *) p2) + cbRow2),
				p3 = (TPixel3 *) (((BYTE *) p3) + cbRow3),
				p4 = (TPixel4 *) (((BYTE *) p4) + cbRow4), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i], p2[i], p3[i], p4[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4>
inline void VisMap5(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4)
{
	CRect rect = (refimage0.Rect() & refimage1.Rect());
	rect &= refimage2.Rect();
	rect &= refimage3.Rect();
	rect &= refimage4.Rect();
	VisMap5(rect, fnop, refimage0, refimage1, refimage2, refimage3, refimage4);
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4, class TPixel5>
inline void VisMap6(const RECT& refrect, TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4,
		CVisImage<TPixel5>& refimage5)
{
	assert(refimage0.NBands() == refimage1.NBands());
	assert(refimage0.NBands() == refimage2.NBands());
	assert(refimage0.NBands() == refimage3.NBands());
	assert(refimage0.NBands() == refimage4.NBands());
	assert(refimage0.NBands() == refimage5.NBands());

	if ((refrect.top < refrect.bottom) && (refrect.left < refrect.right))
	{
		int y = refrect.top;
		int yLim = refrect.bottom;

		// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
		// (The loop condition only involves a comparison of i to 0.)
		int iFirst = (refrect.left - refrect.right) * refimage0.NBands();

		assert(refimage0.Shape().Includes(refrect));
		TPixel0 *p0 = refimage0.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow0 = refimage0.CbRow();

		assert(refimage1.Shape().Includes(refrect));
		TPixel1 *p1 = refimage1.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow1 = refimage1.CbRow();

		assert(refimage2.Shape().Includes(refrect));
		TPixel2 *p2 = refimage2.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow2 = refimage2.CbRow();

		assert(refimage3.Shape().Includes(refrect));
		TPixel3 *p3 = refimage3.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow3 = refimage3.CbRow();

		assert(refimage4.Shape().Includes(refrect));
		TPixel4 *p4 = refimage4.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow4 = refimage4.CbRow();

		assert(refimage5.Shape().Includes(refrect));
		TPixel5 *p5 = refimage5.PixelAddress(refrect.left, y, 0) - iFirst;
		int cbRow5 = refimage5.CbRow();

		for (; y < yLim; p0 = (TPixel0 *) (((BYTE *) p0) + cbRow0),
				p1 = (TPixel1 *) (((BYTE *) p1) + cbRow1),
				p2 = (TPixel2 *) (((BYTE *) p2) + cbRow2),
				p3 = (TPixel3 *) (((BYTE *) p3) + cbRow3),
				p4 = (TPixel4 *) (((BYTE *) p4) + cbRow4),
				p5 = (TPixel5 *) (((BYTE *) p5) + cbRow5), ++y)
		{
			// iFirst will be < 0.  We'll loop from i == iFirst to i == 0.
			// (The loop condition only involves a comparison of i to 0.)
			for (int i = iFirst; i < 0; ++i)
			{
				fnop(p0[i], p1[i], p2[i], p3[i], p4[i], p5[i]);
			}
		}
	}
}

template <class TFnOp, class TPixel0, class TPixel1, class TPixel2,
	class TPixel3, class TPixel4, class TPixel5>
inline void VisMap6(TFnOp& fnop,
		CVisImage<TPixel0>& refimage0,
		CVisImage<TPixel1>& refimage1,
		CVisImage<TPixel2>& refimage2,
		CVisImage<TPixel3>& refimage3,
		CVisImage<TPixel4>& refimage4,
		CVisImage<TPixel5>& refimage5)
{
	CRect rect = (refimage0.Rect() & refimage1.Rect());
	rect &= refimage2.Rect();
	rect &= refimage3.Rect();
	rect &= refimage4.Rect();
	rect &= refimage5.Rect();
	VisMap6(rect, fnop, refimage0, refimage1, refimage2, refimage3, refimage4,
			refimage5);
}


#endif // VIS_OLD_CODE



#endif // VIS_CORE_MAP_INL
