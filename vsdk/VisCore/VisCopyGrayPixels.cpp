// VisCopyGrayPixels.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"


#include "VisCopyGrayPixels.inl"


VisCoreExport void VISAPI VisCopyGrayPixels(EVisNormalize evisnormalize,
		CVisEnumPixel& refenumpixelSrc, CVisEnumPixel& refenumpixelDest,
		double dblMin, double dblMax)
{
	switch (refenumpixelSrc.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<signed char>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayCharPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<signed char>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<signed char>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<short>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayShortPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<short>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<short>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<long>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayLongPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<long>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<long>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayBytePixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayUShortPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayULongPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case evispixfmtFloat:
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<float>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayFloatPixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<float>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<float>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

	case evispixfmtDouble:
		if (evisnormalize == evisnormalizeCastValues)
		{
			VisCopyGrayPixelsWithCast(evisnormalize,
					(CVisEnumPixelOf<double>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else if (evisnormalize == evisnormalizeAdjustNums)
		{
			VisCopyGrayDoublePixelsAdjustNums(evisnormalize,
					(CVisEnumPixelOf<double>&) refenumpixelSrc,
					refenumpixelDest);
		}
		else
		{
			VisCopyGrayPixelsWithScaling(evisnormalize,
					(CVisEnumPixelOf<double>&) refenumpixelSrc,
					refenumpixelDest, dblMin, dblMax);
		}
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}




VisCoreExport void VISAPI VisCopyGrayCharPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<signed char>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<signed char, signed char> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			static bool fFoundLUT = false;
			static short mpbsLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbsLUT[i] = (short) (i | (i << 8));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, short> fnop(mpbsLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			static bool fFoundLUT = false;
			static long mpblLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpblLUT[i] = (long) (i | (i << 8) | (i << 16) | (i << 24));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, long> fnop(mpblLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDestWithMask<signed char, BYTE> fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<BYTE>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			static bool fFoundLUT = false;
			static unsigned short mpbusLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbusLUT[i] = (unsigned short) (((1 << 15) | (1 << 7)) ^ (i | (i << 8)));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, unsigned short> fnop(mpbusLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			static bool fFoundLUT = false;
			static unsigned long mpbulLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbulLUT[i] = (unsigned long) ((0x80808080) ^ (i | (i << 8) | (i << 16) | (i << 24)));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, unsigned long> fnop(mpbulLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			static bool fFoundLUT = false;
			static float mpbfltLUT[256];
			if (!fFoundLUT)
			{
				double dblScale = 1.0 / 128.0;
				for (int i = 0; i < 128; ++i)
					mpbfltLUT[i] = (float) (dblScale * i);
				for (i = 128; i < 256; ++i)
					mpbfltLUT[i] = (float) (dblScale * (i - 256));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, float> fnop(mpbfltLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			static bool fFoundLUT = false;
			static double mpbdblLUT[256];
			if (!fFoundLUT)
			{
				double dblScale = 1.0 / 128.0;
				for (int i = 0; i < 128; ++i)
					mpbdblLUT[i] = (double) (dblScale * i);
				for (i = 128; i < 256; ++i)
					mpbdblLUT[i] = (double) (dblScale * (i - 256));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, double> fnop(mpbdblLUT);
			VisCopyGrayPixelsHelper(fnop,
					(CVisEnumPixelOf<BYTE>&) refenumpixelSrc,
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


VisCoreExport void VISAPI VisCopyGrayBytePixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<BYTE>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDestWithMask<BYTE, signed char> fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			static bool fFoundLUT = false;
			static short mpbsLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbsLUT[i] = (short) ((1 << 15) ^ (i | (i << 8)));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, short> fnop(mpbsLUT);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			static bool fFoundLUT = false;
			static long mpblLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpblLUT[i] = (long) ((1 << 31) ^ (i | (i << 8) | (i << 16) | (i << 24)));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, long> fnop(mpblLUT);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<BYTE, BYTE> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<BYTE>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			static bool fFoundLUT = false;
			static unsigned short mpbusLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbusLUT[i] = (unsigned short) (i | (i << 8));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, unsigned short> fnop(mpbusLUT);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			static bool fFoundLUT = false;
			static unsigned long mpbulLUT[256];
			if (!fFoundLUT)
			{
				for (int i = 0; i < 256; ++i)
					mpbulLUT[i] = (unsigned long) (i | (i << 8) | (i << 16) | (i << 24));
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, unsigned long> fnop(mpbulLUT);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			static bool fFoundLUT = false;
			static float mpbfltLUT[256];
			if (!fFoundLUT)
			{
				double dblScale = 2.0 / 255.0;
				for (int i = 0; i < 256; ++i)
					mpbfltLUT[i] = (float) ((dblScale * i) - 1.0);
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, float> fnop(mpbfltLUT);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			static bool fFoundLUT = false;
			static double mpbdblLUT[256];
			if (!fFoundLUT)
			{
				double dblScale = 2.0 / 255.0;
				for (int i = 0; i < 256; ++i)
					mpbdblLUT[i] = (double) ((dblScale * i) - 1.0);
				fFoundLUT = true;
			}

			CVisFnOpUseLUTForDest<BYTE, double> fnop(mpbdblLUT);
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


VisCoreExport void VISAPI VisCopyGrayShortPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<short>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDest<short, char, 8> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<short, short> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpShortToLong<short, long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<short, unsigned char, 8> fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDestWithMask<short, unsigned short> fnop(1 << 15);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpShortToLongWithMask<short, unsigned long>
					fnop((1 << 15) | (1 << 31));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpScaleSrcToDest<short, float> fnop(1.0 / (1 << 15));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpScaleSrcToDest<short, double> fnop(1.0 / (1 << 15));
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


VisCoreExport void VISAPI VisCopyGrayUShortPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<unsigned short>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<unsigned short, char, 8>
					fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDestWithMask<unsigned short, short> fnop(1 << 15);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpShortToLongWithMask<unsigned short, long> fnop(1 << 31);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDest<unsigned short, unsigned char, 8> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<unsigned short, unsigned short> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpShortToLong<unsigned short, unsigned long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpOffsetAndScaleSrcToDest<unsigned short, float>
					fnop((double) (1 << 15), 1.0 / (1 << 15));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpOffsetAndScaleSrcToDest<unsigned short, double>
					fnop((double) (1 << 15), 1.0 / (1 << 15));
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


VisCoreExport void VISAPI VisCopyGrayIntPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<int>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	assert(sizeof(long) == sizeof(int));
	VisCopyGrayLongPixelsAdjustNums(evisnorm,
		(CVisEnumPixelOf<long>&) refenumpixelSrc,
		refenumpixelDest);
}


VisCoreExport void VISAPI VisCopyGrayUIntPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<unsigned int>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	assert(sizeof(long) == sizeof(int));
	VisCopyGrayULongPixelsAdjustNums(evisnorm,
		(CVisEnumPixelOf<unsigned long>&) refenumpixelSrc,
		refenumpixelDest);
}


VisCoreExport void VISAPI VisCopyGrayLongPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<long>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDest<long, char, 24> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDest<long, short, 16> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDest<long, long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<long, unsigned char, 24> fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<long, unsigned short, 16> fnop(1 << 15);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDestWithMask<long, unsigned long> fnop(1 << 31);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpScaleSrcToDest<long, float> fnop(1.0 / (1 << 31));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpScaleSrcToDest<long, double> fnop(1.0 / (1 << 31));
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


VisCoreExport void VISAPI VisCopyGrayULongPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<unsigned long>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<unsigned long, char, 24>
					fnop(1 << 7);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpRShiftSrcToDestWithMask<unsigned long, short, 16>
					fnop(1 << 15);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpCastSrcToDestWithMask<unsigned long, long> fnop(1 << 31);
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDest<unsigned long, unsigned char, 24> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpRShiftSrcToDest<unsigned long, unsigned short, 16> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpCastSrcToDest<unsigned long, unsigned long> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpOffsetAndScaleSrcToDest<unsigned long, float>
					fnop((double) (1 << 31), 1.0 / (1 << 31));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpOffsetAndScaleSrcToDest<unsigned long, double>
					fnop((double) (1 << 31), 1.0 / (1 << 31));
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


VisCoreExport void VISAPI VisCopyGrayFloatPixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<float>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<float, signed char>
					fnop((signed char) (1 << 7), (signed char) ((1 << 7) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<float, short>
					fnop((short) (1 << 15), (short) ((1 << 15) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<float, long>
					fnop((long) (1 << 31), (long) (((unsigned long) (1 << 31)) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<float, unsigned char>
					fnop((unsigned char) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<float, unsigned short>
					fnop((unsigned short) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<float, unsigned long>
					fnop((unsigned long) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpAdjustFloatToFloat<float, float> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpAdjustFloatToFloat<float, double> fnop;
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


VisCoreExport void VISAPI VisCopyGrayDoublePixelsAdjustNums(EVisNormalize evisnorm,
		CVisEnumPixelOf<double>& refenumpixelSrc,
		CVisEnumPixel& refenumpixelDest)
{
	switch (refenumpixelDest.Image().PixFmt() &
			(evispixfmtNumericTypeMask | evispixfmtUnsignedMask))
	{
	case (evispixfmtChar | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<double, signed char>
					fnop((signed char) (1 << 7), (signed char) ((1 << 7) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<signed char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<double, short>
					fnop((short) (1 << 15), (short) ((1 << 15) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtSigned):
	case (evispixfmtLong | evispixfmtSigned):
		{
			CVisFnOpAdjustFloatToSignedInt<double, long>
					fnop((long) (1 << 31), (long) (((unsigned long) (1 << 31)) - 1));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<long>&) refenumpixelDest);
		}
		break;

	case (evispixfmtChar | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<double, unsigned char>
					fnop((unsigned char) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned char>&) refenumpixelDest);
		}
		break;

	case (evispixfmtShort | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<double, unsigned short>
					fnop((unsigned short) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned short>&) refenumpixelDest);
		}
		break;

	case (evispixfmtInt | evispixfmtUnsigned):
	case (evispixfmtLong | evispixfmtUnsigned):
		{
			CVisFnOpAdjustFloatToUnsignedInt<double, unsigned long>
					fnop((unsigned long) (~0));
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<unsigned long>&) refenumpixelDest);
		}
		break;

	case evispixfmtFloat:
		{
			CVisFnOpAdjustFloatToFloat<double, float> fnop;
			VisCopyGrayPixelsHelper(fnop, refenumpixelSrc,
					(CVisEnumPixelOf<float>&) refenumpixelDest);
		}
		break;

	case evispixfmtDouble:
		{
			CVisFnOpAdjustFloatToFloat<double, double> fnop;
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
