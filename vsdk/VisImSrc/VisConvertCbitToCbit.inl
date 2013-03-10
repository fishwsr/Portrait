// VisConvertCbitToCbit.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_IMSRC_CONVERTCBITTOCBIT_INL
#define __VIS_IMSRC_CONVERTCBITTOCBIT_INL


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


inline void VisInlineConvert555To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineConvert565To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect = false);

inline void VisInlineConvertCbitToCbit(const void *pvSrc, int cbitSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect = false);


inline void VisInlineConvert555To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	if ((dx & 1) == 0)
	{
		// Process two pixels at a time.
		int culRowSrc = dx / 2;
		int dpulRowSrc = culRowSrc;
		int cRowSrc = dy;
		const unsigned long *pulSrcRow = (unsigned long *) pvSrc;
		const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

		unsigned long *pulDestCur = (unsigned long *) pvDest;
		unsigned long *pulDestLim = pulDestCur + dx * dy;

		if (fReflect)
		{
			dpulRowSrc = - dpulRowSrc;
			pulSrcRow = pulSrcLim + dpulRowSrc;
			pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
		}

		for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
		{
			const unsigned long *pulSrcCur = pulSrcRow;
			const unsigned long *pulSrcRowLim = pulSrcCur + culRowSrc;

			for ( ; pulSrcCur != pulSrcRowLim; ++pulSrcCur, ++pulDestCur)
			{
				*pulDestCur = (((*pulSrcCur) & 0x1f) << 3)
						| (((*pulSrcCur) & (0x1f << 5)) << 6)
						| (((*pulSrcCur) & (0x1f << 10)) << 9);
				
#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA

				++ pulDestCur;

				*pulDestCur = (((*pulSrcCur) & 0x1f0000) >> 13)
						| (((*pulSrcCur) & (0x1f0000 << 5)) >> 10)
						| (((*pulSrcCur) & (0x1f0000 << 10)) >> 7);
				
#ifndef VIS_DONT_NEED_ALPHA
					*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
			}
		}
	}
	else
	{
		int dpusRowSrc = dx;
		int cpixRowSrc = dx;
		int cRowSrc = dy;
		const unsigned short *pusSrcRow = (unsigned short *) pvSrc;
		const unsigned short *pusSrcLim = pusSrcRow + cpixRowSrc * cRowSrc;

		unsigned long *pulDestCur = (unsigned long *) pvDest;
		unsigned long *pulDestLim = pulDestCur + dx * dy;

		if (fReflect)
		{
			dpusRowSrc = - dpusRowSrc;
			pusSrcRow = pusSrcLim + dpusRowSrc;
			pusSrcLim = pusSrcRow - cpixRowSrc * cRowSrc;
		}

		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			const unsigned short *pusSrcCur = pusSrcRow;
			const unsigned short *pusSrcRowLim = pusSrcCur + cpixRowSrc;

			for ( ; pusSrcCur != pusSrcRowLim; ++pusSrcCur, ++pulDestCur)
			{
				*pulDestCur = (((*pusSrcCur) & 0x1f) << 3)
						| (((*pusSrcCur) & (0x1f << 5)) << 6)
						| (((*pusSrcCur) & (0x1f << 10)) << 9);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
			}
		}
	}
}


