// VisSequence.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_IMAGEQUEUE_INL__
#define __VIS_CORE_IMAGEQUEUE_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



inline int CVisSequenceBase::Length(void) const
{
	return m_deque.size();
}

inline int CVisSequenceBase::LengthMax(void) const
{
	assert(m_nLengthMax >= 0);
	return m_nLengthMax;
}

inline bool CVisSequenceBase::IsEmpty() const
{
	return m_deque.empty();
}

inline bool CVisSequenceBase::IsFull() const

{
	if (m_deque.size() == m_nLengthMax)
		return true;

	return false;
}


inline int CVisSequenceBase::SequenceOptions(void) const
{
	return m_evissequence;
}

inline void CVisSequenceBase::SetSequenceOptions(int evissequence)
{
	m_evissequence = evissequence;
}


inline int CVisSequenceBase::ICur(void) const
{
	return m_iCur;
}

inline const CVisImageBase& CVisSequenceBase::Cur(void) const
{
	return At(ICur());
}

inline CVisImageBase& CVisSequenceBase::Cur(void)
{
	return At(ICur());
}

inline bool CVisSequenceBase::GetCur(CVisImageBase& refimage)
{
	assert(&refimage != 0);

	return GetAt(ICur(), refimage);
}


inline bool CVisSequenceBase::MoveForward(bool fToFront)
{
	return MoveForward(ICur(), fToFront);
}


inline bool CVisSequenceBase::MoveBackward(bool fToBack)
{
	return MoveBackward(ICur(), fToBack);
}

inline bool CVisSequenceBase::Swap(int i)
{
	return Swap(i, ICur());
}


inline EVisPixFmt CVisSequenceBase::PixFmt(void) const
{
	return m_evispixfmt;
}


inline bool CVisSequenceBase::HasImageSource(void) const
{
	if (m_pimagesource != 0)
		return true;

	return false;
}

inline const CVisImageSource& CVisSequenceBase::ImageSource(void) const
{
	return *m_pimagesource;
}

inline CVisImageSource& CVisSequenceBase::ImageSource(void)
{
	return *m_pimagesource;
}

inline const CVisPropList& CVisSequenceBase::PropList(void) const
{
	return m_proplist;
}

inline CVisPropList& CVisSequenceBase::PropList(void)
{
	return m_proplist;
}


inline bool CVisSequenceBase::IsDying(void) const
{
	return m_IsDying;
}



inline void CVisSequenceBase::Push(void)
{
	FPushBackPImage(PImageNew());
}

inline void CVisSequenceBase::Push(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	FPushBackPImage(PImageNew(refimage));
}

inline void CVisSequenceBase::Push(const CVisShape& refshape)
{
	FPushBackPImage(PImageNew(refshape));
}

inline void CVisSequenceBase::Push(const char *szName)
{
	PushBack(szName);
}

inline bool CVisSequenceBase::Pop(void)
{
	return PopFront();
}

inline bool CVisSequenceBase::Pop(DWORD cmsTimeout)
{
	return PopFront(cmsTimeout);
}

inline bool CVisSequenceBase::Pop(CVisImageBase& refimage, DWORD cmsTimeout)
{
	assert(&refimage != 0);

	return PopFront(refimage, cmsTimeout);
}

inline void CVisSequenceBase::PushFront(void)
{
	FPushFrontPImage(PImageNew());
}

inline void CVisSequenceBase::PushFront(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	FPushFrontPImage(PImageNew(refimage));
}

inline void CVisSequenceBase::PushFront(const CVisShape& refshape)
{
	FPushFrontPImage(PImageNew(refshape));
}

inline void CVisSequenceBase::PushBack(void)
{
	FPushBackPImage(PImageNew());
}

inline void CVisSequenceBase::PushBack(const CVisImageBase& refimage)
{
	assert(&refimage != 0);

	FPushBackPImage(PImageNew(refimage));
}

inline void CVisSequenceBase::PushBack(const CVisShape& refshape)
{
	FPushBackPImage(PImageNew(refshape));
}

inline bool CVisSequenceBase::FEnterBlock(bool fWriteBlock, DWORD cmsTimeout)
{
	// LATER:  Change this to allow multiple reads at the same time?
	return FEnterWriteBlock(cmsTimeout);
}

