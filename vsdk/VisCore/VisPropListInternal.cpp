// VisPropListInternal.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved



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


CVisPropTypeInfoMap::CVisPropTypeInfoMap(void)
  : m_visproptypeinfomapName(CVisFnOpCmpSz()),
	m_visproptypeinfomapTypeInfo(CVisFnOpCmpPTypeInfo())
{
	// LATER:  Add more standard types to the list?  (Or Windows types, like RECT?)
	// or standard pointer types (eg "int *")?

	// LATER:  Call the VisPPropTypeInfoNewForClass function to get the
	// CVisPropTypeInfo objects?  (Or should we make VisPPropTypeInfoNewForClass
	// assert(0), since it should never be called for these classes if we
	// create their PropTypeInfo here.

	// Standard numeric types
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<char>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<signed char>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<unsigned char>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<short>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<unsigned short>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<int>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<unsigned int>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<long>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<unsigned long>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<__int64>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<unsigned __int64>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<float>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<double>(true));

	// Other standard types
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<bool>(true));
	AddPropTypeInfo(new CVisPropTypeInfoWO<char *>("string"));
	AddPropTypeInfo(new CVisPropTypeInfoWO<char const *>("string"));
	AddPropTypeInfo(new CVisPropTypeInfoWithIO<std::string>("string"));

	// Add void * as a non-printing type.
	AddPropTypeInfo(new CVisPropTypeInfoNoIO<void *>);
	AddPropTypeInfo(new CVisPropTypeInfoNoIO<void const *>);

	// CVisPropList
	CVisPropTypeInfoBase *pproptypeinfoT = new CVisPropTypeInfoWithIO<CVisPropList>;
	pproptypeinfoT->SetFContainsPropLists(true);
	AddPropTypeInfo(pproptypeinfoT);

	// Windows types
//	AddPropTypeInfo(new CVisPropTypeInfoWithIO<RECT>);

}



CVisPropKeyData::CVisPropKeyData(const char *szKeyName)
  : m_pchBuf(0)
{
	if (szKeyName != 0)
	{
		m_pchBuf = new char[strlen(szKeyName) + 1 + sizeof(ULONG)];
		CRefKey() = 1;
		strcpy(m_pchBuf + sizeof(ULONG), szKeyName);
	}
}

CVisPropKeyData::CVisPropKeyData(const CVisPropKeyData& refpropkeydata)
  : m_pchBuf(refpropkeydata.m_pchBuf)
{
	AddRefKey();
}

CVisPropKeyData& CVisPropKeyData::operator=(const CVisPropKeyData& refpropkeydata)
{
	if (m_pchBuf != refpropkeydata.m_pchBuf)
	{
		ReleaseKey();
		m_pchBuf = refpropkeydata.m_pchBuf;
		AddRefKey();
	}

	return *this;
}

ULONG CVisPropKeyData::AddRefKey(void)
{
	if (m_pchBuf == 0)
		return 1;

	return (ULONG) InterlockedIncrement((LONG *) &CRefKey());
}

ULONG CVisPropKeyData::ReleaseKey(void)
{
	if (m_pchBuf == 0)
		return 1;

	ULONG cRef = (ULONG) InterlockedDecrement((LONG *) &CRefKey());

	if (cRef == 0)
		delete m_pchBuf;

	return cRef;
}

CVisPropTypeInfoMap::~CVisPropTypeInfoMap(void)
{
	CleanupPropTypeInfo();
}

void CVisPropTypeInfoMap::AddPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo)
{
	assert(pproptypeinfo != 0);

	// Type should be unique
	assert(m_visproptypeinfomapTypeInfo[&(pproptypeinfo->TypeInfo())] == 0);
	m_visproptypeinfomapTypeInfo[&(pproptypeinfo->TypeInfo())] = pproptypeinfo;
	
	// Name may not be unique (eg "std::string" and "char *" will both be called "string")
	m_visproptypeinfomapName[pproptypeinfo->Name()] = pproptypeinfo;
}

CVisPropTypeInfoBase *CVisPropTypeInfoMap::LookupPropTypeInfo(const char *szName)
{
	CVisPropTypeInfoMapName::iterator iteratorT;
	iteratorT = m_visproptypeinfomapName.find(szName);
	if (iteratorT == m_visproptypeinfomapName.end())
		return 0;

	return iteratorT->second;
}

CVisPropTypeInfoBase *CVisPropTypeInfoMap::LookupPropTypeInfo(const type_info& reftypeinfo)
{
	CVisPropTypeInfoMapTypeInfo::iterator iteratorT;
	iteratorT = m_visproptypeinfomapTypeInfo.find(&reftypeinfo);
	if (iteratorT == m_visproptypeinfomapTypeInfo.end())
		return 0;

	return iteratorT->second;
}

