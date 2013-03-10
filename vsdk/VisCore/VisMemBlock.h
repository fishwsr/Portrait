///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMEMBLOCK
//
// @module VisMemBlock.h |
//
// Reference counted memory blocks.  Class definitions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL)
//
// Test the wrapper class in the DMatrix code.
//
// Should debug print statistics method use stream I / O?
//
// Zero-init option?
//
// Verify that images use file mapping but that file mapping allocator
// is not the default (at least not for small images).
//
// <nl>
//
// @xref <l VisMemBlock\.inl> <l VisMemBlock\.cpp>
//
// <nl>
// @index | VISMEMBLOCK
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_MEMBLOCK_H__
#define __VIS_CORE_MEMBLOCK_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef VISAPI
#define VISAPI  __stdcall
#endif // !VISAPI


////////////////////////////////////////////////////////////////////////////
//  
// @cb void | VisMemBlockCallback |
//  Type of callback function used when destroying memory blocks which
//  were not allocated in the <c CVisMemBlock> class.
//
// @parm BYTE * | pb |
//  Pointer to the memory block to be destroyed.
//
// @parm void * | pvUser |
//  Pointer given when the CVisMemBlock object was constructed.  This is
//  often used to store a pointer to a class or function.
//
////////////////////////////////////////////////////////////////////////////
typedef void (VISAPI *VisMemBlockCallback)(void *pb, void *pvUser);


////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisMemBlock |
// Options specified when constructing <c CVisMemBlock> objects.
//
////////////////////////////////////////////////////////////////////////////
enum EVisMemBlock
{
	//------------------------------------------------------------------
	// @group Allocation options

	// @emem
	// Use the default allocator.
	evismemblockAllocDefault			= 0x0001,

	// @emem
	// Use the Windows <f CreateFileMapping> allocator.
	evismemblockAllocFileMapping		= 0x0001,

	// @emem
	// Use the Windows <f GlobalAlloc> allocator.
	evismemblockAllocGlobalAlloc		= 0x0002,

	// @emem
	// Use the C++ <f new> allocator to allocate memory for an array
	// of <c double>.
	evismemblockAllocNewDouble			= 0x0003,

#ifdef VIS_MEMBLOCK_USE_IMALLOC
	evismemblockAllocIMalloc			= 0x0004,
#endif // VIS_MEMBLOCK_USE_IMALLOC

	// @emem
	// Use the C++ <f new> allocator to allocate objects specified
	// in a template parameter in the derived class.
	evismemblockAllocNewTemplate		= 0x0005,

	// @emem
	// Memory allocated by the user using an unknown allocator.
	evismemblockAllocUserUnknown		= 0x0010,

	// @emem
	// Volatile memory allocated by the user.
	evismemblockAllocUserVolatileBuf	= 0x0011,

	// @emem
	// Limited DMA memory allocated by the user.
	evismemblockAllocUserDMABuf			= 0x0012,

	evismemblockAllocDirectDrawSurface	= 0x0014,

	// @emem
	// Mask used to find the allocation options.
	evismemblockAllocMask				= 0x003f,
};


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This object manages a reference-counted block of memory.  The
// reference counting is done in the constructors, assignment
// operator, and destructor of this object, so the user of this
// object does not need to call any reference-counting methods to
// prevent memory leaks.
//
// @xref <c CVisMemBlockOf> <c CVisRefCntMemBlock>
//
////////////////////////////////////////////////////////////////////////////
class CVisMemBlock
{
// @access Public Members
public:
    //------------------------------------------------------------------
	// @group Constructor and Destructor
  
    // @cmember
	// Construct a storage object using a pointer to a block of
	// memory allocated outside of this object.  When this object
	// is destroyed, the <p pfnCallback> function will be called
	// to destroy the memory block allocated outside of this object.
	// If <p pbData> is NULL, the constructor will throw a
	// <c CVisMemoryError> exception.
    VisCoreExport CVisMemBlock(void *pbData, int cbData,
			VisMemBlockCallback pfnCallback = 0, void *pvUser = 0,
			int evismemblock = evismemblockAllocUserUnknown);
  
