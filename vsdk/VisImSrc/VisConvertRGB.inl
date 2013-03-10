// VisConvertRGB.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// LATER:  Finish testing all cases in these functions.

// LATER:  Try using a LUT instead of int mults to improve performance
// of RGB to gray conversion.


inline void VisInlineGrayToRGB(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


inline void VisInlineRGBToGray(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


inline void VisInlineRGBToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect = false);


// Supported conversions:  {1, 2, 4, 8, 16}-bit gray to 32-bit RGBA.
inline void VisInlineGrayToRGB(const void *pvSrc, int cbitSrc,
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
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
						++pulDestCur;

						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur = 0xffffffff;
						else
							*pulDestCur = 0xff000000;
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
							*pulDestCur = 0x00555555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00aaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++pulDestCur;

						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur = 0x00555555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00aaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00555555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur |= 0x00aaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++pulDestCur;

						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur = 0x00555555;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur |= 0x00aaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
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
							*pulDestCur = 0x00111111;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x02) != 0)
							*pulDestCur |= 0x00222222;
						if (((*pbSrcCur) & 0x04) != 0)
							*pulDestCur |= 0x00444444;
						if (((*pbSrcCur) & 0x08) != 0)
							*pulDestCur |= 0x00888888;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
						++pulDestCur;

						if (((*pbSrcCur) & 0x10) != 0)
							*pulDestCur = 0x00111111;
						else
							*pulDestCur = 0;
						if (((*pbSrcCur) & 0x20) != 0)
							*pulDestCur |= 0x00222222;
						if (((*pbSrcCur) & 0x40) != 0)
							*pulDestCur |= 0x00444444;
						if (((*pbSrcCur) & 0x80) != 0)
							*pulDestCur |= 0x00888888;
#ifndef VIS_DONT_NEED_ALPHA
						*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
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

						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

						*pbDestCur = *pbSrcCur;
						++ pbDestCur;

#ifndef VIS_DONT_NEED_ALPHA
						*pbDestCur = 0xff;
#else // VIS_DONT_NEED_ALPHA
						*pbDestCur = 0;
#endif // VIS_DONT_NEED_ALPHA
						++ pbDestCur;

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

#ifndef VIS_DONT_NEED_ALPHA
						*pbDestCur = 0xff;
#else // VIS_DONT_NEED_ALPHA
						*pbDestCur = 0;
#endif // VIS_DONT_NEED_ALPHA
						++ pbDestCur;

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
	
#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}




