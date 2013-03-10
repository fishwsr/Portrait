////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISVFWPROVIDER 
//
// @module VisVFWProvider.cpp |
//
// This file implements the <c CVisVFWProvider> class.  It is derived from
// the <c IVisImSrcProvider> interface.  It is used to get a list of
// available VFW devices and to create a <c CVisVFWCamera> object for
// a specified device.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisVFWProvider\.h>
//
////////////////////////////////////////////////////////////////////////////


#include "VisVFWCamPch.h"
#include "VisVFWCamera.h"


const char *CVisVFWProvider::s_szID = "VisVFW";

#ifdef _DEBUG
const char *CVisVFWProvider::s_szName = "Video for Windows";
#else // _DEBUG
const char *CVisVFWProvider::s_szName = "Video for Windows (Debug)";
#endif // _DEBUG



CVisVFWProvider::CVisVFWProvider(void)
  : m_cRef(1),
	m_plistszConnectedDevices(0)
{
	// Clear the list of connected devices.
	for (int i = 0; i < e_iVFWIDLim; ++i)
		m_mpiiConnectedToVFWID[i] = - 1;
}

CVisVFWProvider::~CVisVFWProvider(void)
{
	if (m_plistszConnectedDevices != 0)
	{
		m_plistszConnectedDevices->Release();
		m_plistszConnectedDevices = 0;
	}
}



STDMETHODIMP CVisVFWProvider::QueryInterface(THIS_ REFIID riid,
		void **ppvObject)
{
	HRESULT hrRet;

	if (ppvObject == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if ((riid == IID_IUnknown)
				|| (riid == IID_IVisImSrcProvider))
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = (IVisImSrcProvider *) this;
		AddRef();
	}
	else
	{
		*ppvObject = 0;
		hrRet = ResultFromScode(E_NOINTERFACE);
	}

	return hrRet;
}

