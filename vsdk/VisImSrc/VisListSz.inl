// VisListSz.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved



#if defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC
#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG
#endif // defined(_WINDLL) || defined(_AFXDLL)



inline CVisListSz::CVisListSz(void)
  : m_cRef(1),
	m_vectorRgsz()
{
}


inline CVisListSz::~CVisListSz(void)
{
	for (int isz = 0; isz < (int) m_vectorRgsz.size(); ++isz)
	{
		assert(m_vectorRgsz[isz] != 0);
		delete[] m_vectorRgsz[isz];
	}
}


inline STDMETHODIMP CVisListSz::QueryInterface(THIS_ REFIID riid, void **ppvObject)
{
	HRESULT hrRet;

	if (ppvObject == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if ((riid == IID_IUnknown)
				|| (riid == IID_IVisListSz))
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = this;
		AddRef();
	}
	else
	{
		*ppvObject = 0;
		hrRet = ResultFromScode(E_NOINTERFACE);
	}

	return hrRet;
}


inline STDMETHODIMP_(ULONG) CVisListSz::AddRef(THIS)
{
	assert(m_cRef > 0);

	return ++m_cRef;
}


inline STDMETHODIMP_(ULONG) CVisListSz::Release(THIS)
{
	unsigned long cRef = --m_cRef;

	if (cRef == 0)
		delete this;

	return cRef;
}


inline STDMETHODIMP_(LONG) CVisListSz::CszGet(THIS) CONST
{
	return m_vectorRgsz.size();
}


inline STDMETHODIMP_(LONG) CVisListSz::IGetSz(THIS_ LPCSTR sz) CONST
{
	int iRet = -1;
	
	if (sz != 0)
	{
		for (int i = 0; i < (int) m_vectorRgsz.size(); ++i)
		{
			assert(m_vectorRgsz[i] != 0);
			if (lstrcmp(m_vectorRgsz[i], sz) == 0)
			{
				iRet = i;
				break;
			}
		}
	}

	return iRet;
}


inline STDMETHODIMP_(LPCSTR) CVisListSz::SzGetI(THIS_ LONG i) CONST
{
	LPCSTR szRet;
	
	if ((i >= 0) && (i < (int) m_vectorRgsz.size()))
		szRet = m_vectorRgsz[i];
	else
		szRet = 0;

	return szRet;
}


inline STDMETHODIMP CVisListSz::AddSz(THIS_ LPCSTR sz)
{
	HRESULT hrRet;

	if (sz == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else
	{
		int cszPrev = m_vectorRgsz.size();
		int cbsz = strlen(sz) + 1;
		LPSTR szCopy = new char[cbsz];

		if (szCopy != 0)
		{
			strcpy(szCopy, sz);
			m_vectorRgsz.push_back(szCopy);

			hrRet = ResultFromScode((cszPrev == (int) m_vectorRgsz.size()) ? E_FAIL : S_OK);
		}
		else
		{
			hrRet = ResultFromScode(E_OUTOFMEMORY);
		}
	}

	return hrRet;
}

#if defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC
#ifdef _DEBUG
#undef new
#endif // _DEBUG
#endif // defined(_WINDLL) || defined(_AFXDLL)  // Defined by MFC