void CVisPropTypeInfoMap::ChangeName(CVisPropTypeInfoBase *pproptypeinfo,
		const char *szName)
{
	assert(pproptypeinfo != 0);

	CVisPropTypeInfoMapName::iterator iteratorT;
	iteratorT = m_visproptypeinfomapName.find(pproptypeinfo->Name());
	bool fInMap = ((iteratorT != m_visproptypeinfomapName.end())
			&& (iteratorT->second == pproptypeinfo));
	if (fInMap)
	{
		m_visproptypeinfomapName.erase(pproptypeinfo->Name());
	}

	pproptypeinfo->ChangeName(szName);

	if (fInMap)
		m_visproptypeinfomapName[pproptypeinfo->Name()] = pproptypeinfo;
}

void CVisPropTypeInfoMap::CleanupPropTypeInfo(void)
{
	CVisPropTypeInfoMapTypeInfo::iterator iteratorMap
			= m_visproptypeinfomapTypeInfo.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropTypeInfoMapTypeInfo::iterator iteratorMapLim = m_visproptypeinfomapTypeInfo.end();
//	for (; iteratorMap != iteratorMapLim; ++iteratorMap)
	int iLim = m_visproptypeinfomapTypeInfo.size();
	for (int i = 0; i != iLim; ++iteratorMap, ++i)
		iteratorMap->second->ClearPropList();

	iteratorMap = m_visproptypeinfomapTypeInfo.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropTypeInfoMapTypeInfo::iterator iteratorMapLim = m_visproptypeinfomapTypeInfo.end();
//	for (; iteratorMap != iteratorMapLim; ++iteratorMap)
	for (i = 0; i != iLim; ++iteratorMap, ++i)
		iteratorMap->second->Release();
	m_visproptypeinfomapName.clear();
	m_visproptypeinfomapTypeInfo.clear();
}

#ifdef _DEBUG
void CVisPropTypeInfoMap::PrintKnownTypes(bool fIncludeTypeIDNames) const
{
	printf("\nKnown property type information:\n");
	CVisPropTypeInfoMapTypeInfo::const_iterator iteratorMap = m_visproptypeinfomapTypeInfo.begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropTypeInfoMapTypeInfo::const_iterator iteratorMapLim = m_visproptypeinfomapTypeInfo.end();
//	for (; iteratorMap != iteratorMapLim; ++iteratorMap)
	int iLim = m_visproptypeinfomapTypeInfo.size();
	for (int i = 0; i != iLim; ++iteratorMap, ++i)
	{
		printf("    %s", iteratorMap->second->Name());
		if (fIncludeTypeIDNames)
			printf("    ( %s )", iteratorMap->second->TypeInfo().name());
		printf("\n");
	}
	printf("\n");
}
#endif // _DEBUG

VisPropListExport void VisAddPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo)
{
	PGlobals()->RPropTypeInfoMap().AddPropTypeInfo(pproptypeinfo);
}

VisPropListExport CVisPropTypeInfoBase *VisLookupPropTypeInfo(const char *szName)
{
	return PGlobals()->RPropTypeInfoMap().LookupPropTypeInfo(szName);
}

VisPropListExport CVisPropTypeInfoBase *VisLookupPropTypeInfo(const type_info& reftypeinfo)
{
	return PGlobals()->RPropTypeInfoMap().LookupPropTypeInfo(reftypeinfo);
}

#ifdef _DEBUG
VisPropListExport void VisPrintPropTypeInfoForDebugging(bool fIncludeTypeIDNames)
{
	PGlobals()->RPropTypeInfoMap().PrintKnownTypes(fIncludeTypeIDNames);
}
#endif // _DEBUG



CVisPropTypeInfoBase::CVisPropTypeInfoBase(const char *szName,
		bool fCanCopyObjectBytes, bool fCanPrint)
  : m_strName(szName),
	m_cRef(1),
	m_fCanCopyObjectBytes(fCanCopyObjectBytes),
	m_fPrint(fCanPrint),
	m_fCanPrint(fCanPrint),
	m_fSimpleType(false),
	m_fChangedName(false),
	m_fContainsPropLists(false),
	m_fCheckedForPropLists(false)
{
	assert((m_fCanPrint) || (!m_fPrint));

	FilterName(m_strName);
}


ULONG CVisPropTypeInfoBase::AddRef(void)
{
	return (ULONG) InterlockedIncrement((LONG *) &m_cRef);
}

ULONG CVisPropTypeInfoBase::Release(void)
{
	ULONG cRef = (ULONG) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}

