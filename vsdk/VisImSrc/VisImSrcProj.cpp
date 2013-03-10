// VisImSrcProj.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



HINSTANCE imsrcCurrentInstanceHandle = NULL;

VisImSrcExport int VISAPI VisDoImSrcDlgHwnd(LPCSTR szTitle, IVisListSz *pIVisListSz,
		HWND hWndParent, int iSelectionInitial)
{
	int iRet = - 1;
	if ((pIVisListSz != 0) && (pIVisListSz->CszGet() > 0))
	{
		// used to use "pWndParent->m_hWnd" as 3rd param...
		CVisDlgListSz dlg(szTitle, pIVisListSz, hWndParent, iSelectionInitial);

		if (dlg.DoModal() == IDOK)
		{
			iRet = dlg.ISelection();
			assert((iRet >= 0) && (iRet < pIVisListSz->CszGet()));
		}
	}

	return iRet;
}


VisImSrcExport IVisListSz * VISAPI VisGetSubkeyList(HKEY hkeyRoot, const char *szKey,
		int eviskeylist)
{
	return VisInlineGetSubkeyList(hkeyRoot, szKey, eviskeylist);
}


VisImSrcExport HRESULT VISAPI VisGetProviderList(IVisListSz **ppIVisListSz, int eviskeylist)
{
	HRESULT hrRet;

	if (ppIVisListSz == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else
	{
		*ppIVisListSz = VisGetSubkeyList(HKEY_LOCAL_MACHINE,
				VIS_SZ_REG_IMSRC_DEVS, eviskeylist);
		GetGlobalProviders(ppIVisListSz);
		hrRet = ResultFromScode((*ppIVisListSz == 0) ? E_FAIL : S_OK);
	}

	return hrRet;
}

VisImSrcExport void VISAPI GetGlobalProviders(IVisListSz **ppIVisListSz)
{
	int nProviders = PGlobals()->NImSrcProviders();
	if((*ppIVisListSz == 0) && (nProviders > 0))
		*ppIVisListSz = new CVisListSz;
	for(int iProvider = 0; iProvider < nProviders; iProvider++ )
	{
		std::string szProvider(PGlobals()->ImSrcProviderID(iProvider));
		szProvider += ": ";
		szProvider += PGlobals()->ImSrcProviderName(iProvider);
		if((*ppIVisListSz)->IGetSz(szProvider.c_str()) < 0)
		{
			((CVisListSz* )(*ppIVisListSz))->AddSz(szProvider.c_str());
		}
	}
}

VisImSrcExport IVisImSrcProvider * VISAPI VisPIVisImSrcProviderGetSz(const char *szProvider)
{
	IVisImSrcProvider *pIVisImSrcProvider = 0;
	const char *szSubkeyDll = "DLL";
	HKEY hkeyDevs;

	int iGlobal = PGlobals()->IGetProvider(szProvider);
	if(iGlobal != -1)
	{
		PGlobals()->ImSrcProviderCallback(iGlobal)(0, szProvider,
												IID_IVisImSrcProvider,
												(void **) &pIVisImSrcProvider);
	}
	
	else if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, VIS_SZ_REG_IMSRC_DEVS,
			0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
			&hkeyDevs) == ERROR_SUCCESS)
	{
		char szDllName[256];
		DWORD cbDllName = sizeof(szDllName);
		DWORD dwType;
		HKEY hkeySubkey;
		if (RegOpenKeyEx(hkeyDevs, szProvider, 0, KEY_QUERY_VALUE,
					&hkeySubkey) == ERROR_SUCCESS)
		{
			if ((RegQueryValueEx(hkeySubkey, szSubkeyDll, 0, &dwType,
							(BYTE *) szDllName, &cbDllName)
							== ERROR_SUCCESS)
					&& (dwType == REG_SZ))
			{
				// Attempt to get the provider from the DLL.
				// LATER:  Add code to call FreeLibrary on this DLL when
				// we exit.  (Is such code useful?)
				HINSTANCE hinstProviderDLL = LoadLibrary(szDllName);

				T_PfnVisGetImSrcProvider pfnGetImSrcProvider = 0;
				if (hinstProviderDLL != 0)
				{
					pfnGetImSrcProvider = (T_PfnVisGetImSrcProvider)
							GetProcAddress(hinstProviderDLL,
							VIS_SZ_FN_GET_IM_SRC_PROVIDER);
				}

				if ((pfnGetImSrcProvider != 0)
						&& (FAILED(pfnGetImSrcProvider(0, szProvider,
								IID_IVisImSrcProvider,
								(void **) &pIVisImSrcProvider))))
				{
					assert(pIVisImSrcProvider == 0);
				}
			}

			RegCloseKey(hkeySubkey);
		}

		RegCloseKey(hkeyDevs);
	}


	return pIVisImSrcProvider;
}


