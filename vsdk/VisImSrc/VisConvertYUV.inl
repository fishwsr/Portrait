// VisConvertYUV.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_YUVTORGB_INL__
#define __VIS_CORE_YUVTORGB_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



inline unsigned long VisInlineGammaCorrect(unsigned long ul);


template<class TPixYUV>
inline void VisInlineYUV422ToYUV(const TPixYUV *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect = false,
		bool fSubsample = false);

inline void VisInlineYUVPlanarToYUV(const void *pvSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);


template<class TPixYUV>
inline void VisInlineYUV422ToRGBA32(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false,
		int evisconvyuv = evisconvyuvDefault);

inline void VisInlineYUV444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault,
		bool fFourBands = false);

inline void VisInlineYUV444UShortToRGBA32(unsigned short *pusSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault,
		bool fFourBands = false);

inline void VisInlineYUVPlanarToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, int evisconvyuv = evisconvyuvDefault,
		int cbppSrc = 16, bool fVBeforeU = false);



template<class TPixYUV>
inline void VisInlineYUV422ToGrayByte(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false);

inline void VisInlineYUV444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVA444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUV444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVA444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVPlanarToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);



template<class TPixYUV>
inline void VisInlineYUV422ToGrayULong(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect = false, bool fSubsample = false);

inline void VisInlineYUV444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVA444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUV444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVA444UShortToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineYUVPlanarToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false,
		int cbppSrc = 16, bool fVBeforeU = false);


inline void VisInlineGrayToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);




// Note:  Integer math is used to multply by decimal numbers in the
// YUV conversion functions below.  To multiply by a decimal number
// d, we multiply BYTE values by ((ULONG) (d * 0x0fffffff / 255.0 + 0.5))
// and shift the result 20 bits to the right, and we multiply USHORT
// values by ((ULONG) (d * 0x000fffff / 255.0 + 0.5)) and shift the
// result 12 bits to the right.


// LATER:  Finish testing all cases in these functions.

// LATER:  Try using a LUT instead of int mults to improve performance
// of YUV conversion.




inline unsigned long VisInlineGammaCorrect(unsigned long ul)
{
	// We assume that indices are gamma-corrected RGB values and we want to
	// find linear RGB values.  For a gamma-corrected value normalized to be
	// in [0, 1], the equations to use to get a linear value y are:
	//    y = x / 4.5, if x < 0.0812
	//    y = ((x + 0.099) / 1.099) ^ 2.2, if x >= 0.0812
	static const unsigned long rgulGammaCorrect[] = 
	{
		0x00,  0x00,  0x00,  0x01,  // 0x00 to 0x03
		0x01,  0x01,  0x01,  0x02,  // 0x04 to 0x07
		0x02,  0x02,  0x02,  0x02,  // 0x08 to 0x0b
		0x03,  0x03,  0x03,  0x03,  // 0x0c to 0x0f
		0x04,  0x04,  0x04,  0x04,  // 0x10 to 0x13
		0x04,  0x05,  0x05,  0x05,  // 0x14 to 0x17
		0x06,  0x06,  0x06,  0x06,  // 0x18 to 0x1b
		0x07,  0x07,  0x07,  0x07,  // 0x1c to 0x1f
		0x08,  0x08,  0x08,  0x09,  // 0x20 to 0x23
		0x09,  0x09,  0x0a,  0x0a,  // 0x24 to 0x27
		0x0a,  0x0b,  0x0b,  0x0b,  // 0x28 to 0x2b
		0x0c,  0x0c,  0x0d,  0x0d,  // 0x2c to 0x2f
		0x0d,  0x0e,  0x0e,  0x0f,  // 0x30 to 0x33
		0x0f,  0x0f,  0x10,  0x10,  // 0x34 to 0x37
		0x11,  0x11,  0x12,  0x12,  // 0x38 to 0x3b
		0x13,  0x13,  0x14,  0x14,  // 0x3c to 0x3f
		0x15,  0x15,  0x16,  0x16,  // 0x40 to 0x43
		0x17,  0x17,  0x18,  0x18,  // 0x44 to 0x47
		0x19,  0x19,  0x1a,  0x1b,  // 0x48 to 0x4b
		0x1b,  0x1c,  0x1c,  0x1d,  // 0x4c to 0x4f
		0x1e,  0x1e,  0x1f,  0x1f,  // 0x50 to 0x53
		0x20,  0x21,  0x21,  0x22,  // 0x54 to 0x57
		0x23,  0x23,  0x24,  0x25,  // 0x58 to 0x5b
		0x25,  0x26,  0x27,  0x28,  // 0x5c to 0x5f
		0x28,  0x29,  0x2a,  0x2b,  // 0x60 to 0x63
		0x2b,  0x2c,  0x2d,  0x2e,  // 0x64 to 0x67
		0x2e,  0x2f,  0x30,  0x31,  // 0x68 to 0x6b
		0x32,  0x33,  0x33,  0x34,  // 0x6c to 0x6f
		0x35,  0x36,  0x37,  0x38,  // 0x70 to 0x73
		0x38,  0x39,  0x3a,  0x3b,  // 0x74 to 0x77
		0x3c,  0x3d,  0x3e,  0x3f,  // 0x78 to 0x7b
		0x40,  0x41,  0x42,  0x43,  // 0x7c to 0x7f
		0x44,  0x45,  0x46,  0x47,  // 0x80 to 0x83
		0x48,  0x49,  0x4a,  0x4b,  // 0x84 to 0x87
		0x4c,  0x4d,  0x4e,  0x4f,  // 0x88 to 0x8b
		0x50,  0x51,  0x52,  0x53,  // 0x8c to 0x8f
		0x54,  0x55,  0x56,  0x57,  // 0x90 to 0x93
		0x59,  0x5a,  0x5b,  0x5c,  // 0x94 to 0x97
		0x5d,  0x5e,  0x5f,  0x61,  // 0x98 to 0x9b
		0x62,  0x63,  0x64,  0x65,  // 0x9c to 0x9f
		0x67,  0x68,  0x69,  0x6a,  // 0xa0 to 0xa3
		0x6c,  0x6d,  0x6e,  0x6f,  // 0xa4 to 0xa7
		0x71,  0x72,  0x73,  0x74,  // 0xa8 to 0xab
		0x76,  0x77,  0x78,  0x7a,  // 0xac to 0xaf
		0x7b,  0x7c,  0x7e,  0x7f,  // 0xb0 to 0xb3
		0x81,  0x82,  0x83,  0x85,  // 0xb4 to 0xb7
		0x86,  0x88,  0x89,  0x8a,  // 0xb8 to 0xbb
		0x8c,  0x8d,  0x8f,  0x90,  // 0xbc to 0xbf
		0x92,  0x93,  0x95,  0x96,  // 0xc0 to 0xc3
		0x98,  0x99,  0x9b,  0x9c,  // 0xc4 to 0xc7
		0x9e,  0x9f,  0xa1,  0xa2,  // 0xc8 to 0xcb
		0xa4,  0xa5,  0xa7,  0xa9,  // 0xcc to 0xcf
		0xaa,  0xac,  0xae,  0xaf,  // 0xd0 to 0xd3
		0xb1,  0xb2,  0xb4,  0xb6,  // 0xd4 to 0xd7
		0xb7,  0xb9,  0xbb,  0xbc,  // 0xd8 to 0xdb
		0xbe,  0xc0,  0xc2,  0xc3,  // 0xdc to 0xdf
		0xc5,  0xc7,  0xc9,  0xca,  // 0xe0 to 0xe3
		0xcc,  0xce,  0xd0,  0xd1,  // 0xe4 to 0xe7
		0xd3,  0xd5,  0xd7,  0xd9,  // 0xe8 to 0xeb
		0xdb,  0xdc,  0xde,  0xe0,  // 0xec to 0xef
		0xe2,  0xe4,  0xe6,  0xe8,  // 0xf0 to 0xf3
		0xe9,  0xeb,  0xed,  0xef,  // 0xf4 to 0xf7
		0xf1,  0xf3,  0xf5,  0xf7,  // 0xf8 to 0xfb
		0xf9,  0xfb,  0xfd,  0xff   // 0xfc to 0xff
	};

	// LATER:  Might want to add additional entries to allow i < 0
	// or i > 255.
	assert((ul >= 0) && (ul < 256));

	return rgulGammaCorrect[ul];
}


