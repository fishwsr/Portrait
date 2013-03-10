// VisCopyGrayPixels.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisCopyFnOps.h"


// These helper functions are defined in the CPP file.


VisCoreExport void VISAPI VisCopyGrayCharPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<signed char>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayShortPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<short>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayIntPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<int>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayLongPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<long>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayBytePixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<BYTE>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayUShortPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<unsigned short>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayUIntPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<unsigned int>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayULongPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<unsigned long>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayFloatPixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<float>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);

VisCoreExport void VISAPI VisCopyGrayDoublePixelsAdjustNums(EVisNormalize evisnormalize,
		CVisEnumPixelOf<double>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest);



// This is similar to VisMap2, but the enumerators here can work
// on a subrect of the image and on a single image band.
// LATER:  Should there be a VisMap variation that take src and
// dest RECTs and bands?
template<class TFnOp, class TSrc, class TDest>
inline void VisCopyGrayPixelsHelper(TFnOp& reffnop,
		CVisEnumPixelOf<TSrc>& refenumpixelSrc,
		CVisEnumPixelOf<TDest>& refenumpixelDest)
{
	for (refenumpixelSrc.Reset(), refenumpixelDest.Reset();
			refenumpixelSrc.FMore();
			refenumpixelSrc.Next(), refenumpixelDest.Next())
	{
		assert(refenumpixelDest.FMore());

		reffnop(refenumpixelSrc.PixelCur(), refenumpixelDest.PixelCur());
	}

	assert(!refenumpixelDest.FMore());
}




template<class TSrc>
inline void VisCopyGrayPixelsWithCast(EVisNormalize evisnormalize,
		CVisEnumPixelOf<TSrc>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<TSrc, signed char> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<TSrc, short> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<TSrc, long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<TSrc, unsigned char> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<TSrc, unsigned short> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<TSrc, unsigned long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpCastSrcToDest<TSrc, float> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpCastSrcToDest<TSrc, double> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<double>&) refenumpixelDest);
		}
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}



template<class TSrc, class TDest>
inline void VisScaleGrayPixels(EVisNormalize evisnormalize,
		CVisEnumPixelOf<TSrc>& refenumpixelSrc,
		CVisEnumPixelOf<TDest>& refenumpixelDest,
		double dblMin, double dblMax)
{
	bool fClip = true;
	if (dblMin >= dblMax)
	{
		// Need to find min and max
		CVisFnOpFindMinMax<TSrc> fnop;
		for (refenumpixelSrc.Reset();
				refenumpixelSrc.FMore();
				refenumpixelSrc.Next())
		{
			fnop(refenumpixelSrc.PixelCur());
		}

		dblMax = fnop.DblMax(evisnormalize);
		dblMin = fnop.DblMin(evisnormalize);

		// LATER:  Should fClip be used below?
		fClip = false;
	}

	// Find offset and scale.
	double dblOffset;
	double dblScale;
	if (dblMax == dblMin)
	{
		dblOffset = dblMin;
		dblScale = 0;
	}
	else
	{
		if ((refenumpixelDest.Image().PixFmt() & evispixfmtUnsignedMask)
				== evispixfmtUnsignedMask)
		{
			dblOffset = dblMin;
		}
		else
		{
			dblOffset = (dblMax + dblMin) / 2.0;
		}

		TDest destT;
		dblScale = (((double) VisRangeMax(destT, true))
						- ((double) VisRangeMin(destT, true)))
				/ (dblMax - dblMin);
	}

	// LATER:  Do we want a special case for BYTE src (with LUT)?
	if ((evisnormalize == evisnormalizeRelMagnitude)
			|| (evisnormalize == evisnormalizeAbsMagnitude))
	{
		if (dblOffset == 0.0)
		{
			CVisFnOpScaleSrcMagToDest<TSrc, TDest> fnop(dblScale);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc, refenumpixelDest);
		}
		else
		{
			CVisFnOpOffsetAndScaleSrcMagToDest<TSrc, TDest> fnop(dblOffset, dblScale);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc, refenumpixelDest);
		}
	}
	else
	{
		if (dblOffset == 0.0)
		{
			CVisFnOpScaleSrcToDest<TSrc, TDest> fnop(dblScale);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc, refenumpixelDest);
		}
		else
		{
			CVisFnOpOffsetAndScaleSrcToDest<TSrc, TDest> fnop(dblOffset, dblScale);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc, refenumpixelDest);
		}
	}
}



template<class TSrc>
inline void VisCopyGrayPixelsWithScaling(EVisNormalize evisnormalize,
		CVisEnumPixelOf<TSrc>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest,
		double dblMin, double dblMax)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case evispixfmtFloat:
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

	case evispixfmtDouble:
		{
			VisScaleGrayPixels(evisnormalize, refenumpixelSrc,
					(CVisEnumPixelOf<double>&) refenumpixelDest,
					dblMin, dblMax);
		}
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}
