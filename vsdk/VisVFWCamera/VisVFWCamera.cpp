////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWCAMERA 
//
// @module VisVFWCamera.cpp |
//
// This file implements the <c CVisVFWCamera> class.  It is derived from
// the <c IVisImSrcSettings> and <c IVisImSrcDevice> interfaces.
// It is used to work with (adjust settings for and get images from)
// a VFW camera.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisVFWCamera\.h>
//
////////////////////////////////////////////////////////////////////////////



#include <process.h>

#include "VisVFWCamPch.h"
#include "VisVFWCamera.h"


CVisVFWCamera::CVisVFWCamera(int iCaptureDevice, bool fOwnThread)
  : m_cRef(1),
	m_strID(),
	m_iCaptureDevice(iCaptureDevice),
	m_evisimencoding(evisimencodingUnknown),
	m_evisconvyuv(evisconvyuvDefault),
	m_evisimencodingGrabCur(evisimencodingUnknown),
	m_hwndPreview(0),
	m_pfnCallback(0),
	m_pvUser(0),
	m_cdwFourCCUnknownCur(0),
	m_fInContinuousGrab(false),
	m_fInSingleFrameGrab(false),
	m_fFlippedImage(true),
	m_fUseNegHtToFlip(false),
	m_fCanSetVideoFormat(true),
	m_hSemaphoreHwndInitialized(0),
	m_hThread(0),
	m_dwThreadID(0)
{
	for (int i = 0; i < evisimencodingLim; ++i)
	{
		m_rgdwFourCCImEncoding[i] = 0;
		m_rgeflipImEncoding[i] = eflipUnknown;
		m_rgfHaveImEncoding[i] = false;
	}
	
	if ((iCaptureDevice >= 0) && (iCaptureDevice <= 9))
	{
		m_strID = g_szVFWProviderID;
		m_strID += ":  ";
		assert(ecchProviderSep == 3);

		char szT[256];
		*szT = 0;
		if ((!capGetDriverDescription(iCaptureDevice, szT, sizeof(szT), 0, 0))
				|| (*szT == 0))
		{
			assert((iCaptureDevice >= 0) && (iCaptureDevice <= 9));
			szT[0] = 'V';
			szT[1] = 'F';
			szT[2] = 'W';
			szT[3] = '0' + iCaptureDevice;
			szT[4] = 0;
		}

		m_strID += szT;

		ZeroMemory(&m_capstatus, sizeof(m_capstatus));

		if (fOwnThread)
		{
			m_hSemaphoreHwndInitialized = CreateSemaphore(0, 0, 1, 0);

			unsigned int uiThreadID;
			m_hThread = (HANDLE) _beginthreadex(0, 0, StaticThreadMain, this, 0,
					&uiThreadID);
			m_dwThreadID = uiThreadID;

			if (m_hThread != 0)
			{
				WaitForSingleObject(m_hSemaphoreHwndInitialized, INFINITE);
			}

			CloseHandle(m_hSemaphoreHwndInitialized);
			m_hSemaphoreHwndInitialized = 0;
		}
		else
		{
			CreateCaptureWindow();
		}

		if ((m_hwndPreview != 0)
				&& (!capDriverConnect(m_hwndPreview, iCaptureDevice)))
		{
			DestroyCaptureWindow();

			if (m_hThread != 0)
			{
				// Kill it.
				::PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
				WaitForSingleObject(m_hThread, INFINITE);
				CloseHandle(m_hThread);
				m_hThread = 0;
			}
		}
	}
}

CVisVFWCamera::~CVisVFWCamera(void)
{
	if (m_hwndPreview != 0)
	{
		if (m_fInContinuousGrab)
		{
			// Stop capture.
			m_fInContinuousGrab = false;
			capCaptureAbort(m_hwndPreview);

			// Wait for camera to stop capture.
			// (We shouldn't need to call Sleep here, but we do it to
			// avoid access violations with some camera drivers.)
			Sleep(1000);
		}
		capDriverDisconnect(m_hwndPreview);
		DestroyCaptureWindow();
	}

	if (m_hThread != 0)
	{
		// Kill it.
		::PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = 0;
	}
}

STDMETHODIMP CVisVFWCamera::QueryInterface(THIS_ REFIID riid, void **ppvObject)
{
	HRESULT hrRet;

	if (ppvObject == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if ((riid == IID_IUnknown)
				|| (riid == IID_IVisImSrcSettings))
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = (IVisImSrcSettings *) this;
		AddRef();
	}
	else if (riid == IID_IVisImSrcDevice)
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = (IVisImSrcDevice *) this;
		AddRef();
	}
	else
	{
		*ppvObject = 0;
		hrRet = ResultFromScode(E_NOINTERFACE);
	}

	return hrRet;
}

