// VisSequenceList.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"

#include <process.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



VisImSrcExport CVisSequenceList::CVisSequenceList(IVisImSrcDevice *pIVisImSrcDevice,
		IVisImSrcSettings *pIVisImSrcSettings)
  : m_cRef(1),
	m_evispixfmt(evispixfmtNil),
	m_pIVisImSrcDevice(pIVisImSrcDevice),
	m_pIVisImSrcSettings(pIVisImSrcSettings),
	m_vectorSequenceList(),
	m_fWantContinuousGrab(false),
	m_fHaveContinuousGrab(false),
	m_fAllowVolatileImages(true),
	m_fWantFrame(false),
	m_fKillThreadGrabber(false),
	m_dwIDThreadGrabber(0),
	m_hThreadGrabber(0),
	m_hSemaphoreGrabber(0),
	m_fKillThreadProcessFrame(false),
	m_dwIDThreadProcessFrame(0),
	m_hThreadProcessFrame(0),
	m_hSemaphoreProcessFrame(0),
	m_fValidFrameCur(false)
{
	if (m_pIVisImSrcDevice != 0)
	{
		InitializeCriticalSection(&m_criticalsectionSequenceList);
		InitializeCriticalSection(&m_criticalsectionFrame);

		m_pIVisImSrcDevice->AddRef();

		assert(pIVisImSrcSettings != 0);
		pIVisImSrcSettings->AddRef();

		unsigned int uiThreadID;
		m_hSemaphoreGrabber = CreateSemaphore(0, 0, 1, 0);
		m_hThreadGrabber = (HANDLE) _beginthreadex(0, 0,
				StaticThreadMainGrabber, this, 0, &uiThreadID);
		m_dwIDThreadGrabber = uiThreadID;

		m_hSemaphoreProcessFrame = CreateSemaphore(0, 0, 1, 0);
		m_hThreadProcessFrame = (HANDLE) _beginthreadex(0, 0,
				StaticThreadMainProcessFrame, this, 0, &uiThreadID);
		m_dwIDThreadProcessFrame = uiThreadID;

		// Not sure if m_hThreadProcessFrame should ideally have
		// THREAD_PRIORITY_HIGHEST or THREAD_PRIORITY_TIME_CRITICAL, but
		// THREAD_PRIORITY_TIME_CRITICAL makes things work better with
		// drivers that have many THREAD_PRIORITY_TIME_CRITICAL threads.
		SetThreadPriority(m_hThreadGrabber, THREAD_PRIORITY_TIME_CRITICAL);
		SetThreadPriority(m_hThreadProcessFrame, THREAD_PRIORITY_TIME_CRITICAL);

#ifdef _DEBUG
		// Make sure that no other sequence lists are connected to this device.
		T_PfnVisImSrcFrameCallback pfnCallback = 0;
		void *pv = 0;
		if (SUCCEEDED(m_pIVisImSrcDevice->GetFrameCallback(&pfnCallback, &pv)))
			assert(pfnCallback == 0);
#endif // _DEBUG

		HRESULT hr = m_pIVisImSrcDevice->SetFrameCallback(StaticFrameCallback,
				this);
		assert(SUCCEEDED(hr));
	}
}

VisImSrcExport CVisSequenceList::~CVisSequenceList(void)
{
	// All sequences should be removed before this object is destroyed.
	assert(NSequences() == 0);

	if (m_pIVisImSrcDevice != 0)
	{
		m_pIVisImSrcDevice->SetFrameCallback(0, 0);

		KillThreadProcessFrame();
		KillThreadGrabber();

		CloseHandle(m_hSemaphoreProcessFrame);
		m_hSemaphoreProcessFrame = 0;

		CloseHandle(m_hSemaphoreGrabber);
		m_hSemaphoreGrabber = 0;

		m_pIVisImSrcDevice->Release();

		assert(m_pIVisImSrcSettings != 0);
		m_pIVisImSrcSettings->Release();

		DeleteCriticalSection(&m_criticalsectionFrame);
		DeleteCriticalSection(&m_criticalsectionSequenceList);
	}
}


