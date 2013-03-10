// VisPropList.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef VIS_CORE_PROPLIST_H
#define VIS_CORE_PROPLIST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef VIS_ONLY_DECLARE_PROPLIST
#define VIS_NO_SD_STREAM
#endif // VIS_ONLY_DECLARE_PROPLIST


#include <string>

#include <vector>
#include <deque>
#include <set>
#include <map>

#include <iostream>
#include <fstream>


#include "VisDim.h"

#ifdef VIS_NO_SD_STREAM
// Use forward declarations
class CVisSDOStream;
class CVisSDIStream;
#else // VIS_NO_SD_STREAM
#include "VisSDOStream.h"
#include "VisSDIStream.h"
#endif // VIS_NO_SD_STREAM

#include "VisPropListInternal.h"

#include "VisProp.h"



template<class TObject>
void VisUsePropType(TObject const & refobj);



// This special property name is reserved for a list of user-defined properties.
#define VIS_SZ_SD_USER_PROP_LIST	"VisPropListUserDefined"

typedef std::vector<std::string> CVisOrderedPropNameList;


class CVisPropList
{
public:
    //------------------------------------------------------------------
	// Type definitions 

	// This type is used internally.
	typedef std::map<const char *, CVisProp, CVisFnOpCmpSz>
			CVisPropValueMap;

	// This is the CVisPropList iterator.  To protect against damage
	// to internal data structures, only a const_iterator is defined.
	class const_iterator
	{
	public:
		VisPropListExport const_iterator(
				CVisPropValueMap::const_iterator const &refit);
		VisPropListExport const_iterator(const const_iterator &refiterator);
		VisPropListExport const_iterator& operator=(
				const const_iterator &refiterator);

		VisPropListExport bool operator==(const const_iterator& refiterator) const;
		VisPropListExport bool operator!=(const const_iterator& refiterator) const;

		VisPropListExport const_iterator& operator++(void);
		VisPropListExport const_iterator operator++(int);

		VisPropListExport const CVisProp& operator*(void) const;

	protected:
		friend class CVisPropList;
		CVisPropValueMap::const_iterator m_iteratorMap;
	};


    //------------------------------------------------------------------
	// Constructors, assignment operator, and destructor 

	VisPropListExport CVisPropList(void);
	VisPropListExport CVisPropList(const CVisPropList& refproplist);
	CVisPropList& operator=(const CVisPropList& refproplist);
	VisPropListExport ~CVisPropList(void);


    //------------------------------------------------------------------
	// Property list attributes

	VisPropListExport bool IsEmpty(void) const;

	VisPropListExport int CProp(void) const;

	VisPropListExport bool HasProp(const char *szPropName) const;

	// The name of the type that this property list represents.
	// This is set when we read an unrecognized type from a file.
	// (This string property is needed.)
	const char *TypeName(void) const;
	VisPropListExport void SetTypeName(const char *szTypeName);


	// LATER:  Offsetting property value references?


    //------------------------------------------------------------------
	// Iterators

	VisPropListExport const_iterator begin(void) const;
	VisPropListExport const_iterator end(void) const;

	const char *Name(const const_iterator& refiterator) const;

	VisPropListExport const_iterator IteratorFromPropName(const char *szPropName)
			const;


    //------------------------------------------------------------------
	// Property attributes

	bool IsShared(const char *szPropName) const;
	bool IsShared(const const_iterator &refiterator) const;

	bool IsPrinted(const char *szPropName) const;
	bool IsPrinted(const const_iterator &refiterator) const;

	bool IsObjReference(const char *szPropName) const;
	bool IsObjReference(const const_iterator &refiterator) const;

	bool IsArray(const char *szPropName) const;
	bool IsArray(const const_iterator &refiterator) const;

	// It is an error to call Dim() if IsArray() is not true.
	const CVisDim& Dim(const char *szPropName) const;
	const CVisDim& Dim(const const_iterator &refiterator) const;


	void SetSharing(const char *szPropName, bool f);
	void SetSharing(const const_iterator &refiterator, bool f);

	void SetPrinting(const char *szPropName, bool f);
	void SetPrinting(const const_iterator &refiterator, bool f);


    //------------------------------------------------------------------
	// Property type information

	const char *TypeName(const char *szPropName) const;
	const char *TypeName(const const_iterator &refiterator) const;

	const type_info& TypeInfo(const char *szPropName) const;
	const type_info& TypeInfo(const const_iterator &refiterator) const;

