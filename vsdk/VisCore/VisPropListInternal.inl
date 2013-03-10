// VisPropListInternal.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



inline CVisPropKeyData::~CVisPropKeyData(void)
{
	ReleaseKey();
}

inline const char *CVisPropKeyData::Name(void) const
{
	assert(m_pchBuf != 0);
	return m_pchBuf + sizeof(ULONG);
}

inline ULONG& CVisPropKeyData::CRefKey(void)
{
	assert(m_pchBuf != 0);
	return *((ULONG *) m_pchBuf);
}



inline CVisPropTypeInfoBase::~CVisPropTypeInfoBase(void)
{
}

inline const char *CVisPropTypeInfoBase::Name(void) const
{
	return m_strName.c_str();
}

inline bool CVisPropTypeInfoBase::ArePropsPrinted(void) const
{
	return m_fPrint;
}

inline bool CVisPropTypeInfoBase::CanPropsPrint(void) const
{
	return m_fCanPrint;
}

inline void CVisPropTypeInfoBase::SetPropPrinting(bool f)
{
	if (m_fCanPrint)
		m_fPrint = f;
}

inline bool CVisPropTypeInfoBase::IsSimpleType(void) const
{
	return m_fSimpleType;
}

inline void CVisPropTypeInfoBase::SetIsSimpleType(bool f)
{
	m_fSimpleType = f;
}

inline bool CVisPropTypeInfoBase::ChangedName(void) const
{
	return m_fChangedName;
}

inline bool CVisPropTypeInfoBase::CanCopyObjectBytes(void) const
{
	return m_fCanCopyObjectBytes;
}

inline void CVisPropTypeInfoBase::SetCanCopyObjectBytes(bool f)
{
	// Don't allow a change from false to true.
	assert((m_fCanCopyObjectBytes) || (!f));
	if ((m_fCanCopyObjectBytes) || (!f))
		m_fCanCopyObjectBytes = f;
}

inline void CVisPropTypeInfoBase::WriteObj(CVisSDOStream& refsdostream,
		const void *pvObj, const char *szName, bool fOnlyValue) const
{
	// The base class implements the version used with types that don't
	// support SDStream I/O.
}

inline void CVisPropTypeInfoBase::WriteObjArray(CVisSDOStream& refsdostream,
		const CVisDim& refdim, const void *pvObj, const char *szName,
		bool fOnlyValue) const
{
	// The base class implements the version used with types that don't
	// support SDStream I/O.
}

inline void CVisPropTypeInfoBase::ReadObj(CVisSDIStream& refsdistream,
		void *pvObj) const
{
	// The base class implements the version used with types that don't
	// support SDStream I/O.
}

inline void CVisPropTypeInfoBase::ReadObjArray(CVisSDIStream& refsdistream,
		const CVisDim& refdim, void *pvObj) const
{
	// The base class implements the version used with types that don't
	// support SDStream I/O.
}


inline bool CVisPropTypeInfoBase::AlwaysUseTypedef(void) const
{
	return false;
}

inline void CVisPropTypeInfoBase::SetAlwaysUseTypedef(bool f)
{
	// This should not be called (since there is no CVisPropList with
	// typedef information).
	assert(0);
}

inline CVisPropList *CVisPropTypeInfoBase::PPropList(const void *pvObj)
{
	return 0;
}


inline void CVisPropTypeInfoBase::SDIFindTypes(CVisSDIStream& refsdistream)
{
}

inline void CVisPropTypeInfoBase::SDOFindTypes(CVisSDOStream& refsdostream,
		const void *pvObj)
{
}

inline void CVisPropTypeInfoBase::SDOFindTypes(CVisSDOStream& refsdostream,
		const void *pvObj, const CVisDim& refdim)
{
}

inline void CVisPropTypeInfoBase::SetFContainsPropLists(bool f)
{
	m_fContainsPropLists = f;
}



template<class TObject>
inline CVisPropTypeInfoNoIO<TObject>::CVisPropTypeInfoNoIO(const char *szName)
  : CVisPropTypeInfoBase(((szName != 0) ? szName : typeid(TObject).name()),
		(sizeof(TObject) <= sizeof(double)), false)
{
}

template<class TObject>
inline CVisPropTypeInfoNoIO<TObject>::CVisPropTypeInfoNoIO(bool fCanPrint,
		const char *szName)
  : CVisPropTypeInfoBase(((szName != 0) ? szName : typeid(TObject).name()),
		(sizeof(TObject) <= sizeof(double)), fCanPrint)
{
}

