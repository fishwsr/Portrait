// VisCylindrical.inl
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
// Warped Frame
//
template<class TPixel>
CVisWarpedFrame<TPixel>::CVisWarpedFrame(void)
  : CVisCompositeFrame<TPixel>(),
	m_imageLocalWarp(),
	m_imageWarped()
{
}

template<class TPixel>
CVisWarpedFrame<TPixel>::CVisWarpedFrame(const RECT& refrect)
  : CVisCompositeFrame<TPixel>(refrect),
	m_imageLocalWarp(refrect),
	m_imageWarped(refrect)
{
	// UNDONE:  Should local warp and warped images be allocated
	// and initialized here?
}

template<class TPixel>
CVisWarpedFrame<TPixel>::CVisWarpedFrame(const CVisShape& refshape)
  : CVisCompositeFrame<TPixel>(refshape),
	m_imageLocalWarp(refshape),
	m_imageWarped(refshape)
{
	// UNDONE:  Should local warp and warped images be allocated
	// and initialized here?
}

template<class TPixel>
CVisWarpedFrame<TPixel>::CVisWarpedFrame(const char *szFilename)
  : CVisCompositeFrame<TPixel>(),
	m_imageLocalWarp(),
	m_imageWarped()
{
	FReadFile(szFilename);
}

template<class TPixel>
CVisWarpedFrame<TPixel>::CVisWarpedFrame(
		const CVisImage<TPixel>& refimage)
  : CVisCompositeFrame<TPixel>(),
	m_imageLocalWarp(),
	m_imageWarped()
{
	(*this) = refimage;
	// UNDONE:  Should local warp and warped images be allocated
	// and initialized here?
}

template<class TPixel>
CVisWarpedFrame<TPixel>& CVisWarpedFrame<TPixel>::operator=(
		const CVisImage<TPixel>& refimage)
{
	// The CVisImage operator= method with synchronize assignment
	// and call the virtual Assign method to do the assignment.
	CVisImage<TPixel>::operator=(refimage);

	return *this;
}

template<class TPixel>
CVisWarpedFrame<TPixel>::~CVisWarpedFrame(void)
{
}


template<class TPixel>
const CVisFloatImage& CVisWarpedFrame<TPixel>::ImageLocalWarp(void) const
{
	if ((IsValid()) && (!m_imageLocalWarp.IsValid()))
	{
		// UNDONE:  Construct a local warp image?
//		assert(0);
	}

	return m_imageLocalWarp;
}

template<class TPixel>
CVisFloatImage& CVisWarpedFrame<TPixel>::ImageLocalWarp(void)
{
	if ((IsValid()) && (!m_imageLocalWarp.IsValid()))
	{
		// UNDONE:  Construct a local warp image?
//		assert(0);
	}

	return m_imageLocalWarp;
}

template<class TPixel>
SColorAdjustParam& CVisWarpedFrame<TPixel>::ColorAdjust(TVisColorDir dir)
{
	return m_ColorAdjust[dir];
}

template<class TPixel>
void CVisWarpedFrame<TPixel>::ClearColorAdjust()
{
	m_ColorAdjust[VIS_COLOR_LEFT].m_Overlap = CVisShape(0,0,0,0);
	m_ColorAdjust[VIS_COLOR_RIGHT].m_Overlap = CVisShape(0,0,0,0);
}

template<class TPixel>
const CVisWarpedFrame<TPixel>::T_Image& CVisWarpedFrame<TPixel>::ImageWarped(void) const
{
	if ((IsValid()) && (!m_imageWarped.IsValid()))
	{
		// UNDONE:  Construct a warped image?
//		assert(0);
	}

	return m_imageWarped;
}

template<class TPixel>
CVisWarpedFrame<TPixel>::T_Image& CVisWarpedFrame<TPixel>::ImageWarped(void)
{
	if ((IsValid()) && (!m_imageWarped.IsValid()))
	{
		// UNDONE:  Construct a warped image?
//		assert(0);
	}

	return m_imageWarped;
}

template<class TPixel>
CVisWarpedFrame<TPixel> CVisWarpedFrame<TPixel>::Cropped(void)
{
	CVisWarpedFrame<TPixel> compframeRet = CVisCompositeFrame<TPixel>::Cropped();

	if (compframeRet.m_imageLocalWarp.IsValid())
		compframeRet.m_imageLocalWarp.SetRect(RectCrop());

	if (compframeRet.m_imageWarped.IsValid())
		compframeRet.m_imageWarped.SetRect(RectCrop());

	return compframeRet;
}