void CVisPropTypeInfoBase::FilterName(std::string& refstrName)
{
	// Remove "class ", "struct ", and extra whitespace from full name.
	// (Keep whitespace following the "unsigned" keyword.)
	std::string::size_type ich;
	std::string::size_type ichNPos = std::string::npos;
	while ((ich = refstrName.find("class ")) != ichNPos)
		refstrName.erase(ich, 6);  // 6 chars in "class "

	// LATER:  Do we want to remove "struct tag" to match common Windows type names?
	// (This could cause name conflicts in rare cases.)
	while ((ich = refstrName.find("struct tag")) != ichNPos)
		refstrName.erase(ich, 10);  // 10 chars in "struct tag"

	while ((ich = refstrName.find("struct ")) != ichNPos)
		refstrName.erase(ich, 7);  // 7 chars in "struct "
	
	
	// UNDONE:  Make sure that this is consistent with the strings
	// used for vector and deque names (and the string for the list of
	// images in a sequence).
	while ((ich = refstrName.find(" >")) != ichNPos)
		refstrName.erase(ich, 1);  // ' ' char in " >"

	// LATER:  Add code to remove std::allocator<T> from std::vector and std::deque
	// definitions.
}

void CVisPropTypeInfoBase::SetName(const char *szName)
{
	assert(szName != 0);
	assert(*szName != 0);
	
	if (m_strName.compare(szName) != 0)
		PGlobals()->RPropTypeInfoMap().ChangeName(this, szName);
}

void CVisPropTypeInfoBase::ChangeName(const char *szName)
{
	m_strName = szName;
	m_fChangedName = true;
}

void CVisPropTypeInfoBase::ClearPropList(void)
{
}

bool CVisPropTypeInfoBase::FContainsPropLists(void)
{
	if (!m_fCheckedForPropLists)
	{
		if (PPropList() != 0)
		{
			CVisPropList::const_iterator iteratorCur = PPropList()->begin();
//			We could use iteratorMapLim instead of looping from 0 to size() below.
//			CVisPropList::const_iterator iteratorLim = PPropList()->end();
//			for (; iteratorCur != iteratorLim; ++iteratorCur)
			int iLim = PPropList()->CProp();
			for (int i = 0; i != iLim; ++iteratorCur, ++i)
			{
				if (PPropList()->PropTypeInfo(iteratorCur).FContainsPropLists())
				{
					SetFContainsPropLists(true);
					break;
				}
			}
		}

		m_fCheckedForPropLists = true;
	}

	return m_fContainsPropLists;
}


CVisRefCntObjArrayBase::~CVisRefCntObjArrayBase(void)
{
}

const CVisDim& CVisRefCntObjArrayBase::Dim(void) const
{
	return m_dim;
}

int CVisRefCntObjArrayBase::CbObj(void) const
{
	return m_cbObj;
}

void *CVisRefCntObjArrayBase::PvObjFirst(void) const
{
	return m_rgvObj;
}


void *CVisRefCntObjArrayBase::PvObj(const CVisDimIndex& refdimindex) const
{
	int iObj = 0;
	if (m_dim.CDim() > 0)
	{
		for (int iDim = 0; iDim < m_dim.CDim() - 1; ++iDim)
		{
			assert(refdimindex[iDim] >= 0);
			assert(refdimindex[iDim] < m_dim[iDim]);
			iObj = iObj * m_dim[iDim + 1] + refdimindex[iDim];
		}
		assert(refdimindex[iDim] >= 0);
		assert(refdimindex[iDim] < m_dim[iDim]);
		iObj += refdimindex[iDim];
	}

	return (void *) (((BYTE *) m_rgvObj) + iObj * m_cbObj);
}

CVisRefCntObjArrayBase::CVisRefCntObjArrayBase(int cbObj, bool fAllocated, const CVisDim& refdim,
		void *pvWhere)
  : m_rgvObj(pvWhere),
	m_cbObj(cbObj),
	m_dim(refdim),
	m_fAllocated(fAllocated)
{
}

CVisRefCntObjArrayBase::CVisRefCntObjArrayBase(const CVisRefCntObjArrayBase& refrefcntobj)
  : m_cbObj(0)
{
	// Copy constructor should not be called
	assert(0);
}

CVisRefCntObjArrayBase& CVisRefCntObjArrayBase::operator=(
		const CVisRefCntObjArrayBase& refrefcntobj)
{
	// Assignment operator should not be called
	assert(0);

	return *this;
}

bool CVisRefCntObjArrayBase::FAllocated(void) const
{
	return m_fAllocated;
}



// Specialized PropList helper functions for common types