STDMETHODIMP_(ULONG) CVisVFWCamera::AddRef(THIS)
{
	assert(m_cRef > 0);

	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CVisVFWCamera::Release(THIS)
{
	unsigned long cRef = --m_cRef;

	if (cRef == 0)
		delete this;

	return cRef;
}

// Returns S_OK if successful.
HRESULT CVisVFWCamera::Init(bool fFullInit)
{
	HRESULT hrRet = ResultFromScode(E_FAIL);

	if (m_hwndPreview != 0)
	{
		if (!fFullInit)
		{
			hrRet = ResultFromScode(S_OK);
		}
		else
		{
			capDriverGetCaps(m_hwndPreview, &m_capdrivercaps,
					sizeof(m_capdrivercaps));

			SetupCaptureParams();

			if (!FReadSettings())
			{
				// if we can't read the settings (or determine the type of images
				// that we're getting), fail the initialization.
				hrRet = ResultFromScode(E_FAIL);
			}
			else
			{
				UpdateCachedInfo();

				// Add the pointer to this object to the preview window.
				capSetUserData(m_hwndPreview, (long) this);

				// Set the frame callback function.
				capSetCallbackOnFrame(m_hwndPreview, StaticFrameCallbackProc);
				capSetCallbackOnVideoStream(m_hwndPreview, StaticFrameCallbackProc);
			
				hrRet = ResultFromScode(S_OK);
			}
		}
	}

	return hrRet;
}


// Get a string identifying this device.
// Returns the number of bytes copied in pcb if successful.
STDMETHODIMP CVisVFWCamera::GetSzID(THIS_ char *sz, LONG *pcb)
{
	return VisGetSzPcbFromSz(sz, pcb, m_strID.c_str());
}

const char *CVisVFWCamera::SzName(void) const
{
	int cchPrefix = strlen(g_szVFWProviderID) + ecchProviderSep;
	assert(*(m_strID.c_str() + strlen(g_szVFWProviderID)) == ':');
	assert(*(m_strID.c_str() + cchPrefix - 1) == ' ');
	assert(*(m_strID.c_str() + cchPrefix) != ' ');

	return m_strID.c_str() + cchPrefix;
}

STDMETHODIMP CVisVFWCamera::GetSzName(THIS_ char *sz, LONG *pcb)
{
	return VisGetSzPcbFromSz(sz, pcb, SzName());
}

STDMETHODIMP CVisVFWCamera::SaveSettings(THIS_ bool fForAllApps)
{
	HRESULT hrRet = ResultFromScode(E_FAIL);

	// Find the registry key.
	std::string strKey = VIS_SZ_REG_IMSRC_DEVS  "\\";
	strKey += g_szVFWProviderID;
	strKey += "\\";
	strKey += SzName();

#ifdef LATER
	// This code is not needed now, but could be used to get an App ID
	// to use in app-specific keys.
	char szFullAppName[MAX_PATH];
	const char *szAppID = 0;
	if (GetModuleFileName(0, szFullAppName, sizeof(szFullAppName)) != 0)
	{
		szAppID = strrchr(szFullAppName, '\\');
		if (szAppID != 0)
			++ szAppID;
		else
			szAppID = szFullAppName;
	}
#endif // LATER

	// Attempt to open the reg key.
	HKEY hkey;
	if (VisFCreateRegKey(&hkey, strKey.c_str()))
	{
		std::string strVal;

		// First, write information about any unknown dwFourCC values that
		// we found.  (Write this even if we don't write any other setting
		// information.)
		for (int iUnknown = 0; iUnknown < m_cdwFourCCUnknownCur; ++ iUnknown)
		{
			char szNumUnknown[4];
			assert(iUnknown < 10);
			szNumUnknown[0] = '0' + iUnknown;
			szNumUnknown[1] = 0;

			strVal = "UnknownFourCC";
			strVal += szNumUnknown;
			VisFWriteDwordToReg(m_rgdwFourCCUnknown[iUnknown], hkey,
					strVal.c_str());

			strVal = "UnknownCbit";
			strVal += szNumUnknown;
			VisFWriteIntToReg(m_rgcbitFourCCUnknown[iUnknown], hkey,
					strVal.c_str());
		}

		bool fFoundImEncoding = false;
		for (int i = evisimencodingFirst; i < evisimencodingLim; ++ i)
		{
			if ((i == evisimencodingUnknown)
					|| (!m_rgfHaveImEncoding[i]))
			{
				continue;
			}

			assert(VisSzGetImEncoding((EVisImEncoding) i) != 0);

			fFoundImEncoding = true;

			strVal = "EVisImEncoding";
			strVal += VisSzGetImEncoding((EVisImEncoding) i);
			VisFWriteDwordToReg(m_rgdwFourCCImEncoding[i], hkey,
					strVal.c_str());

			assert((m_rgeflipImEncoding[i] == eflipUnknown)
					|| (m_rgeflipImEncoding[i] == eflipNo)
					|| (m_rgeflipImEncoding[i] == eflipYes));

			if (m_rgeflipImEncoding[i] != eflipUnknown)
			{
				strVal = "Flipped";
				strVal += VisSzGetImEncoding((EVisImEncoding) i);
				bool f = ((m_rgeflipImEncoding[i] == eflipNo) ? false : true);
				VisFWriteBoolToReg(f, hkey, strVal.c_str());
			}
		}

		if (!fFoundImEncoding)
		{
			// The camera was not properly initialized, so no settings
			// will be written.
			hrRet = ResultFromScode(S_FALSE);
		}
		else
		{
			hrRet = ResultFromScode(S_OK);

			VisFWriteDwordToReg(m_capstatus.uiImageWidth, hkey, "Width");
			VisFWriteDwordToReg(m_capstatus.uiImageHeight, hkey, "Height");

			VisFWriteBoolToReg(m_fFlippedImage, hkey, "Flipped");
			VisFWriteBoolToReg(m_fUseNegHtToFlip, hkey, "UseNegHeightToFlip");
			VisFWriteBoolToReg(m_fCanSetVideoFormat, hkey, "CanSetVideoFormat");

			VisFWriteIntToReg(m_evisconvyuv, hkey, "EVisConvYUV");

			assert(m_evisimencoding != evisimencodingUnknown);
			VisFWriteIntToReg(m_evisimencoding, hkey, "EVisImEncoding");
		}

		VisFCloseRegKey(hkey);
	}

	return hrRet;
}


// Properties of this image source.
STDMETHODIMP CVisVFWCamera::GetImSrcRange(THIS_ EVisImSrc evisimsrc, LONG *plMin,
		LONG *plMax, LONG *pdlStepping, LONG *plDefault, LONG *plFlags) CONST
{
	HRESULT hrRet = ResultFromScode(S_OK);

	LONG lMin = 0;
	LONG lMax = 0;
	LONG dlStepping = 1;
	LONG lDefault = 0;
	LONG lFlags = 0;

	switch (evisimsrc)
	{
	case evisimsrcFContinuousGrab:
		lMax = 1;
		dlStepping = 1;
		lDefault = 0;
		lFlags = evisimsrcflagAuto;
		hrRet = ResultFromScode(S_OK);
		break;

	case evisimsrcFReflectVert:
		lMax = 1;
		dlStepping = 1;
		lDefault = 1;
		lFlags = evisimsrcflagAuto;
		hrRet = ResultFromScode(S_OK);
		break;

	case evisimsrcWidth:
		// Assume that VGA is the best that we can do.
		lMax = 640;
		lFlags = evisimsrcflagAuto;
		break;

	case evisimsrcHeight:
		// Assume that VGA is the best that we can do.
		lMax = 480;
		lFlags = evisimsrcflagAuto;
		break;

	case evisimsrcImEncoding:
		lMax = evisimencodingLim - 1;
		lFlags = evisimsrcflagAuto;
		break;

	case evisimsrcNBuffers:
		lMin = m_capstatus.wNumVideoAllocated;
		lMax = m_capstatus.wNumVideoAllocated;
		break;

	default:
		hrRet = ResultFromScode(E_FAIL);
		break;
	}

	if (plMin != 0)
		*plMin = lMin;
	if (plMax != 0)
		*plMax = lMax;
	if (pdlStepping != 0)
		*pdlStepping = dlStepping;
	if (plDefault != 0)
		*plDefault = lDefault;
	if (plFlags != 0)
		*plFlags = lFlags;

	return hrRet;
}

STDMETHODIMP CVisVFWCamera::GetImSrcValue(THIS_ EVisImSrc evisimsrc, LONG *plValue)
		CONST
{
	HRESULT hrRet = ResultFromScode(S_OK);
	LONG lValue;

	switch (evisimsrc)
	{
	case evisimsrcFContinuousGrab:
		lValue = ((m_fInContinuousGrab) ? 1 : 0);
		break;

	case evisimsrcFReflectVert:
		lValue = ((m_fFlippedImage) ? 1 : 0);
		hrRet = ResultFromScode(S_OK);
		break;

	case evisimsrcWidth:
		lValue = (int) m_capstatus.uiImageWidth;
		break;

	case evisimsrcHeight:
		lValue = (int) m_capstatus.uiImageHeight;
		break;

	case evisimsrcImEncoding:
		lValue = m_evisimencoding;
		break;

	case evisimsrcNBuffers:
		lValue = m_capstatus.wNumVideoAllocated;
		break;

	default:
		lValue = 0;
		hrRet = ResultFromScode(E_FAIL);
		break;
	}

	if (plValue != 0)
		*plValue = lValue;

	return hrRet;
}


STDMETHODIMP CVisVFWCamera::SetImSrcValue(THIS_ EVisImSrc evisimsrc, LONG lValue,
		LONG lFlags)
{
	// LATER:  Handle the manual configuration flag differently?
	HRESULT hrRet;

	switch (evisimsrc)
	{
	case evisimsrcFContinuousGrab:
		if (m_fInContinuousGrab)
		{
			if (lValue)
			{
				hrRet = ResultFromScode(S_FALSE);
			}
			else
			{
				// Stop capture.
				m_fInContinuousGrab = false;
				capCaptureAbort(m_hwndPreview);

				hrRet = ResultFromScode(S_OK);
			}
		}
		else
		{
			if (!lValue)
			{
				hrRet = ResultFromScode(S_FALSE);
			}
			else
			{
				// Start capture.
				m_fInContinuousGrab = true;
				if (!capCaptureSequenceNoFile(m_hwndPreview))
					m_fInContinuousGrab = false;

				if (m_fInContinuousGrab)
				{
					m_evisimencodingGrabCur = m_evisimencoding;
					hrRet = ResultFromScode(S_OK);
				}
				else
				{
					hrRet = ResultFromScode(E_FAIL);
				}
			}
		}
		break;

	case evisimsrcFReflectVert:
		hrRet = ResultFromScode(E_FAIL);
		if (m_fUseNegHtToFlip)
		{
			if (m_fFlippedImage)
			{
				if (lValue)
				{
					hrRet = ResultFromScode(S_FALSE);
				}
				else
				{
					BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();

					if (pbmih != 0)
					{
						assert(pbmih->biHeight >= 0);
						pbmih->biHeight = - pbmih->biHeight;

						if ((m_fCanSetVideoFormat)
								&& (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
										sizeof(BITMAPINFOHEADER))))
						{
							FreePbmi((BITMAPINFO *) pbmih);
							pbmih = (BITMAPINFOHEADER *) PbmiGet();
							if (pbmih->biHeight < 0)
							{
								m_fFlippedImage = false;
								hrRet = ResultFromScode(S_OK);
							}
						}
						
						FreePbmi((BITMAPINFO *) pbmih);
					}
				}
			}
			else
			{
				if (lValue == 0)
				{
					hrRet = ResultFromScode(S_FALSE);
				}
				else
				{
					BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();

					if (pbmih != 0)
					{
						assert(pbmih->biHeight <= 0);
						pbmih->biHeight = - pbmih->biHeight;

						if ((m_fCanSetVideoFormat)
								&& (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
										sizeof(BITMAPINFOHEADER))))
						{
							FreePbmi((BITMAPINFO *) pbmih);
							pbmih = (BITMAPINFOHEADER *) PbmiGet();
							if (pbmih->biHeight > 0)
							{
								m_fFlippedImage = true;
								hrRet = ResultFromScode(S_OK);
							}
						}
						
						FreePbmi((BITMAPINFO *) pbmih);
					}
				}
			}
		}
		break;

	case evisimsrcWidth:
		hrRet = ResultFromScode(E_FAIL);
		if (lValue >= 0)
		{
			BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();

			if (pbmih != 0)
			{
				pbmih->biWidth = lValue;

				if ((m_fCanSetVideoFormat)
						&& (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
								sizeof(BITMAPINFOHEADER))))
				{
					FreePbmi((BITMAPINFO *) pbmih);
					pbmih = (BITMAPINFOHEADER *) PbmiGet();
					if (pbmih->biWidth == lValue)
					{
						hrRet = ResultFromScode(S_OK);
					}
				}
				
				FreePbmi((BITMAPINFO *) pbmih);
			}
		}

		UpdateCachedInfo();
		break;

	case evisimsrcHeight:
		hrRet = ResultFromScode(E_FAIL);
		if (lValue >= 0)
		{
			BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();

			if (pbmih != 0)
			{
				if (pbmih->biHeight >= 0)
					pbmih->biHeight = lValue;
				else
					pbmih->biHeight = - lValue;

				if ((m_fCanSetVideoFormat)
						&& (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
								sizeof(BITMAPINFOHEADER))))
				{
					FreePbmi((BITMAPINFO *) pbmih);
					pbmih = (BITMAPINFOHEADER *) PbmiGet();
					if ((pbmih->biHeight == lValue)
							|| (pbmih->biHeight == - lValue))
					{
						hrRet = ResultFromScode(S_OK);
					}
				}
				
				FreePbmi((BITMAPINFO *) pbmih);
			}
		}

		UpdateCachedInfo();
		break;

	case evisimsrcImEncoding:
		hrRet = ResultFromScode(E_FAIL);

		if ((lValue != evisimencodingUnknown)
				&& (lValue >= evisimencodingFirst)
				&& (lValue < evisimencodingLim)
				&& (m_rgfHaveImEncoding[lValue]))
		{
			BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();

			if (pbmih != 0)
			{
				if (lValue == ImEncodingGetDwFourCC(pbmih->biCompression,
								pbmih->biBitCount))
				{
					hrRet = ResultFromScode(S_FALSE);
				}
				else
				{
					pbmih->biBitCount = VisCbitGetImEncoding((EVisImEncoding) lValue);
					pbmih->biCompression = DwFourCCGetImEncoding((EVisImEncoding) lValue);
					pbmih->biClrUsed = 0;
					pbmih->biClrImportant = 0;

					if ((m_fCanSetVideoFormat)
							&& (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
									sizeof(BITMAPINFOHEADER))))
					{
						FreePbmi((BITMAPINFO *) pbmih);
						pbmih = (BITMAPINFOHEADER *) PbmiGet();
						if (lValue == ImEncodingGetDwFourCC(pbmih->biCompression,
								pbmih->biBitCount))
						{
							hrRet = ResultFromScode(S_OK);
						}
					}
					
					FreePbmi((BITMAPINFO *) pbmih);
				}
			}
		}

		UpdateCachedInfo();
		break;

	default:
		hrRet = ResultFromScode(E_FAIL);
		break;
	}

	return hrRet;
}