VisImSrcExport bool CVisSequenceList::FAddSequence(class CVisSequenceBase *psequence,
		bool fModifyImEncoding)
{
	bool fAdd = false;
	EVisPixFmt evispixfmt = psequence->PixFmt();

	EnterCriticalSection(&m_criticalsectionSequenceList);
	
	if (m_evispixfmt == evispixfmtNil)
	{
		CVisImageFromImSrcFrame imagefromframe(evispixfmt);

		int cimencoding = imagefromframe.CImEncodingSupported();
		if (cimencoding > 0)
		{
			LONG evisimencodingCur = evisimencodingUnknown;
			m_pIVisImSrcSettings->GetImSrcValue(evisimsrcImEncoding,
					&evisimencodingCur);

			if (((fModifyImEncoding)
						&& (evisimencodingCur
								!= imagefromframe.ImEncodingSupportedI(0)))
					|| (!imagefromframe.FIsSupportedImEncoding(
							(EVisImEncoding) evisimencodingCur)))
			{
				EVisImEncoding evisimencodingT;

				for (int i = 0; i < cimencoding; ++i)
				{
					evisimencodingT = imagefromframe.ImEncodingSupportedI(i);
					HRESULT hrT = m_pIVisImSrcDevice
							->IsPreferredImEncoding(evisimencodingT);

					if (((hrT == ResultFromScode(S_OK))
								|| (hrT == ResultFromScode(E_NOTIMPL)))
							&& (SUCCEEDED(m_pIVisImSrcSettings->SetImSrcValue(
									evisimsrcImEncoding, evisimencodingT))))
					{
						evisimencodingCur = evisimencodingT;
						break;
					}
				}

				if (i == cimencoding)
				{
					for (i = 0; i < cimencoding; ++i)
					{
						evisimencodingT = imagefromframe.ImEncodingSupportedI(i);

						if ((m_pIVisImSrcDevice->IsSupportedImEncoding(evisimencodingT)
										== ResultFromScode(S_OK))
								&& (SUCCEEDED(m_pIVisImSrcSettings->SetImSrcValue(
										evisimsrcImEncoding, evisimencodingT))))
						{
							evisimencodingCur = evisimencodingT;
							break;
						}
					}
				}
			}

			if (imagefromframe.FIsSupportedImEncoding(
					(EVisImEncoding) evisimencodingCur))
			{
				fAdd = true;
			}
		}
	}
	else if (m_evispixfmt == evispixfmt)
	{
		fAdd = true;
	}
	
	if (fAdd)
	{
		m_vectorSequenceList.push_back(psequence);
		m_evispixfmt = evispixfmt;
	}

	LeaveCriticalSection(&m_criticalsectionSequenceList);

	if (fAdd)
	{
		// Start grabbing if the sequence is not full.
		GrabIfNeeded(true);
	}

	return fAdd;
}

VisImSrcExport void CVisSequenceList::RemoveSequence(class CVisSequenceBase *psequence)
{
	EnterCriticalSection(&m_criticalsectionSequenceList);
	
    std::vector<class CVisSequenceBase *>::iterator iteratorSequenceList;
    for (iteratorSequenceList = m_vectorSequenceList.begin();
			iteratorSequenceList != m_vectorSequenceList.end();
			iteratorSequenceList++)
	{
		if (*iteratorSequenceList == psequence)
		{
			m_vectorSequenceList.erase(iteratorSequenceList);
			break;
		}
	}

	if (m_vectorSequenceList.empty())
		m_evispixfmt = evispixfmtNil;

	LeaveCriticalSection(&m_criticalsectionSequenceList);
}


VisImSrcExport void STDAPICALLTYPE CVisSequenceList::StaticFrameCallback(
		IVisImSrcDevice *pIVisImSrcDevice,
		SVisImSrcFrame *pimsrcframe, void *pvUser)
{
	((CVisSequenceList *) pvUser)->FrameCallback(pIVisImSrcDevice, pimsrcframe);
}


