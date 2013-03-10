// VisComposite.inl
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


//
// Composite Frame
//
template<class TPixel>
CVisCompositeFrame<TPixel>::CVisCompositeFrame(void)
  : CVisFrame<TPixel>(),
	m_imageFeather()
{
}

template<class TPixel>
CVisCompositeFrame<TPixel>::CVisCompositeFrame(const RECT& refrect)
  : CVisFrame<TPixel>(refrect),
	m_imageFeather(refrect)
{
	// UNDONE:  Should feathered image be allocated
	// and initialized here?
}

template<class TPixel>
CVisCompositeFrame<TPixel>::CVisCompositeFrame(const CVisShape& refshape)
  : CVisFrame<TPixel>(refshape),
	m_imageFeather(refshape)
{
	// UNDONE:  Should feathered image be allocated
	// and initialized here?
}

template<class TPixel>
CVisCompositeFrame<TPixel>::CVisCompositeFrame(const char *szFilename)
  : CVisFrame<TPixel>(),
	m_imageFeather()
{
	FReadFile(szFilename);
	// UNDONE:  Should feathered image be allocated
	// and initialized here?
}

template<class TPixel>
CVisCompositeFrame<TPixel>::CVisCompositeFrame(
		const CVisImage<TPixel>& refimage)
  : CVisFrame<TPixel>(),
	m_imageFeather()
{
	(*this) = refimage;
}

template<class TPixel>
CVisCompositeFrame<TPixel>& CVisCompositeFrame<TPixel>::operator=(
		const CVisImage<TPixel>& refimage)
{
	// The CVisImage operator= method with synchronize assignment
	// and call the virtual Assign method to do the assignment.
	CVisImage<TPixel>::operator=(refimage);

	return *this;
}

template<class TPixel>
CVisCompositeFrame<TPixel>::~CVisCompositeFrame(void)
{
}


template<class TPixel>
CVisFloatImage& CVisCompositeFrame<TPixel>::ImageFeather(void)
{
	if ((IsValid()) && (!m_imageFeather.IsValid()))
	{
		// UNDONE:  Construct a feather image?
//		assert(0);
	}

	return m_imageFeather;
}

template<class TPixel>
CVisCompositeFrame<TPixel> CVisCompositeFrame<TPixel>::Cropped(void)
{
	CVisCompositeFrame<TPixel> compframeRet = CVisFrame<TPixel>::Cropped();

	if (compframeRet.m_imageFeather.IsValid())
		compframeRet.m_imageFeather.SetRect(RectCrop());

	return compframeRet;
}

template<class TPixel>
CVisCompositeFrame<TPixel> CVisCompositeFrame<TPixel>::Cropped(RECT& rect)
{
	CVisCompositeFrame<TPixel> compframeRet = CVisFrame<TPixel>::Cropped(rect);

	if (compframeRet.m_imageFeather.IsValid())
		compframeRet.m_imageFeather.SetRect(rect);

	return compframeRet;
}


template<class TPixel>
void CVisCompositeFrame<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// Just call the base class.  (We don't need to write feathered image info.)
	CVisFrame<TPixel>::SDReadWriteProperties(s, fAddComma);
}

template<class TPixel>
const type_info& CVisCompositeFrame<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelCompositeFrame))
		return typeid(CVisCompositeFrame<TPixel>);

	return CVisFrame<TPixel>::ObjectTypeInfo(iLevel);
}

template<class TPixel>
void CVisCompositeFrame<TPixel>::Alias(const CVisImageBase& refimage)
{
	CVisFrame<TPixel>::Alias(refimage);
	
	// Also copy feathered image.
	if ((ObjectTypeInfo(eilevelCompositeFrame)
					== refimage.ObjectTypeInfo(eilevelCompositeFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageFeather.Alias(
				((const CVisCompositeFrame<TPixel>&) refimage).m_imageFeather);
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageFeather.Deallocate();
	}
}

template<class TPixel>
void CVisCompositeFrame<TPixel>::Copy(const CVisImageBase& refimage)
{
	CVisFrame<TPixel>::Copy(refimage);
	
	// Also copy feathered image.
	if ((ObjectTypeInfo(eilevelCompositeFrame)
					== refimage.ObjectTypeInfo(eilevelCompositeFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageFeather.Copy(
				((const CVisCompositeFrame<TPixel>&) refimage).m_imageFeather);
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageFeather.Deallocate();
	}
}

template<class TPixel>
CVisImageBase *CVisCompositeFrame<TPixel>::Clone(const CVisShape *pshape) const
{
	if (pshape == 0)
		return new CVisCompositeFrame<TPixel>(*this);
	
	return new CVisCompositeFrame<TPixel>(*pshape);
}


template<class TPixel>
void CVisCompositeFrame<TPixel>::Assign(const CVisImageBase& refimage)
{
	CVisFrame<TPixel>::Assign(refimage);
	
	// Also copy feathered image.
	if ((ObjectTypeInfo(eilevelCompositeFrame)
					== refimage.ObjectTypeInfo(eilevelCompositeFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageFeather
				= ((const CVisCompositeFrame<TPixel>&) refimage).m_imageFeather;
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageFeather.Deallocate();
	}
}

template<class TPixel>
bool CVisCompositeFrame<TPixel>::ReadWriteFile(
		SVisFileDescriptor& reffiledescriptor,
		bool fWrite, bool fThrowError, bool fDisplayErrorMessage)
{
	bool fRet =  CVisImageBase::ReadWriteFile(reffiledescriptor, fWrite,
			fThrowError, fDisplayErrorMessage);

	// UNDONE:  Should the feathered image be changed?

	return fRet;
}





#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisCompositeFrame<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisCompositeFrame<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisCompositeFrame<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}



#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisComposite<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisComposite<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisComposite<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG

