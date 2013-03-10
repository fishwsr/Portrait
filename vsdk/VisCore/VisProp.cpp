// VisProp.cpp
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



CVisProp::CVisProp(CVisPropTypeInfoBase *pproptypeinfo)
  : m_fInitialized(false),
	m_fShared(false),
	m_fPointerToObject(false),
	m_fPrint(true),
	m_fObjReference(false),
	m_fArray(false),
	m_llData(0.0),
	m_propkeydata(0),
	m_pproptypeinfo(0)
{
	// Call SetPPropTypeInfo to make sure that the CVisPropTypeInfoBase
	// information is read (eg to set the printing option)
	if (pproptypeinfo != 0)
		SetPPropTypeInfo(pproptypeinfo);
}

CVisProp::CVisProp(
		const CVisProp& refprop)
  : m_fInitialized(false),
	m_fShared(false),
	m_fPointerToObject(false),
	m_fPrint(refprop.m_fPrint),
	m_fObjReference(false),
	m_fArray(false),
	m_llData(0.0),
	m_propkeydata(refprop.m_propkeydata),
	m_pproptypeinfo(refprop.m_pproptypeinfo)
{
	Assign(refprop);
}

void CVisProp::SetSharing(bool f)
{
	assert(IsValid());
	if (f != IsShared())
	{
		if (!IsShared())
		{
			if ((FPointerToObject()) || (IsArray()))
			{
				// Just need to change the flag.
				m_fShared = f;
			}
			else
			{
				if (IsObjReference())
				{
					// Make a temporary copy of the pointer to the object
					SetObjReferenceFromPv(PvObj(), true);
				}
				else
				{
					__int64 llDataT = m_llData;
					SetObjFromPv((void *) &llDataT, true);
				}
			}
		}
		else
		{
			// LATER:  Have a special case for a shared object with only
			// one reference.  (In that case, we only need to change m_fShared.)
			CVisProp propT = *this;
			if (propT.IsArray())
			{
				if (propT.IsObjReference())
				{
					SetObjReferenceFromPv(propT.PRefCntArray()->PvObjFirst(),
							false, propT.Dim());
				}
				else
				{
					SetObjFromPv(propT.PRefCntArray()->PvObjFirst(),
							false, propT.Dim());
				}
			}
			else
			{
				if (propT.IsObjReference())
				{
					SetObjReferenceFromPv(propT.PvObj(), false);
				}
				else
				{
					SetObjFromPv(propT.PvObj(), false);
				}
			}
		}
	}
}

const char *CVisProp::TypeName(void) const
{
	const char *szTypeName = 0;

	assert(PropTypeInfo().TypeInfo() != typeid(CVisProp));
	if (PropTypeInfo().TypeInfo() == typeid(CVisPropList))
	{
		const CVisPropList *pproplist;
		if (IsArray())
		{
			pproplist = (const CVisPropList *) (PRefCntArray()->PvObjFirst());
		}
		else
		{
			pproplist = (const CVisPropList *) PvObj();
				
		}
		assert(pproplist != 0);
		szTypeName = pproplist->TypeName();
	}

	if (szTypeName == 0)
		szTypeName = PropTypeInfo().Name();

	assert(szTypeName != 0);
	return szTypeName;
}

