// VisTimer2.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved


class CVisTimer2
{
public:
	enum { eiCurrentTimer = - 1 };


	VisCoreExport CVisTimer2(int nTimers = 1);
	VisCoreExport ~CVisTimer2(void);


	// The number of timers
	int NTimers(void) const;

	// The index of the current timer.
	int ICur(void) const;

	// Incerment the index of the current timer (modulo NTimers()).
	void IncICur(void);

	// Set the index of the current timer.
	void SetICur(int i);


	// Find the elapsed time (in milliseconds) for the specified timer.
	// This method does not reset the timer.
	VisCoreExport unsigned int CmsElapsed(int i) const;

	// Find the elapsed time (in milliseconds) for the current timer and
	// increment index of the current timer.  This method does not reset
	// the timer.
	VisCoreExport unsigned int CmsElapsed(void);

	// Find the elapsed time (in milliseconds) for the specified timer.
	// If eiCurrentTimer is specified, the index of the current timer is
	// incremented.  This method resets the timer to the current time.
	VisCoreExport unsigned int CmsUpdate(int i = eiCurrentTimer);

	// Find the average rate at which the timers have been updated.  This
	// method is used in some applications to find frame rates averaged
	// over NTimers() frames.
	//
	// The first time that this method is called after calling Reset(),
	// it will return - 1.0.  The next (NTimers() - 1) times that this
	// method is called, it will use the elapsed time since the first call
	// after calling Reset() to find the rate.  After that, the rate
	// will be averaged over the most recent NTimers() calls to this method.
	//
	// The current timer is reset and the index of the current timer is
	// incremented in this method.
	VisCoreExport double RateUpdate(void);

	// Reset all timers to the current time.
	VisCoreExport void Reset(void);


private:
	LARGE_INTEGER m_liPerformanceFrequency;  // LATER:  This could be static
	int m_nTimers;
	int m_iCur;
	LARGE_INTEGER *m_rgliPerformanceCounter;
	int m_cSpecialRateUpdateAfterReset;

	unsigned int CmsElapsed(int i,
			const LARGE_INTEGER& refliPerformanceCounter) const;
};


#include "VisTimer2.inl"
