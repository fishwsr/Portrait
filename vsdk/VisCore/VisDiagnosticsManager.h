// VisDiagnosticsManager.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#pragma once


// Forward Declarations
class CVisString;

class VisPropListExport CVisDiagnosticsManager
{
public:
	CVisDiagnosticsManager(void);

	~CVisDiagnosticsManager(void);

	static void SetWarningDebugStringLevel(DWORD dwLevel);
	static DWORD  WarningDebugStringLevel(void);

	static void SetWarningConsoleLevel(DWORD dwLevel);
	static DWORD  WarningConsoleLevel(void);

	static void SetWarningLogLevel(DWORD dwLevel);
	static DWORD  WarningLogLevel(void);

	static void SetErrorDebugStringLevel(DWORD dwLevel);
	static DWORD  ErrorDebugStringLevel(void);

	static void SetErrorConsoleLevel(DWORD dwLevel);
	static DWORD  ErrorConsoleLevel(void);

	static void SetErrorLogLevel(DWORD dwLevel);
	static DWORD  ErrorLogLevel(void);


	static void SetTraceDebugStringLevel(DWORD dwLevel);
	static DWORD  TraceDebugStringLevel(void);

	static void SetTraceConsoleLevel(DWORD dwLevel);
	static DWORD  TraceConsoleLevel(void);

	static void SetTraceLogLevel(DWORD dwLevel);
	static DWORD  TraceLogLevel(void);

	static void SetProgressDebugStringLevel(DWORD dwLevel);
	static DWORD  ProgressDebugStringLevel(void);

	static void SetProgressConsoleLevel(DWORD dwLevel);
	static DWORD  ProgressConsoleLevel(void);

	static void SetProgressLogLevel(DWORD dwLevel);
	static DWORD  ProgressLogLevel(void);

	static void SetLogFile(FILE* pLogFile);
	static FILE *LogFile(void);

	static void SetLogFileName(const char* szLogFileName);

	static char* LogFileName(void);

	static void EnableDiagnostics(void);
	static void DisableDiagnostics(void);

	static bool DiagnosticsEnabled(void);

	static void EnableConsoleWnd(void);

	static void DisableConsoleWnd(void);

	static bool ShowConsoleWnd(void);

	static void  SetConsoleWindow(CVisConsoleWnd* pConsoleWnd);

	static CVisConsoleWnd* ConsoleWindow(void);


	static void SetCallbackFunction(int nDiagnosticsType, DWORD dwLevel,
							VisPfnDiagnosticsCallback pfnCallback, void* pvUser = 0,
							 EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetCallbackFunction(int nDiagnosticsType, VisPfnDiagnosticsCallback pfnCallback, 
							 void* pvUser = 0,
							 EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);

	static void SetCallbackFunction(int nDiagnosticsType, DWORD dwLevel,
							VisPfnDiagnosticsProgressCallback pfnCallback, void* pvUser = 0,
							 EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetCallbackFunction(int nDiagnosticsType, VisPfnDiagnosticsProgressCallback pfnCallback, 
							 void* pvUser = 0,
							 EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);

	static void SetWarningFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnWarning, 
							void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetWarningFunction(VisPfnDiagnosticsCallback pfnWarning, void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);


	static void SetErrorFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnError, 
							void* pvUser,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetErrorFunction(VisPfnDiagnosticsCallback pfnError, void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);


	static void SetTraceFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnTrace, 
							void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetTraceFunction(VisPfnDiagnosticsCallback pfnTrace, void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);


	static void SetProgressReportFunction(DWORD dwLevel, 
							VisPfnDiagnosticsCallback pfnProgressReport, 
							void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetProgressReportFunction(VisPfnDiagnosticsCallback pfnProgressReport, 
									void* pvUser = 0,
									EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);

	static void SetProgressReportFunction(DWORD dwLevel, 
							VisPfnDiagnosticsProgressCallback pfnProgressReport, 
							void* pvUser = 0,
							EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);
	
	static void SetProgressReportFunction(VisPfnDiagnosticsProgressCallback pfnProgressReport, 
									void* pvUser = 0,
									EVisDiagDescription evisdiagdescriptionFlags = evisdiagdescriptionAll);

	static void RemoveCallback(const void* pfnCallback, const void* pvUser = 0);

	static void RemoveCallback(int nDiagnosticsType, const void* pfnCallback, 
						const void* pvUser = 0);

	static void RemoveWarningCallback(const void* pfnCallback, const void* pvUser = 0);

	static void RemoveErrorCallback(const void* pfnCallback, const void* pvUser = 0);

	static void RemoveTraceCallback(const void* pfnCallback, const void* pvUser = 0);

	static void RemoveProgressCallback(const void* pfnCallback, const void* pvUser = 0);

	static void DisplayConsoleWindow(void);

	static void HideConsoleWindow(void);

protected:
	
	friend class CVisDiagnosticMsg;

	static void SetLevelsModified(bool fLevelsModified);

	static bool LevelsModified(void);
	
	static bool CallbackAvailable(SVisDiagnosticsCallbackInfo& callbackSelect);
	
	static const char* DescriptionText(EVisDiagnostics evisdiagnosticsType);

	static void FormatDiagnosticMessage(char* szDescription, const char* szFileLine, 
										 const char* szTypeLevel, const char* szMessage, 
										 EVisDiagDescription evisDescriptionFlags);

	static void DoMessage(EVisDiagnostics evisdiagnosticsType, DWORD dwLevel, 
					const char* szFile, int iLine,
					const char* szMessage, int nCur = 0, int nLim = 0);
	
	static void TriggerUserFunction(char* szDescription, const char* szFileLine, 
									 const char* szTypeLevel, const char* szMessage, 
									 DWORD dwLevel, EVisDiagnostics evisdiagnosticsType,
									 EVisDiagDescription& evisDescriptionFlags,
									 int nCur, int nLim);

	static void DisplayToStandardOutput(char* szDescription, bool fFormatReady, 
										DWORD dwLevel, EVisDiagnostics evisdiagnosticsType,
										 const char* szFileLine, const char* szTypeLevel, 
										 const char* szMessage, const char* szProgress);

	
	static void OutputToDebugWindow(const char* szDescription);

	static void OutputToConsole(const char* szDescription);

	static void WriteToLogFile(const char* szDescription);


	static bool InitializeLevelsFromRegistry(void);

	static void WriteToRegistry(void);

	static void AddCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
						EVisDiagDescription evisdiagdescriptionFlags,
						VisPfnDiagnosticsProgressCallback pfnCallback, 
						void* pvUser);

	static void AddCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
						EVisDiagDescription evisdiagdescriptionFlags,
						VisPfnDiagnosticsCallback pfnCallback, 
						void* pvUser);
	static void InsertCallback(int nDiagnosticsType, DWORD dwLevel,
						EVisDiagDescription evisdiagdescriptionFlags,
						void* pfnCallback, EVisDiagCallback eviscallbackType,
						void* pvUser);


	static bool MergeCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
							EVisDiagDescription evisdiagdescriptionFlags,
							void* pfnCallback, void* pvUser);

	static void AddStringToConsole(const char* szDescription);

private:


}; // class CVisDiagnosticsManager

#include "VisDiagnosticsManager.inl"

