// VisGlobals.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

extern const char* gvis_szVisDefaultLogFileName;
const DWORD MAX_IMAGE_SIZE			= 128 * 1024 * 1024; // 128 MB

inline bool CVisGlobals::IsAlphaWritten(void) const
{
	return m_fIsAlphaWritten;
}

inline void CVisGlobals::SetIsAlphaWritten(bool fAlpha)
{
	m_fIsAlphaWritten = fAlpha;
}

inline bool CVisGlobals::DoesBMPFileWidthIncludePadding(void) const
{
	return m_fDoesBMPFileWidthIncludePadding;
}

inline void CVisGlobals::SetDoesBMPFileWidthIncludePadding(bool f)
{
	m_fDoesBMPFileWidthIncludePadding = f;
}

inline int CVisGlobals::ImOptsDefault(void) const
{
	return m_imoptsDefault;
}

inline void CVisGlobals::SetImOptsDefault(int wOptions)
{
	m_imoptsDefault = wOptions;
}

inline void CVisGlobals::InitializeWithDefault(void)
{
	m_fIsAlphaWritten = true;
	m_fDoesBMPFileWidthIncludePadding = true;
	m_fDiagnosticsEnabled = true;
	m_fShowConsoleWnd = false;
	m_fUseNamedFileMappings = true;
	m_dwImageSizeMax = MAX_IMAGE_SIZE;
}

inline bool CVisGlobals::DiagnosticsCallbackAvailable(void)
{
	return (m_listUserFunctions != NULL);
}

inline std::vector<SVisDiagnosticsCallbackInfo>* CVisGlobals::DiagCallbackList(void)
{
	if(m_listUserFunctions == NULL)
	{
		m_listUserFunctions = new std::vector<SVisDiagnosticsCallbackInfo >;
	}
	return m_listUserFunctions;
}

inline void CVisGlobals::SetLogFile(FILE* pLogFile)
{
	m_pLogFile = pLogFile;
}

inline FILE* CVisGlobals::LogFile(void) const
{
	return m_pLogFile;
}


