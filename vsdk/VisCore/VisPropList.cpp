// VisPropList.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// LATER:  Add code to use the following types in property lists:
//   CVisDim, CVisDimIndex, CVisDimIterator
//   CVisImageBase::iterator?
//   CVisPump (non-printing)
//   CVisMemBlock (non-printing?  or dump hex data?)
//   CVisSDIStream and CVisSDOStream (both non-printing by default)


// UNDONE:  Add Copy method to CVisPropList and CVisProp.

// UNDONE:  Add the following to the examples section in the documentation:
//		A class that can be used in Prop Lists but not in Streams.
//		A class containing a user Prop List (using the VIS_SZ_SD_USER_PROP_LIST string).
//		Example text from streams when example code is used to write to stream.


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#ifdef VIS_CORE_BUILD
	#include "VisCorePch.h"
	#include "VisCoreProj.h"
#else
	#include "VisPropListProj.h"
#endif


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


// Note:  We want to keep all calls to std::map methods (like begin, end,
// and iterator methods) in the CPP files.  The implementation appears to
// use static variables which may cause problems for us if these methods
// are inline.


CVisPropList::const_iterator::const_iterator(
		CVisPropValueMap::const_iterator const &refit)
  : m_iteratorMap(refit)
{
}

CVisPropList::const_iterator::const_iterator(
		const CVisPropList::const_iterator &refiterator)
  : m_iteratorMap(refiterator.m_iteratorMap)
{
}

CVisPropList::const_iterator& CVisPropList::const_iterator::operator=(
		const CVisPropList::const_iterator &refiterator)
{
	m_iteratorMap = refiterator.m_iteratorMap;
	return *this;
}

bool CVisPropList::const_iterator::operator==(
		const CVisPropList::const_iterator& refiterator) const
{
	return (m_iteratorMap == refiterator.m_iteratorMap);
}

bool CVisPropList::const_iterator::operator!=(
		const CVisPropList::const_iterator& refiterator) const
{
	return (m_iteratorMap != refiterator.m_iteratorMap);
}

CVisPropList::const_iterator
	CVisPropList::const_iterator::operator++(int)
{
	CVisPropList::const_iterator iteratorRet(*this);
	++m_iteratorMap;
	return iteratorRet;
}

CVisPropList::const_iterator&
	CVisPropList::const_iterator::operator++(void)
{
	++m_iteratorMap;
	return *this;
}

const CVisProp& CVisPropList::const_iterator::operator*(void) const
{
	return m_iteratorMap->second;
}

CVisPropList::const_iterator CVisPropList::begin(void) const
{
	return CVisPropList::const_iterator(m_map.begin());
}

CVisPropList::const_iterator CVisPropList::end(void) const
{
	return CVisPropList::const_iterator(m_map.end());
}

bool CVisPropList::IsEmpty(void) const
{
//	return (m_map.size() > 0);
	return m_map.empty();
}

int CVisPropList::CProp(void) const
{
	return m_map.size();
}


CVisPropList::CVisPropList(void)
  : m_map(CVisFnOpCmpSz()),
	m_szTypeName(0),
	m_porderedpropnamelist(0)
{
}

CVisPropList::CVisPropList(const CVisPropList& refproplist)
  : m_map(CVisFnOpCmpSz()),
	m_szTypeName(0),
	m_porderedpropnamelist(0)
{
	Assign(refproplist);
}

CVisPropList::~CVisPropList(void)
{
	Clear();
	SetTypeName(0);
	SetHasOrderedPropNameList(false);
}

CVisPropList::const_iterator CVisPropList::IteratorFromPropName(
		const char *szPropName) const
{
	return CVisPropList::const_iterator(m_map.find(szPropName));
}

void CVisPropList::DeleteProp(const const_iterator &refiterator)
{
	m_map.erase((CVisPropValueMap::iterator&) refiterator.m_iteratorMap);
}

void CVisPropList::Clear(void)
{
	m_map.clear();
	if (m_porderedpropnamelist != 0)
		m_porderedpropnamelist->clear();
}

void CVisPropList::SetTypeName(const char *szTypeName)
{
	if (m_szTypeName != 0)
		delete m_szTypeName;

	if ((szTypeName == 0) || (*szTypeName == 0))
	{
		m_szTypeName = 0;
	}
	else
	{
		m_szTypeName = new char[strlen(szTypeName) + 1];
		strcpy(m_szTypeName, szTypeName);
	}
}

bool CVisPropList::HasProp(const char *szPropName) const
{
	return (m_map.find(szPropName) != m_map.end());
}

