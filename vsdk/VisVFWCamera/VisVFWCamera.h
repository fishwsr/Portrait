////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWCAMERA 
//
// @module VisVFWCamera.h |
//
// This file defines the <c CVisVFWCamera> class.  It is derived from
// the <c IVisImSrcSettings> and <c IVisImSrcDevice> interfaces.
// It is used to work with (adjust settings for and get images from)
// a VFW camera.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisVFWCamera\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISVFWCAMERA_H__
#define __VIS_IMSRC_VISVFWCAMERA_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// We don't want any dependencies on the CVisImage class, so we don't
// include VisCore.h in this file.
#include <Windows.h>
#include <mmsystem.h>
#include <vfw.h>
#include <ole2.h>

#include <assert.h>

#include <string>

#include "..\VisImSrc\VisImSrcNoMFC.h"

#include "VisVFWCamProj.h"
#include "VisVFWProvider.h"


class CVisVFWCamera : public IVisImSrcSettings,
		public IVisImSrcDevice
{
public:
	//------------------------------------------------------------------
	// Constructor and destructor

    CVisVFWCamera(int iCaptureDevice, bool fOwnThread = true);
	virtual ~CVisVFWCamera(void);

	// Returns S_OK if successful.
	HRESULT CVisVFWCamera::Init(bool fFullInit = true);


	//------------------------------------------------------------------
	// IUnknown methods

    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject);
    STDMETHOD_(ULONG, AddRef) (THIS);
    STDMETHOD_(ULONG, Release) (THIS);


	//------------------------------------------------------------------
	// IVisImageSourceSettings properties

	// Properties of this image source.
	STDMETHOD(GetImSrcRange) (THIS_ EVisImSrc evisimsrc, LONG *plMin,
			LONG *plMax, LONG *pdlStepping VISDEFAULT(0),
			LONG *plDefault VISDEFAULT(0), LONG *plFlags VISDEFAULT(0))
			CONST;
	STDMETHOD(GetImSrcValue) (THIS_ EVisImSrc evisimsrc, LONG *plValue)
			CONST;
	STDMETHOD(SetImSrcValue) (THIS_ EVisImSrc evisimsrc, LONG lValue,
			LONG lFlags VISDEFAULT(evisimsrcflagAuto));
	STDMETHOD(SaveSettings) (THIS_ bool fForAllApps VISDEFAULT(false));


	//------------------------------------------------------------------
	// IVisImSrcDevice methods

	STDMETHOD(IsPreferredImEncoding) (THIS_ EVisImEncoding evisimencoding) CONST;
	STDMETHOD(IsSupportedImEncoding) (THIS_ EVisImEncoding evisimencoding) CONST;
    STDMETHOD(GrabNext) (THIS);
	STDMETHOD(AddBuffer) (THIS_ void *pvBuffer, ULONG cbBuffer,
			VisImSrcMemCallback pfnDeleteBuffer, void *pvDeleteBuffer);
	STDMETHOD(GetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback *ppfnCallback, void **ppvUser)
			CONST;
	STDMETHOD(SetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback pfnCallback, void *pvUser);
    STDMETHOD(HasDialog) (THIS_ EVisVidDlg evisviddlg);
    STDMETHOD_(LONG, DoDialog) (THIS_ EVisVidDlg evisviddlg,
			HWND hwndParent VISDEFAULT(0));
    STDMETHOD(GetSzID) (THIS_ LPSTR sz, LONG *pcb);
    STDMETHOD(GetSzName) (THIS_ LPSTR sz, LONG *pcb);


private:
	enum { ecchProviderSep = 3 };

	enum { eidwFourCCUnknownLim = 10 };

	enum EFlip
	{
		eflipUnknown = - 1,
		eflipNo,
		eflipYes
	};

	unsigned long m_cRef;
	std::string m_strID;
	int m_iCaptureDevice;
	EVisImEncoding m_evisimencoding;
	EVisConvYUV m_evisconvyuv;
	EVisImEncoding m_evisimencodingGrabCur;
	CAPDRIVERCAPS m_capdrivercaps;
	CAPTUREPARMS m_captureparms;
	CAPSTATUS m_capstatus;
	HANDLE m_hSemaphoreHwndInitialized;
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	HWND m_hwndPreview;
	T_PfnVisImSrcFrameCallback m_pfnCallback;
	void *m_pvUser;
	DWORD m_rgdwFourCCUnknown[eidwFourCCUnknownLim];
	int m_rgcbitFourCCUnknown[eidwFourCCUnknownLim];
	int m_cdwFourCCUnknownCur;
	DWORD m_rgdwFourCCImEncoding[evisimencodingLim];
	EFlip m_rgeflipImEncoding[evisimencodingLim];
	bool m_rgfHaveImEncoding[evisimencodingLim];
	bool m_fInContinuousGrab;
	bool m_fInSingleFrameGrab;
	bool m_fFlippedImage;
	bool m_fUseNegHtToFlip;
	bool m_fCanSetVideoFormat;

	static unsigned __stdcall StaticThreadMain(void *pvVFWCamera);
	unsigned __stdcall ThreadMain(void);

	static LRESULT CALLBACK StaticFrameCallbackProc(HWND hWnd,
			LPVIDEOHDR lpVHdr);
	LRESULT FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

	void CreateCaptureWindow(void);
	void DestroyCaptureWindow(void);
	const char *SzName(void) const;
	void SetupCaptureParams(void);
	bool FReadSettings(void);
	BITMAPINFO *PbmiGet(void) const;
	void FreePbmi(BITMAPINFO *pbmi) const;
	EVisImEncoding EVisImEncodingGet(BITMAPINFO *pbmi = 0,
			bool fUseCachedInfo = true);
	void UpdateCachedInfo(void);
	bool FFindImEncodings(void);
	EVisImEncoding ImEncodingGetDwFourCC(DWORD dwFourCC, int cbit);
	void FoundUnknownDwFourCC(DWORD dwFourCC, int cbit);
	DWORD DwFourCCGetImEncoding(EVisImEncoding evisimencoding) const;
};


#endif // __VIS_IMSRC_VISVFWCAMERA_H__
