// VisPump.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved



inline CVisPump::~CVisPump(void)
{
	KillThread();

	CloseHandle(m_hEndOfProcessingStep);
	CloseHandle(m_hPaused);
	CloseHandle(m_hRunning);
}



inline CVisPump::T_PfnPumpCallback CVisPump::PfnCallback(void) const
{
	return m_pfnCallback;
}

inline void CVisPump::SetPfnCallback(CVisPump::T_PfnPumpCallback pfnCallback)
{
	m_pfnCallback = pfnCallback;
}

inline void *CVisPump::PvUser(void) const
{
	return m_pvUser;
}

inline void CVisPump::SetPvUser(void *pvUser)
{
	m_pvUser = pvUser;
}

inline const HANDLE CVisPump::HThread(void) const
{
	return m_hThread;
}

inline HANDLE CVisPump::HThread(void)
{
	return m_hThread;
}

inline HANDLE CVisPump::HRunning(void) const
{
	return m_hRunning;
}

inline HANDLE CVisPump::HPaused(void) const
{
	return m_hPaused;
}

inline HANDLE CVisPump::HEndOfProcessingStep(void) const
{
	return m_hEndOfProcessingStep;
}


inline bool CVisPump::IsRunning(void) const
{
	return m_fIsProcessing;
}

inline void CVisPump::SetFIsProcessing(bool f)
{
	m_fIsProcessing = f;
}

inline bool CVisPump::FKillThread(void) const
{
	return m_fKillThread;
}

inline void CVisPump::SetFKillThread(bool f)
{
	m_fKillThread = f;
}

inline bool CVisPump::FOnlyProcessNext(void) const
{
	return m_fOnlyProcessNext;
}

inline void CVisPump::SetFOnlyProcessNext(bool f)
{
	m_fOnlyProcessNext = f;
}

inline bool CVisPump::FSignalEndOfProcessingStep(void) const
{
	return (m_lSignalEndOfProcessingStep != 0);
}

inline void CVisPump::SetFSignalEndOfProcessingStep(bool f)
{
	m_lSignalEndOfProcessingStep = ((f != 0) ? 1 : 0);
}

inline bool CVisPump::ExchangeFSignalEndOfProcessingStep(bool f)
{
	return (InterlockedExchange(&m_lSignalEndOfProcessingStep, (long) f) != 0);
}

inline bool CVisPump::FStarted(void) const
{
	return m_fStarted;
}

inline bool CVisPump::FStepping(void) const
{
	return m_fStepping;
}

inline void CVisPump::SetFStepping(bool f)
{
	m_fStepping = f;
}

inline bool CVisPump::FStepSucceeded(void) const
{
	return m_fStepSucceeded;
}

inline void CVisPump::SetFStepSucceeded(bool f)
{
	m_fStepSucceeded = f;
}

inline void CVisPump::ClearHThread(void)
{
	m_hThread = 0;
}
