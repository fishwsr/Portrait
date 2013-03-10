// VisGlobals.cpp
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef VIS_USE_DXMEDIA
	#include "dxtransguid.h"
#endif // VIS_USE_DXMEDIA

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// UNDONE:  Get from DirectX instead of defining here.
const IID IID_IDirectDraw3
	= { 0x618f8ad4, 0x8b7a, 0x11d0, 0x8f, 0xcc, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0x9d};


extern CVisGlobals gvis_globals;


CVisGlobals::CVisGlobals(void)
{
	InitializeWithDefault();
}

CVisGlobals::~CVisGlobals(void)
{
	Cleanup();
}

void CVisGlobals::Cleanup(void)
{
	if(m_abulNibbleColorMapGray)
	{
		delete[] m_abulNibbleColorMapGray;
		m_abulNibbleColorMapGray = 0;
	}
	if(m_abulNibbleColorMapDefault)
	{
		delete[] m_abulNibbleColorMapDefault;
		m_abulNibbleColorMapDefault = 0;
	}
	if(m_abulByteColorMapGray)
	{
		delete[] m_abulByteColorMapGray;
		m_abulByteColorMapGray = 0;
	}

	if(m_szAppName)
	{
		delete m_szAppName;
		m_szAppName = 0;
	}

	if(m_listUserFunctions)
	{
		m_listUserFunctions->clear();
		delete m_listUserFunctions;
		m_listUserFunctions = 0;
	}
	
	if(m_pLogFile)
	{
		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}

	if(m_szLogFileName)
	{
		delete[] m_szLogFileName;
		m_szLogFileName = 0;
	}

	if(m_pvisconsoleDiagnostics) 
	{
		delete m_pvisconsoleDiagnostics;
		m_pvisconsoleDiagnostics = NULL;
	}	
	if(m_pvisproptypeInfomap)
	{
		delete m_pvisproptypeInfomap;
		m_pvisproptypeInfomap = NULL;
	}
	if(m_nInternalFileHandlers != 0)
		ClearFileHandlers();

	ReleasePictureFileHandler();

	if (m_hModuleIpl)
	{
		FreeLibrary(m_hModuleIpl);
		m_hModuleIpl = 0;
		m_pfnIplCreateImageHeader = 0;
		m_pfnIplDeallocateHeader = 0;
	}

	if(DDrawInitialized())
	{
		m_pDD->Release();
		m_pDD = NULL;
	}
#ifdef VIS_USE_DXMEDIA
	if(DXSFactoryInitialized())
	{
		m_pDXSurfaceFactory->Release();
		m_pDXSurfaceFactory = NULL;
	}
#endif // VIS_USE_DXMEDIA
	if(CoInitialized())
	{
		CoUninitialize();
		m_fCOMInitialized = false;
	}
}

const unsigned long* CVisGlobals::ColorMapGray4bit(void)
{
	if(m_abulNibbleColorMapGray == NULL)
	{
		m_abulNibbleColorMapGray = new unsigned long[16];
		
		m_abulNibbleColorMapGray[0] = knVisAlphaDefault;
		for(int iColor = 1; iColor < 16; iColor ++)
		{
			m_abulNibbleColorMapGray[iColor] = 
					m_abulNibbleColorMapGray[iColor - 1] + 0x111111;
		}
	}
	return m_abulNibbleColorMapGray;
}
	