STDMETHODIMP CVisVFWCamera::IsPreferredImEncoding (THIS_
		EVisImEncoding evisimencoding) CONST
{
	if ((evisimencoding != evisimencodingUnknown)
			&& (evisimencoding >= evisimencodingFirst)
			&& (evisimencoding < evisimencodingLim)
			&& (m_rgfHaveImEncoding[evisimencoding]))
	{
		return ResultFromScode(S_OK);
	}

	return ResultFromScode(S_FALSE);
}

STDMETHODIMP CVisVFWCamera::IsSupportedImEncoding (THIS_
		EVisImEncoding evisimencoding) CONST
{
	return IsPreferredImEncoding(evisimencoding);
}


STDMETHODIMP CVisVFWCamera::HasDialog(THIS_ EVisVidDlg evisviddlg)
{
	HRESULT hrRet;

	if (((evisviddlg == evisviddlgVFWCompression))
			|| ((evisviddlg == evisviddlgVFWDisplay)
				&& (m_capdrivercaps.fHasDlgVideoDisplay))
			|| ((evisviddlg == evisviddlgVFWFormat)
				&& (m_capdrivercaps.fHasDlgVideoFormat))
			|| ((evisviddlg == evisviddlgVFWSource)
				&& (m_capdrivercaps.fHasDlgVideoSource)))
	{
		hrRet = ResultFromScode(S_OK);
	}
	else
	{
		hrRet = ResultFromScode(S_FALSE);
	}

	return hrRet;
}