    // @cmember
	// Construct a storage object using a handle to a file mapping
	// object and an optional offset to the start of the data in
	// the file mapping object.  When this object is destroyed
	// the <p CloseHandle> function will be called to destroy
	// the handle.  If <p hFileMapping> is NULL, the constructor
	// will throw a <c CVisMemoryError> exception.
	// Since <c HANDLE> is <c void *>, we require that both
	// <p cbDat> and <p ibOffset> need to be specified when
	// using this constructor.
    VisCoreExport CVisMemBlock(HANDLE hFileMapping, int cbData, int ibOffset);

    // @cmember
	// Allocate a memory block of the specified size in bytes.  The
	// allocator used can be specified in the optional <p opt>
	// parameter.
    VisCoreExport CVisMemBlock(int cbData,
			int evismemblock = evismemblockAllocDefault);

    VisCoreExport CVisMemBlock(IDirectDrawSurface* pIDDS);
	
	// @cmember
	// Copy Constructor.
    VisCoreExport CVisMemBlock(const CVisMemBlock& refmemblock);

    // @cmember
	// Default Constructor.
    VisCoreExport CVisMemBlock(void);

    // @cmember
	// Assignment operator.
    VisCoreExport CVisMemBlock& operator=(const CVisMemBlock& refmemblock);

    // @cmember
	// Destructor called when this object is destroyed.  If this
	// object allocated memory in its constructor, that memory
	// will be freed when the destructor is called.  The destructor
	// is virtual so that other classes can be derived from this
	// class.
    ~CVisMemBlock(void);

    //------------------------------------------------------------------
	// @group Data Access

	// @cmember
	// Get a pointer to the memory block controlled by this object.
	BYTE *PbData(void) const;

	// @cmember
	// Get the size, in <t BYTE>s, of the memory block controlled by
	// this object.
	int CbData(void) const;

	IDirectDrawSurface* PIDDS(void) const;
	
	// @cmember
	// Get a handle to the file mapping object allocated for this
	// memory block.  Returns zero if this block was not allocated
	// using the <f CreateFileMapping> function.
	HANDLE HFileMapping(void) const;
 
	// @cmember
	// Get an unsigned long identifying the file mapping for shared
	// memory access.
	unsigned long FileMappingID(void) const;

	// @cmember
	// Get an unsigned 64-bit integer identifying this process.
	unsigned __int64 FileMappingAppID(void) const;

	// @cmember
	// Fill a string with the name used for this file mapping.
	// (The string string may contain up to 32-characters, including
	// the trailing null character.)
	void GetFileMappingNameString(char *szName) const;

	// @cmember
	// If this memory was allocated using CreateFileMapping, return
	// the offset to the start of data in the file mapping.
	int IbOffset(void) const;

	// @cmember
	// Return the options specified when this object was created..
	EVisMemBlock MemBlockOptions(void) const;

#ifdef _DEBUG
    //------------------------------------------------------------------
	// @group Debugging 
 
	// @cmember
	// Print information to make sure that storage is reclaimed.
 	// This method is only present in the DEBUG build of the library.
	// @todo:(INTERNAL) Add code for PrintStatistics and static var init to
	// the CPP file.
   VisCoreExport static void PrintStatistics(FILE *stream = stderr);
#endif // _DEBUG

// @access:(IMPL) Protected Members
protected:
    
    CVisMemBlock(int cbData, int ibOffset, unsigned long ulFileMappingID,
				 unsigned __int64 ulFileMappingAppID);
	
	VisCoreExport friend CVisMemBlock VisCreateMemBlock(int cbSize, int ibOffset, 
										unsigned long ulFileMappingID,
										unsigned __int64 ulFileMappingAppID);
	
	//------------------------------------------------------------------
	// @group:(IMPL) Implementation Variables
 
    // @cmember:(IMPL)
	// Reference counted object that controls the memory block.
    class CVisRefCntMemBlock *m_pRefCntMemBlock;
};



