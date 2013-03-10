// VisFrame.inl
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


template<class TPixel>
inline CVisFrame<TPixel>::CVisFrame(void)
  : CVisImage<TPixel>(),
	m_transformchain(),
	m_rectCrop(0, 0, 0, 0),
	m_fHasPixelInvisible(false)
{
}

template<class TPixel>
inline CVisFrame<TPixel>::CVisFrame(const RECT& refrect)
  : CVisImage<TPixel>(refrect),
	m_transformchain(),
	m_rectCrop(0, 0, 0, 0),
	m_fHasPixelInvisible(false)
{
}

template<class TPixel>
inline CVisFrame<TPixel>::CVisFrame(const CVisShape& refshape)
  : CVisImage<TPixel>(refshape),
	m_transformchain(),
	m_rectCrop(0, 0, 0, 0),
	m_fHasPixelInvisible(false)
{
}

template<class TPixel>
inline CVisFrame<TPixel>::CVisFrame(const char *szFilename)
  : CVisImage<TPixel>(),
	m_transformchain(),
	m_rectCrop(0, 0, 0, 0),
	m_fHasPixelInvisible(false)
{
	FReadFile(szFilename);
}


// Construct an image from another image.
// Warning:  Derived classes should provide their own copy constructor
// that first constructs a default object and then uses the assignment
// operator to synchronize  image creation, assignment, and
// destruction.  Derived classes should not call the copy constructor
// in the base class.
template<class TPixel>
inline CVisFrame<TPixel>::CVisFrame(const CVisImage<TPixel>& refimage)
  : CVisImage<TPixel>(),
	m_transformchain(),
	m_rectCrop(0, 0, 0, 0),
	m_fHasPixelInvisible(false)
{
	*this = refimage;
}


template<class TPixel>
inline CVisFrame<TPixel>& CVisFrame<TPixel>::operator=(
		const CVisImage<TPixel>& refimage)
{
	CVisImage<TPixel>::operator=(refimage);

	return *this;
}


template<class TPixel>
inline CVisFrame<TPixel>::~CVisFrame(void)
{
}


template<class TPixel>
inline const CVisTransformChain& CVisFrame<TPixel>::TransformChain(void) const
{
	return m_transformchain;
}

template<class TPixel>
inline CVisTransformChain& CVisFrame<TPixel>::TransformChain(void)
{
	return m_transformchain;
}

template<class TPixel>
inline void CVisFrame<TPixel>::SetTransformChain(
		const CVisTransformChain& reftransformchain)
{
	m_transformchain = reftransformchain;
}


template<class TPixel>
inline const CVisRect& CVisFrame<TPixel>::RectCrop(void) const
{
	return m_rectCrop;
}

template<class TPixel>
inline void CVisFrame<TPixel>::SetRectCrop(const RECT& refrectCrop)
{
	m_rectCrop = refrectCrop;
}


template<class TPixel>
inline const TPixel& CVisFrame<TPixel>::PixelInvisible(void) const
{
	return m_pixelInvisible;
}

template<class TPixel>
inline void CVisFrame<TPixel>::SetPixelInvisible(
		const TPixel& refpixelInvisible)
{
	m_pixelInvisible = refpixelInvisible;
}


template<class TPixel>
inline bool CVisFrame<TPixel>::HasPixelInvisible(void) const
{
	return m_fHasPixelInvisible;
}

template<class TPixel>
inline void CVisFrame<TPixel>::SetHasPixelInvisible(bool fHasPixelInvisible)
{
	m_fHasPixelInvisible = fHasPixelInvisible;
}



template<class TPixel>
inline const type_info& CVisFrame<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if (iLevel == 0)
		return typeid(CVisImageBase);
	else if  (iLevel == 1)
		return typeid(CVisImage<TPixel>);

	return typeid(CVisFrame);
}