// LATER:  We might want to ignore some small coeffcients, like .004 or .002,
// when converting YUV to RGB.  (Add a #define VIS_USE_SMALL_YUV_COEFF.)




template<class TPixYUV>
inline void VisInlineYUV422ToYUV(const TPixYUV *ppixSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, bool fSubsample)
{
	// Each source pixel structure contains two image pixel values.
	assert((dx & 1) == 0);

	int dppixRowSrc = (dx / 2);
	int cpixRowSrc = (dx / 2);
	const TPixYUV *ppixSrcRow = ppixSrc;
	const TPixYUV *ppixSrcLim = ppixSrcRow + cpixRowSrc * dy;

	BYTE *pbDestCur = ((BYTE *) pvDest);

	const int iShift = 20;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dppixRowSrc = - dppixRowSrc;
		ppixSrcRow = ppixSrcLim + dppixRowSrc;
		ppixSrcLim = ppixSrcRow - cpixRowSrc * dy;
	}

	if (fSubsample)
	{
		dppixRowSrc *= 2;
	}

	// Variables used in the loops below.
	const TPixYUV *ppixSrcCur;
	const TPixYUV *ppixSrcRowLim;

	switch (cbitDest)
	{
	case 24:
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bU ^ 0x80);
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bV ^ 0x80);
				++pbDestCur;

				if (!fSubsample)
				{
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bU ^ 0x80);
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bV ^ 0x80);
					++pbDestCur;
				}
			}
		}
		break;

	case 32:
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bU ^ 0x80);
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bV ^ 0x80);
				++pbDestCur;
				*pbDestCur = 0;
				++pbDestCur;

				if (!fSubsample)
				{
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bU ^ 0x80);
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bV ^ 0x80);
					++pbDestCur;
					*pbDestCur = 0;
					++pbDestCur;
				}
			}
		}
		break;

	case 48:
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bU;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bU ^ 0x80);
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bV;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bV ^ 0x80);
				++pbDestCur;

				if (!fSubsample)
				{
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bU;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bU ^ 0x80);
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bV;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bV ^ 0x80);
					++pbDestCur;
				}
			}
		}
		break;

	case 64:
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bY1;
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bU;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bU ^ 0x80);
				++pbDestCur;
				*pbDestCur = ppixSrcCur->bV;
				++pbDestCur;
				*pbDestCur = (ppixSrcCur->bV ^ 0x80);
				++pbDestCur;
				*pbDestCur = 0;
				++pbDestCur;
				*pbDestCur = 0;
				++pbDestCur;

				if (!fSubsample)
				{
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bY1;
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bU;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bU ^ 0x80);
					++pbDestCur;
					*pbDestCur = ppixSrcCur->bV;
					++pbDestCur;
					*pbDestCur = (ppixSrcCur->bV ^ 0x80);
					++pbDestCur;
					*pbDestCur = 0;
					++pbDestCur;
					*pbDestCur = 0;
					++pbDestCur;
				}
			}
		}
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}


inline void VisInlineYUVPlanarToYUV(const void *pvSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect, int cbppSrc,
		bool fVBeforeU)
{
	assert((cbppSrc == 9) || (cbppSrc == 12) || (cbppSrc == 16));
	assert((dx % 2) == 0);
	assert((cbppSrc != 9) || ((dx % 4) == 0));
	assert((cbppSrc == 16) || ((dy % 2) == 0));
	assert((cbppSrc != 9) || ((dy % 4) == 0));

	int cbYRowSrc = dx;
	int dpbYRowSrc = cbYRowSrc;
	const BYTE *pbYSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbYSrcLim = pbYSrcRow + cbYRowSrc * dy;

	int cColPerUV;
	int cRowPerUV;
	if (cbppSrc == 9)
	{
		cColPerUV = 4;
		cRowPerUV = 4;
	}
	else if (cbppSrc == 12)
	{
		cColPerUV = 2;
		cRowPerUV = 2;
	}
	else
	{
		cColPerUV = 2;
		cRowPerUV = 1;
	}

	int cbUVRowSrc = (cbYRowSrc / cColPerUV);
	int dpbUVRowSrc = cbUVRowSrc;
	int cUVRowSrc = (dy / cRowPerUV);
	const BYTE *pbUSrcRow;
	const BYTE *pbVSrcRow;
	if (fVBeforeU)
	{
		pbVSrcRow = pbYSrcLim;
		pbUSrcRow = pbYSrcLim + cbUVRowSrc * cUVRowSrc;
	}
	else
	{
		pbUSrcRow = pbYSrcLim;
		pbVSrcRow = pbYSrcLim + cbUVRowSrc * cUVRowSrc;
	}

	BYTE *pbDestCur = ((BYTE *) pvDest);

	const int iShift = 20;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dpbYRowSrc = - dpbYRowSrc;
		pbYSrcRow = pbYSrcLim + dpbYRowSrc;
		pbYSrcLim = pbYSrcRow - cbYRowSrc * dy;
	
		dpbUVRowSrc = - dpbUVRowSrc;
		pbUSrcRow += (cbUVRowSrc * (cUVRowSrc - 1));
		pbVSrcRow += (cbUVRowSrc * (cUVRowSrc - 1));
	}

	// Variables used in the loops below.
	int cRowUVLeft;
	int cYPerUVRemaining;
	const BYTE *pbYSrcCur;
	const BYTE *pbYSrcRowLim;
	const BYTE *pbUSrcCur;
	const BYTE *pbVSrcCur;

	switch (cbitDest)
	{
	case 24:
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbUSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = (*pbVSrcCur ^ 0x80);
						++ pbDestCur;

						++ pbYSrcCur;
					}

					++ pbUSrcCur;
					++ pbVSrcCur;
				}
			}
		}
		break;

	case 32:
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbUSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = (*pbVSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = 0;
						++ pbDestCur;

						++ pbYSrcCur;
					}

					++ pbUSrcCur;
					++ pbVSrcCur;
				}
			}
		}
		break;

	case 48:
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = *pbUSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbUSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = *pbVSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbVSrcCur ^ 0x80);
						++ pbDestCur;

						++ pbYSrcCur;
					}

					++ pbUSrcCur;
					++ pbVSrcCur;
				}
			}
		}
		break;

	case 64:
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = *pbYSrcCur;
						++ pbDestCur;
						*pbDestCur = *pbUSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbUSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = *pbVSrcCur;
						++ pbDestCur;
						*pbDestCur = (*pbVSrcCur ^ 0x80);
						++ pbDestCur;
						*pbDestCur = 0;
						++ pbDestCur;
						*pbDestCur = 0;
						++ pbDestCur;

						++ pbYSrcCur;
					}

					++ pbUSrcCur;
					++ pbVSrcCur;
				}
			}
		}
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}