inline void VisInlineConvert565To32bit(const void *pvSrc, void *pvDest,
		int dx, int dy, bool fReflect)
{
	if ((dx & 1) == 0)
	{
		// Process two pixels at a time.
		int culRowSrc = dx / 2;
		int dpulRowSrc = culRowSrc;
		int cRowSrc = dy;
		const unsigned long *pulSrcRow = (unsigned long *) pvSrc;
		const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

		unsigned long *pulDestCur = (unsigned long *) pvDest;
		unsigned long *pulDestLim = pulDestCur + dx * dy;

		if (fReflect)
		{
			dpulRowSrc = - dpulRowSrc;
			pulSrcRow = pulSrcLim + dpulRowSrc;
			pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
		}

		for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
		{
			const unsigned long *pulSrcCur = pulSrcRow;
			const unsigned long *pulSrcRowLim = pulSrcCur + culRowSrc;

			for ( ; pulSrcCur != pulSrcRowLim; ++pulSrcCur, ++pulDestCur)
			{
				*pulDestCur = (((*pulSrcCur) & 0x1f) << 3)
						| (((*pulSrcCur) & (0x3f << 5)) << 5)
						| (((*pulSrcCur) & (0x1f << 11)) << 8);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA

				++ pulDestCur;

				*pulDestCur = (((*pulSrcCur) & 0x1f0000) >> 13)
						| (((*pulSrcCur) & (0x3f0000 << 5)) >> 11)
						| (((*pulSrcCur) & (0x1f0000 << 10)) >> 6);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
			}
		}
	}
	else
	{
		int dpusRowSrc = dx;
		int cpixRowSrc = dx;
		int cRowSrc = dy;
		const unsigned short *pusSrcRow = (unsigned short *) pvSrc;
		const unsigned short *pusSrcLim = pusSrcRow + cpixRowSrc * cRowSrc;

		unsigned long *pulDestCur = (unsigned long *) pvDest;
		unsigned long *pulDestLim = pulDestCur + dx * dy;

		if (fReflect)
		{
			dpusRowSrc = - dpusRowSrc;
			pusSrcRow = pusSrcLim + dpusRowSrc;
			pusSrcLim = pusSrcRow - cpixRowSrc * cRowSrc;
		}

		for ( ; pusSrcRow != pusSrcLim; pusSrcRow += dpusRowSrc)
		{
			const unsigned short *pusSrcCur = pusSrcRow;
			const unsigned short *pusSrcRowLim = pusSrcCur + cpixRowSrc;

			for ( ; pusSrcCur != pusSrcRowLim; ++pusSrcCur, ++pulDestCur)
			{
				*pulDestCur = (((*pusSrcCur) & 0x1f) << 3)
						| (((*pusSrcCur) & (0x3f << 5)) << 5)
						| (((*pusSrcCur) & (0x1f << 11)) << 8);
#ifndef VIS_DONT_NEED_ALPHA
				*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
			}
		}
	}
}


