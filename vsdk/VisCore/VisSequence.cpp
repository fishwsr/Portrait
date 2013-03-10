// VisSequence.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// #define VIS_WANT_QUEUE_DEBUG_STRINGS 1


#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "VisFileStreamIO.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// When reading from streams, should we always use the AVI stream
// handler?
bool CVisSequenceBase::s_fAlwaysUseAVIStreamHandler = true;

VisCoreExport bool CVisSequenceBase::AlwaysUseAVIStreamHandler(void)
{
	return s_fAlwaysUseAVIStreamHandler;
}

VisCoreExport void CVisSequenceBase::SetAlwaysUseAVIStreamHandler(bool f)
{
	s_fAlwaysUseAVIStreamHandler = f;
}


#ifdef LATER  // We'll need to change the sequence ctors to use this.
// LATER:  Default sequence options (EVisSequence values)
int CVisSequenceBase::s_evissequenceDefault = 0;

VisCoreExport int CVisSequenceBase::DefaultOptions(void)
{
	return s_evissequenceDefault;
}

VisCoreExport void CVisSequenceBase::SetDefaultOptions(int w)
{
	s_evissequenceDefault = w;
}
#endif // LATER


VisCoreExport void CVisSequenceBase::Clear()

{
	if (FEnterWriteBlock())
	{
		DeleteImages();
		
		LeaveWriteBlock();
	}
}

VisCoreExport void CVisSequenceBase::Erase(int i)
{
	if (FEnterWriteBlock())
	{
		assert(!m_deque.empty());
		assert(i >= 0);
		assert(i < m_deque.size());

		DeleteImage(i);
        m_deque.erase(m_deque.begin() + i);
        
        if( m_iCur >= m_deque.size() )
            SetLast();
		
		LeaveWriteBlock();
	}
}

VisCoreExport void CVisSequenceBase::Erase(int iFirst, int iLim)
{
	if (FEnterWriteBlock())
	{
		if (iLim == - 1)
			iLim = m_deque.size();
		
		assert(iFirst >= 0);
		assert(iFirst < m_deque.size());
		assert(iLim >= iFirst);
		assert(iLim <= m_deque.size());

		DeleteImages(iFirst, iLim);

        if( m_iCur >= m_deque.size() )
            SetLast();
		
		LeaveWriteBlock();
	}
}


VisCoreExport void CVisSequenceBase::SetICur(int i)
{
	int iOld = m_iCur;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	if (i < 0)
		m_iCur = 0;
	else if (i >= Length())
		SetLast();
	else
		m_iCur = i;

	if (m_iCur != iOld)
		OnICurChanged();
}

VisCoreExport void CVisSequenceBase::SetFirst(void)
{
	int iOld = m_iCur;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	m_iCur = 0;

	if (m_iCur != iOld)
		OnICurChanged();
}

VisCoreExport void CVisSequenceBase::SetLast(void)
{
	int iOld = m_iCur;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	m_iCur = Length() - 1;
	if (m_iCur < 0)
		m_iCur = 0;

	if (m_iCur != iOld)
		OnICurChanged();
}

VisCoreExport void CVisSequenceBase::SetNext(void)
{
	int iOld = m_iCur;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	if (++m_iCur >= Length())
		SetLast();

	if (m_iCur != iOld)
		OnICurChanged();
}

VisCoreExport void CVisSequenceBase::SetPrev(void)
{
	int iOld = m_iCur;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	if (--m_iCur < 0)
		m_iCur = 0;

	if (m_iCur != iOld)
		OnICurChanged();
}


VisCoreExport bool CVisSequenceBase::MoveForward(int i, bool fToFront)
{
	bool fRet;

	// UNDONE:  The code in CVisLayer used to skip empty frames
	// when changing m_iCur.  Should we do that here?
	
	if (fToFront)
		fRet = Swap(i, 0);
	else if (i > 0)
		fRet = Swap(i, i - 1);
	else
		fRet = false;

	if (fRet)
		OnOrderChanged();

	return fRet;
}

VisCoreExport bool CVisSequenceBase::MoveBackward(int i, bool fToBack)
{
	if (fToBack)
		return Swap(i, Length() - 1);
	
	return Swap(i, i + 1);
}


VisCoreExport bool CVisSequenceBase::Swap(int i, int j)
{
	bool fRet = false;

	if (FEnterWriteBlock())
	{
		if ((i >= 0) && (i < Length())
				&& (j >= 0) && (j < Length())
				&& (i != j))
		{
			SVisSequenceDequeEl sel = m_deque[i];
			m_deque[i] = m_deque[j];
			m_deque[j] = sel;

			fRet = true;
		}

		LeaveWriteBlock();

		if (fRet)
			OnSequenceChanged();
	}

	if (fRet)
		OnOrderChanged();

	return fRet;
}



VisCoreExport bool CVisSequenceBase::ConnectToSource(
		CVisImageSource& refimagesource, bool fModifyImEncoding,
		bool fUseContinuousGrab)
{
	return ((CVisImageSourceBase &) refimagesource).AddSequence(*this,
			fModifyImEncoding, fUseContinuousGrab);
}

VisCoreExport void CVisSequenceBase::DisconnectFromSource(void)
{
	if (m_pimagesource != 0)
	{
		((CVisImageSourceBase *) m_pimagesource)->DeleteFromSequence(*this);
		m_pimagesource = 0;
	}
}


VisCoreExport CVisSequenceBase::~CVisSequenceBase(void)
{
	DisconnectFromSource();

	// Abort operations in other threads.
	m_fValidSequence = 0;
	// LATER:  Add asserts that the ReleaseSemaphore calls succeed?
	ReleaseSemaphore(m_rgh[eihSemaphoreInBlock], ecSemaphoreLim / 2, 0);
	ReleaseSemaphore(m_rgh[eihSemaphoreImagesAvailable], ecSemaphoreLim / 2, 0);
	VisWaitForSingleObject(m_hEventNotInBlock, INFINITE);

	if (m_pimageCur != 0)
	{
		delete m_pimageCur;
		m_pimageCur = 0;
	}

	DeleteImages();

	if (m_rgh[eihSemaphoreImagesAvailable] != 0)
		CloseHandle(m_rgh[eihSemaphoreImagesAvailable]);
	if (m_rgh[eihSemaphoreInBlock] != 0)
		CloseHandle(m_rgh[eihSemaphoreInBlock]);
	CloseHandle(m_hEventNotInBlock);

}

VisCoreExport void CVisSequenceBase::AddCallback(T_PfnCallback pfnCallback,
		void *pvUser, bool fCallOnChange, bool fCallWhenDying)
{
	SVisSequenceCallbackInfo callbackinfo;
	callbackinfo.m_pfnCallback = pfnCallback;
	callbackinfo.m_pvUser = pvUser;
	callbackinfo.m_fCallOnChange = fCallOnChange;
	callbackinfo.m_fCallWhenDying = fCallWhenDying;

	m_rgcallback.push_back(callbackinfo);
}


VisCoreExport void CVisSequenceBase::RemoveCallback(T_PfnCallback pfnCallback)
{
	T_Rgcallback::iterator iteratorCur = m_rgcallback.begin();
	T_Rgcallback::iterator iteratorLim = m_rgcallback.end();
	for (; iteratorCur != iteratorLim; ++iteratorCur)
	{
		if (iteratorCur->m_pfnCallback == pfnCallback)
		{
			m_rgcallback.erase(iteratorCur);
			break;
		}
	}
}


VisCoreExport const type_info& CVisSequenceBase::ObjectTypeInfo(int iLevel) const
{
	return typeid(CVisSequenceBase);
}


VisCoreExport const char *CVisSequenceBase::WorkingDirectory(void) const
{
	if (m_strWorkingDirectory.empty())
		return 0;

	return m_strWorkingDirectory.c_str();
}

VisCoreExport void CVisSequenceBase::SetWorkingDirectory(const char *szPath)
{
	if (szPath == 0)
		m_strWorkingDirectory = "";
	else
		m_strWorkingDirectory = szPath;
}


VisCoreExport const CVisImageBase& CVisSequenceBase::Front(void) const
{
	assert(!m_deque.empty());
	assert(m_deque.front().m_pImage != 0);

	ReadFrontIfNeeded();
	return *m_deque.front().m_pImage;
}

VisCoreExport CVisImageBase& CVisSequenceBase::Front(void)
{
	assert(!m_deque.empty());
	assert(m_deque.front().m_pImage != 0);

	ReadFrontIfNeeded();
	return *m_deque.front().m_pImage;
}

VisCoreExport const CVisImageBase& CVisSequenceBase::Back(void) const
{
	assert(!m_deque.empty());
	assert(m_deque.back().m_pImage != 0);

	ReadBackIfNeeded();
	return *m_deque.back().m_pImage;
}

VisCoreExport CVisImageBase& CVisSequenceBase::Back(void)
{
	assert(!m_deque.empty());
	assert(m_deque.back().m_pImage != 0);

	ReadBackIfNeeded();
	return *m_deque.back().m_pImage;
}

VisCoreExport const CVisImageBase& CVisSequenceBase::At(int i) const
{
	assert(!m_deque.empty());
	assert(i >= 0);
	assert(i < m_deque.size());
	assert(m_deque[i].m_pImage != 0);

	ReadIfNeeded(i);
	return *m_deque.at(i).m_pImage;
}

VisCoreExport CVisImageBase& CVisSequenceBase::At(int i)
{
	assert(!m_deque.empty());
	assert(i >= 0);
	assert(i < m_deque.size());
	assert(m_deque[i].m_pImage != 0);

	ReadIfNeeded(i);
	return *m_deque.at(i).m_pImage;
}

VisCoreExport const CVisImageBase& CVisSequenceBase::operator[](int i) const
{
	assert(!m_deque.empty());
	assert(i >= 0);
	assert(i < m_deque.size());
	assert(m_deque[i].m_pImage != 0);

	ReadIfNeeded(i);
	return *m_deque[i].m_pImage;
}

VisCoreExport CVisImageBase& CVisSequenceBase::operator[](int i)
{
	assert(!m_deque.empty());
	assert(i >= 0);
	assert(i < m_deque.size());
	assert(m_deque[i].m_pImage != 0);

	ReadIfNeeded(i);
	return *m_deque[i].m_pImage;
}