template<class TPixYUV>
inline void VisInlineYUV422ToRGBA32(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample, int evisconvyuv)
{
	// Each source pixel structure contains two image pixel values.
	assert((dx & 1) == 0);

	int dppixRowSrc = (dx / 2);
	int cpixRowSrc = (dx / 2);
	const TPixYUV *ppixSrcRow = ppixSrc;
	const TPixYUV *ppixSrcLim = ppixSrcRow + cpixRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 20;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dppixRowSrc = - dppixRowSrc;
		ppixSrcRow = ppixSrcLim + dppixRowSrc;
		ppixSrcLim = ppixSrcRow - cpixRowSrc * dy;
	}

	if (fSubsample)
	{
		dppixRowSrc *= 2;
		pulDestLim = ((unsigned long *) pvDest) + (dx / 2) * (dy / 2);
	}

	// Variables used in the loops below.
	const TPixYUV *ppixSrcCur;
	const TPixYUV *ppixSrcRowLim;
	unsigned long ulU;
	unsigned long ulV;
	unsigned long ulYT;
	unsigned long ulB;
	unsigned long ulG;
	unsigned long ulR;

	switch (evisconvyuv)
	{
	case evisconvyuvBasic:
		// YUVBasic
		// Basic YUV Conversion (0 <= Y <= 255, - 112 <= U <= 112, - 157 <= V <= 157)
		// R = Y + 1.140 V
		// G = Y - 0.394 U - 0.581 V
		// B = Y + 2.032 U
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
// Note:  We ignore the ((ppixSrcCur->bY1) >> 4) term because it is small.
				ulYT = ((ppixSrcCur->bY1) << iShift)
						| ((ppixSrcCur->bY1) << (iShift - 8))
						| ((ppixSrcCur->bY1) << (iShift - 16));

				ulB = ((ulYT + 0x0020a3b6 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00065427 * ulU - 0x0009551c * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x00124fc0 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA

				++ pulDestCur;

				if (!fSubsample)
				{
// Note:  We ignore the ((ppixSrcCur->bY2) >> 4) term because it is small.
					ulYT = ((ppixSrcCur->bY2) << iShift)
							| ((ppixSrcCur->bY2) << (iShift - 8))
							| ((ppixSrcCur->bY2) << (iShift - 16));

					ulB = ((ulYT + 0x0020a3b6 * ulU) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x00065427 * ulU - 0x0009551c * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT + 0x00124fc0 * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuv422:
		// YUV422
		// 4:2:2 YUV Conversion
		// R = Y + 1.402 V
		// G = Y - 0.344 U - 0.714 V
		// B = Y + 1.772 U
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
// Note:  We ignore the ((ppixSrcCur->bY1) >> 4) term because it is small.
				ulYT = ((ppixSrcCur->bY1) << iShift)
						| ((ppixSrcCur->bY1) << (iShift - 8))
						| ((ppixSrcCur->bY1) << (iShift - 16));

				ulB = ((ulYT + 0x001c7693 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x0005868d * ulU - 0x000b7803 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x0016851d * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
// Note:  We ignore the ((ppixSrcCur->bY2) >> 4) term because it is small.
				ulYT = ((ppixSrcCur->bY2) << iShift)
						| ((ppixSrcCur->bY2) << (iShift - 8))
						| ((ppixSrcCur->bY2) << (iShift - 16));

					ulB = ((ulYT + 0x001c7693 * ulU) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x0005868d * ulU - 0x000b7803 * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT + 0x0016851d * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvNTSCNoSat:
		// YUVNTSCNoSat
		// NTSC or PAL (N) YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.969 Y - 0.009 U + 2.244 V
		// G = 1.969 Y - 0.775 U - 1.143 V
		// B = 1.969 Y + 4.006 U - 0.009 V
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = 0x001fa0a7 * (ppixSrcCur->bY1);

				ulB = ((ulYT + 0x004058ec * ulU - 0x00002502 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000c72d9 * ulU - 0x00125c16 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00002502 * ulU + 0x00240b78 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = 0x001fa0a7 * (ppixSrcCur->bY2);

					ulB = ((ulYT + 0x004058ec * ulU - 0x00002502 * ulV) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x000c72d9 * ulU - 0x00125c16 * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x00002502 * ulU + 0x00240b78 * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvNTSCSat:
		// YUVNTSCSat
		// NTSC or PAL (N) YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.691 Y - 0.008 U + 1.927 V + 16
		// G = 1.691 Y - 0.666 U - 0.982 V + 16
		// B = 1.691 Y + 3.440 U - 0.008 V + 16
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = 0x001b2980 * (ppixSrcCur->bY1);

				ulB = ((ulYT + 0x0037417f * ulU - 0x000020e6 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000ab2a2 * ulU - 0x000fc60c * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x000020e6 * ulU + 0x001ef3f2 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = 0x001b2980 * (ppixSrcCur->bY2);

					ulB = ((ulYT + 0x0037417f * ulU - 0x000020e6 * ulV) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x000ab2a2 * ulU - 0x000fc60c * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x000020e6 * ulU + 0x001ef3f2 * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvPALNoSat:
		// YUVPALNoSat
		// PAL YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.862 Y - 0.005 U + 2.121 V
		// G = 1.862 Y - 0.731 U - 1.083 V
		// B = 1.862 Y + 3.788 U - 0.002 V
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = 0x001dcac1 * (ppixSrcCur->bY1);

				ulB = ((ulYT + 0x003cd87e * ulU - 0x00000839 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000bbdeb * ulU - 0x0011655d * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x0000148f * ulU + 0x002211af * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = 0x001dcac1 * (ppixSrcCur->bY2);

					ulB = ((ulYT + 0x003cd87e * ulU - 0x00000839 * ulV) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x000bbdeb * ulU - 0x0011655d * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x0000148f * ulU + 0x002211af * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvPALSat:
		// YUVPALSat
		// PAL YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.599 Y - 0.004 U + 1.822 V + 16
		// G = 1.599 Y - 0.628 U - 0.930 V + 16
		// B = 1.599 Y + 3.253 U - 0.002 V + 16
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = 0x00199581 * (ppixSrcCur->bY1);

				ulB = ((ulYT + 0x0034408a * ulU - 0x00000839 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000a1660 * ulU - 0x000ef038 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00001073 * ulU + 0x001d442e * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = 0x00199581 * (ppixSrcCur->bY2);

					ulB = ((ulYT + 0x0034408a * ulU - 0x00000839 * ulV) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x000a1660 * ulU - 0x000ef038 * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x00001073 * ulU + 0x001d442e * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvGrayscale:
		// YUVGrayscale
		// Grayscale YUV Conversion (using intensity information)
		// R = Y
		// G = Y
		// B = Y
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				*pulDestCur = (ppixSrcCur->bY1)
						 | ((ppixSrcCur->bY1) << 8)
						 | ((ppixSrcCur->bY1) << 16);

				++ pulDestCur;

				if (!fSubsample)
				{
					*pulDestCur = (ppixSrcCur->bY2)
							 | ((ppixSrcCur->bY2) << 8)
							 | ((ppixSrcCur->bY2) << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvUnscaled:
		// YUVUnscaled
		// Unscaled YUV (not in the source above)
		// It has been suggested that the following conversion may be useful with some
		// cameras.  (We can derive the equation from Y = 0.299 R + 0.587 G + 0.114 B,
		// U = B - Y, and V = R - Y.)
		// R = Y + V
		// G = Y - 0.245 U - 0.509 V
		// B = Y + U
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = ppixSrcCur->bY1;

				ulB = ulYT + ulU;
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ulYT - ((0x0003ef75 * ulU + 0x00082d0a * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ulYT + ulV;
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = ppixSrcCur->bY2;

					ulB = ulYT + ulU;
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ulYT - ((0x0003ef75 * ulU + 0x00082d0a * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ulYT + ulV;
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvCbCrNoSat:
		// YCbCrNoSat
		// YCbCr Conversion (nominal range 16 to 235, no thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = Y - 1.371 (Cr - 128)
		// G = Y - 0.336 (Cb - 128) - 0.698 (Cr - 128)
		// B = Y + 1.732 (Cb - 128)
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
// Note:  We ignore the ((ppixSrcCur->bY1) >> 4) term because it is small.
				ulYT = ((ppixSrcCur->bY1) << iShift)
						| ((ppixSrcCur->bY1) << (iShift - 8))
						| ((ppixSrcCur->bY1) << (iShift - 16));

				ulB = ((ulYT + 0x001bd218 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000565a7 * ulU - 0x000b3638 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x001605a3 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
// Note:  We ignore the ((ppixSrcCur->bY2) >> 4) term because it is small.
					ulYT = ((ppixSrcCur->bY2) << iShift)
							| ((ppixSrcCur->bY2) << (iShift - 8))
							| ((ppixSrcCur->bY2) << (iShift - 16));

					ulG = ((ulYT - 0x000565a7 * ulU - 0x000b3638 * ulV) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x000565a7 * ulU - 0x000b3638 * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x001605a3 * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	case evisconvyuvCbCrSat:
		// YCbCrSat
		// YCbCr Conversion (nominal range 0 to 255, thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = 1.164 (Y - 16) - 1.596 (Cr - 128)
		// G = 1.164 (Y - 16) - 0.392 (Cb - 128) - 0.813 (Cr - 128)
		// B = 1.164 (Y - 16) + 2.017 (Cb - 128)
		for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
		{
			ppixSrcCur = ppixSrcRow;
			ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
			
			for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
			{
				ulU = ppixSrcCur->bU - 128;
				ulV = ppixSrcCur->bV - 128;
				ulYT = 0x0012b271 * ((ppixSrcCur->bY1) - 16);

				ulB = ((ulYT + 0x00206608 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00064bee * ulU - 0x000d0f1b * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x0019a2da * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;

				if (!fSubsample)
				{
					ulYT = 0x0012b271 * ((ppixSrcCur->bY2) - 16);

					ulB = ((ulYT + 0x00206608 * ulU) >> iShift);
					if ((ulB & 0xffffff00) != 0)
						ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur = ulB;

					ulG = ((ulYT - 0x00064bee * ulU - 0x000d0f1b * ulV) >> iShift);
					if ((ulG & 0xffffff00) != 0)
						ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulG << 8);

					ulR = ((ulYT - 0x0019a2da * ulV) >> iShift);
					if ((ulR & 0xffffff00) != 0)
						ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
					*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
					++ pulDestCur;
				}
			}
		}
		break;

	default:
		assert(0);
		break;
	}

	assert(pulDestCur == pulDestLim);
}


inline void VisInlineYUV444ByteToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv, bool fFourBands)
{
	// Warning:  This code is written for unsinged U and V values.  It
	// needs to be changed to work correctly with the signed U and V
	// values used in the CVisYUVA class.
	// LATER:  Should this be changed to work with signed U and V values?
	// UNDONE:  Review this file.  In some places, cbRowSrc was being
	// assigned a count of pixels, not a count of bytes.
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpbRowSrc = cbRowSrc;
	const BYTE *pbSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 20;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dpbRowSrc = - dpbRowSrc;
		pbSrcRow = pbSrcLim + dpbRowSrc;
		pbSrcLim = pbSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loops below.
	const BYTE *pbSrcCur;
	const BYTE *pbSrcRowLim;
	unsigned long ulU;
	unsigned long ulV;
	unsigned long ulYT;
	unsigned long ulB;
	unsigned long ulG;
	unsigned long ulR;

	switch (evisconvyuv)
	{
	case evisconvyuvBasic:
		// YUVBasic
		// Basic YUV Conversion (0 <= Y <= 255, - 112 <= U <= 112, - 157 <= V <= 157)
		// R = Y + 1.140 V
		// G = Y - 0.394 U - 0.581 V
		// B = Y + 2.032 U
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
// Note:  We ignore the ((*pbSrcCur) >> 4) term because it is small.
				ulYT = ((*pbSrcCur) << iShift)
						| ((*pbSrcCur) << (iShift - 8))
						| ((*pbSrcCur) << (iShift - 16));
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x0020a3b6 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00065427 * ulU - 0x0009551c * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x00124fc0 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuv422:
		// YUV422
		// 4:2:2 YUV Conversion
		// R = Y + 1.402 V
		// G = Y - 0.344 U - 0.714 V
		// B = Y + 1.772 U
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
// Note:  We ignore the ((*pbSrcCur) >> 4) term because it is small.
				ulYT = ((*pbSrcCur) << iShift)
						| ((*pbSrcCur) << (iShift - 8))
						| ((*pbSrcCur) << (iShift - 16));
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x001c7693 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x0005868d * ulU - 0x000b7803 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x0016851d * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvNTSCNoSat:
		// YUVNTSCNoSat
		// NTSC or PAL (N) YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.969 Y - 0.009 U + 2.244 V
		// G = 1.969 Y - 0.775 U - 1.143 V
		// B = 1.969 Y + 4.006 U - 0.009 V
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = 0x001fa0a7 * (*pbSrcCur);
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x004058ec * ulU - 0x00002502 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000c72d9 * ulU - 0x00125c16 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00002502 * ulU + 0x00240b78 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvNTSCSat:
		// YUVNTSCSat
		// NTSC or PAL (N) YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.691 Y - 0.008 U + 1.927 V + 16
		// G = 1.691 Y - 0.666 U - 0.982 V + 16
		// B = 1.691 Y + 3.440 U - 0.008 V + 16
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = 0x001b2980 * (*pbSrcCur);
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x0037417f * ulU - 0x000020e6 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000ab2a2 * ulU - 0x000fc60c * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x000020e6 * ulU + 0x001ef3f2 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvPALNoSat:
		// YUVPALNoSat
		// PAL YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.862 Y - 0.005 U + 2.121 V
		// G = 1.862 Y - 0.731 U - 1.083 V
		// B = 1.862 Y + 3.788 U - 0.002 V
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = 0x001dcac1 * (*pbSrcCur);
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x003cd87e * ulU - 0x00000839 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000bbdeb * ulU - 0x0011655d * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x0000148f * ulU + 0x002211af * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvPALSat:
		// YUVPALSat
		// PAL YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.599 Y - 0.004 U + 1.822 V + 16
		// G = 1.599 Y - 0.628 U - 0.930 V + 16
		// B = 1.599 Y + 3.253 U - 0.002 V + 16
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = 0x00199581 * (*pbSrcCur);
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x0034408a * ulU - 0x00000839 * ulV) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000a1660 * ulU - 0x000ef038 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00001073 * ulU + 0x001d442e * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvGrayscale:
		// YUVGrayscale
		// Grayscale YUV Conversion (using intensity information)
		// R = Y
		// G = Y
		// B = Y
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				// LATER:  Would a LUT be faster?
				*pulDestCur = (*pbSrcCur)
						| ((*pbSrcCur) << 8)
						| ((*pbSrcCur) << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA

				if (fFourBands)
					pbSrcCur += 4;
				else
					pbSrcCur += 3;

				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvUnscaled:
		// YUVUnscaled
		// Unscaled YUV (not in the source above)
		// It has been suggested that the following conversion may be useful with some
		// cameras.  (We can derive the equation from Y = 0.299 R + 0.587 G + 0.114 B,
		// U = B - Y, and V = R - Y.)
		// R = Y + V
		// G = Y - 0.245 U - 0.509 V
		// B = Y + U
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = *pbSrcCur;
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ulYT + ulU;
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ulYT - ((0x0003ef75 * ulU + 0x00082d0a * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ulYT + ulV;
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA

				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvCbCrNoSat:
		// YCbCrNoSat
		// YCbCr Conversion (nominal range 16 to 235, no thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = Y - 1.371 (Cr - 128)
		// G = Y - 0.336 (Cb - 128) - 0.698 (Cr - 128)
		// B = Y + 1.732 (Cb - 128)
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
// Note:  We ignore the ((*pbSrcCur) >> 4) term because it is small.
				ulYT = ((*pbSrcCur) << iShift)
						| ((*pbSrcCur) << (iShift - 8))
						| ((*pbSrcCur) << (iShift - 16));
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x001bd218 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x000565a7 * ulU - 0x000b3638 * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x001605a3 * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvCbCrSat:
		// YCbCrSat
		// YCbCr Conversion (nominal range 0 to 255, thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = 1.164 (Y - 16) - 1.596 (Cr - 128)
		// G = 1.164 (Y - 16) - 0.392 (Cb - 128) - 0.813 (Cr - 128)
		// B = 1.164 (Y - 16) + 2.017 (Cb - 128)
		for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
		{
			pbSrcCur = pbSrcRow;
			pbSrcRowLim = pbSrcCur + cbRowSrc;
			
			while (pbSrcCur != pbSrcRowLim)
			{
				ulYT = 0x0012a0 * ((*pbSrcCur) - 16);
				++ pbSrcCur;
				ulU = *pbSrcCur - 128;
				++ pbSrcCur;
				ulV = *pbSrcCur - 128;
				++ pbSrcCur;

				if (fFourBands)
					++ pbSrcCur;

				ulB = ((ulYT + 0x00206608 * ulU) >> iShift);
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00064bee * ulU - 0x000d0f1b * ulV) >> iShift);
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x0019a2da * ulV) >> iShift);
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	default:
		assert(0);
		break;
	}

	assert(pulDestCur == pulDestLim);
}


inline void VisInlineYUV444UShortToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv, bool fFourBands)
{
	// Warning:  This code is written for unsinged U and V values.  It
	// needs to be changed to work correctly with the signed U and V
	// values used in the CVisYUVA class.
	// LATER:  Should this be changed to work with signed U and V values?
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpusRowSrc = cbRowSrc;
	const unsigned short *pusSrcRow = ((const unsigned short *) pvSrc);
	const unsigned short *pusSrcLim = pusSrcRow + cbRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 12;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dpusRowSrc = - dpusRowSrc;
		pusSrcRow = pusSrcLim + dpusRowSrc;
		pusSrcLim = pusSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loops below.
	const unsigned short *pusSrcCur;
	const unsigned short *pusSrcRowLim;
	unsigned long ulU;
	unsigned long ulV;
	unsigned long ulYT;
	unsigned long ulB;
	unsigned long ulG;
	unsigned long ulR;

	switch (evisconvyuv)
	{
	case evisconvyuvBasic:
		// YUVBasic
		// Basic YUV Conversion (0 <= Y <= 255, - 112 <= U <= 112, - 157 <= V <= 157)
		// R = Y + 1.140 V
		// G = Y - 0.394 U - 0.581 V
		// B = Y + 2.032 U
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
// Note:  We ignore the ((*pusSrcCur) >> 4) term because it is small.
				ulYT = ((*pusSrcCur) << iShift)
						| ((*pusSrcCur) << (iShift - 8));
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x000020a4 * ulU) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000654 * ulU - 0x00000955 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x00001250 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuv422:
		// YUV422
		// 4:2:2 YUV Conversion
		// R = Y + 1.402 V
		// G = Y - 0.344 U - 0.714 V
		// B = Y + 1.772 U
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
// Note:  We ignore the ((*pusSrcCur) >> 4) term because it is small.
				ulYT = ((*pusSrcCur) << iShift)
						| ((*pusSrcCur) << (iShift - 8));
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00001c77 * ulU) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000587 * ulU - 0x00000b78 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT + 0x00001685 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvNTSCNoSat:
		// YUVNTSCNoSat
		// NTSC or PAL (N) YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.969 Y - 0.009 U + 2.244 V
		// G = 1.969 Y - 0.775 U - 1.143 V
		// B = 1.969 Y + 4.006 U - 0.009 V
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = 0x001fa1 * (*pusSrcCur);
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00004059 * ulU - 0x00000025 * ulV) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000c73 * ulU - 0x0000125c * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00000025 * ulU + 0x0000240b * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvNTSCSat:
		// YUVNTSCSat
		// NTSC or PAL (N) YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.691 Y - 0.008 U + 1.927 V + 16
		// G = 1.691 Y - 0.666 U - 0.982 V + 16
		// B = 1.691 Y + 3.440 U - 0.008 V + 16
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = 0x001b2a * (*pusSrcCur);
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00003741 * ulU - 0x00000021 * ulV) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000ab3 * ulU - 0x00000fc6 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00000021 * ulU + 0x00001ef4 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvPALNoSat:
		// YUVPALNoSat
		// PAL YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.862 Y - 0.005 U + 2.121 V
		// G = 1.862 Y - 0.731 U - 1.083 V
		// B = 1.862 Y + 3.788 U - 0.002 V
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = 0x001de9 * (*pusSrcCur);
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00003cd8 * ulU - 0x00000008 * ulV) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000bbe * ulU - 0x00001165 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00000015 * ulU + 0x00002212 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvPALSat:
		// YUVPALSat
		// PAL YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.599 Y - 0.004 U + 1.822 V + 16
		// G = 1.599 Y - 0.628 U - 0.930 V + 16
		// B = 1.599 Y + 3.253 U - 0.002 V + 16
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = 0x0019af * (*pusSrcCur);
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00003441 * ulU - 0x00000008 * ulV) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000a16 * ulU - 0x00000ef0 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00000010 * ulU + 0x00001d44 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvGrayscale:
		// YUVGrayscale
		// Grayscale YUV Conversion (using intensity information)
		// R = Y
		// G = Y
		// B = Y
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				// LATER:  Would a LUT be faster?
				// LATER:  Do we want to round or truncate here?
				*pulDestCur = (*pusSrcCur >> 8)
						| (*pusSrcCur & 0xff00)
						| ((*pusSrcCur & 0xff00) << 8);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				if (fFourBands)
					pusSrcCur += 4;
				else
					pusSrcCur += 3;

				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvUnscaled:
		// YUVUnscaled
		// Unscaled YUV (not in the source above)
		// It has been suggested that the following conversion may be useful with some
		// cameras.  (We can derive the equation from Y = 0.299 R + 0.587 G + 0.114 B,
		// U = B - Y, and V = R - Y.)
		// R = Y + V
		// G = Y - 0.245 U - 0.509 V
		// B = Y + U
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = *pusSrcCur;
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ulYT + ulU;
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ulYT - ((0x000003ef * ulU + 0x0000082d * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ulYT + ulV;
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvCbCrNoSat:
		// YCbCrNoSat
		// YCbCr Conversion (nominal range 16 to 235, no thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = Y - 1.371 (Cr - 128)
		// G = Y - 0.336 (Cb - 128) - 0.698 (Cr - 128)
		// B = Y + 1.732 (Cb - 128)
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
// Note:  We ignore the ((*pusSrcCur) >> 4) term because it is small.
				ulYT = ((*pusSrcCur) << iShift)
						| ((*pusSrcCur) << (iShift - 8));
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00001bd2 * ulU) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x00000566 * ulU - 0x00000b36 * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x00001606 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvCbCrSat:
		// YCbCrSat
		// YCbCr Conversion (nominal range 0 to 255, thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = 1.164 (Y - 16) - 1.596 (Cr - 128)
		// G = 1.164 (Y - 16) - 0.392 (Cb - 128) - 0.813 (Cr - 128)
		// B = 1.164 (Y - 16) + 2.017 (Cb - 128)
		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			pusSrcCur = pusSrcRow;
			pusSrcRowLim = pusSrcCur + cbRowSrc;
			
			while (pusSrcCur != pusSrcRowLim)
			{
				ulYT = 0x000012b2 * ((*pusSrcCur) - 16);
				++ pusSrcCur;
				ulU = *pusSrcCur - 128;
				++ pusSrcCur;
				ulV = *pusSrcCur - 128;
				++ pusSrcCur;

				if (fFourBands)
					++ pusSrcCur;

				ulB = ((ulYT + 0x00002066 * ulU) >> (iShift + 8));
				if ((ulB & 0xffffff00) != 0)
					ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur = ulB;

				ulG = ((ulYT - 0x0000064c * ulU - 0x00000d0f * ulV) >> (iShift + 8));
				if ((ulG & 0xffffff00) != 0)
					ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulG << 8);

				ulR = ((ulYT - 0x000019a3 * ulV) >> (iShift + 8));
				if ((ulR & 0xffffff00) != 0)
					ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
				*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pulDestCur;
			}
		}
		break;

	default:
		assert(0);
		break;
	}

	assert(pulDestCur == pulDestLim);
}


inline void VisInlineYUVPlanarToRGBA32(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int evisconvyuv, int cbppSrc,
		bool fVBeforeU)
{
	assert((cbppSrc == 9) || (cbppSrc == 12) || (cbppSrc == 16));
	assert((dx % 2) == 0);
	assert((cbppSrc != 9) || ((dx % 4) == 0));
	assert((cbppSrc == 16) || ((dy % 2) == 0));
	assert((cbppSrc != 9) || ((dy % 4) == 0));

	int cbYRowSrc = dx;
	int dpbYRowSrc = cbYRowSrc;
	const BYTE *pbYSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbYSrcLim = pbYSrcRow + cbYRowSrc * dy;

	int cColPerUV;
	int cRowPerUV;
	if (cbppSrc == 9)
	{
		cColPerUV = 4;
		cRowPerUV = 4;
	}
	else if (cbppSrc == 12)
	{
		cColPerUV = 2;
		cRowPerUV = 2;
	}
	else
	{
		cColPerUV = 2;
		cRowPerUV = 1;
	}

	int cbUVRowSrc = (cbYRowSrc / cColPerUV);
	int dpbUVRowSrc = cbUVRowSrc;
	int cUVRowSrc = (dy / cRowPerUV);
	const BYTE *pbUSrcRow;
	const BYTE *pbVSrcRow;
	if (fVBeforeU)
	{
		pbVSrcRow = pbYSrcLim;
		pbUSrcRow = pbYSrcLim + cbUVRowSrc * cUVRowSrc;
	}
	else
	{
		pbUSrcRow = pbYSrcLim;
		pbVSrcRow = pbYSrcLim + cbUVRowSrc * cUVRowSrc;
	}

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 20;
	const unsigned long ulRound = (1 << (iShift - 1));
	const unsigned long ulHigh = ((1 << 8) - 1);
	const unsigned long ulSign = (1 << (31 - iShift));

	if (fReflect)
	{
		dpbYRowSrc = - dpbYRowSrc;
		pbYSrcRow = pbYSrcLim + dpbYRowSrc;
		pbYSrcLim = pbYSrcRow - cbYRowSrc * dy;
	
		dpbUVRowSrc = - dpbUVRowSrc;
		pbUSrcRow += (cbUVRowSrc * (cUVRowSrc - 1));
		pbVSrcRow += (cbUVRowSrc * (cUVRowSrc - 1));
	}

	// Variables used in the loops below.
	int cRowUVLeft;
	int cYPerUVRemaining;
	const BYTE *pbYSrcCur;
	const BYTE *pbYSrcRowLim;
	const BYTE *pbUSrcCur;
	const BYTE *pbVSrcCur;
	unsigned long ulU;
	unsigned long ulV;
	unsigned long ulYT;
	unsigned long ulB;
	unsigned long ulG;
	unsigned long ulR;

	switch (evisconvyuv)
	{
	case evisconvyuvBasic:
		// YUVBasic
		// Basic YUV Conversion (0 <= Y <= 255, - 112 <= U <= 112, - 157 <= V <= 157)
		// R = Y + 1.140 V
		// G = Y - 0.394 U - 0.581 V
		// B = Y + 2.032 U
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
// Note:  We ignore the ((*pbYSrcCur) >> 4) term because it is small.
						ulYT = ((*pbYSrcCur) << iShift)
								| ((*pbYSrcCur) << (iShift - 8))
								| ((*pbYSrcCur) << (iShift - 16));
						++ pbYSrcCur;

						ulB = ((ulYT + 0x0020a3b6 * ulU) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x00065427 * ulU - 0x0009551c * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT + 0x00124fc0 * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuv422:
		// YUV422
		// 4:2:2 YUV Conversion
		// R = Y + 1.402 V
		// G = Y - 0.344 U - 0.714 V
		// B = Y + 1.772 U
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
// Note:  We ignore the ((*pbYSrcCur) >> 4) term because it is small.
						ulYT = ((*pbYSrcCur) << iShift)
								| ((*pbYSrcCur) << (iShift - 8))
								| ((*pbYSrcCur) << (iShift - 16));
						++ pbYSrcCur;

						ulB = ((ulYT + 0x001c7693 * ulU) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x0005868d * ulU - 0x000b7803 * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT + 0x0016851d * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvNTSCNoSat:
		// YUVNTSCNoSat
		// NTSC or PAL (N) YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.969 Y - 0.009 U + 2.244 V
		// G = 1.969 Y - 0.775 U - 1.143 V
		// B = 1.969 Y + 4.006 U - 0.009 V
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = 0x001fa0a7 * (*pbYSrcCur);
						++ pbYSrcCur;

						ulB = ((ulYT + 0x004058ec * ulU - 0x00002502 * ulV) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x000c72d9 * ulU - 0x00125c16 * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x00002502 * ulU + 0x00240b78 * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvNTSCSat:
		// YUVNTSCSat
		// NTSC or PAL (N) YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 130)
		// R = 1.691 Y - 0.008 U + 1.927 V + 16
		// G = 1.691 Y - 0.666 U - 0.982 V + 16
		// B = 1.691 Y + 3.440 U - 0.008 V + 16
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = 0x001b2980 * (*pbYSrcCur);
						++ pbYSrcCur;

						ulB = ((ulYT + 0x0037417f * ulU - 0x000020e6 * ulV) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x000ab2a2 * ulU - 0x000fc60c * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x000020e6 * ulU + 0x001ef3f2 * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvPALNoSat:
		// YUVPALNoSat
		// PAL YUV Conversion (nominal range 16 to 235, no thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.862 Y - 0.005 U + 2.121 V
		// G = 1.862 Y - 0.731 U - 1.083 V
		// B = 1.862 Y + 3.788 U - 0.002 V
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = 0x001dcac1 * (*pbYSrcCur);
						++ pbYSrcCur;

						ulB = ((ulYT + 0x003cd87e * ulU - 0x00000839 * ulV) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x000bbdeb * ulU - 0x0011655d * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x0000148f * ulU + 0x002211af * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvPALSat:
		// YUVPALSat
		// PAL YUV Conversion (nominal range 0 to 255, thresholding needed)
		// (0 <= Y <= 137)
		// R = 1.599 Y - 0.004 U + 1.822 V + 16
		// G = 1.599 Y - 0.628 U - 0.930 V + 16
		// B = 1.599 Y + 3.253 U - 0.002 V + 16
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = 0x00199581 * (*pbYSrcCur);
						++ pbYSrcCur;

						ulB = ((ulYT + 0x0034408a * ulU - 0x00000839 * ulV) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x000a1660 * ulU - 0x000ef038 * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x00001073 * ulU + 0x001d442e * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvGrayscale:
		// YUVGrayscale
		// Grayscale YUV Conversion (using intensity information)
		// R = Y
		// G = Y
		// B = Y
		for ( ; pbYSrcRow != pbYSrcLim; pbYSrcRow += dpbYRowSrc)
		{
			pbYSrcCur = pbYSrcRow;
			pbYSrcRowLim = pbYSrcCur + cbYRowSrc;
			
			while (pbYSrcCur != pbYSrcRowLim)
			{
				*pulDestCur = (*pbYSrcCur)
						| ((*pbYSrcCur) << 8)
						| ((*pbYSrcCur) << 16);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
				++ pbYSrcCur;
				++ pulDestCur;
			}
		}
		break;

	case evisconvyuvUnscaled:
		// YUVUnscaled
		// Unscaled YUV (not in the source above)
		// It has been suggested that the following conversion may be useful with some
		// cameras.  (We can derive the equation from Y = 0.299 R + 0.587 G + 0.114 B,
		// U = B - Y, and V = R - Y.)
		// R = Y + V
		// G = Y - 0.245 U - 0.509 V
		// B = Y + U
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = *pbYSrcCur;
						++ pbYSrcCur;

						ulB = ulYT + ulU;
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ulYT - ((0x0003ef75 * ulU + 0x00082d0a * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ulYT + ulV;
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvCbCrNoSat:
		// YCbCrNoSat
		// YCbCr Conversion (nominal range 16 to 235, no thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = Y - 1.371 (Cr - 128)
		// G = Y - 0.336 (Cb - 128) - 0.698 (Cr - 128)
		// B = Y + 1.732 (Cb - 128)
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
// Note:  We ignore the ((*pbYSrcCur) >> 4) term because it is small.
						ulYT = ((*pbYSrcCur) << iShift)
								| ((*pbYSrcCur) << (iShift - 8))
								| ((*pbYSrcCur) << (iShift - 16));
						++ pbYSrcCur;

						ulB = ((ulYT + 0x001bd218 * ulU) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x000565a7 * ulU - 0x000b3638 * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x001605a3 * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	case evisconvyuvCbCrSat:
		// YCbCrSat
		// YCbCr Conversion (nominal range 0 to 255, thresholding needed)
		// (Nominal input ranges:  16 <= Y <= 235, 16 <= Cb <= 235, 16 <= Cr <= 235)
		// R = 1.164 (Y - 16) - 1.596 (Cr - 128)
		// G = 1.164 (Y - 16) - 0.392 (Cb - 128) - 0.813 (Cr - 128)
		// B = 1.164 (Y - 16) + 2.017 (Cb - 128)
		for ( ; pbYSrcRow != pbYSrcLim;
				pbUSrcRow += dpbUVRowSrc, pbVSrcRow += dpbUVRowSrc)
		{
			for (cRowUVLeft = cRowPerUV;
					cRowUVLeft  != 0;
					-- cRowUVLeft, pbYSrcRow += dpbYRowSrc)
			{
				pbYSrcCur = pbYSrcRow;
				pbYSrcRowLim = pbYSrcCur + cbYRowSrc;

				pbUSrcCur = pbUSrcRow;
				pbVSrcCur = pbVSrcRow;
				
				while (pbYSrcCur != pbYSrcRowLim)
				{
					ulU = *pbUSrcCur - 128;
					++ pbUSrcCur;
					ulV = *pbVSrcCur - 128;
					++ pbVSrcCur;

					for (cYPerUVRemaining = cColPerUV;
							cYPerUVRemaining != 0;
							-- cYPerUVRemaining)
					{
						ulYT = ((*pbYSrcCur) << iShift);
						++ pbYSrcCur;

						ulB = ((ulYT + 0x00206608 * ulU) >> iShift);
						if ((ulB & 0xffffff00) != 0)
							ulB = (((ulB & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur = ulB;

						ulG = ((ulYT - 0x00064bee * ulU - 0x000d0f1b * ulV) >> iShift);
						if ((ulG & 0xffffff00) != 0)
							ulG = (((ulG & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulG << 8);

						ulR = ((ulYT - 0x0019a2da * ulV) >> iShift);
						if ((ulR & 0xffffff00) != 0)
							ulR = (((ulR & ulSign) != 0) ? 0 : ulHigh);
						*pulDestCur |= (ulR << 16);

#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++ pulDestCur;
					}
				}
			}
		}
		break;

	default:
		assert(0);
		break;
	}

	assert(pulDestCur == pulDestLim);
}



template<class TPixYUV>
inline void VisInlineYUV422ToGrayByte(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample)
{
	// Each source pixel structure contains two image pixel values.
	assert((dx & 1) == 0);

	int dppixRowSrc = (dx / 2);
	int cpixRowSrc = (dx / 2);
	const TPixYUV *ppixSrcRow = ppixSrc;
	const TPixYUV *ppixSrcLim = ppixSrcRow + cpixRowSrc * dy;

	BYTE *pbDestCur = ((BYTE *) pvDest);
	BYTE *pbDestLim = ((BYTE *) pvDest) + dx * dy;

	if (fReflect)
	{
		dppixRowSrc = - dppixRowSrc;
		ppixSrcRow = ppixSrcLim + dppixRowSrc;
		ppixSrcLim = ppixSrcRow - cpixRowSrc * dy;
	}

	if (fSubsample)
	{
		dppixRowSrc *= 2;
		pbDestLim = ((BYTE *) pvDest) + (dx / 2) * (dy / 2);
	}

	// Variables used in the loop below.
	const TPixYUV *ppixSrcCur;
	const TPixYUV *ppixSrcRowLim;

	for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
	{
		ppixSrcCur = ppixSrcRow;
		ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
		
		for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
		{
			*pbDestCur = (ppixSrcCur->bY1);
			++ pbDestCur;

			if (!fSubsample)
			{
				*pbDestCur = (ppixSrcCur->bY2);
				++ pbDestCur;
			}
		}
	}
}

inline void VisInlineYUV444ByteToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fFourBands)
{
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpbRowSrc = cbRowSrc;
	const BYTE *pbSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * dy;

	BYTE *pbDestCur = ((BYTE *) pvDest);
	BYTE *pbDestLim = ((BYTE *) pvDest) + dx * dy;

	if (fReflect)
	{
		dpbRowSrc = - dpbRowSrc;
		pbSrcRow = pbSrcLim + dpbRowSrc;
		pbSrcLim = pbSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loop below.
	const BYTE *pbSrcCur;
	const BYTE *pbSrcRowLim;

	for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
	{
		pbSrcCur = pbSrcRow;
		pbSrcRowLim = pbSrcCur + cbRowSrc;
		
		while (pbSrcCur != pbSrcRowLim)
		{
			*pbDestCur = *pbSrcCur;
			++ pbDestCur;

			if (fFourBands)
				pbSrcCur += 4;
			else
				pbSrcCur += 3;
		}
	}
}

inline void VisInlineYUV444UShortToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fFourBands)
{
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpusRowSrc = cbRowSrc;
	const unsigned short *pusSrcRow = ((const unsigned short *) pvSrc);
	const unsigned short *pusSrcLim = pusSrcRow + cbRowSrc * dy;

	BYTE *pbDestCur = ((BYTE *) pvDest);
	BYTE *pbDestLim = ((BYTE *) pvDest) + dx * dy;

	if (fReflect)
	{
		dpusRowSrc = - dpusRowSrc;
		pusSrcRow = pusSrcLim + dpusRowSrc;
		pusSrcLim = pusSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loop below.
	const unsigned short *pusSrcCur;
	const unsigned short *pusSrcRowLim;

	for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
	{
		pusSrcCur = pusSrcRow;
		pusSrcRowLim = pusSrcCur + cbRowSrc;
		
		while (pusSrcCur != pusSrcRowLim)
		{
			// LATER:  Would a LUT be faster?
			// LATER:  Do we want to round or truncate here?
			*pbDestCur = (BYTE) (*pusSrcCur >> 8);

			if (fFourBands)
				pusSrcCur += 4;
			else
				pusSrcCur += 3;

			++ pbDestCur;
		}
	}
}

inline void VisInlineYUVPlanarToGrayByte(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int cbppSrc, bool fVBeforeU)
{
	// LATER:  The fVBeforeU parameter really isn't needed in this method.
	VisUnreferenced(fVBeforeU);

	assert((cbppSrc == 9) || (cbppSrc == 12) || (cbppSrc == 16));
	assert((dx % 2) == 0);
	assert((cbppSrc != 9) || ((dx % 4) == 0));
	assert((cbppSrc == 16) || ((dy % 2) == 0));
	assert((cbppSrc != 9) || ((dy % 4) == 0));

	int cbYRowSrc = dx;
	int dpbYRowSrc = cbYRowSrc;
	const BYTE *pbYSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbYSrcLim = pbYSrcRow + cbYRowSrc * dy;

	BYTE *pbDestCur = ((BYTE *) pvDest);
	BYTE *pbDestLim = ((BYTE *) pvDest) + dx * dy;

	if (fReflect)
	{
		dpbYRowSrc = - dpbYRowSrc;
		pbYSrcRow = pbYSrcLim + dpbYRowSrc;
		pbYSrcLim = pbYSrcRow - cbYRowSrc * dy;
	}

	for ( ; pbYSrcRow != pbYSrcLim;
			pbYSrcRow += dpbYRowSrc, pbDestCur += cbYRowSrc)
	{
		CopyMemory(pbDestCur, pbYSrcRow, cbYRowSrc);
	}
}



template<class TPixYUV>
inline void VisInlineYUV422ToGrayULong(const TPixYUV *ppixSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fSubsample)
{
	// Each source pixel structure contains two image pixel values.
	assert((dx & 1) == 0);

	int dppixRowSrc = (dx / 2);
	int cpixRowSrc = (dx / 2);
	const TPixYUV *ppixSrcRow = ppixSrc;
	const TPixYUV *ppixSrcLim = ppixSrcRow + cpixRowSrc * dy;

	const int iShift = 24;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	if (fReflect)
	{
		dppixRowSrc = - dppixRowSrc;
		ppixSrcRow = ppixSrcLim + dppixRowSrc;
		ppixSrcLim = ppixSrcRow - cpixRowSrc * dy;
	}

	if (fSubsample)
	{
		dppixRowSrc *= 2;
		pulDestLim = ((unsigned long *) pvDest) + (dx / 2) * (dy / 2);
	}

	// Variables used in the loop below.
	const TPixYUV *ppixSrcCur;
	const TPixYUV *ppixSrcRowLim;

	for ( ; ppixSrcRow != ppixSrcLim; ppixSrcRow += dppixRowSrc)
	{
		ppixSrcCur = ppixSrcRow;
		ppixSrcRowLim = ppixSrcCur + cpixRowSrc;
		
		for ( ; ppixSrcCur != ppixSrcRowLim; ++ppixSrcCur)
		{
			*pulDestCur = ((ppixSrcCur->bY1) << iShift);
			++ pulDestCur;

			if (!fSubsample)
			{
				*pulDestCur = ((ppixSrcCur->bY2) << iShift);
				++ pulDestCur;
			}
		}
	}
}

inline void VisInlineYUV444ByteToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, bool fFourBands)
{
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpbRowSrc = cbRowSrc;
	const BYTE *pbSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 24;

	if (fReflect)
	{
		dpbRowSrc = - dpbRowSrc;
		pbSrcRow = pbSrcLim + dpbRowSrc;
		pbSrcLim = pbSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loop below.
	const BYTE *pbSrcCur;
	const BYTE *pbSrcRowLim;

	for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
	{
		pbSrcCur = pbSrcRow;
		pbSrcRowLim = pbSrcCur + cbRowSrc;
		
		while (pbSrcCur != pbSrcRowLim)
		{
			*pulDestCur = (*pbSrcCur << iShift);
			++ pulDestCur;

			if (fFourBands)
				pbSrcCur += 4;
			else
				pbSrcCur += 3;
		}
	}
}

inline void VisInlineYUV444UShortToGrayULong(const void *pvSrc,
		void *pvDest, int dx, int dy, bool fReflect, bool fFourBands)
{
	int cbRowSrc = dx * (fFourBands ? 4 : 3);
	int dpusRowSrc = cbRowSrc;
	const unsigned short *pusSrcRow = ((const unsigned short *) pvSrc);
	const unsigned short *pusSrcLim = pusSrcRow + cbRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 16;

	if (fReflect)
	{
		dpusRowSrc = - dpusRowSrc;
		pusSrcRow = pusSrcLim + dpusRowSrc;
		pusSrcLim = pusSrcRow - cbRowSrc * dy;
	}

	// Variables used in the loop below.
	const unsigned short *pusSrcCur;
	const unsigned short *pusSrcRowLim;

	for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
	{
		pusSrcCur = pusSrcRow;
		pusSrcRowLim = pusSrcCur + cbRowSrc;
		
		while (pusSrcCur != pusSrcRowLim)
		{
			*pulDestCur = (*pusSrcCur << iShift);
			++ pulDestCur;

			if (fFourBands)
				pusSrcCur += 4;
			else
				pusSrcCur += 3;
		}
	}
}

inline void VisInlineYUVPlanarToGrayULong(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect, int cbppSrc, bool fVBeforeU)
{
	// LATER:  The fVBeforeU parameter really isn't needed in this method.
	VisUnreferenced(fVBeforeU);

	assert((cbppSrc == 9) || (cbppSrc == 12) || (cbppSrc == 16));
	assert((dx % 2) == 0);
	assert((cbppSrc != 9) || ((dx % 4) == 0));
	assert((cbppSrc == 16) || ((dy % 2) == 0));
	assert((cbppSrc != 9) || ((dy % 4) == 0));

	int cbYRowSrc = dx;
	int dpbYRowSrc = cbYRowSrc;
	const BYTE *pbYSrcRow = ((const BYTE *) pvSrc);
	const BYTE *pbYSrcLim = pbYSrcRow + cbYRowSrc * dy;

	unsigned long *pulDestCur = ((unsigned long *) pvDest);
	unsigned long *pulDestLim = ((unsigned long *) pvDest) + dx * dy;

	const int iShift = 24;

	if (fReflect)
	{
		dpbYRowSrc = - dpbYRowSrc;
		pbYSrcRow = pbYSrcLim + dpbYRowSrc;
		pbYSrcLim = pbYSrcRow - cbYRowSrc * dy;
	}

	// Variables used in the loop below.
	const BYTE *pbYSrcCur;
	const BYTE *pbYSrcRowLim;

	for ( ; pbYSrcRow != pbYSrcLim; pbYSrcRow += dpbYRowSrc)
	{
		pbYSrcCur = pbYSrcRow;
		pbYSrcRowLim = pbYSrcCur + cbYRowSrc;
		
		while (pbYSrcCur != pbYSrcRowLim)
		{
			*pulDestCur = (*pbYSrcCur << iShift);
			++ pulDestCur;
			++ pbYSrcCur;
		}
	}
}


// Supported conversions:  {1, 2, 4, 8, 16}-bit gray to {32, 48, 64}-bit YUV.
inline void VisInlineGrayToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	switch (cbitSrc)
	{
	case 1:
		switch (cbitDest)
		{
		case 32:
			{
				assert ((dx % 8) == 0);

				int cbRowSrc = dx / 8;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur = 0x000000ff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 48:
			{
				assert ((dx % 8) == 0);

				int cbRowSrc = dx / 8;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur = 0xffff0000;
						else
							*pulDestCur = 0;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur = 0xffff0000;
						else
							*pulDestCur = 0;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur = 0xffff0000;
						else
							*pulDestCur = 0;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur = 0xffff0000;
						else
							*pulDestCur = 0;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 64:
			{
				assert ((dx % 8) == 0);

				int cbRowSrc = dx / 8;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur = 0x0000ffff;
						else
							*pulDestCur = 0;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;
	
#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
		break;
	
	case 2:
		switch (cbitDest)
		{
		case 32:
			{
				assert ((dx % 4) == 0);

				int cbRowSrc = dx / 4;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00000055;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x000000aa;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x00000055;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x000000aa;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00000055;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur |= 0x000000aa;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x00000055;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur |= 0x000000aa;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 48:
			{
				assert ((dx % 4) == 0);

				int cbRowSrc = dx / 4;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x55550000;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0xaaaa0000;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x55550000;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur |= 0xaaaa0000;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 64:
			{
				assert ((dx % 4) == 0);

				int cbRowSrc = dx / 4;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x00005555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur |= 0x0000aaaa;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
		break;
	
	case 4:
		switch (cbitDest)
		{
		case 32:
			{
				assert ((dx % 2) == 0);

				int cbRowSrc = dx / 2;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00000011;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00000022;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00000044;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00000088;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00000011;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00000022;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00000044;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00000088;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 48:
			{
				assert ((dx % 2) == 0);

				int cbRowSrc = dx / 2;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00001111;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00002222;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00004444;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00008888;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x11110000;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x22220000;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x44440000;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x88880000;
						++pulDestCur;

						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 64:
			{
				assert ((dx % 2) == 0);

				int cbRowSrc = dx / 2;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				unsigned long *pulDestCur = (unsigned long *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						if (((*pbSrcCur) & 0x01) != 0)
							*pulDestCur = 0x00001111;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00002222;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00004444;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00008888;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00001111;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00002222;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00004444;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00008888;
						++pulDestCur;
						*pulDestCur = 0;
						++pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
		break;
	
	case 8:
		switch (cbitDest)
		{
		case 32:
			{
				int cbRowSrc = dx;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pbDestCur = (BYTE *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

						*pbDestCur = 0;
						++ pbDestCur;

						*pbDestCur = 0;
						++ pbDestCur;

						*pbDestCur = 0;
						++ pbDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 48:
			{
				int cbRowSrc = dx;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pbDestCur = (BYTE *) pvDest;
				ZeroMemory(pbDestCur, dx * dy * 6);

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						*pbDestCur = *pbSrcCur;

						++ pbDestCur;

						*pbDestCur = *pbSrcCur;

						pbDestCur += 5;
						++pbSrcCur;
					}
				}
			}
			break;

		case 64:
			{
				int cbRowSrc = dx;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pulDestCur = (BYTE *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						*pulDestCur = (BYTE) ((*pbSrcCur) | (*pbSrcCur << 8));
						++ pulDestCur;
						*pulDestCur = 0;
						++ pulDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
		break;

	case 16:
		switch (cbitDest)
		{
		case 32:
			{
				int cbRowSrc = dx * 2;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pbDestCur = (BYTE *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						// Skip the low byte.
						++pbSrcCur;

						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

						*pbDestCur = 0;
						++ pbDestCur;

						++pbSrcCur;
					}
				}
			}
			break;

		case 48:
			{
				int cbRowSrc = dx;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pbDestCur = (BYTE *) pvDest;
				ZeroMemory(pbDestCur, dx * dy * 6);

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						*pbDestCur = *pbSrcCur;
						++pbSrcCur;

						++ pbDestCur;

						*pbDestCur = *pbSrcCur;
						++pbSrcCur;

						pbDestCur += 5;
					}
				}
			}
			break;

		case 64:
			{
				int cbRowSrc = dx;
				int dpbRowSrc = cbRowSrc;
				int cRowSrc = dy;
				const BYTE *pbSrcRow = (const BYTE *) pvSrc;
				const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

				if (fReflect)
				{
					dpbRowSrc = - dpbRowSrc;
					pbSrcRow = pbSrcLim + dpbRowSrc;
					pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
				}

				BYTE *pulDestCur = (BYTE *) pvDest;

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
				{
					pbSrcCur = pbSrcRow;
					pbSrcRowLim = pbSrcCur + cbRowSrc;
		
					while (pbSrcCur != pbSrcRowLim)
					{
						*pulDestCur = (*pbSrcCur);

						++pbSrcCur;
						*pulDestCur |= ((*pbSrcCur) << 8);

						++ pulDestCur;
						*pulDestCur = 0;
						++ pulDestCur;

						++pbSrcCur;
					}
				}
			}

#ifdef _DEBUG
		default:
			assert(0);
			break;
#endif // _DEBUG
		}
		break;
	
#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}


#endif // __VIS_CORE_YUVTORGB_INL__
