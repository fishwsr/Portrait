// VisPropListInternal.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#include <string>

#include <map>


// Forward declarations
class CVisPropList;

#pragma once

class CVisFnOpCmpSz
{
public:
	bool operator()(const char *sz1, const char *sz2) const
	{
		assert(sz1 != 0);
		assert(sz2 != 0);
//		return ((sz2 != 0)
//				&& ((sz1 == 0) || (strcmp(sz1, sz2) < 0)));
		return (strcmp(sz1, sz2) < 0);
	}
};


class VisPropListExport CVisPropKeyData
{
public:
	CVisPropKeyData(const char *szKeyName);
	CVisPropKeyData(const CVisPropKeyData& refpropkeydata);
	CVisPropKeyData& operator=(const CVisPropKeyData& refpropkeydata);
	~CVisPropKeyData(void);

	const char *Name(void) const;

private:
	char *m_pchBuf;

	ULONG& CRefKey(void);
	ULONG AddRefKey(void);
	ULONG ReleaseKey(void);
};



// The following class will be used to share propery values between
// different property lists.

// This class allows multiple references to an object.  Note that there are
// no virtual functions and the object is the first data member.  An article
// in CUJ suggested this to avoid an offset when accessing the object.
template<class TObject>
class CVisRefCntObj
{
public:
	CVisRefCntObj(void);
	CVisRefCntObj(const TObject& refobj);
	~CVisRefCntObj(void);

	ULONG AddRef(void);
	ULONG Release(void);

    const TObject& Obj(void) const;
    TObject& Obj(void);

protected:
	// Don't allow copy ctor and assignment operator.
	CVisRefCntObj(const CVisRefCntObj<TObject>& refrefcntobj);
	CVisRefCntObj<TObject>& operator=(const CVisRefCntObj<TObject>& refrefcntobj);

private:
    TObject m_obj;
	ULONG m_cRef;
};



class VisPropListExport CVisRefCntObjArrayBase
{
public:
	~CVisRefCntObjArrayBase(void);

	const CVisDim& Dim(void) const;

	int CbObj(void) const;

	void *PvObjFirst(void) const;

	void *PvObj(const CVisDimIndex& refdimindex) const;

protected:
	CVisRefCntObjArrayBase(int cbObj, bool fAllocated, const CVisDim& refdim);
	CVisRefCntObjArrayBase(int cbObj, bool fAllocated, const CVisDim& refdim,
			void *pvWhere);

	// Don't allow copy ctor and assignment operator.
	CVisRefCntObjArrayBase(const CVisRefCntObjArrayBase& refrefcntobj);
	CVisRefCntObjArrayBase& operator=(
			const CVisRefCntObjArrayBase& refrefcntobj);

	bool FAllocated(void) const;

private:
    void *m_rgvObj;
	const int m_cbObj;
	CVisDim m_dim;
	bool m_fAllocated;
};


template<class TObject>
class CVisRefCntObjArray : public CVisRefCntObjArrayBase
{
public:
	// We don't use 0 as a default for pvWhere because we might want to
	// allow a reference to 0 that will later be offset by a this pointer.
	CVisRefCntObjArray(const CVisDim& refdim);
	CVisRefCntObjArray(const CVisDim& refdim, void *pvWhere);
	~CVisRefCntObjArray(void);

	// Note:  We could make AddRef, Release, and Clone virtual functions.
	// We chose not to use virtual functions, since we don't use them with
	// the CVisRefCntObj class.
	ULONG AddRef(void);
	ULONG Release(void);

	CVisRefCntObjArray<TObject> *Clone(void) const;

	TObject *PObjFirst(void) const;

	TObject& Obj(const CVisDimIndex& refdimindex) const;

protected:
	// Don't allow copy ctor and assignment operator.
	CVisRefCntObjArray(const CVisRefCntObjArray<TObject>& refrefcntobj);
	CVisRefCntObjArray<TObject>& operator=(
			const CVisRefCntObjArray<TObject>& refrefcntobj);

private:
	ULONG m_cRef;
};





class VisPropListExport CVisPropTypeInfoBase
{
public:
	virtual ~CVisPropTypeInfoBase(void);

	// LATER:  May want Release to be virtual so that we don't have problems
	// with mismatched new and delete operators.
	ULONG AddRef(void);
	ULONG Release(void);

	virtual const type_info& TypeInfo(void) const = 0;

	const char *Name(void) const;
	void SetName(const char *szName);

	bool ArePropsPrinted(void) const;
	bool CanPropsPrint(void) const;
	void SetPropPrinting(bool f);

	bool IsSimpleType(void) const;
	void SetIsSimpleType(bool f);

	bool ChangedName(void) const;


	virtual void WriteObj(class CVisSDOStream& refsdostream, const void *pvObj,
			const char *szName = 0, bool fOnlyValue = false) const;

