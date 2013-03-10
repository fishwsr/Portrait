// VisImEncoding.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// LATER:  Remove special case code for the IBM Thinkpad.


inline bool VisFCheckEVisImEncoding(void)
{
	static bool fCheck
			= (evisimencodingUnknown		== 0)
			&& (evisimencodingFirst			== 0)
			&& (evisimencodingBW1bit		== 1)
			&& (evisimencodingGray4bit		== 2)
			&& (evisimencodingColor4bit		== 3)
			&& (evisimencodingGray8bit		== 4)
			&& (evisimencodingColor8bit		== 5)
			&& (evisimencodingGray16bit		== 6)
			&& (evisimencodingRGB565		== 7)
			&& (evisimencodingRGB555		== 8)
			&& (evisimencodingRGB24			== 9)
			&& (evisimencodingRGBA32		== 10)
			&& (evisimencodingRGB48			== 11)
			&& (evisimencodingRGBA64		== 12)
			&& (evisimencodingYUV24			== 13)
			&& (evisimencodingYUVA32		== 14)
			&& (evisimencodingYUV48			== 15)
			&& (evisimencodingYUVA64		== 16)
			&& (evisimencodingUYVY			== 17)
			&& (evisimencodingYUY2			== 18)
			&& (evisimencodingYVYU			== 19)
			&& (evisimencodingY41P			== 20)
			&& (evisimencodingY211			== 21)
			&& (evisimencodingY41T			== 22)
			&& (evisimencodingY42T			== 23)
			&& (evisimencodingCLJR			== 24)
			&& (evisimencodingYVU9			== 25)
			&& (evisimencodingIF09			== 26)
			&& (evisimencodingYV12			== 27)
			&& (evisimencodingIYUV			== 28)
			&& (evisimencodingCLPL			== 29)
			&& (evisimencodingY210			== 30)
			&& (evisimencodingY410			== 31)
			&& (evisimencodingYUVP			== 32)
			&& (evisimencodingLim			== 33);

	return fCheck;
}


inline int VisCbitGetImEncoding(EVisImEncoding evisimencoding)
{
	assert(VisFCheckEVisImEncoding());
	assert((evisimencoding >= evisimencodingFirst)
			&& (evisimencoding < evisimencodingLim));
	
	static DWORD rgcbit[evisimencodingLim] =
	{
		0,		// evisimencodingUnknown
		1,		// evisimencodingBW1bit
		4,		// evisimencodingGray4bit
		4,		// evisimencodingColor4bi
		8,		// evisimencodingGray8bit
		8,		// evisimencodingColor8bi
		16,		// evisimencodingGray16bit
		16,		// evisimencodingRGB565
		16,		// evisimencodingRGB555
		24,		// evisimencodingRGB24
		32,		// evisimencodingRGBA32
		48,		// evisimencodingRGB48
		64,		// evisimencodingRGBA64
		24,		// evisimencodingYUV24
		32,		// evisimencodingYUVA32
		48,		// evisimencodingYUV48
		64,		// evisimencodingYUVA64
		16,		// evisimencodingUYVY
		16,		// evisimencodingYUY2
		16,		// evisimencodingYVYU
		12,		// evisimencodingY41P
		8,		// evisimencodingY211
		12,		// evisimencodingY41T
		16,		// evisimencodingY42T
		8,		// evisimencodingCLJR
		9,		// evisimencodingYVU9
		9,		// evisimencodingIF09
		12,		// evisimencodingYV12
		12,		// evisimencodingIYUV
		12,		// evisimencodingCLPL
		8,		// evisimencodingY210
		12,		// evisimencodingY410	
		16,		// evisimencodingYUVP
	};		
		
	return rgcbit[evisimencoding];
}