template<class TObject>
inline const type_info& CVisPropTypeInfoNoIO<TObject>::TypeInfo(void) const
{
	return typeid(TObject);
}

template<class TObject>
inline void CVisPropTypeInfoNoIO<TObject>::AssignObjToObj(const void *pvObjSrc,
		void *pvObjDest) const
{
	assert(pvObjSrc != 0);
	assert(pvObjDest != 0);
	*((TObject *) pvObjDest) = *((const TObject *) pvObjSrc);
}

template<class TObject>
inline void *CVisPropTypeInfoNoIO<TObject>::ClonePvRefCntObj(const void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	const CVisRefCntObj<TObject> *prefcntobj = (const CVisRefCntObj<TObject> *) pvRefCntObj;
	return (void *) new CVisRefCntObj<TObject>(prefcntobj->Obj());
}

template<class TObject>
inline ULONG CVisPropTypeInfoNoIO<TObject>::AddRefPvRefCntObj(void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	return ((CVisRefCntObj<TObject> *) pvRefCntObj)->AddRef();
}

template<class TObject>
inline ULONG CVisPropTypeInfoNoIO<TObject>::ReleasePvRefCntObj(void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	return ((CVisRefCntObj<TObject> *) pvRefCntObj)->Release();
}

template<class TObject>
inline void *CVisPropTypeInfoNoIO<TObject>::PvRefCntObjMakePvObj(const void *pvObj) const
{
	if (pvObj == 0)
		return (void *) new CVisRefCntObj<TObject>;
	return (void *) new CVisRefCntObj<TObject>(*((const TObject *) pvObj));
}

template<class TObject>
inline void *CVisPropTypeInfoNoIO<TObject>::ClonePvRefCntArray(
		 const void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	return ((CVisRefCntObjArray<TObject> *) pvRefCntObj)->Clone();
}

template<class TObject>
inline ULONG CVisPropTypeInfoNoIO<TObject>::AddRefPvRefCntArray(
		 void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	return ((CVisRefCntObjArray<TObject> *) pvRefCntObj)->AddRef();
}

template<class TObject>
inline ULONG CVisPropTypeInfoNoIO<TObject>::ReleasePvRefCntArray(
		 void *pvRefCntObj) const
{
	assert(pvRefCntObj != 0);
	return ((CVisRefCntObjArray<TObject> *) pvRefCntObj)->Release();
}

template<class TObject>
inline CVisRefCntObjArrayBase *CVisPropTypeInfoNoIO<TObject>::PRefCntArrayMake(
		 const CVisDim& refdim) const
{
	return new CVisRefCntObjArray<TObject>(refdim);
}

template<class TObject>
inline CVisRefCntObjArrayBase *CVisPropTypeInfoNoIO<TObject>::PRefCntArrayMake(
		 const CVisDim& refdim, void *pvWhere) const
{
	return new CVisRefCntObjArray<TObject>(refdim, pvWhere);
}



template<class TObject>
inline CVisPropTypeInfoWO<TObject>::CVisPropTypeInfoWO(
		const char *szName)
  : CVisPropTypeInfoNoIO<TObject>(true, szName)
{
}

template<class TObject>
inline CVisPropTypeInfoWO<TObject>::CVisPropTypeInfoWO(
		bool fSimpleType, const char *szName)
  : CVisPropTypeInfoNoIO<TObject>(true, szName)
{
	SetIsSimpleType(fSimpleType);
}

template<class TObject>
inline CVisPropTypeInfoWO<TObject>::~CVisPropTypeInfoWO(void)
{
}

template<class TObject>
inline void CVisPropTypeInfoWO<TObject>::WriteObj(CVisSDOStream& refsdostream,
		const void *pvObj, const char *szName, bool fOnlyValue) const
{
	TObject const & refobj = *((TObject const *) pvObj);

#ifndef VIS_NO_SD_STREAM
	VisSDOFindTypes(refsdostream, refobj);

	if (fOnlyValue)
		VisSDWriteValue(refsdostream, refobj);
	else
		VisSDWriteObj(refsdostream, refobj, szName);
#endif // VIS_NO_SD_STREAM
}

