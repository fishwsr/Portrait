// VisCopyStandardPixels.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// For now, we don't include the Property List code when compiling this file.
// (We get a compiler heap limit error if we include the Property list and
// new file I/O code.)
// #undef VIS_INCLUDE_SDSTREAM
#define VIS_ONLY_DECLARE_PROPLIST


#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "VisCopyGrayPixels.h"
#include "VisCopyStandardPixels.inl"


VisCoreExport bool VISAPI VisCopyStandardPixels(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize, double dblMin, double dblMax)
{
	bool fRet = false;
	
	// LATER:  Should we allow zero-width or zero-height rectSrc values?
	assert(refrectSrc.left < refrectSrc.right);
	assert(refrectSrc.top < refrectSrc.bottom);
	assert(refimageSrc.ContainsPoint(refrectSrc.left, refrectSrc.top, 0));
	assert(refimageSrc.ContainsPoint(refrectSrc.right - 1, refrectSrc.bottom - 1, 0));
	assert((evisnormalize >= evisnormalizeFirst) && (evisnormalize < evisnormalizeLim));

	// Need to check that cbandSrc == cbandDest in the enumeration.
	int cbandSrc = ((bandSrc == - 1) ? refimageSrc.NBands() : 1);
	int cbandDest = ((bandDest == - 1) ? refimageDest.NBands() : 1);
	if (cbandSrc != cbandDest)
		return false;

	EVisPixFmt evispixfmtSrc = refimageSrc.PixFmt();
	EVisPixFmt evispixfmtDest = refimageDest.PixFmt();

	if (((refimageSrc.PixelTypeInfo() == refimageDest.PixelTypeInfo())
				&& ((evisnormalize == evisnormalizeAdjustNums)
					|| (evisnormalize == evisnormalizeCastValues)
					|| (evisnormalize == evisnormalizeCopyBytesSameType)
					|| (evisnormalize == evisnormalizeCopyBytesSameSize)))
			|| ((refimageSrc.CbPixel() == refimageDest.CbPixel())
				&& ((evisnormalize == evisnormalizeCopyBytesSameSize)
					|| ((evisnormalize == evisnormalizeCastValues)
						&& (((evispixfmtSrc & evispixfmtStructureMask)
									== evispixfmtGray)
							|| ((evispixfmtSrc & evispixfmtStructureMask)
									== evispixfmtRGBA)
							|| ((evispixfmtSrc & evispixfmtStructureMask)
									== evispixfmtYUVA))
						&& ((evispixfmtDest & evispixfmtStructureMask)
									== (evispixfmtSrc & evispixfmtStructureMask))
						&& ((evispixfmtSrc & evispixfmtNumericTypeMask)
									!= evispixfmtFloat)
						&& ((evispixfmtSrc & evispixfmtNumericTypeMask)
									!= evispixfmtDouble)))))
	{
		// Just copy the pixel bytes.
		VisCopyPixelBytes(bandSrc, bandDest, refimageSrc,
				refimageDest, refrectSrc, pointDest, evisnormalize);
		fRet = true;
	}
	else if ((evisnormalize != evisnormalizeCopyBytesSameType)
			&& (evisnormalize != evisnormalizeCopyBytesSameSize))
	{
		if ((evispixfmtSrc & evispixfmtStructureMask) == evispixfmtGray)
		{
			fRet = VisCopyGrayPixels(bandSrc, bandDest, refimageSrc,
					refimageDest, refrectSrc, pointDest, evisnormalize,
					dblMin, dblMax);
		}
		else if ((evispixfmtSrc & evispixfmtStructureMask)
						== (evispixfmtDest & evispixfmtStructureMask))
		{
			if ((evispixfmtDest & evispixfmtStructureMask)
							== evispixfmtRGBA)
			{
				fRet = VisCopyRGBAPixels(bandSrc, bandDest, refimageSrc,
						refimageDest, refrectSrc, pointDest,
						evisnormalize, dblMin, dblMax);
			}
			else if ((evispixfmtDest & evispixfmtStructureMask)
							== evispixfmtYUVA)
			{
				fRet = VisCopyYUVAPixels(bandSrc, bandDest, refimageSrc,
						refimageDest, refrectSrc, pointDest,
						evisnormalize, dblMin, dblMax);
			}
		}
	}

	return fRet;
}