inline bool CVisSequenceBase::FEnterReadBlock(DWORD cmsTimeout)
{
	// LATER:  Change this to allow multiple reads at the same time?
	return FEnterWriteBlock(cmsTimeout);
}


inline bool CVisSequenceBase::FEnterReadBlockWithImage(DWORD cmsTimeout)
{
	// LATER:  Change this to allow multiple reads at the same time?
	return FEnterWriteBlockWithImage(cmsTimeout);
}


inline void CVisSequenceBase::LeaveBlock(bool fWriteBlock)
{
	// LATER:  Change this to allow multiple reads at the same time?
	LeaveWriteBlock();
}

inline void CVisSequenceBase::LeaveReadBlock(void)
{
	// LATER:  Change this to allow multiple reads at the same time?
	LeaveWriteBlock();
}




inline void CVisSequenceBase::NeedImage(void)
{
	if (m_pimagesource != 0)
	{
		((CVisImageSourceBase *) m_pimagesource)->NeedImage(*this);
	}
}


inline void CVisSequenceBase::ReadFrontIfNeeded(void) const
{
	ReadIfNeeded(0);
}

inline void CVisSequenceBase::ReadBackIfNeeded(void) const
{
	ReadIfNeeded(m_deque.size() - 1);
}


inline void CVisSequenceBase::CopyPImageSource(
		const CVisImageSource *pimagesource)
{
	assert(m_pimagesource == 0);
	
	if (pimagesource != 0)
	{
		((CVisImageSourceBase *) pimagesource)->AddSequence(*this);
	}
}


inline bool CVisSequenceBase::PushCanRemoveOldImages(void) const
{
	if ((m_evissequence & evissequenceMaskPushRemovesOldImages)
			== evissequencePushCanRemoveOldImages)
	{
		return true;
	}

	return false;
}

inline bool CVisSequenceBase::InsertCanRemoveOldImages(void) const
{
	if ((m_evissequence & evissequenceMaskInsertRemovesOldImages)
			== evissequenceInsertCanRemoveOldImages)
	{
		return true;
	}

	return false;
}


template<class TPixel>
inline CVisSequence<TPixel>::CVisSequence(int nLength,
		int evissequence)
  : CVisSequenceBase(EVisPixFmtGet(), evissequence)
{
	Assign(nLength);
}

template<class TPixel>
inline CVisSequence<TPixel>::CVisSequence(int nLength,
		const CVisSequence<TPixel>::T_Image& refimage, int evissequence)
  : CVisSequenceBase(EVisPixFmtGet(), evissequence)
{
	Assign(nLength, refimage);
}

template<class TPixel>
inline CVisSequence<TPixel>::CVisSequence(int nLength,
		const CVisShape& refshape, int evissequence)
  : CVisSequenceBase(EVisPixFmtGet(), evissequence)
{
	Assign(nLength, refshape);
}

template<class TPixel>
inline CVisSequence<TPixel>::CVisSequence(
		const CVisSequence<TPixel>& refsequence, int iFirst,
		int iLim)
  : CVisSequenceBase(EVisPixFmtGet(), refsequence.SequenceOptions())
{
	assert(PixFmt() == refsequence.PixFmt());

	Assign(refsequence, iFirst, iLim);

	CopyPImageSource(&(refsequence.ImageSource()));
}

template<class TPixel>
inline CVisSequence<TPixel>::CVisSequence(
		const CVisSequence<TPixel>& refsequence, int iFirst,
		int iLim, int evissequence)
  : CVisSequenceBase(EVisPixFmtGet(), evissequence)
{
	assert(PixFmt() == refsequence.PixFmt());

	Assign(refsequence, iFirst, iLim);

	CopyPImageSource(&(refsequence.ImageSource()));
}

template<class TPixel>
inline CVisSequence<TPixel>& CVisSequence<TPixel>::operator=(
		const CVisSequence<TPixel>& refsequence)
{
	CVisSequenceBase::operator=(refsequence);
	return *this;
}

template<class TPixel>
inline CVisSequence<TPixel>::~CVisSequence(void)
{
	SequenceIsDying();
}

