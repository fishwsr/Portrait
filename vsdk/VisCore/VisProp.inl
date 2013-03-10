// VisProp.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


inline CVisProp::~CVisProp(void)
{
	ReleaseObj();
}


inline CVisProp& CVisProp::operator=(
		const CVisProp& refprop)
{
	Assign(refprop);
	return *this;
}


inline bool CVisProp::IsValid(void) const
{
	assert((!m_fInitialized) || (m_pproptypeinfo != 0));
	return m_fInitialized;
}

inline bool CVisProp::IsShared(void) const
{
	assert(IsValid());
	return m_fShared;
}

inline bool CVisProp::IsPrinted(void) const
{
	assert(IsValid());
	return m_fPrint;
}

inline bool CVisProp::IsObjReference(void) const
{
	assert(IsValid());
	return m_fObjReference;
}

inline bool CVisProp::IsArray(void) const
{
	assert(IsValid());
	return m_fArray;
}

inline void CVisProp::SetPrinting(bool f)
{
	m_fPrint = f;
}

inline const type_info& CVisProp::TypeInfo(void) const
{
	return PropTypeInfo().TypeInfo();
}

inline CVisPropTypeInfoBase& CVisProp::PropTypeInfo(void) const
{
	assert(m_pproptypeinfo != 0);
	return *m_pproptypeinfo;
}

inline bool CVisProp::FPointerToObject(void) const
{
	return m_fPointerToObject;
}


inline void CVisProp::ClearValue(void)
{
	m_llData = 0.0;
}

inline void CVisProp::SetFInitialized(bool f)
{
	m_fInitialized = f;
}

inline void CVisProp::SetFShared(bool f)
{
	m_fShared = f;
}

inline void CVisProp::SetFPointerToObject(bool f)
{
	m_fPointerToObject = f;
}

inline void CVisProp::SetFObjReference(bool f)
{
	m_fObjReference = f;
}

inline void CVisProp::SetFArray(bool f)
{
	m_fArray = f;
}

inline const char *CVisProp::Name(void) const
{
	return m_propkeydata.Name();
}

inline const CVisPropKeyData& CVisProp::KeyData(void) const
{
	return m_propkeydata;
}

inline void CVisProp::SetKeyData(const CVisPropKeyData& refpropkeydata)
{
	m_propkeydata = refpropkeydata;
}

inline void CVisProp::SetPPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo)
{
	assert(pproptypeinfo != 0);

	m_pproptypeinfo = pproptypeinfo;
	m_fPrint = m_pproptypeinfo->ArePropsPrinted();
}

inline void *CVisProp::PvRefCntObj(void) const
{
	assert(!IsArray());
	assert(FPointerToObject());
	return *((void * *) (void *) &m_llData);
}

inline const CVisDim& CVisProp::Dim(void) const
{
	return PRefCntArray()->Dim();
}

inline void *CVisProp::PvObj(const CVisDimIndex& refdimindex) const
{
	assert(IsArray());
	return PRefCntArray()->PvObj(refdimindex);
}


inline CVisRefCntObjArrayBase *CVisProp::PRefCntArray(void) const
{
	assert(IsArray());
	assert(FPointerToObject());
	return *((CVisRefCntObjArrayBase * *) (void *) &m_llData);
}

VIS_PROP_LIST_CLASS_WITH_IO(CVisProp)


#ifndef VIS_NO_SD_STREAM

VIS_DECLARE_SDI_FIND_TYPES_PL(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDI_READ_OBJ_DEFAULT(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDI_READ_OP_DEFAULT(VisPropListExportDeclspec, CVisProp)

VIS_DECLARE_SDO_FIND_TYPES_PL(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(VisPropListExportDeclspec, CVisProp)
VIS_DECLARE_SDO_WRITE_OP_DEFAULT(VisPropListExportDeclspec, CVisProp)

#endif // VIS_NO_SD_STREAM


#ifdef _DEBUG
#undef new
#endif // _DEBUG