VisCoreExport void VISAPI VisCopyPixelBytes(int bandSrc, int bandDest, const CVisImageBase& refimageSrc,
		CVisImageBase& refimageDest, const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize)
{
	RECT rectDest;
	rectDest.left = pointDest.x;
	rectDest.top = pointDest.y;
	rectDest.right = rectDest.left + (refrectSrc.right - refrectSrc.left);
	rectDest.bottom = rectDest.top + (refrectSrc.bottom - refrectSrc.top);
	assert(refimageDest.ContainsPoint(pointDest, 0));
	assert(refimageDest.ContainsPoint(rectDest.right - 1,
			rectDest.bottom - 1, 0));

	if (((refimageSrc.NBands() == 1) && (refimageDest.NBands() == 1))
			|| ((bandSrc == - 1) && (bandDest == - 1)))
	{
		// Pixels are adjacent, so we can use CopyMemory for efficiency.
		int cbRowCopy = refimageSrc.CbPoint() * (refrectSrc.right - refrectSrc.left);
		if ((cbRowCopy == refimageSrc.CbRow()) && (cbRowCopy == refimageDest.CbRow()))
		{
			// We can copy all pixels in one call.
			CopyMemory(refimageDest.PbPixel(pointDest, 0),
					refimageSrc.PbPixel(refrectSrc.left, refrectSrc.top, 0),
					cbRowCopy * (refrectSrc.bottom - refrectSrc.top));
		}
		else
		{
			int cRow = rectDest.bottom - rectDest.top;
			for (int iRow = 0; iRow < cRow; ++iRow)
			{
				CopyMemory(refimageDest.PbPixel(rectDest.left, rectDest.top + iRow, 0),
						refimageSrc.PbPixel(refrectSrc.left, refrectSrc.top + iRow, 0),
						cbRowCopy);
			}
		}
	}
	else
	{
		CVisEnumPixel enumpixelSrc(refimageSrc, refrectSrc, bandSrc);
		CVisEnumPixel enumpixelDest(refimageDest, rectDest, bandDest);

		const BYTE *pbSrcFirst = refimageSrc.PbPixel(refrectSrc.left, refrectSrc.top, 0);
		BYTE *pbDestFirst = refimageDest.PbPixel(rectDest.left, rectDest.top, 0);
		
		// The following checks see if the size of pixels and their addresses
		// are multiples of 8, 4, 2, or 1 bytes.
		int w = refimageSrc.CbPixel() | refimageSrc.CbRow() | refimageDest.CbRow()
				| ((int) pbSrcFirst) | ((int) pbDestFirst);
		if ((w & 7) == 0)
		{
			assert(sizeof(double) == 8);

			int cdbl = refimageSrc.CbPixel() / sizeof(double);
			assert(cdbl == refimageDest.CbPixel() / sizeof(double));

			if (cdbl == 1)
			{
				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					*((double *) enumpixelDest.PbPixelCur())
							= *((double *) enumpixelSrc.PbPixelCur());
				}
			}
			else
			{
				int idblLast = cdbl - 1;

				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					for (int idbl = idblLast; idbl >= 0; -- idbl)
					{
						((double *) enumpixelDest.PbPixelCur())[idbl]
								= ((double *) enumpixelSrc.PbPixelCur())[idbl];
					}
				}
			}

			assert(!enumpixelDest.FMore());
		}
		else if ((w & 3) == 0)
		{
			assert(sizeof(long) == 4);

			int cl = refimageSrc.CbPixel() / sizeof(long);
			assert(cl == refimageDest.CbPixel() / sizeof(long));

			if (cl == 1)
			{
				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					*((long *) enumpixelDest.PbPixelCur())
							= *((long *) enumpixelSrc.PbPixelCur());
				}
			}
			else
			{
				int ilLast = cl - 1;

				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					for (int il = ilLast; il >= 0; -- il)
					{
						((long *) enumpixelDest.PbPixelCur())[il]
								= ((long *) enumpixelSrc.PbPixelCur())[il];
					}
				}
			}

			assert(!enumpixelDest.FMore());
		}
		else
		{
			int cb = refimageSrc.CbPixel();
			assert(cb == refimageDest.CbPixel());

			if (cb == 1)
			{
				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					*enumpixelDest.PbPixelCur() = *enumpixelSrc.PbPixelCur();
				}
			}
			else
			{
				int ibLast = cb - 1;

				for (enumpixelSrc.Reset(), enumpixelDest.Reset();
						enumpixelSrc.FMore();
						enumpixelSrc.Next(), enumpixelDest.Next())
				{
					assert(enumpixelDest.FMore());

					for (int ib = ibLast; ib >= 0; -- ib)
						enumpixelDest.PbPixelCur()[ib] = enumpixelSrc.PbPixelCur()[ib];
				}
			}

			assert(!enumpixelDest.FMore());
		}
	}
}