template<class TObject>
inline void CVisPropTypeInfoWO<TObject>::WriteObjArray(CVisSDOStream& refsdostream,
		const CVisDim& refdim, const void *pvObj, const char *szName,
		bool fOnlyValue) const
{
	// LATER:  Should this just call VisSDWriteValue or VisSDWriteObj?
	TObject const *pobjCur = ((TObject const *) pvObj);

#ifndef VIS_NO_SD_STREAM
	VisSDOFindTypes(refsdostream, *pobjCur);

	if (!fOnlyValue)
	{
		// Special case
		// LATER:  Make this a specialized method so the code isn't generated for
		// other types.
		if ((TypeInfo() == typeid(CVisPropList))
				&& (((CVisPropList const *) pvObj)->TypeName() != 0))
		{
			// LATER:  This code has not been tested.
			refsdostream.StartObjType(((CVisPropList const *) pvObj)->TypeName(),
					refdim, szName);
		}
		else
		{
			refsdostream.StartObjType(this, refdim, szName);
		}
	}

	// Loop through all indices writing each value
	const int cDim = refdim.CDim();
	int iDim;
	CVisDimIterator dimiteratorCur = refdim.begin();
	const CVisDimIterator dimiteratorLim = refdim.end();
	while (dimiteratorCur != dimiteratorLim)
	{
		// Check for starts of groups
		// LATER:  Make this part a helper function that does not depend on the pixel type
		// (so that the code can be moved to a CPP file and used with all templated types).
		int cGroupStart = 0;
		for (iDim = cDim - 1; iDim >= 0; --iDim)
		{
			if (dimiteratorCur[iDim] != 0)
				break;
			++ cGroupStart;
		}

		while (--cGroupStart > 0)
			refsdostream.OpenGroup(true);
		if (cGroupStart == 0)
			refsdostream.OpenGroup(!IsSimpleType());


		// Print the value.
		VisSDWriteValue(refsdostream, *pobjCur);


		// Increment pointers and check for ends of groups
		// LATER:  Make this part a helper function that does not depend on the pixel type
		// (so that the code can be moved to a CPP file and used with all templated types).
		++ pobjCur;
		int cGroupEnd = 0;
		if (++dimiteratorCur == dimiteratorLim)
		{
			cGroupEnd = cDim;
		}
		else
		{
			for (iDim = cDim - 1; iDim >= 0; --iDim)
			{
				if (dimiteratorCur[iDim] != 0)
					break;
			++ cGroupEnd;
			}
		}

		if (--cGroupEnd >= 0)
		{
			refsdostream.CloseGroup(!IsSimpleType());
			if (IsSimpleType())
				refsdostream.NewLineNext();
		}
		else if (!IsSimpleType())
			refsdostream.NewLineNext();

		while (--cGroupEnd >= 0)
			refsdostream.CloseGroup(true);
	}

	if (!fOnlyValue)
		refsdostream.EndObj(true);
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline void CVisPropTypeInfoWO<TObject>::SDOFindTypes(CVisSDOStream& refsdostream,
			const void *pvObj)
{
#ifndef VIS_NO_SD_STREAM
	VisSDOFindTypes(refsdostream, *((const TObject*) pvObj));
#endif // VIS_NO_SD_STREAM
}

template<class TObject>
inline void CVisPropTypeInfoWO<TObject>::SDOFindTypes(CVisSDOStream& refsdostream,
			const void *pvObj, const CVisDim& refdim)
{
#ifndef VIS_NO_SD_STREAM
	const TObject *pobjCur = (const TObject *) pvObj;

	// If this type does not contain property list types, we obly need to find
	// type information from the first element in the array.
	const TObject *pobjLim;
	if (FContainsPropLists())
		pobjLim = pobjCur + refdim.CObj();
	else
		pobjLim = pobjCur + 1;

	for (; pobjCur < pobjLim; ++pobjCur)
		VisSDOFindTypes(refsdostream, *pobjCur);
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline CVisPropTypeInfoWithIO<TObject>::CVisPropTypeInfoWithIO(
		const char *szName)
  : CVisPropTypeInfoWO<TObject>(szName)
{
}

template<class TObject>
inline CVisPropTypeInfoWithIO<TObject>::CVisPropTypeInfoWithIO(
		bool fSimpleType, const char *szName)
  : CVisPropTypeInfoWO<TObject>(fSimpleType, szName)
{
}

template<class TObject>
inline CVisPropTypeInfoWithIO<TObject>::~CVisPropTypeInfoWithIO(void)
{
}


template<class TObject>
inline void CVisPropTypeInfoWithIO<TObject>::ReadObj(CVisSDIStream& refsdistream,
		void *pvObj) const
{
	TObject & refobj = *((TObject *) pvObj);

#ifndef VIS_NO_SD_STREAM
	VisSDIFindTypes(refsdistream, refobj);

	VisSDReadValue(refsdistream, refobj);
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline void CVisPropTypeInfoWithIO<TObject>::ReadObjArray(
		CVisSDIStream& refsdistream, const CVisDim& refdim, void *pvObj) const
{
	// LATER:  Should this just call VisSDWriteValue or VisSDWriteObj?
	TObject *pobjCur = ((TObject *) pvObj);

#ifndef VIS_NO_SD_STREAM
	VisSDIFindTypes(refsdistream, *pobjCur);

	// Loop through all indices writing each value
	const int cDim = refdim.CDim();
	int iDim;
	CVisDimIterator dimiteratorCur = refdim.begin();
	const CVisDimIterator dimiteratorLim = refdim.end();
	while (dimiteratorCur != dimiteratorLim)
	{
		// Check for starts of groups
		// LATER:  Make this part a helper function that does not depend on the pixel type
		// (so that the code can be moved to a CPP file and used with all templated types).
		for (iDim = cDim - 1; iDim >= 0; --iDim)
		{
			if (dimiteratorCur[iDim] != 0)
				break;
			refsdistream.OpenGroup();
		}

		// Read the value.
		VisSDReadValue(refsdistream, *pobjCur);


		// Increment pointers and check for ends of groups
		// LATER:  Make this part a helper function that does not depend on the pixel type
		// (so that the code can be moved to a CPP file and used with all templated types).
		++ pobjCur;
		int cGroupEnd = 0;
		if (++dimiteratorCur == dimiteratorLim)
		{
			cGroupEnd = cDim;
		}
		else
		{
			for (iDim = cDim - 1; iDim >= 0; --iDim)
			{
				if (dimiteratorCur[iDim] != 0)
					break;
			++ cGroupEnd;
			}
		}

		while (--cGroupEnd >= 0)
			refsdistream.CloseGroup();
	}
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline void CVisPropTypeInfoWithIO<TObject>::SDIFindTypes(CVisSDIStream& refsdistream)
{
#ifndef VIS_NO_SD_STREAM
	VisSDIFindTypes(refsdistream, TObject());
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline void VisSetPropTypeInfoDequeName(
		CVisPropTypeInfoDeque<std::deque<TObject> >&refproptypeinfo)
{
	std::string strTypeIOName("deque<");
	strTypeIOName += VisGetPropTypeInfo(TObject())->Name();
	strTypeIOName += ">";
	refproptypeinfo.SetName(strTypeIOName.c_str());
}

template<class TObject>
inline CVisPropTypeInfoDeque<TObject>::CVisPropTypeInfoDeque(void)
  : CVisPropTypeInfoWithIO<TObject>()
{
	VisSetPropTypeInfoDequeName(*this);
}


template<class TObject>
inline CVisPropTypeInfoDeque<TObject>::~CVisPropTypeInfoDeque(void)
{
}


template<class TObject>
inline void VisSetPropTypeInfoVectorName(
		CVisPropTypeInfoVector<std::vector<TObject> >&refproptypeinfo)
{
	std::string strTypeIOName("vector<");
	strTypeIOName += VisGetPropTypeInfo(TObject())->Name();
	strTypeIOName += ">";
	refproptypeinfo.SetName(strTypeIOName.c_str());
}

template<class TObject>
inline CVisPropTypeInfoVector<TObject>::CVisPropTypeInfoVector(
		void)
  : CVisPropTypeInfoWithIO<TObject>()
{
	VisSetPropTypeInfoVectorName(*this);
}


template<class TObject>
inline CVisPropTypeInfoVector<TObject>::~CVisPropTypeInfoVector(
		void)
{
}


template<class TObject>
inline CVisPropTypeInfoWithIOPropList<TObject>::CVisPropTypeInfoWithIOPropList(
		const char *szName, bool fAlwaysUseTypedef)
  : CVisPropTypeInfoWithIO<TObject>(szName),
	m_pproplist(0),
	m_fAlwaysUseTypedef(fAlwaysUseTypedef)
{
}

template<class TObject>
inline CVisPropTypeInfoWithIOPropList<TObject>::~CVisPropTypeInfoWithIOPropList(
		void)
{
	if (m_pproplist != 0)
		ClearPropList();
}

template<class TObject>
inline bool CVisPropTypeInfoWithIOPropList<TObject>::AlwaysUseTypedef(void)
		const
{
	return m_fAlwaysUseTypedef;
}

template<class TObject>
inline void CVisPropTypeInfoWithIOPropList<TObject>::SetAlwaysUseTypedef(bool f)
{
	m_fAlwaysUseTypedef = f;
}

template<class TObject>
inline CVisPropList *CVisPropTypeInfoWithIOPropList<TObject>::PPropList(
        const void *pvObj)
{
	if (m_pproplist == 0)
	{
		// Use a static helper function to allocate the CVisPropList.
		m_pproplist = CVisPropList::PPropListNew();
		m_pproplist->SetTypeName(Name());
		m_pproplist->SetHasOrderedPropNameList(true);

		// The BuildPropList method should not be virtual.  (It does not
		// need to be virtual because a virtual function can be used to find
		// the right CVisPropTypeInfoWithIOPropList template to use for a
		// given object.)
		// We could end up calling virtual functions to get property type
        // information here, se we need to pass a valid pointer to an
        // object to VisBuildObjPropList.
        // (We can't just pass "*((TObject *) 0)".)
        // UNDONE:  We need to offset the references by the object's address.
        if (pvObj != 0)
        {
	    	VisBuildObjPropList(*((TObject *) pvObj), *m_pproplist);
            m_pproplist->OffsetReferences(- (int) pvObj);
        }
        else
        {
	    	TObject objectT;
            VisBuildObjPropList(objectT, *m_pproplist);
            m_pproplist->OffsetReferences(- (int) &objectT);
        }
	}

	return m_pproplist;
}


template<class TObject>
inline void CVisPropTypeInfoWithIOPropList<TObject>::SDIFindTypes(CVisSDIStream& refsdistream)
{
#ifndef VIS_NO_SD_STREAM
	refsdistream.UsePropListType(this);
#endif // VIS_NO_SD_STREAM
}


template<class TObject>
inline void CVisPropTypeInfoWithIOPropList<TObject>::SDOFindTypes(
		CVisSDOStream& refsdostream, const void *pvObj)
{
#ifndef VIS_NO_SD_STREAM
	refsdostream.UsePropListType(this, pvObj);
#endif // VIS_NO_SD_STREAM
}

template<class TObject>
inline void CVisPropTypeInfoWithIOPropList<TObject>::SDOFindTypes(
		CVisSDOStream& refsdostream, const void *pvObj, const CVisDim& refdim)
{
#ifndef VIS_NO_SD_STREAM
	const TObject *pobjCur = (const TObject *) pvObj;

	// If this type does not contain property list types, we obly need to find
	// type information from the first element in the array.
	const TObject *pobjLim;
	if (FContainsPropLists())
		pobjLim = pobjCur + refdim.CObj();
	else
		pobjLim = pobjCur + 1;

	for (; pobjCur < pobjLim; ++pobjCur)
		refsdostream.UsePropListType(this, pobjCur);
#endif // VIS_NO_SD_STREAM
}

template<class TObject>
inline void CVisPropTypeInfoWithIOPropList<TObject>::ClearPropList(void)
{
	if (m_pproplist != 0)
	{
		CVisPropList::DeletePPropList(m_pproplist);
		m_pproplist = 0;
	}
}


template<class TObject>
inline CVisRefCntObj<TObject>::CVisRefCntObj(void)
  : m_obj(),
  m_cRef(1)
{
}

template<class TObject>
inline CVisRefCntObj<TObject>::CVisRefCntObj(const TObject& refobj)
  : m_obj(refobj),
  m_cRef(1)
{
}

template<class TObject>
inline CVisRefCntObj<TObject>::~CVisRefCntObj(void)
{
}

template<class TObject>
inline ULONG CVisRefCntObj<TObject>::AddRef(void)
{
	return (ULONG) InterlockedIncrement((LONG *) &m_cRef);
}

template<class TObject>
inline ULONG CVisRefCntObj<TObject>::Release(void)
{
	ULONG cRef = (ULONG) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}

template<class TObject>
inline const TObject& CVisRefCntObj<TObject>::Obj(void) const
{
	return m_obj;
}

template<class TObject>
inline TObject& CVisRefCntObj<TObject>::Obj(void)
{
	return m_obj;
}



template<class TObject>
inline CVisRefCntObjArray<TObject>::CVisRefCntObjArray(const CVisDim& refdim)
  : CVisRefCntObjArrayBase(sizeof(TObject), true, refdim,
			(void *) new TObject[refdim.CObj()]),
	m_cRef(1)
{
}

template<class TObject>
inline CVisRefCntObjArray<TObject>::CVisRefCntObjArray(const CVisDim& refdim,
		void *pvWhere)
  : CVisRefCntObjArrayBase(sizeof(TObject), false, refdim, pvWhere),
	m_cRef(1)
{
}

template<class TObject>
inline CVisRefCntObjArray<TObject>::~CVisRefCntObjArray(void)
{
}

template<class TObject>
inline ULONG CVisRefCntObjArray<TObject>::AddRef(void)
{
	return (ULONG) InterlockedIncrement((LONG *) &m_cRef);
}

template<class TObject>
inline ULONG CVisRefCntObjArray<TObject>::Release(void)
{
	ULONG cRef = (ULONG) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}

template<class TObject>
inline TObject *CVisRefCntObjArray<TObject>::PObjFirst(void) const
{
	return ((TObject *) PvObjFirst());
}

template<class TObject>
inline TObject& CVisRefCntObjArray<TObject>::Obj(const CVisDimIndex& refdimindex) const
{
	return *((TObject *) PvObj(refdimindex));
}

template<class TObject>
inline CVisRefCntObjArray<TObject> *CVisRefCntObjArray<TObject>::Clone(void) const
{
	CVisRefCntObjArray<TObject> *pClone;
	
	if (FAllocated())
	{
		pClone = new CVisRefCntObjArray<TObject>(Dim());

		// Need to copy the object values.
		const TObject *pObjSrc = PObjFirst();
		const TObject *pObjSrcLim = pObjSrc + Dim().CObj();
		TObject *pObjDest = pClone->PObjFirst();
		for (; pObjSrc != pObjSrcLim; ++pObjDest, ++pObjSrc)
			*pObjDest = *pObjSrc;
	}
	else
	{
		pClone = new CVisRefCntObjArray<TObject>(Dim(), PvObjFirst());
	}

	return pClone;
}

template<class TObject>
inline CVisRefCntObjArray<TObject>::CVisRefCntObjArray(const CVisRefCntObjArray<TObject>& refrefcntobj)
{
	// Copy constructor should not be called
	assert(0);
}

template<class TObject>
inline CVisRefCntObjArray<TObject>& CVisRefCntObjArray<TObject>::operator=(
		const CVisRefCntObjArray<TObject>& refrefcntobj)
{
	// Assignment operator should not be called
	assert(0);

	return *this;
}



// Helper functions

// For common types, we'll want to specialize this and put the specialization in
// a CPP file.  (E.g. for CVisImage<BYTE>, but maybe not for CVisImage<__int64>.)
template<class TObject>
inline CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		TObject const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<TObject>(0, fAlwaysTypedef); }

// For common types, we'll want to specialize this and put the specialization in
// a CPP file.
template<class TObject>
inline CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIO(TObject const &refobj,
		bool fSimpleType)
	{ return new CVisPropTypeInfoWithIO<TObject>(fSimpleType); }


// (We can specialize this for some types if needed.)
template<class TObject>
inline void VisBuildObjPropList(TObject& refobj, CVisPropList& refproplist)
	{ refobj.BuildPropList(refproplist); }


// Helper macros useful for user-defined classes

#define VIS_DECLARE_PROP_LIST_CLASS_BUILD_PL(Storage, TObject)			\
	Storage void VisBuildObjPropList(TObject & refobj,			\
			CVisPropList& refproplist);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoForClass(				\
			TObject const & refobj);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoForClass(				\
			TObject const & refobj);									\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const & refobj);

#define VIS_TEMPLATE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(TTemplate, TObject)	\
	template<class TTemplate >												\
	inline CVisPropTypeInfoBase *VisPPropTypeInfoForClass(					\
			TObject const & refobj)											\
		{ return refobj.PropTypeInfoLookup(); }								\
	template<class TTemplate >												\
	inline CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(				\
			TObject const & refobj)											\
		{ return refobj.PropTypeInfoNew(); }


#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(			\
					Storage, TObject, fAlwaysTypedef)				\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_NO_IO(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_DEQUE(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj);

#define VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_VECTOR(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj);

#define VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisGetPropTypeInfo(TObject const& refobject);



#define VIS_DEFINE_PROP_LIST_CLASS_BUILD_PL(Storage, TObject)			\
	Storage void VisBuildObjPropList(TObject & refobj,			\
			CVisPropList& refproplist)									\
		{ refobj.BuildPropList(refproplist); }


#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoForClass(				\
			TObject const & refobj)										\
		{ return VisLookupPropTypeInfo(typeid(TObject)); }

#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoForClass(				\
			TObject const & refobj)										\
		{ return refobj.PropTypeInfoLookup(); }							\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const & refobj)										\
		{ return refobj.PropTypeInfoNew(); }