////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This object manages a reference-counted block of memory containing
// a specified number of objects of a given type.
//
// @tcarg class | TObj | Type of objects to allocate.
//  
// @base public | CVisMemBlock
//
// @xref <c CVisMemBlock> <c CVisRefCntMemBlockOf>
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
class CVisMemBlockOf : public CVisMemBlock
{
// @access Public Members
public:
    //------------------------------------------------------------------
	// @group Constructor and Destructor

    // @cmember
	// Allocate a block of memory containing a specified number of
	// objects of a given type using the C++ <f new> operator.
    CVisMemBlockOf(int cObj);

    // @cmember
	// Copy Constructor.
    CVisMemBlockOf(const CVisMemBlockOf<TObj>& refmemblock);

    // @cmember
	// Default Constructor.
    CVisMemBlockOf(void);

    // @cmember
	// Assignment operator.
    CVisMemBlockOf<TObj>& operator=(
			const CVisMemBlockOf<TObj>& refmemblock);

    //------------------------------------------------------------------
	// @group Data Access

	// @cmember
	// Get a pointer to the first object in the
	// memory block controlled by this object.
	TObj *PObj(void) const;

	// @cmember
	// Get a pointer to the specified object in the
	// memory block controlled by this object.
	TObj *PObj(int iObj) const;

	// @cmember
	// Get the number of objects in the memory block controlled by
	// this object.
	int CObj(void) const;
};


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This reference-counted object may be given a pointer to a block of
// memory or it may allocate a block of memory of a specified size.
// If it allocates memory, it will free the memory allocated when
// it is destroyed.  This object is contained in the <c CMemBlock>
// class.  Users of the library should probably use the <c CMemBlock>
// class instead of using this class directly.
//
// @xref <c CVisMemBlock> <c CVisRefCntMemBlockOf>
//
////////////////////////////////////////////////////////////////////////////
class VisCoreExport CVisRefCntMemBlock
{
// @access Public Members
public:
    //------------------------------------------------------------------
	// @group Constructor and Destructor
  
    // @cmember
	// Construct a storage object using a pointer to a block of
	// memory allocated outside of this object.  When this object
	// is destroyed, the <p pfnCallback> function will be called
	// to destroy the memory block allocated outside of this object.
	// If <p pbData> is NULL, the constructor will throw a
	// <c CVisMemoryError> exception.
    CVisRefCntMemBlock(void *pbData, int cbData,
			VisMemBlockCallback pfnCallback = 0, void *pvUser = 0,
			int evismemblock = evismemblockAllocUserUnknown);
  
    // @cmember
	// Construct a storage object using a handle to a file mapping
	// object and an optional offset to the start of the data in
	// the file mapping object.  When this object is destroyed
	// the <p CloseHandle> function will be called to destroy
	// the handle.  If <p hFileMapping> is NULL, the constructor
	// will throw a <c CVisMemoryError> exception.
 	// Since <c HANDLE> is <c void *>, we require that both
	// <p cbDat> and <p ibOffset> need to be specified when
	// using this constructor.
    CVisRefCntMemBlock(HANDLE hFileMapping, int cbData,
			int ibOffset);

	CVisRefCntMemBlock(int cbData, int ibOffset, 
					   unsigned long ulFileMappingID,
					   unsigned __int64 ulFileMappingAppID);
    
	// @cmember
	// Allocate a memory block of the specified size in bytes.  The
	// allocator used can be specified in the optional <p opt>
	// parameter.
    CVisRefCntMemBlock(int cbData,
			int evismemblock = evismemblockAllocDefault);

    // @cmember
	// Destructor called when this object is destroyed.  If this
	// object allocated memory in its constructor, that memory
	// will be freed when the destructor is called.  The destructor
	// is virtual so that other classes can be derived from this
	// class.
    virtual ~CVisRefCntMemBlock(void);

    //------------------------------------------------------------------
	// @group Reference counting

	// @cmember
	// Add a reference to this object by increasing the internal
	// reference count.
	unsigned long AddRef(void);

