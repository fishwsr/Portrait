// VisCopyStandardPixels.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// Prototypes of the main functions.


VisCoreExport void VISAPI VisCopyPixelBytes(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize);


inline bool VisCopyGrayPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax);


inline bool VisCopyRGBAPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax);


inline bool VisCopyYUVAPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax);


inline bool VisCopyGrayPixelsToRGBA(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax);


inline bool VisCopyGrayPixelsToYUVA(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax);



// Prototypes of helper functions.


template<class TNumSrc>
inline bool VisCopyRGBAPixelsHelper1(int bandSrc, int bandDest,
		const CVisImage< CVisRGBA<TNumSrc> >& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);

template<class TNumSrcYA, class TNumSrcUV>
inline bool VisCopyYUVAPixelsHelper1(int bandSrc, int bandDest,
		const CVisImage< CVisYUVA<TNumSrcYA, TNumSrcUV> >& refimageSrc,
		CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);


template<class TSrc>
inline bool VisCopyGrayPixelsToRGBAHelper1(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);


template<class TSrc>
inline bool VisCopyGrayPixelsToYUVAHelper1(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);


template<class TNumSrc, class TNumDest>
inline bool VisCopyRGBAPixelsHelper2(int bandSrc, int bandDest,
		const CVisImage< CVisRGBA<TNumSrc> >& refimageSrc,
		CVisImage< CVisRGBA<TNumDest> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);

template<class TNumSrcYA, class TNumSrcUV, class TNumDestYA, class TNumDestUV>
inline bool VisCopyYUVAPixelsHelper2(int bandSrc, int bandDest,
		const CVisImage< CVisYUVA<TNumSrcYA, TNumSrcUV> >& refimageSrc,
		CVisImage< CVisYUVA<TNumDestYA, TNumDestUV> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);


template<class TSrc, class TNumDest>
inline bool VisCopyGrayPixelsToRGBAHelper2(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc, CVisImage< CVisRGBA<TNumDest> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);


template<class TSrc, class TNumDestYA, class TNumDestUV>
inline bool VisCopyGrayPixelsToYUVAHelper2(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc,
		CVisImage< CVisYUVA<TNumDestYA, TNumDestUV> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax);



// Implementations.


