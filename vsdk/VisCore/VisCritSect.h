// VisCritSect.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// UNDONE:  Move this to VisCoreProj.h?
#include <string>


// This should be INFINITE unless we want to try to debug deadlock
// situations.
const DWORD kviscmsTimeoutDebug = INFINITE;
// const DWORD kviscmsTimeoutDebug = 30 * 60 * 1000;  // 30 minutes


// Helper function to find possible deadlock situations.
inline DWORD VisWaitForSingleObject(HANDLE h, DWORD cmsTimeout)
{
#ifdef _DEBUG
	if (cmsTimeout == INFINITE)
	{
		// Special code to try to catch deadlocks after 30 minutes.
		DWORD dwRet = WaitForSingleObject(h, kviscmsTimeoutDebug);

		assert(dwRet != WAIT_TIMEOUT);

		return dwRet;
	}

	return WaitForSingleObject(h, cmsTimeout);
#else // _DEBUG
	return WaitForSingleObject(h, cmsTimeout);
#endif // _DEBUG
}


// Helper function to find possible deadlock situations.
inline DWORD VisWaitForMultipleObjects(DWORD cHandle, const HANDLE *rgh, 
		bool fWaitAll, DWORD cmsTimeout)
{
#ifdef _DEBUG
	if (cmsTimeout == INFINITE)
	{
		// Special code to try to catch deadlocks after 30 minutes.
		DWORD dwRet = WaitForMultipleObjects(cHandle, rgh, fWaitAll,
				kviscmsTimeoutDebug);

		assert(dwRet != WAIT_TIMEOUT);

		return dwRet;
	}

	return WaitForMultipleObjects(cHandle, rgh, fWaitAll, cmsTimeout);
#else // _DEBUG
	return WaitForMultipleObjects(cHandle, rgh, fWaitAll, cmsTimeout);
#endif // _DEBUG
}


// Helper class.
// LATER:  Move this to another header file.
class CVisCritSect
{
public:
	inline CVisCritSect(void)
		{ InitializeCriticalSection(&m_criticalsection); }
	inline ~CVisCritSect(void)
		{ DeleteCriticalSection(&m_criticalsection); }
	inline void Enter(void)
		{ EnterCriticalSection(&m_criticalsection); }
	inline void Leave(void)
		{ LeaveCriticalSection(&m_criticalsection); }
protected:
	CRITICAL_SECTION m_criticalsection;
};


// This class uses a magic number to try to avoid problems
// when the static variable of this type has been destroyed.
template<int t_wMagicNumber>
class CVisStaticCritSect
{
public:
	inline CVisStaticCritSect(void)
	  : m_wMagicNumber(t_wMagicNumber)
	{ 
		InitializeCriticalSection(&m_criticalsection);
	}
	inline ~CVisStaticCritSect(void)
	{ 
		m_wMagicNumber = ~ t_wMagicNumber;
		DeleteCriticalSection(&m_criticalsection);
	}
	inline void Enter(void)
	{
		if (m_wMagicNumber == t_wMagicNumber)
			EnterCriticalSection(&m_criticalsection);
	}
	inline void Leave(void)
	{
		if (m_wMagicNumber == t_wMagicNumber)
			LeaveCriticalSection(&m_criticalsection);
	}
protected:
	CRITICAL_SECTION m_criticalsection;
	int m_wMagicNumber;
};



class CVisMutex
{
public:
	inline CVisMutex(const char *szName)
	  : m_strName(szName)
	{
		m_hMutex = CreateMutex(0, FALSE, szName);
		assert(m_hMutex != 0);
	}
	inline ~CVisMutex(void)
	{
		assert(m_hMutex != 0);
		CloseHandle(m_hMutex);
		m_hMutex = 0;
	}
	inline void Enter(void)
	{
		assert(m_hMutex != 0);
		VisWaitForSingleObject(m_hMutex, INFINITE);
	}
	inline void Leave(void)
	{
		assert(m_hMutex != 0);
		ReleaseMutex(m_hMutex);
	}
private:
	HANDLE m_hMutex;
	std::string m_strName;
};
