// VisDib.inl
//
// Copyright © 1999-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

inline CVisDIB::~CVisDIB(void) 
{

}

inline const BITMAPINFOHEADER& CVisDIB::BitmapInfoHeader(void) const
{
	return *((BITMAPINFOHEADER*)m_memblockHeader.PbData());
}

inline BITMAPINFOHEADER& CVisDIB::BMIH(void)
{
	return *((BITMAPINFOHEADER*)m_memblockHeader.PbData());
}

inline const CVisMemBlock& CVisDIB::MemBlockPixels(void) const
{
	return m_memblockPixels;
}

inline const void* CVisDIB::PvPixels(void) const
{
	return m_memblockPixels.PbData();
}

inline void* CVisDIB::PvPixels(void)
{
	return m_memblockPixels.PbData();
}

inline long CVisDIB::Width(void) const
{
	return BitmapInfoHeader().biWidth;
}

inline long CVisDIB::Height(void) const
{
	return BitmapInfoHeader().biHeight;
}

inline DWORD CVisDIB::Compression(void) const
{
	return BitmapInfoHeader().biCompression;
}

inline int CVisDIB::CbitPixel(void) const
{
	return BitmapInfoHeader().biBitCount;
}

inline const RGBQUAD* CVisDIB::ColorMap() const
{
	return (const RGBQUAD *)(m_memblockHeader.PbData() + BitmapInfoHeader().biSize);
}

inline RGBQUAD CVisDIB::Color(int iColor) const
{
	assert((void*)(ColorMap() + iColor + 1) <= 
					m_memblockHeader.PbData() + m_memblockHeader.CbData());
	return ColorMap()[iColor];
}

inline HIC CVisDIB::HDecompressor(const BITMAPINFOHEADER *pbmih) const
{
	HIC hic = ICLocate(ICTYPE_VIDEO, 0L, (BITMAPINFOHEADER *) &BitmapInfoHeader(), 
						(BITMAPINFOHEADER *) pbmih, ICMODE_DECOMPRESS);
	return hic;
}

inline void CVisDIB::CloseHDecompressor(HIC hicDecompressor)
{
	if(hicDecompressor != 0)
		ICClose(hicDecompressor);
}

inline bool CVisDIB::IsGray8bit() const
{
	return m_fGray8bit;
}