// VisProp.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved

#pragma once
class CVisProp
{
public:
	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	VisPropListExport CVisProp(CVisPropTypeInfoBase *pproptypeinfo = 0);
	VisPropListExport CVisProp(const CVisProp& refprop);
	CVisProp& operator=(const CVisProp& refprop);
	~CVisProp(void);


	//------------------------------------------------------------------
	// Attributes

	bool IsValid(void) const;
	bool IsShared(void) const;
	bool IsPrinted(void) const;
	bool IsObjReference(void) const;
	bool IsArray(void) const;

	// It is an error to call Dim() if IsArray() is not true.
	const CVisDim& Dim(void) const;

	VisPropListExport void SetSharing(bool f);
	void SetPrinting(bool f);


    //------------------------------------------------------------------
	// Type

	VisPropListExport const char *TypeName(void) const;
	const type_info& TypeInfo(void) const;
	CVisPropTypeInfoBase& PropTypeInfo(void) const;


    //------------------------------------------------------------------
	// Name

	const char *Name(void) const;


    //------------------------------------------------------------------
	// (Scalar) Value

	template<class TObject>
	void GetObj(TObject& refobj) const
		{ GetObjInternal(refobj); }


	template<class TObject>
	void SetObj(const TObject& refobj)
		{ SetObjInternal(refobj, FSharedDefault()); }

	template<class TObject>
	void SetObj(const TObject& refobj, bool fShared)
		{ SetObjInternal(refobj, fShared); }

	template<class TObject>
	void SetObjReference(TObject& refobj, bool fShared = false)
		{ SetObjRefInternal(refobj, fShared); }


    //------------------------------------------------------------------
	// Array Value

	template<class TObject>
	void GetObj(TObject& refobj, const CVisDimIndex& refdimindex) const
		{ GetObjInternal(refobj, refdimindex); }


	template<class TObject>
	void SetObj(const TObject& refobj, const CVisDimIndex& refdimindex)
		{ SetObjInternal(refobj, refdimindex); }


	template<class TObject>
	void SetObj(const TObject& refobj, const CVisDim& refdim,
			bool fShared = true)
		{ SetObjInternal(refobj, refdim, fShared); }

	template<class TObject>
	void SetObjReference(TObject& refobj, const CVisDim& refdim,
			bool fShared = true)
		{ SetObjRefInternal(refobj, refdim, fShared); }


    //------------------------------------------------------------------
	// SDStream I / O

	VisPropListExport void WriteValue(CVisSDOStream& refsdostream,
			bool fOnlyValue = false, const void *pvReferenceOffset = 0) const;

	VisPropListExport void ReadValue(CVisSDIStream& refsdistream,
			bool fOnlyValue = false, const void *pvReferenceOffset = 0);

	VisPropListExport void SDOFindTypes(CVisSDOStream& refsdostream,
			const void *pvReferenceOffset = 0) const;


    //------------------------------------------------------------------
	// Misc

	// This is used internally.
	VisPropListExport void SetDefaultValue(const CVisDim& refdim, bool fShared);
	

protected:
	friend class CVisPropList;
#ifndef VIS_NO_SD_STREAM
	friend void CVisSDOStream::WriteClassPropList(CVisPropList& refproplist,
			const void *pvObjOffset, const CVisSDOTypedef *ptypedef);
	friend void CVisSDOStream::UseTypedef(CVisPropTypeInfoBase *pproptypeinfo,
			const CVisSDOTypedef& reftypedef);
#endif // VIS_NO_SD_STREAM


	// LATER:  Remove these if we change the prop list to use a vector
	// instead of a map.
	const CVisPropKeyData& KeyData(void) const;
	void SetKeyData(const CVisPropKeyData& refkeydata);

	VisPropListExport void SetTypeNameAndDim(const char *szType,
			const char *szName, const CVisDim& refdim);
	VisPropListExport void SetTypeNameAndDim(const char *szType,
			const char *szName, const CVisDim& refdim, bool fShared);

	VisPropListExport bool FSharedDefault(void) const;


	void ClearValue(void);
	VisPropListExportDeclspec void ReleaseObj(void);
	
	VisPropListExport void Assign(const CVisProp& refproplistvaluebase);
	