inline void VisInlineConvertCbitToCbit(const void *pvSrc, int cbitSrc, void *pvDest,
		int cbitDest, int dx, int dy, bool fReflect)
{
	if (cbitSrc == cbitDest)
	{
		if (!fReflect)
		{
			CopyMemory(pvDest, pvSrc, dx * dy * cbitSrc / 8);
		}
		else
		{
			int cbRow = dx * cbitSrc / 8;
			const BYTE *pbSrcCur = (const BYTE *) pvSrc;
			const BYTE *pbSrcLim = pbSrcCur + dy * cbRow;
			BYTE *pbDestCur = ((BYTE *) pvDest) + (dy - 1) * cbRow;
			for ( ; pbSrcCur != pbSrcLim; pbSrcCur += cbRow, pbDestCur -= cbRow)
				CopyMemory(pbDestCur, pbSrcCur, cbRow);
		}
	}
	else
	{
		switch (cbitSrc)
		{
		case 1:
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

				switch (cbitDest)
				{
				case 8:
					{
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
								if (((*pbSrcCur) & 0x02) != 0)
									*pulDestCur |= 0x0000ff00;
								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur |= 0x00ff0000;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0xff000000;

								++pulDestCur;

								*pulDestCur = 0;
								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur |= 0x000000ff;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0x0000ff00;
								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur |= 0x00ff0000;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0xff000000;

								++pulDestCur;
								++pbSrcCur;
							}
						}
					}
					break;

				case 16:
					{
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
								if (((*pbSrcCur) & 0x02) != 0)
									*pulDestCur |= 0xffff0000;

								++pulDestCur;

								*pulDestCur = 0;
								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur |= 0x0000ffff;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0xffff0000;

								++pulDestCur;

								*pulDestCur = 0;
								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur |= 0x0000ffff;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0xffff0000;

								++pulDestCur;

								*pulDestCur = 0;
								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur |= 0x0000ffff;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0xffff0000;

								++pulDestCur;
								++pbSrcCur;
							}
						}
					}

				case 32:
					{
						for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
						{
							pbSrcCur = pbSrcRow;
							pbSrcRowLim = pbSrcCur + cbRowSrc;
				
							while (pbSrcCur != pbSrcRowLim)
							{
								if (((*pbSrcCur) & 0x01) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x02) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur = (unsigned long) ~0;
								else
									*pulDestCur = 0xff000000;
								++pulDestCur;

								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur = (unsigned long) ~0;
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
			}

		case 2:
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

				switch (cbitDest)
				{
				case 8:
					{
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
								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur |= 0x00005500;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0x0000aa00;
								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur |= 0x00550000;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0x00aa0000;
								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur |= 0x55000000;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0xaa000000;

								++pulDestCur;
								++pbSrcCur;
							}
						}
					}
					break;

				case 16:
					{
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
								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur |= 0x55550000;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0xaaaa0000;

								++pulDestCur;

								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur |= 0x00005555;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0x0000aaaa;
								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur |= 0x55550000;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0xaaaa0000;

								++pulDestCur;
								++pbSrcCur;
							}
						}
					}
					break;

				case 32:
					{
						for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
						{
							pbSrcCur = pbSrcRow;
							pbSrcRowLim = pbSrcCur + cbRowSrc;
				
							while (pbSrcCur != pbSrcRowLim)
							{
								if (((*pbSrcCur) & 0x01) != 0)
									*pulDestCur = 0x555555;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x02) != 0)
									*pulDestCur |= 0xaaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
								*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
								++pulDestCur;

								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur = 0x555555;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0xaaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
								*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
								++pulDestCur;

								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur = 0x555555;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0xaaaaaa;
#ifndef VIS_DONT_NEED_ALPHA
								*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
								++pulDestCur;

								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur = 0x555555;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0xaaaaaa;
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
			}
			break;

		case 4:
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

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				switch (cbitDest)
				{
				case 8:
					{
						BYTE *pbDestCur = (BYTE *) pvDest;

						for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
						{
							pbSrcCur = pbSrcRow;
							pbSrcRowLim = pbSrcCur + cbRowSrc;
				
							while (pbSrcCur != pbSrcRowLim)
							{
								// Note:  Instead of shifting, we multiply by 0x11 to
								// use the full 8-bit range.
								*pbDestCur = (BYTE) ((*pbSrcCur << 4) | (*pbSrcCur & 0x0f));
								++pbDestCur;

								*pbDestCur = (BYTE) ((*pbSrcCur >> 4) | (*pbSrcCur & 0xf0));
								++pbDestCur;

								++pbSrcCur;
							}
						}
					}
					break;

				case 16:
					{
						// Mapping from BYTE values to pairs of dest values.
						static unsigned long mpiul[256];
						static bool fSetMpiul = false;
						if (!fSetMpiul)
						{
							for (int i = 0; i < 256; ++i)
							{
								// Note:  Instead of shifting, we multiply by 0x1111 to
								// use the full 8-bit range.
								mpiul[i] = (i & 0x0f) * 0x1111 + (i & 0xf0) * 0x01111000;
							}

							fSetMpiul = true;
						}

						unsigned long *pulDestCur = (unsigned long *) pvDest;

						for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
						{
							pbSrcCur = pbSrcRow;
							pbSrcRowLim = pbSrcCur + cbRowSrc;
				
							while (pbSrcCur != pbSrcRowLim)
							{
								*pulDestCur = mpiul[*pbSrcCur];
								++pulDestCur;
								++pbSrcCur;
							}
						}
					}
					break;

				case 32:
					{
						unsigned long *pulDestCur = (unsigned long *) pvDest;

						for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
						{
							pbSrcCur = pbSrcRow;
							pbSrcRowLim = pbSrcCur + cbRowSrc;
				
							while (pbSrcCur != pbSrcRowLim)
							{
								if (((*pbSrcCur) & 0x01) != 0)
									*pulDestCur = 0x11111111;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x02) != 0)
									*pulDestCur |= 0x222222;
								if (((*pbSrcCur) & 0x04) != 0)
									*pulDestCur |= 0x444444;
								if (((*pbSrcCur) & 0x08) != 0)
									*pulDestCur |= 0x888888;
#ifndef VIS_DONT_NEED_ALPHA
								*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
								++pulDestCur;

								if (((*pbSrcCur) & 0x10) != 0)
									*pulDestCur = 0x111111;
								else
									*pulDestCur = 0;
								if (((*pbSrcCur) & 0x20) != 0)
									*pulDestCur |= 0x222222;
								if (((*pbSrcCur) & 0x40) != 0)
									*pulDestCur |= 0x444444;
								if (((*pbSrcCur) & 0x80) != 0)
									*pulDestCur |= 0x888888;
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
			}

		case 8:
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

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				BYTE *pbDestCur = (BYTE *) pvDest;

				switch (cbitDest)
				{
				case 16:
					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Note:  Instead of shifting, we multiply by 0x101 to
							// use the full 16-bit range.
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;
						}
					}
					break;

				case 32:
					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

#ifndef VIS_DONT_NEED_ALPHA
							*pbDestCur = 0xff;
#else // VIS_DONT_NEED_ALPHA
							*pbDestCur = *pbSrcCur;
