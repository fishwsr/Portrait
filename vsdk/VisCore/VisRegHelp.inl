// VisRegHelp.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


inline bool VisFCreateRegKey(HKEY *phkey, const char *szKey,
		HKEY hkeyBase)
{
	assert(szKey != 0);
	
	if (RegCreateKeyEx(hkeyBase, szKey, 0, "Vis", REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, 0, phkey, 0) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}


inline bool VisFOpenRegKey(HKEY *phkey, const char *szKey,
		HKEY hkeyBase)
{
	assert(szKey != 0);
	
	if (RegOpenKeyEx(hkeyBase, szKey, 0, KEY_ALL_ACCESS, phkey)
			== ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}


inline bool VisFCloseRegKey(HKEY hkey)
{
	if (RegCloseKey(hkey) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}



inline bool VisFReadDwordFromReg(DWORD *pdw, HKEY hkey, const char *szValue)
{
	assert(pdw != 0);
	
	DWORD dwType;
	DWORD cbT = sizeof(DWORD);

	if ((RegQueryValueEx(hkey, (char *) szValue, 0, &dwType,
					(BYTE *) pdw, &cbT) == ERROR_SUCCESS)
			&& (dwType == REG_DWORD))
	{
		return true;
	}

	return false;
}


inline bool VisFReadIntFromReg(int *pi, HKEY hkey, const char *szValue)
{
	assert(sizeof(int) == sizeof(DWORD));

	return VisFReadDwordFromReg((DWORD *) pi, hkey, szValue);
}

inline bool VisFReadFloatFromReg(float *pflt, HKEY hkey, const char *szValue)
{
	assert(pflt != 0);
	
	char szT[50];
	return ((VisFReadSzFromReg(szT, sizeof(szT), hkey, szValue))
			&& (sscanf(szT, "%f", pflt) > 0));
}


inline bool VisFReadBoolFromReg(bool *pf, HKEY hkey, const char *szValue)
{
	assert(pf != 0);

	bool fRet = false;
	DWORD dwType;
	DWORD dwT;
	DWORD cbT = sizeof(dwT);

	if (RegQueryValueEx(hkey, (char *) szValue, 0, &dwType,
					(BYTE *) &dwT, &cbT) == ERROR_SUCCESS)
	{
		if (dwType == REG_DWORD)
		{
			if (dwT == 0)
			{
				*pf = false;
				fRet = true;
			}
			else if (dwT == 1)
			{
				*pf = true;
				fRet = true;
			}
		}
		else if ((dwType == REG_SZ) && (((const char *) &dwT)[1] == 0))
		{
			if ((((const char *) &dwT)[0] == '0')
					|| (((const char *) &dwT)[0] == 0))
			{
				*pf = false;
				fRet = true;
			}
			else if (((const char *) &dwT)[0] == '1')
			{
				*pf = true;
				fRet = true;
			}
		}
	}

	return fRet;
}


inline bool VisFReadSzFromReg(char *szBuf, unsigned long cbBuf,
		HKEY hkey, const char *szValue)
{
	assert(szBuf != 0);
	
	DWORD dwType;
	DWORD cbT = sizeof(DWORD);

	if ((RegQueryValueEx(hkey, (char *) szValue, 0, &dwType,
					(BYTE *) szBuf, &cbBuf) == ERROR_SUCCESS)
			&& (dwType == REG_SZ))
	{
		return true;
	}

	return false;
}



inline bool VisFWriteDwordToReg(DWORD dw, HKEY hkey, const char *szValue)
{
	if (RegSetValueEx(hkey, (char *) szValue, 0, REG_DWORD,
			(const BYTE *) &dw, sizeof(DWORD)) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}


inline bool VisFWriteIntToReg(int i, HKEY hkey, const char *szValue)
{
	return VisFWriteDwordToReg((DWORD) i, hkey, szValue);
}

inline bool VisFWriteFloatToReg(float flt, HKEY hkey, const char *szValue)
{
	char szT[50];
	sprintf(szT, "%f", flt);

	return VisFWriteSzToReg(szT, hkey, szValue);
}


inline bool VisFWriteBoolToReg(bool f, HKEY hkey, const char *szValue)
{
	return VisFWriteDwordToReg((f ? 1 : 0), hkey, szValue);
}


inline bool VisFWriteSzToReg(const char *sz, HKEY hkey, const char *szValue)
{
	assert(sz != 0);
	
	if (RegSetValueEx(hkey, szValue, 0, REG_SZ, (const BYTE *) sz,
			strlen(sz) + 1) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}