	bool FPointerToObject(void) const;

	void SetFInitialized(bool f);
	void SetFShared(bool f);
	void SetFPointerToObject(bool f);
	void SetFObjReference(bool f);
	void SetFArray(bool f);

	void SetPPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo);

	VisPropListExport void SetPropListTypeName(const char *szType);


	// Setting scalar values
    VisPropListExport void *PvObj(void) const;

	VisPropListExport void SetObjFromPv(const void *pvObj, bool fShared);

	VisPropListExport void SetObjReferenceFromPv(void *pvObj, bool fShared);

	void *PvRefCntObj(void) const;
	VisPropListExport void SetPvRefCntObj(void *pvRefCntObj);


    // Setting array values
	void *PvObj(const CVisDimIndex& refdimindex) const;

	VisPropListExport void SetObjFromPv(const void *pvObj,
			const CVisDimIndex& refdimindex);

	VisPropListExport void SetObjFromPv(const void *pvObj, bool fShared,
			const CVisDim& refdim);

	VisPropListExport void SetObjReferenceFromPv(void *pvObj, bool fShared,
			const CVisDim& refdim);

	CVisRefCntObjArrayBase *PRefCntArray(void) const;
	VisPropListExport void SetPvRefCntArray(void *pvRefCntArray);

	VisPropListExport void CopyArrayElements(const void *pvArrayElements);



	template<class TObject>
	void GetObjInternal(TObject& refobj) const
	{
		assert(IsValid());
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		refobj = *((const TObject *) PvObj());
	}


	template<class TObject>
	void SetObjInternal(const TObject& refobj, bool fShared)
	{
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		SetObjFromPv((const void *) &refobj, fShared);
	}

	template<class TObject>
	void SetObjRefInternal(TObject& refobj, bool fShared)
	{
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		SetObjReferenceFromPv((void *) &refobj, fShared);
	}


	template<class TObject>
	void GetObjInternal(TObject& refobj, const CVisDimIndex& refdimindex) const
	{
		assert(IsValid());
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		refobj = *((const TObject *) PvObj(refdimindex));
	}


	template<class TObject>
	void SetObjInternal(const TObject& refobj, const CVisDimIndex& refdimindex)
	{
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		SetObjFromPv((const void *) &refobj, refdimindex);
	}


	template<class TObject>
	void SetObjInternal(const TObject& refobj, const CVisDim& refdim,
			bool fShared)
	{
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		SetObjFromPv((const void *) &refobj, fShared, refdim);
	}

	template<class TObject>
	void SetObjRefInternal(TObject& refobj, const CVisDim& refdim,
			bool fShared)
	{
		assert(&PropTypeInfo() == VisPPropTypeInfoForClass(refobj));
		SetObjReferenceFromPv((void *) &refobj, fShared, refdim);
	}


    // Special helper function used internally.
    VisPropListExport void OffsetReference(int ibOffset);


private:
	// Use an 8-byte type here in case the compiler needs to align 8-byte data.
	__int64 m_llData;

	CVisPropKeyData m_propkeydata;

	CVisPropTypeInfoBase *m_pproptypeinfo;

	bool m_fInitialized;
	bool m_fShared;
	bool m_fPointerToObject;
	bool m_fPrint;
	bool m_fObjReference;
	bool m_fArray;
};


VisPropListExport CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		CVisProp const& refobj);


VisPropListExport void VisSDIFindTypes(CVisSDIStream& refsdistream,
		CVisProp const& refobj);
VisPropListExport void VisSDReadValue(CVisSDIStream& refsdistream,
		CVisProp& refobj);
VisPropListExport void VisSDReadObj(CVisSDIStream& refsdistream,
		CVisProp& refobj);
VisPropListExport CVisSDIStream& operator>>(CVisSDIStream& refsdistream,
		CVisProp& refobj);

VisPropListExport void VisSDOFindTypes(CVisSDOStream& refsdostream,
		CVisProp const& refobj);
VisPropListExport void VisSDWriteValue(CVisSDOStream& refsdostream,
		const CVisProp& refobj);
VisPropListExport void VisSDWriteObj(CVisSDOStream& refsdostream,
		const CVisProp& refobj);
VisPropListExport CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		const CVisProp& refobj);
