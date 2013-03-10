// VisPropList.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



template<class TObject>
void VisUsePropTypeWithIO(const TObject& refobj)
	{ VisGetPropTypeInfo(refobj); }



inline CVisPropList& CVisPropList::operator=(const CVisPropList& refproplist)
{
	Assign(refproplist);
	return *this;
}

inline const char *CVisPropList::Name(
		const CVisPropList::const_iterator& refiterator) const
{
	return (*refiterator).Name();
}

inline void CVisPropList::DeleteProp(const char *szPropName)
{
	DeleteProp(IteratorFromPropName(szPropName));
}


inline const char *CVisPropList::TypeName(void) const
{
	return m_szTypeName;
}

inline bool CVisPropList::IsShared(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).IsShared();
}

inline bool CVisPropList::IsShared(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).IsShared();
}

inline bool CVisPropList::IsPrinted(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).IsPrinted();
}

inline bool CVisPropList::IsPrinted(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).IsPrinted();
}

inline bool CVisPropList::IsObjReference(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).IsObjReference();
}

inline bool CVisPropList::IsObjReference(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).IsObjReference();
}

inline bool CVisPropList::IsArray(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).IsArray();
}

inline bool CVisPropList::IsArray(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).IsArray();
}

inline const CVisDim& CVisPropList::Dim(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).Dim();
}

inline const CVisDim& CVisPropList::Dim(const char *szPropName) const
{
	return Dim(IteratorFromPropName(szPropName));
}

inline void CVisPropList::SetPrinting(
		const CVisPropList::const_iterator &refiterator, bool f)
{
	(const_cast<CVisProp &>(*refiterator)).SetPrinting(f);
}

inline void CVisPropList::SetPrinting(const char *szPropName, bool f)
{
	assert(HasProp(szPropName));
	PropGetSz(szPropName).SetPrinting(f);
}

inline void CVisPropList::SetSharing(
		const CVisPropList::const_iterator &refiterator, bool f)
{
	(const_cast<CVisProp &>(*refiterator)).SetSharing(f);
}

inline void CVisPropList::SetSharing(const char *szPropName, bool f)
{
	assert(HasProp(szPropName));
	PropGetSz(szPropName).SetSharing(f);
}

inline const char *CVisPropList::TypeName(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).TypeName();
}

inline const char *CVisPropList::TypeName(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).TypeName();
}

inline const type_info& CVisPropList::TypeInfo(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).PropTypeInfo().TypeInfo();
}

inline const type_info& CVisPropList::TypeInfo(const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).PropTypeInfo().TypeInfo();
}

inline CVisPropTypeInfoBase& CVisPropList::PropTypeInfo(
		const CVisPropList::const_iterator &refiterator) const
{
	return (*refiterator).PropTypeInfo();
}

inline CVisPropTypeInfoBase& CVisPropList::PropTypeInfo(
		const char *szPropName) const
{
	assert(HasProp(szPropName));
	return Prop(szPropName).PropTypeInfo();
}

inline const CVisProp& CVisPropList::Prop(const char *szPropName) const
{
	return PropGetSz(szPropName);
}

inline const CVisProp& CVisPropList::PropGetIterator(
		const const_iterator &refiterator) const
{
	return *refiterator;
}

inline CVisProp& CVisPropList::PropGetIterator(
		const const_iterator &refiterator)
{
	return const_cast<CVisProp&>(*refiterator);
}


inline bool CVisPropList::HasOrderedPropNameList(void) const
{
	return (m_porderedpropnamelist != 0);
}

inline const CVisOrderedPropNameList *CVisPropList::POrderedPropNameList(void)
		const
{
	return m_porderedpropnamelist;
}



inline CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(
		CVisPropList const &refproplist)
{
	// PropTypeInfo for PropList is added in the CVisPropTypeInfoMap ctor, so
	// we should be able to look it up here.
	CVisPropTypeInfoBase *pproptypeinfo
			= VisLookupPropTypeInfo(typeid(CVisPropList));
	assert(pproptypeinfo != 0);
	return pproptypeinfo;
}


VIS_DECLARE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(VisPropListExportDeclspec, CVisPropList)
VIS_DECLARE_PROP_LIST_GET_PROP_TYPE_INFO(VisPropListExportDeclspec, CVisPropList)


#ifndef VIS_NO_SD_STREAM

VisPropListExport void VisSDOFindTypes(CVisSDOStream& refsdostream,
		CVisPropList const& refobj);

VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisPropList)

VisPropListExport void VisSDWriteObj(CVisSDOStream& refsdostream,
		CVisPropList const& refobj, const char *szName);

VIS_DECLARE_SDO_WRITE_OP_DEFAULT(VisPropListExportDeclspec, CVisPropList)


//VIS_DECLARE_SDI_FIND_TYPES_DEFAULT(CVisPropList)
VIS_DECLARE_SDI_FIND_TYPES_NOOP(VisPropListExportDeclspec, CVisPropList)
VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisPropList)