VisCoreExport void CVisSequenceBase::LeaveWriteBlock(void)
{
	ReleaseSemaphore(m_rgh[eihSemaphoreInBlock], 1, 0);
	if (--m_cThreadInBlock == 0)
		SetEvent(m_hEventNotInBlock);
}


VisCoreExport void CVisSequenceBase::ImageAdded(int cImage)
{
	if (cImage > 0)
		ReleaseSemaphore(m_rgh[eihSemaphoreImagesAvailable], cImage, 0);
}

VisCoreExport void CVisSequenceBase::ImageRemoved(int cImage)
{
	for (int i = 0; i < cImage; ++i)
	{
		DWORD dw = VisWaitForSingleObject(m_rgh[eihSemaphoreImagesAvailable],
				0);
#ifdef _DEBUG
		// If (dw == WAIT_TIMEOUT), then the semaphore count is zero.
		// We expect that the semaphore count is > 0 when this function
		// is called.  (If not, something's wrong in the way we count
		// available images.)
		assert(dw == WAIT_OBJECT_0);
#endif // _DEBUG
	}
}

VisCoreExport void CVisSequenceBase::ReadIfNeeded(int iimage) const
{
    assert(iimage >= 0);
    assert(m_deque.size() > iimage);

    bool fRead = const_cast<CVisSequenceBase *>(this)->UpdateMRU(
                     const_cast<SVisSequenceDequeEl&>(m_deque.at(iimage)) );
    
    if ( ((m_evissequence & evissequenceMaskDelayRead) ==
             evissequenceDelayReadUntilNeeded) || fRead )
    {
        CVisImageBase* pImage = m_deque.at(iimage).m_pImage;
        const char *szName = pImage->Name(true);
        CVisStreamHandler* pStream = pImage->StreamHandler();

        if ( (((szName != 0) && (*szName != 0)) || pStream) && 
              (!pImage->IsValid()) )
        {
// LATER:  Do we want to avoid exceptions here?
//            pImage->FReadFile();
            pImage->ReadFile();
        }
    }
}

VisCoreExport bool CVisSequenceBase::FEnterWriteBlock(DWORD cmsTimeout)
{
	bool fRet = false;
	DWORD dwWait = WAIT_OBJECT_0;
	
	if ((m_fValidSequence)
			&& (m_rgh[eihSemaphoreInBlock] != 0)
			&& (m_rgh[eihSemaphoreImagesAvailable] != 0)
			&& (m_hEventNotInBlock != 0))
	{
		++ m_cThreadInBlock;
		ResetEvent(m_hEventNotInBlock);
		dwWait = VisWaitForSingleObject(m_rgh[eihSemaphoreInBlock], cmsTimeout);

		if (dwWait == WAIT_OBJECT_0)
		{
			if (m_fValidSequence)
			{
				fRet = true;
			}
			else
			{
				LeaveWriteBlock();
			}
		}
		else
		{
			if (--m_cThreadInBlock == 0)
				SetEvent(m_hEventNotInBlock);
		}
	}

#ifdef VIS_WANT_QUEUE_DEBUG_STRINGS
	if (dwWait == WAIT_OBJECT_0)
	{
		if (!m_fValidSequence)
			OutputDebugString("CVisSequenceBase not valid - block not entered.\n");
		else if (!fRet)
			OutputDebugString("Failed to enter block in CVisSequenceBase.\n");
	}
	else if (dwWait == WAIT_FAILED)
	{
		OutputDebugString("Wait function failed in CVisSequenceBase.\n");
	}
#endif // VIS_WANT_QUEUE_DEBUG_STRINGS

	return fRet;
}


VisCoreExport bool CVisSequenceBase::FEnterWriteBlockWithImage(DWORD cmsTimeout)
{
	bool fRet = false;
	DWORD dwWait = WAIT_OBJECT_0;

	if ((m_fValidSequence)
			&& (m_rgh[eihSemaphoreInBlock] != 0)
			&& (m_rgh[eihSemaphoreImagesAvailable] != 0)
			&& (m_hEventNotInBlock != 0))
	{
		++ m_cThreadInBlock;
		ResetEvent(m_hEventNotInBlock);

		if (IsEmpty())
			dwWait = WAIT_TIMEOUT;
		else
			dwWait = VisWaitForMultipleObjects(eihLim, m_rgh, TRUE, 0);

		if (dwWait == WAIT_TIMEOUT)
		{
			NeedImage();

			++ m_cWaitingForImage;
			dwWait = VisWaitForMultipleObjects(eihLim, m_rgh, TRUE, cmsTimeout);
			-- m_cWaitingForImage;
		}

		if ((dwWait >= WAIT_OBJECT_0) && (dwWait < WAIT_OBJECT_0 + eihLim))
		{
			if (m_fValidSequence)
			{
				fRet = true;
			}
			else
			{
				// We didn't pop the image.
				ImageAdded();

				LeaveWriteBlock();
			}
		}
		else
		{
			if (--m_cThreadInBlock == 0)
				SetEvent(m_hEventNotInBlock);
		}
	}

#ifdef VIS_WANT_QUEUE_DEBUG_STRINGS
	if ((dwWait >= WAIT_OBJECT_0) && (dwWait < WAIT_OBJECT_0 + eihLim))
	{
		if (!m_fValidSequence)
			OutputDebugString("CVisSequenceBase not valid - image block not entered.\n");
		else if (!fRet)
			OutputDebugString("Failed to enter image block in CVisSequenceBase.\n");
	}
	else if (dwWait == WAIT_FAILED)
	{
		OutputDebugString("Image wait function failed in CVisSequenceBase.\n");
	}
#endif // VIS_WANT_QUEUE_DEBUG_STRINGS

	return fRet;
}


VisCoreExport CVisSequenceBase::CVisSequenceBase(EVisPixFmt evispixfmt,
		int evissequence)
 :	m_evissequence(evissequence),
	m_nLengthMax(enTrueLengthMax),
	m_iCur(0),  // UNDONE (in Rel1):  Need to make sure that m_iCur is updated when images are added, removed, or swapped.
	m_evispixfmt(evispixfmt),
	m_deque(),
    m_pMemHead(0),
    m_pMemTail(0),
    m_cMemUsageSize(0),
    m_cMemUsageMax(32),
	m_pimagesource(0),
	m_pimageCur(0),
	m_strWorkingDirectory(),
	m_cThreadInBlock(0),
	m_cWaitingForImage(0),
	m_proplist(),
	m_rgcallback(),
	m_IsDying(false),
	m_HasDied(false),
	m_fValidSequence(true),
	m_fCalledSetLengthMax(false)
{
	if (((unsigned int) m_deque.max_size())
			< ((unsigned int) m_nLengthMax))
	{
		m_nLengthMax = m_deque.max_size();
	}

	InitSynchObjects();
}

VisCoreExport CVisSequenceBase& CVisSequenceBase::operator=(
		const CVisSequenceBase& refsequence)
{
	assert(m_evispixfmt == refsequence.m_evispixfmt);

	Clear();

	// Assignment should change the sequence options so that the
	// evissequenceDelayReadUntilNeeded option will be set in the
	// destination sequence if needed.
	m_evissequence = refsequence.m_evissequence;
	m_fCalledSetLengthMax = refsequence.m_fCalledSetLengthMax;
	m_nLengthMax = refsequence.m_nLengthMax;

	if (m_pimagesource != 0)
	{
		((CVisImageSourceBase *) m_pimagesource)->DeleteFromSequence(*this);
		m_pimagesource = 0;
	}

	Assign(refsequence);

	if (refsequence.m_pimagesource != 0)
	{
		((CVisImageSourceBase *) refsequence.m_pimagesource)->AddSequence(*this);
	}
	
	OnSequenceChanged();

	return *this;
}


VisCoreExport void CVisSequenceBase::SetLengthMax(int n)
{
	if (FEnterWriteBlock())
	{
		m_fCalledSetLengthMax = true;
		if (n >= 0)
		{
			m_nLengthMax = n;
		}
		else if (((unsigned int) m_deque.max_size())
				> ((unsigned int) enTrueLengthMax))
		{
			m_nLengthMax = enTrueLengthMax;
		}
		else
		{
			m_nLengthMax = m_deque.max_size();
		}

		// Delete images at the end of the sequence, if needed.
		bool fDeletedImages = false;
		if (m_deque.size() > m_nLengthMax)
		{
			DeleteImages(m_nLengthMax, m_deque.size());
	
			fDeletedImages = true;
		}

		LeaveWriteBlock();

		if (fDeletedImages)
			OnSequenceChanged();
	}
}
/*
VisCoreExport void CVisSequenceBase::ReadSequenceInfo(const char *szFilename,
		bool fReadFiles, bool fChangeOptionsAndLengthMax)
{
#ifdef VIS_INCLUDE_SDSTREAM
	int iCur = ICur();
	bool fCalledSetLengthMax = m_fCalledSetLengthMax;
	int nLengthMax = m_nLengthMax;
	int evissequence = m_evissequence;

	// Note:  Ideally, this would be a const method, but we use self-describing
	// streams for the I/O, and they call non-const methods.
    CVisSDStream stream;
    stream.Open(szFilename, CVisSDStream::Read);

    stream << *this;
    stream.Close();

	if (!fChangeOptionsAndLengthMax)
	{
		m_evissequence = evissequence;

		SetICur(iCur);

		if (fCalledSetLengthMax)
		{
			SetLengthMax(nLengthMax);
		}
		else
		{
			m_fCalledSetLengthMax = fCalledSetLengthMax;
			m_nLengthMax = nLengthMax;
		}
	}

	if (fReadFiles)
		ReadFiles();
#else
	assert(false);
#endif
}


VisCoreExport void CVisSequenceBase::WriteSequenceInfo(const char *szFilename,
		bool fWriteFiles)
{
#ifdef VIS_INCLUDE_SDSTREAM

	// Note:  Ideally, this would be a const method, but we use self-describing
	// streams for the I/O, and they call non-const methods.
    CVisSDStream stream(0);
    stream.Open(szFilename, CVisSDStream::Write);

	// LATER:  Do we want to add a comment string to the stream?
//    string comment = "\ngenerated by: ";
//    for (int i = 0; i < argc; i++)
//        comment += " " + string(argv[i]);

// UNDONE (in Rel1):  Can we make the writing of descriptions optional?  (If
// not, should CVisFrameSequemce make sure that CVisSequence is also described?)
    stream << *this;
    stream.Close();

	if (fWriteFiles)
		WriteFiles();
#else
	assert(false);
#endif
}


VisCoreExport bool CVisSequenceBase::FReadSequenceInfo(const char *szFilename,
		bool fReadFiles, bool fChangeOptionsAndLengthMax)
{
#ifdef VIS_INCLUDE_SDSTREAM
	bool fRet = false;
	CVisSequenceBase *psequenceClone = 0;

	try
	{
		psequenceClone = Clone();
		
		// New should throw an exception, but we check the return in case that
		// option is not used.
		if (psequenceClone != 0)
		{
			psequenceClone->ReadSequenceInfo(szFilename, fReadFiles,
					fChangeOptionsAndLengthMax);

			fRet = true;

			*this = *psequenceClone;
		}
	}
	catch (...)
	{
	}

	if (psequenceClone != 0)
	{
		delete psequenceClone;
		psequenceClone = 0;
	}

	return fRet;
#else
	assert(false);
return false;
#endif
}


VisCoreExport bool CVisSequenceBase::FWriteSequenceInfo(const char *szFilename,
		bool fWriteFiles)
{
#ifdef VIS_INCLUDE_SDSTREAM
	bool fRet = false;

	try
	{
		WriteSequenceInfo(szFilename, fWriteFiles);

		fRet = true;
	}
	catch (...)
	{
	}

	return fRet;
#else
	assert(false);
	return false;
#endif
}
*/
VisCoreExport void CVisSequenceBase::ReadFiles(bool fOnlyIfNoImage)
{
	if (FEnterReadBlock())
	{
		try
		{
			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
				assert((*iteratorT).m_pImage != 0);

				if (((*iteratorT).m_pImage->Name(true) != 0)
						&& ((!fOnlyIfNoImage)
							|| ((*iteratorT).m_pImage->Width() == 0)
							|| ((*iteratorT).m_pImage->Height() == 0)))
				{
                    UpdateMRU( *iteratorT );
					(*iteratorT).m_pImage->ReadFile();
				}
			}
		}
		catch (CVisError& referror)
		{
			LeaveReadBlock();
			referror.ThrowLast();
		}
		catch (...)
		{
			LeaveReadBlock();
			throw;
		}

		LeaveReadBlock();
	}
}