inline DWORD VisDwFourCCGetImEncoding(EVisImEncoding evisimencoding)
{
	assert(VisFCheckEVisImEncoding());
	assert((evisimencoding >= evisimencodingFirst)
			&& (evisimencoding < evisimencodingLim));
	
	static DWORD rgdwFourCC[evisimencodingLim] =
	{
		0,			// evisimencodingUnknown
		BI_RGB,		// evisimencodingBW1bit
		BI_RGB,		// evisimencodingGray4bit
		BI_RGB,		// evisimencodingColor4bi
		BI_RGB,		// evisimencodingGray8bit
		BI_RGB,		// evisimencodingColor8bi
		BI_RGB,		// evisimencodingGray16bit
		BI_RGB,		// evisimencodingRGB565
		BI_RGB,		// evisimencodingRGB555
		BI_RGB,		// evisimencodingRGB24
		BI_RGB,		// evisimencodingRGBA32
		BI_RGB,		// evisimencodingRGB48
		BI_RGB,		// evisimencodingRGBA64
		' VUY',		// evisimencodingYUV24
		'AVUY',		// evisimencodingYUVA32
		' VUY',		// evisimencodingYUV48
		'AVUY',		// evisimencodingYUVA64
		'YVYU',		// evisimencodingUYVY
		'2YUY',		// evisimencodingYUY2
		'UYVY',		// evisimencodingYVYU
		'P14Y',		// evisimencodingY41P
		'112Y',		// evisimencodingY211
		'T14Y',		// evisimencodingY41T
		'T24Y',		// evisimencodingY42T
		'RJLC',		// evisimencodingCLJR
		'9UVY',		// evisimencodingYVU9
		'90FI',		// evisimencodingIF09
		'21VY',		// evisimencodingYV12
		'VUYI',		// evisimencodingIYUV
		'LPLC',		// evisimencodingCLPL
		'012Y',		// evisimencodingY210
		'014Y',		// evisimencodingY410	
		'PVUY',		// evisimencodingYUVP
	};		
		
	return rgdwFourCC[evisimencoding];
}


inline const char *VisSzGetImEncoding(EVisImEncoding evisimencoding)
{
	assert(VisFCheckEVisImEncoding());
	assert((evisimencoding >= evisimencodingFirst)
			&& (evisimencoding < evisimencodingLim));
	
	static const char *rgszImEncoding[evisimencodingLim] =
	{
		"Unknown",		// evisimencodingUnknown
		"BW1bit",		// evisimencodingBW1bit
		"Gray4bit",		// evisimencodingGray4bit
		"Color4bit",	// evisimencodingColor4bit
		"Gray8bit",		// evisimencodingGray8bit
		"Color8bit",	// evisimencodingColor8bit
		"Gray16bit",	// evisimencodingGray16bit
		"RGB565",		// evisimencodingRGB565
		"RGB555",		// evisimencodingRGB555
		"RGB24",		// evisimencodingRGB24
		"RGBA32",		// evisimencodingRGBA32
		"RGB48",		// evisimencodingRGB24
		"RGBA64",		// evisimencodingRGBA32
		"YUV24",		// evisimencodingYUV24
		"YUVA32",		// evisimencodingYUVA32
		"YUV48",		// evisimencodingYUV48
		"YUVA64",		// evisimencodingYUVA64
		"UYVY",			// evisimencodingUYVY
		"YUY2",			// evisimencodingYUY2
		"YVYU",			// evisimencodingYVYU
		"Y41P",			// evisimencodingY41P
		"Y211",			// evisimencodingY211
		"Y41T",			// evisimencodingY41T
		"Y42T",			// evisimencodingY42T
		"CLJR",			// evisimencodingCLJR
		"YVU9",			// evisimencodingYVU9
		"IF09",			// evisimencodingIF09
		"YV12",			// evisimencodingYV12
		"IYUV",			// evisimencodingIYUV
		"CLPL",			// evisimencodingCLPL
		"Y210",			// evisimencodingY210
		"Y410",			// evisimencodingY410
		"YUVP",			// evisimencodingYUVP
	};

	return rgszImEncoding[evisimencoding];
}