template<class TPixel>
inline const CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Front(void) const
{
	return (const CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::Front();
}

template<class TPixel>
inline CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Front(void)
{
	return (CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::Front();
}

template<class TPixel>
inline const CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Back(void) const
{
	return (const CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::Back();
}

template<class TPixel>
inline CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Back(void)
{
	return (CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::Back();
}

template<class TPixel>
inline const CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::At(int i) const
{
	return (const CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::At(i);
}

template<class TPixel>
inline CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::At(int i)
{
	return (CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::At(i);
}

template<class TPixel>
inline const CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::operator[](int i) const
{
	return (const CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::At(i);
}

template<class TPixel>
inline CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::operator[](int i)
{
	return (CVisSequence<TPixel>::T_Image&)
			CVisSequenceBase::At(i);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::GetFront(
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::GetFront(refimage, cmsTimeout);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::GetBack(
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::GetBack(refimage, cmsTimeout);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::GetAt(int i,
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::GetAt(i, refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Push(void)
{
	CVisSequenceBase::PushBack();
}

template<class TPixel>
inline void CVisSequence<TPixel>::Push(
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::PushBack(refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Push(const CVisShape& refshape)
{
	CVisSequenceBase::PushBack(refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Push(const char *szName)
{
	CVisSequenceBase::PushBack(szName);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::Pop(void)
{
	return CVisSequenceBase::PopFront();
}

template<class TPixel>
inline bool CVisSequence<TPixel>::Pop(DWORD cmsTimeout)
{
	return CVisSequenceBase::PopFront(cmsTimeout);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::Pop(
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::PopFront(refimage, cmsTimeout);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushFront(void)
{
	CVisSequenceBase::PushFront();
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushFront(
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::PushFront(refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushFront(const CVisShape& refshape)
{
	CVisSequenceBase::PushFront(refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushFront(const char *szName)
{
	CVisSequenceBase::PushFront(szName);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::PopFront(void)
{
	return CVisSequenceBase::PopFront();
}

template<class TPixel>
inline bool CVisSequence<TPixel>::PopFront(
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::PopFront(refimage, cmsTimeout);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushBack(void)
{
	CVisSequenceBase::PushBack();
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushBack(
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::PushBack(refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushBack(const CVisShape& refshape)
{
	CVisSequenceBase::PushBack(refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::PushBack(const char *szName)
{
	CVisSequenceBase::PushBack(szName);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::PopBack(void)
{
	return CVisSequenceBase::PopBack();
}

template<class TPixel>
inline bool CVisSequence<TPixel>::PopBack(
		CVisSequence<TPixel>::T_Image& refimage, DWORD cmsTimeout)
{
	return CVisSequenceBase::PopBack(refimage, cmsTimeout);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Assign(int n)
{
	CVisSequenceBase::Assign(n);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Assign(int n,
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::Assign(n, refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Assign(int n,
		const CVisShape& refshape)
{
	CVisSequenceBase::Assign(n, refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Assign(int n,
		const  char *szName)
{
	CVisSequenceBase::Assign(n, szName);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Assign(
		const CVisSequence<TPixel>& refsequence, int iFirst,
		int iLim)
{
	CVisSequenceBase::Assign(refsequence, iFirst, iLim);
}

#ifdef LATER // Build problems
template<class TPixel>
inline void CVisSequence<TPixel>::Assign(
		CVisSequence<TPixel>& refsequence, int iFirst,
		int iLim)
{
	CVisSequenceBase::Assign(refsequence, iFirst, iLim);
}
#endif // LATER

template<class TPixel>
inline void CVisSequence<TPixel>::Resize(int n)
{
	CVisSequenceBase::Resize(n);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Resize(int n,
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::Resize(n, refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Resize(int n,
		const CVisShape& refshape)

{
	CVisSequenceBase::Resize(n, refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Resize(int n,
		const char *szName)
{
	CVisSequenceBase::Resize(n, szName);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i)
{
	CVisSequenceBase::Insert(i);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i, int n)
{
	CVisSequenceBase::Insert(n, n);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i,
		const CVisShape& refshape)
{
	CVisSequenceBase::Insert(i, refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i, int n,
		const CVisShape& refshape)
{
	CVisSequenceBase::Insert(i, n, refshape);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i,
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::Insert(i, refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i, int n,
		const CVisSequence<TPixel>::T_Image& refimage)
{
	CVisSequenceBase::Insert(i, n, refimage);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i,
		const char *szName)
{
	CVisSequenceBase::Insert(i, szName);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i, int n,
		const char *szName)
{
	CVisSequenceBase::Insert(i, n, szName);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i,
		const CVisSequence<TPixel>& refsequence, int iFirst, int iLim)

{
	CVisSequenceBase::Insert(i, refsequence, iFirst, iLim);
}

#ifdef LATER // Build problems
template<class TPixel>
inline void CVisSequence<TPixel>::Insert(int i,
		CVisSequence<TPixel>& refsequence, int iFirst, int iLim)

{
	CVisSequenceBase::Insert(i, refsequence, iFirst, iLim);
}
#endif // LATER

template<class TPixel>
inline bool CVisSequence<TPixel>::Swap(int i)
{
	return CVisSequenceBase::Swap(i);
}

template<class TPixel>
inline bool CVisSequence<TPixel>::Swap(int i, int j)
{
	return CVisSequenceBase::Swap(i, j);
}

template<class TPixel>
inline void CVisSequence<TPixel>::Swap(
		CVisSequence<TPixel>& refsequence)
{
	CVisSequenceBase::Swap(refsequence);
}


template<class TPixel>
inline const CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Cur(void) const
{
	return At(ICur());
}

template<class TPixel>
inline CVisSequence<TPixel>::T_Image&
	CVisSequence<TPixel>::Cur(void)
{
	return At(ICur());
}

template<class TPixel>
inline bool CVisSequence<TPixel>::GetCur(
		CVisSequence<TPixel>::T_Image& refimage)
{
	return GetAt(ICur(), refimage);
}



template<class TPixel>
inline const type_info& CVisSequence<TPixel>::PixelTypeInfo(void) const
{
	return typeid(TPixel);
}

template<class TPixel>
inline const type_info& CVisSequence<TPixel>::ImageTypeInfo(void) const
{
	return typeid(CVisSequence<TPixel>::T_Image);
}

template<class TPixel>
inline const type_info& CVisSequence<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if (iLevel == 0)
		return typeid(CVisSequenceBase);
	
	return typeid(CVisSequence<TPixel>);
}

template<class TPixel>
inline CVisSequenceBase *CVisSequence<TPixel>::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisSequence<TPixel>(*this);

	return new CVisSequence<TPixel>;
}



template<class TPixel>
inline CVisImageBase *CVisSequence<TPixel>::PImageNew(void) const
{
	return new CVisSequence<TPixel>::T_Image;
}

template<class TPixel>
inline CVisImageBase *CVisSequence<TPixel>::PImageNew(
		const CVisShape& refshape) const
{
	return new CVisSequence<TPixel>::T_Image(refshape);
}

template<class TPixel>
inline CVisImageBase *CVisSequence<TPixel>::PImageNew(
		const CVisImageBase& refimage) const
{
	assert(&refimage != 0);
	assert(refimage.PixelTypeInfo() == PixelTypeInfo());

	return new CVisSequence<TPixel>::T_Image(
			(const CVisSequence<TPixel>::T_Image &) refimage);
}



template<class TPixel>
inline EVisPixFmt CVisSequence<TPixel>::EVisPixFmtGet(void)
{
#ifdef VIS_OLD_CODE

	static EVisPixFmt evispixfmtRet = evispixfmtNil;

	if (evispixfmtRet == evispixfmtNil)
	{
		evispixfmtRet = VisPixFmtGetTPixel(TPixel());
	}

	assert(evispixfmtRet == CVisPixFmtGetTPixel<TPixel>::PixFmt());

	return evispixfmtRet;

#else // VIS_OLD_CODE

	return CVisPixFmtGetTPixel<TPixel>::PixFmt();

#endif // VIS_OLD_CODE
}



// These virtual functions could be pure virtual functions.
// UNDONE (in Rel1):  Make them pure after changing the self-describing
// stream code to not require static exemplar objects.
inline CVisImageBase *CVisSequenceBase::PImageNew(void) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::PImageNew",
			__FILE__, __LINE__);
	return 0;
}

inline CVisImageBase *CVisSequenceBase::PImageNew(const CVisShape& refshape) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::PImageNew",
			__FILE__, __LINE__);
	return 0;
}

inline CVisImageBase *CVisSequenceBase::PImageNew(const CVisImageBase& refimage) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::PImageNew",
			__FILE__, __LINE__);
	return 0;
}

inline const type_info& CVisSequenceBase::PixelTypeInfo(void) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::PixelTypeInfo",
			__FILE__, __LINE__);
	return *((const type_info *) 0);
}

inline const type_info& CVisSequenceBase::ImageTypeInfo(void) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::ImageTypeInfo",
			__FILE__, __LINE__);
	return typeid(CVisImageBase);
}

inline CVisSequenceBase *CVisSequenceBase::Clone(bool fCopyThis) const
{
	assert(0);
	throw CVisError(
			"CVisSequenceBase virtual function should be implemented in derived class",
			eviserrorNotImplemented, "CVisSequenceBase::Clone",
			__FILE__, __LINE__);
	return 0;
}


inline void CVisSequenceBase::OnSequenceChanged(void)
{
	if (!m_IsDying)
	{
		T_Rgcallback::iterator iteratorCur = m_rgcallback.begin();
		T_Rgcallback::iterator iteratorLim = m_rgcallback.end();
		for (; iteratorCur != iteratorLim; ++iteratorCur)
		{
			assert(iteratorCur->m_pfnCallback != 0);
			if (iteratorCur->m_fCallOnChange)
			{
				((T_PfnCallback) (iteratorCur->m_pfnCallback))(*this,
						iteratorCur->m_pvUser);
			}
		}
	}
}


inline void CVisSequenceBase::SequenceIsDying(void)
{
	if (!m_HasDied)
	{
		m_IsDying = true;

		T_Rgcallback::iterator iteratorCur = m_rgcallback.begin();
		T_Rgcallback::iterator iteratorLim = m_rgcallback.end();
		for (; iteratorCur != iteratorLim; ++iteratorCur)
		{
			assert(iteratorCur->m_pfnCallback != 0);
			if (iteratorCur->m_fCallWhenDying)
			{
				((T_PfnCallback) (iteratorCur->m_pfnCallback))(*this,
						iteratorCur->m_pvUser);
			}
		}

		m_HasDied = true;
	}
}

#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common sequence types
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<BYTE> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<int> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<float> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<double> const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisSequence<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisSequence<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList<CVisSequence<TPixel> >(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisSequence<TPixel>::PropTypeInfoLookup(void)
		const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisSequence<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}



// Old self-describing stream I/O.
/*
CVisSDObject< CVisSequenceBase >&
	CVisSDObject< CVisSequenceBase >::Exemplar(void)
{
	// Global variable used with self-describing streams.
	// LATER:  Find a way to avoid using this global variable.
	extern CVisSDObject< CVisSequenceBase > g_vissequencebaseExemplar;

	return g_vissequencebaseExemplar;
}
*/
#ifdef VIS_INCLUDE_SDSTREAM

inline void CVisSequenceBase::ReadWriteImages(CVisSDStream& s)
{
    bool fRead = (s.Status() == CVisSDStream::Read);

	if (FEnterBlock(fRead))
	{
		try
		{
			if (fRead)
			{
				DeleteImages();
				while (s.PeekChar() != '}')
				{
					CVisImageBase *pimage = PImageNew();
					s << *pimage;
					m_deque.push_back(NewDequeEl(pimage));
					if (s.SomeChar(",}") == ',')
						s.Comma();
				}

				ImageAdded(m_deque.size());
			}
			else
			{
				int n = m_deque.size();
				int i = 0;
				T_Deque::iterator iteratorCur = m_deque.begin();
				T_Deque::iterator iteratorLim = m_deque.end();
				for ( ; iteratorCur != iteratorLim; ++iteratorCur, ++i)
				{
					if ((*iteratorCur).m_pImage == 0)
						continue;
					s << *(*iteratorCur).m_pImage;
					if (i < n-1)
						s.Comma();
					s.NewLine();
					if (s.Status() == CVisSDStream::CollectDescription)
						break;  // only need to check first one in list
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
			LeaveBlock(fRead);
			throw;
		}

		LeaveBlock(fRead);
	}
}


// This will call virtual functions to write the correct information
// for derived classes.
inline CVisSDStream& operator<<(CVisSDStream& s, CVisSequenceBase& refsequence)
	{ return VisSDReadWriteClass(s, refsequence); }
#endif

#ifdef _DEBUG
#undef new
#endif // _DEBUG


#endif // __VIS_CORE_IMAGEQUEUE_INL__


