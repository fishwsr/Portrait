///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISMEMBLOCK
//
// @module VisMemBlock.cpp |
//
// Reference counted memory blocks.  Large (not inline) functions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisMemBlock\.h> <l VisMemBlock\.inl>
//
// <nl>
// @index | VISMEMBLOCK
//
///////////////////////////////////////////////////////////////////////////



// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"


#ifdef VIS_MEMBLOCK_USE_IMALLOC
#include <objbase.h>	// For OLE task allocator
#endif // VIS_MEMBLOCK_USE_IMALLOC


#include <assert.h>
#include <stdio.h>      // needed by PrintStatistics

#include "VisMemBlock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


VisCoreExport CVisMemBlock VisCreateMemBlock(int cbSize, int ibOffset, unsigned long ulFileMappingID,
										unsigned __int64 ulFileMappingAppID)
{
	CVisMemBlock memblock(cbSize, ibOffset, ulFileMappingID, ulFileMappingAppID);
	return memblock;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisMemBlock | CVisMemBlock |
//
// Construct a storage object by allocating a block of memory, using
// a pointer to a block of memory allocated outside of this object,
// using a handle to a file mapping object, or using an other
// <c CVisMemBlock> object.
//
// @syntax CVisMemBlock(void *pbData, int cbData,
//		VisMemBlockCallback pfnCallback = 0, void *pvUser = 0);
// @syntax CVisMemBlock(HANDLE hFileMapping, int cbData, int ibOffset);
// @syntax CVisMemBlock(int cbData, int opt = optDefault);
// @syntax CVisMemBlock(const CVisMemBlock& refmemblock);
// @syntax CVisMemBlock(void);
//
// @parm BYTE * | pbData |
// Pointer to a block of memory allocated outside of this object.  If
// <p pbData> is NULL, the constructor will throw a
// <c CVisMemoryError> exception.
//
// @parm HANDLE | hFileMapping |
// Handle to a file mapping created outside of this object.
// When this object is destroyed
// the <p CloseHandle> function will be called to destroy
// the handle.  If <p hFileMapping> is NULL, the constructor
// will throw a <c CVisMemoryError> exception.
// Since <c HANDLE> is <c void *>, we require that both
// <p cbDat> and <p ibOffset> need to be specified when
// using this constructor.
//
// @parmopt int | ibOffset | 0 |
// The offset to use when getting a block of memory from a the file
// mapping specified by <p hFileMapping>.
//
// @parm int | cbData |
// The size of the memory block pointed to by <p pbData> or referenced
// by <p hFileMapping>, or the size of the memory block to be allocated.
//
// @parmopt VisMemBlockCallback | pfnCallback | 0 |
// Pointer to a callback function to be called when this <c CVisMemBlock>
// object is destroyed.  This may be NULL if no callback is needed.
//
// @parmopt void * | pvUser | 0 |
// Pointer to user information.  This pointer is passed to the callback
// function specified by <p pfnCallback>.
//
// @parmopt int | opt | optDefault |
// The options to use when creating this <c CVisMemBlock> object.
//
// @parm const | CVisMemBlock& refmemblock |
// A <c CVisMemBlock> object to be used when constructing this object.
// (This <c CVisMemBlock> object will use the same memory block as
// <p refmemblock>.)
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisMemBlock::CVisMemBlock(void *pbData, int cbData,
		VisMemBlockCallback pfnCallback, void *pvUser, int evismemblock)
  : m_pRefCntMemBlock(0)
{
	m_pRefCntMemBlock = new CVisRefCntMemBlock(pbData, cbData, pfnCallback,
			pvUser, evismemblock);

	if (m_pRefCntMemBlock == 0)
	{
		throw CVisMemoryError("CVisMemBlock::CVisMemBlock"
				"(BYTE *, int, VisMemBlockCallback, void *)",
				__FILE__,__LINE__);
	}
}

VisCoreExport CVisMemBlock::CVisMemBlock(HANDLE hFileMapping, int cbData,
		int ibOffset)
  : m_pRefCntMemBlock(0)
{
	m_pRefCntMemBlock = new CVisRefCntMemBlock(hFileMapping, cbData,
			ibOffset);

	if (m_pRefCntMemBlock == 0)
	{
		throw CVisMemoryError("CVisMemBlock::CVisMemBlock"
				"(HANDLE, int, int)",
				__FILE__,__LINE__);
	}
}

CVisMemBlock::CVisMemBlock(int cbData, int ibOffset, unsigned long ulFileMappingID,
							unsigned __int64 ulFileMappingAppID)
  : m_pRefCntMemBlock(0)
{
	m_pRefCntMemBlock = new CVisRefCntMemBlock(cbData, ibOffset, ulFileMappingID, 
												ulFileMappingAppID);

	if (m_pRefCntMemBlock == 0)
	{
		throw CVisMemoryError("CVisMemBlock::CVisMemBlock"
				"(int, int, unsigned long, unsigned __int64)",
				__FILE__,__LINE__);
	}
}

VisCoreExport CVisMemBlock::CVisMemBlock(int cbData, int opt)
  : m_pRefCntMemBlock(0)
{
	m_pRefCntMemBlock = new CVisRefCntMemBlock(cbData, opt);

	if (m_pRefCntMemBlock == 0)
	{
		throw CVisMemoryError("CVisMemBlock::CVisMemBlock"
				"(int, int)",
				__FILE__,__LINE__);
	}
}

VisCoreExport CVisMemBlock::CVisMemBlock(IDirectDrawSurface* pIDDS)
  : m_pRefCntMemBlock(0)
{
	DDSURFACEDESC ddsdMemoryCheck;
	ddsdMemoryCheck.dwSize = sizeof(DDSURFACEDESC);

    ddsdMemoryCheck.dwFlags = DDSD_CAPS;
 	
	HRESULT hr = pIDDS->GetSurfaceDesc(&ddsdMemoryCheck);
	assert(hr == DD_OK);
	DWORD dwStyle = ddsdMemoryCheck.ddsCaps.dwCaps;

	if((dwStyle & DDSCAPS_OFFSCREENPLAIN) && (dwStyle & DDSCAPS_SYSTEMMEMORY))
	{
		DDSURFACEDESC& ddsdSysMemory = ddsdMemoryCheck;
		ZeroMemory(&ddsdSysMemory, sizeof(DDSURFACEDESC));
		ddsdSysMemory.dwSize = sizeof(DDSURFACEDESC);
		ddsdSysMemory.dwFlags = DDSD_LPSURFACE | DDSD_WIDTH | DDSD_HEIGHT | 
								DDSD_PIXELFORMAT | DDSD_PITCH;

		hr = pIDDS->Lock(NULL, &ddsdSysMemory,
						DDLOCK_READONLY | DDLOCK_SURFACEMEMORYPTR | DDLOCK_NOSYSLOCK, 
						NULL);
		assert(hr == DD_OK);

		int cbitPixel = ddsdSysMemory.ddpfPixelFormat.dwRGBBitCount;
		int cbSize = (ddsdSysMemory.lPitch * ddsdSysMemory.dwHeight * cbitPixel) / 8;
		m_pRefCntMemBlock = new CVisRefCntMemBlock(ddsdSysMemory.lpSurface, 
												   cbSize, 0, (void *)pIDDS, 
												   evismemblockAllocDirectDrawSurface);
		pIDDS->Unlock(NULL);
	}

}

VisCoreExport CVisMemBlock::CVisMemBlock(const CVisMemBlock& refmemblock)
  : m_pRefCntMemBlock(refmemblock.m_pRefCntMemBlock)
{
	assert(&refmemblock != 0);

	if (m_pRefCntMemBlock != 0)
		m_pRefCntMemBlock->AddRef();
}

VisCoreExport CVisMemBlock::CVisMemBlock(void)
  : m_pRefCntMemBlock(0)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisMemBlock& | CVisMemBlock | operator= |
//
// Assignment operator.
//
// @parm const | CVisMemBlock& refmemblock |
// A <c CVisMemBlock> object to be copied.
// (This <c CVisMemBlock> object will use the same memory block as
// <p refmemblock>.)
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisMemBlock& CVisMemBlock::operator=(
		const CVisMemBlock& refmemblock)
{
	assert(&refmemblock != 0);

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
// @mfunc | CVisRefCntMemBlock | CVisRefCntMemBlock |
//
// Construct a storage object using a pointer to a block of
// memory allocated outside of this object.  When this object
// is destroyed, the <p pfnCallback> function will be called
// to destroy the memory block allocated outside of this object.
// If <p pbData> is NULL, the constructor will throw a
// <c CVisMemoryError> exception.
// 
// @todo:(INTERNAL) add parmeter descriptions to function definitions.
//
////////////////////////////////////////////////////////////////////////////
CVisRefCntMemBlock::CVisRefCntMemBlock(void *pbData, int cbData,
		VisMemBlockCallback pfnCallback, void *pvUser, int evismemblock)
  : m_cRef(1),
	m_pbData((BYTE *) pbData),
	m_cbData(cbData),
	m_evismemblock((EVisMemBlock) evismemblock),
	m_hFileMapping(0),
	m_ibOffset(0),
	m_pfnCallback(pfnCallback),
	m_pvUser(pvUser),
	m_ulFileMappingID(0)
{
	if ((m_evismemblock != evismemblockAllocNewTemplate)  // Derived class
			&& (m_evismemblock != evismemblockAllocUserVolatileBuf)
			&& (m_evismemblock != evismemblockAllocUserDMABuf)
			&& (m_evismemblock != evismemblockAllocUserUnknown)
			&& (m_evismemblock != evismemblockAllocDirectDrawSurface))
	{
		assert(0);  // Bad evismemblock value passed to constructor.
		m_evismemblock = evismemblockAllocUserUnknown;
	}

	if (m_pbData == 0)
	{
		throw CVisMemoryError("CVisRefCntMemBlock::CVisRefCntMemBlock"
				"(BYTE *, int, VisMemBlockCallback, void *)",
				__FILE__,__LINE__);
	}

#ifdef _DEBUG
	// Add this memory block to the globals list of all memory blocks
	// currently in use.
	// LATER:  Do we want to use a static synchronization object instead of
	// a named mutex?
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisRefCntMemBlockDebugMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pRefCntMemBlockNext = s_pRefCntMemBlockFirst;
	s_pRefCntMemBlockFirst = this;

	// Increment the count of blocks of this type.
	++ s_cBlockExternal;
	s_cbExternalTotal += m_cbData;

	// Implicit AddRef in constructor.
	++ s_cAddRefTotal;

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
#endif // _DEBUG
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRefCntMemBlock | CVisRefCntMemBlock |
//
// Construct a storage object using a handle to a file mapping
// object and an optional offset to the start of the data in
// the file mapping object.  When this object is destroyed
// the <p CloseHandle> function will be called to destroy
// the handle.  If <p hFileMapping> is NULL, the constructor
// will throw a <c CVisMemoryError> exception.
//
// @parm HANDLE | hFileMapping |
// 
//
// @parm int | cbData |
// 
//
// @parmopt int | ibOffset | 0 |
// 
//
////////////////////////////////////////////////////////////////////////////
CVisRefCntMemBlock::CVisRefCntMemBlock(HANDLE hFileMapping, int cbData,
		int ibOffset)
  : m_cRef(1),
	m_pbData(0),
	m_cbData(cbData),
	m_evismemblock(evismemblockAllocFileMapping),
	m_hFileMapping(hFileMapping),
	m_ibOffset(ibOffset),
	m_pfnCallback(0),
	m_pvUser(0),
	m_ulFileMappingID(0)
{
	if (m_hFileMapping != 0)
	{
		m_pbData = (BYTE *) MapViewOfFile(m_hFileMapping,
				FILE_MAP_ALL_ACCESS, 0, m_ibOffset, m_cbData);

		if (m_pbData == 0)
		{
			// Clean-up before throwing exception below.
			CloseHandle(m_hFileMapping);
			m_hFileMapping = 0;
		}
	}

	if (m_pbData == 0)
	{
		throw CVisMemoryError("CVisRefCntMemBlock::CVisRefCntMemBlock"
				"(HANDLE, int, int)",
				__FILE__,__LINE__);
	}

#ifdef _DEBUG
	// Add this memory block to the globals list of all memory blocks
	// currently in use.
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisRefCntMemBlockDebugMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pRefCntMemBlockNext = s_pRefCntMemBlockFirst;
	s_pRefCntMemBlockFirst = this;

	// Increment the count of blocks of this type.
	++ s_cBlockAlloc[m_evismemblock];
	s_cbAllocTotal[m_evismemblock] += m_cbData;

	// Implicit AddRef in constructor.
	++ s_cAddRefTotal;

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
#endif // _DEBUG
}

CVisRefCntMemBlock::CVisRefCntMemBlock(int cbData, int ibOffset, 
									   unsigned long ulFileMappingID,
									   unsigned __int64 ulFileMappingAppID)
  : m_cRef(1),
	m_pbData(0),
	m_cbData(cbData),
	m_evismemblock(evismemblockAllocFileMapping),
	m_hFileMapping(0),
	m_ibOffset(ibOffset),
	m_pfnCallback(0),
	m_pvUser(0),
	m_ulFileMappingID(ulFileMappingID),
	m_ulFileMappingAppID(ulFileMappingAppID)
{
	char szBuf[32];
	FillFileMappingNameString(szBuf, FileMappingPrefixString(), 
							  ulFileMappingID, ulFileMappingAppID);

	m_hFileMapping = CreateFileMapping((HANDLE) 0xffffffff, 0,
			PAGE_READWRITE, 0, m_cbData + 8, szBuf);

	assert(m_hFileMapping != 0);

	m_pbData = (BYTE *) MapViewOfFile(m_hFileMapping,
			FILE_MAP_ALL_ACCESS, 0, 0, 0);

	assert(m_pbData != 0);
}


// Helper function (used to put numbers into the names of file mappings).
// Eight numbers will be written.  No '\0' charater will be written at
// the end of the string.
void VisUlToSz(unsigned long ul, char *pch)
{
	assert(pch != 0);
	for (int i = 0; i < 8; ++i)
	{
		char ch = '0' + (ul & 0xf);
		if (ch > '9')
			ch += ('A' - '9' - 1);
		pch[i] = ch;

		ul >>= 4;
	}
}

void VisUllToSz(unsigned __int64& refull, char *pch)
{
	assert(sizeof(__int64) == 2 * sizeof(long));

	VisUlToSz(((unsigned long *) &refull)[0], pch);
	VisUlToSz(((unsigned long *) &refull)[1], pch + 8);
}

unsigned __int64 CVisRefCntMemBlock::FileMappingAppID(void)
{
	if (m_ulFileMappingAppID == 0)
	{
		m_ulFileMappingAppID = PGlobals()->FileMappingAppID();
	}
	return m_ulFileMappingAppID;
}

const char *CVisRefCntMemBlock::FileMappingPrefixString(void)
{
	return "VisMB";
}

void CVisRefCntMemBlock::FillFileMappingNameString(char *szName,
		const char *szPrefix, unsigned long ulFileMappingID,
		unsigned __int64 ullFileMappingAppID)
{
	assert(szName != 0);
	assert(szPrefix != 0);
	assert(strlen(szPrefix) < 8);

	strcpy(szName, szPrefix);
	char *pch = szName + strlen(szName);

	VisUlToSz(ulFileMappingID, pch);
	pch += 8;

	VisUllToSz(ullFileMappingAppID, pch);
	pch += 16;

	*pch = '\0';
}


bool CVisRefCntMemBlock::UseNamedFileMappings(void)
{
	return PGlobals()->UseNamedFileMappings();
}

void CVisRefCntMemBlock::SetUseNamedFileMappings(bool f)
{
	PGlobals()->SetUseNamedFileMappings( f );
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRefCntMemBlock | CVisRefCntMemBlock |
//
// Allocate a memory block of the specified size in bytes.  If
// <p fShared> is TRUE, the memory will be allocated with the
// <f CreateFileMapping> function.  Otherwise, the memory will
// be allocated using <f GlobalAlloc>.
//
////////////////////////////////////////////////////////////////////////////
CVisRefCntMemBlock::CVisRefCntMemBlock(int cbData, int evismemblock)
  : m_cRef(1),
	m_pbData(0),
	m_cbData(cbData),
	m_evismemblock((EVisMemBlock) evismemblock),
	m_hFileMapping(0),
	m_ibOffset(0),
	m_ulFileMappingID(0),
	m_pfnCallback(0),
	m_pvUser(0)
{
	assert(cbData > 0);
	if((m_cbData + 8) <= PGlobals()->CbImageSizeLimit())
	{
		if ((m_evismemblock != evismemblockAllocFileMapping)
				&& (m_evismemblock != evismemblockAllocGlobalAlloc)
#ifdef VIS_MEMBLOCK_USE_IMALLOC
				&& (m_evismemblock != evismemblockAllocIMalloc)
#endif // VIS_MEMBLOCK_USE_IMALLOC
				&& (m_evismemblock != evismemblockAllocNewDouble))
		{
			assert(0);  // Bad evismemblock value passed to constructor.
			m_evismemblock = evismemblockAllocDefault;
		}
		
		// UNDONE:  Add an option to use VirtualAlloc?
		if (m_evismemblock == evismemblockAllocFileMapping)
		{
			// Allocate memory using CreateFileMapping.
			
			// Create a fairly unique name to apply to the shared file that
			// can be easily passed to other apps for the most robust access
			// to this data.  (Use a string with <= 32 characters, incl. null.)
			//
			// UNDONE:  Should we have a global option to specify whether
			// or not names should be used with file mappings?  (Does the
			// use of names affect performance?)
			//
			// UNDONE:  In theory, we may get two names that are not unique.
			// We can store dwId and the string used to create the name in an
			// application global to handle the case that two threads start
			// allocating images at the same time.

			// Update the count of file mappings.
			// LATER:  Should we skip this if we're not using named mappings?
			long lFileMappingID = PGlobals()->NextFileMappingID();

			// Assert that we don't create more than 2^32 file mappings.
			// (If we do, we should get a new tick count to use in the app ID.)
			assert(lFileMappingID != 0);

			char szBuf[32];
			const char *szFileMappingName = 0;
			if (UseNamedFileMappings())
			{
				m_ulFileMappingID = (unsigned long) lFileMappingID;
				GetFileMappingNameString(szBuf);
				szFileMappingName = szBuf;
			}
			else
			{
				szFileMappingName = 0;
			}

			// Note:  Because of a problem with templates in the functions that
			// copy pixels between images, we add 8 bytes to m_cbData to
			// prevent access violations at the end of memory blocks.
			// LATER:  Don't add these 8 bytes after the problem is fixed.
			m_hFileMapping = CreateFileMapping((HANDLE) 0xffffffff, 0,
					PAGE_READWRITE, 0, m_cbData + 8, szFileMappingName);

			// Check that this name is unique.
			assert(GetLastError() != ERROR_ALREADY_EXISTS);

			if (m_hFileMapping != 0)
			{
				m_pbData = (BYTE *) MapViewOfFile(m_hFileMapping,
						FILE_MAP_ALL_ACCESS, 0, 0, 0);
			}

			if (m_pbData == 0)
			{
				// Clean-up before throwing exception below.
				CloseHandle(m_hFileMapping);
				m_hFileMapping = 0;
			}
		}
		else if (m_evismemblock == evismemblockAllocNewDouble)
		{
			// Allocate memory using operator new.
			// Note:  Because of a problem with templates in the functions that
			// copy pixels between images, we add 8 bytes to m_cbData to
			// prevent access violations at the end of memory blocks.
			// LATER:  Don't add these 8 bytes after the problem is fixed.
			m_pbData = (BYTE *) new
					double[(m_cbData + sizeof(double) - 1) / sizeof(double) + 1];
		}
#ifdef VIS_MEMBLOCK_USE_IMALLOC
		else if (m_evismemblock == evismemblockAllocIMalloc)
		{
			// Allocate memory using the OLE task allocator.
			// Note:  Because of a problem with templates in the functions that
			// copy pixels between images, we add 8 bytes to m_cbData to
			// prevent access violations at the end of memory blocks.
			// LATER:  Don't add these 8 bytes after the problem is fixed.
			m_pbData = (BYTE *) CoTaskMemAlloc(m_cbData + 8);
		}
#endif // VIS_MEMBLOCK_USE_IMALLOC
		else
		{
			// Allocate memory using GlobalAlloc.
			// Note:  Because of a problem with templates in the functions that
			// copy pixels between images, we add 8 bytes to m_cbData to
			// prevent access violations at the end of memory blocks.
			// LATER:  Don't add these 8 bytes after the problem is fixed.
			assert(m_evismemblock == evismemblockAllocGlobalAlloc);
			m_pbData = (BYTE *) GlobalAlloc(GPTR, m_cbData + 8);
		}
	}
	if (m_pbData == 0)
	{
		throw CVisMemoryError("CVisRefCntMemBlock::CVisRefCntMemBlock"
				"(int, int)",
				__FILE__,__LINE__);
	}

#ifdef _DEBUG
	// Add this memory block to the globals list of all memory blocks
	// currently in use.
	HANDLE hMutex =  CreateMutex(0, FALSE, "CVisRefCntMemBlockDebugMutex");
	assert(hMutex != 0);
	VisWaitForSingleObject(hMutex, INFINITE);

	m_pRefCntMemBlockNext = s_pRefCntMemBlockFirst;
	s_pRefCntMemBlockFirst = this;

	// Increment the count of blocks of this type.
	++ s_cBlockAlloc[m_evismemblock];
	s_cbAllocTotal[m_evismemblock] += m_cbData;

	// Implicit AddRef in constructor.
	++ s_cAddRefTotal;

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
#endif // _DEBUG
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisRefCntMemBlock | ~CVisRefCntMemBlock |
//
// Destructor called when this object is destroyed.  If this
// object allocated memory in its constructor, that memory
// will be freed when the destructor is called.  The destructor
// is virtual so that other classes can be derived from this
// class.
//
////////////////////////////////////////////////////////////////////////////
CVisRefCntMemBlock::~CVisRefCntMemBlock(void)
{
#ifdef _DEBUG
	if (m_pbData != 0)
	{
		// Decrement the count of blocks of this type.
		if ((m_evismemblock != evismemblockAllocFileMapping)
				&& (m_evismemblock != evismemblockAllocGlobalAlloc)
#ifdef VIS_MEMBLOCK_USE_IMALLOC
			&& (m_evismemblock != evismemblockAllocIMalloc)
#endif // VIS_MEMBLOCK_USE_IMALLOC
			&& (m_evismemblock != evismemblockAllocNewDouble))
		{
			-- s_cBlockExternal;
			s_cbExternalTotal -= m_cbData;
		}
		else
		{
			assert((m_evismemblock > 0) && (m_evismemblock <= cMemBlockAllocators));
			-- s_cBlockAlloc[m_evismemblock];
			s_cbAllocTotal[m_evismemblock] -= m_cbData;
		}

		// Ref count may not be zero if delete is used to destroy this
		// object.
		s_cAddRefTotal -= m_cRef;

		// Remove this memory block to the globals list of all memory blocks
		// currently in use.
		HANDLE hMutex =  CreateMutex(0, FALSE, "CVisRefCntMemBlockDebugMutex");
		assert(hMutex != 0);
		VisWaitForSingleObject(hMutex, INFINITE);

		CVisRefCntMemBlock *pRefCntMemBlockPrev = 0;
		CVisRefCntMemBlock *pRefCntMemBlock = s_pRefCntMemBlockFirst;
		while ((pRefCntMemBlock != this) && (pRefCntMemBlock != 0))
		{
			pRefCntMemBlockPrev = pRefCntMemBlock;
			pRefCntMemBlock = pRefCntMemBlock->m_pRefCntMemBlockNext;
		}

		if (pRefCntMemBlock != 0)
		{
			if (pRefCntMemBlockPrev == 0)
			{
				s_pRefCntMemBlockFirst = m_pRefCntMemBlockNext;
				m_pRefCntMemBlockNext = 0;
			}
			else
			{
				pRefCntMemBlockPrev->m_pRefCntMemBlockNext
						= m_pRefCntMemBlockNext;
				m_pRefCntMemBlockNext = 0;
			}
		}

		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
#endif // _DEBUG

	if (m_evismemblock == evismemblockAllocFileMapping)
	{
		// Free memory allocated using CreateFileMapping.
		assert(m_pbData != 0);
		UnmapViewOfFile(m_pbData);
		CloseHandle(m_hFileMapping);
		m_hFileMapping = 0;
	}
	else if (m_evismemblock == evismemblockAllocNewDouble)
	{
		// Free memory allocated using operator new.
		delete [] (double *) m_pbData;
	}
	else if (m_evismemblock == evismemblockAllocGlobalAlloc)
	{
		// Free memory allocated using GlobalAlloc.
		GlobalFree(m_pbData);
	}
#ifdef VIS_MEMBLOCK_USE_IMALLOC
	else if (m_evismemblock == evismemblockAllocIMalloc)
	{
		// Allocate memory using the OLE task allocator.
		CoTaskMemFree(m_pbData);
	}
#endif // VIS_MEMBLOCK_USE_IMALLOC
	else if (m_evismemblock == evismemblockAllocDirectDrawSurface)
	{
		// Release memory allocated using DirectDrawSurface
		assert(m_pvUser != 0);
		IDirectDrawSurface* pDDS = (IDirectDrawSurface *)m_pvUser;
		pDDS->Release();
	}
	else if (m_pfnCallback != 0)
	{
		m_pfnCallback(m_pbData, m_pvUser);
	}

	m_pbData = 0;
}


#ifdef _DEBUG
////////////////////////////////////////////////////////////////////////////
//
// Static Debug Variables.
//
////////////////////////////////////////////////////////////////////////////

// Number of data blocks passed to constructors of these objects.
// This variable is only present in the debug build of the library.
VisCoreExport unsigned long CVisRefCntMemBlock::s_cBlockExternal = 0;

// Number of non-shared data blocks allocated by these objects.
// This variable is only present in the debug build of the library.
VisCoreExport unsigned long CVisRefCntMemBlock::s_cBlockAlloc[cMemBlockAllocators + 1]
		= {0, 0, 0, 0, 0};

// Number of data blocks passed to constructors of these objects.
// This variable is only present in the debug build of the library.
VisCoreExport unsigned long CVisRefCntMemBlock::s_cbExternalTotal = 0;

// Number of non-shared data blocks allocated by these objects.
// This variable is only present in the debug build of the library.
VisCoreExport unsigned long CVisRefCntMemBlock::s_cbAllocTotal[cMemBlockAllocators + 1]
		= {0, 0, 0, 0, 0};

// Number of times that AddRef was called on all objects of this
// type.
// This variable is only present in the debug build of the library.
VisCoreExport unsigned long CVisRefCntMemBlock::s_cAddRefTotal = 0;

// First <c CVisRefCntMemBlock> object in a global list of all
// <c CVisRefCntMemBlock> objects currently in use.
// This variable is only present in the debug build of the library.
VisCoreExport CVisRefCntMemBlock *CVisRefCntMemBlock::s_pRefCntMemBlockFirst = 0;


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisMemBlock | PrintStatistics |
//
// Print information to make sure that storage is reclaimed.
// This method is only present in the debug build of the library.
// @todo:(INTERNAL) The PrintStatistics needs to be finished.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisMemBlock::PrintStatistics(FILE *stream)
{
	CVisRefCntMemBlock::PrintStatistics(stream);
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc void | CVisRefCntMemBlock | PrintStatistics |
//
// Print information to make sure that storage is reclaimed.
// This method is only present in the debug build of the library.
// @todo:(INTERNAL) The PrintStatistics needs to be finished.
//
////////////////////////////////////////////////////////////////////////////
void CVisRefCntMemBlock::PrintStatistics(FILE *stream)
{
	if (stream)
	{
		fprintf(stream, "%d references total.\n", s_cAddRefTotal);
		fprintf(stream,
				"%lu external memory blocks use %lu bytes of storage.\n",
				s_cBlockExternal, s_cbExternalTotal);
		fprintf(stream,
				"%lu file mapping memory blocks use %lu bytes of storage.\n",
				s_cBlockAlloc[evismemblockAllocFileMapping],
				s_cbAllocTotal[evismemblockAllocFileMapping]);
		fprintf(stream,
				"%lu GlobalAlloc memory blocks use %lu bytes of storage.\n",
				s_cBlockAlloc[evismemblockAllocGlobalAlloc],
				s_cbAllocTotal[evismemblockAllocGlobalAlloc]);
		fprintf(stream,
				"%lu operator new memory blocks use %lu bytes of storage.\n",
				s_cBlockAlloc[evismemblockAllocNewDouble],
				s_cbAllocTotal[evismemblockAllocNewDouble]);
#ifdef VIS_MEMBLOCK_USE_IMALLOC
		fprintf(stream,
				"%lu OLE IMalloc memory blocks use %lu bytes of storage.\n",
				s_cBlockAlloc[evismemblockAllocIMalloc],
				s_cbAllocTotal[evismemblockAllocIMalloc]);
#endif // VIS_MEMBLOCK_USE_IMALLOC
	}
}
#endif // _DEBUG

VIS_DEFINE_SD_CLASS_NO_IO(VisCoreExportDeclspec, CVisMemBlock)