	// The CVisPropTypeInfo class is used internally.
	CVisPropTypeInfoBase& PropTypeInfo(const char *szPropName) const;
	CVisPropTypeInfoBase& PropTypeInfo(const const_iterator &refiterator)
			const;


    //------------------------------------------------------------------
	// Adding (scalar) properties

	// UNDONE:  Document the variations of these functions better (eg.
	// that refdim is optional).

	// (The list is sorted by property name, so the name must be specified
	// when the proerty is added.)
	// The default sharing is true for large objects, false for small objects
	// false for small objects (to reduce the total number of allocs).

	template<class TObject>
	void AddProp(const char *szPropName, const TObject& refobj)
		{ AddPropInternal(szPropName, refobj); }

	template<class TObject>
	void AddProp(const char *szPropName, const TObject& refobj,
			bool fShared)
		{ AddPropInternal(szPropName, refobj, fShared); }

	template<class TObject>
	void AddPropReference(const char *szPropName, TObject& refobj,
			bool fShared = false)
		{ AddPropRefInternal(szPropName, refobj, fShared); }



	// The CVisProp class is used internally.
	VisPropListExport void AddProp(const CVisProp& refprop);


    //------------------------------------------------------------------
	// Adding array properties

	template<class TObject>
	void AddProp(const char *szPropName, const TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ AddPropInternal(szPropName, refobj, refdim, fShared); }

	template<class TObject>
	void AddPropReference(const char *szPropName, TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ AddPropRefInternal(szPropName, refobj, refdim, fShared); }



    //------------------------------------------------------------------
	// Removing properties

	VisPropListExport void Clear(void);

	void DeleteProp(const char *szPropName);
	VisPropListExport void DeleteProp(const const_iterator &refiterator);


    //------------------------------------------------------------------
	// Getting property values

	// Get a scalar value using a property name
	template<class TObject>
	void GetObj(const char *szPropName, TObject& refobj) const
		{ GetObjInternal(szPropName, refobj); }

	// Get a scalar value using an iterator
	template<class TObject>
	void GetObj(const char *szPropName, TObject& refobj,
			const CVisDimIndex& refdimindex) const
		{ GetObjInternal(szPropName, refobj, refdimindex); }

	// Get a value in an array using a property name
	template<class TObject>
	void GetObj(const const_iterator &refiterator, TObject& refobj) const
		{ GetObjInternal(refiterator, refobj); }

	// Get a value in an array using an iterator
	template<class TObject>
	void GetObj(const const_iterator &refiterator, TObject& refobj,
			const CVisDimIndex& refdimindex) const
		{ GetObjInternal(refiterator, refobj, refdimindex); }



    //------------------------------------------------------------------
	// Setting scalar property values using property names

	template<class TObject>
	void SetObj(const char *szPropName, const TObject& refobj)
		{ SetObjInternal(szPropName, refobj); }

	template<class TObject>
	void SetObj(const char *szPropName, const TObject& refobj, bool fShared)
		{ SetObjInternal(szPropName, refobj, fShared); }

	template<class TObject>
	void SetObjReference(const char *szPropName, TObject& refobj,
			bool fShared = false)
		{ SetObjRefInternal(szPropName, refobj, fShared); }


    //------------------------------------------------------------------
	// Setting array property values using property names

	// Set a value in an array
	template<class TObject>
	void SetObj(const char *szPropName, const TObject& refobj,
			const CVisDimIndex& refdimindex)
		{ SetObjInternal(szPropName, refobj, refdimindex); }

	// Set an entire array
	template<class TObject>
	void SetObj(const char *szPropName, const TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ SetObjInternal(szPropName, refobj, refdim, fShared); }

	// Set an entire array
	template<class TObject>
	void SetObjReference(const char *szPropName, TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ SetObjRefInternal(szPropName, refobj, refdim, fShared); }


    //------------------------------------------------------------------
	// Setting scalar property values using iterators

	template<class TObject>
	void SetObj(const const_iterator &refiterator, const TObject& refobj)
		{ SetObjInternal(refiterator, refobj); }

	template<class TObject>
	void SetObj(const const_iterator &refiterator, const TObject& refobj,
			bool fShared)
		{ SetObjInternal(refiterator, refobj, fShared); }

	template<class TObject>
	void SetObjReference(const const_iterator &refiterator, TObject& refobj,
			bool fShared = false)
		{ SetObjRefInternal(refiterator, refobj, fShared); }


    //------------------------------------------------------------------
	// Setting array property values using iterators