template<class TPixel>
inline void CVisFrame<TPixel>::Alias(const CVisImageBase& refimage)
{
	CVisImage<TPixel>::Alias(refimage);
	CopyFrameInfo(refimage);
	// UNDONE:  This is not right, because we need to use CVisImage::Alias()
	// instead of CVisImage::operator= when copying base class information.
	// UNDONE:  Also note that we should synchronize all image Alias() and Copy()
	// methods, but we aren't currently doing that.
}


template<class TPixel>
inline void CVisFrame<TPixel>::Copy(const CVisImageBase& refimage)
{
	CVisImage<TPixel>::Copy(refimage);
	CopyFrameInfo(refimage);
}


template<class TPixel>
inline CVisImageBase *CVisFrame<TPixel>::Clone(const CVisShape *pshape) const
{
	if (pshape == 0)
		return new CVisFrame<TPixel>(*this);
	
	return new CVisFrame<TPixel>(*pshape);
}

template<class TPixel>
inline bool CVisFrame<TPixel>::ReadWriteFile(
		SVisFileDescriptor& reffiledescriptor, bool fWrite, bool fThrowError,
		bool fDisplayErrorMessage)
{
	bool fRet =  CVisImageBase::ReadWriteFile(reffiledescriptor, fWrite,
			fThrowError, fDisplayErrorMessage);

	return fRet;
}

template<class TPixel>
inline CVisFrame<TPixel> CVisFrame<TPixel>::Cropped(void)
{
	if ((IsValid()) && (m_rectCrop.Width() > 0) && (m_rectCrop.Height() > 0))
	{
		CVisRect rectCropped;
		
		rectCropped.left = ((m_rectCrop.left < Left())
				? Left() : m_rectCrop.left);
		rectCropped.top = ((m_rectCrop.top < Top())
				? Top() : m_rectCrop.top);
		rectCropped.right = ((m_rectCrop.right > Right())
				? Right() : m_rectCrop.right);
		rectCropped.bottom = ((m_rectCrop.bottom > Bottom())
				? Bottom() : m_rectCrop.bottom);

		if (rectCropped != Rect())
			return SubImage(rectCropped);
	}

	return *this;
}

template<class TPixel>
inline CVisFrame<TPixel> CVisFrame<TPixel>::Cropped(RECT& rect)
{
	if ((IsValid()) && (rect.right - rect.left > 0) && 
						(rect.bottom - rect.top > 0))
	{
		CVisRect rectCropped;
		
		rectCropped.left = ((rect.left < Left())
				? Left() : rect.left);
		rectCropped.top = ((rect.top < Top())
				? Top() : rect.top);
		rectCropped.right = ((rect.right > Right())
				? Right() : rect.right);
		rectCropped.bottom = ((rect.bottom > Bottom())
				? Bottom() : rect.bottom);

		if (rectCropped != Rect())
			return SubImage(rectCropped);
	}

	return *this;
}


template<class TPixel>
inline bool CVisFrame<TPixel>::operator==(const CVisFrame<TPixel>& refframe)
		const
{
	bool fRet = CVisImage<TPixel>::operator==(refframe);

	if (fRet)
	{
		fRet = ((TransformChain() == refframe.TransformChain())
				&& (RectCrop() == refframe.RectCrop())
				&& (HasPixelInvisible() == refframe.HasPixelInvisible()));

		if ((fRet) && (HasPixelInvisible()))
			fRet = (PixelInvisible() == refframe.PixelInvisible());
	}

	return fRet;
}

template<class TPixel>
inline bool CVisFrame<TPixel>::operator!=(const CVisFrame<TPixel>& refframe)
		const
{
	return !operator==(refframe);
}



template<class TPixel>
inline void CVisFrame<TPixel>::Assign(const CVisImageBase& refimage)
{
	CVisImage<TPixel>::Assign(refimage);
	CopyFrameInfo(refimage);
}