VisCoreExport void CVisSequenceBase::WriteFiles(void)
{
	// We'd like this to be a const method, but the WriteFile method of
	// CVisImageBase is not const (because it calls methods that might
	// change the file name).
	// Since this is not const, we can add synchronization to this method.
	if (FEnterReadBlock())
	{
		try
		{
			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
				assert((*iteratorT).m_pImage != 0);

				if (((*iteratorT).m_pImage->Name(true) != 0)
						&& ((*iteratorT).m_pImage->IsValid()))
				{
                    UpdateMRU( *iteratorT );
					(*iteratorT).m_pImage->WriteFile();
				}
			}
		}
		catch (CVisError& referror)
		{
			LeaveReadBlock();
			referror.ThrowLast();
		}
		catch (...)
		{
			LeaveReadBlock();
			throw;
		}

		LeaveReadBlock();
	}
}






VisCoreExport bool CVisSequenceBase::FReadFiles(bool fOnlyIfNoImage,
		bool fRemoveBadImages)
{
	bool fRet = false;

	if (FEnterBlock(fRemoveBadImages))
	{
		fRet = true;

		T_Deque::iterator iteratorT = m_deque.begin();
		while (iteratorT != m_deque.end())

		{
			assert((*iteratorT).m_pImage != 0);

			if (((*iteratorT).m_pImage->Name(true) != 0)
					&& ((!fOnlyIfNoImage)
						|| ((*iteratorT).m_pImage->Width() == 0)
						|| ((*iteratorT).m_pImage->Height() == 0)))
			{
                UpdateMRU( *iteratorT );
				if (!(*iteratorT).m_pImage->FReadFile())
					fRet = false;
			}
			
			// Advance to the next image by either removing the current
			// image or incrementing the iterator.
			if ((fRemoveBadImages)
					&& (!(*iteratorT).m_pImage->IsValid()))
			{
				DeleteImage( *iteratorT );
				m_deque.erase(iteratorT);
			}
			else
			{
				++iteratorT;
			}
		}

		LeaveBlock(fRemoveBadImages);
	}

	return fRet;
}


VisCoreExport bool CVisSequenceBase::FWriteFiles(void)
{
	bool fRet = false;

	if (FEnterWriteBlock())
	{
		fRet = true;

		T_Deque::iterator iteratorT = m_deque.begin();
		T_Deque::iterator iteratorTLim = m_deque.end();
		for ( ; iteratorT != iteratorTLim; ++iteratorT)
		{
			assert((*iteratorT).m_pImage != 0);

			if ((*iteratorT).m_pImage->Name(true) != 0)
			{
                UpdateMRU( *iteratorT );
				if (!(*iteratorT).m_pImage->FWriteFile())
					fRet = false;
			}
		}

		LeaveWriteBlock();
	}

	return fRet;
}

VisCoreExport void CVisSequenceBase::ReadStream( const char* filename )
{
    // clear the current sequence state
    Clear();

    InsertStream( filename, 0 );
}

VisCoreExport void CVisSequenceBase::AppendStream( const char* filename )
{
    InsertStream( filename, m_deque.size() );
}

VisCoreExport void CVisSequenceBase::InsertStream( const char* filename, int pos )
{
    // UNDONE: if filename==NULL generate a filename
    assert( filename );

    // open a stream handler
    CVisStreamHandler* pStrmHandler = NULL;
    UINT framecnt = 0;

    try
    {
        pStrmHandler = OpenStreamHandler( filename, OF_READ );

        // create individual images for each frames
        framecnt = pStrmHandler->FrameCount();

        Insert( pos, framecnt );
    }
    catch (CVisError& referror)
    {
        if ( pStrmHandler )
            pStrmHandler->Release();
        referror.ThrowLast();
    }
    catch (...)
    {
        // We could get an exception if memory can't be allocated.
        if ( pStrmHandler )
            pStrmHandler->Release();
        throw;
    }

    if (FEnterWriteBlock())
    {
        try
        {
            // iterate through all newly inserted frames
            T_Deque::iterator iteratorT    = m_deque.begin() + pos;
            T_Deque::iterator iteratorTLim = iteratorT       + framecnt;
            for (int i = 0; iteratorT != iteratorTLim; ++iteratorT, ++i)
            {
                assert( (*iteratorT).m_pImage != 0 );

                (*iteratorT).m_pImage->SetStreamHandler(pStrmHandler, i);

                // optionally load the image
                if ( (m_evissequence & evissequenceMaskDelayRead)
                     == evissequenceDontDelayRead )
                {
                    pStrmHandler->ReadFrame( *(*iteratorT).m_pImage, i );
                }
            }
        }
        catch (CVisError& referror)
        {
            if ( pStrmHandler )
                pStrmHandler->Release();
            LeaveWriteBlock();
            referror.ThrowLast();
        }
        catch (...)
        {
            // We could get an exception if memory can't be allocated.
            if ( pStrmHandler )
                pStrmHandler->Release();
            LeaveWriteBlock();
            throw;
        }
        LeaveWriteBlock();
    }

    if ( pStrmHandler )
        pStrmHandler->Release();
}

VisCoreExport void CVisSequenceBase::WriteStream( const char* filename,
                          UINT uBitCount, bool fCompressorDlg, 
                          bool fAddSilentAudio )
{
    // UNDONE: if filename==NULL generate a filename
    assert( filename );

    CVisStreamHandler* pStrmHandler = NULL;

    if (FEnterWriteBlock())
    {    
        try
        {
            // open a stream handler
            pStrmHandler = OpenStreamHandler(
                filename, OF_READWRITE | OF_CREATE, uBitCount, fCompressorDlg );

            // iterate through all newly inserted frames
            T_Deque::iterator iteratorT    = m_deque.begin();
            T_Deque::iterator iteratorTLim = m_deque.end();
            for (int i = 0; iteratorT != iteratorTLim; ++iteratorT, ++i)
            {
                assert( (*iteratorT).m_pImage != 0 );

                // load the image
                ReadIfNeeded(i);

                // write it to the stream
                pStrmHandler->WriteFrame( *(*iteratorT).m_pImage, i );

                 // set the stream handler 
                (*iteratorT).m_pImage->SetStreamHandler(pStrmHandler, i);
            }

            if( fAddSilentAudio )
            {
                INT64 auddur = pStrmHandler->TimeFromFrameCount( i );
                
                CVisAudioChannel mono(8000);
                UINT uNumSamp;
                float* pAudio = mono.StartWrite(0, auddur, &uNumSamp);
                memset( pAudio, 0, sizeof(float)*uNumSamp );
                mono.EndWrite(0, auddur);

                CVisAudioData audio;
                audio.InitializeChannel( 0, &mono, 0, auddur );
                pStrmHandler->WriteAudio( audio, 0 );
            }
        }
        catch (CVisError& referror)
        {
            if( pStrmHandler )
                pStrmHandler->Release();
            LeaveWriteBlock();
            referror.ThrowLast();
        }
        catch (...)
        {
            // We could get an exception if memory can't be allocated.
            if( pStrmHandler )
                pStrmHandler->Release();
            LeaveWriteBlock();
            throw;
        }
        if( pStrmHandler )
            pStrmHandler->Release();
        LeaveWriteBlock();
    }
}

typedef struct _strmvecel
{
    CVisStreamHandler* pStrmHandler;
    BOOL               bExists;
} strmvecel;