STDMETHODIMP_(LONG) CVisVFWCamera::DoDialog(THIS_ EVisVidDlg evisviddlg,
		HWND hwndParent)
{
	HRESULT hrRet;

	if (HasDialog(evisviddlg) == ResultFromScode(S_OK))
	{
		if (hwndParent == 0)
			hwndParent = GetForegroundWindow();

		SetForegroundWindow(m_hwndPreview);

		switch (evisviddlg)
		{
		case evisviddlgVFWCompression:
			if (capDlgVideoCompression(m_hwndPreview))
				hrRet = ResultFromScode(S_OK);
			else
				hrRet = ResultFromScode(S_FALSE);
			break;

		case evisviddlgVFWDisplay:
			if (capDlgVideoDisplay(m_hwndPreview))
				hrRet = ResultFromScode(S_OK);
			else
				hrRet = ResultFromScode(S_FALSE);
			break;

		case evisviddlgVFWFormat:
			if (capDlgVideoFormat(m_hwndPreview))
				hrRet = ResultFromScode(S_OK);
			else
				hrRet = ResultFromScode(S_FALSE);
			break;

		case evisviddlgVFWSource:
			if (capDlgVideoSource(m_hwndPreview))
				hrRet = ResultFromScode(S_OK);
			else
				hrRet = ResultFromScode(S_FALSE);
			break;
		}

		UpdateCachedInfo();

		if (hwndParent != 0)
			SetForegroundWindow(hwndParent);
	}
	else
	{
		hrRet = ResultFromScode(E_FAIL);
	}

	return hrRet;
}


// Attempt to specify a buffer to use to store image data.
STDMETHODIMP CVisVFWCamera::AddBuffer(THIS_ void *pvBuffer,
		unsigned long cbBuffer, VisImSrcMemCallback pfnDeleteBuffer,
		void *pvDeleteBuffer)
{
	// We can't specify the buffers used by VFW.
	return ResultFromScode(E_FAIL);
}


// May wait until we get a new image if (imageMostRecent == imageCur).
// If the buffer was provided by the caller by using the AddBuffer call,
// the caller is responsible for freeing the buffer.
STDMETHODIMP CVisVFWCamera::GrabNext(THIS)
{
	HRESULT hrRet;

	if (m_fInContinuousGrab)
	{
		// LATER:  Block until we get the next frame?
		hrRet = ResultFromScode(S_FALSE);
	}
	else
	{
		// Need to grab another frame.
		m_fInSingleFrameGrab = true;
		m_evisimencodingGrabCur = m_evisimencoding;
		capGrabFrameNoStop(m_hwndPreview);
		m_fInSingleFrameGrab = false;

		hrRet = ResultFromScode(S_OK);
	}

	return hrRet;
}