#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(			\
					Storage, TObject, fAlwaysTypedef)				\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj)									\
		{ return VisPPropTypeInfoNewWithIOPropList<TObject >(refobj,\
				fAlwaysTypedef); }

#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj)										\
		{ return VisPPropTypeInfoNewWithIO<TObject >(refobj, false); }

#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_NO_IO(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj)										\
		{ return new CVisPropTypeInfoNoIO<TObject >; }

#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_DEQUE(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj)										\
		{ return new CVisPropTypeInfoDeque<TObject >; }

#define VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VECTOR(Storage, TObject)	\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj)										\
		{ return new CVisPropTypeInfoVector<TObject >; }


#define VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)				\
	Storage CVisPropTypeInfoBase *VisGetPropTypeInfo(TObject const& refobject)	\
	{																			\
		CVisPropTypeInfoBase *pproptypeinfo										\
				= VisPPropTypeInfoForClass(refobject);							\
		if (pproptypeinfo == 0)													\
		{																		\
			pproptypeinfo = VisPPropTypeInfoNewForClass(refobject);				\
			VisAddPropTypeInfo(pproptypeinfo);									\
		}																		\
		return pproptypeinfo;													\
	}
#ifdef TAKEOUT // UNDONE:  Remove references to this macro.
#endif // TAKEOUT // UNDONE:  Remove references to this macro.