VisCoreExport void CVisSequenceBase::OpenStreams(UINT mode)
{
    char StreamName[MAX_PATH];
    typedef std::vector<strmvecel> T_STRMVEC;
    T_STRMVEC streams;

    UINT xmode = mode & OF_WRITE | OF_READWRITE;

    // iterate through all images in the sequence
    T_Deque::iterator iteratorT = m_deque.begin();
    T_Deque::iterator iteratorTLim = m_deque.end();
    for ( ; iteratorT != iteratorTLim; ++iteratorT)
    {
        CVisImageBase* pImg = (*iteratorT).m_pImage;

        assert( pImg != 0);
        const char* pName = pImg->Name(true);

        // check if this filename is a streamname
        if ( pName != 0 && pName[strlen(pName)-1] == ']' )
        {
            // get the filename without the brackets
            strcpy(StreamName, pName);
            char* pOB = strrchr(StreamName,'[');
            if( !pOB )
            {
                continue;
            }
            int frmnum = atoi(pOB+1);
            *pOB = '\0';

            // iterate through currently open streams
            T_STRMVEC::iterator iteratorV    = streams.begin();
            T_STRMVEC::iterator iteratorVLim = streams.end();
            for( ; iteratorV != iteratorVLim; ++iteratorV )
            {
                // if stream is already open break;
                if( strcmp(iteratorV->pStrmHandler->FileName(), StreamName) == 0 )
                {
                    break;
                }
            }

            // open a new streamhandler if necessary
            strmvecel backel, *pvecel;
            if( iteratorV == iteratorVLim )
            {
                backel.bExists      = _access(StreamName, 0) != -1;
                assert( !(!backel.bExists && xmode == OF_READ) ); 
                backel.pStrmHandler = OpenStreamHandler( StreamName, mode );

                streams.push_back(backel);
                pvecel = &backel;
            }
            else
            {
                pvecel = iteratorV;
            }

            // set this image's stream handler
            (*iteratorT).m_pImage->SetStreamHandler(pvecel->pStrmHandler, frmnum );

            // optionally read the file
            if ( ((m_evissequence & evissequenceMaskDelayRead) ==
                     evissequenceDontDelayRead) && 
                 xmode != OF_WRITE && pvecel->bExists )
            {
                UpdateMRU( *iteratorT );
                (*iteratorT).m_pImage->ReadFile();
            }
        }
    }

    // iterate through open streams and release local reference
    T_STRMVEC::iterator iteratorV    = streams.begin();
    T_STRMVEC::iterator iteratorVLim = streams.end();
    for( ; iteratorV != iteratorVLim; ++iteratorV )
    {
        // reference count should be > 1 or else something
        // weird has happened
        assert(iteratorV->pStrmHandler->Release());
    }
}

VisCoreExport bool CVisSequenceBase::RemoveEmptyImages(void)
{
    bool fRet = false;

	if (FEnterWriteBlock())
	{
		T_Deque::iterator iteratorT = m_deque.begin();
		while (iteratorT != m_deque.end())
		{
			assert((*iteratorT).m_pImage != 0);
			
			// Advance to the next image by either removing the current
			// image or incrementing the iterator.
			if (!(*iteratorT).m_pImage->IsValid())
			{
				fRet = true;

				DeleteImage( *iteratorT );
				m_deque.erase(iteratorT);
			}
			else
			{
				++iteratorT;
			}
		}

		LeaveWriteBlock();
	}

	return fRet;
}


VisCoreExport void CVisSequenceBase::AddFile(const char *szFilename,
		bool fReadFile)
{
	CVisImageBase *pimage = 0;
	
	try
	{
		pimage = PImageNew();

		pimage->SetName(szFilename);

		if (fReadFile)
			pimage->ReadFile();

		PushBack(*pimage);

		delete pimage;
	}
	catch (CVisError& referror)
	{
		if (pimage != 0)
			delete pimage;
		referror.ThrowLast();
	}
	catch(...)
	{
		if (pimage != 0)
			delete pimage;
		throw;
	}
}


VisCoreExport bool CVisSequenceBase::FAddFile(const char *szFilename,
		bool fReadFile)
{
	bool fRet = false;

	CVisImageBase *pimage = 0;
	
	try
	{
		pimage = PImageNew();

		pimage->SetName(szFilename);

		if ((!fReadFile) || (pimage->FReadFile()))
		{
			PushBack(*pimage);

			fRet = true;
		}

		delete pimage;
	}
	catch(...)
	{
		if (pimage != 0)
			delete pimage;
	}

	return false;
}


VisCoreExport void CVisSequenceBase::RenameFiles(const char *szBase,
		const char *szExtension, int cchDigits)
{
	if (FEnterReadBlock())
	{
		try
		{
			char szFormat[128];
			sprintf(szFormat, "%%s%%0%dd.%%s", cchDigits);

			std::string strBase;

			if ((szBase == 0) || (strchr(szBase, '\\') == 0))
			{
				if ((szBase[0] != 0) && (szBase[1] == ':'))
				{
					strBase = "";
					m_strWorkingDirectory = ":";
					m_strWorkingDirectory.insert(m_strWorkingDirectory.begin(),
							szBase[0]);
				}
				else if (WorkingDirectory() != 0)
				{
					strBase = WorkingDirectory();
					strBase += "\\";
				}
				else
				{
					strBase = "";
				}

				if (szBase != 0)
					strBase += szBase;
			}
			else
			{
				strBase = szBase;

				// Remember the working directory.
				m_strWorkingDirectory = szBase;

				int ichPathSep = m_strWorkingDirectory.find_last_of('\\');
				m_strWorkingDirectory.erase(
						m_strWorkingDirectory.begin() + ichPathSep,
						m_strWorkingDirectory.end());

				// Special case - root directories.
				// UNDONE (in Rel1):  This also could have problems with network paths.
				if (m_strWorkingDirectory.length() == 0)
				{
					m_strWorkingDirectory = "\\";
				}
				else if ((m_strWorkingDirectory[0] != '\\')
						&& (m_strWorkingDirectory.length() <= 2))
				{
					m_strWorkingDirectory += "\\";
				}
			}

			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for (int i = 0 ; iteratorT != iteratorTLim; ++iteratorT, ++i)
			{
				assert((*iteratorT).m_pImage != 0);

				char szFilename[1024];
				sprintf(szFilename, szFormat, strBase.c_str(), i, szExtension);

				(*iteratorT).m_pImage->SetName(szFilename);
			}
		}
		catch (CVisError& referror)
		{
			LeaveReadBlock();
			referror.ThrowLast();
		}
		catch (...)
		{
			// We could get an exception if memory can't be allocated.
			LeaveReadBlock();
			throw;
		}

		LeaveReadBlock();
	}
}