VisImSrcExport IVisImSrcProvider * VISAPI VisPIVisImSrcProviderGetI(int iProvider)
{
	// UNDONE:  Change to do the following:
	// (1)  Get the list of all providers.
	// (2)  Find the string for iProvider.
	// (3)  Use the strting in VisPIVisImSrcProviderGetSz.



	// Get the list of available providers.
	IVisListSz *pIVisListSzProvider = 0;
	VisGetProviderList(&pIVisListSzProvider, eviskeylistAvailID);

	CVisString szSelect = pIVisListSzProvider->SzGetI(iProvider);

	IVisImSrcProvider *pIVisImSrcProvider
			= VisPIVisImSrcProviderGetSz(szSelect.c_str());

	return pIVisImSrcProvider;

/*
	const char *szSubkeyDll = "DLL";
	HKEY hkeyDevs;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, VIS_SZ_REG_IMSRC_DEVS,
			0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
			&hkeyDevs) == ERROR_SUCCESS)
	{
		DWORD iSubkeyLim = 0;
		RegQueryInfoKey(hkeyDevs, 0, 0, 0, &iSubkeyLim, 0, 0, 0, 0, 0, 0, 0);
		if ((iProvider >= 0) && (iProvider < (int) iSubkeyLim))
		{
			char szSubkey[256];
			char szDllName[256];
			DWORD cbSubkey = sizeof(szSubkey);
			DWORD cbDllName = sizeof(szDllName);
			DWORD dwType;
			HKEY hkeySubkey;
			if ((RegEnumKeyEx(hkeyDevs, iProvider, szSubkey, &cbSubkey, 0, 0,
							0, 0) == ERROR_SUCCESS)
					&& (RegOpenKeyEx(hkeyDevs, szSubkey, 0, KEY_QUERY_VALUE,
						&hkeySubkey) == ERROR_SUCCESS))
			{
				if ((RegQueryValueEx(hkeySubkey, szSubkeyDll, 0, &dwType,
								(BYTE *) szDllName, &cbDllName)
								== ERROR_SUCCESS)
						&& (dwType == REG_SZ))
				{
					// Attempt to get the provider from the DLL.
					// LATER:  Add code to call FreeLibrary on this DLL when
					// we exit.  (Is such code useful?)
					HINSTANCE hinstProviderDLL = LoadLibrary(szDllName);

					T_PfnVisGetImSrcProvider pfnGetImSrcProvider = 0;
					if (hinstProviderDLL != 0)
					{
						pfnGetImSrcProvider = (T_PfnVisGetImSrcProvider)
								GetProcAddress(hinstProviderDLL,
								VIS_SZ_FN_GET_IM_SRC_PROVIDER);
					}

					if ((pfnGetImSrcProvider != 0)
							&& (FAILED(pfnGetImSrcProvider(0, szSubkey,
									IID_IVisImSrcProvider,
									(void **) &pIVisImSrcProvider))))
					{
						assert(pIVisImSrcProvider == 0);
					}
				}

				RegCloseKey(hkeySubkey);
			}
		}

		RegCloseKey(hkeyDevs);
	}


	return pIVisImSrcProvider;
*/
}