#define VIS_PROP_LIST_CLASS_BUILD_PL(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_BUILD_PL(inline, TObject)


#define VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(inline, TObject)

#define VIS_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(inline, TObject)


#define VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(TObject,	\
			fAlwaysTypedef)									\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(		\
					inline, TObject, fAlwaysTypedef)

#define VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(inline, TObject)

#define VIS_PROP_LIST_CLASS_TYPE_INFO_NO_IO(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_NO_IO(inline, TObject)

#define VIS_PROP_LIST_CLASS_TYPE_INFO_DEQUE(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_DEQUE(inline, TObject)

#define VIS_PROP_LIST_CLASS_TYPE_INFO_VECTOR(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VECTOR(inline, TObject)


#define VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(inline, TObject)



// Helper macros used for STL containers

#define VIS_DECLARE_PROP_LIST_CLASS_DEQUE(Storage, TObject)						\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_DEQUE(Storage, std::deque<TObject >)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, std::deque<TObject >)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, std::deque<TObject >)

#define VIS_DECLARE_PROP_LIST_CLASS_VECTOR(Storage, TObject)						\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_VECTOR(Storage, std::vector<TObject >)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, std::vector<TObject >)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, std::vector<TObject >)


#define VIS_DEFINE_PROP_LIST_CLASS_DEQUE(Storage, TObject)						\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_DEQUE(Storage, std::deque<TObject >)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, std::deque<TObject >)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, std::deque<TObject >)