inline bool VisCopyGrayPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax)
{
	bool fRet = false;

	if ((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtGray)
	{
		switch (refimageDest.PixFmt() & evispixfmtStructureMask)
		{
		case evispixfmtGray:
			{
				// Create enumerators to pass to the functions that do the copying.
				RECT rectDest;
				rectDest.left = pointDest.x;
				rectDest.top = pointDest.y;
				rectDest.right = rectDest.left + (refrectSrc.right - refrectSrc.left);
				rectDest.bottom = rectDest.top + (refrectSrc.bottom - refrectSrc.top);
				assert(refimageDest.ContainsPoint(pointDest, 0));
				assert(refimageDest.ContainsPoint(rectDest.right - 1,
						rectDest.bottom - 1, 0));
				CVisEnumPixel enumpixelSrc(refimageSrc, refrectSrc, bandSrc);
				CVisEnumPixel enumpixelDest(refimageDest, rectDest, bandDest);

				VisCopyGrayPixels(evisnormalize, enumpixelSrc, enumpixelDest,
						dblMin, dblMax);
				fRet = true;
			}
			break;

		case evispixfmtRGBA:
			fRet = VisCopyGrayPixelsToRGBA(bandSrc, bandDest, refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtYUVA:
			fRet = VisCopyGrayPixelsToYUVA(bandSrc, bandDest, refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;
		}
	}

	return fRet;
}


inline bool VisCopyRGBAPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax)
{
	bool fRet = false;

	if (((refimageSrc.PixFmt() & evispixfmtStructureMask)
					== evispixfmtRGBA)
			&& ((refimageDest.PixFmt() & evispixfmtStructureMask)
					== evispixfmtRGBA))
	{
		switch (refimageSrc.PixFmt()
				& (evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
		{
		case (evispixfmtChar | evispixfmtSigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<signed char> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case (evispixfmtShort | evispixfmtSigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<short> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case (evispixfmtInt | evispixfmtSigned):
		case (evispixfmtLong | evispixfmtSigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<long> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case (evispixfmtChar | evispixfmtUnsigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<unsigned char> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case (evispixfmtShort | evispixfmtUnsigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<unsigned short> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case (evispixfmtInt | evispixfmtUnsigned):
		case (evispixfmtLong | evispixfmtUnsigned):
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<unsigned long> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtFloat:
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<float> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtDouble:
			fRet = VisCopyRGBAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisRGBA<double> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
	}

	return fRet;
}


inline bool VisCopyYUVAPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax)
{
	bool fRet = false;

	if (((refimageSrc.PixFmt() & evispixfmtStructureMask)
					== evispixfmtYUVA)
			&& ((refimageDest.PixFmt() & evispixfmtStructureMask)
					== evispixfmtYUVA))
	{
		switch (refimageSrc.PixFmt() & evispixfmtNumericTypeMask)
		{
		case evispixfmtChar:
			fRet = VisCopyYUVAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisYUVA<unsigned char, signed char> >&)
							refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtShort:
			fRet = VisCopyYUVAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisYUVA<unsigned short, short> >&)
							refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtInt:
		case evispixfmtLong:
			fRet = VisCopyYUVAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisYUVA<unsigned long, long> >&)
							refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtFloat:
			fRet = VisCopyYUVAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisYUVA<float, float> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

		case evispixfmtDouble:
			fRet = VisCopyYUVAPixelsHelper1(bandSrc, bandDest,
					(const CVisImage< CVisYUVA<double, double> >&) refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
			break;

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
	}

	return fRet;
}


inline bool VisCopyGrayPixelsToRGBA(int bandSrc, int bandDest,
		const CVisImageBase& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtGray);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtRGBA);

	switch (refimageSrc.PixFmt()
			& (evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<signed char>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<short>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<long>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned char>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned short>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned long>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<float>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyGrayPixelsToRGBAHelper1(bandSrc, bandDest,
				(const CVisImage<double>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}


inline bool VisCopyGrayPixelsToYUVA(int bandSrc, int bandDest,
		const CVisImageBase& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtGray);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtYUVA);

	switch (refimageSrc.PixFmt()
			& (evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<signed char>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<short>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<long>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned char>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned short>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<unsigned long>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<float>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyGrayPixelsToYUVAHelper1(bandSrc, bandDest,
				(const CVisImage<double>&) refimageSrc, refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}




template<class TNumSrc>
inline bool VisCopyRGBAPixelsHelper1(int bandSrc, int bandDest,
		const CVisImage< CVisRGBA<TNumSrc> >& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtRGBA);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtRGBA);

	switch (refimageDest.PixFmt()
			& (evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<signed char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtSigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<float> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyRGBAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<double> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}


template<class TNumSrcYA, class TNumSrcUV>
inline bool VisCopyYUVAPixelsHelper1(int bandSrc, int bandDest,
		const CVisImage< CVisYUVA<TNumSrcYA, TNumSrcUV> >& refimageSrc,
		CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtYUVA);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtYUVA);

	switch (refimageDest.PixFmt() & evispixfmtNumericTypeMask)
	{
	case evispixfmtChar:
		fRet = VisCopyYUVAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned char, signed char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtShort:
		fRet = VisCopyYUVAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned short, short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtInt:
	case evispixfmtLong:
		fRet = VisCopyYUVAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned long, long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyYUVAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<float, float> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyYUVAPixelsHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<double, double> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}



template<class TSrc>
inline bool VisCopyGrayPixelsToRGBAHelper1(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtGray);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtRGBA);

	switch (refimageDest.PixFmt()
			& (evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<signed char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<unsigned long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<float> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyGrayPixelsToRGBAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisRGBA<double> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}


template<class TSrc>
inline bool VisCopyGrayPixelsToYUVAHelper1(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	assert((refimageSrc.PixFmt() & evispixfmtStructureMask) == evispixfmtGray);
	assert((refimageDest.PixFmt() & evispixfmtStructureMask) == evispixfmtYUVA);

	switch (refimageDest.PixFmt() & evispixfmtNumericTypeMask)
	{
	case evispixfmtChar:
		fRet = VisCopyGrayPixelsToYUVAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned char, signed char> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtShort:
		fRet = VisCopyGrayPixelsToYUVAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned short, short> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtInt:
	case evispixfmtLong:
		fRet = VisCopyGrayPixelsToYUVAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<unsigned long, long> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtFloat:
		fRet = VisCopyGrayPixelsToYUVAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<float, float> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

	case evispixfmtDouble:
		fRet = VisCopyGrayPixelsToYUVAHelper2(bandSrc, bandDest, refimageSrc,
				(CVisImage< CVisYUVA<double, double> >&) refimageDest,
				refrectSrc, pointDest, evisnormalize, dblMin, dblMax);
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}

	return fRet;
}



template<class TNumSrc, class TNumDest>
inline bool VisCopyRGBAPixelsHelper2(int bandSrc, int bandDest,
		const CVisImage< CVisRGBA<TNumSrc> >& refimageSrc,
		CVisImage< CVisRGBA<TNumDest> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	int cbandSrc = ((bandSrc == - 1) ? refimageSrc.NBands() : 1);
	int cbandDest = ((bandDest == - 1) ? refimageDest.NBands() : 1);
	assert(cbandSrc == cbandDest);

	if (bandSrc == - 1)
		bandSrc = 0;
	if (bandDest == - 1)
		bandDest = 0;

	CVisImage<TNumSrc> imageSrcAlias;
	imageSrcAlias.Alias(refimageSrc);
	CVisImage<TNumDest> imageDestAlias;
	imageDestAlias.Alias(refimageDest);

	bool fUseMinAndMax = false;
	if (((evisnormalize == evisnormalizeRelative)
				|| (evisnormalize == evisnormalizeAbsolute)
				|| (evisnormalize == evisnormalizeRelMagnitude)
				|| (evisnormalize == evisnormalizeAbsMagnitude))
			&& (dblMin >= dblMax))
	{
		fRet = true;

		// Need to find dblMin and dblMax for all components.
		fUseMinAndMax = true;
		CVisFnOpFindMinMax<TNumSrc> fnop;
		
		RECT rectDest;
		rectDest.left = pointDest.x;
		rectDest.top = pointDest.y;
		rectDest.right = rectDest.left + (refrectSrc.right - refrectSrc.left);
		rectDest.bottom = rectDest.top + (refrectSrc.bottom - refrectSrc.top);
		assert(refimageDest.ContainsPoint(pointDest, 0));
		assert(refimageDest.ContainsPoint(rectDest.right - 1,
				rectDest.bottom - 1, 0));

		for (int ibandOrig = 0; ibandOrig < cbandSrc; ++ibandOrig)
		{
			for (int iComponent = 0; iComponent < 3; ++iComponent)
			{
				if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + iComponent,
						(bandDest + ibandOrig) * 4 + iComponent,
						imageSrcAlias, imageDestAlias, refrectSrc, pointDest,
						evisnormalize, dblMin, dblMax))
				{
					fRet = false;
					break;
				}
				CVisEnumPixelOf<TNumSrc> enumpixelSrc(imageSrcAlias, refrectSrc,
						(bandSrc + ibandOrig) * 4 + iComponent);
				CVisEnumPixelOf<TNumSrc> enumpixelDest(imageDestAlias, rectDest,
						(bandDest + ibandOrig) * 4 + iComponent);

				for (enumpixelSrc.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next())
				{
					fnop(enumpixelSrc.PixelCur());
				}
			}
		}

		dblMax = fnop.DblMax(evisnormalize);
		dblMin = fnop.DblMin(evisnormalize);
	}

	fRet = true;
	for (int ibandOrig = 0; ibandOrig < cbandSrc; ++ibandOrig)
	{
		for (int iComponent = 0; iComponent < 4; ++iComponent)
		{
			if ((iComponent < 3) || (!fUseMinAndMax))
			{
				if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + iComponent,
						(bandDest + ibandOrig) * 4 + iComponent,
						imageSrcAlias, imageDestAlias, refrectSrc, pointDest,
						evisnormalize, dblMin, dblMax))
				{
					fRet = false;
					break;
				}
			}
			else
			{
				if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + iComponent,
						(bandDest + ibandOrig) * 4 + iComponent,
						imageSrcAlias, imageDestAlias, refrectSrc, pointDest,
						evisnormalizeAdjustNums, dblMin, dblMax))
				{
					fRet = false;
					break;
				}
			}
		}
	}

	return fRet;
}


template<class TNumSrcYA, class TNumSrcUV, class TNumDestYA, class TNumDestUV>
inline bool VisCopyYUVAPixelsHelper2(int bandSrc, int bandDest,
		const CVisImage< CVisYUVA<TNumSrcYA, TNumSrcUV> >& refimageSrc,
		CVisImage< CVisYUVA<TNumDestYA, TNumDestUV> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = false;

	int cbandSrc = ((bandSrc == - 1) ? refimageSrc.NBands() : 1);
	int cbandDest = ((bandDest == - 1) ? refimageDest.NBands() : 1);
	assert(cbandSrc == cbandDest);

	if (bandSrc == - 1)
		bandSrc = 0;
	if (bandDest == - 1)
		bandDest = 0;

	assert(sizeof(TNumSrcYA) == sizeof(TNumSrcUV));
	assert(sizeof(TNumDestYA) == sizeof(TNumDestUV));
	CVisImage<TNumSrcYA> imageSrcAliasYA;
	imageSrcAliasYA.Alias(refimageSrc);
	CVisImage<TNumDestYA> imageDestAliasYA;
	imageDestAliasYA.Alias(refimageDest);
	CVisImage<TNumSrcUV> imageSrcAliasUV;
	imageSrcAliasUV.Alias(refimageSrc);
	CVisImage<TNumDestUV> imageDestAliasUV;
	imageDestAliasUV.Alias(refimageDest);

	if ((evisnormalize != evisnormalizeRelative)
			&& (evisnormalize != evisnormalizeAbsolute)
			&& (evisnormalize != evisnormalizeRelMagnitude)
			&& (evisnormalize != evisnormalizeAbsMagnitude))
	{
		fRet = true;
		for (int ibandOrig = 0; ibandOrig < cbandSrc; ++ibandOrig)
		{
			if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + 0,
					(bandDest + ibandOrig) * 4 + 0,
					imageSrcAliasYA, imageDestAliasYA, refrectSrc, pointDest,
					evisnormalize, dblMin, dblMax))
			{
				fRet = false;
				break;
			}
			if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + 1,
					(bandDest + ibandOrig) * 4 + 1,
					imageSrcAliasUV, imageDestAliasUV, refrectSrc, pointDest,
					evisnormalize, dblMin, dblMax))
			{
				fRet = false;
				break;
			}
			if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + 2,
					(bandDest + ibandOrig) * 4 + 2,
					imageSrcAliasUV, imageDestAliasUV, refrectSrc, pointDest,
					evisnormalize, dblMin, dblMax))
			{
				fRet = false;
				break;
			}
			if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4 + 3,
					(bandDest + ibandOrig) * 4 + 3,
					imageSrcAliasYA, imageDestAliasYA, refrectSrc, pointDest,
					evisnormalize, dblMin, dblMax))
			{
				fRet = false;
				break;
			}
		}
	}

	return fRet;
}