void CVisVFWCamera::SetupCaptureParams(void)
{
	// These settings are not stored in the registry because the user can't
	// modify them.

	// Set the preview rate in case we want to support previews in the future.
	// (Previews are not currently supported.)
	capPreviewRate(m_hwndPreview, 33);

	// Attempt to get the capture parameters.
	capDriverGetCaps(m_hwndPreview, &m_capdrivercaps,
			sizeof(m_capdrivercaps));

	// Default values.
	m_captureparms.dwRequestMicroSecPerFrame = 33333;
//	m_captureparms.dwRequestMicroSecPerFrame = 500000;
	m_captureparms.fMakeUserHitOKToCapture = FALSE;
	m_captureparms.wPercentDropForError = 100;
	m_captureparms.fYield = TRUE;
	m_captureparms.dwIndexSize = 0;
	m_captureparms.wChunkGranularity = 0;
	m_captureparms.fUsingDOSMemory = FALSE;
	m_captureparms.wNumVideoRequested = 3;
	m_captureparms.fCaptureAudio = FALSE;
	m_captureparms.wNumAudioRequested = 0;
	m_captureparms.vKeyAbort = 0;
	m_captureparms.fAbortLeftMouse = FALSE;
	m_captureparms.fAbortRightMouse = FALSE;
	m_captureparms.fLimitEnabled = FALSE;
	m_captureparms.wTimeLimit = 0;
	m_captureparms.fMCIControl = FALSE;
	m_captureparms.fStepMCIDevice = FALSE;
	m_captureparms.dwMCIStartTime = 0;
	m_captureparms.dwMCIStopTime = 0;
	m_captureparms.fStepCaptureAt2x = FALSE;
	m_captureparms.wStepCaptureAverageFrames = 5;
	m_captureparms.dwAudioBufferSize = 0;
	m_captureparms.fDisableWriteCache = FALSE;
	m_captureparms.AVStreamMaster = 0;

	// Attempt to set the capture parameters.
	capCaptureSetSetup(m_hwndPreview, &m_captureparms,
			sizeof(m_captureparms));

	// Make sure that the values we have are correct.
	capCaptureGetSetup(m_hwndPreview, &m_captureparms,
			sizeof(m_captureparms));
}


bool CVisVFWCamera::FReadSettings(void)
{
	bool fRet = false;
	
	for (int i = 0; i < evisimencodingLim; ++i)
	{
		m_rgdwFourCCImEncoding[i] = 0;
		m_rgeflipImEncoding[i] = eflipUnknown;
		m_rgfHaveImEncoding[i] = false;
	}

	// Find the registry key.
	std::string strKey = VIS_SZ_REG_IMSRC_DEVS  "\\";
	strKey += g_szVFWProviderID;
	strKey += "\\";
	strKey += SzName();

#ifdef LATER
	// This code is not needed now, but could be used to get an App ID
	// to use in app-specific keys.
	char szFullAppName[MAX_PATH];
	const char *szAppID = 0;
	if (GetModuleFileName(0, szFullAppName, sizeof(szFullAppName)) != 0)
	{
		szAppID = strrchr(szFullAppName, '\\');
		if (szAppID != 0)
			++ szAppID;
		else
			szAppID = szFullAppName;
	}
#endif // LATER

	// Attempt to open the reg key.
	HKEY hkey;
	if (!VisFOpenRegKey(&hkey, strKey.c_str()))
	{
		fRet = FFindImEncodings();
	}
	else
	{
		DWORD dwT;

		if (!VisFReadBoolFromReg(&m_fCanSetVideoFormat, hkey, "CanSetVideoFormat"))
		{
			// Default assumption is that we can set the video format.
			m_fCanSetVideoFormat = true;
		}

		if (!VisFReadBoolFromReg(&m_fFlippedImage, hkey, "Flipped"))
		{
			// Default assumption is that the image is flipped.
			m_fFlippedImage = true;
		}

		if (!VisFReadBoolFromReg(&m_fUseNegHtToFlip, hkey, "UseNegHeightToFlip"))
		{
			// Default assumption is that we can't use negative height to filp
			// images.
			m_fUseNegHtToFlip = false;
		}

		if ((VisFReadDwordFromReg(&dwT, hkey, "EVisConvYUV"))
				&& (dwT >= evisconvyuvFirst)
				&& (dwT < evisconvyuvLim))
		{
			m_evisconvyuv = (EVisConvYUV) dwT;
		}
		else
		{
			m_evisconvyuv = evisconvyuvDefault;
		}

		int dx;
		bool fHaveWidth = false;
		if (VisFReadIntFromReg(&dx, hkey, "Width")
				&& (dx > 0) && (dx < 5000))
		{
			fHaveWidth = true;
		}
		else
		{
			// Use 320 as the default width.
			dx = 320;
		}

		int dy;
		if (VisFReadIntFromReg(&dy, hkey, "Height")
				&& (dy > 0) && (dy < 5000))
		{
			if (!fHaveWidth)
				dx = 6400 * dy / 4800;
		}
		else if (fHaveWidth)
		{
			dy = 4800 * dx / 6400;
		}
		else
		{
			// Use 320 as the default width.
			dx = 320;
		}

		if ((VisFReadDwordFromReg(&dwT, hkey, "EVisImEncoding"))
				&& (dwT >= evisimencodingFirst)
				&& (dwT != evisimencodingUnknown)
				&& (dwT < evisimencodingLim))
		{
			m_evisimencoding = (EVisImEncoding) dwT;
		}

		// We'll need to get a BITMAPINFOHEADER to set the width, height,
		// and EVisImEncoding.
		int cbVideoFormat = capGetVideoFormatSize(m_hwndPreview);
		BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();
		if (pbmih == 0)
		{
			fRet = FFindImEncodings();
		}
		else
		{
			if (((int) pbmih->biHeight) < 0)
				m_fUseNegHtToFlip = true;

			bool fFoundImEncoding = false;
			std::string strVal;
			for (int i = evisimencodingFirst; i < evisimencodingLim; ++ i)
			{
				if (i == evisimencodingUnknown)
					continue;

				assert(VisSzGetImEncoding((EVisImEncoding) i) != 0);

				strVal = "EVisImEncoding";
				strVal += VisSzGetImEncoding((EVisImEncoding) i);
				if (VisFReadDwordFromReg(&dwT, hkey, strVal.c_str()))
				{
					fFoundImEncoding = true;
					m_rgfHaveImEncoding[i] = true;
					m_rgdwFourCCImEncoding[i] = dwT;

					strVal = "Flipped";
					strVal += VisSzGetImEncoding((EVisImEncoding) i);
					bool fFlipped;
					if (VisFReadBoolFromReg(&fFlipped, hkey, strVal.c_str()))
					{
						m_rgeflipImEncoding[i] = (fFlipped ? eflipYes : eflipNo);
					}
					else
					{
						m_rgeflipImEncoding[i] = eflipUnknown;
					}
				}
			}

			if (!fFoundImEncoding)
				fFoundImEncoding = FFindImEncodings();

			fRet = fFoundImEncoding;

			if (fRet)
			{
				bool fChangedVideoFormat = false;

				if (pbmih->biWidth != (DWORD) dx)
				{
					pbmih->biWidth = (DWORD) dx;
					fChangedVideoFormat = true;
				}

				if ((pbmih->biHeight != (DWORD) dy)
						&& (pbmih->biHeight != - (DWORD) dy))
				{
					if ((m_fUseNegHtToFlip) && (!m_fFlippedImage))
						pbmih->biHeight = - (DWORD) dy;
					else
						pbmih->biHeight = (DWORD) dy;
					fChangedVideoFormat = true;
				}

				if (m_fUseNegHtToFlip)
				{
					if (m_fFlippedImage)
					{
						if (((int) pbmih->biHeight) < 0)
						{
							pbmih->biHeight = - pbmih->biHeight;
							fChangedVideoFormat = true;
						}
					}
					else
					{
						if (((int) pbmih->biHeight) > 0)
						{
							pbmih->biHeight = - pbmih->biHeight;
							fChangedVideoFormat = true;
						}
					}
				}

				if (m_evisimencoding == evisimencodingUnknown)
				{
					m_evisimencoding = ImEncodingGetDwFourCC(
							pbmih->biCompression, pbmih->biBitCount);
				}
				else if (m_evisimencoding != ImEncodingGetDwFourCC(
						pbmih->biCompression, pbmih->biBitCount))
				{
					pbmih->biBitCount = VisCbitGetImEncoding(m_evisimencoding);
					pbmih->biCompression
							= DwFourCCGetImEncoding(m_evisimencoding);
					pbmih->biClrUsed = 0;
					pbmih->biClrImportant = 0;
					fChangedVideoFormat = true;
				}

				// Attempt to use the new settings.
				if ((m_fCanSetVideoFormat) && (fChangedVideoFormat))
				{
					capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
							cbVideoFormat);
				}
			}

			// Clean up memory allocated above.
			FreePbmi((BITMAPINFO *) pbmih);
			pbmih = 0;
		}

		VisFCloseRegKey(hkey);
	}

	return fRet;
}

