// VisGlobals.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#pragma once

enum EVisDiagnostics
{
	evisdiagnosticsWarning			= 0x0001,
	evisdiagnosticsError			= 0x0002,
	evisdiagnosticsTrace			= 0x0004,
	evisdiagnosticsProgress			= 0x0008
};

enum EVisDiagDescription
{
	evisdiagdescriptionNil				= (1 << 31),
	evisdiagdescriptionTextOnly         = 0x0000,
	evisdiagdescriptionFileAndLine      = 0x0001,
	evisdiagdescriptionTypeAndLevel     = 0x0002,
	evisdiagdescriptionAll				= 0x0003
};

enum EVisDiagCallback
{
	evisdiagcallbackDefault             = 0x0000,
	evisdiagcallbackProgress            = 0x0001
};


typedef void (VISAPI *VisPfnDiagnosticsCallback)(void *pvUser,
												  EVisDiagnostics evisdiagnosticsType,
												  DWORD dwLevel,
												  const char* szDescription);

typedef void (VISAPI *VisPfnDiagnosticsProgressCallback)(void *pvUser,
												  EVisDiagnostics evisdiagnosticsType,
												  DWORD dwLevel, int nCur, int nLim,
												  const char* szDescription);

typedef HRESULT (STDAPICALLTYPE *VisPfnGetImSrcProvider)(LPUNKNOWN pUnkOuter, 
														 const char *szProvider, REFIID riid,
														 void **ppvObject);


struct SVisDiagnosticsCallbackInfo
{
	int m_nDiagnosticsType;
	DWORD m_dwLevel;
	
	EVisDiagDescription m_enumdescriptionFlags;

	void *m_pfnCallback;
	void *m_pvUser;

	EVisDiagCallback m_enumcallbackType;
};

// Forward declaration
class CVisConsoleWnd;
class CVisFileHandler;


class VisPropListExport CVisGlobals
{
public:

	CVisGlobals(void);

	~CVisGlobals(void);

	void InitializeWithDefault(void);

	void Cleanup(void);
	

	bool IsAlphaWritten(void) const;
	void SetIsAlphaWritten(bool fAlpha);

	bool DoesBMPFileWidthIncludePadding(void) const;
	void SetDoesBMPFileWidthIncludePadding(bool f);

	int ImOptsDefault(void) const;
	void SetImOptsDefault(int wOptions);

	const unsigned long* ColorMapGray4bit(void);
	
	const unsigned long* ColorMapDefault4bit(void);
	
	const unsigned long* ColorMapGray8bit(void);

	const char* AppName(void);


	DWORD ProcessId(void);

	bool DiagnosticsCallbackAvailable(void);
	
	std::vector<SVisDiagnosticsCallbackInfo>* DiagCallbackList(void);

	void SetLogFile(FILE* pLogFile);
	FILE *LogFile(void) const;

	void SetLogFileName(const char* szLogFileName);
	char* LogFileName(void) const;

	void SetWarningDebugStringLevel(DWORD dwLevel);
	DWORD  WarningDebugStringLevel(void) const;

	void SetWarningConsoleLevel(DWORD dwLevel);
	DWORD  WarningConsoleLevel(void) const;

	void SetWarningLogLevel(DWORD dwLevel);
	DWORD  WarningLogLevel(void) const;
	

	void SetErrorDebugStringLevel(DWORD dwLevel);
	DWORD  ErrorDebugStringLevel(void) const;

	void SetErrorConsoleLevel(DWORD dwLevel);
	DWORD  ErrorConsoleLevel(void) const;

	void SetErrorLogLevel(DWORD dwLevel);
	DWORD  ErrorLogLevel(void) const;


	void SetTraceDebugStringLevel(DWORD dwLevel);
	DWORD  TraceDebugStringLevel(void) const;

	void SetTraceConsoleLevel(DWORD dwLevel);
	DWORD  TraceConsoleLevel(void) const;

	void SetTraceLogLevel(DWORD dwLevel);
	DWORD  TraceLogLevel(void) const;


	void SetProgressDebugStringLevel(DWORD dwLevel);
	DWORD  ProgressDebugStringLevel(void) const;

	void SetProgressConsoleLevel(DWORD dwLevel);
	DWORD  ProgressConsoleLevel(void) const;

	void SetProgressLogLevel(DWORD dwLevel);
	DWORD  ProgressLogLevel(void) const;


	void SetDiagLevelsModified(bool fLevelsModified);

	bool DiagLevelsModified(void) const;

	void EnableDiagnostics(void);
	void DisableDiagnostics(void);

	bool DiagnosticsEnabled(void) const;


	void EnableConsoleWnd(void);

	void DisableConsoleWnd(void);

	bool ShowConsoleWnd(void);

	void SetConsoleWindow(CVisConsoleWnd* pConsoleWnd);

	CVisConsoleWnd* ConsoleWindow(void);	

	void AddImSrcProvider(const char* szID, VisPfnGetImSrcProvider pfnCallback,
						  const char* szName);

	const char* ImSrcProviderID(int index) const;

	const VisPfnGetImSrcProvider ImSrcProviderCallback(int index) const;

	const char* ImSrcProviderName(int index) const;

	int IGetProvider(const char* szID) const;

	int NImSrcProviders(void) const;

	bool RemoveImSrcProvider(const VisPfnGetImSrcProvider pfnCallback, 
							 const char* szName = 0);

	bool RemoveImSrcProvider(const char* szName);

	bool RemoveImSrcProvider(const char* szID, const char* szName);