#endif // VIS_DONT_NEED_ALPHA
							++pbDestCur;

							++pbSrcCur;
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

				const BYTE *pbSrcCur;
				const BYTE *pbSrcRowLim;

				BYTE *pbDestCur = (BYTE *) pvDest;

				switch (cbitDest)
				{
				case 8:
					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;
						}
					}
					break;

				case 32:
					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = pbSrcCur[1];
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;
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
			switch (cbitDest)
			{
			case 32:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int dpulRowSrc = (dx / 4) * 3;
					int culRowSrc = (dx / 4) * 3;
					int cRowSrc = dy;
					const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
					const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

					unsigned long *pulDestCur = (unsigned long *) pvDest;

					if (fReflect)
					{
						dpulRowSrc = - dpulRowSrc;
						pulSrcRow = pulSrcLim + dpulRowSrc;
						pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
					}

					const unsigned long *pulSrcCur;
					const unsigned long *pulSrcRowLim;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							// One (dest)
							*pulDestCur = (*pulSrcCur & 0xffffff);
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
							++pulDestCur;
							*pulDestCur = (*pulSrcCur >> 24);
							++pulSrcCur;

							// Two (dest)
							*pulDestCur |= ((*pulSrcCur & 0xffff) << 8);
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
							++pulDestCur;
							*pulDestCur = (*pulSrcCur >> 16);
							++pulSrcCur;

							// Three (dest)
							*pulDestCur |= ((*pulSrcCur & 0xff) << 16);
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
							++pulDestCur;

							// Four (dest)
							*pulDestCur = (*pulSrcCur >> 8);
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xff000000;
#endif // VIS_DONT_NEED_ALPHA
							++pulSrcCur;
							++pulDestCur;
						}
					}
				}
				break;

			case 48:
				{
					int cbRowSrc = dx * 3;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Note:  Instead of shifting, we multiply by 0x101 to
							// use the full 16-bit range.
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;
						}
					}
				}
				break;

			case 64:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int cbRowSrc = dx * 3;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Note:  Instead of shifting, we multiply by 0x101 to
							// use the full 16-bit range.

							// One
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							// Two
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							// Three (and four in dest)
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							pbDestCur++;
							*pbDestCur = 0xff;
							pbDestCur++;
							*pbDestCur = 0xff;
							pbDestCur++;

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

		case 32:
			switch (cbitDest)
			{
			case 24:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int dpulRowSrc = dx;
					int culRowSrc = dx;
					int cRowSrc = dy;
					const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
					const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

					unsigned long *pulDestCur = (unsigned long *) pvDest;

					if (fReflect)
					{
						dpulRowSrc = - dpulRowSrc;
						pulSrcRow = pulSrcLim + dpulRowSrc;
						pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
					}

					const unsigned long *pulSrcCur;
					const unsigned long *pulSrcRowLim;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							// One (dest)
							*pulDestCur = (*pulSrcCur & 0xffffff);
							++pulSrcCur;
							*pulDestCur |= (*pulSrcCur << 24);
							++pulDestCur;
							*pulDestCur = ((*pulSrcCur >> 8) & 0xffff);
							++pulSrcCur;

							// Two (dest)
							*pulDestCur |= (*pulSrcCur << 16);
							++pulDestCur;
							*pulDestCur = ((*pulSrcCur >> 16) & 0xff);
							++pulSrcCur;

							// Three (dest)
							*pulDestCur |= (*pulSrcCur << 8);
							++pulSrcCur;
							++pulDestCur;
						}
					}
				}
				break;

			case 48:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int cbRowSrc = dx * 4;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Note:  Instead of shifting, we multiply by 0x101 to
							// use the full 16-bit range.

							// One
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							// Two
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							// Three
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							++pbSrcCur;

							// (and four in source)
							++pbSrcCur;
						}
					}
				}
				break;

			case 64:
				{
					int cbRowSrc = dx * 4;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Note:  Instead of shifting, we multiply by 0x101 to
							// use the full 16-bit range.
							*pbDestCur = *pbSrcCur;
							++pbDestCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;

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

		case 48:
			switch (cbitDest)
			{
			case 24:
				{
					int cbRowSrc = dx * 6;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Skip the low-byte
							++pbSrcCur;

							*pbDestCur = *pbSrcCur;

							++pbDestCur;
							++pbSrcCur;
						}
					}
				}
				break;

			case 32:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int cbRowSrc = dx * 6;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// One (dest)
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;

							// Two (dest)
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;

							// Three (dest)
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;

							// Four (dest)
#ifndef VIS_DONT_NEED_ALPHA
							*pbDestCur = 0xff;
#else // VIS_DONT_NEED_ALPHA
							*pbDestCur = 0;