#define VIS_DEFINE_PROP_LIST_CLASS_VECTOR(Storage, TObject)						\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VECTOR(Storage, std::vector<TObject >)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, std::vector<TObject >)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, std::vector<TObject >)


#define VIS_PROP_LIST_CLASS_DEQUE(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_DEQUE(inline, TObject)

#define VIS_PROP_LIST_CLASS_VECTOR(TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_VECTOR(inline, TObject)	


// Helper macros used for standard types

#define VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj);									\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_WO_STRING_CLASS(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj);									\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_STRING_CLASS(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj);									\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)


#define VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(Storage, TObject)				\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(			\
			TObject const &refobj)										\
		{ return VisPPropTypeInfoNewWithIO<TObject >(refobj, true); }	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_WO_STRING_CLASS(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj)									\
		{ return new CVisPropTypeInfoWO<TObject >("string"); }		\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_STRING_CLASS(Storage, TObject)		\
	Storage CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(		\
			TObject const &refobj)									\
		{ return new CVisPropTypeInfoWithIO<TObject >("string"); }	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)


#define VIS_PROP_LIST_SIMPLE_CLASS(TObject)	\
	VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(inline, TObject)

#define VIS_PROP_LIST_WO_STRING_CLASS(TObject)	\
	VIS_DEFINE_PROP_LIST_WO_STRING_CLASS(inline, TObject)