void CVisPropList::AddProp(const CVisProp& refprop)
{
	CVisProp& refpropExisting = PropGetSz(refprop.Name());
	
	bool fHaveExisting = (&refpropExisting != 0);
	if (fHaveExisting)
	{
		CVisPropKeyData propkeydataT(refpropExisting.KeyData());
		refpropExisting = refprop;
		refpropExisting.SetKeyData(propkeydataT);
	}
	else
	{
		m_map[refprop.Name()] = refprop;
	}
}

const CVisProp& CVisPropList::PropGetSz(const char *szPropName) const
{
	CVisPropValueMap::const_iterator iteratorMap = m_map.find(szPropName);
	if (iteratorMap == m_map.end())
		return *((const CVisProp *) 0);
	return iteratorMap->second;
}

CVisProp& CVisPropList::PropGetSz(const char *szPropName)
{
	CVisPropValueMap::iterator iteratorMap = m_map.find(szPropName);
	if (iteratorMap == m_map.end())
		return *((CVisProp *) 0);
	return iteratorMap->second;
}

void CVisPropList::AddSzOrderedPropName(const char *szPropName)
{
	assert(CProp() >= 1);
	assert(HasOrderedPropNameList());
	m_porderedpropnamelist->push_back(szPropName);
}

void CVisPropList::SetHasOrderedPropNameList(bool f)
{
	if (f)
	{
		assert(CProp() == 0);
		if (m_porderedpropnamelist == 0)
			m_porderedpropnamelist = new CVisOrderedPropNameList;
	}
	else
	{
		if (m_porderedpropnamelist != 0)
		{
			delete m_porderedpropnamelist;
			m_porderedpropnamelist = 0;
		}
	}
}

CVisPropList *CVisPropList::PPropListNew(void)
{
	return new CVisPropList;
}

void CVisPropList::DeletePPropList(CVisPropList *pproplist)
{
	delete pproplist;
}


CVisProp& CVisPropList::PropListValueNew(const char *szPropName)
{
	assert(!HasProp(szPropName));

	// The CVisProp must control the const char * used as its key in the map,
	// so we need to use the CVisPropKeyData object here.
	CVisPropKeyData propkeydata(szPropName);

	CVisProp& refprop = m_map[propkeydata.Name()];
	refprop.SetKeyData(propkeydata);

	return refprop;
}

void CVisPropList::Assign(const CVisPropList& refproplist)
{
	SetTypeName(refproplist.TypeName());
	if (refproplist.HasOrderedPropNameList())
	{
		if (m_porderedpropnamelist == 0)
			m_porderedpropnamelist = new CVisOrderedPropNameList;
		m_porderedpropnamelist->clear();
		int istrLim = refproplist.m_porderedpropnamelist->size();
		for (int istr = 0; istr < istrLim; ++istr)
		{
			(*m_porderedpropnamelist)[istr]
					= (*refproplist.m_porderedpropnamelist)[istr];
		}
	}
	else
	{
		if (m_porderedpropnamelist != 0)
		{
			delete m_porderedpropnamelist;
			m_porderedpropnamelist = 0;
		}
	}

	CVisPropValueMap::const_iterator iteratorMap = refproplist.m_map.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	for (; iteratorMap != iteratorMapLim; ++iteratorMap)
	int iLim = refproplist.m_map.size();
	for (int i = 0; i != iLim; ++iteratorMap, ++i)
	{
		// This line is actually very tricky:  The const char * returned from
		// szName is ref-counted so that it can be used as the key in both maps.
		m_map[iteratorMap->second.Name()] = iteratorMap->second;
	}
}


void CVisPropList::OffsetReferences(int ibOffset)
{
	CVisPropValueMap::const_iterator iteratorMap = m_map.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	for (; iteratorMap != iteratorMapLim; ++iteratorMap)
	int iLim = m_map.size();
	for (int i = 0; i != iLim; ++iteratorMap, ++i)
	{
		if (iteratorMap->second.IsObjReference())
        {
            const_cast<CVisProp&>(iteratorMap->second).OffsetReference(
					ibOffset);
        }
	}
}

