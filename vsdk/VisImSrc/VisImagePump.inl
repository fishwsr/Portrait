// VisImagePump.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved



inline CVisImagePumpBase::CVisImagePumpBase(CVisSequenceBase& refsequencebase,
			T_PfnPumpCallback pfnCallback, void *pvUser, bool fStartSuspended)
  : CVisPump(pfnCallback, pvUser, true),
	m_refsequencebase(refsequencebase),
	m_fControlsContinuousGrab(true)
{
}


inline bool CVisImagePumpBase::HasImageSource(void) const
{
	return m_refsequencebase.HasImageSource();
}

inline const CVisImageSource& CVisImagePumpBase::ImageSource(void) const
{
	return m_refsequencebase.ImageSource();
}

inline CVisImageSource& CVisImagePumpBase::ImageSource(void)
{
	return m_refsequencebase.ImageSource();
}

inline bool CVisImagePumpBase::ConnectToSource(
		CVisImageSource& refimagesource, bool fModifyImEncoding,
		bool fUseContinuousGrab)
{
	return m_refsequencebase.ConnectToSource(refimagesource, fModifyImEncoding,
			fUseContinuousGrab);
}

inline void CVisImagePumpBase::DisconnectFromSource(void)
{
	m_refsequencebase.DisconnectFromSource();
}

inline bool CVisImagePumpBase::ControlsContinuousGrab(void) const
{
	return m_fControlsContinuousGrab;
}

inline void CVisImagePumpBase::SetControlsContinuousGrab(bool f)
{
	m_fControlsContinuousGrab = f;
}

inline void CVisImagePumpBase::OnRun(void)
{
	if ((ControlsContinuousGrab()) && (HasImageSource()))
		ImageSource().SetUseContinuousGrab(true);
}

inline void CVisImagePumpBase::OnPause(void)
{
	if ((ControlsContinuousGrab()) && (HasImageSource()))
		ImageSource().SetUseContinuousGrab(false);
}



template<class TPixel>
inline CVisImagePump<TPixel>::CVisImagePump(
		T_PfnImagePumpCallback pfnCallback, void *pvUser,
		bool fStartSuspended, int nSequenceLenghthMax, int evissequence)
  : CVisImagePumpBase(m_sequence, (CVisPump::T_PfnPumpCallback) pfnCallback,
			pvUser, true),
	m_sequence(nSequenceLenghthMax, evissequence),
	m_image()
{
#ifdef VIS_OLD_IMAGE_PUMP

	// By default, the old class found an image source in its constructor.
//	FindImageSource();

#endif // VIS_OLD_IMAGE_PUMP

	if (!fStartSuspended)
		Run();
}


template<class TPixel>
inline CVisImagePump<TPixel>::~CVisImagePump(void)
{
	KillThread();
	m_sequence.Clear();
}


template<class TPixel>
inline const CVisImage<TPixel>& CVisImagePump<TPixel>::Image(void) const
{
	assert(m_image.IsValid());
	return m_image;
}

template<class TPixel>
inline CVisImage<TPixel>& CVisImagePump<TPixel>::Image(void)
{
	assert(m_image.IsValid());
	return m_image;
}


template<class TPixel>
inline const CVisSequence<TPixel> &CVisImagePump<TPixel>::Sequence(void) const
{
	return m_sequence;
}

template<class TPixel>
inline CVisSequence<TPixel> &CVisImagePump<TPixel>::Sequence(void)
{
	return m_sequence;
}



template<class TPixel>
inline bool CVisImagePump<TPixel>::PumpCore(void)
{
	bool fRet = true;

	// (If we're in Step(), the INFINITE timeout could halt the program!)
	DWORD cmsTimeout = ((FStepping()) ? 5000 : INFINITE);

	// We need to store the image in a member variable so that it will
	// be accessible to the callback function.  (The callback function
	// is stored in the base class, and we don't want to make an unsafe
	// type cast.)
	bool fPopped = m_sequence.Pop(ImageInternal(), cmsTimeout);
	if ((FStepping()) && (fPopped))
		SetFStepSucceeded(fPopped);

	if ((fPopped) && (ImageInternal().IsValid()))
	{
		if (!FKillThread())
			fRet = ProcessImage(ImageInternal());

		// Free memory used by the image as soon as we're done
		// processing it.
		ImageInternal().Deallocate();
	}

	return fRet;
}