#define VIS_PROP_LIST_STRING_CLASS(TObject)	\
	VIS_DEFINE_PROP_LIST_STRING_CLASS(inline, TObject)



// Helper macros like those in VisPropList.h, but with storage specifiers

#ifdef VIS_NO_SD_STREAM

#define VIS_DECLARE_SDI_CLASS_WITH_PL(Storage, TObject)
#define VIS_DECLARE_SDI_CLASS(Storage, TObject)
#define VIS_DECLARE_SDI_CLASS_NO_IO(Storage, TObject)

#define VIS_DECLARE_SDO_CLASS_WITH_PL(Storage, TObject)
#define VIS_DECLARE_SDO_CLASS(Storage, TObject)
#define VIS_DECLARE_SDO_CLASS_NO_IO(Storage, TObject)


#define VIS_DEFINE_SDI_CLASS_WITH_PL(Storage, TObject)
#define VIS_DEFINE_SDI_CLASS(Storage, TObject)
#define VIS_DEFINE_SDI_CLASS_NO_IO(Storage, TObject)

#define VIS_DEFINE_SDO_CLASS_WITH_PL(Storage, TObject)
#define VIS_DEFINE_SDO_CLASS(Storage, TObject)
#define VIS_DEFINE_SDO_CLASS_NO_IO(Storage, TObject)