// Supported conversions:  {15, 16, 24, 32, 48, 64}-bit RGB to {8, 16, 32}-bit gray.
inline void VisInlineRGBToGray(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	switch (cbitSrc)
	{
	case 15:
		{
			assert ((dx % 2) == 0);

			int culRowSrc = dx / 2;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							*pulDestCur = (ulY >> 16);

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							*pulDestCur |= (ulY & 0xffff);
							++pulDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							++pulDestCur;
							++pulSrcCur;
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
		break;
	
	case 16:
		{
			assert ((dx % 2) == 0);

			int culRowSrc = dx / 2;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 27) & 0x1f);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							*pulDestCur = (ulY >> 16);

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 27) & 0x1f);
							*pulDestCur |= (ulY & 0xffff);
							++pulDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 27) & 0x1f);
							++pulDestCur;
							++pulSrcCur;
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
		break;

	case 24:
		{
			assert ((dx % 4) == 0);

			int culRowSrc = dx * 3 / 4;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							ulY += evisrgbtoyuvYG8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 8) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR8 * (*pulSrcCur & 0xff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 24) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							*pulDestCur = (ulY >> 16);

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							ulY += evisrgbtoyuvYG8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 8) & 0xff);
							*pulDestCur |= (ulY & 0xffff0000);
							++pulDestCur;

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR8 * (*pulSrcCur & 0xff);
							*pulDestCur = (ulY >> 16);

							ulY = evisrgbtoyuvYB8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 24) & 0xff);
							*pulDestCur |= (ulY & 0xffff0000);
							++pulDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							*pulDestCur += evisrgbtoyuvYG8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 8) & 0xff);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 24) & 0xff);
							++pulSrcCur;
							*pulDestCur += evisrgbtoyuvYR8 * (*pulSrcCur & 0xff);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 16) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 24) & 0xff);
							++pulDestCur;
							++pulSrcCur;
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
		break;

	case 32:
		{
			int culRowSrc = dx;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 16);
							++pbDestCur;
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB8 * (*pulSrcCur & 0xff)
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							++pulDestCur;
							++pulSrcCur;
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
		break;

	case 48:
		{
			assert ((dx % 2) == 0);

			int culRowSrc = dx * 3 / 2;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;

							ulY = evisrgbtoyuvYB8 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYG16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYR16 * (*pulSrcCur >> 16);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							*pulDestCur = (ulY >> 16);

							ulY = evisrgbtoyuvYB8 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYG16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYR16 * (*pulSrcCur >> 16);
							*pulDestCur |= (ulY & 0xffff);
							++pulDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							*pulDestCur += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							++pulDestCur;

							*pulDestCur = evisrgbtoyuvYB8 * (*pulSrcCur >> 16);
							++pulSrcCur;
							*pulDestCur += evisrgbtoyuvYG16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYR16 * (*pulSrcCur >> 16);
							++pulDestCur;
							++pulSrcCur;
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
		break;

	case 64:
		{
			int culRowSrc = dx;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;

			switch (cbitDest)
			{
			case 8:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 16:
				{
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							*pbDestCur = (BYTE) (ulY >> 16);
							++pbDestCur;
							*pbDestCur = (BYTE) (ulY >> 24);
							++pbDestCur;
							++pulSrcCur;
						}
					}
				}
				break;
		
			case 32:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							*pulDestCur = evisrgbtoyuvYB16 * (*pulSrcCur & 0xffff)
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							*pulDestCur += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							++pulDestCur;
							++pulSrcCur;
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
		break;
	
#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}




inline void VisInlineRGBToYUV(const void *pvSrc, int cbitSrc,
		void *pvDest, int cbitDest, int dx, int dy, bool fReflect)
{
	switch (cbitSrc)
	{
	case 15:
		{
			assert ((dx % 2) == 0);

			int culRowSrc = dx / 2;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;
			unsigned long ulU;
			unsigned long ulV;

			switch (cbitDest)
			{
			case 24:
			case 32:
				{
					bool f32bit = (cbitDest == 32);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (((*pulSrcCur << 3) & 0xf8) - ulY);
							ulU >>= 24;
							*pbDestCur = (BYTE) ulU;
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 7) & 0xf8) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (((*pulSrcCur >> 13) & 0xf8) - ulY);
							ulU >>= 24;
							*pbDestCur = (BYTE) ulU;
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 18) & 0xf8) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							if (f32bit)
								++pbDestCur;

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 48:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur << 11) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * (((*pulSrcCur << 1) & 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							*pulDestCur |= (ulY & 0xffff0000);
							ulY >>= 16;
							++pulDestCur;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur >> 5) & 0xf800) - ulY);
							ulU >>= 16;
							*pulDestCur = ulU;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = ((evisrgbtoyuvV14 * (((*pulSrcCur >> 10) & 0xf800) - ulY) << 2) & 0xffff0000);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur |= ulV;
							++pulDestCur;

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 64:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 10) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur << 11) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * (((*pulSrcCur << 1) & 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;
							++pulDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x1f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 26) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur >> 5) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * (((*pulSrcCur >> 10) & 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;
							++pulDestCur;

							++pulSrcCur;
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
		break;
	
	case 16:
		{
			assert ((dx % 2) == 0);

			int culRowSrc = dx / 2;
			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;
			unsigned long ulU;
			unsigned long ulV;

			switch (cbitDest)
			{
			case 24:
			case 32:
				{
					bool f32bit = (cbitDest == 32);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (((*pulSrcCur << 3) & 0xf8) - ulY);
							ulU >>= 24;
							*pbDestCur = (BYTE) ulU;
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 8) & 0xf8) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 27) & 0x1f);
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (((*pulSrcCur >> 13) & 0xf8) - ulY);
							ulU >>= 24;
							*pbDestCur = (BYTE) ulU;
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 19) & 0xf8) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							if (f32bit)
								++pbDestCur;

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 48:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur << 11) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * ((*pulSrcCur & 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG6 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 27) & 0x1f);
							*pulDestCur |= (ulY & 0xffff0000);
							ulY >>= 16;
							++pulDestCur;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur >> 5) & 0xf800) - ulY);
							ulU >>= 16;
							*pulDestCur = ulU;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = ((evisrgbtoyuvV14 * (((*pulSrcCur >> 11) & 0xf800) - ulY) << 2) & 0xffff0000);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur |= ulV;
							++pulDestCur;

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 64:
				{
					unsigned long *pulDestCur = (unsigned long *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulY = evisrgbtoyuvYB5 * (*pulSrcCur & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 5) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 11) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur << 11) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * ((*pulSrcCur& 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;
							++pulDestCur;

							ulY = evisrgbtoyuvYB5 * ((*pulSrcCur >> 16) & 0x1f)
									+ evisrgbtoyuvYG5 * ((*pulSrcCur >> 21) & 0x3f)
									+ evisrgbtoyuvYR5 * ((*pulSrcCur >> 21) & 0x1f);
							ulY >>= 16;
							*pulDestCur = ulY;

							ulU = evisrgbtoyuvU16 * (((*pulSrcCur >> 5) & 0xf800) - ulY);
							*pulDestCur |= (ulU & 0xffff0000);
							++pulDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * (((*pulSrcCur >> 11) & 0xf800) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pulDestCur = ulV;
							++pulDestCur;

							++pulSrcCur;
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
		break;

	case 24:
	case 32:
		{
			bool f24bitSrc;
			int culRowSrc;

			if (cbitSrc == 24)
			{
				assert ((dx % 4) == 0);

				f24bitSrc = true;
				culRowSrc = dx * 3 / 4;
			}
			else
			{
				f24bitSrc = false;
				culRowSrc = dx;
			}

			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;
			unsigned long ulU;
			unsigned long ulV;
			unsigned long ulB;

			switch (cbitDest)
			{
			case 24:
			case 32:
				{
					bool f32bitDest = (cbitDest == 32);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulB = (*pulSrcCur & 0xff);
							ulY = evisrgbtoyuvYB8 * ulB
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (ulB - ulY);
							*pbDestCur = (BYTE) (ulU >> 24);
							++pbDestCur;

							// UNDONE:  Lines like
							//      ulV = evisrgbtoyuvV6 * (((*pulSrcCur >> 16) & 0xff) - ulY);
							// in other places may need to be changed to use ulT before checking for
							// overflow.
#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
							unsigned long ulT = (((*pulSrcCur >> 16) & 0xff) - ulY);
							if ((ulT & 0x80000080)  != 0x80000000)
							{
								if ((ulT & 0x80000080)  != 0x00000080)
								{
									ulV = ((evisrgbtoyuvV6 * ulT) >> 22);  // No clipping needed.
								}
								else
								{
									ulV = 0x7f;  // Clip to the highest possible value
								}
							}
							else
							{
								ulV = 0x80;  // Clip to the lowest possible value
							}
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 16) & 0xff) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							if (f32bitDest)
								++pbDestCur;

							if (f24bitSrc)
							{
								ulB = (*pulSrcCur >> 24);

								ulY = evisrgbtoyuvYB8 * (ulB & 0xff);
								++pulSrcCur;
								ulY += evisrgbtoyuvYG8 * (*pulSrcCur & 0xff)
										+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 8) & 0xff);
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
								ulT = (((*pulSrcCur >> 8) & 0xff) - ulY);
								if ((ulT & 0x80000080)  != 0x80000000)
								{
									if ((ulT & 0x80000080)  != 0x00000080)
									{
										ulV = ((evisrgbtoyuvV6 * ulT) >> 22);  // No clipping needed.
									}
									else
									{
										ulV = 0x7f;  // Clip to the highest possible value
									}
								}
								else
								{
									ulV = 0x80;  // Clip to the lowest possible value
								}
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 8) & 0xff) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;

								if (f32bitDest)
									++pbDestCur;


								ulB = ((*pulSrcCur >> 16) & 0xff);
								ulY = evisrgbtoyuvYB8 * ulB
										+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 24) & 0xff);
								++pulSrcCur;
								ulY += evisrgbtoyuvYR8 * (*pulSrcCur & 0xff);
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
								ulT = ((*pulSrcCur & 0xff) - ulY);
								if ((ulT & 0x80000080)  != 0x80000000)
								{
									if ((ulT & 0x80000080)  != 0x00000080)
									{
										ulV = ((evisrgbtoyuvV6 * ulT) >> 22);  // No clipping needed.
									}
									else
									{
										ulV = 0x7f;  // Clip to the highest possible value
									}
								}
								else
								{
									ulV = 0x80;  // Clip to the lowest possible value
								}
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * ((*pulSrcCur & 0xff) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;

								if (f32bitDest)
									++pbDestCur;


								ulB = ((*pulSrcCur >> 8) & 0xff);
								ulY = evisrgbtoyuvYB8 * ulB
										+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 16) & 0xff)
										+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 24) & 0xff);
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
								ulT = (((*pulSrcCur >> 24) & 0xff) - ulY);
								if ((ulT & 0x80000080)  != 0x80000000)
								{
									if ((ulT & 0x80000080)  != 0x00000080)
									{
										ulV = ((evisrgbtoyuvV6 * ulT) >> 22);  // No clipping needed.
									}
									else
									{
										ulV = 0x7f;  // Clip to the highest possible value
									}
								}
								else
								{
									ulV = 0x80;  // Clip to the lowest possible value
								}
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 24) & 0xff) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;

								if (f32bitDest)
									++pbDestCur;
							}

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 48:
			case 64:
				{
					bool f64bitDest = (cbitDest == 64);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							unsigned long ulB = (*pulSrcCur & 0xff);
							ulY = evisrgbtoyuvYB8 * ulB
									+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 8) & 0xff)
									+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 16) & 0xff);
							*pbDestCur = (BYTE) (ulY >> 16);
							++pbDestCur;
							ulY >>= 24;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;

							ulU = evisrgbtoyuvU8 * (ulB - ulY);
							*pbDestCur = (BYTE) (ulU >> 16);
							++pbDestCur;
							*pbDestCur = (BYTE) (ulU >> 24);
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 16) & 0xff) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;
							*pbDestCur = (BYTE) (ulV >> 8);
							++pbDestCur;

							if (f64bitDest)
								pbDestCur += 2;

							if (f24bitSrc)
							{
								ulB = (*pulSrcCur >> 24);

								ulY = evisrgbtoyuvYB8 * (ulB & 0xff);
								++pulSrcCur;
								ulY += evisrgbtoyuvYG8 * (*pulSrcCur & 0xff)
										+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 8) & 0xff);
								*pbDestCur = (BYTE) (ulY >> 16);
								++pbDestCur;
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 16);
								++pbDestCur;
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 8) & 0xff) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;
								*pbDestCur = (BYTE) (ulV >> 8);
								++pbDestCur;

								if (f64bitDest)
									pbDestCur += 2;



								ulB = ((*pulSrcCur >> 16) & 0xff);
								ulY = evisrgbtoyuvYB8 * ulB
										+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 24) & 0xff);
								++pulSrcCur;
								ulY += evisrgbtoyuvYR8 * (*pulSrcCur & 0xff);
								*pbDestCur = (BYTE) (ulY >> 16);
								++pbDestCur;
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 16);
								++pbDestCur;
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * ((*pulSrcCur & 0xff) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;
								*pbDestCur = (BYTE) (ulV >> 8);
								++pbDestCur;

								if (f64bitDest)
									pbDestCur += 2;


								ulB = ((*pulSrcCur >> 8) & 0xff);
								ulY = evisrgbtoyuvYB8 * ulB
										+ evisrgbtoyuvYG8 * ((*pulSrcCur >> 16) & 0xff)
										+ evisrgbtoyuvYR8 * ((*pulSrcCur >> 24) & 0xff);
								*pbDestCur = (BYTE) (ulY >> 16);
								++pbDestCur;
								ulY >>= 24;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;

								ulU = evisrgbtoyuvU8 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 16);
								++pbDestCur;
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV6 * (((*pulSrcCur >> 24) & 0xff) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;
								*pbDestCur = (BYTE) (ulV >> 8);
								++pbDestCur;

								if (f64bitDest)
									pbDestCur += 2;
							}

							++pulSrcCur;
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
		break;

	case 48:
	case 64:
		{
			bool f48bitSrc;
			int culRowSrc;

			if (cbitSrc == 24)
			{
				assert ((dx % 4) == 0);

				f48bitSrc = true;
				culRowSrc = dx * 3 / 2;
			}
			else
			{
				f48bitSrc = false;
				culRowSrc = dx * 2;
			}

			int dpulRowSrc = culRowSrc;
			int cRowSrc = dy;
			const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
			const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

			if (fReflect)
			{
				dpulRowSrc = - dpulRowSrc;
				pulSrcRow = pulSrcLim + dpulRowSrc;
				pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
			}

			const unsigned long *pulSrcCur;
			const unsigned long *pulSrcRowLim;

			unsigned long ulY;
			unsigned long ulU;
			unsigned long ulV;
			unsigned long ulB;

			switch (cbitDest)
			{
			case 24:
			case 32:
				{
					bool f32bitDest = (cbitDest == 32);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulB = (*pulSrcCur & 0xffff);
							ulY = evisrgbtoyuvYB16 * ulB
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							ulY >>= 16;
							*pbDestCur = (BYTE) (ulY >> 8);
							++pbDestCur;

							ulU = evisrgbtoyuvU16 * (ulB - ulY);
							*pbDestCur = (BYTE) (ulU >> 24);
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * ((*pulSrcCur & 0xffff) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;

							if (f32bitDest)
								++pbDestCur;

							if (f48bitSrc)
							{
								ulB = (*pulSrcCur >> 16);
								++pulSrcCur;
								ulY = evisrgbtoyuvYB16 * (ulB & 0xff)
										+ evisrgbtoyuvYG16 * (*pulSrcCur & 0xffff)
										+ evisrgbtoyuvYR16 * (*pulSrcCur >> 16);
								ulY >>= 16;
								*pbDestCur = (BYTE) (ulY >> 8);
								++pbDestCur;

								ulU = evisrgbtoyuvU16 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV14 * ((*pulSrcCur >> 16) - ulY) >> 22);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								++pbDestCur;

								if (f32bitDest)
									++pbDestCur;
							}

							++pulSrcCur;
						}
					}
				}
				break;
		
			case 48:
			case 64:
				{
					bool f64bitDest = (cbitDest == 64);
					BYTE *pbDestCur = (BYTE *) pvDest;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							ulB = (*pulSrcCur & 0xffff);
							ulY = evisrgbtoyuvYB16 * ulB
									+ evisrgbtoyuvYG16 * (*pulSrcCur >> 16);
							++pulSrcCur;
							ulY += evisrgbtoyuvYR16 * (*pulSrcCur & 0xffff);
							ulY >>= 16;
							*pbDestCur = (BYTE) ulY;
							++pbDestCur;
							*pbDestCur = (BYTE) (ulY >> 8);
							++pbDestCur;

							ulU = evisrgbtoyuvU16 * (ulB - ulY);
							*pbDestCur = (BYTE) (ulU >> 16);
							++pbDestCur;
							*pbDestCur = (BYTE) (ulU >> 24);
							++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
							// The new RGB-to-YUV equations do not require clipping.
							ulV = (evisrgbtoyuvV14 * ((*pulSrcCur & 0xffff) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
							*pbDestCur = (BYTE) ulV;
							++pbDestCur;
							*pbDestCur = (BYTE) (ulV >> 8);
							++pbDestCur;

							if (f64bitDest)
								pbDestCur+=2;

							if (f48bitSrc)
							{
								ulB = (*pulSrcCur >> 16);
								++pulSrcCur;
								ulY = evisrgbtoyuvYB16 * (ulB & 0xff)
										+ evisrgbtoyuvYG16 * (*pulSrcCur & 0xffff)
										+ evisrgbtoyuvYR16 * (*pulSrcCur >> 16);
								ulY >>= 16;
								*pbDestCur = (BYTE) ulY;
								++pbDestCur;
								*pbDestCur = (BYTE) (ulY >> 8);
								++pbDestCur;

								ulU = evisrgbtoyuvU16 * (ulB - ulY);
								*pbDestCur = (BYTE) (ulU >> 16);
								++pbDestCur;
								*pbDestCur = (BYTE) (ulU >> 24);
								++pbDestCur;

#ifdef VIS_IMSRC_CONVERT_RGB_CLIP_V
#error UNDONE:  Clipping V
#else // VIS_IMSRC_CONVERT_RGB_CLIP_V
								// The new RGB-to-YUV equations do not require clipping.
								ulV = (evisrgbtoyuvV14 * ((*pulSrcCur >> 16) - ulY) >> 14);
#endif // VIS_IMSRC_CONVERT_RGB_CLIP_V
								*pbDestCur = (BYTE) ulV;
								++pbDestCur;
								*pbDestCur = (BYTE) (ulV >> 8);
								++pbDestCur;

								if (f64bitDest)
									pbDestCur+=2;
							}

							++pulSrcCur;
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
		break;

#ifdef _DEBUG
	default:
		assert(0);
		break;
#endif // _DEBUG
	}
}
