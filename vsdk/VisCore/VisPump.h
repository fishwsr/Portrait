// VisPump.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_VISPUMP_H__
#define __VIS_CORE_VISPUMP_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// LATER:  This class is very similar to the CThread class.  Perhaps we
// should derive this class from CThread.

// LATER:  We might want an option to make these objects self-deleting
// so that the thread is not destroyed when they go out of scope.

class CVisPump
{
public:
	typedef bool (VISAPI *T_PfnPumpCallback)(CVisPump *ppump);

	VisCoreExport CVisPump(T_PfnPumpCallback pfnCallback = 0, void *pvUser = 0,
			bool fStartSuspended = true);
	virtual ~CVisPump(void);


	T_PfnPumpCallback PfnCallback(void) const;
	void SetPfnCallback(T_PfnPumpCallback pfnCallback);

	void *PvUser(void) const;
	void SetPvUser(void *pvUser);


	bool IsRunning(void) const;
	

	// (The Run, Step, and Pause methods are not synchronized because 
	VisCoreExport virtual bool Run(void);
	VisCoreExport virtual bool Step(void);
	VisCoreExport virtual void Pause(void);
	VisCoreExport virtual void KillThread(void);


	VisCoreExport BOOL SetThreadPriority(int nPriority);


protected:

	const HANDLE HThread(void) const;
	
	HANDLE HThread(void);


	// ThreadMain is called when the thread is started.  When ThreadMain
	// returns, the thread will exit.  The default implementation loops
	// while FKillThread() is false, calling PumpCore() during each
	// iteration (and breaks out of the loop if PumpCore() returns false).
	// Override this method to replace the default thread behavior.
	VisCoreExport virtual UINT ThreadMain(void);

	// PumpCore is called repeatedly by the default implementation of
	// ThreadMain until the thread is killed or PumpCore returns false.
	// The default implementation calls the callback function if the
	// pointer to the callback function is not NULL.  If false is returned,
	// the thread will exit.  Override this method to repeatedly perform
	// some other action in this thread.
	VisCoreExport virtual bool PumpCore(void);


	// Called in Run() just before the thread is started running.
	VisCoreExport virtual void OnRun(void);

	// Called in Pause() just after the thread is paused.
	VisCoreExport virtual void OnPause(void);


	// Call StartThread to start a thread if the StartSuspended option was used
	// when the CVisPump was created.  (This is called by the default implementation
	// of Run(), Step(), Pause(), and KillThread().)
	VisCoreExport void StartThread(bool fRunning = true);


	HANDLE HRunning(void) const;
	HANDLE HPaused(void) const;
	HANDLE HEndOfProcessingStep(void) const;

	bool FKillThread(void) const;
	bool FOnlyProcessNext(void) const;
	bool FSignalEndOfProcessingStep(void) const;
	bool FStarted(void) const;
	bool FStepping(void) const;
	bool FStepSucceeded(void) const;

	void SetFKillThread(bool f);
	void SetFIsProcessing(bool f);
	void SetFOnlyProcessNext(bool f);
	void SetFSignalEndOfProcessingStep(bool f);
	bool ExchangeFSignalEndOfProcessingStep(bool f);
	void SetFStepping(bool f);
	void SetFStepSucceeded(bool f);

	void ClearHThread(void);


private:

	T_PfnPumpCallback m_pfnCallback;
	void *m_pvUser;
	HANDLE m_hThread;
	unsigned int m_nThreadId;

	HANDLE m_hRunning;
	HANDLE m_hPaused;
	HANDLE m_hEndOfProcessingStep;
	long m_lSignalEndOfProcessingStep;
	bool m_fKillThread;
	bool m_fIsProcessing;
	bool m_fOnlyProcessNext;
	bool m_fStarted;
	bool m_fStepping;
	bool m_fStepSucceeded;

	static VisCoreExport UINT __stdcall StaticThreadMain(void *pvPump);
};


#include "VisPump.inl"


#endif // __VIS_CORE_VISPUMP_H__