inline void CVisGlobals::SetWarningDebugStringLevel(DWORD dwLevel)
{
	m_dwWarningDebugStringLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::WarningDebugStringLevel(void) const
{
	return m_dwWarningDebugStringLevel;
}

inline void CVisGlobals::SetWarningConsoleLevel(DWORD dwLevel)
{
	m_dwWarningConsoleLevel = dwLevel;
	m_fLevelsModified = true;
}
		
inline DWORD  CVisGlobals::WarningConsoleLevel(void) const
{
	return m_dwWarningConsoleLevel;
}

inline void CVisGlobals::SetWarningLogLevel(DWORD dwLevel)
{
	m_dwWarningLogLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::WarningLogLevel(void) const
{
	return m_dwWarningLogLevel;
}

inline void CVisGlobals::SetErrorDebugStringLevel(DWORD dwLevel)
{
	m_dwErrorDebugStringLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ErrorDebugStringLevel(void) const
{
	return m_dwErrorDebugStringLevel;
}

inline void CVisGlobals::SetErrorConsoleLevel(DWORD dwLevel)
{
	m_dwErrorConsoleLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ErrorConsoleLevel(void) const
{
	return m_dwErrorConsoleLevel;
}

inline void CVisGlobals::SetErrorLogLevel(DWORD dwLevel)
{
	m_dwErrorLogLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ErrorLogLevel(void) const
{
	return m_dwErrorLogLevel;
}

inline void CVisGlobals::SetTraceDebugStringLevel(DWORD dwLevel)
{
	m_dwTraceDebugStringLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::TraceDebugStringLevel(void) const
{
	return m_dwTraceDebugStringLevel;
}

inline void CVisGlobals::SetTraceConsoleLevel(DWORD dwLevel)
{
	m_dwTraceConsoleLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::TraceConsoleLevel(void) const
{
	return m_dwTraceConsoleLevel;
}

inline void CVisGlobals::SetTraceLogLevel(DWORD dwLevel)
{
	m_dwTraceLogLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::TraceLogLevel(void) const
{
	return m_dwTraceLogLevel;
}

inline void CVisGlobals::SetProgressDebugStringLevel(DWORD dwLevel)
{
	m_dwProgressDebugStringLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ProgressDebugStringLevel(void) const
{
	return m_dwProgressDebugStringLevel;
}

inline void CVisGlobals::SetProgressConsoleLevel(DWORD dwLevel)
{
	m_dwProgressConsoleLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ProgressConsoleLevel(void) const
{
	return m_dwProgressConsoleLevel;
}

inline void CVisGlobals::SetProgressLogLevel(DWORD dwLevel)
{
	m_dwProgressLogLevel = dwLevel;
	m_fLevelsModified = true;
}

inline DWORD  CVisGlobals::ProgressLogLevel(void) const
{
	return m_dwProgressLogLevel;
}

inline void CVisGlobals::EnableDiagnostics(void)
{
	m_fDiagnosticsEnabled = true;
}

inline void CVisGlobals::DisableDiagnostics(void)
{
	m_fDiagnosticsEnabled = false;
}

inline bool CVisGlobals::DiagnosticsEnabled(void) const
{
	return m_fDiagnosticsEnabled;
}

inline void CVisGlobals::SetDiagLevelsModified(bool fLevelsModified)
{
	m_fLevelsModified = fLevelsModified;
}

inline bool CVisGlobals::DiagLevelsModified(void) const
{
	return m_fLevelsModified;
}

inline void CVisGlobals::EnableConsoleWnd(void)
{
	m_fShowConsoleWnd = true;
}

inline void CVisGlobals::DisableConsoleWnd(void)
{
	m_fShowConsoleWnd = false;
}

inline bool CVisGlobals::ShowConsoleWnd(void)
{
	return m_fShowConsoleWnd;
}

inline void  CVisGlobals::SetConsoleWindow(CVisConsoleWnd* pConsoleWnd)
{
	m_pvisconsoleDiagnostics = pConsoleWnd;
}

inline CVisConsoleWnd* CVisGlobals::ConsoleWindow(void)
{
	return m_pvisconsoleDiagnostics;
}

inline void CVisGlobals::SetLogFileName(const char* szLogFileName)
{
	// LATER:  Do we want to allow the log file name to be changed when
	// the log file is open?  If so, should we close the current log file
	// and open a new log file?
	if (m_szLogFileName != 0)
		delete[] m_szLogFileName;
	
	if ((szLogFileName == 0) || (*szLogFileName == 0))
		szLogFileName = gvis_szVisDefaultLogFileName;

	m_szLogFileName = new char[strlen(szLogFileName) + 1];
	strcpy(m_szLogFileName, szLogFileName);
}

inline char* CVisGlobals::LogFileName(void) const
{
	return m_szLogFileName;
}

inline void CVisGlobals::SetPXCLAPACK(class CVisXCLAPACK *pvisXCLAPACK)
{
	m_pvisXCLAPACK = pvisXCLAPACK;
}

inline class CVisXCLAPACK* CVisGlobals::PXCLAPACK(void)
{
	return m_pvisXCLAPACK;
}

inline void CVisGlobals::SetPXCMINPACK(class CVisXMINPACK *pvisXCMINPACK)
{
	m_pvisXMINPACK = pvisXCMINPACK;
}

inline class CVisXMINPACK* CVisGlobals::PXCMINPACK(void)
{
	return m_pvisXMINPACK;
}

inline bool CVisGlobals::CoInitialized(void) const
{
	return m_fCOMInitialized;
}

inline void CVisGlobals::CoInitialize(void)
{
	if(!m_fCOMInitialized)
	{
		::CoInitialize(NULL);
		m_fCOMInitialized = true;
	}
}

inline bool CVisGlobals::UseNamedFileMappings(void) const
{
	return m_fUseNamedFileMappings;
}

inline void CVisGlobals::SetUseNamedFileMappings(bool f)
{
	m_fUseNamedFileMappings = f;
}

inline long CVisGlobals::NextFileMappingID(void)
{
	return InterlockedIncrement(&m_cFileMappingTotal);
}

inline DWORD CVisGlobals::CbImageSizeLimit(void) const
{
	return m_dwImageSizeMax;
}

inline void CVisGlobals::SetMaxImageSize(DWORD dwImageLim)
{
	m_dwImageSizeMax = dwImageLim;
}