// Set a callback function to be called when new frames arrive during
// a continuous grab operation.
STDMETHODIMP CVisVFWCamera::GetFrameCallback(T_PfnVisImSrcFrameCallback *ppfnCallback,
		void **ppvUser) CONST
{
	if (ppfnCallback != 0)
		*ppfnCallback = m_pfnCallback;
	if (ppvUser != 0)
		*ppvUser = m_pvUser;

	return ResultFromScode(S_OK);
}

STDMETHODIMP CVisVFWCamera::SetFrameCallback(T_PfnVisImSrcFrameCallback pfnCallback,
		void *pvUser)
{
	m_pfnCallback = pfnCallback;
	m_pvUser = pvUser;

	return ResultFromScode(S_OK);
}


unsigned __stdcall CVisVFWCamera::StaticThreadMain(void *pvVFWCamera)
{
	CVisVFWCamera *pcamera = (CVisVFWCamera *) pvVFWCamera;
	assert(pcamera != 0);
	return pcamera->ThreadMain();
}


unsigned __stdcall CVisVFWCamera::ThreadMain(void)
{
	CreateCaptureWindow();

	ReleaseSemaphore(m_hSemaphoreHwndInitialized, 1, 0);

	// Message loop
	MSG msg;
	while (::GetMessage(&msg, NULL, NULL, NULL) == 1)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK CVisVFWCamera::StaticFrameCallbackProc(HWND hWnd,
		LPVIDEOHDR lpVHdr)
{
	assert(hWnd != 0);
	assert(lpVHdr != 0);

	CVisVFWCamera *pvfwcamera = (CVisVFWCamera *) capGetUserData(hWnd);
	assert(pvfwcamera != 0);

	return pvfwcamera->FrameCallbackProc(hWnd, lpVHdr);
}

LRESULT CVisVFWCamera::FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	assert(lpVHdr != 0);

	int cb = min(lpVHdr->dwBufferLength, lpVHdr->dwBytesUsed);
	if (((m_fInContinuousGrab) || (m_fInSingleFrameGrab))
			&& (m_pfnCallback != 0)
			&& (cb > 0))
	{
		SVisImSrcFrame imsrcframe;
		ZeroMemory(&imsrcframe, sizeof(imsrcframe));

		imsrcframe.m_rect.left = m_capstatus.ptScroll.x;
		imsrcframe.m_rect.top = m_capstatus.ptScroll.y;
		imsrcframe.m_rect.right = imsrcframe.m_rect.left +
				(int) m_capstatus.uiImageWidth;
		int dy = (int) m_capstatus.uiImageHeight;
		imsrcframe.m_rect.bottom = imsrcframe.m_rect.top +
				((dy >= 0) ? dy : - dy);

		assert(m_evisimencodingGrabCur != evisimencodingUnknown);
		if ((m_evisimencodingGrabCur != m_evisimencoding)
				&& (VisCbitGetImEncoding(m_evisimencodingGrabCur)
						!= VisCbitGetImEncoding(m_evisimencoding))
				&& ((cb * 8 / ((int) m_capstatus.uiImageWidth) / dy)
					== VisCbitGetImEncoding(m_evisimencoding)))
		{
			imsrcframe.m_evisimencoding = m_evisimencoding;
		}
		else
		{
			imsrcframe.m_evisimencoding = m_evisimencodingGrabCur;
		}

		int cbit = VisCbitGetImEncoding(imsrcframe.m_evisimencoding);
		if (cb * 8 < ((int) m_capstatus.uiImageWidth)
						* ((int) m_capstatus.uiImageHeight)
						* cbit)
		{
			imsrcframe.m_rect.bottom = imsrcframe.m_rect.top +
					(cb * 8 / ((int) m_capstatus.uiImageWidth) / cbit);
		}

		// LATER:  imsrcframe.m_filetime = ??? + lpVHdr->dmTimeCaptured;
		GetSystemTimeAsFileTime(&(imsrcframe.m_filetime));

		imsrcframe.m_pvData = lpVHdr->lpData;
		imsrcframe.m_cbData = cb;

		imsrcframe.m_evisconvyuv = m_evisconvyuv;

		// For the beta, we don't support color maps.
		// LATER:  We might also try getting the color table from palette
		// entries by using the HPALETTE in the CAPSTATUS data structure.
		// Or it might be part of the BITMAPINFO structure.
		imsrcframe.m_prgbqColormap = 0;
		imsrcframe.m_crgbqColormap = 0;

		imsrcframe.m_pfnDeleteData = 0;
		imsrcframe.m_pvDeleteData = 0;

		imsrcframe.dwReserved = 0;

		imsrcframe.m_dwFlags = evisfiminfVolatileData;

		if (m_rgeflipImEncoding[imsrcframe.m_evisimencoding] != eflipUnknown)
		{
			if (m_rgeflipImEncoding[imsrcframe.m_evisimencoding] == eflipYes)
				imsrcframe.m_dwFlags |= evisfiminfVertReflect;
		}
		else if (m_fFlippedImage)
		{
			imsrcframe.m_dwFlags |= evisfiminfVertReflect;
		}

		if (imsrcframe.m_rect.bottom > 0)
			m_pfnCallback((IVisImSrcDevice *) this, &imsrcframe, m_pvUser);

//		FreePbmi(pbmi);
	}

	return TRUE;
}


