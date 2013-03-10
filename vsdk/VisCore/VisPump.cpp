// VisPump.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



VisCoreExport CVisPump::CVisPump(T_PfnPumpCallback pfnCallback, void *pvUser,
		bool fStartSuspended)
  : m_pfnCallback(pfnCallback),
	m_pvUser(pvUser),
	m_lSignalEndOfProcessingStep(0),
	m_fKillThread(false),
	m_fIsProcessing(false),
	m_fOnlyProcessNext(false),
	m_fStarted(false),
	m_fStepping(false),
	m_fStepSucceeded(false)
{
	m_hRunning = CreateEvent(0, true, false, 0);
	m_hPaused = CreateEvent(0, true, true, 0);
	m_hEndOfProcessingStep = CreateEvent(0, true, true, 0);

	m_hThread = (HANDLE )_beginthreadex(NULL, 0, &StaticThreadMain, this, 
										CREATE_SUSPENDED, &m_nThreadId);

	if (!fStartSuspended)
		StartThread();
}


VisCoreExport void CVisPump::StartThread(bool fRunning)
{
	assert(!m_fStarted);
	
	if (m_hThread != 0)
	{
		m_fIsProcessing = fRunning;
		::ResumeThread(m_hThread);
		m_fStarted = true;
	}
	
}


VisCoreExport BOOL CVisPump::SetThreadPriority(int nPriority)
{
	if (m_hThread != 0)
		return ::SetThreadPriority(m_hThread, nPriority);

	return 0;
}


VisCoreExport void CVisPump::KillThread(void)
{
	
	if (HThread() != 0)
	{
		m_fKillThread = true;
		m_fIsProcessing = false;
		SetEvent(m_hRunning);

		if (!m_fStarted)
			StartThread(true);

		VisWaitForSingleObject(HThread(), INFINITE);
		ClearHThread();

	}
}


VisCoreExport UINT __stdcall CVisPump::StaticThreadMain(void *pvPump)
{
	assert(pvPump != 0);

	UINT nRet = ((CVisPump *) pvPump)->ThreadMain();

	return nRet;
}



VisCoreExport UINT CVisPump::ThreadMain(void)
{
	// Warning:  The CVisPump ThreadMain, Run, Step, and Pause methods
	// were written to work together to avoid deadlock.  Be very careful
	// if you attempt to modify them.
	while (!m_fKillThread)
	{
		if (IsRunning())
		{
			if (FOnlyProcessNext())
			{
				SetFOnlyProcessNext(false);
				SetFIsProcessing(false);
			}

			if (!PumpCore())
				m_fKillThread = true;

			if (FSignalEndOfProcessingStep())
			{
				SetFSignalEndOfProcessingStep(false);
				SetEvent(HEndOfProcessingStep());
			}
		}
		else if (!m_fKillThread)
		{
			// Note:  We need to check (!m_fKillThread) above to avoid
			// a potential deadlock when killing the thread.
			ResetEvent(m_hRunning);
			SetEvent(m_hPaused);
			VisWaitForSingleObject(m_hRunning, INFINITE);
			ResetEvent(m_hPaused);
		}
	}

	// Set events that Pause() or Step() may be expecting.
	SetFIsProcessing(false);
	SetEvent(HPaused());
	SetEvent(HEndOfProcessingStep());

	return 0;
}


VisCoreExport bool CVisPump::PumpCore(void)
{
	if (m_pfnCallback != 0)
	{
		return m_pfnCallback(this);
	}

	// Don't do anything (and end the thread) if there's no callback function.
	return false;
}







VisCoreExport bool CVisPump::Run(void)
{
	// Warning:  The CVisPump ThreadMain, Run, Step, and Pause methods
	// were written to work together to avoid deadlock.  Be very careful
	// if you attempt to modify them.
	bool fRet = false;
	
	if ((HThread() != 0) && (!FKillThread()))
	{
		OnRun();

		m_fIsProcessing = true;

		if (!m_fStarted)
			StartThread(true);

		fRet = (SetEvent(m_hRunning) != 0);
	}

	return fRet;
}


VisCoreExport bool CVisPump::Step(void)
{
	// Warning:  The CVisPump ThreadMain, Run, Step, and Pause methods
	// were written to work together to avoid deadlock.  Be very careful
	// if you attempt to modify them.
	bool fWasProcessing = m_fIsProcessing;

	m_fStepping = true;

	m_fStepSucceeded = false;

	if ((HThread() != 0) && (!FKillThread()))
	{
		if (fWasProcessing)
		{
			assert(m_fStarted);
			m_fStepSucceeded = true;

			ResetEvent(m_hEndOfProcessingStep);
			SetFSignalEndOfProcessingStep(true);

			// Wait until we're done with this processing step.
			// (Need to check IsRunning() again to avoid deadlock
			// when PumpCore() return false.)
			if (IsRunning())
				VisWaitForSingleObject(m_hEndOfProcessingStep, INFINITE);
		}
		else
		{
			m_fOnlyProcessNext = true;
			m_fIsProcessing = true;
			
			ResetEvent(m_hPaused);
			m_fStepSucceeded = (SetEvent(m_hRunning) != 0);

			if (m_fStepSucceeded)
			{
				if (!m_fStarted)
					StartThread(true);

				// Wait until we have finished this processing step and pasued again.
				VisWaitForSingleObject(m_hPaused, INFINITE);
			}
		}
	}

	m_fStepping = false;

	return m_fStepSucceeded;
}


VisCoreExport void CVisPump::Pause(void)
{
	// Warning:  The CVisPump ThreadMain, Run, Step, and Pause methods
	// were written to work together to avoid deadlock.  Be very careful
	// if you attempt to modify them.
	if ((HThread() != 0) && (!FKillThread()))
	{
		m_fIsProcessing = false;

		if (!m_fStarted)
			StartThread(false);

		// Wait until we have finished current capture
		VisWaitForSingleObject(m_hPaused, INFINITE);

		OnPause();
	}
}


VisCoreExport void CVisPump::OnRun(void)
{
	// Don't do anything in the base class.
}


VisCoreExport void CVisPump::OnPause(void)
{
	// Don't do anything in the base class.
}