	virtual void WriteObjArray(class CVisSDOStream& refsdostream,
			const CVisDim& refdim, const void *pvObj,
			const char *szName = 0, bool fOnlyValue = false) const;


	virtual void ReadObj(class CVisSDIStream& refsdistream, void *pvObj) const;

	virtual void ReadObjArray(class CVisSDIStream& refsdistream,
			const CVisDim& refdim, void *pvObj) const;


	virtual bool AlwaysUseTypedef(void) const;
	virtual void SetAlwaysUseTypedef(bool f);

	virtual CVisPropList *PPropList(const void *pvObj = 0);

	virtual void SDIFindTypes(CVisSDIStream& refsdistream);
	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj);
	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj,
			const CVisDim& refdim);

	bool FContainsPropLists(void);
	void SetFContainsPropLists(bool f);


	// This flag is used internally.  If sizeof(TObject) <= sizeof(double) and
	// the contructor and destructor really need to be called for objects of
	// this type, your code may need to set this flag to false before using
	// objects of this type in property lists.
	bool CanCopyObjectBytes(void) const;
	void SetCanCopyObjectBytes(bool f);


protected:
	friend class CVisProp;
	friend class CVisPropTypeInfoMap;  // CVisPropTypeInfoMap::ChangeName

	virtual void AssignObjToObj(const void *pvObjSrc, void *pvObjDest) const = 0;
	void AssignPvPropToPvProp(const void *pvObjSrc, void *pvObjDest) const;

	virtual void *ClonePvRefCntObj(const void *pvRefCntObj) const = 0;
	virtual ULONG AddRefPvRefCntObj(void *pvRefCntObj) const = 0;
	virtual ULONG ReleasePvRefCntObj(void *pvRefCntObj) const = 0;
	virtual void *PvRefCntObjMakePvObj(const void *pvObj) const = 0;

	virtual void *ClonePvRefCntArray(const void *pvRefCntObj) const = 0;
	virtual ULONG AddRefPvRefCntArray(void *pvRefCntObj) const = 0;
	virtual ULONG ReleasePvRefCntArray(void *pvRefCntObj) const = 0;
	virtual CVisRefCntObjArrayBase *PRefCntArrayMake(const CVisDim& refdim)
			const = 0;
	virtual CVisRefCntObjArrayBase *PRefCntArrayMake(const CVisDim& refdim,
			void *pvWhere) const = 0;

	void ChangeName(const char *szName); // Called by CVisPropTypeInfoMap::ChangeName

	virtual void ClearPropList(void);

	// Helper function used internally.
	static void FilterName(std::string& refstrName);

	CVisPropTypeInfoBase(const char *szName, bool fCanCopyObjectBytes,
			bool fCanPrint);

private:
	std::string m_strName;
	ULONG m_cRef;
	bool m_fCanCopyObjectBytes;
	bool m_fPrint;
	bool m_fCanPrint;
	bool m_fSimpleType;
	bool m_fChangedName;
	bool m_fContainsPropLists;
	bool m_fCheckedForPropLists;
};


template<class TObject>
class CVisPropTypeInfoNoIO : public CVisPropTypeInfoBase
{
public:
	CVisPropTypeInfoNoIO(const char *szName = 0);

	virtual const type_info& TypeInfo(void) const;

protected:
	friend class CVisProp;

	CVisPropTypeInfoNoIO(bool fCanPrint, const char *szName);

	virtual void AssignObjToObj(const void *pvObjSrc, void *pvObjDest) const;

	virtual void *ClonePvRefCntObj(const void *pvRefCntObj) const;
	virtual ULONG AddRefPvRefCntObj(void *pvRefCntObj) const;
	virtual ULONG ReleasePvRefCntObj(void *pvRefCntObj) const;
	virtual void *PvRefCntObjMakePvObj(const void *pvObj) const;

	virtual void *ClonePvRefCntArray(const void *pvRefCntObj) const;
	virtual ULONG AddRefPvRefCntArray(void *pvRefCntObj) const;
	virtual ULONG ReleasePvRefCntArray(void *pvRefCntObj) const;
	virtual CVisRefCntObjArrayBase *PRefCntArrayMake(const CVisDim& refdim)
			const;
	virtual CVisRefCntObjArrayBase *PRefCntArrayMake(const CVisDim& refdim,
			void *pvWhere) const;
};


template<class TObject>
class CVisPropTypeInfoWO : public CVisPropTypeInfoNoIO<TObject>
{
public:
	CVisPropTypeInfoWO(const char *szName);
	CVisPropTypeInfoWO(bool fSimpleType = false, const char *szName = 0);
	virtual ~CVisPropTypeInfoWO(void);

	virtual void WriteObj(class CVisSDOStream& refsdostream, const void *pvObj,
			const char *szName = 0, bool fOnlyValue = false) const;

	virtual void WriteObjArray(class CVisSDOStream& refsdostream,
			const CVisDim& refdim, const void *pvObj,
			const char *szName = 0, bool fOnlyValue = false) const;

	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj = 0);
	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj,
			const CVisDim& refdim);
};


