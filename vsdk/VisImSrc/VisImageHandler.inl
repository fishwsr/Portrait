// VisImageHandler.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


template<class TPixel>
inline CVisImageHandler<TPixel>::CVisImageHandler(bool fBackgroundProcessing,
		UINT nMsgNotify)
  : CVisImagePump<TPixel>(),
	m_hwndNotify(0),
	m_nMsgNotify(nMsgNotify),
	m_pfnSetImage(0),
	m_pvUser(0),
	m_fBackgroundProcessing(fBackgroundProcessing)
{
}

template<class TPixel>
inline CVisImageHandler<TPixel>::~CVisImageHandler()
{
}

template<class TPixel>
inline HWND CVisImageHandler<TPixel>::SetNotifyWindow(HWND hwndNotify)
{
	ASSERT(::IsWindow(hwndNotify));
	HWND hwndOld = m_hwndNotify;
	m_hwndNotify = hwndNotify;
	return hwndOld;
}

template<class TPixel>
inline void CVisImageHandler<TPixel>::SetPfnSetImage(
		TVisPfnSetImage pfnSetImage, void *pvUser)
{
	m_pfnSetImage = pfnSetImage;
	m_pvUser = pvUser;
}

template<class TPixel>
inline bool CVisImageHandler<TPixel>::ProcessImage(T_Image& refimage)
{
	// This method is used to copy the image to a local variable and
	// notify the document that a new image is available.  DO NOT DO
	// IMAGE PROCESSING FUNCTIONS IN THIS METHOD.  (Add image processing
	// functions in the CWT1Doc::SetImage method.)
	m_image = refimage;

	if ((m_fBackgroundProcessing) && (m_pfnSetImage != 0))
		m_pfnSetImage(refimage, m_pvUser);

	if (IsWindow(m_hwndNotify))
		::PostMessage(m_hwndNotify, m_nMsgNotify, 0, 0);

	return TRUE;
}

template<class TPixel>
inline const CVisImage<TPixel>& CVisImageHandler<TPixel>::Image(void) const
{
	return m_image;
}
template<class TPixel>
inline CVisImage<TPixel>& CVisImageHandler<TPixel>::Image(void)
{
	return m_image;
}

template<class TPixel>
inline bool CVisImageHandler<TPixel>::IsBackgroundProcessing(void) const
{
	return m_fBackgroundProcessing;
}