	bool RemoveImSrcProvider(const char* szID, 
							 const VisPfnGetImSrcProvider pfnCallback,
							 const char* szName = 0);

	void SetPXCLAPACK(class CVisXCLAPACK *pvisXCLAPACK);

	class CVisXCLAPACK* PXCLAPACK(void);

	void SetPXCMINPACK(class CVisXMINPACK *pvisXCMINPACK);

	class CVisXMINPACK* PXCMINPACK(void);

	static int __cdecl OnExit(void);

	CVisPropTypeInfoMap* PropTypeInfoMap(void);

	CVisPropTypeInfoMap& RPropTypeInfoMap(void);

	void InitFileHandlers(void);

	void ClearFileHandlers(void);

	void AddInternalFileHandler(CVisFileHandler *pfilehandler);

	void AddExternalFileHandler(CVisFileHandler *pfilehandler);

	CVisFileHandler *FileHandler(const char *szFiletype);

	CVisFileHandler *PictureFileHandler(void);

	void ReleasePictureFileHandler(void);
	
	interface IDirectDraw* IDDraw(void);

	interface IDXSurfaceFactory* IDXSFactory(void);

	unsigned __int64 FileMappingAppID(void);

	bool UseNamedFileMappings(void) const;

	void SetUseNamedFileMappings(bool f);

	long NextFileMappingID(void);

	DWORD CbImageSizeLimit(void) const;

	void SetMaxImageSize(DWORD dwImageLim);

	void *PfnIplCreateImageHeader(void);

	void *PfnIplDeallocateHeader(void);

protected:

	void ClearProvider(int index);

	bool DDrawInitialized(void);

	bool DXSFactoryInitialized(void);

	void InitializeDDraw(void);

	void InitializeDXSFactory(void);

	bool CoInitialized(void) const;

	void CoInitialize(void);


private:

	struct SVisProviderCallbackInfo
	{
		VisPfnGetImSrcProvider m_pfnCallback;
		char m_szName[256];
		char m_szID[50];
	};

	enum { knVisAlphaDefault = 0xff000000 };

	// Is alpha information written to files?
	bool m_fIsAlphaWritten;

	// When writing BMP files for images whose row width is not a multiple
	// of 4 bytes, should the width in the BMP file header be increased to
	// account for the padding added when rows are written to the file.
	bool m_fDoesBMPFileWidthIncludePadding;

	// Default image options (EVisImOpt values)
	int m_imoptsDefault;

	// @cmember:(IMPL)
	// The default colormap for 4-bit grayscale images.
	// @comm:(IMPL) This may change in the final release of the SDK.
	unsigned long* m_abulNibbleColorMapGray;

	// @cmember:(IMPL)
	// The default colormap for 4-bit color images.
	// @comm:(IMPL) This may change in the final release of the SDK.
	unsigned long *m_abulNibbleColorMapDefault;

	// @cmember:(IMPL)
	// The default colormap for 8-bit grayscale images.
	// @comm:(IMPL) This may change in the final release of the SDK.
	unsigned long *m_abulByteColorMapGray;
	
	char* m_szAppName;

	DWORD   m_dwProcessId;

	DWORD	m_dwWarningDebugStringLevel;
	DWORD	m_dwWarningConsoleLevel;
	DWORD   m_dwWarningLogLevel;

	DWORD	m_dwErrorDebugStringLevel;
	DWORD	m_dwErrorConsoleLevel;
	DWORD   m_dwErrorLogLevel;

	DWORD	m_dwTraceDebugStringLevel;
	DWORD	m_dwTraceConsoleLevel;
	DWORD   m_dwTraceLogLevel;
	
	DWORD	m_dwProgressDebugStringLevel;
	DWORD	m_dwProgressConsoleLevel;
	DWORD   m_dwProgressLogLevel;

	bool m_fLevelsModified;

	// CVisDiagnosticMsg functions check this flag before proceeding.
	bool m_fDiagnosticsEnabled;

	bool m_fShowConsoleWnd;

	FILE*   m_pLogFile; 

	char*   m_szLogFileName;

	std::vector<SVisDiagnosticsCallbackInfo > *m_listUserFunctions;

	CVisConsoleWnd*		m_pvisconsoleDiagnostics;

	SVisProviderCallbackInfo  m_ProviderList[8];

	int						m_nProvidersAvailable;

	class CVisXCLAPACK		*m_pvisXCLAPACK;

	class CVisXMINPACK		*m_pvisXMINPACK;

	CVisPropTypeInfoMap		*m_pvisproptypeInfomap;

	CVisFileHandler 		*m_pfilehandlersInternal[10];

	int						m_nInternalFileHandlers;

	CVisFileHandler 		*m_pfilehandlersExternal[20];

	int						m_nExternalFileHandlers;

    CVisFileHandler			*m_pPictureFileHandler;
	
	interface IDirectDraw			*m_pDD;

	interface IDXSurfaceFactory		*m_pDXSurfaceFactory;

	bool					m_fCOMInitialized;

	unsigned __int64		m_ulFileMappingAppID;

	bool					m_fUseNamedFileMappings;

	long					m_cFileMappingTotal;

	DWORD					m_dwImageSizeMax;

	
	HMODULE m_hModuleIpl;
	bool m_fCheckedForIpl;
	void *m_pfnIplCreateImageHeader;
	void *m_pfnIplDeallocateHeader;
}; // class CVisGlobals


VisPropListExportDeclspec CVisGlobals *PGlobals(void);

#include "VisGlobals.inl"
