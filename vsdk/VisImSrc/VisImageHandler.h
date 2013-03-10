// VisImageHandler.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#pragma once


typedef void (VISAPI *TVisPfnSetImage)(CVisImageBase& refimagebase,
		void *pvUser);


template<class TPixel>
class CVisImageHandler : public CVisImagePump<TPixel>
{
public:
	typedef CVisImage<TPixel> T_Image;


	CVisImageHandler(bool fBackgroundProcessing = false,
			UINT nMsgNotify = WM_USER + 1);
	virtual ~CVisImageHandler();


	HWND SetNotifyWindow(HWND hwndNotify);

	void SetPfnSetImage(TVisPfnSetImage pfnSetImage, void *pvUser);

	virtual bool ProcessImage(T_Image& refimage);

	const CVisImage<TPixel>& Image(void) const;
	CVisImage<TPixel>& Image(void);

	bool IsBackgroundProcessing(void) const;


protected:
	HWND m_hwndNotify;
	UINT m_nMsgNotify;
	TVisPfnSetImage m_pfnSetImage;
	void *m_pvUser;
	T_Image m_image;

	const bool m_fBackgroundProcessing;
};



#include "VisImageHandler.inl"