void CVisVFWCamera::CreateCaptureWindow(void)
{
	m_hwndPreview = capCreateCaptureWindow(m_strID.c_str(), 0, 40, 40, 120, 160, 0, 0);
}


void CVisVFWCamera::DestroyCaptureWindow(void)
{
	::DestroyWindow(m_hwndPreview);
	m_hwndPreview = 0;
}


BITMAPINFO *CVisVFWCamera::PbmiGet(void) const
{
	BYTE *pbVideoFormat = 0;
	int cbVideoFormat = capGetVideoFormatSize(m_hwndPreview);
	if (cbVideoFormat > 0)
	{
		pbVideoFormat = new BYTE[cbVideoFormat];

		if (pbVideoFormat != 0)
		{
			ZeroMemory(pbVideoFormat, cbVideoFormat);
			((BITMAPINFOHEADER *) pbVideoFormat)->biSize = cbVideoFormat;

			int cbT = capGetVideoFormat(m_hwndPreview,
					(BITMAPINFO *) pbVideoFormat, cbVideoFormat);

			assert(cbVideoFormat >= cbT);

			if (cbT == 0)
			{
				delete[] pbVideoFormat;
				pbVideoFormat = 0;
			}
		}
	}

	return (BITMAPINFO *) pbVideoFormat;
}

void CVisVFWCamera::FreePbmi(BITMAPINFO *pbmi) const
{
	if (pbmi != 0)
		delete[] ((BYTE *) pbmi);
}

EVisImEncoding CVisVFWCamera::EVisImEncodingGet(BITMAPINFO *pbmi,
		bool fUseCachedInfo)
{
	if ((!fUseCachedInfo)
			|| (m_evisimencoding == evisimencodingUnknown))
	{
		BITMAPINFOHEADER *pbmih;
		if (pbmi != 0)
		{
			pbmih = (BITMAPINFOHEADER *) pbmi;
		}
		else
		{
			pbmih = (BITMAPINFOHEADER *) PbmiGet();
		}

		if (pbmih != 0)
		{
			m_evisimencoding = ImEncodingGetDwFourCC(pbmih->biCompression,
					pbmih->biBitCount);
#ifdef _DEBUG
			if (m_evisimencoding == evisimencodingUnknown)
			{
				char szBuf[255];
				sprintf(szBuf, "VisSDK: (VisVFW) %d-bit format %#010x (== '%c%c%c%c",
						pbmih->biBitCount, pbmih->biCompression,
						((char *) &(pbmih->biCompression))[3],
						((char *) &(pbmih->biCompression))[2],
						((char *) &(pbmih->biCompression))[1],
						((char *) &(pbmih->biCompression))[0]);
				OutputDebugString(szBuf);
				OutputDebugString("') is not known!\n");
			}
#endif // _DEBUG
		}

		if ((pbmi == 0) && (pbmih != 0))
			FreePbmi((BITMAPINFO *) pbmih);
	}

	assert(m_evisimencoding >= evisimencodingFirst);
	assert(m_evisimencoding < evisimencodingLim);

	return m_evisimencoding;
}


void CVisVFWCamera::UpdateCachedInfo(void)
{
	BITMAPINFO *pbmi = PbmiGet();

	EVisImEncodingGet(pbmi, false);

	if (pbmi != 0)
	{
		if (pbmi->bmiHeader.biHeight >= 0)
		{
			if (m_fUseNegHtToFlip)
				m_fFlippedImage = true;
		}
		else
		{
			m_fUseNegHtToFlip = true;
			m_fFlippedImage = false;
		}
		
		FreePbmi(pbmi);
	}

	capGetStatus(m_hwndPreview, &m_capstatus, sizeof (m_capstatus));
}


bool CVisVFWCamera::FFindImEncodings(void)
{
	bool fFoundImEncoding = false;
	std::string strVal;

	// Save the current video format.
	BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) PbmiGet();
	BITMAPINFOHEADER bmih;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 3150;
	bmih.biYPelsPerMeter = 3150;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	bmih.biWidth = 320;
	bmih.biHeight = 240;

	if (m_fCanSetVideoFormat)
	{
		// Try setting a garbage format.  If this succeeds, we can't determine
		// the supported video formats by looking at the capSetVideoFormat
		// return value.
		bmih.biBitCount = 16;
		bmih.biCompression = 'ZzzZ';
		if (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) &bmih,
				sizeof(BITMAPINFOHEADER)))
		{
			m_fCanSetVideoFormat = false;
		}
	}

	if (m_fCanSetVideoFormat)
	{
		// We can try different formats and check the capSetVideoFormat
		// return value to see if they are supported.
		for (int i = evisimencodingFirst; i < evisimencodingLim; ++ i)
		{
			if ((i == evisimencodingUnknown)
					|| (i == evisimencodingGray4bit)	// Same as color4bit
					|| (i == evisimencodingGray8bit)	// Same as color8bit
					|| (i == evisimencodingGray16bit)	// Same as RGB555
					|| (i == evisimencodingRGB565)		// Same as RGB555
					|| (i == evisimencodingRGB48)		// cbit is too large
					|| (i == evisimencodingRGBA64)		// cbit is too large
					|| (i == evisimencodingYUV48)		// cbit is too large
					|| (i == evisimencodingYUVA64)		// cbit is too large
					|| (i == evisimencodingY41P)		// Not supported - ignore
					|| (i == evisimencodingY41T)		// Not supported - ignore
					|| (i == evisimencodingCLJR)		// Not supported - ignore
					|| (i == evisimencodingCLPL)		// Not supported - ignore
					|| (i == evisimencodingY210)		// Not supported - ignore
					|| (i == evisimencodingY410)		// Not supported - ignore
					|| (i == evisimencodingYUVP))		// Not supported - ignore
			{
				continue;
			}

#ifdef TRY_MULTIPLE_SIZES
			for (int iSize = 0; iSize < 2; ++iSize)
			{
				if (iSize == 0)
				{
					bmih.biWidth = 320;
					bmih.biHeight = 240;
				}
				else
				{
					bmih.biWidth = 160;
					bmih.biHeight = 120;
				}
#endif // TRY_MULTIPLE_SIZES

				bmih.biBitCount = VisCbitGetImEncoding((EVisImEncoding) i);
				bmih.biCompression = VisDwFourCCGetImEncoding((EVisImEncoding) i);

				if (capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) &bmih,
						sizeof(BITMAPINFOHEADER)))
				{
					fFoundImEncoding = true;
					m_rgfHaveImEncoding[i] = true;
					m_rgeflipImEncoding[i] = eflipUnknown;
					m_rgdwFourCCImEncoding[i] = bmih.biCompression;

#ifdef TRY_MULTIPLE_SIZES
					// If we can set the video format with one size, we don't
					// need to try other sizes.
					break;
#endif // TRY_MULTIPLE_SIZES
				}

				// UNDONE:  Add special case code for the I420 and cyuv FourCC values.
				// (I420 is the same as IYUV and cyuv is UYVY flipped.)
#ifdef TRY_MULTIPLE_SIZES
			}