template<class TPixel>
inline void CVisFrame<TPixel>::CopyFrameInfo(const CVisImageBase& refimage)
{
	// LATER:  Use typeid(CVisFrame<TPixel>) to find the right prefix string?
	const char *szFrame = "class CVisFrame<";
	static int cchFrame = 0;
	if (cchFrame == 0)
		cchFrame = strlen(szFrame);

	// Verify that the szFrame string contains the prefix of this class name.
	assert(strnicmp(ObjectTypeInfo(2).name(), szFrame, cchFrame) == 0);

	// If refimage is a CVisFrame, get CVisFrame information.
	if ((ObjectTypeInfo(2) == refimage.ObjectTypeInfo(2))
			|| (strnicmp(refimage.ObjectTypeInfo(2).name(),
					szFrame, cchFrame) == 0))
	{
		// We assume that there is no pixel-specific information in the
		// CVisImage class.  If this assert is satisfied, the cast used to
		// find refframe should be safe, even if TPixel is not the pixel
		// type used in refimage.
		// (Even if we get this assert, the cast will still be okay unless
		// (ObjectTypeInfo(2) != refimage.ObjectTypeInfo(2)).)
		assert(sizeof(CVisImage<BYTE>)
				== sizeof(CVisImage<CVisRGBABytePixel>));

		const CVisFrame<TPixel>& refframe
				= (const CVisFrame<TPixel>&) refimage;

		SetTransformChain(refframe.TransformChain());
		SetRectCrop(refframe.RectCrop());
		SetHasPixelInvisible(refframe.HasPixelInvisible());
		if (refframe.HasPixelInvisible())
			SetPixelInvisible(refframe.PixelInvisible());
	}
}