void CVisPropList::WriteValue(CVisSDOStream& refsdostream,
		bool fGroupValue, const void *pvReferenceOffset) const
{
#ifndef VIS_NO_SD_STREAM
	CVisPropValueMap::const_iterator iteratorMap = m_map.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropValueMap::const_iterator iteratorMapLim = m_map.end();
	bool fEmpty = m_map.empty();

	if (fGroupValue)
		refsdostream.OpenGroup(!fEmpty);

	if (HasOrderedPropNameList())
	{
		int iLim = POrderedPropNameList()->size();
		assert(iLim == CProp());
		for (int i = 0; i != iLim; ++i)
		{
			const char *szProp = (*POrderedPropNameList())[i].c_str();
			iteratorMap = m_map.find(szProp);
			iteratorMap->second.WriteValue(refsdostream, false,
					pvReferenceOffset);
		}
	}
	else
	{
//		for (; iteratorMap != iteratorMapLim; ++iteratorMap)
		int iLim = m_map.size();
		for (int i = 0; i != iLim; ++iteratorMap, ++i)
		{
			iteratorMap->second.WriteValue(refsdostream, false,
					pvReferenceOffset);
		}
	}

	if (fGroupValue)
		refsdostream.CloseGroup(!fEmpty);
#endif // VIS_NO_SD_STREAM
}

void CVisPropList::ReadPropValue(const char *szType, const char *szName,
		const CVisDim& refdim, bool fShared, CVisSDIStream& refsdistream,
		const void *pvObjOffset)
{
#ifndef VIS_NO_SD_STREAM
	// Assume properties are all expected
	assert((szName != 0) && (*szName != 0) && (*szName != '~'));
	CVisProp *pprop = &(PropGetSz(szName));

	if (pprop != 0)
	{
		assert(pprop->IsValid());
		assert((pvObjOffset == 0) || (pprop->IsObjReference()));

		// Check that the type is as expected.
		if (strcmp(szType, pprop->TypeName()) != 0)
		{
#ifdef _DEBUG
			OutputDebugString("VisSDK Warning:  Property '");
			OutputDebugString(szName);
			OutputDebugString("' type '");
			OutputDebugString(szType);
			OutputDebugString("' doesn't match type '");
			OutputDebugString(pprop->TypeName());
			OutputDebugString("' in CVisPropList::ReadPropValue.\n");
#endif // _DEBUG

			if (refsdistream.UseStrictTypeMatching())
			{
				throw CVisSDIError("Type mismatch",
						"CVisPropList::ReadPropValue", refsdistream.ILine());
			}
		}

		// Check that the array dimensions are as expected.
		if (((pprop->IsArray()) && (refdim.CDim() == 0))
				|| ((!pprop->IsArray()) && (refdim.CDim() != 0)))
		{
			throw CVisSDIError("Array type mismatch",
					"CVisPropList::ReadPropValue", refsdistream.ILine());
		}
	}

	if (pprop == 0)
	{
		CVisPropList *pproplistUser = 0;
		if (pvObjOffset == 0)
		{
			// This isn't a class property list.
			pproplistUser = this;
		}
		else
		{
			// Since pvObjOffset != 0, we assume that this is a class property
			// list.

			// Check for the special VIS_SZ_SD_USER_PROP_LIST property that is
			// sometimes used to store usre-defined properties.
			CVisPropList::const_iterator iteratorPropList
						= IteratorFromPropName(VIS_SZ_SD_USER_PROP_LIST);

			if (iteratorPropList != end())
			{
				assert(TypeInfo(iteratorPropList) == typeid(CVisPropList));
				
				// The following code get a reference to the CVisPropList that
				// contains user-defined properties.
				pproplistUser = (CVisPropList *) (void *)
						(((BYTE *) (*iteratorPropList).PvObj())
								+ ((int) pvObjOffset));
			}
		}

		if (pproplistUser != 0)
		{
			CVisPropTypeInfoBase *pproptypeinfo
					= VisLookupPropTypeInfo(szType);

			bool fNeedToSetTypeName = false;
			if (pproptypeinfo == 0)
			{
				// The type was not recognized, so we'll use a CVisPropList
				// object to read the value.
				// LATER:  This code has not been tested.
#ifdef _DEBUG
		// LATER:  Use the CVisDiagnostics class to print the message?
		OutputDebugString("VisSDK Warning:  Type '");
		OutputDebugString(szType);
		OutputDebugString("' not recognized in CVisPropList::ReadPropValue.\n");
#endif // _DEBUG
				pproptypeinfo = VisLookupPropTypeInfo(typeid(CVisPropList));
				assert(pproptypeinfo != 0);
				fNeedToSetTypeName = true;
			}

			pprop = &(pproplistUser->PropListValueNew(szName));
			assert(pprop != 0);
			pprop->SetPPropTypeInfo(pproptypeinfo);
			pprop->SetDefaultValue(refdim, fShared);
			if (fNeedToSetTypeName)
				pprop->SetPropListTypeName(szType);

			// Change the offset used below when reading the prop value.
			pvObjOffset = 0;
		}
	}

	if (pprop != 0)
	{
		pprop->ReadValue(refsdistream, true, pvObjOffset);
	}
	else
	{
		// If there's no PropertyList for user-defined properties, we'll
		// just (print a Debug message and) ignore the property (by
		// skipping its value).
		// LATER:  We might want an option to throw an exception in this
		// case.
		// LATER: This code has not been tested.
#ifdef _DEBUG
		// LATER:  Use the CVisDiagnostics class to print the message?
		OutputDebugString("VisSDK Warning:  Unrecognized property '");
		OutputDebugString(szName);
		OutputDebugString("' in class '");
		// Since pvObjOffset != 0, we assume that this is a class property list
		// with a non-empty type name.
		assert(TypeName() != 0);
		OutputDebugString(TypeName());
		OutputDebugString("' ignored in CVisPropList::ReadPropValue.\n");
#endif // _DEBUG
		refsdistream.SkipValue();
		return;
	}
#endif // VIS_NO_SD_STREAM
}