VisImSrcExport CVisImageSource VISAPI VisFindImageSource(const char *szImageSourceID)
{
	CVisImageSource imagesource;

	if (szImageSourceID == 0)
		return VisFindImageSourceHwnd();

	// Look for a provider ID followed by a colon.
	// Assume that provider IDs will not be longer than 100 characters.
	char szProvider[100];
	for (int ich = 0;
			((szImageSourceID[ich] != 0) && (ich < sizeof(szProvider) - 1));
			++ich)
	{
		if (szImageSourceID[ich] != ':')
			szProvider[ich] = szImageSourceID[ich];
		else
			break;
	}

	// Fail if we don't find the end of the string or a colon.
	if ((szImageSourceID[ich] != 0) && (szImageSourceID[ich] != ':'))
		return  imagesource;

	// Don't forget to terminate the provider string!
	szProvider[ich] = 0;

	// Get the list of available providers.
	IVisListSz *pIVisListSzProvider = 0;
	VisGetProviderList(&pIVisListSzProvider, eviskeylistAvailID);

	int iProvider = - 1;
	if (*szProvider == 0)
	{
		// If the provider string is empty, attempt to get a default provider
		// from the registry.
		HKEY hkeyT;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, VIS_SZ_REG_IMSRC_DEVS,
				0, KEY_QUERY_VALUE, &hkeyT) == ERROR_SUCCESS)
		{
			DWORD dwType;
			DWORD cbT = sizeof(szProvider);
			if ((RegQueryValueEx(hkeyT, "Default", 0, &dwType,
							(BYTE *) &szProvider, &cbT) != ERROR_SUCCESS)
					|| (dwType != REG_SZ))
			{
				// Clear the provider ID string if we can't read a
				// string value from the registry.
				*szProvider = 0;
			}
			else
			{
				// Get the index of this provider for use below.
				iProvider = pIVisListSzProvider->IGetSz(szProvider);
			}

			RegCloseKey(hkeyT);
		}

		// If we can't get it from the registry, use the first
		// provider that we find.
		if ((*szProvider == 0)
				&& (pIVisListSzProvider != 0)
				&& (*(pIVisListSzProvider->SzGetI(0)) != 0)
				&& (strlen(pIVisListSzProvider->SzGetI(0))
						< sizeof(szProvider)))
		{
			strcpy(szProvider, pIVisListSzProvider->SzGetI(0));
			iProvider = 0;
		}
	}
	else if ((szProvider[0] >= '0')
			&& (szProvider[0] <= '9')
			&& ((szProvider[1] == 0)
				|| ((szProvider[1] >= '0')
					&& (szProvider[1] <= '9')
					&& (szProvider[2] == 0))))
	{
		// If the provider string consists of a single digit or a pair
		// of digits, use the number as an index specifying the provider.
		iProvider = szProvider[0] - '0';
		if (szProvider[1] != 0)
			iProvider = 10 * iProvider + szProvider[1] - '0';

		if ((pIVisListSzProvider != 0)
				&& (*(pIVisListSzProvider->SzGetI(iProvider)) != 0)
				&& (strlen(pIVisListSzProvider->SzGetI(iProvider))
						< sizeof(szProvider)))
		{
			strcpy(szProvider, pIVisListSzProvider->SzGetI(iProvider));
		}
	}
	else
	{
		iProvider = pIVisListSzProvider->IGetSz(szProvider);
	}

	// Fail if no providers are available or if no provider has an ID that
	// matches the ID that we're looking for.
	if ((pIVisListSzProvider == 0) || (iProvider < 0))
	{
		if (pIVisListSzProvider != 0)
			pIVisListSzProvider->Release();

		return  imagesource;
	}

	// Get the provider.
	IVisImSrcProvider *pIVisImSrcProvider
			= VisPIVisImSrcProviderGetSz(szProvider);

	// LATER:  If we didn't find a provider that matched this ID, we might
	// want to check to see if the ID is an index into the list of
	// available providers.  (This could be complicated because the order
	// of entries in the registry could change.)

	// We're done with the list of provider IDs, so we can release it here.
	pIVisListSzProvider->Release();
	pIVisListSzProvider = 0;

	// Fail if we can't get the provider.
	if (pIVisImSrcProvider == 0)
		return  imagesource;

	// Find the start of the first non-space character after the colon in the
	// string identifying the image source.  (We increment ich in the loop
	// to handle the case of an empty image source ID string.)
	while ((szImageSourceID[ich] == ':') || (szImageSourceID[ich] == ' '))
	{
		++ich;
	}

	// Attempt to get the image source interface from the provider.
	IVisImSrcDevice *pIVisImSrcDevice = 0;
	if ((szImageSourceID[ich] >= '0')
		&& (szImageSourceID[ich] <= '9')
		&& ((szImageSourceID[ich + 1] == 0)
			|| ((szImageSourceID[ich + 1] >= '0')
				&& (szImageSourceID[ich + 1] <= '9')
				&& (szImageSourceID[ich + 2] == 0))))
	{
		// If the device ID string consists of a single digit or a pair
		// of digits, use the number as an index specifying the device.
		int iDevice = szImageSourceID[ich] - '0';
		if (szImageSourceID[ich + 1] != 0)
			iDevice = 10 * iDevice + szImageSourceID[ich + 1] - '0';

		pIVisImSrcProvider->GetDeviceI(&pIVisImSrcDevice, iDevice);
	}
	else
	{
		// Pass the string to the provider and let the provider parse the string
		// (and look at reg entries) to find the appropriate device.  (Note that
		// the provider could choose to use a default device if we pass it an
		// empty string.)
		pIVisImSrcProvider->GetDeviceSz(&pIVisImSrcDevice, szImageSourceID + ich);
	}

	// We're done with the  provider, so we can release it here.
	pIVisImSrcProvider->Release();
	pIVisImSrcProvider = 0;

	// Fail if we don't have an image source interface.
	if (pIVisImSrcDevice == 0)
		return imagesource;

	// If we have an image source interface, we'll need to construct a
	// CVisImageSource object from it.
	CVisImageSource imagesourceT(pIVisImSrcDevice);
	imagesource = imagesourceT;
	pIVisImSrcDevice->Release();
	pIVisImSrcDevice = 0;

	return imagesource;
}