template<class TPixel>
inline CVisFrameSequence<TPixel>::CVisFrameSequence(int nLength,
		int evissequence)
  : CVisSequence<TPixel>(0, evissequence),
	m_fHasPixelInvisible(false)
{
	Assign(nLength);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::CVisFrameSequence(int nLength,
		const T_Image& refimage,
		int evissequence)
  : CVisSequence<TPixel>(0, evissequence),
	m_fHasPixelInvisible(false)
{
	Assign(nLength, refimage);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::CVisFrameSequence(int nLength,
		const CVisShape& refshape,
		int evissequence)
  : CVisSequence<TPixel>(0, evissequence),
	m_fHasPixelInvisible(false)
{
	Assign(nLength, refshape);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::CVisFrameSequence(
		const CVisSequence<TPixel>& refsequence,
		int iFirst, int iLim)
  : CVisSequence<TPixel>(0, refsequence.SequenceOptions()),
	m_fHasPixelInvisible(false)
{
	assert(PixFmt() == refsequence.PixFmt());

	Assign(refsequence, iFirst, iLim);

	CopyPImageSource(&(refsequence.ImageSource()));

	CopyFrameSequenceInfo(refsequence);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::CVisFrameSequence(
		const CVisSequence<TPixel>& refsequence,
		int iFirst, int iLim, int evissequence)
  : CVisSequence<TPixel>(0, evissequence),
	m_fHasPixelInvisible(false)
{
	assert(PixFmt() == refsequence.PixFmt());

	Assign(refsequence, iFirst, iLim);

	CopyPImageSource(&(refsequence.ImageSource()));

	CopyFrameSequenceInfo(refsequence);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>& CVisFrameSequence<TPixel>::operator=(
		const CVisSequence<TPixel>& refsequence)
{
	CVisSequence<TPixel>::operator=(refsequence);

	CopyFrameSequenceInfo(refsequence);

	return *this;
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::~CVisFrameSequence(void)
{
}


template<class TPixel>
inline const TPixel& CVisFrameSequence<TPixel>::PixelInvisible(void) const
{
	return m_pixelInvisible;
}

template<class TPixel>
inline void CVisFrameSequence<TPixel>::SetPixelInvisible(
		const TPixel& pixelInvisible)
{
	m_pixelInvisible = pixelInvisible;
}


template<class TPixel>
inline bool CVisFrameSequence<TPixel>::HasPixelInvisible(void) const
{
	return m_fHasPixelInvisible;
}

template<class TPixel>
inline void CVisFrameSequence<TPixel>::SetHasPixelInvisible(
		bool fHasPixelInvisible)
{
	m_fHasPixelInvisible = fHasPixelInvisible;
}


template<class TPixel>
inline const CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::Front(void) const
{
	return (const T_Frame&) CVisSequenceBase::Front();
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::Front(void)
{
	return (T_Frame&) CVisSequenceBase::Front();
}

template<class TPixel>
inline const CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::Back(void) const
{
	return (const T_Frame&) CVisSequenceBase::Back();
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::Back(void)
{
	return (T_Frame&) CVisSequenceBase::Back();
}

template<class TPixel>
inline const CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::At(int i) const
{
	return (const T_Frame&) CVisSequenceBase::At(i);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::At(int i)
{
	return (T_Frame&) CVisSequenceBase::At(i);
}

template<class TPixel>
inline const CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::operator[](int i) const
{
	return (const T_Frame&) CVisSequenceBase::At(i);
}

template<class TPixel>
inline CVisFrameSequence<TPixel>::T_Frame&
	CVisFrameSequence<TPixel>::operator[](int i)
{
	return (T_Frame&) CVisSequenceBase::At(i);
}


template<class TPixel>
inline const type_info& CVisFrameSequence<TPixel>::ImageTypeInfo(void) const
{
	return typeid(CVisFrame<TPixel>);
}

template<class TPixel>
inline const type_info& CVisFrameSequence<TPixel>::ObjectTypeInfo(int iLevel)
		const
{
	if (iLevel == 0)
		return typeid(CVisSequenceBase);
	else if (iLevel == 1)
		return typeid(CVisSequence<TPixel>);
	
	return typeid(CVisFrameSequence<TPixel>);
}

template<class TPixel>
inline CVisSequenceBase *CVisFrameSequence<TPixel>::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisFrameSequence<TPixel>(*this);

	return new CVisFrameSequence<TPixel>;
}


template<class TPixel>
inline CVisImageBase *CVisFrameSequence<TPixel>::PImageNew(void) const
{
	T_Frame *pframe = new T_Frame();
	if (HasPixelInvisible())
		pframe->SetPixelInvisible(PixelInvisible());
	return pframe;
}

template<class TPixel>
inline CVisImageBase *CVisFrameSequence<TPixel>::PImageNew(
		const CVisShape& refshape) const
{
	T_Frame *pframe = new T_Frame(refshape);
	if (HasPixelInvisible())
		pframe->SetPixelInvisible(PixelInvisible());
	return pframe;
}

template<class TPixel>
inline CVisImageBase *CVisFrameSequence<TPixel>::PImageNew(
		const CVisImageBase& refimage) const
{
	assert(refimage.PixelTypeInfo() == PixelTypeInfo());
	T_Frame *pframe = new T_Frame();
	if (HasPixelInvisible())
		pframe->SetPixelInvisible(PixelInvisible());
	*pframe = (const T_Image&) refimage;
	return pframe;
}



template<class TPixel>
inline void CVisFrameSequence<TPixel>::CopyFrameSequenceInfo(
		const CVisSequence<TPixel>& refsequence)
{
	// If refsequence is a sequence of CVisFrames, we should copy the
	// invisible pixel information.
	if (refsequence.ObjectTypeInfo(eilevelFrameSequence) == typeid(CVisFrameSequence<TPixel>))
	{
		const CVisFrameSequence<TPixel>& refframesequence
				= (const CVisFrameSequence<TPixel>&) refsequence;

		SetHasPixelInvisible(refframesequence.HasPixelInvisible());
		if (refframesequence.HasPixelInvisible())
			SetPixelInvisible(refframesequence.PixelInvisible());
	}
}



template <class TPixel>
inline void CVisFrame<TPixel>::ReadDescription(FILE *stream, const char *desc_type,
                                   char *working_dir)
{                     
    char szFilename[1024];
    fgets(szFilename, 1024, stream);
    szFilename[strlen(szFilename)-1] = 0;     // remove trailing \n
    // Optionally add collection directory as prefix
    if (working_dir && working_dir[0] && strlen(szFilename) > 1 && 
        szFilename[1] != ':' && szFilename[0] != '\\') {
        char newname[2048];
        strcpy(newname, working_dir);
        strcat(newname, szFilename);
        strcpy(szFilename, newname);
    }
    SetName(szFilename);
    if (strcmp(desc_type, "filename_only") != 0) {
        m_transformchain.Read(stream);
        if (strcmp(desc_type, "filename_geometry_interlace") == 0) {
            // backwards compatibility
            fscanf(stream, "%s", szFilename);
        }
        fgets(szFilename, 1024, stream); // read rest of line
    }
    if (strcmp(desc_type, "filename_geometry_cropped") == 0) {
        int srow, scol, nrows, ncols;
        fscanf(stream, "%d %d %d %d\n", &srow, &scol, &nrows, &ncols);
        m_rectCrop = CVisRect(scol, srow, scol+ncols, srow+nrows);
    }
	if (strcmp(desc_type, "filename_geometry_track_cropped") == 0) {
		int srow, scol, nrows, ncols;
        fscanf(stream, "%d %d %d %d\n", &srow, &scol, &nrows, &ncols);
        m_rectCrop = CVisRect(scol, srow, scol+ncols, srow+nrows);
    }
    if (strcmp(desc_type, "filename_geometry_shape") == 0) {
        int srow, scol, nrows, ncols;
        fscanf(stream, "%d %d %d %d\n", &srow, &scol, &nrows, &ncols);
        if (Height() == 0)
            ReadFile();
        ResetOrigin(scol, srow);
    }
	if (strcmp(desc_type, "filename_layered_stereo_shape") == 0) {
        int srow, scol, nrows, ncols;
        fscanf(stream, "%d %d %d %d\n", &srow, &scol, &nrows, &ncols);
        CVisRect r(scol, srow, scol+ncols, srow+nrows);
        if (r.Width() > 0 && r.Height() > 0)
		    Allocate(CVisRect(scol, srow, scol+ncols, srow+nrows));
		ClearPixels();
	}
}


template <class TPixel>
inline void CVisFrame<TPixel>::WriteDescription(FILE *stream, const char *desc_type,
                                    char *working_dir)
{
    if (working_dir && working_dir[0] &&
        strncmp(working_dir, Name(true), strlen(working_dir)) == 0)
        fprintf(stream, "%s\n", Name(true)+strlen(working_dir));
    else
        fprintf(stream, "%s\n", Name(true));
    if (strcmp(desc_type, "filename_only") != 0) {
        m_transformchain.Write(stream, 1);
        fprintf(stream, "\n");
    }
    if (strcmp(desc_type, "filename_geometry_cropped") == 0) {
        CVisRect r = m_rectCrop;
        fprintf(stream, "%d %d %d %d\n",
                r.top, r.left, r.Height(), r.Width());
    }
	//KE /////////////////////////////////////////
	if (strcmp(desc_type, "filename_geometry_track_cropped") == 0) {
        CVisRect r = m_rectCrop;
        fprintf(stream, "%d %d %d %d\n",
                r.top, r.left, r.Height(), r.Width());
    }
    if (strcmp(desc_type, "filename_geometry_shape") == 0) {
        CVisRect r = Shape();
        fprintf(stream, "%d %d %d %d\n",
                r.top, r.left, r.Height(), r.Width());
    }
    if (strcmp(desc_type, "filename_layered_stereo_shape") == 0) {
        CVisRect r = Shape();
        fprintf(stream, "%d %d %d %d\n",
                r.top, r.left, r.Height(), r.Width());
    }
}



// Self-describing stream I/O

template <class TPixel>
inline void CVisFrame<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
#ifdef VIS_INCLUDE_SDSTREAM
	CVisImageBase::SDReadWriteProperties(s, true);

	s << m_transformchain;
	s.Comma();
	s.Comment("Transform Chain");

	s << m_rectCrop;
	s.Comma();
	s.Comment("Cropping rect");

	s.OpenParen();
	s << m_fHasPixelInvisible;
	if (m_fHasPixelInvisible)
	{
		s.Comma();
		s << (TPixel&) m_pixelInvisible;
	}
	s.CloseParen();
	if (fAddComma)
		s.Comma();
	s.Comment("Invisible pixel information");
#else
	assert(false); // SDStreams not included
#endif
}


template <class TPixel>
inline void CVisFrameSequence<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
#ifdef VIS_INCLUDE_SDSTREAM
	CVisSequenceBase::SDReadWriteProperties(s, true);

	s.OpenParen();
	s << m_fHasPixelInvisible;
	if (m_fHasPixelInvisible)
	{
		s.Comma();
		s << (TPixel&) m_pixelInvisible;
	}
	s.CloseParen();
	if (fAddComma)
		s.Comma();
	s.Comment("Invisible pixel information");
#else
	assert(false); // SDStreams not included
#endif
}





#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisFrame<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList<CVisFrame<TPixel> >(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisFrame<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisFrame<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


template <class TPixel>
inline void CVisFrame<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	T_Image::BuildPropList(refproplist);

	refproplist.AddPropReference("m_rectCrop", m_rectCrop, false);
	refproplist.AddPropReference("m_fHasPixelInvisible", m_fHasPixelInvisible,
			false);
	refproplist.AddPropReference("m_pixelInvisible", m_pixelInvisible, false);
	refproplist.AddPropReference("m_transformchain", m_transformchain, false);
}



#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisFrameSequence<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList<CVisFrameSequence<TPixel> >(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisFrameSequence<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisFrameSequence<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


template <class TPixel>
inline void CVisFrameSequence<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	T_BaseSequence::BuildPropList(refproplist);

	// UNDONE:  Do we want pre / post read / write methods to change these values in
	// the images?
	refproplist.AddPropReference("m_fHasPixelInvisible", m_fHasPixelInvisible,
			false);
	refproplist.AddPropReference("m_pixelInvisible", m_pixelInvisible, false);
}



template <class TPixel>
inline void CVisFrameSequence<TPixel>::PostReadValue(CVisSDIStream& refsdistream)
{
	T_BaseSequence::PostReadValue(refsdistream);

	bool fDelayRead = ((SequenceOptions() & evissequenceMaskDelayRead)
			== evissequenceDelayReadUntilNeeded);

	if (fDelayRead)
	{
		SetSequenceOptions((SequenceOptions() & ~evissequenceMaskDelayRead)
				| evissequenceDontDelayRead);
	}

	if (HasPixelInvisible())
	{
		for (int i = 0; i < Length(); ++i)
		{
			At(i).SetPixelInvisible(PixelInvisible());
			At(i).SetHasPixelInvisible(true);
		}
	}
	else
	{
		for (int i = 0; i < Length(); ++i)
		{
			At(i).SetHasPixelInvisible(false);
		}
	}

	if (fDelayRead)
	{
		SetSequenceOptions((SequenceOptions() & ~evissequenceMaskDelayRead)
				| evissequenceDelayReadUntilNeeded);
	}
}

template <class TPixel>
inline void CVisFrameSequence<TPixel>::PreWriteValue(CVisSDOStream& refsdostream) const
{
	T_BaseSequence::PreWriteValue(refsdostream);

	CVisFrameSequence<TPixel> *pframesequenceThisNonConst
			= const_cast<CVisFrameSequence<TPixel> *>(this);

	if (HasPixelInvisible())
	{
		for (int i = 0; i < Length(); ++i)
		{
			pframesequenceThisNonConst->At(i).SetPixelInvisible(PixelInvisible());
			pframesequenceThisNonConst->At(i).SetHasPixelInvisible(true);
		}
	}
	else
	{
		for (int i = 0; i < Length(); ++i)
		{
			pframesequenceThisNonConst->At(i).SetHasPixelInvisible(false);
		}
	}
}

#ifdef _DEBUG
#undef new
#endif // _DEBUG