	// Set a value in an array
	template<class TObject>
	void SetObj(const const_iterator &refiterator, const TObject& refobj,
			const CVisDimIndex& refdimindex)
		{ SetObjInternal(refiterator, refobj, refdimindex); }

	// Set an entire array
	template<class TObject>
	void SetObj(const const_iterator &refiterator, const TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ SetObjInternal(refiterator, refobj, refdim, fShared); }

	// Set an entire array
	template<class TObject>
	void SetObjReference(const const_iterator &refiterator, TObject& refobj,
			const CVisDim& refdim, bool fShared = true)
		{ SetObjRefInternal(refiterator, refobj, refdim, fShared); }


    //------------------------------------------------------------------
	// CVisProp objects 

	// The CVisProp class is used internally.
	// Return a reference to a property value in the list.  A reference
	// to *((const CVisProp *) 0) is returned if there is no property
	// with the specified name in the list.
	const CVisProp& Prop(const char *szPropName) const;


    //------------------------------------------------------------------
	// SDStream I / O

	VisPropListExport void WriteValue(CVisSDOStream& refsdostream,
			bool fGroupValue = true, const void *pvReferenceOffset = 0) const;

	VisPropListExport void ReadValue(CVisSDIStream& refsdistream,
			const void *pvObjOffset = 0);

	// Helper method used internally.
	VisPropListExport void ReadPropValue(const char *szType, const char *szName,
			const CVisDim& refdim, bool fShared, CVisSDIStream& refsdistream,
			const void *pvObjOffset = 0);
	

    //------------------------------------------------------------------
	// Ordered Property Name List (used when describing other classes)

	bool HasOrderedPropNameList(void) const;
	VisPropListExport void SetHasOrderedPropNameList(bool f);
	const CVisOrderedPropNameList *POrderedPropNameList(void) const;

	// Static helper methods used to create and destroy CVisPropLists.
	static VisPropListExport CVisPropList *PPropListNew(void);
	static VisPropListExport void DeletePPropList(CVisPropList *pproplist);


    // Special helper function used internally.
    VisPropListExport void OffsetReferences(int ibOffset);


protected:
	VisPropListExport void Assign(const CVisPropList& refproplist);

	VisPropListExport const CVisProp& PropGetSz(const char *szPropName) const;
	VisPropListExport CVisProp& PropGetSz(const char *szPropName);

	const CVisProp& PropGetIterator(const const_iterator &refiterator) const;
	CVisProp& PropGetIterator(const const_iterator &refiterator);

	VisPropListExport void AddSzOrderedPropName(const char *szPropName);


	// Helper methods used with AddProp
	VisPropListExport CVisProp& PropListValueNew(const char *szPropName);

	template<class TObject>
	CVisProp *PPropForAdd(const char *szPropName, const TObject& refobj)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		if (&refprop != 0)
			return &refprop;
		
		CVisProp *pprop = &(PropListValueNew(szPropName));
		assert(pprop != 0);
		pprop->SetPPropTypeInfo(VisGetPropTypeInfo(refobj));