template<class TSrc, class TNumDest>
inline bool VisCopyGrayPixelsToRGBAHelper2(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc,
		CVisImage< CVisRGBA<TNumDest> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = true;

	int cbandSrc = ((bandSrc == - 1) ? refimageSrc.NBands() : 1);
	int cbandDest = ((bandDest == - 1) ? refimageDest.NBands() : 1);
	assert(cbandSrc == cbandDest);

	if (bandSrc == - 1)
		bandSrc = 0;
	if (bandDest == - 1)
		bandDest = 0;

	CVisImage<TSrc> imageSrcAlias;
	imageSrcAlias.Alias(refimageSrc);
	CVisImage<TNumDest> imageDestAlias;
	imageDestAlias.Alias(refimageDest);

	// Copy the grayscale values to the R, G, and B components.  (The A
	// component will not be set in this function.)
	// LATER:  We might want to precompute dblMin and dblMax when they'll be
	// needed with the normalization option.
	for (int ibandOrig = 0; ibandOrig < cbandSrc; ++ibandOrig)
	{
		for (int iComponent = 0; iComponent < 3; ++iComponent)
		{
			int ibandSrcCur = ((cbandSrc == 1) ? 0 : (bandSrc + ibandOrig));
			int ibandDestCur = ((cbandDest == 1) ? iComponent
					: (bandDest + ibandOrig) * 4 + iComponent);
			if (!VisCopyStandardPixels(ibandSrcCur, ibandDestCur,
					imageSrcAlias, imageDestAlias, refrectSrc, pointDest,
					evisnormalize, dblMin, dblMax))
			{
				fRet = false;
				break;
			}
		}
	}

	return fRet;
}