#endif // VIS_NO_SD_STREAM


#define VIS_DECLARE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)		\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(Storage, TObject, true)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)				\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_CLASS_WITH_IO_PL(Storage, TObject)				\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(Storage, TObject, false)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)				\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_CLASS_WITH_IO(Storage, TObject)		\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_CLASS_NO_IO(Storage, TObject)			\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_NO_IO(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DECLARE_PROP_LIST_CLASS_VIRTUAL(Storage, TObject)		\
	VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)


#define VIS_DEFINE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(Storage, TObject, true)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)				\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_CLASS_WITH_IO_PL(Storage, TObject)				\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(Storage, TObject, false)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)				\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_CLASS_WITH_IO(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_CLASS_NO_IO(Storage, TObject)			\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_NO_IO(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)

#define VIS_DEFINE_PROP_LIST_CLASS_VIRTUAL(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(Storage, TObject)



#define VIS_DECLARE_SD_CLASS_WITH_IO_PL(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_CLASS_WITH_IO_PL(Storage, TObject)	\
	VIS_DECLARE_SDI_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DECLARE_SDO_CLASS_WITH_PL(Storage, TObject)

#define VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)	\
	VIS_DECLARE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)	\
	VIS_DECLARE_SDI_CLASS_WITH_PL(Storage, TObject)					\
	VIS_DECLARE_SDO_CLASS_WITH_PL(Storage, TObject)	

#define VIS_DECLARE_SD_VIRTUAL_CLASS_WITH_IO_PL(Storage, TObject)\
	VIS_DECLARE_PROP_LIST_CLASS_VIRTUAL(Storage, TObject)		\
	VIS_DECLARE_SDI_CLASS_WITH_PL(Storage, TObject)				\
	VIS_DECLARE_SDO_CLASS_WITH_PL(Storage, TObject)

#define VIS_DECLARE_SD_CLASS_WITH_IO(Storage, TObject)		\
	VIS_DECLARE_PROP_LIST_CLASS_WITH_IO(Storage, TObject)	\
	VIS_DECLARE_SDI_CLASS(Storage, TObject)			\
	VIS_DECLARE_SDO_CLASS(Storage, TObject)

#define VIS_DECLARE_SD_CLASS_NO_IO(Storage, TObject)		\
	VIS_DECLARE_PROP_LIST_CLASS_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDI_CLASS_NO_IO(Storage, TObject)		\
	VIS_DECLARE_SDO_CLASS_NO_IO(Storage, TObject)


#define VIS_DEFINE_SD_CLASS_WITH_IO_PL(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_WITH_IO_PL(Storage, TObject)	\
	VIS_DEFINE_SDI_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DEFINE_SDO_CLASS_WITH_PL(Storage, TObject)

#define VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)	\
	VIS_DEFINE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(Storage, TObject)	\
	VIS_DEFINE_SDI_CLASS_WITH_PL(Storage, TObject)					\
	VIS_DEFINE_SDO_CLASS_WITH_PL(Storage, TObject)

#define VIS_DEFINE_SD_VIRTUAL_CLASS_WITH_IO_PL(Storage, TObject)\
	VIS_DEFINE_PROP_LIST_CLASS_VIRTUAL(Storage, TObject)		\
	VIS_DEFINE_SDI_CLASS_WITH_PL(Storage, TObject)				\
	VIS_DEFINE_SDO_CLASS_WITH_PL(Storage, TObject)

#define VIS_DEFINE_SD_CLASS_WITH_IO(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_CLASS_WITH_IO(Storage, TObject)	\
	VIS_DEFINE_SDI_CLASS(Storage, TObject)			\
	VIS_DEFINE_SDO_CLASS(Storage, TObject)

#define VIS_DEFINE_SD_CLASS_NO_IO(Storage, TObject)		\
	VIS_DEFINE_PROP_LIST_CLASS_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDI_CLASS_NO_IO(Storage, TObject)		\
	VIS_DEFINE_SDO_CLASS_NO_IO(Storage, TObject)



#ifdef _DEBUG
#undef new
#endif // _DEBUG