VisImSrcExport void CVisSequenceList::FrameCallback(
		IVisImSrcDevice *pIVisImSrcDevice,
		SVisImSrcFrame *pimsrcframe)
{
	assert(pIVisImSrcDevice != 0);
	assert(pimsrcframe != 0);
	assert(m_pIVisImSrcDevice == pIVisImSrcDevice);

	EnterCriticalSection(&m_criticalsectionFrame);

	// Discard any current frame information.
	if (m_fValidFrameCur)
	{
		m_fValidFrameCur = false;
		DiscardFrame(&m_imsrcframeCur);
	}

	// Make a copy of the frame information.
	CopyMemory(&m_imsrcframeCur, pimsrcframe, sizeof(SVisImSrcFrame));
	m_fValidFrameCur = true;

	LeaveCriticalSection(&m_criticalsectionFrame);

	// Indicate to the caller that we'll use the callback provided to
	// delete the frame data (if a callback was given).
	pimsrcframe->m_pfnDeleteData = 0;
	pimsrcframe->m_pvDeleteData = 0;

	// Tell the background thread that we have a new frame.
	// UNDONE:  For volatile buffers and scarce DMA buffers, we might
	// want to process the frame in this function (before the callback
	// returns) instead of waiting to process it in another thread.
	// (We should #ifdef that change, so that it can be corrected if
	// it causes problems.)
	ReleaseSemaphore(m_hSemaphoreProcessFrame, 1, 0);
}


VisImSrcExport void CVisSequenceList::GrabIfNeeded(bool fAddingSequence)
{
	if ((fAddingSequence)
			|| ((!m_fHaveContinuousGrab) && (m_fWantContinuousGrab)))
	{
		EnterCriticalSection(&m_criticalsectionSequenceList);
	
		// If any sequences at not full, we should try to grab another frame
		// to try to keep them full.
		std::vector<class CVisSequenceBase *>::iterator iteratorSequenceList;
		for (iteratorSequenceList = m_vectorSequenceList.begin();
				iteratorSequenceList != m_vectorSequenceList.end();
				iteratorSequenceList++)
		{
			if ((!((*iteratorSequenceList)->IsFull()))
					|| (((*iteratorSequenceList)->LengthMax()) == 0))
			{
				GrabNextInBackground();
				break;
			}
		}

		LeaveCriticalSection(&m_criticalsectionSequenceList);
	}
}


VisImSrcExport unsigned __stdcall CVisSequenceList::StaticThreadMainGrabber(
		void *pvSequenceList)
{
	assert(pvSequenceList != 0);
	return ((CVisSequenceList *) pvSequenceList)->ThreadMainGrabber();
}


VisImSrcExport unsigned CVisSequenceList::ThreadMainGrabber(void)
{
	while (!m_fKillThreadGrabber)
	{
		// To avoid a possible deadlock when we want a frame, we wait two
		// seconds for a frame and then try to grab it again, if needed.
		// UNDONE:  We should look into this problem more.  It seems to happen
		// when we are using a zero-length sequence and we turn off continuous
		// grabbing.
		DWORD cmsTimeout = ((m_fWantFrame) ? 2000 : INFINITE);
		VisWaitForSingleObject(m_hSemaphoreGrabber, cmsTimeout);

		if (m_fWantFrame)
		{
			m_pIVisImSrcDevice->GrabNext();
		}
	}

	return 0;
}


VisImSrcExport void CVisSequenceList::KillThreadGrabber(void)
{
	if (m_hThreadGrabber != 0)
	{
		m_fKillThreadGrabber = true;
		m_fWantFrame = false;
		ReleaseSemaphore(m_hSemaphoreGrabber, 1, 0);

		VisWaitForSingleObject(m_hThreadGrabber, INFINITE);

		m_hThreadGrabber = 0;
	}
}


VisImSrcExport unsigned __stdcall CVisSequenceList::StaticThreadMainProcessFrame(
		void *pvSequenceList)
{
	assert(pvSequenceList != 0);
	return ((CVisSequenceList *) pvSequenceList)->ThreadMainProcessFrame();
}


VisImSrcExport unsigned CVisSequenceList::ThreadMainProcessFrame(void)
{
	while (!m_fKillThreadProcessFrame)
	{
		VisWaitForSingleObject(m_hSemaphoreProcessFrame, INFINITE);

		EnterCriticalSection(&m_criticalsectionFrame);

		if (m_fValidFrameCur)
		{
			ProcessFrame(&m_imsrcframeCur);

			m_fValidFrameCur = false;
		}

		LeaveCriticalSection(&m_criticalsectionFrame);
	}

	return 0;
}