void CVisProp::Assign(
		const CVisProp& refprop)
{
	bool fSameTypes = (m_pproptypeinfo == refprop.m_pproptypeinfo);
	m_pproptypeinfo = refprop.m_pproptypeinfo;
	m_propkeydata = refprop.m_propkeydata;
	m_fPrint = refprop.m_fPrint;

	if (!refprop.IsValid())
	{
		ReleaseObj();
	}
	else if ((IsValid()) && (IsArray()) && (!IsShared()) && (refprop.IsArray())
			&& (!refprop.IsShared()) && (Dim() == refprop.Dim()) && (fSameTypes))
	{
		// Special case:  Just copy the array values (without reallocating).
		CopyArrayElements(refprop.PRefCntArray()->PvObjFirst());
	}
	else
	{
		assert(m_pproptypeinfo != 0);

		if ((IsValid()) && (IsArray()))
			ReleaseObj();

		if (refprop.IsArray())
		{
			ReleaseObj();
			SetFObjReference(refprop.IsObjReference());
			if (refprop.IsShared())
			{
				SetPvRefCntArray(refprop.PRefCntArray());
				// Need to set m_fInitialized to true here so that we can
				// assert(IsValid()) in the call to PRefCntArray().
				SetFInitialized(true);
				PropTypeInfo().AddRefPvRefCntArray(PRefCntArray());
			}
			else
			{
				SetObjFromPv(refprop.PRefCntArray()->PvObjFirst(),
						false, refprop.Dim());
			}
		}
		else if (refprop.IsObjReference())
		{
			ReleaseObj();
			SetFObjReference(true);
			if (refprop.IsShared())
			{
				SetPvRefCntObj(refprop.PvRefCntObj());
				// Need to set m_fInitialized to true here so that we can
				// assert(IsValid()) in the call to PvRefCntObj().
				SetFInitialized(true);
				((CVisRefCntObj<void *> *) PvRefCntObj())->AddRef();
			}
			else
			{
				SetFPointerToObject(false);
				*((void * *) (void *) &m_llData) = refprop.PvObj();
			}
		}
		else
		{
			m_fObjReference = false;
			if (!refprop.FPointerToObject())
			{
				ReleaseObj();
				SetFPointerToObject(false);
//				m_pproptypeinfo->AssignObjToObj(refprop.PvObj(), PvObj());
				m_pproptypeinfo->AssignObjToObj(refprop.PvObj(), (void *) &m_llData);
			}
			else if (!refprop.IsShared())
			{
				if (!IsValid())
				{
					SetPvRefCntObj(m_pproptypeinfo->ClonePvRefCntObj(refprop.PvRefCntObj()));
				}
				else
				{
					assert(FPointerToObject());
					m_pproptypeinfo->AssignObjToObj(refprop.PvObj(), PvObj());
				}
			}
			else
			{
				SetPvRefCntObj(refprop.PvRefCntObj());
				// Need to set m_fInitialized to true here so that we can
				// assert(IsValid()) in the call to PvRefCntObj().
				SetFInitialized(true);
				m_pproptypeinfo->AddRefPvRefCntObj(PvRefCntObj());
			}
		}

		SetFShared(refprop.IsShared());
		SetFInitialized(true);
	}
}

bool CVisProp::FSharedDefault(void) const
{
	if (IsValid())
		return IsShared();
	
	return !(PropTypeInfo().CanCopyObjectBytes());
}

void *CVisProp::PvObj(void) const
{
	assert(IsValid());
	assert(!IsArray());

	void *pvObj = (void *) &m_llData;

	if (FPointerToObject())
		pvObj = *((void * *) pvObj);

	if (IsObjReference())
		pvObj = *((void * *) pvObj);
	
	return pvObj;
}


void CVisProp::OffsetReference(int ibOffset)
{
	assert(IsObjReference());
	assert(IsValid());

	void *pvObj = (void *) &m_llData;

	if (FPointerToObject())
		pvObj = *((void * *) pvObj);

	*((void * *) pvObj)
			= (void *) (((BYTE *) *((void * *) pvObj)) + ibOffset);
}

void CVisProp::SetObjFromPv(const void *pvObj, bool fShared)
{
	assert(pvObj != 0);
	assert(!IsValid() || !FPointerToObject() || PvRefCntObj() != 0);

	const CVisPropTypeInfoBase *pproptypeinfo = &PropTypeInfo();

	if (IsValid() && (IsObjReference() || IsArray()))
		ReleaseObj();
	SetFObjReference(false);

	if ((IsValid()) && (IsShared() == fShared))
	{
		pproptypeinfo->AssignObjToObj(pvObj, PvObj());
	}
	else
	{
		if (IsValid())
			ReleaseObj();

		SetFShared(fShared);
		SetFPointerToObject(fShared || !pproptypeinfo->CanCopyObjectBytes());

		if (!FPointerToObject())
		{
			SetFInitialized(true);
			pproptypeinfo->AssignObjToObj(pvObj, PvObj());
		}
		else
		{
			SetPvRefCntObj(pproptypeinfo->PvRefCntObjMakePvObj(pvObj));
			SetFInitialized(true);
		}
	}
}