template<class TPixel>
CVisWarpedFrame<TPixel> CVisWarpedFrame<TPixel>::Cropped(RECT& rect)
{
	CVisWarpedFrame<TPixel> compframeRet = CVisCompositeFrame<TPixel>::Cropped(rect);

	if (compframeRet.m_imageLocalWarp.IsValid())
		compframeRet.m_imageLocalWarp.SetRect(rect);

	if (compframeRet.m_imageWarped.IsValid())
		compframeRet.m_imageWarped.SetRect(rect);

	return compframeRet;
}


template<class TPixel>
void CVisWarpedFrame<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// Just call the base class for now.
	// UNDONE:  Do we need to write local warp or warped images?
	CVisCompositeFrame<TPixel>::SDReadWriteProperties(s, fAddComma);
}

template<class TPixel>
const type_info& CVisWarpedFrame<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelWarpedFrame))
		return typeid(CVisWarpedFrame<TPixel>);

	return CVisCompositeFrame<TPixel>::ObjectTypeInfo(iLevel);
}

template<class TPixel>
void CVisWarpedFrame<TPixel>::Alias(const CVisImageBase& refimage)
{
	CVisCompositeFrame<TPixel>::Alias(refimage);
	
	// Also copy local warp image.
	if ((ObjectTypeInfo(eilevelWarpedFrame)
					== refimage.ObjectTypeInfo(eilevelWarpedFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageLocalWarp.Alias(
				((const CVisWarpedFrame<TPixel>&) refimage).m_imageLocalWarp);
		m_imageWarped.Alias(
				((const CVisWarpedFrame<TPixel>&) refimage).m_imageWarped);
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageLocalWarp.Deallocate();
		m_imageWarped.Deallocate();
	}
}

template<class TPixel>
void CVisWarpedFrame<TPixel>::Copy(const CVisImageBase& refimage)
{
	CVisCompositeFrame<TPixel>::Copy(refimage);
	
	// Also copy local warp image.
	if ((ObjectTypeInfo(eilevelWarpedFrame)
					== refimage.ObjectTypeInfo(eilevelWarpedFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageLocalWarp.Copy(
				((const CVisWarpedFrame<TPixel>&) refimage).m_imageLocalWarp);
		m_imageWarped.Copy(
				((const CVisWarpedFrame<TPixel>&) refimage).m_imageWarped);
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageLocalWarp.Deallocate();
		m_imageWarped.Deallocate();
	}
}

template<class TPixel>
CVisImageBase *CVisWarpedFrame<TPixel>::Clone(const CVisShape *pshape) const
{
	if (pshape == 0)
		return new CVisWarpedFrame<TPixel>(*this);
	
	return new CVisWarpedFrame<TPixel>(*pshape);
}


template<class TPixel>
void CVisWarpedFrame<TPixel>::Assign(const CVisImageBase& refimage)
{
	CVisCompositeFrame<TPixel>::Assign(refimage);
	
	// Also copy local warp image.
	if ((ObjectTypeInfo(eilevelWarpedFrame)
					== refimage.ObjectTypeInfo(eilevelWarpedFrame))
			&& (NBands() == refimage.NBands()))
	{
		m_imageLocalWarp
				= ((const CVisWarpedFrame<TPixel>&) refimage).m_imageLocalWarp;
		m_imageWarped
				= ((const CVisWarpedFrame<TPixel>&) refimage).m_imageWarped;
	}
	else
	{
		// LATER:  Add code to do the right thing with multi-band images?
		m_imageLocalWarp.Deallocate();
		m_imageWarped.Deallocate();
	}
}

template<class TPixel>
bool CVisWarpedFrame<TPixel>::ReadWriteFile(
		SVisFileDescriptor& reffiledescriptor,
		bool fWrite, bool fThrowError, bool fDisplayErrorMessage)
{
	bool fRet =  CVisImageBase::ReadWriteFile(reffiledescriptor, fWrite,
			fThrowError, fDisplayErrorMessage);

	// UNDONE:  Should the local warp image be changed?

	return fRet;
}




#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisWarpedFrame<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisWarpedFrame<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisWarpedFrame<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}



#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisWarpedComposite<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisWarpedComposite<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisWarpedComposite<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