VisCoreExport bool CVisSequenceBase::GetFront(CVisImageBase& refimage,
		DWORD cmsTimeout)
{
	assert(&refimage != 0);

	bool fRet = false;
	
	if ((m_nLengthMax != 0)
			&& (FEnterReadBlockWithImage(cmsTimeout)))
	{
		assert(!m_deque.empty());

		ReadFrontIfNeeded();
		refimage = *(m_deque.front().m_pImage);
		fRet = true;

		// We didn't pop the image.
		ImageAdded();

		LeaveReadBlock();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::GetBack(CVisImageBase& refimage,
		DWORD cmsTimeout)
{
	assert(&refimage != 0);

	bool fRet = false;
	
	if ((m_nLengthMax != 0)
			&& (FEnterReadBlockWithImage(cmsTimeout)))
	{
		assert(!m_deque.empty());

		ReadBackIfNeeded();
		refimage = *(m_deque.back().m_pImage);
		fRet = true;

		// We didn't pop the image.
		ImageAdded();

		LeaveReadBlock();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::GetAt(int i, CVisImageBase& refimage)
{
	assert(&refimage != 0);

	bool fRet = false;
	
	if ((m_nLengthMax != 0) && (FEnterReadBlock()))
	{
		if ((!m_deque.empty()) && (i >= 0) && (i < m_deque.size()))
		{
			assert(m_deque[i].m_pImage != 0);

			ReadIfNeeded(i);
			refimage = *m_deque.at(i).m_pImage;

			fRet = true;
		}

		LeaveReadBlock();
	}

	return fRet;
}


VisCoreExport void CVisSequenceBase::PushFront(const char *szName)
{
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		FPushFrontPImage(pimagebase);
	}
}

VisCoreExport void CVisSequenceBase::PushBack(const char *szName)
{
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		FPushBackPImage(pimagebase);
	}
}


VisCoreExport bool CVisSequenceBase::PopFront(void)
{
	bool fRet = false;
	
	if (FEnterWriteBlock())
	{
		if (m_nLengthMax == 0)
		{
			if (m_pimageCur != 0)
			{
				delete m_pimageCur;
				m_pimageCur = 0;
				ImageRemoved();
				fRet = true;
			}
		}
		else
		{
			if (!m_deque.empty())
			{
				DeleteFront();
				NeedImage();
				fRet = true;
			}
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::PopFront(DWORD cmsTimeout)
{
	bool fRet = false;
	
	if (FEnterWriteBlockWithImage(cmsTimeout))
	{
		if (m_nLengthMax == 0)
		{
			assert(m_pimageCur != 0);

			delete m_pimageCur;
			m_pimageCur = 0;
			fRet = true;
		}
		else
		{
			assert(!m_deque.empty());

			DeleteFront(false);
			NeedImage();
			fRet = true;
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::PopFront(CVisImageBase& refimage,
		DWORD cmsTimeout)
{
	assert(&refimage != 0);

	bool fRet = false;
	
	if (FEnterWriteBlockWithImage(cmsTimeout))
	{
		if (m_nLengthMax == 0)
		{
			assert(m_pimageCur != 0);

			refimage = *m_pimageCur;
			delete m_pimageCur;
			m_pimageCur = 0;
			fRet = true;
		}
		else
		{
			assert(!m_deque.empty());

			ReadFrontIfNeeded();
			refimage = *(m_deque.front().m_pImage);
			DeleteFront(false);
			NeedImage();
			fRet = true;
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::PopBack(void)
{
	bool fRet = false;
	
	if (FEnterWriteBlock())
	{
		if (m_nLengthMax == 0)
		{
			if (m_pimageCur != 0)
			{
				delete m_pimageCur;
				m_pimageCur = 0;
				ImageRemoved();
				fRet = true;
			}
		}
		else
		{
			if (!m_deque.empty())
			{
				DeleteBack();
				NeedImage();
				fRet = true;
			}
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::PopBack(DWORD cmsTimeout)
{
	bool fRet = false;
	
	if (FEnterWriteBlockWithImage(cmsTimeout))
	{
		if (m_nLengthMax == 0)
		{
			assert(m_pimageCur != 0);

			delete m_pimageCur;
			m_pimageCur = 0;
			fRet = true;
		}
		else
		{
			assert(!m_deque.empty());

			DeleteBack(false);
			NeedImage();
			fRet = true;
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::PopBack(CVisImageBase& refimage,
		DWORD cmsTimeout)
{
	assert(&refimage != 0);

	bool fRet = false;
	
	if (FEnterWriteBlockWithImage(cmsTimeout))
	{
		if (m_nLengthMax == 0)
		{
			assert(m_pimageCur != 0);

			refimage = *m_pimageCur;
			delete m_pimageCur;
			m_pimageCur = 0;
			fRet = true;
		}
		else
		{
			assert(!m_deque.empty());

			ReadBackIfNeeded();
			refimage = *(m_deque.back().m_pImage);
			DeleteBack(false);
			NeedImage();
			fRet = true;
		}

		LeaveWriteBlock();
	
		if (fRet)
			OnSequenceChanged();
	}

	return fRet;
}


VisCoreExport void CVisSequenceBase::Assign(int n)
{
	if (FEnterWriteBlock())
	{
		DeleteImages();
		
		if (n > 0)
		{
			m_deque.assign(n);

			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
				InitDequeEl( *iteratorT, PImageNew() );
			}

			ImageAdded(n);
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Assign(int n, const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	if (FEnterWriteBlock())
	{
		DeleteImages();
		
		if (n > 0)
		{
			m_deque.assign(n);
            
			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew(refimage) ); 
			}

			ImageAdded(n);
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Assign(int n, const CVisShape& refshape)
{
	if (FEnterWriteBlock())
	{
		DeleteImages();
		
		if (n > 0)
		{
			m_deque.assign(n);

			T_Deque::iterator iteratorT = m_deque.begin();
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew(refshape) ); 
			}

			ImageAdded(n);
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Assign(int n, const char *szName)
{
	// LATER:  We might want to rewrite this to avoid creating the
	// temporary image.
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		Assign(n, *pimagebase);

		delete pimagebase;
	}
}

VisCoreExport void CVisSequenceBase::Assign(
		const CVisSequenceBase& refsequence, int iFirst, int iLim)
{
	if (FEnterWriteBlock())
	{
		DeleteImages();

		if (!refsequence.IsEmpty())
		{
			int di;
			if (iLim == - 1)
				di = refsequence.Length() - iFirst;
			else
				di = iLim - iFirst;
			assert(di > 0);
			if ((m_nLengthMax >= 0) && (di > m_nLengthMax))
				di = m_nLengthMax;

			if (di > 0)
			{
				m_deque.assign(di);

				T_Deque::iterator iteratorT = m_deque.begin();
				T_Deque::iterator iteratorTLim = m_deque.end();
				for (int i = iFirst ; iteratorT != iteratorTLim; ++i, ++iteratorT)
				{
                    InitDequeEl( *iteratorT, PImageNew(*(refsequence.m_deque[i].m_pImage)/*refsequence[i]*/) ); 
				}

				ImageAdded(di);
			}
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}

#ifdef LATER // Build problems
VisCoreExport void CVisSequenceBase::Assign(
		CVisSequenceBase& refsequence, int iFirst, int iLim)
{
	if (FEnterWriteBlock())
	{
		DeleteImages();
		
		if (refsequence.FEnterReadBlock())
		{
			if (!refsequence.IsEmpty())
			{
				int di;
				if (iLim == - 1)
					di = refsequence.Length() - iFirst;
				else
					di = iLim - iFirst;
				assert(di > 0);
				if ((m_nLengthMax >= 0) && (di > m_nLengthMax))
					di = m_nLengthMax;

				if (di > 0)
				{
					m_deque.assign(di);

					T_Deque::iterator iteratorT = m_deque.begin();
					T_Deque::iterator iteratorTLim = m_deque.end();
					for (int i = iFirst ; iteratorT != iteratorTLim; ++i, ++iteratorT)
					{
                        InitDequeEl( *iteratorT, PImageNew(refsequence[i]) ); 
					}

					ImageAdded(di);
				}
			}
			
			refsequence.LeaveReadBlock();
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}
#endif // LATER

VisCoreExport void CVisSequenceBase::Resize(int n)
{
	if (FEnterWriteBlock())
	{
		assert(n <= LengthMax());

		bool fChanged = false;
		int nCur = m_deque.size();
		if (nCur > n)
		{
			DeleteImages(n, nCur);

			fChanged = true;
		}
		else if (nCur < n)
		{
			m_deque.resize(n);

			T_Deque::iterator iteratorT = m_deque.begin() + nCur;
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew() ); 
			}

			ImageAdded(n - nCur);

			fChanged = true;
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Resize(int n, const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	if (FEnterWriteBlock())
	{
		assert(n <= LengthMax());

		bool fChanged = false;
		int nCur = m_deque.size();
		if (nCur > n)
		{
			DeleteImages(n, nCur);

			fChanged = true;
		}
		else if (nCur < n)
		{
			m_deque.resize(n);

			T_Deque::iterator iteratorT = m_deque.begin() + nCur;
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew(refimage) ); 
			}

			ImageAdded(n - nCur);

			fChanged = true;
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Resize(int n, const CVisShape& refshape)
{
	if (FEnterWriteBlock())
	{
		assert(n <= LengthMax());

		bool fChanged = false;
		int nCur = m_deque.size();
		if (nCur > n)
		{
			DeleteImages(n, nCur);

			fChanged = true;
		}
		else if (nCur < n)
		{
			m_deque.resize(n);

			T_Deque::iterator iteratorT = m_deque.begin() + nCur;
			T_Deque::iterator iteratorTLim = m_deque.end();
			for ( ; iteratorT != iteratorTLim; ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew(refshape) ); 
			}

			ImageAdded(n - nCur);

			fChanged = true;
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Resize(int n, const char *szName)
{
	// LATER:  We might want to rewrite this to avoid creating the
	// temporary image.
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		Resize(n, *pimagebase);

		delete pimagebase;
	}
}


VisCoreExport void CVisSequenceBase::Insert(int i)
{
	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);

			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew();
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				DeleteImage(m_deque.size() - 1);
                m_deque.erase(m_deque.end() - 1);    

				fChanged = true;
			}

			if (m_deque.size() < LengthMax())
			{
				m_deque.insert( m_deque.begin() + i,
                                NewDequeEl(PImageNew()) );

				ImageAdded();

				fChanged = true;
			}
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, int n)
{
	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);
			assert(n == 1);
			
			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew();
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert(n <= LengthMax());
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				int cRemove = m_deque.size() + n - m_nLengthMax;
				DeleteImages(m_deque.size() - cRemove, m_deque.size());

				fChanged = true;
			}

			if (m_deque.size() + n > LengthMax())
				n = LengthMax() - m_deque.size();

			if (n > 0)
			{
				m_deque.insert(m_deque.begin() + i, n,
					SVisSequenceDequeEl());
				
				T_Deque::iterator iteratorT = m_deque.begin() + i;
				T_Deque::iterator iteratorTLim = iteratorT + n;
				for ( ; iteratorT != iteratorTLim; ++iteratorT)
				{
                    InitDequeEl( *iteratorT, PImageNew() ); 
				}

				ImageAdded(n);

				fChanged = true;
			}
		}

		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, const CVisShape& refshape)
{
	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);

			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew(refshape);
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				DeleteImage(m_deque.size() - 1);
                m_deque.erase(m_deque.end() - 1);    

				fChanged = true;
			}

			if (m_deque.size() < LengthMax())
			{
				m_deque.insert( m_deque.begin() + i,
                                NewDequeEl(PImageNew(refshape)) );

				ImageAdded();

				fChanged = true;
			}
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, int n,
		const CVisShape& refshape)
{
	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);
			assert(n == 1);
			
			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew(refshape);
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert(n <= LengthMax());
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				int cRemove = m_deque.size() + n - m_nLengthMax;
				DeleteImages(m_deque.size() - cRemove, m_deque.size());

				fChanged = true;
			}

			if (m_deque.size() + n > LengthMax())
				n = LengthMax() - m_deque.size();

			if (n > 0)
			{
				m_deque.insert(m_deque.begin() + i, n,
					SVisSequenceDequeEl());
				
				T_Deque::iterator iteratorT = m_deque.begin() + i;
				T_Deque::iterator iteratorTLim = iteratorT + n;
				for ( ; iteratorT != iteratorTLim; ++iteratorT)
				{
                    InitDequeEl( *iteratorT, PImageNew(refshape) ); 
				}

				ImageAdded(n);

				fChanged = true;
			}
		}

		LeaveWriteBlock();
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);

			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew(refimage);
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				DeleteImage(m_deque.size() - 1);
                m_deque.erase(m_deque.end() - 1);    

				fChanged = true;
			}

			if (m_deque.size() < LengthMax())
			{
				m_deque.insert( m_deque.begin() + i,
                                NewDequeEl(PImageNew(refimage)) );

				ImageAdded();

				fChanged = true;
			}
		}
		
		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, int n,
		const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			assert(i == 0);
			assert(n == 1);

			if (m_pimageCur == 0)
			{
				m_pimageCur = PImageNew(refimage);
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			assert(n <= LengthMax());
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				int cRemove = m_deque.size() + n - m_nLengthMax;
				DeleteImages(m_deque.size() - cRemove, m_deque.size());

				fChanged = true;
			}

			if (m_deque.size() + n > LengthMax())
				n = LengthMax() - m_deque.size();

			if (n > 0)
			{
				m_deque.insert(m_deque.begin() + i, n,
					SVisSequenceDequeEl());
				
				T_Deque::iterator iteratorT = m_deque.begin() + i;
				T_Deque::iterator iteratorTLim = iteratorT + n;
				for ( ; iteratorT != iteratorTLim; ++iteratorT)
				{
                    InitDequeEl( *iteratorT, PImageNew(refimage) ); 
				}

				ImageAdded(n);

				fChanged = true;
			}
		}

		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}


VisCoreExport void CVisSequenceBase::Insert(int i, const char *szName)
{
	// LATER:  We might want to rewrite this to avoid creating the
	// temporary image.
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		Insert(i, *pimagebase);

		delete pimagebase;
	}
}

VisCoreExport void CVisSequenceBase::Insert(int i, int n, const char *szName)
{
	// LATER:  We might want to rewrite this to avoid creating the
	// temporary image.
	CVisImageBase *pimagebase = PImageNew();
	if (pimagebase != 0)
	{
		pimagebase->SetName(szName);

#ifdef LATER  // UNDONE:  Should we have an option to read when inserting?
		if ((EVisSequence() & evissequenceMaskDelayRead)
				== evissequenceDontDelayRead)
		{
			pimagebase->FReadFile();
		}
#endif // LATER

		Insert(i, n, *pimagebase);

		delete pimagebase;
	}
}


VisCoreExport void CVisSequenceBase::Insert(int i,
		const CVisSequenceBase& refsequence, int iFirst, int iLim)
{
	if (FEnterWriteBlock())
	{
		bool fChanged = false;
		if (m_nLengthMax == 0)
		{
			if ((m_pimageCur == 0) && (!refsequence.IsEmpty()))
			{
				assert(iFirst >= 0);
				assert(iFirst < refsequence.Length());

				m_pimageCur = PImageNew(refsequence[iFirst]);
				ImageAdded();

				fChanged = true;
			}
		}
		else
		{
			int n;
			if (iLim == - 1)
				n = refsequence.Length() - iFirst;
			else
				n = iLim - iFirst;
			
			assert(iFirst >= 0);
			assert(iFirst + n <= refsequence.Length());
			assert(n <= LengthMax());
			assert((i >= 0) && (i <= m_deque.size()));
			assert(m_deque.size() <= LengthMax());

			if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
			{
				int cRemove = m_deque.size() + n - m_nLengthMax;
				DeleteImages(m_deque.size() - cRemove, m_deque.size());

				fChanged = true;
			}

			if (m_deque.size() + n > LengthMax())
				n = LengthMax() - m_deque.size();

			if (n > 0)
			{
				m_deque.insert(m_deque.begin() + i, n,
					SVisSequenceDequeEl());
				
				T_Deque::iterator iteratorT = m_deque.begin() + i;
				T_Deque::iterator iteratorTLim = iteratorT + n;
				for (int iimage = iFirst ; iteratorT != iteratorTLim; ++iimage, ++iteratorT)
				{
                    InitDequeEl( *iteratorT, PImageNew(refsequence[iimage]) ); 
				}

				ImageAdded(n);

				fChanged = true;
			}
		}

		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}

#ifdef LATER // Build problems
VisCoreExport void CVisSequenceBase::Insert(int i,
		CVisSequenceBase& refsequence, int iFirst, int iLim)
{
	if (FEnterWriteBlock())
	{
		if (refsequence.FEnterReadBlock())
		{
			bool fChanged = false;
			if (m_nLengthMax == 0)
			{
				if ((m_pimageCur == 0) && (!refsequence.IsEmpty()))
				{
					assert(iFirst >= 0);
					assert(iFirst < refsequence.Length());

					m_pimageCur = PImageNew(refsequence[iFirst]);
					ImageAdded();

					fChanged = true;
				}
			}
			else
			{
				int n;
				if (iLim == - 1)
					n = refsequence.Length() - iFirst;
				else
					n = iLim - iFirst;
				
				assert(iFirst >= 0);
				assert(iFirst + n <= refsequence.Length());
				assert(n <= LengthMax());
				assert((i >= 0) && (i <= m_deque.size()));
				assert(m_deque.size() <= LengthMax());

				if ((InsertCanRemoveOldImages())
					&& (m_deque.size() == LengthMax()))
				{
					int cRemove = m_deque.size() + n - m_nLengthMax;
					DeleteImages(m_deque.size() - cRemove, m_deque.size());

					fChanged = true;
				}

				if (m_deque.size() + n > LengthMax())
					n = LengthMax() - m_deque.size();

				if (n > 0)
				{
					m_deque.insert(m_deque.begin() + i, n, 0);
					
					T_Deque::iterator iteratorT = m_deque.begin() + i;
					T_Deque::iterator iteratorTLim = iteratorT + n;
					for (int iimage = iFirst ; iteratorT != iteratorTLim; ++iimage, ++iteratorT)
					{
                        InitDequeEl( *iteratorT, PImageNew(refsequence[iimage]) ); 
					}

					ImageAdded(n);

					fChanged = true;
				}
			}
			
			refsequence.LeaveReadBlock();
		}

		LeaveWriteBlock();
	
		if (fChanged)
			OnSequenceChanged();
	}
}
#endif // LATER

VisCoreExport void CVisSequenceBase::Swap(CVisSequenceBase& refsequence)
{
	if (FEnterWriteBlock())
	{
		if (refsequence.FEnterReadBlock())
		{
			int n;
			if (m_deque.size() <= LengthMax())
				n = m_deque.size();
			else
				n = m_nLengthMax;
			
			// Copy the images from the other sequence.
			T_Deque dequeT;
			dequeT.resize(n);
			T_Deque::iterator iteratorT = dequeT.begin();
			T_Deque::iterator iteratorTLim = dequeT.end();
			for (int i = 0 ; iteratorT != iteratorTLim; ++i, ++iteratorT)
			{
                InitDequeEl( *iteratorT, PImageNew(refsequence[i]) ); 
			}
			
			refsequence.LeaveReadBlock();

			// Give the images in this sequence to the other sequence.
			refsequence.Assign(*this);

			// Clear the images in this sequence.
			DeleteImages();

			// Move the images that we copied above into this sequence.
			m_deque.swap(dequeT);

			ImageAdded(n);
		}

		LeaveWriteBlock();
	
		OnSequenceChanged();
	}
}


VisCoreExport bool CVisSequenceBase::FPushFrontPImage(CVisImageBase *pimage)
{
	bool fRet = false;

	if (FEnterWriteBlock())
	{
		assert(pimage != 0);

		if (m_nLengthMax == 0)
		{
			if (m_pimageCur == 0)
			{
				m_pimageCur = pimage;
				ImageAdded();
				fRet = true;
			}
			else if (PushCanRemoveOldImages())
			{
				delete m_pimageCur;
				m_pimageCur = pimage;
				fRet = true;
			}
			else
			{
				delete pimage;
			}
		}
		else
		{
			if ((PushCanRemoveOldImages()) && (IsFull()))
			{
				DeleteBack();
			}

			if (IsFull())
			{
				delete pimage;
			}
			else
			{
				m_deque.push_front(NewDequeEl(pimage));
				ImageAdded();
				fRet = true;
			}
		}

		LeaveWriteBlock();
	}

	return fRet;
}

VisCoreExport bool CVisSequenceBase::FPushBackPImage(CVisImageBase *pimage)
{
	bool fRet = false;

	if (FEnterWriteBlock())
	{
		assert(pimage != 0);

		if (m_nLengthMax == 0)
		{
			if (m_pimageCur == 0)
			{
				m_pimageCur = pimage;
				ImageAdded();
				fRet = true;
			}
			else if (PushCanRemoveOldImages())
			{
				delete m_pimageCur;
				m_pimageCur = pimage;
				fRet = true;
			}
			else
			{
				delete pimage;
			}
		}
		else
		{
			if ((PushCanRemoveOldImages()) && (IsFull()))
			{
				DeleteFront();
			}

			if (IsFull())
			{
				delete pimage;
			}
			else
			{
				m_deque.push_back(NewDequeEl(pimage));
				ImageAdded();
				fRet = true;
			}
		}

		LeaveWriteBlock();
	}

	return fRet;
}


VisCoreExport void CVisSequenceBase::DeleteFront(bool fChangeCount)
{
	if (!m_deque.empty())
	{
		DeleteImage( m_deque.front(), fChangeCount );  
		m_deque.pop_front();
	}
}

VisCoreExport void CVisSequenceBase::DeleteBack(bool fChangeCount)
{
	if (!m_deque.empty())
	{
		DeleteImage( m_deque.back(), fChangeCount );
		m_deque.pop_back();
	}
}

VisCoreExport void CVisSequenceBase::DeleteImage(int i, 
                                                 bool fChangeCount)
{
    DeleteImage( m_deque[i], fChangeCount );
}

VisCoreExport void 
CVisSequenceBase::DeleteImage( SVisSequenceDequeEl& sDequeEl,
							   bool fChangeCount )
{
    if( sDequeEl.m_pMemUsage &&
        (sDequeEl.m_pMemUsage->m_pImage == sDequeEl.m_pImage) )
    {
        EmptyMemUsage( sDequeEl.m_pMemUsage );
    }
    sDequeEl.m_pMemUsage = 0;

    if( sDequeEl.m_pImage )
    {
        delete sDequeEl.m_pImage;
        sDequeEl.m_pImage = 0;
        if( fChangeCount )
        {
            ImageRemoved();
        }
    }
}

VisCoreExport void CVisSequenceBase::DeleteImages(void)
{
	if (!m_deque.empty())
	{
		// Destroy images before we loose the pointers.
		T_Deque::iterator iteratorT = m_deque.begin();
		T_Deque::iterator iteratorTLim = m_deque.end();
		for ( ; iteratorT != iteratorTLim; ++iteratorT)
		{
			if ((*iteratorT).m_pImage != 0)
			{
				delete (*iteratorT).m_pImage;
				(*iteratorT).m_pImage    = 0;
                (*iteratorT).m_pMemUsage = 0;
				ImageRemoved();
			}
		}

		m_deque.clear();
        m_iCur = 0;     // reset the current frame to the 1st one
	}

    // destroy the memusage list
    while( m_pMemHead )
    {
        SVisSequenceMemUsageEl* pNext = m_pMemHead->m_pNext;
        delete m_pMemHead;
        m_pMemHead = pNext;
    }
    m_pMemTail      = NULL;
    m_cMemUsageSize = 0;
}

VisCoreExport void CVisSequenceBase::DeleteImages(int iFirst, int iLim)
{
	assert(iFirst <= iLim);

	if ((!m_deque.empty()) && (iLim > iFirst))
	{
		// Destroy images before we loose the pointers.
		T_Deque::iterator iteratorT = m_deque.begin() + iFirst;
		T_Deque::iterator iteratorTLim = m_deque.begin() + iLim;
		for ( ; iteratorT != iteratorTLim; ++iteratorT)
		{
			if ((*iteratorT).m_pImage != 0)
			{
				DeleteImage( *iteratorT );
			}
		}

		m_deque.erase(m_deque.begin() + iFirst, m_deque.begin() + iLim);
	}
}

VisCoreExport CVisSequenceBase::SVisSequenceDequeEl
CVisSequenceBase::NewDequeEl( CVisImageBase* pimage )
{
    SVisSequenceDequeEl sDequeEl;

    InitDequeEl( sDequeEl, pimage );

    return sDequeEl;
}

VisCoreExport void
CVisSequenceBase::InitDequeEl( SVisSequenceDequeEl& sDequeEl, CVisImageBase* pimage )
{
    assert( sDequeEl.m_pImage    == NULL );
    assert( sDequeEl.m_pMemUsage == NULL );

    sDequeEl.m_pImage = pimage;
    if( pimage && pimage->IsValid() )
    {
        UpdateMRU( sDequeEl );
    }
}

VisCoreExport CVisSequenceBase::SVisSequenceMemUsageEl* 
CVisSequenceBase::NewMemUsageEl( CVisImageBase *pImage )
{
    SVisSequenceMemUsageEl* pNewEl = new SVisSequenceMemUsageEl();

    pNewEl->m_pImage = pImage;
    pNewEl->m_pPrev  = m_pMemTail;
    pNewEl->m_pNext  = NULL;

    if( m_pMemHead == NULL )
    {
        m_pMemHead = pNewEl;
    }
    else
    {
        m_pMemTail->m_pNext = pNewEl;
    }
    m_cMemUsageSize++;
    return m_pMemTail = pNewEl;
}

VisCoreExport void CVisSequenceBase::MoveToMRU( SVisSequenceMemUsageEl* pEl )
{
    assert( m_pMemHead && m_pMemTail );

    if ( m_pMemTail != pEl )   // if not already the MRU
    {
        // remove from current position in the list
        if ( pEl == m_pMemHead )
        {
            m_pMemHead = pEl->m_pNext;
			m_pMemHead->m_pPrev = NULL;
        }
        else
        {
            pEl->m_pPrev->m_pNext = pEl->m_pNext;
			pEl->m_pNext->m_pPrev = pEl->m_pPrev;
        }

        // make the argument the (MRU) 
        pEl->m_pPrev = m_pMemTail;
        pEl->m_pNext = NULL;

        m_pMemTail->m_pNext = pEl;
        m_pMemTail          = pEl;
    }
}


VisCoreExport void CVisSequenceBase::RemoveMemUsage( SVisSequenceMemUsageEl* pEl )
{
    assert( m_pMemHead && m_pMemTail );
    
    // remove from current position in the list
    bool fDone = false;
    if ( pEl == m_pMemHead )
    {
        m_pMemHead = pEl->m_pNext;
        if( m_pMemHead )
        {
            m_pMemHead->m_pPrev = NULL;
        }
        fDone = true;
    }

    if( pEl == m_pMemTail )
    {
        m_pMemTail = pEl->m_pPrev;
        if( m_pMemTail )
        {
            m_pMemTail->m_pNext= NULL;
        }
        fDone = true;
    }
    
    if( !fDone )
    {
        pEl->m_pPrev->m_pNext = pEl->m_pNext;
        pEl->m_pNext->m_pPrev = pEl->m_pPrev;
    }

    // Remove any reference to this from the deque
	T_Deque::iterator iteratorT = m_deque.begin();
	T_Deque::iterator iteratorTLim = m_deque.end();
	for ( ; iteratorT != iteratorTLim; ++iteratorT)
	{
		if ((*iteratorT).m_pMemUsage == pEl)
        {
            (*iteratorT).m_pMemUsage = NULL;
            break;
        }
    }

    // should be a reference somewhere
    assert( iteratorT != iteratorTLim );
}

VisCoreExport void CVisSequenceBase::EmptyMemUsage( SVisSequenceMemUsageEl* pDelEl )
{
    pDelEl->m_pImage = 0;
    if( pDelEl == m_pMemHead )
    {
        return;
    }

    if( pDelEl == m_pMemTail )
    {
        m_pMemTail = pDelEl->m_pPrev;
    }

    // remove from current position in the list
    pDelEl->m_pPrev->m_pNext = pDelEl->m_pNext;

    // make this the new LRU
    pDelEl->m_pPrev = NULL;
    pDelEl->m_pNext = m_pMemHead;

    m_pMemHead->m_pPrev = pDelEl;
    m_pMemHead          = pDelEl;
}

VisCoreExport bool CVisSequenceBase::UpdateMRU( SVisSequenceDequeEl& sDequeEl )
{
    if ((m_evissequence & evissequenceMaskLimitMemoryUsage )
					== evissequenceDontLimitMemoryUsage )
    {
        return FALSE;
    }

    assert( sDequeEl.m_pImage );
	
	bool rval;

    //char text[100];

    // following condition is true if this image hasn't yet fallen
    // out of the m_cMemUsageSize most recently used images.  If
    // this is true make this image the MRU.
    if( sDequeEl.m_pMemUsage &&
        sDequeEl.m_pMemUsage->m_pImage == sDequeEl.m_pImage )
    {
        MoveToMRU( sDequeEl.m_pMemUsage );
        rval = FALSE;
    }
    else
    {
		SVisSequenceMemUsageEl* pMemUsage;
        if( m_cMemUsageSize < m_cMemUsageMax )
        {
            pMemUsage = NewMemUsageEl( sDequeEl.m_pImage );
        }
        else
        {
            // goto the first non-dirty image
            pMemUsage = m_pMemHead;
            CVisImageBase*  pImg = pMemUsage->m_pImage;
            while( pImg && 
                   (pImg->IsDirty() || 
                    pImg->Name()==0 || *(pImg->Name())==0)  )
            {
                pMemUsage = pMemUsage->m_pNext;
				// UNDONE mattu: if all images are dirty assert, we could
                //               perhaps temporarily expand the chain?
                assert(pMemUsage);
                pImg      = pMemUsage->m_pImage;
            }

            // deallocate LRU non-dirty image and update the image pointer
            if( pImg )
            {
                pImg->Deallocate();
            }

			//sprintf(text, "deallocate LRU image %x:%x %s",
			//	pMemUsage,pMemUsage->m_pImage,pMemUsage->m_pImage->Name());
			//TRACE("%s\n", text);

            pMemUsage->m_pImage = sDequeEl.m_pImage;

            MoveToMRU( pMemUsage );
        }
		sDequeEl.m_pMemUsage = pMemUsage;
		rval = TRUE;
    }

	assert( sDequeEl.m_pMemUsage->m_pImage == m_pMemTail->m_pImage );
    //sprintf(text, "image %x:%x %s is now MRU",
	//	sDequeEl.m_pMemUsage,m_pMemTail->m_pImage,m_pMemTail->m_pImage->Name());
    //TRACE("%s\n", text);
        
	return rval;
}

VisCoreExport void CVisSequenceBase::SetMRULength( int length )
{
    assert( length );

    if( length < m_cMemUsageSize )
    {
        // deallocate the first 'diff' non-dirty images
        int diff = m_cMemUsageSize - length;

        SVisSequenceMemUsageEl* pMemUsage = m_pMemHead;
        for( int i = 0; i < diff; i++ )
        {
            assert( pMemUsage );

            CVisImageBase*  pImg = pMemUsage->m_pImage;
            while( pImg && 
                   (pImg->IsDirty() || 
                    pImg->Name()==0 || *(pImg->Name())==0)  )
            {
                pMemUsage = pMemUsage->m_pNext;

                // UNDONE mattu: if all images are dirty assert, we could
                //               perhaps temporarily expand the chain?
                assert(pMemUsage);
                pImg      = pMemUsage->m_pImage;
            }
    
            RemoveMemUsage( pMemUsage );
                        
            // deallocate LRU non-dirty image and update the image pointer
            if( pImg )
            {
                pImg->Deallocate();
            }
            SVisSequenceMemUsageEl* pTmp = pMemUsage->m_pNext;
            delete pMemUsage;
            pMemUsage = pTmp;
        }
    }

    m_cMemUsageMax = length;
}

VisCoreExport void CVisSequenceBase::InitSynchObjects(void)
{
	m_rgh[eihSemaphoreImagesAvailable] = CreateSemaphore(0, 0, ecSemaphoreLim, 0);
	m_rgh[eihSemaphoreInBlock] = CreateSemaphore(0, 1, ecSemaphoreLim, 0);
	m_hEventNotInBlock = CreateEvent(0, TRUE, TRUE, 0);

	if ((m_rgh[eihSemaphoreImagesAvailable] == 0)
			|| (m_rgh[eihSemaphoreInBlock] == 0)
			|| (m_hEventNotInBlock == 0))
	{
		assert(0);

		if (m_rgh[eihSemaphoreImagesAvailable] != 0)
		{
			CloseHandle(m_rgh[eihSemaphoreImagesAvailable]);
			m_rgh[eihSemaphoreImagesAvailable] = 0;
		}
		if (m_rgh[eihSemaphoreInBlock] != 0)
		{
			CloseHandle(m_rgh[eihSemaphoreInBlock]);
			m_rgh[eihSemaphoreInBlock] = 0;
		}
		if (m_hEventNotInBlock != 0)
		{
			CloseHandle(m_hEventNotInBlock);
			m_hEventNotInBlock = 0;
		}

		throw CVisError("Failed to create synchronization objects.",
				eviserrorUnexpected, "CVisSequenceBase::InitSynchObjects",
				"VisSequence.cpp", __LINE__);
	}
}


VisCoreExport void CVisSequenceBase::OnICurChanged(void)
{
}


VisCoreExport void CVisSequenceBase::OnOrderChanged(void)
{
}




void CVisSequenceBase::ReadValue(CVisSDIStream& refsdistream)
{
	// UNDONE:  Change working directory if needed

	PreReadValue(refsdistream);

	EVisPixFmt pixfmtPrev = PixFmt();


	// This code is similar to the code in CVisSDIStream::ReadClassUsingPropList,
	// but this method calls special-case code to read the list of images.
	refsdistream.OpenGroup();

	CVisPropTypeInfoBase *pproptypeinfo = VisPPropTypeInfoForClass(*this);
	CVisPropList& refproplist = *(pproptypeinfo->PPropList());
	assert(&refproplist != 0);

	while (!refsdistream.AtEndOfGroup())
	{
		// Need to read type and name and then create storate for object.
		std::string strType;
		std::string strName;
		bool fShared;
		CVisDim dim;
		refsdistream.ReadTypeNameAndDim(strType, strName, dim, &fShared);
		assert(!strName.empty());

		if (strName == "m_deque")
		{
			ReadImageList(refsdistream);
		}
		else
		{
			refproplist.ReadPropValue(strType.c_str(), strName.c_str(), dim,
					fShared, refsdistream, (void *) this);
		}
		refsdistream.EndObj();
	}

	refsdistream.CloseGroup();


	if (PixFmt() != pixfmtPrev)
	{
		// LATER:  Should this be a CVisFileIOError?
		throw CVisError(
				"Sequence pixel type mismatch when reading from a stream.",
				eviserrorPixFmt, "CVisSequenceBase::ReadValue",
				__FILE__, __LINE__);
	}

	PostReadValue(refsdistream);

	// UNDONE:  Change working directory back if needed
}

void CVisSequenceBase::WriteValue(CVisSDOStream& refsdostream) const
{
	// UNDONE:  Change working directory if needed

	PreWriteValue(refsdostream);

	// This code is similar to the code in CVisSDOStream::WriteClassUsingPropList,
	// but this method calls special-case code to write the list of images.
	refsdostream.OpenGroup(true);

	CVisPropTypeInfoBase *pproptypeinfo = VisPPropTypeInfoForClass(*this);
	CVisPropList& refproplist = *(pproptypeinfo->PPropList());
	assert(&refproplist != 0);

	CVisPropList::const_iterator iteratorPropListUser
			= refproplist.IteratorFromPropName(VIS_SZ_SD_USER_PROP_LIST);
	assert(iteratorPropListUser != refproplist.end());

	// Write class properties
	bool fPrintPropListUser = refproplist.IsPrinted(iteratorPropListUser);
	refproplist.SetPrinting(iteratorPropListUser, false);
	refproplist.WriteValue(refsdostream, false, (const void *) this);
	refproplist.SetPrinting(iteratorPropListUser, fPrintPropListUser);

	// Write user-defined properties
	if (fPrintPropListUser)
		PropList().WriteValue(refsdostream, false, (const void *) this);

	// Write images
	WriteImageList(refsdostream);

	refsdostream.CloseGroup(true);


	PostWriteValue(refsdostream);

	// UNDONE:  Change working directory back if needed
}

void CVisSequenceBase::BuildPropList(CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());

	refproplist.AddPropReference("m_evissequence",
			(unsigned int &) m_evissequence, false);
	refproplist.SetPrinting("m_evissequence", false);

	refproplist.AddPropReference("m_iCur", m_iCur, false);

	// LATER:  Add special I/O code that only reads or writes m_nLengthMax if
	// m_fCalledSetLengthMax is true.
	refproplist.AddPropReference("m_fCalledSetLengthMax",
			m_fCalledSetLengthMax, false);
	refproplist.AddPropReference("m_nLengthMax", m_nLengthMax, false);

	refproplist.AddPropReference("m_evispixfmt",
			(unsigned int &) m_evispixfmt, false);

	// LATER:  Should Working Directory be in the prop list?
//	refproplist.AddPropReference("m_strWorkingDirectory", m_strWorkingDirectory,
//		false);
	// LATER:  ImageSource information?

	// The name VIS_SZ_SD_USER_PROP_LIST indicates a special class member
	// that stores properties added by the user.
	refproplist.AddPropReference(VIS_SZ_SD_USER_PROP_LIST, m_proplist, false);
}

CVisPropTypeInfoBase *CVisSequenceBase::PropTypeInfoLookup(void) const
{
	// LATER:  Make this a pure virtual method after the old SDStream code is
	// removed.
	assert(0);
	return 0;
}

CVisPropTypeInfoBase *CVisSequenceBase::PropTypeInfoNew(void) const
{
	// LATER:  Make this a pure virtual method after the old SDStream code is
	// removed.
	assert(0);
	return 0;
}


void CVisSequenceBase::ReadImageList(CVisSDIStream& refsdistream)
{
	refsdistream.OpenGroup();

	bool fForceDelayRead = ((SequenceOptions() & evissequenceMaskDelayRead)
			== evissequenceDelayReadUntilNeeded);

	while (!refsdistream.AtEndOfGroup())
	{
		PushBack();
		// The call to the ReadValue method assumes that VisSDReadValue
		// just calls ReadValue when passed an image reference.
		m_deque.back().m_pImage->ReadValue(refsdistream, fForceDelayRead);
	}

	refsdistream.CloseGroup();
}

void CVisSequenceBase::WriteImageList(CVisSDOStream& refsdostream) const
{
	// UNDONE:  Do we want to write an empty m_deque if the sequence is empty?
	// (We'd have to create a temporary image object to get the image type right.)
	if (!IsEmpty())
	{
		CVisPropTypeInfoBase *pproptypeinfo
				= VisGetPropTypeInfo(*m_deque.front().m_pImage);
		assert(pproptypeinfo != 0);

		// UNDONE:  Provide a global function to make this name (to make sure that
		// it is the same name used with std::deque).  We need to decide whether
		// there should be a ' ' character before the '>' suffix.
		std::string strType = "deque<";
		strType += pproptypeinfo->Name();
		strType += ">";

		refsdostream.StartObjType(strType.c_str(), "m_deque");
		refsdostream.OpenGroup(true);

		// The call to the WriteValue method assumes that VisSDWriteValue
		// just calls WriteValue when passed an image reference.
		for (int i = 0; i < m_deque.size(); ++i)
			m_deque[i].m_pImage->WriteValue(refsdostream);

		refsdostream.CloseGroup(true);
		refsdostream.EndObj();
	}
}


void CVisSequenceBase::PreReadValue(CVisSDIStream& refsdistream)
{
	Clear();
}

void CVisSequenceBase::PostReadValue(CVisSDIStream& refsdistream)
{
    OpenStreams();
}

void CVisSequenceBase::PreWriteValue(CVisSDOStream& refsdostream) const
{
	if ((refsdostream.FileName() != 0)
			&& ((SequenceOptions() & evissequenceMaskRenameImagesWhenWriting)
				== evissequenceRenameImagesWhenWriting))
	{
		std::string strBaseName;
		const char *pch = strrchr(refsdostream.FileName(), '\\');
		if (pch == 0)
			strBaseName = refsdostream.FileName();
		else
			strBaseName = pch + 1;
		int iLastDot = strBaseName.find_last_of('.');
		if (iLastDot != std::string::npos)
			strBaseName.erase(iLastDot, std::string::npos);

		const_cast<CVisSequenceBase&>(*this).RenameFiles(strBaseName.c_str(), "bmp");
	}

	if ((SequenceOptions() & evissequenceMaskWriteImagesWhenWriting)
			== evissequenceWriteImagesWhenWriting)
	{
		const_cast<CVisSequenceBase&>(*this).WriteFiles();
	}
}

void CVisSequenceBase::PostWriteValue(CVisSDOStream& refsdostream) const
{
}

void CVisSequenceBase::SDOFindTypes(CVisSDOStream& refsdostream) const
{
	refsdostream.UsePropListObjType(*this);

	if (!IsEmpty())
	{
		// Tell the stream about the contained image type.
		VisSDOFindTypes(refsdostream, *m_deque.front().m_pImage );

		// Tell the stream about any user-defined properties.
		for (int i = 1; i < m_deque.size(); ++i)
		{
			CVisPropList& refproplistUser = m_deque[i].m_pImage->PropList();
			if (!refproplistUser.IsEmpty())
				VisSDOFindTypes(refsdostream, refproplistUser);
		}
	}
}
#ifdef VIS_INCLUDE_SDSTREAM
inline CVisSDObject< CVisSequenceBase >&
	CVisSDObject< CVisSequenceBase >::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject< CVisSequenceBase > g_vissequencebaseExemplar;

	return g_vissequencebaseExemplar;
}
#endif // VIS_INCLUDE_SDSTREAM
// UNDONE:  The FindTypes functions should tell about the contained image type.
VIS_DEFINE_PROP_LIST_CLASS_VIRTUAL(VisCoreExportDeclspec, CVisSequenceBase)

// CVisSDIStream doesn't really need to know about contained image types,
// so we can use the default implementation.
VIS_DEFINE_SDI_FIND_TYPES_PL(VisCoreExportDeclspec, CVisSequenceBase)

VisPropListExport void VisSDOFindTypes(CVisSDOStream& refsdostream,
		CVisSequenceBase const& refsequence)
{
	refsequence.SDOFindTypes(refsdostream);
}

VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DEFINE_SDI_READ_OBJ_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DEFINE_SDI_READ_OP_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)

VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DEFINE_SDO_WRITE_OP_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)




// Old stream I/O
/*
VisCoreExport const char *CVisSequenceBase::ReadWriteField(CVisSDStream& s,
		int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM
    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisSequenceBase {";

    case eftProperties:
		s.NewLine();
		s.OpenParen();
		s.NewLine();
		SDReadWriteProperties(s);
		// UNDONE (in Rel1):  Skip extra fields until we get to the next brace.
		// UNDONE (in Rel1):  Should the (derived) class name be written to the file.
		s.CloseParen();
		return "std::list<void *> m_listProps;  // Properties";

    case eftImages:
		s.NewLine();
		s.OpenParen();
		s.NewLine();
		ReadWriteImages(s);
		// UNDONE (in Rel1):  Skip extra fields until we get to the next brace.
		s.CloseParen();
        return "std::deque<class CVisImageBase*> m_dequeImages;  // Images";
    }

    return 0;
#else
	assert(false);  //SDStreams are not included
	return 0;
#endif
}
*/

VisCoreExport void CVisSequenceBase::SDReadWriteProperties(CVisSDStream& s, bool fAddComma)
{
#ifdef VIS_INCLUDE_SDSTREAM
	s.Num((int&) m_evissequence, true);
	s.Comma();
	s.Comment("Sequence options");

	s << m_iCur;
	s.Comma();
	s.Comment("Current index");

	s.OpenParen();

	s << m_fCalledSetLengthMax;
	s.Comma();
	
	s.Num((int&) m_nLengthMax, true);

	s.CloseParen();
	
	if (fAddComma)
		s.Comma();

	s.Comment("Max length");
#else
	assert(false);  //SDStreams are not included
#endif
}



// Move to VisSequence.cpp

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<BYTE> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<int> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<float> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<double> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<CVisRGBA<int> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<CVisRGBA<float> > >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisSequence<CVisRGBA<double> > >(0, fAlwaysTypedef); }