void CVisProp::SetObjReferenceFromPv(void *pvObj, bool fShared)
{
	if (IsValid() && IsArray())
		ReleaseObj();

	assert(!IsValid() || !FPointerToObject() || PvRefCntObj() != 0);

	if ((IsValid()) && (IsShared() == fShared) && (IsObjReference()))
	{
		((CVisRefCntObj<void *> *) PvRefCntObj())->Obj() = pvObj;
	}
	else
	{
		ReleaseObj();
		SetFShared(fShared);
		SetFPointerToObject(fShared);

		if (fShared)
		{
			SetPvRefCntObj(new CVisRefCntObj<void *>(pvObj));
		}
		else
		{
			*((void * *) (void *) &m_llData) = pvObj;
		}

		SetFObjReference(true);
		SetFInitialized(true);
	}
}

void CVisProp::SetPvRefCntObj(void *pvRefCntObj)
{
	ReleaseObj();
	assert(pvRefCntObj != 0);
	SetFPointerToObject(true);

	*((void * *) (void *) &m_llData) = pvRefCntObj;
}

void CVisProp::SetObjFromPv(const void *pvObj,
		const CVisDimIndex& refdimindex)
{
	assert(IsArray());
	PropTypeInfo().AssignObjToObj(pvObj,
			PRefCntArray()->PvObj(refdimindex));
}

void CVisProp::SetObjFromPv(const void *pvObj, bool fShared,
		const CVisDim& refdim)
{
	// SetPvRefCntArray calls ReleaseObj()
	SetPvRefCntArray(PropTypeInfo().PRefCntArrayMake(refdim));
	SetFShared(fShared);
	SetFObjReference(false);
	SetFInitialized(true);

	// Copy the array elements.
	CopyArrayElements((const CVisRefCntObjArrayBase *) pvObj);
}

void CVisProp::SetObjReferenceFromPv(void *pvObj, bool fShared,
		const CVisDim& refdim)
{
	// SetPvRefCntArray calls ReleaseObj()
	SetPvRefCntArray(PropTypeInfo().PRefCntArrayMake(refdim, pvObj));
	SetFShared(fShared);
	SetFObjReference(true);
	SetFInitialized(true);
}

void CVisProp::SetPvRefCntArray(void *pvRefCntArray)
{
	ReleaseObj();
	assert(pvRefCntArray != 0);
	SetFPointerToObject(true);
	SetFArray(true);

	*((void * *) (void *) &m_llData) = pvRefCntArray;
}

void CVisProp::CopyArrayElements(const void *pvArrayElements)
{
	const int iLim = Dim().CObj();
	const int cbObj = PRefCntArray()->CbObj();
	const BYTE *pbSrc = (const BYTE *) pvArrayElements;
	BYTE *pbDest = (BYTE *) PRefCntArray()->PvObjFirst();
	for (int i = 0; i < iLim; ++i, pbSrc += cbObj, pbDest += cbObj)
	{
		PropTypeInfo().AssignObjToObj((const void *) pbSrc,
				(void *) pbDest);
	}
}

void CVisProp::ReleaseObj(void)
{
	if (IsValid())
	{
		if (IsArray())
		{
			PropTypeInfo().ReleasePvRefCntArray(PRefCntArray());
		}
		else if (FPointerToObject())
		{
			if (IsObjReference())
				((CVisRefCntObj<void *> *) PvRefCntObj())->Release();
			else
				PropTypeInfo().ReleasePvRefCntObj(PvRefCntObj());
		}
	}
	ClearValue();
	m_fArray = false;
	SetFInitialized(false);
}