template<class TObject>
class CVisPropTypeInfoWithIO : public CVisPropTypeInfoWO<TObject>
{
public:
	CVisPropTypeInfoWithIO(const char *szName);
	CVisPropTypeInfoWithIO(bool fSimpleType = false, const char *szName = 0);
	virtual ~CVisPropTypeInfoWithIO(void);


	virtual void ReadObj(class CVisSDIStream& refsdistream, void *pvObj) const;

	virtual void ReadObjArray(class CVisSDIStream& refsdistream,
			const CVisDim& refdim, void *pvObj) const;

	virtual void SDIFindTypes(CVisSDIStream& refsdistream);
};


template<class TObject>
class CVisPropTypeInfoWithIOPropList : public CVisPropTypeInfoWithIO<TObject>
{
public:
	CVisPropTypeInfoWithIOPropList(const char *szName = 0,
			bool fAlwaysUseTypedef = false);
	virtual ~CVisPropTypeInfoWithIOPropList(void);

	virtual bool AlwaysUseTypedef(void) const;
	virtual void SetAlwaysUseTypedef(bool f);

	virtual CVisPropList *PPropList(const void *pvObj = 0);

	virtual void SDIFindTypes(CVisSDIStream& refsdistream);
	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj = 0);
	virtual void SDOFindTypes(CVisSDOStream& refsdostream, const void *pvObj,
			const CVisDim& refdim);

protected:
	virtual void ClearPropList(void);

private:
	CVisPropList *m_pproplist;
	bool m_fAlwaysUseTypedef;
};

class CVisFnOpCmpPTypeInfo
{
public:
	bool operator()(const type_info *ptypeinfo1, const type_info *ptypeinfo2) const
	{
		assert(ptypeinfo1 != 0);
		assert(ptypeinfo2 != 0);
		return (ptypeinfo1->before(*ptypeinfo2) != 0);
	}
};



typedef std::map<const char *, CVisPropTypeInfoBase *, CVisFnOpCmpSz>
		CVisPropTypeInfoMapName;

// LATER:  Is it okay to store the pointers returned from typeid(TObject) in this class?
// (It seems to work okay.)
typedef std::map<const type_info *, CVisPropTypeInfoBase *, CVisFnOpCmpPTypeInfo>
		CVisPropTypeInfoMapTypeInfo;

class CVisPropTypeInfoMap
{
public:
	CVisPropTypeInfoMap(void);
	~CVisPropTypeInfoMap(void);

	void AddPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo);
	CVisPropTypeInfoBase *LookupPropTypeInfo(const char *szName);
	CVisPropTypeInfoBase *LookupPropTypeInfo(const type_info& reftypeinfo);
	void ChangeName(CVisPropTypeInfoBase *pproptypeinfo, const char *szName);

#ifdef _DEBUG
	void PrintKnownTypes(bool fIncludeTypeIDNames) const;
#endif // _DEBUG

private:
	CVisPropTypeInfoMapName m_visproptypeinfomapName;
	CVisPropTypeInfoMapTypeInfo m_visproptypeinfomapTypeInfo;

	void CleanupPropTypeInfo(void);
};

// LATER:  We could add an intermediate CVisPropTypeInfoSTLContainer class
// that takes szContainer (eg "deque" or "vector") in a protected ctor.
template<class TObject>
class CVisPropTypeInfoDeque : public CVisPropTypeInfoWithIO<TObject>
{
public:
	CVisPropTypeInfoDeque(void);
	virtual ~CVisPropTypeInfoDeque(void);
};


template<class TObject>
class CVisPropTypeInfoVector : public CVisPropTypeInfoWithIO<TObject>
{
public:
	CVisPropTypeInfoVector(void);
	virtual ~CVisPropTypeInfoVector(void);
};


// Function that creates a new prop type info object.
//template<class TObject>
//CVisPropTypeInfoBase *VisPPropTypeInfoNewForClass(TObject const &refobj);

// Function that looks up an existing prop type info object (and returns 0
// if none is found).
// UNDONE:  Do we need prototypes for these?  (If so, it's hard to use them with templates.)
//template<class TObject>
//CVisPropTypeInfoBase *VisPPropTypeInfoForClass(TObject const &refobj);
//
//template<class TObject>
//CVisPropTypeInfoBase *VisGetPropTypeInfo(const TObject& refobject);


// LATER:  Export these
VisPropListExport void VisAddPropTypeInfo(CVisPropTypeInfoBase *pproptypeinfo);

VisPropListExport CVisPropTypeInfoBase *VisLookupPropTypeInfo(const char *szFullName);

VisPropListExport CVisPropTypeInfoBase *VisLookupPropTypeInfo(const type_info& reftypeinfo);

#ifdef _DEBUG
VisPropListExport void VisPrintPropTypeInfoForDebugging(bool fIncludeTypeIDNames = false);
#endif // _DEBUG