template<class TSrc, class TNumDestYA, class TNumDestUV>
inline bool VisCopyGrayPixelsToYUVAHelper2(int bandSrc, int bandDest,
		const CVisImage<TSrc>& refimageSrc,
		CVisImage< CVisYUVA<TNumDestYA, TNumDestUV> >& refimageDest,
		const RECT& refrectSrc, const POINT pointDest, EVisNormalize evisnormalize,
		double dblMin, double dblMax)
{
	bool fRet = true;

	int cbandSrc = ((bandSrc == - 1) ? refimageSrc.NBands() : 1);
	int cbandDest = ((bandDest == - 1) ? refimageDest.NBands() : 1);
	assert(cbandSrc == cbandDest);

	if (bandSrc == - 1)
		bandSrc = 0;
	if (bandDest == - 1)
		bandDest = 0;

	assert(sizeof(TNumDestYA) == sizeof(TNumDestUV));
	CVisImage<TSrc> imageSrcAlias;
	imageSrcAlias.Alias(refimageSrc);
	CVisImage<TNumDestYA> imageDestAlias;
	imageDestAlias.Alias(refimageDest);

	// Copy the grayscale values to the Y component.
	for (int ibandOrig = 0; ibandOrig < cbandSrc; ++ibandOrig)
	{
		// Clear the U, V, and alpha values.
		// UNDONE (in Rel1):  This may not be right if the dest image's rect
		// is larger than the rect from which we're copying.  Also, we might
		// be able to make this more efficient.  (Also, do we need / want
		// to clear or set alhpa?)
		for (int iComponent = 1; iComponent < 4; ++ iComponent)
			imageDestAlias.FillPixels(0, (bandDest + ibandOrig) * 4 + iComponent);

		if (!VisCopyStandardPixels((bandSrc + ibandOrig) * 4,
				(bandDest + ibandOrig) * 4,
				imageSrcAlias, imageDestAlias, refrectSrc, pointDest,
				evisnormalize, dblMin, dblMax))
		{
			fRet = false;
			break;
		}
	}

	return fRet;
}