void CVisProp::WriteValue(CVisSDOStream& refsdostream,
		bool fOnlyValue, const void *pvReferenceOffset) const
{
	assert(IsValid());
	if (IsPrinted())
	{
#ifndef VIS_NO_SD_STREAM
		if ((!fOnlyValue) && (IsShared()))
				refsdostream.WriteSharedPrefix();

		if (IsArray())
		{
			void *pvObjFirst = PRefCntArray()->PvObjFirst();
			if ((pvReferenceOffset != 0) && (IsObjReference()))
			{
				pvObjFirst = (void *)
						(((BYTE *) pvObjFirst) + ((int) pvReferenceOffset));
			}

			m_pproptypeinfo->WriteObjArray(refsdostream, Dim(),
					pvObjFirst, Name(), fOnlyValue);
		}
		else
		{
			void *pvObj = PvObj();
			if ((pvReferenceOffset != 0) && (IsObjReference()))
			{
				pvObj = (void *)
						(((BYTE *) pvObj) + ((int) pvReferenceOffset));
			}

			m_pproptypeinfo->WriteObj(refsdostream, pvObj, Name(),
					fOnlyValue);
		}
#endif // VIS_NO_SD_STREAM
	}
}

void CVisProp::SetTypeNameAndDim(const char *szType,
		const char *szName, const CVisDim& refdim)
{
	SetTypeNameAndDim(szType, szName, refdim, FSharedDefault());
}

void CVisProp::SetTypeNameAndDim(const char *szType,
		const char *szName, const CVisDim& refdim, bool fShared)
{
	CVisPropTypeInfoBase *pproptypeinfo = VisLookupPropTypeInfo(szType);

	bool fNeedToSetTypeName = false;
	if (pproptypeinfo == 0)
	{
		// The type was not recognized, so we'll use a CVisPropList
		// object to read the value.
		// LATER:  This code has not been tested.
#ifdef _DEBUG
		OutputDebugString("VisSDK Warning:  Type '");
		OutputDebugString(szType);
		OutputDebugString("' not recognized in CVisProp::SetTypeNameAndDim.\n");
#endif // _DEBUG
		pproptypeinfo = VisLookupPropTypeInfo(typeid(CVisPropList));
		assert(pproptypeinfo != 0);
		fNeedToSetTypeName = true;
	}

	ReleaseObj();
	SetKeyData(CVisPropKeyData(szName));
	SetPPropTypeInfo(pproptypeinfo);

	// Make a new object to store the value.
	SetDefaultValue(refdim, fShared);
	if (fNeedToSetTypeName)
		SetPropListTypeName(szType);
}

void CVisProp::SetPropListTypeName(const char *szType)
{
	assert(TypeInfo() == typeid(CVisPropList));
	if (IsArray())
	{
		CVisPropList *pproplistCur
				= (CVisPropList *) (PRefCntArray()->PvObjFirst());
		CVisPropList *pproplistLim
				= pproplistCur +  PRefCntArray()->Dim().CObj();
		for (; pproplistCur < pproplistLim; ++pproplistCur)
			pproplistCur->SetTypeName(szType);
	}
	else
	{
		((CVisPropList *) PvObj())->SetTypeName(szType);
	}
}

void CVisProp::SetDefaultValue(const CVisDim& refdim, bool fShared)
{
	ReleaseObj();
	SetFObjReference(false);
	SetFShared(fShared);
	SetFPointerToObject(fShared || !(PropTypeInfo().CanCopyObjectBytes()));
	if (refdim.CObj() > 1)
		SetPvRefCntArray(PropTypeInfo().PRefCntArrayMake(refdim));
	else if (!FPointerToObject())
		ClearValue();
	else
		SetPvRefCntObj(PropTypeInfo().PvRefCntObjMakePvObj(0));
	SetFInitialized(true);
}