	// @cmember
	// Release a reference to this object by decreasing the internal
	// reference count.  If the reference count falls to zero, this
	// object will be deleted.
	unsigned long Release(void);

    //------------------------------------------------------------------
	// @group Data Access

	// @cmember
	// Get a pointer to the memory block controlled by this object.
	BYTE *PbData(void) const;

	// @cmember
	// Get the size, in <t BYTE>s, of the memory block controlled by
	// this object.
	int CbData(void) const;

	// @cmember
	// Get a handle to the file mapping object allocated for this
	// memory block.  Returns zero if this block was not allocated
	// using the <f CreateFileMapping> function.
	HANDLE HFileMapping(void) const;
 
	// @cmember
	// If this memory was allocated using CreateFileMapping, return
	// the offset to the start of data in the file mapping.
	int IbOffset(void) const;

	// @cmember
	// Return the options specified when this object was created..
	EVisMemBlock MemBlockOptions(void) const;
 

	IDirectDrawSurface* PIDDS(void) const;
	//------------------------------------------------------------------
	// @group Named file mappings

	// @cmember
	// Get an unsigned long identifying the file mapping for shared
	// memory access.
	unsigned long FileMappingID(void) const;

	// @cmember
	// Get an unsigned 64-bit integer identifying this process.
	unsigned __int64 FileMappingAppID(void);

	// @cmember
	// Get a string used as the prefix of file mapping names.
	static const char *FileMappingPrefixString(void);

	// @cmember
	// Fill a string with the name used for this file mapping.
	// (The string string may contain up to 32-characters, including
	// the trailing null character.)
	void GetFileMappingNameString(char *szName);

	// @cmember
	// Fill a string with the name used for a file mapping, given the
	// parameters used to make the name string.
	// (The string string may contain up to 32-characters, including
	// the trailing null character.  The prefix string should not be
	// longer than 8 characters in length.)
	static void FillFileMappingNameString(char *szName,
			unsigned long ulFileMappingID,
			unsigned __int64 ulFileMappingAppID = 0);

	static void FillFileMappingNameString(char *szName,
			const char *szPrefix, unsigned long ulFileMappingID,
			unsigned __int64 ullFileMappingAppID);

	// @cmember
	// Should we use names with file mappings?
	static bool UseNamedFileMappings(void);
	static void SetUseNamedFileMappings(bool f);



 #ifdef _DEBUG
   //------------------------------------------------------------------
	// @group Debugging 
 
	// @cmember
	// Print information to make sure that storage is reclaimed.
 	// This method is only present in the DEBUG build of the library.
	// @todo:(INTERNAL) Add code for PrintStatistics and static var init to the CPP file.
   static void PrintStatistics(FILE *stream = stderr);
#endif // _DEBUG

// @access:(IMPL) Private Members
private:
	//------------------------------------------------------------------
	// @group:(IMPL) Implementation Variables

    // @cmember:(IMPL)
	// Reference count for this object.
    unsigned long m_cRef;
 
    // @cmember:(IMPL)
	// Pointer to the memory block controlled by this object.
    BYTE *m_pbData;
 
	// @cmember:(IMPL)
	// The size, in <t BYTE>s, of the memory block controlled by
	// this object.
	int m_cbData;

    // @cmember:(IMPL)
	// Offset to the start of data in a file mapping.
    int m_ibOffset;

	// @cmember:(IMPL)
	// Handle to a File Mapping object if the memory for this object
	// was allocated using <f CreateFileMapping>, zero otherwise.
	HANDLE m_hFileMapping;

	// @cmember:(IMPL)
	// long ID that allows other apps to access the File Mapping object
	unsigned long m_ulFileMappingID;

	// @cmember:(IMPL)
	// long App ID that allows other apps to check for the process
	unsigned __int64 m_ulFileMappingAppID;
    
	// @cmember:(IMPL)
	// Options specified when this object was created.
    EVisMemBlock m_evismemblock;

	// @cmember:(IMPL)
	// Pointer to function to called to destroy the memory block when
	// this object is deleted.
	VisMemBlockCallback m_pfnCallback;