		return pprop;
	}


	// LATER:  It might be a little easier on the compiler if the CVisPropList
	// internal Add / Get / Set methods called the CVisProp internal Get / Set
	// methods rather than calling the public wrapper methods in CVisProp.


	template<class TObject>
	void AddPropInternal(const char *szPropName, const TObject& refobj)
	{
		CVisProp *pprop = PPropForAdd(szPropName, refobj);
		assert(pprop != 0);
		pprop->SetObjInternal(refobj, pprop->FSharedDefault());
		assert(!HasOrderedPropNameList());
	}

	template<class TObject>
	void AddPropInternal(const char *szPropName, const TObject& refobj,
			bool fShared)
	{
		CVisProp *pprop = PPropForAdd(szPropName, refobj);
		assert(pprop != 0);
		pprop->SetObjInternal(refobj, fShared);
		assert(!HasOrderedPropNameList());
	}

	template<class TObject>
	void AddPropRefInternal(const char *szPropName, TObject& refobj,
			bool fShared)
	{
		CVisProp *pprop = PPropForAdd(szPropName, refobj);
		assert(pprop != 0);
		pprop->SetObjRefInternal(refobj, fShared);
		if (HasOrderedPropNameList())
			AddSzOrderedPropName(szPropName);
	}


	template<class TObject>
	void AddPropInternal(const char *szPropName, const TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp *pprop = PPropForAdd(szPropName, refobj);
		assert(pprop != 0);
		pprop->SetObjInternal(refobj, refdim, fShared);
		assert(!HasOrderedPropNameList());
	}

	template<class TObject>
	void AddPropRefInternal(const char *szPropName, TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp *pprop = PPropForAdd(szPropName, refobj);
		assert(pprop != 0);
		pprop->SetObjRefInternal(refobj, refdim, fShared);
		if (HasOrderedPropNameList())
			AddSzOrderedPropName(szPropName);
	}


	template<class TObject>
	void GetObjInternal(const char *szPropName, TObject& refobj) const
	{
		const CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.GetObjInternal(refobj);
	}

	template<class TObject>
	void GetObjInternal(const char *szPropName, TObject& refobj,
			const CVisDimIndex& refdimindex) const
	{
		const CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.GetObjInternal(refobj, refdimindex);
	}

	template<class TObject>
	void GetObjInternal(const const_iterator &refiterator, TObject& refobj) const
	{
		const CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.GetObjInternal(refobj);
	}

	template<class TObject>
	void GetObjInternal(const const_iterator &refiterator, TObject& refobj,
			const CVisDimIndex& refdimindex) const
	{
		const CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.GetObjInternal(refobj, refdimindex);
	}


	// Note:  The Set methods are needed, because we don't want the
	// CVisPropList to return non-const references to the CVisProp
	// objects that it uses internally.

	template<class TObject>
	void SetObjInternal(const const_iterator &refiterator, const TObject& refobj)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refprop.FSharedDefault());
	}

	template<class TObject>
	void SetObjInternal(const const_iterator &refiterator, const TObject& refobj,
			bool fShared)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, fShared);
	}

	template<class TObject>
	void SetObjRefInternal(const const_iterator &refiterator, TObject& refobj,
			bool fShared)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjRefInternal(refobj, fShared);
	}


	template<class TObject>
	void SetObjInternal(const const_iterator &refiterator,
			const TObject& refobj, const CVisDimIndex& refdimindex)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refdimindex);
	}

	template<class TObject>
	void SetObjInternal(const const_iterator &refiterator, const TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refdim, fShared);
	}

	template<class TObject>
	void SetObjRefInternal(const const_iterator &refiterator, TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp& refprop = PropGetIterator(refiterator);
		assert(&refprop != 0);
		refprop.SetObjRefInternal(refobj, refdim, fShared);
	}


	template<class TObject>
	void SetObjInternal(const char *szPropName, const TObject& refobj)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refprop.FSharedDefault());
	}

	template<class TObject>
	void SetObjInternal(const char *szPropName, const TObject& refobj,
			bool fShared)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, fShared);
	}

	template<class TObject>
	void SetObjRefInternal(const char *szPropName, TObject& refobj,
			bool fShared)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjRefInternal(refobj, fShared);
	}


	template<class TObject>
	void SetObjInternal(const char *szPropName, const TObject& refobj,
			const CVisDimIndex& refdimindex)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refdimindex);
	}

	template<class TObject>
	void SetObjInternal(const char *szPropName, const TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjInternal(refobj, refdim, fShared);
	}

	template<class TObject>
	void SetObjRefInternal(const char *szPropName, TObject& refobj,
			const CVisDim& refdim, bool fShared)
	{
		CVisProp& refprop = PropGetSz(szPropName);
		assert(&refprop != 0);
		refprop.SetObjRefInternal(refobj, refdim, fShared);
	}


private:
	// LATER:  Change the code to use a sorted array instead of a map.
	// (Arrays may want to use CVisProp * instead of CVisProp.)
	CVisPropValueMap m_map;

// If we use std::string variables here, we get an internal compiler error,
// so we use char * variables for our strings.
//	std::string m_strTypeName;
	// LATER:  It might be better to store this in a special non-printing
	// property (that is shared by defualt).  If the property is not in
	// the list, the type name string is empty.
	char *m_szTypeName;
	CVisOrderedPropNameList *m_porderedpropnamelist;
};



VisPropListExport CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		CVisPropList const& refobj);


// LATER:  May want to make empty macros if VIS_ONLY_DECLARE_PROPLIST is defined
// so this check doesn't need to be made in places where the macros are used.
#ifndef VIS_ONLY_DECLARE_PROPLIST



#ifdef VIS_NO_SD_STREAM

#define VIS_SDI_CLASS_WITH_PL(TObject)
#define VIS_SDI_CLASS_WITH_IO(TObject)
#define VIS_SDI_CLASS_NO_IO(TObject)