void CVisProp::ReadValue(CVisSDIStream& refsdistream, bool fOnlyValue,
		const void *pvReferenceOffset)
{
	if (fOnlyValue)
	{
		// We assume that the type and name have already been read and that
		// storage for the object has been allocated.
		assert(IsValid());
		if (IsArray())
		{
			void *pvObjFirst = PRefCntArray()->PvObjFirst();
			if ((pvReferenceOffset != 0) && (IsObjReference()))
			{
				pvObjFirst = (void *)
						(((BYTE *) pvObjFirst) + ((int) pvReferenceOffset));
			}

			m_pproptypeinfo->ReadObjArray(refsdistream, Dim(),
					pvObjFirst);
		}
		else
		{
			void *pvObj = PvObj();
			if ((pvReferenceOffset != 0) && (IsObjReference()))
			{
				pvObj = (void *)
						(((BYTE *) pvObj) + ((int) pvReferenceOffset));
			}

			m_pproptypeinfo->ReadObj(refsdistream, pvObj);
		}
	}
	else
	{
#ifndef VIS_NO_SD_STREAM
		// Need to read type and name and then create storate for object.
		std::string strType;
		std::string strName;
		bool fShared;
		CVisDim dim;
		refsdistream.ReadTypeNameAndDim(strType, strName, dim, &fShared);
		SetTypeNameAndDim(strType.c_str(), strName.c_str(), dim, fShared);
		ReadValue(refsdistream, true, pvReferenceOffset);
		refsdistream.EndObj();
#endif // VIS_NO_SD_STREAM
	}
}


void CVisProp::SDOFindTypes(CVisSDOStream& refsdostream,
		const void *pvReferenceOffset) const
{
	assert(IsValid());
	assert((pvReferenceOffset == 0) || (IsObjReference()));
	if (IsArray())
	{
		void *pvObjFirst = PRefCntArray()->PvObjFirst();
		if ((pvReferenceOffset != 0) && (IsObjReference()))
		{
			pvObjFirst = (void *)
					(((BYTE *) pvObjFirst) + ((int) pvReferenceOffset));
		}

		m_pproptypeinfo->SDOFindTypes(refsdostream, pvObjFirst,
				PRefCntArray()->Dim());
	}
	else
	{
		void *pvObj = PvObj();
		if ((pvReferenceOffset != 0) && (IsObjReference()))
		{
			pvObj = (void *)
					(((BYTE *) pvObj) + ((int) pvReferenceOffset));
		}

		m_pproptypeinfo->SDOFindTypes(refsdostream, pvObj);
	}
}



VisPropListExport void VisSDOFindTypes(CVisSDOStream& refsdostream,
		CVisProp const& refobj)
	{ refobj.SDOFindTypes(refsdostream); }

VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisProp)


VisPropListExport void VisSDWriteObj(CVisSDOStream& refsdostream,
		CVisProp const& refobj, const char *szName)
{
	VisSDOFindTypes(refsdostream, refobj);
	// Call WriteValue to write the contained object to the stream.
	// (The object isn't  marked as being contained in a CVisProp object.)
	refobj.WriteValue(refsdostream);
}

VIS_DEFINE_SDO_WRITE_OP_DEFAULT(VisPropListExportDeclspec, CVisProp)


VisPropListExport void VisSDIFindTypes(CVisSDIStream& refsdistream,
		CVisProp const& refobj)
{
	if (refobj.IsValid())
	{
		// We only need to find type information about top-level objects, so
		// we don't need to pass a pvObj to SDIFindTypes.
		refobj.PropTypeInfo().SDIFindTypes(refsdistream);
	}
}

VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(VisPropListExportDeclspec, CVisProp)

//VIS_DEFAULT_SDREADOBJ(CVisProp)
VisPropListExport void VisSDReadObj(CVisSDIStream& refsdistream,
		CVisProp& refobj, const char *szName)
{
	VisSDIFindTypes(refsdistream, refobj);
	refobj.ReadValue(refsdistream);
}

// But operator<< should just write the value (and not say that the value
// was stored in a CVisProp object).
VisPropListExport CVisSDIStream& operator>>(CVisSDIStream& refsdistream,
		CVisProp& refobj)
{
	VisSDIFindTypes(refsdistream, refobj);
	refobj.ReadValue(refsdistream);
	return refsdistream;
}