VisImSrcExport CVisImageSource VISAPI VisFindImageSourceHwnd(HWND hWndParent,
		bool fAlwaysAsk)
{
	char szProviderDefault[256];
	char szDeviceDefault[256];
	bool fSetDefault = false;

	*szProviderDefault = 0;
	*szDeviceDefault = 0;

	// First, look in the registry to find default values and to see if we
	// should always diaplay selection dialogs to the user.
	HKEY hkeyT;
	bool fAlwaysAskReg = false;
	if (VisFOpenRegKey(&hkeyT, VIS_SZ_REG_IMSRC_DEVS))
	{
		// OR the value of the "Always Ask" flag passed to this function with
		// the value found in the registry.
		if (!fAlwaysAsk)
		{
			VisFReadBoolFromReg(&fAlwaysAskReg, hkeyT, "AlwaysAsk");
			fAlwaysAsk = fAlwaysAskReg;
		}

		VisFReadSzFromReg(szProviderDefault, sizeof(szProviderDefault),
				hkeyT, "Default");

		if (*szProviderDefault != 0)
		{
			HKEY hkeySubkeyT;
			if (VisFOpenRegKey(&hkeySubkeyT, szProviderDefault, hkeyT))
			{
				VisFReadSzFromReg(szDeviceDefault, sizeof(szDeviceDefault),
						hkeySubkeyT, "Default");

				RegCloseKey(hkeySubkeyT);
			}
		}

		RegCloseKey(hkeyT);
		hkeyT = 0;
	}

	if (*szDeviceDefault == 0)
	{
		// If we didn't get a default value from the registry, we'll
		// probably want to set the device selected as the default.
		fSetDefault = true;
	}
	else if (!fAlwaysAsk)
	{
		// If we could read a default device and we don't want to always
		// ask the user, try using the default device.
		std::string strT = szProviderDefault;
		strT += ": ";
		strT += szDeviceDefault;

		CVisImageSource imagesourceT = VisFindImageSource(strT.c_str());
		if (imagesourceT.IsValid())
			return imagesourceT;
	}

	// If we didn't find a default image source that we could use, try displaying
	// dialogs to let the user choose an image source.
	IVisImSrcDevice *pIVisImSrcDevice = 0;
	IVisListSz *pIVisListSzProvider = 0;
	VisGetProviderList(&pIVisListSzProvider);

	if ((pIVisListSzProvider == 0)
			|| (pIVisListSzProvider->CszGet() == 0))
	{
		MessageBox(hWndParent, 
				"No image source providers were found in the registry.", 
				"WARNING", MB_OK);
	}
	else
	{
		while (pIVisImSrcDevice == 0)
		{
			std::string strMsg;

			long iszProvider;
			for (iszProvider = 0; iszProvider < pIVisListSzProvider->CszGet();
					++iszProvider)
			{
				char szT[256];
				assert(*(pIVisListSzProvider->SzGetI(iszProvider)) != 0);
				assert(strlen(pIVisListSzProvider->SzGetI(iszProvider))
						< sizeof(szT));
				strncpy(szT, pIVisListSzProvider->SzGetI(iszProvider),
						sizeof(szT) - 1);
				szT[sizeof(szT) - 1] = 0;

				char *pch = strchr(szT, ':');
				if (pch != 0)
					*pch = 0;

				if (stricmp(szProviderDefault, szT) == 0)
					break;
			}

			if (iszProvider == pIVisListSzProvider->CszGet())
				iszProvider = - 1;

			iszProvider = VisDoImSrcDlgHwnd("Select an Image Source Provider",
					pIVisListSzProvider, hWndParent, iszProvider);
			assert((iszProvider >= -1)
					&& (iszProvider < pIVisListSzProvider->CszGet()));

			// If we cancelled the provider selection dialog, break out of the
			// loop.
			if (iszProvider == - 1)
				break;

			// Attempt to get an IVisImSrcProvider.
			// Note:  We can get szProvider DLL by looking at subkey iszProvider.
			// We really should lock the registry while doing this dialog, but
			// we don't need to in pratice because apps using cameras won't be
			// in this dialog when we change the registry (while adding a new
			// device).
			IVisImSrcProvider *pIVisImSrcProvider
					= VisPIVisImSrcProviderGetI(iszProvider);
//			IVisImSrcProvider *pIVisImSrcProvider
//					= VisPIVisImSrcProviderGetSz(szSelect);

			if (pIVisImSrcProvider == 0)
			{
				strMsg = "The provider '";
				strMsg += pIVisListSzProvider->SzGetI(iszProvider);
				strMsg += "' is not available.";

				if (pIVisListSzProvider->CszGet() == 1)
				{
					MessageBox(hWndParent, strMsg.c_str(), "WARNING", MB_OK);
					break;
				}

				strMsg += "  Do you want to choose a different provider?";

				if (MessageBox(hWndParent, strMsg.c_str(), "WARNING", MB_YESNO) == IDYES)
					continue;
				else
					break;
			}

			assert(pIVisImSrcProvider != 0);

			IVisListSz *pIVisListSzDevice = 0;
			pIVisImSrcProvider->GetConnectedDeviceList(&pIVisListSzDevice);

			if ((pIVisListSzDevice == 0)
					|| (pIVisListSzDevice->CszGet() == 0))
			{
				// See if a default device is available.
				pIVisImSrcProvider->GetDeviceSz(&pIVisImSrcDevice, "");
				if (pIVisImSrcDevice != 0)
				{
					// Use the default device.
					if (pIVisListSzDevice != 0)
						pIVisListSzDevice->Release();
					pIVisListSzDevice = 0;
					break;
				}

				strMsg = "The provider '";
				strMsg += pIVisListSzProvider->SzGetI(iszProvider);
				strMsg += "' does not have any connected devices.";

				if (pIVisListSzProvider->CszGet() == 1)
				{
					MessageBox(hWndParent, strMsg.c_str(), "WARNING", MB_OK);
					break;
				}

				strMsg += "  Do you want to choose a different provider?";

				// Release the provider and list of devices.
				if (pIVisListSzDevice != 0)
					pIVisListSzDevice->Release();
				pIVisListSzDevice = 0;
				pIVisImSrcProvider->Release();
				pIVisImSrcProvider = 0;

				if (MessageBox(hWndParent, strMsg.c_str(), "Warning", MB_YESNO) == IDYES)
					continue;
				else
					break;
			}

SelectDevice:
			assert(pIVisListSzDevice != 0);

			long iszDevice = - 1;

			if (pIVisListSzDevice->CszGet() > 0)
			{
				// Look in the registry under the provider's key to find
				// the default device for the provider.
				iszDevice = - 1;
				std::string strT = VIS_SZ_REG_IMSRC_DEVS "\\";
				strT += pIVisListSzProvider->SzGetI(iszProvider);
				if (VisFOpenRegKey(&hkeyT, strT.c_str()))
				{
					if (VisFReadSzFromReg(szDeviceDefault,
									sizeof(szDeviceDefault), hkeyT, "Default")
							&& (*szDeviceDefault != 0))
					{
						iszDevice = pIVisListSzDevice->IGetSz(szDeviceDefault);
					}

					RegCloseKey(hkeyT);
					hkeyT = 0;
				}

				assert(iszDevice >= - 1);
				CVisDlgListSz dlg("Select an Image Source Device",
						pIVisListSzDevice, 0, iszDevice);

				dlg.ShowCheckboxes(fAlwaysAskReg, fSetDefault);

				if (dlg.DoModal() == IDOK)
				{
					iszDevice = dlg.ISelection();

					if (dlg.m_fAlwaysAsk)
						fAlwaysAskReg = true;
					else
						fAlwaysAskReg = false;

					if (dlg.m_fSetDefault)
						fSetDefault = true;
					else
						fSetDefault = false;
				}
			}

			assert((iszDevice >= -1)
					&& (iszDevice < pIVisListSzDevice->CszGet()));


			if (iszDevice == -1)
			{
				// Release the provider and list of devices.
				if (pIVisListSzDevice != 0)
					pIVisListSzDevice->Release();
				pIVisListSzDevice = 0;
				pIVisImSrcProvider->Release();
				pIVisImSrcProvider = 0;

				if ((pIVisListSzProvider->CszGet() > 1)
						&& (MessageBox(hWndParent, 
								"Do you want to choose a different provider?","WARNING", 
								MB_YESNO) == IDYES))
				{
					continue;
				}
				else
				{
					break;
				}
			}
			
			pIVisImSrcProvider->GetDeviceI(&pIVisImSrcDevice, iszDevice);

			if (pIVisImSrcDevice != 0)
			{
				// Since we found a device, we should update the registry entries.
				if (VisFOpenRegKey(&hkeyT, VIS_SZ_REG_IMSRC_DEVS))
				{
					VisFWriteBoolToReg(fAlwaysAskReg, hkeyT, "AlwaysAsk");

					assert(*(pIVisListSzProvider->SzGetI(iszProvider)) != 0);
					assert(strlen(pIVisListSzProvider->SzGetI(iszProvider))
							< sizeof(szProviderDefault));
					strncpy(szProviderDefault,
							pIVisListSzProvider->SzGetI(iszProvider),
							sizeof(szProviderDefault) - 1);
					szProviderDefault[sizeof(szProviderDefault) - 1] = 0;

					char *pch = strchr(szProviderDefault, ':');
					if (pch != 0)
						*pch = 0;

					VisFWriteSzToReg(szProviderDefault, hkeyT, "Default");

					HKEY hkeySubkeyT;
					if (VisFOpenRegKey(&hkeySubkeyT, szProviderDefault, hkeyT))
					{
						assert(*(pIVisListSzDevice->SzGetI(iszDevice)) != 0);
						VisFWriteSzToReg(pIVisListSzDevice->SzGetI(iszDevice),
								hkeySubkeyT, "Default");

						RegCloseKey(hkeySubkeyT);
					}

					RegCloseKey(hkeyT);
					hkeyT = 0;
				}
			}
			else
			{
				strMsg = "The device '";
				strMsg += pIVisListSzDevice->SzGetI(iszDevice);
				strMsg += "' is not available.";

				if ((pIVisListSzProvider->CszGet() == 1)
						&& (pIVisListSzDevice->CszGet() == 1))
				{
					// Release the provider and list of devices.
					if (pIVisListSzDevice != 0)
						pIVisListSzDevice->Release();
					pIVisListSzDevice = 0;
					pIVisImSrcProvider->Release();
					pIVisImSrcProvider = 0;

					MessageBox(hWndParent, strMsg.c_str(), "WARNING", MB_OK);
					break;
				}

				strMsg += "Do you want to choose a different ";

				if (pIVisListSzDevice->CszGet() > 1)
					strMsg +="device?";
				else
					strMsg +="provider?";

				if (MessageBox(hWndParent, strMsg.c_str(), "WARNING", MB_YESNO) == IDYES)
				{
					if (pIVisListSzDevice->CszGet() > 1)
					{
						goto SelectDevice;  // LATER:  Get rid of this goto statement?
					}
					else
					{
						// Release the provider and list of devices.
						if (pIVisListSzDevice != 0)
							pIVisListSzDevice->Release();
						pIVisListSzDevice = 0;
						pIVisImSrcProvider->Release();
						pIVisImSrcProvider = 0;

						continue;
					}
				}
				else
				{
					// Release the provider and list of devices.
					if (pIVisListSzDevice != 0)
						pIVisListSzDevice->Release();
					pIVisListSzDevice = 0;
					pIVisImSrcProvider->Release();
					pIVisImSrcProvider = 0;

					break;
				}
			}

			// Release the provider and list of devices.
			if (pIVisListSzDevice != 0)
				pIVisListSzDevice->Release();
			pIVisListSzDevice = 0;
			pIVisImSrcProvider->Release();
			pIVisImSrcProvider = 0;
		}
	}

	if (pIVisListSzProvider != 0)
	{
		pIVisListSzProvider->Release();
		pIVisListSzProvider = 0;
	}

	CVisImageSource imagesource(pIVisImSrcDevice);
	if (pIVisImSrcDevice != 0)
		pIVisImSrcDevice->Release();

	return imagesource;
}


VisImSrcExport void VISAPI VisAddProviderRegEntry(const char *szID, const char *szName,
		const char *szDLL, bool fAvailable)
{
	VisInlineAddProviderRegEntry(szID, szName, szDLL, fAvailable);
}