#endif // VIS_DONT_NEED_ALPHA
							++pbDestCur;
						}
					}
				}
				break;

			case 64:
				{
					// This code assumes that the width is a multiple of two pixels.
					assert(dx % 2 == 0);

					int culRowSrc = dx * 3 / 2;
					int dpulRowSrc = culRowSrc;
					int cRowSrc = dy;
					const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
					const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

					unsigned long *pulDestCur = (unsigned long *) pvDest;

					if (fReflect)
					{
						dpulRowSrc = - dpulRowSrc;
						pulSrcRow = pulSrcLim + dpulRowSrc;
						pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
					}

					const unsigned long *pulSrcCur;
					const unsigned long *pulSrcRowLim;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							// One
							*pulDestCur = *pulSrcCur;
							++pulSrcCur;
							++pulDestCur;

							*pulDestCur = (*pulSrcCur & 0xffff);
							++pulSrcCur;
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xffff0000;
#endif // VIS_DONT_NEED_ALPHA
							++pulDestCur;

							// Two
							*pulDestCur = (*pulSrcCur >> 16);
							++pulSrcCur;
							*pulDestCur |= (*pulSrcCur << 16);
							++pulDestCur;

							*pulDestCur = (*pulSrcCur >> 16);
							++pulSrcCur;
#ifndef VIS_DONT_NEED_ALPHA
							*pulDestCur |= 0xffff0000;
#endif // VIS_DONT_NEED_ALPHA
							++pulDestCur;
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

		case 64:
			switch (cbitDest)
			{
			case 24:
				{
					// This code assumes that the width is a multiple of four pixels.
					assert(dx % 4 == 0);

					int cbRowSrc = dx * 8;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// One
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;

							// Two
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;

							// Three (and four for source)
							++pbSrcCur;
							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							pbSrcCur += 3;
						}
					}
				}
				break;

			case 32:
				{
					int cbRowSrc = dx * 8;
					int dpbRowSrc = cbRowSrc;
					int cRowSrc = dy;
					const BYTE *pbSrcRow = (const BYTE *) pvSrc;
					const BYTE *pbSrcLim = pbSrcRow + cbRowSrc * cRowSrc;

					BYTE *pbDestCur = (BYTE *) pvDest;

					if (fReflect)
					{
						dpbRowSrc = - dpbRowSrc;
						pbSrcRow = pbSrcLim + dpbRowSrc;
						pbSrcLim = pbSrcRow - cbRowSrc * cRowSrc;
					}

					const BYTE *pbSrcCur;
					const BYTE *pbSrcRowLim;

					for ( ; pbSrcRow != pbSrcLim; pbSrcRow += dpbRowSrc)
					{
						pbSrcCur = pbSrcRow;
						pbSrcRowLim = pbSrcCur + cbRowSrc;
			
						while (pbSrcCur != pbSrcRowLim)
						{
							// Skip the low-byte
							++pbSrcCur;

							*pbDestCur = *pbSrcCur;
							++pbDestCur;
							++pbSrcCur;
						}
					}
				}
				break;

			case 48:
				{
					// This code assumes that the width is a multiple of two pixels.
					assert(dx % 2 == 0);

					int culRowSrc = dx * 2;
					int dpulRowSrc = culRowSrc;
					int cRowSrc = dy;
					const unsigned long *pulSrcRow = (const unsigned long *) pvSrc;
					const unsigned long *pulSrcLim = pulSrcRow + culRowSrc * cRowSrc;

					unsigned long *pulDestCur = (unsigned long *) pvDest;

					if (fReflect)
					{
						dpulRowSrc = - dpulRowSrc;
						pulSrcRow = pulSrcLim + dpulRowSrc;
						pulSrcLim = pulSrcRow - culRowSrc * cRowSrc;
					}

					const unsigned long *pulSrcCur;
					const unsigned long *pulSrcRowLim;

					for ( ; pulSrcRow != pulSrcLim; pulSrcRow += dpulRowSrc)
					{
						pulSrcCur = pulSrcRow;
						pulSrcRowLim = pulSrcCur + culRowSrc;
			
						while (pulSrcCur != pulSrcRowLim)
						{
							// One
							*pulDestCur = *pulSrcCur;
							++pulSrcCur;
							++pulDestCur;

							*pulDestCur = (*pulSrcCur & 0xffff);
							++pulSrcCur;
							*pulDestCur |= (*pulSrcCur << 16);
							++pulDestCur;

							// Two
							*pulDestCur = (*pulSrcCur >> 16);
							++pulSrcCur;
							*pulDestCur |= (*pulSrcCur << 16);
							++pulSrcCur;
							++pulDestCur;
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
}


#endif // __VIS_IMSRC_CONVERTCBITTOCBIT_INL