void CVisPropList::ReadValue(CVisSDIStream& refsdistream,
		const void *pvObjOffset)
{
#ifndef VIS_NO_SD_STREAM
	const CVisSDITypedef *ptypedef;
	if (TypeName() == 0)
		ptypedef = 0;
	else
		ptypedef = refsdistream.PTypedef(TypeName());

	bool fUseGroup = ((ptypedef == 0) || (ptypedef->size() != 1));

	if (fUseGroup)
		refsdistream.OpenGroup();

	if (ptypedef != 0)
	{
		// Use typedef.
		CVisSDITypedef::const_iterator iteratorCur = ptypedef->begin();
		CVisSDITypedef::const_iterator iteratorLim = ptypedef->end();
		for (; iteratorCur != iteratorLim; ++iteratorCur)
		{
			ReadPropValue(iteratorCur->strType.c_str(),
					iteratorCur->strName.c_str(), iteratorCur->dim,
					iteratorCur->fShared, refsdistream, pvObjOffset);
		}
	}
	else
	{
		// No typedef for this class
		while (!refsdistream.AtEndOfGroup())
		{
			// Need to read type and name and then create storate for object.
			std::string strType;
			std::string strName;
			bool fShared;
			CVisDim dim;
			refsdistream.ReadTypeNameAndDim(strType, strName, dim, &fShared);
			assert(!strName.empty());
			ReadPropValue(strType.c_str(), strName.c_str(), dim, fShared,
					refsdistream, pvObjOffset);
			refsdistream.EndObj();
		}
	}

	if (fUseGroup)
		refsdistream.CloseGroup();
#endif // VIS_NO_SD_STREAM
}



#ifndef VIS_NO_SD_STREAM

void VisSDOFindTypes(CVisSDOStream& refsdostream,
		CVisPropList const& refobj)
{
	CVisPropList::const_iterator iteratorCur = refobj.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropList::const_iterator iteratorLim = refobj.end();
//	for (; iteratorCur != iteratorLim; ++iteratorCur)
	int iLim = refobj.CProp();
	for (int i = 0; i != iLim; ++iteratorCur, ++i)
	{
		(*iteratorCur).SDOFindTypes(refsdostream);
	}
}

void VisSDWriteObj(CVisSDOStream& refsdostream,
		CVisPropList const& refobj, const char *szName)
{
	VisSDOFindTypes(refsdostream, refobj);

	if (refobj.TypeName() == 0)
		refsdostream.StartObj(refobj, szName);
	else
		refsdostream.StartObjType(refobj.TypeName(), szName);
	VisSDWriteValue(refsdostream, refobj);
	refsdostream.EndObj();
}

void VisSDReadObj(CVisSDIStream& refsdistream,
		CVisPropList& refobj)
{
	VisSDIFindTypes(refsdistream, refobj);

	if (refobj.TypeName() == 0)
		refsdistream.StartObj(refobj);
	else
		refsdistream.StartObjType(refobj.TypeName());
	VisSDReadValue(refsdistream, refobj);
	refsdistream.EndObj();
}


VIS_DEFINE_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(VisPropListExportDeclspec, CVisPropList)
VIS_DEFINE_PROP_LIST_GET_PROP_TYPE_INFO(VisPropListExportDeclspec, CVisPropList)


VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisPropList)
VIS_DEFINE_SDO_WRITE_OP_DEFAULT(VisPropListExportDeclspec, CVisPropList)

VIS_DEFINE_SDI_FIND_TYPES_NOOP(VisPropListExportDeclspec, CVisPropList)
VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisPropList)
VIS_DEFINE_SDI_READ_OP_DEFAULT(VisPropListExportDeclspec, CVisPropList)



#endif // VIS_NO_SD_STREAM

