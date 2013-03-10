
// VisImSrcNoMFC.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_IMSRC_IMSRCNOMFC_INL__
#define __VIS_IMSRC_IMSRCNOMFC_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <string>


#if defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC
#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG
#endif // defined(_WINDLL) || defined(_AFXDLL)


inline IVisListSz *VisInlineGetSubkeyList(HKEY hkeyRoot, const char *szKey,
		int eviskeylist)
{
	IVisListSz *pIVisListSz = 0;
	HKEY hkey;

	if (RegOpenKeyEx(hkeyRoot, szKey,
			0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
			&hkey) == ERROR_SUCCESS)
	{
		CVisListSz *pListSz = new CVisListSz;

		if (pListSz == 0)
			return 0;

		DWORD iSubkeyLim = 0;
		RegQueryInfoKey(hkey, 0, 0, 0, &iSubkeyLim, 0, 0, 0, 0, 0, 0, 0);
		for (DWORD iSubkey = 0; iSubkey < iSubkeyLim; ++iSubkey)
		{
			char szSubkey[256];
			char szValue[256];
			DWORD cbSubkey = sizeof(szSubkey);
			DWORD cbValue = sizeof(szValue);
			DWORD dwType;
			HKEY hkeySubkey;
			if ((RegEnumKeyEx(hkey, iSubkey, szSubkey, &cbSubkey, 0, 0,
							0, 0) == ERROR_SUCCESS)
					&& (RegOpenKeyEx(hkey, szSubkey, 0, KEY_QUERY_VALUE,
						&hkeySubkey) == ERROR_SUCCESS))
			{
				bool fAvailable;
				if ((eviskeylist & eviskeylistOnlyAvailableSubkeys) == 0)
				{
					fAvailable = true;
				}
				else
				{
					fAvailable = false;

					cbValue = sizeof(szValue);
					if (RegQueryValueEx(hkeySubkey, "Available", 0, &dwType,
									(BYTE *) szValue, &cbValue)
									== ERROR_SUCCESS)
					{
						if (((dwType == REG_SZ)
									&& (szValue[0] == '1')
									&& (szValue[1] == 0))
								|| ((dwType == REG_DWORD)
									&& (*((DWORD *) szValue) == 1)))
						{
							fAvailable = true;
						}
					}
				}

				if (fAvailable)
				{
					std::string strT;

					if ((eviskeylist & eviskeylistID) != 0)
					{
						strT = szSubkey;
					}

					if ((eviskeylist & eviskeylistDefaultValue) != 0)
					{
						cbValue = sizeof(szValue);
						if ((RegQueryValueEx(hkeySubkey, 0, 0, &dwType,
										(BYTE *) szValue, &cbValue)
										== ERROR_SUCCESS)
								&& (dwType == REG_SZ))
						{
							if (!strT.empty())
							{
								strT += ": ";
							}

							strT += szValue;
						}
					}
							
					if (!strT.empty())
					{
						pListSz->AddSz(strT.c_str());
					}
				}

				RegCloseKey(hkeySubkey);
			}
		}

		RegCloseKey(hkey);
		
		if (pListSz->CszGet() > 0)
			pIVisListSz = pListSz;
		else
			delete pListSz;
	}

	return pIVisListSz;
}


inline bool VisInlineFIsSzKeyAvailable(const char *szKey, HKEY hkeyRoot)
{
	bool fRet = false;

	assert(szKey != 0);
	assert(*szKey != 0);

	HKEY hkey;
	if (RegOpenKeyEx(hkeyRoot, szKey, 0, KEY_QUERY_VALUE, &hkey)
			== ERROR_SUCCESS)
	{
		DWORD dwType;
		char szValue[256];
		DWORD cbValue = sizeof(szValue);
		if (RegQueryValueEx(hkey, "Available", 0, &dwType,
				(BYTE *) szValue, &cbValue) == ERROR_SUCCESS)
		{
			if (((dwType == REG_SZ)
						&& (szValue[0] == '1')
						&& (szValue[1] == 0))
					|| ((dwType == REG_DWORD)
						&& (*((DWORD *) szValue) == 1)))
			{
				fRet = true;
			}
		}
	}

	return fRet;
}


inline void VisInlineAddProviderRegEntry(const char *szID, const char *szName,
		const char *szDLL, bool fAvailable)
{
	assert(szID != 0);
	assert(szName != 0);
	assert(szDLL != 0);

	std::string strKey = VIS_SZ_REG_IMSRC_DEVS "\\";
	strKey += szID;

	HKEY hkey;
	if (VisFCreateRegKey(&hkey, strKey.c_str()))
	{
		VisFWriteSzToReg(szName, hkey);
		VisFWriteSzToReg(szDLL, hkey, "DLL");
		VisFWriteBoolToReg(fAvailable, hkey, "Available");
		VisFCloseRegKey(hkey);
	}
}


#if defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC
#ifdef _DEBUG
#undef new
#endif // _DEBUG
#endif // defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC


#endif // !__VIS_IMSRC_IMSRCNOMFC_INL__
