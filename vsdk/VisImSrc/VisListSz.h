////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISLISTSZ
//
// @module VisListSz.h |
//
// This file defines the <c CVisListSz> class that implements the
// <i IVisListSz> interface.  <c CVisListSz> objects can be used to store
// a list of strings.  A list of strings stored in a <c CVisListSz>
// object can be used with a <c CVisDlgListSz> object to display a
// dialog and let the user select a string from a list of strings.
//
// All <c CVisListSz> methods are inline, so this class can be used
// by applications that do not use the VisImSrc project DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisListSz\.inl>
//
// <nl>
// @index | VISLISTSZ
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISLISTSZ_H__
#define __VIS_IMSRC_VISLISTSZ_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include <vector>
#include "VisImSrcIFace.h"


////////////////////////////////////////////////////////////////////////////
//  
// @class
//  
// This class is used to build a list of strings.  Since it implements
// the <i IVisListSz> interface, it can be used to pass a list of strings
// to the <f VisDoImSrcDlg> function.
//
////////////////////////////////////////////////////////////////////////////
class CVisListSz : public IVisListSz
{
public:
	//------------------------------------------------------------------
	// @group Constructor and destructor

	// @cmember
	// Constructor
	CVisListSz(void);

	// @cmember
	// Destructor
	~CVisListSz(void);


	//------------------------------------------------------------------
	// @group IUnknown methods

    // @cmember <c HRESULT> | QueryInterface |
	//	(<c REFIID> riid, <c void> * * ppvObject) |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject);

    // @cmember <c ULONG> | AddRef |
	//	(void) |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS);

    // @cmember <c ULONG> | Release |
	//	(void) |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS);


	//------------------------------------------------------------------
	// @group IVisListSz methods

    // @cmember <c LONG> | CszGet |
	//	(void) const |
	// Returns the number of strings in the list.
    STDMETHOD_(LONG, CszGet) (THIS) CONST;

    // @cmember <c LONG> | IGetSz |
	//	(<c const char> * sz) const |
	// Given a string, find its index in the list.  If the string is
	// not in the list, this method will return a negative number.
    STDMETHOD_(LONG, IGetSz) (THIS_ LPCSTR sz) CONST;

    // @cmember <c LPCSTR> | SzGetI |
	//	(<c long> * i) const |
	// Get an indexed string from the list.  If the index is not valid,
	// a NULL string pointer will be returned.
    STDMETHOD_(LPCSTR, SzGetI) (THIS_ LONG i) CONST;
 

	//------------------------------------------------------------------
	// @group CVisListSz methods
 
	// @cmember <c HRESULT> | AddSz |
	//	(<c const char> * sz) |
	// Add a string to the list.  The string will be appended to
	// the list, so the indices of strings already in the list will
	// not change.
    STDMETHOD(AddSz) (THIS_ LPCSTR sz);

private:
	ULONG m_cRef;
	std::vector<LPSTR> m_vectorRgsz;
};


#endif // __VIS_IMSRC_VISLISTSZ_H__