	// @cmember:(IMPL)
	// User-supplied <t void *> value passed to callback function when
	// this object is deleted.
	void *m_pvUser;

#ifdef _DEBUG
	//------------------------------------------------------------------
	// @group:(IMPL) Debugging Variables

	enum { cMemBlockAllocators = 4 };

	// @cmember:(IMPL)
	// Number of data blocks passed to constructors of these objects.
	// This variable is only present in the debug build of the library.
	static unsigned long s_cBlockExternal;

	// @cmember:(IMPL)
	// Number of non-shared data blocks allocated by these objects.
	// This variable is only present in the debug build of the library.
	static unsigned long s_cBlockAlloc[cMemBlockAllocators + 1];

	// @cmember:(IMPL)
	// Number of data blocks passed to constructors of these objects.
	// This variable is only present in the debug build of the library.
	static unsigned long s_cbExternalTotal;

	// @cmember:(IMPL)
	// Number of non-shared data blocks allocated by these objects.
	// This variable is only present in the debug build of the library.
	static unsigned long s_cbAllocTotal[cMemBlockAllocators + 1];

	// @cmember:(IMPL)
	// Number of times that AddRef was called on all objects of this
	// type.
 	// This variable is only present in the DEBUG build of the library.
	// @devnote Need to check how the total_ref_count member was used
	// in the previous implementation.
	static unsigned long s_cAddRefTotal;

    // @cmember:(IMPL)
	// First <c CVisMemBlock> object in a global list of all
	// <c CVisMemBlock> objects currently in use.
	// This variable is only present in the DEBUG build of the library.
	static CVisRefCntMemBlock *s_pRefCntMemBlockFirst;

    // @cmember:(IMPL)
	// Next <c CVisMemBlock> object in a global list of all
	// <c CVisMemBlock> objects currently in use.
	// This variable is only present in the DEBUG build of the library.
	CVisRefCntMemBlock *m_pRefCntMemBlockNext;
#endif // _DEBUG
};



////////////////////////////////////////////////////////////////////////////
//  
// @class
//  
// This reference-counted object allocate a block of memory containing
// a specified number of objects of a given type.  When this object
// is destroyed, it will delete the objects that it allocated.
//  
// @tcarg class | TObj | Type of objects to allocate.
//  
// @base public | CVisRefCntMemBlock
//
// @xref <c CVisMemBlock> <c CVisRefCntMemBlock>
//
////////////////////////////////////////////////////////////////////////////
template <class TObj>
class CVisRefCntMemBlockOf : public CVisRefCntMemBlock
{
// @access Public Members
public:
    //------------------------------------------------------------------
	// @group Constructor and Destructor

    // @cmember
	// Allocate a block of memory containing a specified number of
	// objects of a given type using the C++ <f new> operator.
    CVisRefCntMemBlockOf(int cObj);

    // @cmember
	// Destructor called when this object is destroyed.  Objects
	// created in the constructor will be freed using the C++
	// <f delete> operator.
    virtual ~CVisRefCntMemBlockOf(void);

    //------------------------------------------------------------------
	// @group Data Access

	// @cmember
	// Get a pointer to the first object in the
	// memory block controlled by this object.
	TObj *PObj(void) const;

	// @cmember
	// Get a pointer to the specified object in the
	// memory block controlled by this object.
	TObj *PObj(int iObj) const;

	// @cmember
	// Get the number of objects in the memory block controlled by
	// this object.
	int CObj(void) const;

// @access:(IMPL) Private Members
private:
    //------------------------------------------------------------------
	// @group:(IMPL) Implementation Variables
 
	// @cmember:(IMPL)
	// Get the number of objects in the memory block controlled by
	// this object.
	int m_cObj;

};


#ifndef VIS_NO_SD_STREAM
VIS_DECLARE_SD_CLASS_NO_IO(VisCoreExportDeclspec, CVisMemBlock)
#endif // !VIS_NO_SD_STREAM


#include "VisMemBlock.inl"


#endif // __VIS_CORE_MEMBLOCK_H__