STDMETHODIMP_(ULONG) CVisVFWProvider::AddRef(THIS)
{
	assert(m_cRef > 0);

	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CVisVFWProvider::Release(THIS)
{
	unsigned long cRef = --m_cRef;

	if (cRef == 0)
		delete this;

	return cRef;
}



STDMETHODIMP CVisVFWProvider::HasConnectedDevices(THIS)
{
	if (m_plistszConnectedDevices == 0)
		FindConnectedDevices();
	
	HRESULT hrRet;

	if (m_plistszConnectedDevices == 0)
		hrRet = ResultFromScode(E_FAIL);
	else if (m_plistszConnectedDevices->CszGet() == 0)
		hrRet = ResultFromScode(S_FALSE);
	else
		hrRet = ResultFromScode(S_OK);

	return hrRet;
}

STDMETHODIMP CVisVFWProvider::HasSelectionDialog(THIS)
{
	// VFW devices do not have their own selection dialog.
	return ResultFromScode(S_FALSE);
}

STDMETHODIMP_(LONG) CVisVFWProvider::DoSelectionDialog(THIS_
		HWND hwndParent)
{
	// VFW devices do not have their own selection dialog.
	return - 1;
}

STDMETHODIMP CVisVFWProvider::GetConnectedDeviceList(THIS_
		IVisListSz **ppIVisListSz)
{
	if (m_plistszConnectedDevices == 0)
		FindConnectedDevices();
	
	HRESULT hrRet;

	if (ppIVisListSz == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else
	{
		if (m_plistszConnectedDevices == 0)
			FindConnectedDevices();
		
		if (m_plistszConnectedDevices == 0)
			hrRet = ResultFromScode(E_FAIL);
		else
			hrRet = ResultFromScode(S_OK);

		*ppIVisListSz = m_plistszConnectedDevices;

		if (m_plistszConnectedDevices != 0)
			m_plistszConnectedDevices->AddRef();
	}

	return hrRet;
}

STDMETHODIMP CVisVFWProvider::GetDeviceSz(THIS_
		IVisImSrcDevice **ppIVisImSrcDevice, LPCSTR sz)
{
	// UNDONE:  We shouldn't need to find the list of all devices here.  We
	// only need to attempt to instantiate devices that have the name specified.
	if (m_plistszConnectedDevices == 0)
		FindConnectedDevices();
	
	HRESULT hrRet;

	if (ppIVisImSrcDevice == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if (sz == 0)
	{
		hrRet = ResultFromScode(E_INVALIDARG);
		*ppIVisImSrcDevice = 0;
	}
	else if (m_plistszConnectedDevices == 0)
	{
		hrRet = ResultFromScode(E_FAIL);
		*ppIVisImSrcDevice = 0;
	}
	else if ((sz[0] >= '0') && (sz[0] <= '9') && (sz[1] == 0))
	{
		hrRet = GetDeviceI(ppIVisImSrcDevice, sz[0] - '0');
	}
	else
	{
		hrRet = GetDeviceI(ppIVisImSrcDevice,
				m_plistszConnectedDevices->IGetSz(sz));

		if (FAILED(hrRet))
			hrRet = FindDeviceSz(ppIVisImSrcDevice, sz);
	}

	return hrRet;
}

STDMETHODIMP CVisVFWProvider::GetDeviceI(THIS_
		IVisImSrcDevice **ppIVisImSrcDevice, LONG i)
{
	// UNDONE:  We shouldn't need to find the list of all devices here.  We
	// only need to attempt to instantiate i devices to find the device specified.
	if (m_plistszConnectedDevices == 0)
		FindConnectedDevices();
	
	HRESULT hrRet;

	if (ppIVisImSrcDevice == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if (m_plistszConnectedDevices == 0)
	{
		hrRet = ResultFromScode(E_FAIL);
		*ppIVisImSrcDevice = 0;
	}
	else if ((i < 0) || (i >= m_plistszConnectedDevices->CszGet()))
	{
		hrRet = ResultFromScode(E_INVALIDARG);
		*ppIVisImSrcDevice = 0;
	}
	else
	{
		assert(m_mpiiConnectedToVFWID[i] >= 0);
		CVisVFWCamera *pCamera = new CVisVFWCamera(m_mpiiConnectedToVFWID[i],
				true);
		
		if (pCamera == 0)
			hrRet = ResultFromScode(E_OUTOFMEMORY);
		else
			hrRet = pCamera->Init(true);

		if (FAILED(hrRet))
		{
			delete pCamera;
			pCamera = 0;
		}

		*ppIVisImSrcDevice = pCamera;
	}

	return hrRet;
}

STDMETHODIMP CVisVFWProvider::GetSzID(THIS_ LPSTR sz, LONG *pcb)
{
	return VisGetSzPcbFromSz(sz, pcb, s_szID);
}

STDMETHODIMP CVisVFWProvider::GetSzName(THIS_ LPSTR sz, LONG *pcb)
{
	return VisGetSzPcbFromSz(sz, pcb, s_szName);
}


void  CVisVFWProvider::FindConnectedDevices(void)
{
	if (m_plistszConnectedDevices == 0)
	{
		CVisListSz *plistsz = new CVisListSz;

		// Check for connected devices.
		int iConnected = 0;
		for (int iDevice = 0; iDevice < e_iVFWIDLim; ++iDevice)
		{
			CVisVFWCamera *pCamera = new CVisVFWCamera(iDevice, false);
			if (pCamera != 0)
			{
				char szDevName[256];
				long cb = sizeof(szDevName);
				if ((SUCCEEDED(pCamera->Init(false)))
						&& (SUCCEEDED(pCamera->GetSzName(szDevName, &cb))))
				{
					// We could connect to this device!
					m_mpiiConnectedToVFWID[iConnected++] = iDevice;
					plistsz->AddSz(szDevName);
				}

				delete pCamera;
			}
		}

		m_plistszConnectedDevices = plistsz;
	}
}

HRESULT CVisVFWProvider::FindDeviceSz(IVisImSrcDevice **ppIVisImSrcDevice,
		LPCSTR sz)
{
	HRESULT hrRet = ResultFromScode(E_FAIL);

	assert(sz != 0);
	assert(ppIVisImSrcDevice != 0);
	assert(*ppIVisImSrcDevice == 0);

	if (*sz == 0)
	{
		// Find default device from registry or use first connected device.
		int iDevice = 0;

		const char *szReg = VIS_SZ_REG_IMSRC_DEVS "\\VisVFW";
		HKEY hkeyT;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szReg, 0,
				KEY_QUERY_VALUE, &hkeyT) == ERROR_SUCCESS)
		{
			DWORD dwType;
			char szT[256];
			DWORD cbT = sizeof(szT);
			if (RegQueryValueEx(hkeyT, "Default", 0, &dwType,
							(BYTE *) &szT, &cbT) == ERROR_SUCCESS)
			{
				if ((dwType == REG_DWORD) && (*((DWORD *) szT) <= 9))
				{
					// Look for a connected device with this VFW ID.
					iDevice = IConnectedGetIVFWID((int) *((DWORD *) szT));
				}
				else if ((dwType == REG_SZ) && (*szT != 0))
				{
					if ((*szT >= '0') && (*szT <= '9') && (szT[1] == 0))
					{
						// Look for a connected device with this VFW ID.
						iDevice = IConnectedGetIVFWID(*szT - '0');
					}
					else if (m_plistszConnectedDevices != 0)
					{
						// Look for a connected device with this ID string.
						iDevice = m_plistszConnectedDevices->IGetSz(szT);
					}
				}
			}

			RegCloseKey(hkeyT);
		}

		if ((iDevice >= 0) && (iDevice < e_iVFWIDLim))
			hrRet = GetDeviceI(ppIVisImSrcDevice, iDevice);
	}
	else if (((sz[0] == 'V') || (sz[0] == 'v'))
			&& ((sz[1] == 'F') || (sz[1] == 'f'))
			&& ((sz[2] == 'W') || (sz[2] == 'w'))
			&& ((sz[3] >= '0') && (sz[3] <= '9'))
			&& (sz[4] == 0))
	{
		// Parse sz to try to find the device.
		// Use VFW ID (sz[3] - '0'), if available
		hrRet = GetDeviceI(ppIVisImSrcDevice, IConnectedGetIVFWID(sz[3] - '0'));
	}
	// LATER:  We might want to let sz specify a reg entry containing the
	// device index or try substring matching?

	// Make sure that we return E_FAIL if we can't find a device.
	// (For example, we don't want to return E_INVALIDARG if we pass a bad
	// index to GetDeviceI.)
	if (FAILED(hrRet))
		hrRet = ResultFromScode(E_FAIL);

	return hrRet;
}


int CVisVFWProvider::IConnectedGetIVFWID(int iVFWID)
{
	for (int iDevice = 0; iDevice < e_iVFWIDLim; ++iDevice)
	{
		if (m_mpiiConnectedToVFWID[iDevice] == iVFWID)
			return iDevice;
	}

	return - 1;
}