template<class TPixel>
inline bool CVisImagePump<TPixel>::ProcessImage(T_Image& refimage)
{
	if (PfnCallback() != 0)
	{
		// We need to store the image in a member variable so that it will
		// be accessible to the callback function.  (The callback function
		// is stored in the base class, and we don't want to make an unsafe
		// type cast.)
		assert(Image().IsValid());
		assert(&refimage == &Image());

		return PfnCallback()(this);
	}


#ifdef VIS_OLD_IMAGE_PUMP

	// Call the old overload of this method.
	return ProcessImage();

#else // VIS_OLD_IMAGE_PUMP

	// Don't do anything if there's no callback function.
	SetFIsProcessing(false);

	return true;

#endif // VIS_OLD_IMAGE_PUMP
}

template<class TPixel>
inline void CVisImagePump<TPixel>::KillThread(void)
{
	if (HThread() != 0)
	{
		SetFKillThread(true);
		SetFIsProcessing(false);
		SetEvent(HRunning());

		if (!FStarted())
			StartThread(true);

		// Add another image to make sure that we don't get stuck with the
		// INFINITE timeout when we try to pop an image from the sequence.
		T_Image imageT;
		Sequence().Push(imageT);

		VisWaitForSingleObject(HThread(), INFINITE);
		ClearHThread();
	}
}


template<class TPixel>
inline const CVisImage<TPixel>& CVisImagePump<TPixel>::ImageInternal(void) const
{
	return m_image;
}

template<class TPixel>
inline CVisImage<TPixel>& CVisImagePump<TPixel>::ImageInternal(void)
{
	return m_image;
}




#ifdef VIS_OLD_IMAGE_PUMP
template<class TPixel>
inline CVisImagePump<TPixel>::CVisImagePump(
		T_PfnImagePumpCallback pfnCallback, void *pvUser,
		bool fStartSuspended, bool fFindImageSource,
		int nSequenceLenghthMax, int evissequence)
  : CVisImagePumpBase(m_sequence, (CVisPump::T_PfnPumpCallback) pfnCallback,
			pvUser, true),
	m_sequence(nSequenceLenghthMax, evissequence),
	m_image()
{
	if (fFindImageSource)
		FindImageSource();

	if (!fStartSuspended)
		StartThread(HasImageSource());
}


template<class TPixel>
inline BOOL CVisImagePump<TPixel>::ResumeCapture(void)
{
	return Run();
}

template<class TPixel>
inline void CVisImagePump<TPixel>::SuspendCapture(void)
{
	Pause();
}

template<class TPixel>
inline bool CVisImagePump<TPixel>::CaptureActive(void) const
{
	return IsRunning();
}

template<class TPixel>
inline bool CVisImagePump<TPixel>::FindImageSource(class CWnd* pWndParent,
		bool fAlwaysAsk)
{
	return VisFindImageSource(m_sequence, pWndParent, fAlwaysAsk);
}

template<class TPixel>
inline bool CVisImagePump<TPixel>::FindImageSource(const char *szImageSourceID)
{
	return VisFindImageSource(m_sequence, szImageSourceID);
}


template<class TPixel>
inline void CVisImagePump<TPixel>::AddImage(
		const CVisImage<TPixel> & refimage)
{
	return m_sequence.Push(refimage);
}


template<class TPixel>
inline bool CVisImagePump<TPixel>::ProcessImage(void)
{
	// Stop trying to grab images if this method is not overridden.
	return false;
}
#endif // VIS_OLD_IMAGE_PUMP