#endif // TRY_MULTIPLE_SIZES
		}
	}

	if (pbmih != 0)
	{
		// See if we recognize the current format.
		EVisImEncoding evisimencoding = VisImEncodingGetDwFourCC(
				pbmih->biCompression, pbmih->biBitCount);

		assert((evisimencoding >= evisimencodingFirst)
				&& (evisimencoding < evisimencodingLim));

		if (evisimencoding != evisimencodingUnknown)
		{
			fFoundImEncoding = true;
			m_rgfHaveImEncoding[evisimencoding] = true;
			m_rgeflipImEncoding[evisimencoding] = eflipUnknown;
			m_rgdwFourCCImEncoding[evisimencoding] = pbmih->biCompression;
		}
		else
		{
			FoundUnknownDwFourCC(pbmih->biCompression, (int) pbmih->biBitCount);
		}

		// Attempt to restore the video format.
		if (m_fCanSetVideoFormat)
		{
			capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) pbmih,
					sizeof(BITMAPINFOHEADER));
		}
		
		FreePbmi((BITMAPINFO *) pbmih);
	}
	else
	{
		// Assert because it should be rare to get here.
		// (If we get here often, we may need to improve this code.)
		assert(0);

		if (m_fCanSetVideoFormat)
		{
			// Try to set a reasonable format, like RGB24.
			bmih.biBitCount = 24;
			bmih.biCompression = BI_RGB;
			capSetVideoFormat(m_hwndPreview, (BITMAPINFO *) &bmih,
					sizeof(BITMAPINFOHEADER));
		}
	}

	return fFoundImEncoding;
}


EVisImEncoding CVisVFWCamera::ImEncodingGetDwFourCC(DWORD dwFourCC, int cbit)
{
	EVisImEncoding evisimencoding = evisimencodingUnknown;

	for (int i = evisimencodingFirst; i < evisimencodingLim; ++ i)
	{
		if ((i == evisimencodingUnknown)
				|| (!m_rgfHaveImEncoding[i]))
		{
			continue;
		}

		if ((m_rgdwFourCCImEncoding[i] == dwFourCC)
				&& (VisCbitGetImEncoding((EVisImEncoding) i) == cbit))
		{
			evisimencoding = (EVisImEncoding) i;
			break;
		}
	}

	if (evisimencoding == evisimencodingUnknown)
	{
		evisimencoding = VisImEncodingGetDwFourCC(dwFourCC, cbit);

		if (evisimencoding != evisimencodingUnknown)
		{
			// Add this value to the list of known image encodings.
			m_rgfHaveImEncoding[evisimencoding] = true;
			m_rgdwFourCCImEncoding[evisimencoding] = dwFourCC;
			m_rgeflipImEncoding[evisimencoding] = eflipUnknown;
		}
		else
		{
			FoundUnknownDwFourCC(dwFourCC, cbit);
		}
	}

	return evisimencoding;
}


void CVisVFWCamera::FoundUnknownDwFourCC(DWORD dwFourCC, int cbit)
{
	for (int iUnknown = 0; iUnknown < m_cdwFourCCUnknownCur; ++ iUnknown)
	{
		if ((m_rgdwFourCCUnknown[iUnknown] == dwFourCC)
				&& (m_rgcbitFourCCUnknown[iUnknown] == cbit))
		{
			break;
		}
	}

	if (iUnknown == m_cdwFourCCUnknownCur)
	{
		if (m_cdwFourCCUnknownCur + 1 < eidwFourCCUnknownLim)
		{
			++ m_cdwFourCCUnknownCur;
			m_rgdwFourCCUnknown[iUnknown] = dwFourCC;
			m_rgcbitFourCCUnknown[iUnknown] = cbit;
		}

#ifdef _DEBUG
		char szBuf[255];
		sprintf(szBuf,
				"VisSDK: (VisVFW) %d-bit format %#010x (== '%c%c%c%c",
				cbit, dwFourCC,
				((char *) &dwFourCC)[3],
				((char *) &dwFourCC)[2],
				((char *) &dwFourCC)[1],
				((char *) &dwFourCC)[0]);
		OutputDebugString(szBuf);
		OutputDebugString("') is not known!\n");
#endif // _DEBUG
	}
}


DWORD CVisVFWCamera::DwFourCCGetImEncoding(EVisImEncoding evisimencoding) const
{
	DWORD dwRet = 0;
	
	if ((evisimencoding >= evisimencodingFirst)
			&& (evisimencoding != evisimencodingUnknown)
			&& (evisimencoding < evisimencodingLim))
	{
		if (m_rgfHaveImEncoding[evisimencoding])
			dwRet = m_rgdwFourCCImEncoding[evisimencoding];
		else
			dwRet = VisDwFourCCGetImEncoding(evisimencoding);
	}

	return dwRet;
}

