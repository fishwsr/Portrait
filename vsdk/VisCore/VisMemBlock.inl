///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMEMBLOCK
//
// @module VisMemBlock.inl |
//
// Reference counted memory blocks.  Inline functions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisMemBlock\.h> <l VisMemBlock\.cpp>
//
// <nl>
// @index | VISMEMBLOCK
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_MEMBLOCK_INL__
#define __VIS_CORE_MEMBLOCK_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <assert.h>
#include "VisError.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisMemBlock | ~CVisMemBlock |
//
// Destructor called when this object is destroyed.  If this
// object allocated memory in its constructor, that memory
// will be freed when the destructor is called.  The destructor
// is virtual so that other classes can be derived from this
// class.
//
////////////////////////////////////////////////////////////////////////////
inline CVisMemBlock::~CVisMemBlock(void)
{
	if (m_pRefCntMemBlock != 0)
	{
		m_pRefCntMemBlock->Release();
		m_pRefCntMemBlock = 0;
	}

#ifdef _DEBUG
    m_pRefCntMemBlock = 0;
#endif // _DEBUG
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc BYTE * | CVisMemBlock | PbData |
//
// Get a pointer to the memory block controlled by this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline BYTE *CVisMemBlock::PbData(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->PbData();
	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisMemBlock | CbData |
//
// Get the size, in <t BYTE>s, of the memory block controlled by
// this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisMemBlock::CbData(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->CbData();
	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc HANDLE | CVisMemBlock | HFileMapping |
//
// Get a handle to the file mapping object allocated for this
// memory block.  Returns zero if this block was not allocated
// using the <f CreateFileMapping> function.
//
////////////////////////////////////////////////////////////////////////////
inline HANDLE CVisMemBlock::HFileMapping(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->HFileMapping();
	return (HANDLE) 0;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc HANDLE | CVisRefCntMemBlock | LID |
//
// Get a long to allow reference to shared memory
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisMemBlock::FileMappingID(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->FileMappingID();
	return 0;
}

inline unsigned __int64 CVisMemBlock::FileMappingAppID(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->FileMappingAppID();
	return 0;
}

inline void CVisMemBlock::GetFileMappingNameString(char *szName) const
{
	assert(szName != 0);
	if (m_pRefCntMemBlock != 0)
		m_pRefCntMemBlock->GetFileMappingNameString(szName);
	else
		*szName = 0;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisMemBlock | IbOffset |
//
// If this memory was allocated using CreateFileMapping, return
// the offset to the start of data in the file mapping.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisMemBlock::IbOffset(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->IbOffset();
	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc EVisMemBlock | CVisMemBlock | MemBlockOptions |
//
// Return the options specified when this object was created..
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline EVisMemBlock CVisMemBlock::MemBlockOptions(void) const
{
	if (m_pRefCntMemBlock != 0)
		return m_pRefCntMemBlock->MemBlockOptions();
	return (EVisMemBlock) 0;
}

inline IDirectDrawSurface* CVisMemBlock::PIDDS(void) const
{
	IDirectDrawSurface* pIDDS = 0;
	if (m_pRefCntMemBlock != 0)
		pIDDS = m_pRefCntMemBlock->PIDDS();
	return pIDDS;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisMemBlockOf | CVisMemBlockOf |
//
// Allocate a block of memory containing a specified number of
// objects of a given type using the C++ <f new> operator or use
// the same block of memory as another <c CVisMemBlockOf> object.
//
// @syntax CVisMemBlockOf(int cObj);
// @syntax CVisMemBlockOf(const CVisMemBlockOf& refmemblock);
// @syntax CVisMemBlockOf(void);
//
// @parm int | cObj |
// The number of objects to be created.  A memory block containing the
// specified number of objects will be allocated.
//
// @parm const | CVisMemBlockOf& refmemblock |
// A <c CVisMemBlockOf> object to be used when constructing this object.
// (This <c CVisMemBlockOf> object will use the same memory block as
// <p refmemblock>.)
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline CVisMemBlockOf<TObj>::CVisMemBlockOf(int cObj)
  : CVisMemBlock()
{
	m_pRefCntMemBlock = new CVisRefCntMemBlockOf<TObj>(cObj);

	if (m_pRefCntMemBlock == 0)
	{
		throw CVisMemoryError("CVisMemBlockOf::CVisMemBlockOf(int)",
				__FILE__,__LINE__);
	}
}

template <class TObj>
inline CVisMemBlockOf<TObj>::CVisMemBlockOf(
		const CVisMemBlockOf<TObj>& refmemblock)
  : CVisMemBlock(refmemblock)
{
}

template <class TObj>
inline CVisMemBlockOf<TObj>::CVisMemBlockOf(void)
  : CVisMemBlock()
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisMemBlockOf& | CVisMemBlockOf | operator= |
//
// Assignment operator.
//
// @parm const | CVisMemBlockOf& refmemblock |
// A <c CVisMemBlockOf> object to be copied.
// (This <c CVisMemBlockOf> object will use the same memory block as
// <p refmemblock>.)
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline CVisMemBlockOf<TObj>& CVisMemBlockOf<TObj>::operator=(
		const CVisMemBlockOf<TObj>& refmemblock)
{
	if (m_pRefCntMemBlock != refmemblock.m_pRefCntMemBlock)
	{
		if (m_pRefCntMemBlock != 0)
			m_pRefCntMemBlock->Release();

		m_pRefCntMemBlock = refmemblock.m_pRefCntMemBlock;
		if (m_pRefCntMemBlock != 0)
			m_pRefCntMemBlock->AddRef();
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc TObj * | CVisMemBlockOf | PObj |
//
// Get a pointer to the first object in the
// memory block controlled by this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline TObj *CVisMemBlockOf<TObj>::PObj(void) const
{
	if (m_pRefCntMemBlock != 0)
		return ((CVisRefCntMemBlockOf<TObj> *) m_pRefCntMemBlock)->PObj();

	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TObj * | CVisMemBlockOf | PObj |
//
// Get a pointer to the specified object in the
// memory block controlled by this object.
//
// @this const
//
// @parm int | iObj |
// The zero-based index of the object in the memory block.
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline TObj *CVisMemBlockOf<TObj>::PObj(int iObj) const
{
	if (m_pRefCntMemBlock != 0)
		return ((CVisRefCntMemBlockOf<TObj> *) m_pRefCntMemBlock)->PObj(iObj);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisMemBlockOf | CObj |
//
// Get the number of objects in the memory block controlled by
// this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline int CVisMemBlockOf<TObj>::CObj(void) const
{
	if (m_pRefCntMemBlock != 0)
		return ((CVisRefCntMemBlockOf<TObj> *) m_pRefCntMemBlock)->CObj();

	return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc unsigned long | CVisRefCntMemBlock | AddRef |
//
// Add a reference to this object by increasing the internal
// reference count.
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisRefCntMemBlock::AddRef(void)
{
#ifdef _DEBUG
	++ s_cAddRefTotal;
#endif // _DEBUG
	return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc unsigned long | CVisRefCntMemBlock | Release |
//
// Release a reference to this object by decreasing the internal
// reference count.  If the reference count falls to zero, this
// object will be deleted.
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisRefCntMemBlock::Release(void)
{
	unsigned long cRef
			= (unsigned long) InterlockedDecrement((LONG *) &m_cRef);

#ifdef _DEBUG
	-- s_cAddRefTotal;
#endif // _DEBUG

	if (cRef == 0)
		delete this;

	return cRef;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc BYTE * | CVisRefCntMemBlock | PbData |
//
// Get a pointer to the memory block controlled by this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline BYTE *CVisRefCntMemBlock::PbData(void) const
{
	return m_pbData;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisRefCntMemBlock | CbData |
//
// Get the size, in <t BYTE>s, of the memory block controlled by
// this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisRefCntMemBlock::CbData(void) const
{
	return m_cbData;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc HANDLE | CVisRefCntMemBlock | HFileMapping |
//
// Get a handle to the file mapping object allocated for this
// memory block.  Returns zero if this block was not allocated
// using the <f CreateFileMapping> function.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline HANDLE CVisRefCntMemBlock::HFileMapping(void) const
{
	return m_hFileMapping;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc HANDLE | CVisRefCntMemBlock | LID |
//
// Get a long to allow reference to shared memory
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline unsigned long CVisRefCntMemBlock::FileMappingID(void) const
{
	return m_ulFileMappingID;
}

inline void CVisRefCntMemBlock::GetFileMappingNameString(char *szName)
{
	FillFileMappingNameString(szName, FileMappingPrefixString(),
			FileMappingID(), FileMappingAppID());
}

inline void CVisRefCntMemBlock::FillFileMappingNameString(char *szName,
		unsigned long ulFileMappingID,
		unsigned __int64 ulFileMappingAppID)
{
	FillFileMappingNameString(szName, FileMappingPrefixString(), ulFileMappingID, 
			(ulFileMappingAppID != 0) ? ulFileMappingAppID : PGlobals()->FileMappingAppID());
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc EVisMemBlock | CVisRefCntMemBlock | MemBlockOptions |
//
// Return the options specified when this object was created..
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline EVisMemBlock CVisRefCntMemBlock::MemBlockOptions(void) const
{
	return m_evismemblock;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisRefCntMemBlock | IbOffset |
//
// If this memory was allocated using CreateFileMapping, return
// the offset to the start of data in the file mapping.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisRefCntMemBlock::IbOffset(void) const
{
	return m_ibOffset;
}

inline IDirectDrawSurface* CVisRefCntMemBlock::PIDDS(void) const
{
	IDirectDrawSurface* pDDS = NULL;
	if(m_evismemblock == evismemblockAllocDirectDrawSurface)
		pDDS = (IDirectDrawSurface *)m_pvUser;

	return pDDS;
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRefCntMemBlockOf | CVisRefCntMemBlockOf |
//
// Allocate a block of memory containing a specified number of
// objects of a given type using the C++ <f new> operator.
//
// @parm int | iObj |
// The zero-based index of the object in the memory block.
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline CVisRefCntMemBlockOf<TObj>::CVisRefCntMemBlockOf(int cObj)
 : CVisRefCntMemBlock((BYTE *) new TObj[cObj], cObj * sizeof(TObj),
		0, 0, evismemblockAllocNewTemplate)
{
	assert(cObj > 0);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRefCntMemBlockOf | ~CVisRefCntMemBlockOf |
//
// Destructor called when this object is destroyed.  Objects
// created in the constructor will be freed using the C++
// <f delete> operator.
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline CVisRefCntMemBlockOf<TObj>::~CVisRefCntMemBlockOf(void)
{
	delete[] PObj();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TObj | CVisRefCntMemBlockOf | PObj |
//
// Get a pointer to the first object in the
// memory block controlled by this object.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline TObj *CVisRefCntMemBlockOf<TObj>::PObj(void) const
{
	return (TObj *) PbData();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc TObj | CVisRefCntMemBlockOf | PObj |
//
// Get a pointer to the specified object in the
// memory block controlled by this object.
//
// @this const
//
// @parm int | iObj |
// The zero-based index of the object in the memory block.
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
inline TObj *CVisRefCntMemBlockOf<TObj>::PObj(int iObj) const
{
	assert(iObj >= 0);
	assert(iObj < m_cObj);

	return &(((TObj *) PbData())[iObj]);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisRefCntMemBlockOf | CObj |
//
// Get the number of objects in the memory block controlled by
// this object.
//
// @this const
//
///////////////////////////////////////////////////////////////////////////
template <class TObj>
inline int CVisRefCntMemBlockOf<TObj>::CObj(void) const
{
	return m_cObj;
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG


#endif // __VIS_CORE_MEMBLOCK_INL__