#define VIS_SDO_CLASS_WITH_PL(TObject)
#define VIS_SDO_CLASS_WITH_IO(TObject)
#define VIS_SDO_CLASS_NO_IO(TObject)

#else // VIS_NO_SD_STREAM

#include "VisSDOStream.inl"
#include "VisSDIStream.inl"

#endif // VIS_NO_SD_STREAM


#include "VisPropListInternal.inl"



// Helper macros


// These macros define property-list helper functions for user-defined
// classes.  (They do not define stream helper functions.)


#define VIS_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(TObject)		\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(TObject, true)	\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)			\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_PROP_LIST_CLASS_WITH_IO_PL(TObject)				\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO_PL(TObject, false)\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)			\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_PROP_LIST_CLASS_WITH_IO(TObject)		\
	VIS_PROP_LIST_CLASS_TYPE_INFO_WITH_IO(TObject)	\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)	\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_PROP_LIST_CLASS_NO_IO(TObject)			\
	VIS_PROP_LIST_CLASS_TYPE_INFO_NO_IO(TObject)	\
	VIS_PROP_LIST_CLASS_TYPE_INFO_LOOKUP(TObject)	\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)

#define VIS_PROP_LIST_CLASS_VIRTUAL(TObject)		\
	VIS_PROP_LIST_CLASS_TYPE_INFO_VIRTUAL(TObject)	\
	VIS_PROP_LIST_GET_PROP_TYPE_INFO(TObject)



// These macros define property-list and stream helper functions for
// user-defined classes.

// LATER:  Should we include the Deque and Vector macros in the
// macros below.  (They could add extra code that we don't really
// need, making the compiler more likely to exceed its heap limit.)

// LATER:  We could add the VIS_PROP_LIST_CLASS_DEQUE and
// VIS_PROP_LIST_CLASS_VECTOR macros to these macros, but that would
// significantly increase the memory needed by the compiler.


// This will probably be the most commonly used macro.
// Classes need to implement BuildPropList.  (BuildPropList should not be
// virtual.  It must work correctly when this == 0.)
#define VIS_SD_CLASS_WITH_IO_PL(TObject)	\
	VIS_PROP_LIST_CLASS_WITH_IO_PL(TObject)	\
	VIS_SDI_CLASS_WITH_PL(TObject)			\
	VIS_SDO_CLASS_WITH_PL(TObject)

// Use this for small types that should always be written using a typedef.
// Classes need to implement BuildPropList.  (BuildPropList should not be
// virtual.  It must work correctly when this == 0.)
#define VIS_SD_CLASS_WITH_IO_PL_TYPEDEF(TObject)	\
	VIS_PROP_LIST_CLASS_WITH_IO_PL_TYPEDEF(TObject)	\
	VIS_SDI_CLASS_WITH_PL(TObject)					\
	VIS_SDO_CLASS_WITH_PL(TObject)

// Use this macro for most polymorphic classes (eg CVisImage).
// Classes need to implement PropTypeInfoLookup, PropTypeInfoNew, and
// BuildPropList.  (BuildPropList should not be virtual.  It must work
// correctly when this == 0.)
#define VIS_SD_VIRTUAL_CLASS_WITH_IO_PL(TObject)\
	VIS_PROP_LIST_CLASS_VIRTUAL(TObject)		\
	VIS_SDI_CLASS_WITH_PL(TObject)				\
	VIS_SDO_CLASS_WITH_PL(TObject)

// Use this for classes with their own ReadValue / WriteValue methods.
#define VIS_SD_CLASS_WITH_IO(TObject)		\
	VIS_PROP_LIST_CLASS_WITH_IO(TObject)	\
	VIS_SDI_CLASS(TObject)					\
	VIS_SDO_CLASS(TObject)

// Use this method for classes that should not be used with stream I/O.
#define VIS_SD_CLASS_NO_IO(TObject)		\
	VIS_PROP_LIST_CLASS_NO_IO(TObject)	\
	VIS_SDI_CLASS_NO_IO(TObject)		\
	VIS_SDO_CLASS_NO_IO(TObject)


// Helper macros are defined in VisPropListInternal.inl.
// Template macros are defined in VisPropList.inl.
// Stream macros are defined in VisSDIStream.h and VisSDOStream.h.


#include "VisPropList.inl"

#include "VisProp.inl"

#endif // !VIS_ONLY_DECLARE_PROPLIST


#endif // VIS_CORE_PROPLIST_H