inline EVisImEncoding VisImEncodingGetDwFourCC(DWORD dwFourCC, int cbit)
{
	int iImEncoding;

	if (dwFourCC == 'n2yt')
	{
		// Special case:  YUV2 on IBM Thinkpad
		assert(cbit == 16);
		iImEncoding = evisimencodingYUY2;
	}
	else if (dwFourCC == 'n0yt')
	{
		// Special case:  IYUV on IBM Thinkpad
		assert(cbit == 12);
		iImEncoding = evisimencodingIYUV;
	}
	else if ((dwFourCC == 'cyuv') || (dwFourCC == 'vuyc'))
	{
		// Special case:  UYVY with height reversed.
		assert(cbit == 16);
		iImEncoding = evisimencodingUYVY;
	}
	else if ((dwFourCC == 'I420') || (dwFourCC == '024I'))
	{
		// Special case:  I420 is the same as IYUV
		assert(cbit == 12);
		iImEncoding = evisimencodingIYUV;
	}
	else if (dwFourCC == BI_RGB)
	{
		switch (cbit)
		{
		case 1:
			iImEncoding = evisimencodingBW1bit;
			break;

		case 4:
			iImEncoding = evisimencodingColor4bit;
			break;

		case 8:
			iImEncoding = evisimencodingColor8bit;
			break;

		case 16:
			iImEncoding = evisimencodingRGB555;
			break;

		case 24:
			iImEncoding = evisimencodingRGB24;
			break;

		case 32:
			iImEncoding = evisimencodingRGBA32;
			break;

		case 48:
			iImEncoding = evisimencodingRGB48;
			break;

		case 64:
			iImEncoding = evisimencodingRGBA64;
			break;

		default:
			iImEncoding = evisimencodingUnknown;
			break;
		}
	}
	else if (dwFourCC == 'YUV')
	{
		switch (cbit)
		{
		case 24:
			iImEncoding = evisimencodingYUV24;
			break;

		case 32:
			iImEncoding = evisimencodingYUVA32;
			break;

		case 48:
			iImEncoding = evisimencodingYUV48;
			break;

		case 64:
			iImEncoding = evisimencodingYUVA64;
			break;

		default:
			iImEncoding = evisimencodingUnknown;
			break;
		}
	}
	else if (dwFourCC == 'YUVA')
	{
		switch (cbit)
		{
		case 32:
			iImEncoding = evisimencodingYUVA32;
			break;

		case 64:
			iImEncoding = evisimencodingYUVA64;
			break;

		default:
			iImEncoding = evisimencodingUnknown;
			break;
		}
	}
	else
	{
		assert(evisimencodingUnknown == evisimencodingFirst);

		iImEncoding = evisimencodingLim;
		while (--iImEncoding > evisimencodingFirst)
		{
			if (VisDwFourCCGetImEncoding((EVisImEncoding) iImEncoding)
					== dwFourCC)
			{
				break;
			}
		}
		
		if (iImEncoding == evisimencodingFirst)
		{
			// If we didn't find the FourCC value above, try switching
			// the order of the BYTEs in the DWORD.
			DWORD dwT = ((dwFourCC & 0x000000ff) << 24)
					| ((dwFourCC & 0x0000ff00) << 8)
					| ((dwFourCC & 0x00ff0000) >> 8)
					| ((dwFourCC & 0xff000000) >> 24);
			iImEncoding = evisimencodingLim;
			while (--iImEncoding > evisimencodingFirst)
			{
				if (VisDwFourCCGetImEncoding((EVisImEncoding) iImEncoding)
						== dwT)
				{
					break;
				}
			}
		}
	}

	return (EVisImEncoding) iImEncoding;
}


inline EVisImEncoding VisImEncodingGetSz(const char *sz)
{
	assert(sz != 0);

	int iImEncoding;

	if (strcmp(sz, "ty2n") == 0)
	{
		// Special case:  YUV2 on IBM Thinkpad
		iImEncoding = evisimencodingYUY2;
	}
	else if (strcmp(sz, "cyuv") == 0)
	{
		// Special case:  UYVY with height reversed.
		iImEncoding = evisimencodingUYVY;
	}
	else if (strcmp(sz, "I420") == 0)
	{
		// Special case:  I420 is the same as IYUV
		iImEncoding = evisimencodingIYUV;
	}
	else
	{
		assert(evisimencodingUnknown == evisimencodingFirst);

		iImEncoding = evisimencodingLim;
		while (--iImEncoding > evisimencodingFirst)
		{
			if (strcmpi(VisSzGetImEncoding((EVisImEncoding) iImEncoding), sz)
					== 0)
			{
				break;
			}
		}
	}

	return (EVisImEncoding) iImEncoding;
}