VisImSrcExport void CVisSequenceList::KillThreadProcessFrame(void)
{
	if (m_hThreadProcessFrame != 0)
	{
		EnterCriticalSection(&m_criticalsectionFrame);

		if (m_fValidFrameCur)
		{
			DiscardFrame(&m_imsrcframeCur);
			m_fValidFrameCur = false;
		}

		LeaveCriticalSection(&m_criticalsectionFrame);

		m_fKillThreadProcessFrame = true;
		ReleaseSemaphore(m_hSemaphoreProcessFrame, 1, 0);

		VisWaitForSingleObject(m_hThreadProcessFrame, INFINITE);

		m_hThreadProcessFrame = 0;
	}
}


VisImSrcExport void CVisSequenceList::DiscardFrame(
		SVisImSrcFrame *pimsrcframe)
{
	if (pimsrcframe != 0)
	{
		if (pimsrcframe->m_pfnDeleteData != 0)
		{
			pimsrcframe->m_pfnDeleteData(pimsrcframe->m_pvData,
					pimsrcframe->m_pvDeleteData);
			pimsrcframe->m_pfnDeleteData = 0;
			pimsrcframe->m_pvDeleteData = 0;
		}
	}
}


VisImSrcExport void CVisSequenceList::ProcessFrame(
		SVisImSrcFrame *pimsrcframe)
{
	// If we want a continuous grab, attempt to start grabbing another
	// frame before starting to process this frame.  (This may improve
	// performance with some devices, but may not have any effect on
	// other devices.)
	// LATER:  Make sure that this doesn't degrade performance!
	if (m_fWantContinuousGrab)
		GrabNextInBackground();

	m_fWantFrame = false;  // If we wanted one, this is it.
	bool fProcessImage = false;
	bool fCopyDMABuffers = !m_fAllowVolatileImages;  // UNDONE (in Rel1):  Support this option?
	bool fSubsampleYUV422 = false;
	
	EnterCriticalSection(&m_criticalsectionSequenceList);

	std::vector<class CVisSequenceBase *>::iterator iteratorSequenceList;
	for (iteratorSequenceList = m_vectorSequenceList.begin();
			iteratorSequenceList != m_vectorSequenceList.end();
			iteratorSequenceList++)
	{
		if ((!((*iteratorSequenceList)->IsFull()))
				|| (((*iteratorSequenceList)->LengthMax()) == 0))
		{
			fProcessImage = true;
			break;
		}
	}

	LeaveCriticalSection(&m_criticalsectionSequenceList);

	if (pimsrcframe->m_pvData != 0)
	{
		if (fProcessImage)
		{
			CVisImageFromImSrcFrame imagefromframe(m_evispixfmt);
			
			CVisImageBase *pimage = imagefromframe.PImageGetImSrcFrame(*pimsrcframe,
					!m_fAllowVolatileImages, fCopyDMABuffers, fSubsampleYUV422);
			
			// Discard any remaining frame information.
			DiscardFrame(pimsrcframe);

			// Note:  pimage may be 0 if we have an OOM error.  We can't throw
			// if we get an OOM error because this may be in a different thread.
			if (pimage != 0)
			{
				EnterCriticalSection(&m_criticalsectionSequenceList);
			
				for (iteratorSequenceList = m_vectorSequenceList.begin();
						iteratorSequenceList != m_vectorSequenceList.end();
						iteratorSequenceList++)
				{
				if ((!((*iteratorSequenceList)->IsFull()))
						|| (((*iteratorSequenceList)->LengthMax()) == 0))
					{
						(*iteratorSequenceList)->PushBack(*pimage);
					}
				}

				LeaveCriticalSection(&m_criticalsectionSequenceList);

				delete pimage;
				pimage = 0;
			}
#ifdef _DEBUG
			else
			{
				// Warn the user that we probably had a rare OOM error.
				// LATER:  Find a better way to handle OOM errors like this,
				// perhaps by calling an API in VisCore that will do the
				// right thing.
				bool fAsserted = false;
				assert(fAsserted);
				fAsserted = true;
			}
#endif // _DEBUG

			// LATER:  Should this be moved up so that we don't lose frames if the
			// copying above takes too long?
			GrabIfNeeded();
		}

		// Discard any remaining frame information.
		DiscardFrame(pimsrcframe);
	}
}
