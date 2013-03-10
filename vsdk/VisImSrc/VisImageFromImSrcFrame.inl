// VisImageFromImSrcFrame.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


inline int CVisImageFromImSrcFrame::CImEncodingSupported(void) const
{
	return m_cimencodingSupported;
}


inline EVisImEncoding CVisImageFromImSrcFrame::ImEncodingSupportedI(int i)
		const
{
	EVisImEncoding evisimencoding;
	
	if ((i < 0) || (i >= m_cimencodingSupported))
		evisimencoding = evisimencodingUnknown;
	else if (i == 0)
		evisimencoding = m_imencodingPreferred;
	else
		evisimencoding = m_rgimencodingSupported[i];

	return evisimencoding;
}

inline bool CVisImageFromImSrcFrame::FIsSupportedImEncoding(
		EVisImEncoding evisimencoding) const
{
	bool fRet = false;

	for (int i = 0; i < m_cimencodingSupported; ++i)
	{
		if (ImEncodingSupportedI(i) == evisimencoding)
		{
			fRet = true;
			break;
		}
	}

	return fRet;
}
