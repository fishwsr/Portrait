// VisDiagnosticsManager.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


inline void CVisDiagnosticsManager::SetWarningDebugStringLevel(DWORD dwLevel)
{
	PGlobals()->SetWarningDebugStringLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::WarningDebugStringLevel(void)
{
	return PGlobals()->WarningDebugStringLevel();
}

inline void CVisDiagnosticsManager::SetWarningConsoleLevel(DWORD dwLevel)
{
	PGlobals()->SetWarningConsoleLevel(dwLevel);
}
		
inline DWORD  CVisDiagnosticsManager::WarningConsoleLevel(void)
{
	return PGlobals()->WarningConsoleLevel();
}

inline void CVisDiagnosticsManager::SetWarningLogLevel(DWORD dwLevel)
{
	PGlobals()->SetWarningLogLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::WarningLogLevel(void)
{
	return PGlobals()->WarningLogLevel();
}

inline void CVisDiagnosticsManager::SetErrorDebugStringLevel(DWORD dwLevel)
{
	PGlobals()->SetErrorDebugStringLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ErrorDebugStringLevel(void)
{
	return PGlobals()->ErrorDebugStringLevel();
}

inline void CVisDiagnosticsManager::SetErrorConsoleLevel(DWORD dwLevel)
{
	PGlobals()->SetErrorConsoleLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ErrorConsoleLevel(void)
{
	return PGlobals()->ErrorConsoleLevel();
}

inline void CVisDiagnosticsManager::SetErrorLogLevel(DWORD dwLevel)
{
	PGlobals()->SetErrorLogLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ErrorLogLevel(void)
{
	return PGlobals()->ErrorLogLevel();
}

inline void CVisDiagnosticsManager::SetTraceDebugStringLevel(DWORD dwLevel)
{
	PGlobals()->SetTraceDebugStringLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::TraceDebugStringLevel(void)
{
	return PGlobals()->TraceDebugStringLevel();
}

inline void CVisDiagnosticsManager::SetTraceConsoleLevel(DWORD dwLevel)
{
	PGlobals()->SetTraceConsoleLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::TraceConsoleLevel(void)
{
	return PGlobals()->TraceConsoleLevel();
}

inline void CVisDiagnosticsManager::SetTraceLogLevel(DWORD dwLevel)
{
	PGlobals()->SetTraceLogLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::TraceLogLevel(void)
{
	return PGlobals()->TraceLogLevel();
}

inline void CVisDiagnosticsManager::SetProgressDebugStringLevel(DWORD dwLevel)
{
	PGlobals()->SetProgressDebugStringLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ProgressDebugStringLevel(void)
{
	return PGlobals()->ProgressDebugStringLevel();
}

inline void CVisDiagnosticsManager::SetProgressConsoleLevel(DWORD dwLevel)
{
	PGlobals()->SetProgressConsoleLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ProgressConsoleLevel(void)
{
	return PGlobals()->ProgressConsoleLevel();
}

inline void CVisDiagnosticsManager::SetProgressLogLevel(DWORD dwLevel)
{
	PGlobals()->SetProgressLogLevel(dwLevel);
}

inline DWORD  CVisDiagnosticsManager::ProgressLogLevel(void)
{
	return PGlobals()->ProgressLogLevel();
}

inline void CVisDiagnosticsManager::EnableDiagnostics(void)
{
	PGlobals()->EnableDiagnostics();
}

inline void CVisDiagnosticsManager::DisableDiagnostics(void)
{
	PGlobals()->DisableDiagnostics();
}

inline bool CVisDiagnosticsManager::DiagnosticsEnabled(void)
{
	return PGlobals()->DiagnosticsEnabled();
}

inline void CVisDiagnosticsManager::EnableConsoleWnd(void)
{
	PGlobals()->EnableConsoleWnd();
}

inline void CVisDiagnosticsManager::DisableConsoleWnd(void)
{
	PGlobals()->DisableConsoleWnd();
}

inline bool CVisDiagnosticsManager::ShowConsoleWnd(void)
{
	return PGlobals()->ShowConsoleWnd();
}


inline void CVisDiagnosticsManager::SetLevelsModified(bool fLevelsModified)
{
	PGlobals()->SetDiagLevelsModified(fLevelsModified);
}

inline bool CVisDiagnosticsManager::LevelsModified(void)
{
	return PGlobals()->DiagLevelsModified();
}