VisPropListExport void VisSDReadObj(CVisSDIStream& refsdistream,
		CVisPropList& refobj);

VIS_DECLARE_SDI_READ_OP_DEFAULT(VisPropListExportDeclspec, CVisPropList)

#endif // VIS_NO_SD_STREAM


// Helper function definitions for standard types.
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, bool)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, char)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, signed char)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned char)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, short)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned short)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, int)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned int)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, long)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned long)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, __int64)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned __int64)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, float)
VIS_DECLARE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, double)

VIS_DECLARE_PROP_LIST_WO_STRING_CLASS(VisPropListExportDeclspec, char *)
VIS_DECLARE_PROP_LIST_WO_STRING_CLASS(VisPropListExportDeclspec, char const *)
VIS_DECLARE_PROP_LIST_STRING_CLASS(VisPropListExportDeclspec, std::string)

VIS_DECLARE_PROP_LIST_CLASS_VECTOR(VisPropListExportDeclspec, std::string)

VIS_DECLARE_PROP_LIST_CLASS_NO_IO(VisPropListExportDeclspec, void *)
VIS_DECLARE_PROP_LIST_CLASS_NO_IO(VisPropListExportDeclspec, void const *)


VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, POINT)
VisPropListExport void VisBuildObjPropList(POINT& refpoint,
		CVisPropList& refproplist);

VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, SIZE)
VisPropListExport void VisBuildObjPropList(SIZE& refsize,
		CVisPropList& refproplist);

VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, RECT)
VisPropListExport void VisBuildObjPropList(RECT& refrect,
		CVisPropList& refproplist);



// Template macros (which don't define vector or deque prop helper functions)

#define VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(TTemplate, TObject)	\
	template<class TTemplate >												\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(TObject, true)					\
	template<class TTemplate >												\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)							\
	template<class TTemplate >												\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO_PL(TTemplate, TObject)	\
	template<class TTemplate >										\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(TObject, false)		\
	template<class TTemplate >										\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)					\
	template<class TTemplate >										\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO(TTemplate, TObject)\
	template<class TTemplate >									\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(TObject)				\
	template<class TTemplate >									\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)				\
	template<class TTemplate >									\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_TEMPLATE_PROP_LIST_CLASS_NO_IO(TTemplate, TObject)	\
	template<class TTemplate >									\
	VIS_PROP_LIST_CLASS_TYPE_INFO_NO_IO(TObject)				\
	template<class TTemplate >									\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)				\
	template<class TTemplate >									\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_TEMPLATE_PROP_LIST_CLASS_VIRTUAL(TTemplate, TObject)		\
	VIS_TEMPLATE_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(TTemplate, TObject)	\
	template<class TTemplate >											\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)


#define VIS_TEMPLATE_SD_CLASS_WITH_IO_PL(TTemplate, TObject)	\
	VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO_PL(TTemplate, TObject)	\
	VIS_TEMPLATE_SDI_CLASS_WITH_PL(TTemplate, TObject)			\
	VIS_TEMPLATE_SDO_CLASS_WITH_PL(TTemplate, TObject)

#define VIS_TEMPLATE_SD_CLASS_WITH_IO_PL_TYPEDEF(TTemplate, TObject)	\
	VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(TTemplate, TObject)	\
	VIS_TEMPLATE_SDI_CLASS_WITH_PL(TTemplate, TObject)					\
	VIS_TEMPLATE_SDO_CLASS_WITH_PL(TTemplate, TObject)

#define VIS_TEMPLATE_SD_VIRTUAL_CLASS_WITH_IO_PL(TTemplate, TObject)\
	VIS_TEMPLATE_PROP_LIST_CLASS_VIRTUAL(TTemplate, TObject)		\
	VIS_TEMPLATE_SDI_CLASS_WITH_PL(TTemplate, TObject)				\
	VIS_TEMPLATE_SDO_CLASS_WITH_PL(TTemplate, TObject)

#define VIS_TEMPLATE_SD_CLASS_WITH_IO(TTemplate, TObject)		\
	VIS_TEMPLATE_PROP_LIST_CLASS_WITH_IO(TTemplate, TObject)	\
	VIS_TEMPLATE_SDI_CLASS_WITH_IO(TTemplate, TObject)			\
	VIS_TEMPLATE_SDO_CLASS_WITH_IO(TTemplate, TObject)

#define VIS_TEMPLATE_SD_CLASS_NO_IO(TTemplate, TObject)		\
	VIS_TEMPLATE_PROP_LIST_CLASS_NO_IO(TTemplate, TObject)	\
	VIS_TEMPLATE_SDI_CLASS_NO_IO(TTemplate, TObject)		\
	VIS_TEMPLATE_SDO_CLASS_NO_IO(TTemplate, TObject)



#ifdef _DEBUG
#undef new
#endif // _DEBUG