const unsigned long* CVisGlobals::ColorMapDefault4bit(void)
{
	if(m_abulNibbleColorMapDefault == NULL)
	{
		m_abulNibbleColorMapDefault = new unsigned long[16];

		m_abulNibbleColorMapDefault[0] = 
				(const unsigned long) (0x000000 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[1] = 
				(const unsigned long) (0x0000ff | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[2] = 
				(const unsigned long) (0x00ff00 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[3] = 
				(const unsigned long) (0xff0000 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[4] = 
				(const unsigned long) (0x00ffff | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[5] = 
				(const unsigned long) (0xff00ff | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[6] = 
				(const unsigned long) (0xffff00 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[7] = 
				(const unsigned long) (0xffffff | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[8] = 
				(const unsigned long) (0x808080 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[9] = 
				(const unsigned long) (0x000080 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[10] = 
				(const unsigned long) (0x008000 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[11] = 
				(const unsigned long) (0x800000 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[12] = 
				(const unsigned long) (0x008080 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[13] = 
				(const unsigned long) (0x800080 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[14] = 
				(const unsigned long) (0x808000 | knVisAlphaDefault);
		m_abulNibbleColorMapDefault[15] = 
				(const unsigned long) (0xff0080 | knVisAlphaDefault);
	}
	return m_abulNibbleColorMapDefault;

}
	
const unsigned long* CVisGlobals::ColorMapGray8bit(void)
{
	if(m_abulByteColorMapGray == NULL)
	{
		m_abulByteColorMapGray = new unsigned long[256];

		m_abulByteColorMapGray[0] = knVisAlphaDefault;
		for(int iColor = 1; iColor < 256; iColor ++)
		{
			m_abulByteColorMapGray[iColor] = 
					m_abulByteColorMapGray[iColor - 1] + 0x010101;
		}
	}
	return m_abulByteColorMapGray;
}

const char* CVisGlobals::AppName(void)
{
	if(m_szAppName == NULL)
	{
		char szFullAppName[MAX_PATH];
		int nLength;
		if ((nLength = GetModuleFileName(0, szFullAppName, sizeof(szFullAppName))) != 0)
		{
			char* szAppName = strrchr( szFullAppName, '\\' );
			nLength -= (szAppName - szFullAppName + 1);
			m_szAppName = new char[nLength + 1];
			memcpy(m_szAppName, szAppName + 1, nLength);
			m_szAppName[nLength] = '\0';
		}
	}
	return m_szAppName;
}

DWORD CVisGlobals::ProcessId(void)
{
	if(m_dwProcessId == 0)
	{
		m_dwProcessId = GetCurrentProcessId();
	}
	return m_dwProcessId;
}

void CVisGlobals::AddImSrcProvider(const char* szID, VisPfnGetImSrcProvider pfnCallback,
								   const char* szName)
{
	assert(m_nProvidersAvailable < 8);
	assert((szID != 0) && (pfnCallback != 0) && (szName != 0));
	assert(strlen(szID) < 50);
	assert(strlen(szName) < 256);

	RemoveImSrcProvider(szID, pfnCallback, szName);
	
	m_nProvidersAvailable ++;
	strcpy(m_ProviderList[m_nProvidersAvailable - 1].m_szID, szID);
	m_ProviderList[m_nProvidersAvailable - 1].m_pfnCallback = pfnCallback;
	strcpy(m_ProviderList[m_nProvidersAvailable - 1].m_szName, szName);

}

const char* CVisGlobals::ImSrcProviderID(int index) const
{
	assert(index < m_nProvidersAvailable);
	return (m_ProviderList[index].m_szID);
}

const VisPfnGetImSrcProvider CVisGlobals::ImSrcProviderCallback(int index) const
{
	assert(index < m_nProvidersAvailable);
	return (m_ProviderList[index].m_pfnCallback);
}

const char* CVisGlobals::ImSrcProviderName(int index) const
{
	assert(index < m_nProvidersAvailable);
	return (m_ProviderList[index].m_szName);

}

int CVisGlobals::IGetProvider(const char* szID) const
{
	int index = -1;
	for( int iProvider = 0; iProvider < m_nProvidersAvailable; iProvider++ )
	{
		if(_stricmp(szID, m_ProviderList[iProvider].m_szID) == 0)
		{
			index = iProvider;
			break;
		}
	}
	return index;
}

int CVisGlobals::NImSrcProviders(void) const
{
	return m_nProvidersAvailable;
}

bool CVisGlobals::RemoveImSrcProvider(const VisPfnGetImSrcProvider pfnCallback, 
				 					  const char* szName)
{
	bool fResult = false;
	if(pfnCallback == 0)
		fResult = RemoveImSrcProvider(szName);
	else
	{
		for( int iProvider = 0; iProvider < m_nProvidersAvailable; iProvider++ )
		{
			if((pfnCallback == m_ProviderList[iProvider].m_pfnCallback) &&
				((szName == 0) || (_stricmp(szName, m_ProviderList[iProvider].m_szName) == 0)))
			{
				ClearProvider(iProvider);
				fResult = true;
				break;
			}
		}
	}
	return fResult;
}

bool CVisGlobals::RemoveImSrcProvider(const char* szName)
{
	assert(szName != 0);
	bool fResult = false;
	for( int iProvider = 0; iProvider < m_nProvidersAvailable; iProvider++ )
	{
		if(_stricmp(szName, m_ProviderList[iProvider].m_szName) == 0)
		{
			ClearProvider(iProvider);
			fResult = true;
			break;
		}
	}
	return fResult;
}

bool CVisGlobals::RemoveImSrcProvider(const char* szID, const char* szName)
{
	bool fResult = false;
	if(szID == 0)
		fResult = RemoveImSrcProvider(szName);
	else
	{
		for( int iProvider = 0; iProvider < m_nProvidersAvailable; iProvider++ )
		{
			if((_stricmp(szID, m_ProviderList[iProvider].m_szID) == 0) &&
				((szName == 0) || (_stricmp(szName, m_ProviderList[iProvider].m_szName) == 0)))
			{
				ClearProvider(iProvider);
				fResult = true;
				break;
			}
		}
	}
	return fResult;
}


bool CVisGlobals::RemoveImSrcProvider(const char* szID, 
									  const VisPfnGetImSrcProvider pfnCallback, 
									  const char* szName)
{
	bool fResult = false;
	if(szID == 0)
		fResult = RemoveImSrcProvider(pfnCallback, szName);
	else if (pfnCallback == 0)
		fResult = RemoveImSrcProvider(szID, szName);
	else
	{
		for( int iProvider = 0; iProvider < m_nProvidersAvailable; iProvider++ )
		{
			if((_stricmp(szID, m_ProviderList[iProvider].m_szID) == 0) &&
				(pfnCallback == m_ProviderList[iProvider].m_pfnCallback) &&
				((szName == 0) || (_stricmp(szName, m_ProviderList[iProvider].m_szName) == 0)))
			{
				ClearProvider(iProvider);
				fResult = true;
				break;
			}
		}
	}
	return fResult;
}

void CVisGlobals::ClearProvider(int index)
{
	assert(index < m_nProvidersAvailable);
	if(index != m_nProvidersAvailable - 1)
	{
		strcpy(m_ProviderList[index].m_szID, m_ProviderList[m_nProvidersAvailable - 1].m_szID);
		m_ProviderList[index].m_pfnCallback = m_ProviderList[m_nProvidersAvailable - 1].m_pfnCallback;
		strcpy(m_ProviderList[index].m_szName, m_ProviderList[m_nProvidersAvailable - 1].m_szName);
		index = m_nProvidersAvailable - 1;
	}
	ZeroMemory(m_ProviderList[index].m_szID, 50);
	m_ProviderList[index].m_pfnCallback = 0;
	ZeroMemory(m_ProviderList[index].m_szName, 256);

	m_nProvidersAvailable--;
}

CVisPropTypeInfoMap* CVisGlobals::PropTypeInfoMap(void)
{
	if(m_pvisproptypeInfomap == NULL)
	{
		m_pvisproptypeInfomap = new CVisPropTypeInfoMap();
	}
	return m_pvisproptypeInfomap;
}

CVisPropTypeInfoMap& CVisGlobals::RPropTypeInfoMap(void)
{
	return *(PropTypeInfoMap());
}

void CVisGlobals::InitFileHandlers(void)
{
	if(m_nInternalFileHandlers == 0)
	{
		extern CVisFileHandler *CVisPPMFileHandler_Init();
		AddInternalFileHandler(CVisPPMFileHandler_Init());

		// ImageMagick, if available
		extern CVisFileHandler *Magick_FileHandler_Init();
		AddInternalFileHandler(Magick_FileHandler_Init());

		// Ross Cutler's DIB file I/O handler.
		extern CVisFileHandler *CVisDibFileHandler_Init();
		AddInternalFileHandler(CVisDibFileHandler_Init());

		// Our postscript file handler
		extern CVisFileHandler *CVisPSFileHandler_Init();
		AddInternalFileHandler(CVisPSFileHandler_Init());

		// Our MSV file handler
		extern CVisFileHandler *CVisMSVFileHandler_Init();
		AddInternalFileHandler(CVisMSVFileHandler_Init());

	#ifdef VIS_USE_GENERIC_FILE_HANDLER
		// Generic file handler, either ImageMagick or IFL
		extern CVisFileHandler *Generic_FileHandler_Init();
		AddInternalFileHandler(Generic_FileHandler_Init());
	#endif // VIS_USE_GENERIC_FILE_HANDLER

	}
}


void CVisGlobals::ClearFileHandlers(void)
{
    for (int index = m_nInternalFileHandlers - 1; index >= 0; --index)
	{
        if (m_pfilehandlersInternal[index] != NULL)
		{
            delete m_pfilehandlersInternal[index];
			m_pfilehandlersInternal[index] = NULL;
		}
	}
	m_nInternalFileHandlers = 0;
}

void CVisGlobals::AddInternalFileHandler(CVisFileHandler *pfilehandler)
{
	if((m_nInternalFileHandlers < 10) && (pfilehandler != NULL))
	{
		m_pfilehandlersInternal[m_nInternalFileHandlers++] = pfilehandler;
	}
}

void CVisGlobals::AddExternalFileHandler(CVisFileHandler *pfilehandler)
{
	if((m_nExternalFileHandlers < 20) && (pfilehandler != NULL))
	{
		m_pfilehandlersExternal[m_nExternalFileHandlers++] = pfilehandler;
	}
}

CVisFileHandler* CVisGlobals::FileHandler(const char *szFiletype)
{
	CVisFileHandler *pfilehandler = NULL;
    for (int index = m_nExternalFileHandlers - 1; index >= 0; --index)
	{
        if (m_pfilehandlersExternal[index]->MatchExtension(szFiletype))
		{
            pfilehandler = m_pfilehandlersExternal[index];
			break;
		}
	}

	if(!pfilehandler)
	{
		if(m_nInternalFileHandlers == 0)
			InitFileHandlers();
		for (int index = m_nInternalFileHandlers - 1; index >= 0; --index)
		{
			if (m_pfilehandlersInternal[index]->MatchExtension(szFiletype))
			{
				pfilehandler = m_pfilehandlersInternal[index];
				break;
			}
		}
	}
    return pfilehandler;
}

CVisFileHandler* CVisGlobals::PictureFileHandler(void)
{
	if(m_pPictureFileHandler == 0)
		m_pPictureFileHandler = new CVisPictureFileHandler;

	return m_pPictureFileHandler;
}

void CVisGlobals::ReleasePictureFileHandler(void)
{
	if(m_pPictureFileHandler)
	{
		delete m_pPictureFileHandler;
		m_pPictureFileHandler = 0;
	}
}

bool CVisGlobals::DDrawInitialized(void)
{
	return (m_pDD != NULL) ? true : false;
}

interface IDirectDraw* CVisGlobals::IDDraw(void)
{
	if(!DDrawInitialized())
	{
		InitializeDDraw();
	}
	return (m_pDD);
}

void CVisGlobals::InitializeDDraw(void)
{
	CoInitialize();
	HRESULT hr;

#if DIRECTDRAW_VERSION < 0x0700
	hr = DirectDrawCreate(NULL, &m_pDD, NULL);
#else // DIRECTDRAW_VERSION < 0x0700
 	IDirectDraw7* pDD7 = NULL;
	hr = DirectDrawCreateEx(NULL, (void **)&pDD7, IID_IDirectDraw7, NULL);
	assert(hr == DD_OK);
	hr = pDD7->QueryInterface(IID_IDirectDraw, (void **)&m_pDD);
	pDD7->Release();
#endif // DIRECTDRAW_VERSION < 0x0700
	assert(hr == DD_OK);
	m_pDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
}

bool CVisGlobals::DXSFactoryInitialized(void)
{
	return (m_pDXSurfaceFactory != NULL) ? true : false;
}

interface IDXSurfaceFactory* CVisGlobals::IDXSFactory(void)
{
#ifdef VIS_USE_DXMEDIA
	if(!DXSFactoryInitialized())
	{
		InitializeDXSFactory();
	}
#endif // VIS_USE_DXMEDIA
	return (m_pDXSurfaceFactory);
}

void CVisGlobals::InitializeDXSFactory(void)
{
#ifdef VIS_USE_DXMEDIA
	CoInitialize();
 
 	IDXTransformFactory* pDXTransformFactory = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_DXTransformFactory, 
									NULL, CLSCTX_INPROC,
									IID_IDXTransformFactory, 
									(void **) &pDXTransformFactory);
	assert(hr == DD_OK);
	hr = pDXTransformFactory->QueryInterface(IID_IDXSurfaceFactory, 
											(void **) &m_pDXSurfaceFactory);
	assert(hr == DD_OK);
	
	IDirectDraw3* pDDraw3 = NULL;
	hr = IDDraw()->QueryInterface(IID_IDirectDraw3, (void **)&pDDraw3);

	// UNDONE: Should we set the Services SID_SDirectDraw & SID_SDirect3DRM?

	//  The service ID for DirectDraw3 is the same as IID_IDirectDraw3
	pDXTransformFactory->SetService(IID_IDirectDraw3, pDDraw3, FALSE);

#endif VIS_USE_DXMEDIA
}

void *CVisGlobals::PfnIplCreateImageHeader(void)
{
	// Assume that this is the first Ipl function requested.
	// (We're only using Create and Deallocate at this time.)
	if (!m_fCheckedForIpl)
	{
		m_hModuleIpl = LoadLibrary("ipl.dll");
		m_fCheckedForIpl = true;

		if (m_hModuleIpl != 0)
		{
			m_pfnIplCreateImageHeader = (void *)
					GetProcAddress(m_hModuleIpl, "iplCreateImageHeader");
			m_pfnIplDeallocateHeader = (void *)
					GetProcAddress(m_hModuleIpl, "iplDeallocateHeader");

			// Fail if we can't find the functions we're looking for.
			if ((m_pfnIplCreateImageHeader == 0)
					|| (m_pfnIplDeallocateHeader == 0))
			{
				FreeLibrary(m_hModuleIpl);
				m_hModuleIpl = 0;
				m_pfnIplCreateImageHeader = 0;
				m_pfnIplDeallocateHeader = 0;
			}
		}
	}

	return m_pfnIplCreateImageHeader;
}

void *CVisGlobals::PfnIplDeallocateHeader(void)
{
	// Assume that we already looked for the IPL DLL
	// (in CVisGlobals::PfnIplCreateImageHeader).
	assert(m_fCheckedForIpl);
	return m_pfnIplDeallocateHeader;
}

unsigned __int64 CVisGlobals::FileMappingAppID(void)
{
	if(m_ulFileMappingAppID == 0)
	{
		((unsigned long *) &m_ulFileMappingAppID)[0] = GetCurrentProcessId();
		assert(m_ulFileMappingAppID);
		((unsigned long *) &m_ulFileMappingAppID)[1] = GetTickCount();
	}
	return m_ulFileMappingAppID;
}

int __cdecl CVisGlobals::OnExit(void)
{
	gvis_globals.Cleanup();
	return 0;
}

CVisGlobals gvis_globals;

// UNDONE: Should a friend function be defined which initializes the
// variables with default values?

VisPropListExportDeclspec CVisGlobals *PGlobals(void)
{
	return &gvis_globals;
}

