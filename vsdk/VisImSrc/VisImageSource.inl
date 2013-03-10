// VisImageSource.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved



#ifndef __VIS_IMSRC_IMAGESOURCE_INL__
#define __VIS_IMSRC_IMAGESOURCE_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


inline bool CVisImageSource::HasDialog(EVisVidDlg evisviddlg) const
{
	assert(IsValid());
	
	bool fRet;

	if (m_pIVisImSrcDevice->HasDialog(evisviddlg) == S_OK)
		fRet = true;
	else
		fRet = false;

	return fRet;
}

inline long CVisImageSource::DoDialog(EVisVidDlg evisviddlg, HWND hwndParent)
{
	assert(IsValid());
	
	return m_pIVisImSrcDevice->DoDialog(evisviddlg, hwndParent);
}


inline long CVisImageSource::GetImSrcValue(EVisImSrc evisimsrc) const
{
	assert(IsValid());
	
	long lValue;
	
	if (FAILED(m_pIVisImSrcSettings->GetImSrcValue(evisimsrc, &lValue)))
	{
		lValue = 0;  // LATER:  Should we have a std error value, like 1 << 31?
	}

	return lValue;
}

inline bool CVisImageSource::SetImSrcValue(EVisImSrc evisimsrc, long lValue,
		long lFlags)
{
	assert(IsValid());
	
	bool fRet;

	if (m_pIVisImSrcSettings->SetImSrcValue(evisimsrc, lValue, lFlags) == S_OK)
		fRet = true;
	else
		fRet = false;

	return fRet;
}



inline bool CVisImageSource::AllowVolatileImages(void) const
{
	assert(IsValid());
	
	return m_pSequenceList->AllowVolatileImages();
}


inline void CVisImageSource::SetAllowVolatileImages(bool f)
{
	assert(IsValid());
	
	m_pSequenceList->SetAllowVolatileImages(f);
}



inline void CVisImageSource::GrabNextInBackground(void)
{
	assert(IsValid());
	
	m_pSequenceList->GrabNextInBackground();
}


inline const IVisImSrcDevice *CVisImageSource::PIVisImSrcDeviceGet(void) const
{
	return m_pIVisImSrcDevice;
}


inline IVisImSrcDevice *CVisImageSource::PIVisImSrcDeviceGet(void)
{
	return m_pIVisImSrcDevice;
}


inline const IVisImSrcSettings *CVisImageSource::PIVisImSrcSettings(void) const
{
	return m_pIVisImSrcSettings;
}


inline IVisImSrcSettings *CVisImageSource::PIVisImSrcSettings(void)
{
	return m_pIVisImSrcSettings;
}



#endif // !__VIS_IMSRC_IMAGESOURCE_INL__
